/*
 * CommandsFactory.cpp
 *
 *  Created on: Jul 12, 2016
 *  @author Corteggiani Nassim <nassim.corteggiani@maximintegrated.com>
 */

#include "CommandsFactory.h"

bool CommandsFactory::first_io = true;

using namespace std;

CommandsFactory::CommandsFactory() {}

CommandsFactory::~CommandsFactory() {}

/*
 *  The JTAG-DP register accessed depends on both:
   •       - The Instruction Register (IR) value for the DAP access
   •       - A[3:2] from the address field of the DAP acces
 *
 */
jtag::Command *CommandsFactory::CreateCommand(COMMAND_TYPE type,
                                              vector<uint32_t> &argv) {

  jtag::Command *cmd = new jtag::Command(type);

  switch (type) {

  case SELECT:

    if (CommandsFactory::check_arg(argv, 1) == true) {

      CommandsFactory::select(cmd, argv.at(0));

      CommandsFactory::first_io = true;
    } else
      ALERT("CommandsFactory", "SELECT args missing");

    break;

  case RESET:

    for (int i = 0; i < 5; i++)
      cmd->add_command(1, 0, 0, 0);

    CommandsFactory::first_io = true;

    break;

  case READ_U32:

    if (check_arg(argv, 1) == true) {
      CommandsFactory::read_u32(cmd, argv.at(0));
      CommandsFactory::first_io = false;
    } else
      ALERT("CommandsFactory", "READ_U32 args missing");

    break;

  case WRITE_U32:

    if (check_arg(argv, 2) == true) {
      CommandsFactory::write_u32(cmd, argv.at(1), argv.at(0));
      CommandsFactory::first_io = false;
    } else
      ALERT("CommandsFactory", "WRITE_U32 args missing");

    break;

  case IDCODE:
    CommandsFactory::idcode(cmd);
    CommandsFactory::first_io = true;
    break;
  case ACTIVE:
    CommandsFactory::active(cmd);
    CommandsFactory::first_io = true;
    break;
  case EXIT:
    break;
  case TRACE:
    CommandsFactory::first_io = false;
    CommandsFactory::trace(cmd);
    break;
  case UNTRACE:
    CommandsFactory::first_io = false;
    CommandsFactory::untrace(cmd);
    break;
  }

  argv.clear();

  return cmd;
}

bool CommandsFactory::check_arg(vector<uint32_t> &argv, uint32_t required) {

  if (argv.size() != required)
    return false;

  return true;
}

void CommandsFactory::trace(jtag::Command *cmd) {
  // 5 MHz

  /*
  * Data Watchpoint and Trace (DWT) provides profiling, PC and exception trace
  * support.
  * Exception tracing is enabled using the EXCTRCENA bit in DWT_CTRL register.
  *
  * Exception tracing is enabled using the EXCTRCENA bit in the DWT_CTRL
  * register.
  * When the bit is set the DWT emits an exception trace packet
  * under the following conditions:
  *  • exception entry (from Thread mode or pre-emption of thread or handler).
  *  • exception exit when exiting a handler with an EXC_RETURN vector.
  *      See Exception return behavior on page B1-25.
  *  • exception return when re-entering a pre-empted thread or handler code
  * sequence.
  */
  write_u32(cmd, 0xE000EDFC, 1<<24); // Enable Trace Mode Flag into Debug Exception and Monitor Control Register

  write_u32(cmd, 0xE0040010, 0); // DWT_CTRL regiter : EXCTRCENA

  write_u32(cmd, 0xE0001000, 1<<16); // DWT_CTRL regiter : EXCTRCENA

  write_u32(cmd, 0xE0000E80, 0x0001000d/*0xC*/); // Enable ITM TCR

  write_u32(cmd, 0xE0000E00, 0xffffffff); // Enable ITM TER

  /*TPIU*/
  write_u32(cmd, 0xE0040000, 0x4); // Set TPIU SSPSR

  write_u32(cmd, 0xE00400F0, 0x0); // Set TPIU SSPPR

  write_u32(cmd, 0xE0040FC8, 0x20); // Set TPIU TYPE

  write_u32(cmd, 0xE0040304, 0); //TPIU FFCR : formater
}

void CommandsFactory::untrace(jtag::Command *cmd) {}

void CommandsFactory::idcode(jtag::Command *cmd) {

  cmd->move_to(jtag::TAP_IRSHIFT);

  cmd->write_ir(0xE); // 1110 = IDCODE IR

  cmd->move_to(jtag::TAP_DRSHIFT);
  cmd->write_dr(DPAP_READ, 0, 0);

  cmd->move_to(jtag::TAP_IDLE);
}

void CommandsFactory::active(jtag::Command *cmd) {

  cmd->move_to(jtag::TAP_IRSHIFT);

  cmd->write_ir(DPACC);

  cmd->move_to(jtag::TAP_DRSHIFT);

  cmd->write_dr(DPAP_WRITE, CTRL_STAT, 0x50000000);

  for (int i = 0; i < 30; i++)
    cmd->add_command(0, 0, 0, 0);

  cmd->move_to(jtag::TAP_IDLE);
}

void CommandsFactory::select(jtag::Command *cmd, uint32_t bank_id) {

  uint32_t sel = (bank_id << 24);
  sel += (0x0 << 4);

  // Set the correct JTAG-DP
  cmd->move_to(jtag::TAP_IRSHIFT);
  cmd->write_ir(DPACC); // 1010 = DPACC IR

  // set select register value
  cmd->move_to(jtag::TAP_DRSHIFT);

  cmd->write_dr(DPAP_WRITE, SEL_ADDR, sel);

  cmd->move_to(jtag::TAP_IDLE);
}

void CommandsFactory::read_u32(jtag::Command *cmd, uint32_t address) {

  // uint32_t header, csw_value;

  // Set the correct JTAG-DP
  // if (CommandsFactory::first_io == true) {

    SUCCESS("CommandsFactory", "First IO here");

    cmd->move_to(jtag::TAP_IRSHIFT);
    cmd->write_ir(APACC); // 1011 = APACC IR

    // CSW register value
    // csw_value = CSW_32BIT | CSW_ADDRINC_OFF | CSW_DBGSWENABLE |
    // CSW_MASTER_DEBUG | CSW_HPROT;

    // set csw register value
    cmd->move_to(jtag::TAP_DRSHIFT);
    cmd->write_dr(DPAP_WRITE, CSW_ADDR, 0x23000042);
    cmd->move_to(jtag::TAP_IDLE);
    for (int i = 0; i < 15; i++)
      cmd->add_command(0, 0, 0, 0);
  // }

  // set tar register value
  cmd->move_to(jtag::TAP_DRSHIFT);
  cmd->write_dr(DPAP_WRITE, TAR_ADDR, address);
  cmd->move_to(jtag::TAP_IDLE);
  for (int i = 0; i < 15; i++)
    cmd->add_command(0, 0, 0, 0);

  // set drw register value
  cmd->move_to(jtag::TAP_DRSHIFT);
  cmd->write_dr(DPAP_READ, DRW_ADDR, 0);
  cmd->move_to(jtag::TAP_IDLE);
  for (int i = 0; i < 15; i++)
    cmd->add_command(0, 0, 0, 0);

  // set drw register value
  cmd->move_to(jtag::TAP_DRSHIFT);
  cmd->write_dr(DPAP_READ, DRW_ADDR, 0);
  cmd->move_to(jtag::TAP_IDLE);
  for (int i = 0; i < 15; i++)
    cmd->add_command(0, 0, 0, 0);
}

void CommandsFactory::write_u32(jtag::Command *cmd, uint32_t address,
                                uint32_t datain) {

  uint32_t csw_value;

  // if (CommandsFactory::first_io == true) {

    SUCCESS("CommandsFactory", "First IO here");

    // Set the correct JTAG-DP
    cmd->move_to(jtag::TAP_IRSHIFT);
    cmd->write_ir(APACC); // 1011 = APACC IR

    // CSW register value
    csw_value = CSW_32BIT | CSW_ADDRINC_OFF | CSW_HPROT | CSW_MASTER_DEBUG | CSW_DBGSWENABLE;
                                                                // 0x23000042;
    // CSW_32BIT | CSW_ADDRINC_OFF | CSW_DBGSWENABLE;
    // CSW_MASTER_DEBUG |
    // CSW_SPROT | CSW_ADDRINC_SINGLE | CSW_SPIDEN;
    // printf("CSW = 0x%08x\n\n", csw_value);

    // set csw register value
    cmd->move_to(jtag::TAP_DRSHIFT);
    cmd->write_dr(DPAP_WRITE, CSW_ADDR, csw_value);

    // cmd->move_to(jtag::TAP_DRSHIFT);
    // cmd->write_dr(DPAP_READ, CSW_ADDR, 0);
    // for (int i = 0; i < 30; i++)
    //   cmd->add_command(0, 0, 0, 0);
    cmd->move_to(jtag::TAP_IDLE);
    for (int i = 0; i < 15; i++)
      cmd->add_command(0, 0, 0, 0);
  // }

  // set tar register value
  cmd->move_to(jtag::TAP_DRSHIFT);
  cmd->write_dr(DPAP_WRITE, TAR_ADDR, address);
  cmd->move_to(jtag::TAP_IDLE);
  for (int i = 0; i < 15; i++)
    cmd->add_command(0, 0, 0, 0);

  // set DRW register value
  cmd->move_to(jtag::TAP_DRSHIFT);
  cmd->write_dr(DPAP_WRITE, DRW_ADDR, datain);
  cmd->move_to(jtag::TAP_IDLE);
  for (int i = 0; i < 15; i++)
    cmd->add_command(0, 0, 0, 0);

  // cmd->move_to(jtag::TAP_DRSHIFT);
  // cmd->write_dr(DPAP_READ, CSW_ADDR, 0);
  // cmd->move_to(jtag::TAP_IDLE);
  //
  // cmd->move_to(jtag::TAP_DRSHIFT);
  // cmd->write_dr(DPAP_READ, CSW_ADDR, 0);
  // cmd->move_to(jtag::TAP_IDLE);
}
