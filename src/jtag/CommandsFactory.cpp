/*
 * CommandsFactory.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
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

    if (check_arg(argv, 1) == true)
      CommandsFactory::read_u32(cmd, argv.at(0));
    else
      ALERT("CommandsFactory", "READ_U32 args missing");

    break;

  case WRITE_U32:

    if (check_arg(argv, 2) == true)
      CommandsFactory::write_u32(cmd, argv.at(1), argv.at(0));
    else
      ALERT("CommandsFactory", "WRITE_U32 args missing");

    break;

  case IDCODE:
    CommandsFactory::idcode(cmd);
    break;
  case ACTIVE:
    CommandsFactory::active(cmd);
    break;
  case EXIT:
    break;
  case TRACE:
    CommandsFactory::trace(cmd);
    break;
  case UNTRACE:
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

void CommandsFactory::idcode(jtag::Command *cmd) {

  cmd->move_to(jtag::TAP_IRSHIFT);

  cmd->write_ir(0xE); // 1110 = IDCODE IR

  cmd->move_to(jtag::TAP_DRSHIFT);
  cmd->write_dr(DPAP_READ, 0, 0);

  cmd->move_to(jtag::TAP_IDLE);

  CommandsFactory::first_io = true;
}

void CommandsFactory::active(jtag::Command *cmd) {

  cmd->move_to(jtag::TAP_IRSHIFT);

  cmd->write_ir(DPACC);

  cmd->move_to(jtag::TAP_DRSHIFT);

  cmd->write_dr(DPAP_WRITE, CTRL_STAT, 0x50000000);

  for (int i = 0; i < 30; i++)
    cmd->add_command(0, 0, 0, 0);

  cmd->move_to(jtag::TAP_IDLE);

  CommandsFactory::first_io = true;
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
  if (CommandsFactory::first_io == true) {

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

    CommandsFactory::first_io = false;
  }

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

  if (CommandsFactory::first_io == true) {

    // Set the correct JTAG-DP
    cmd->move_to(jtag::TAP_IRSHIFT);
    cmd->write_ir(APACC); // 1011 = APACC IR

    // CSW register value
    csw_value = CSW_32BIT | CSW_ADDRINC_OFF | CSW_MASTER_DEBUG; ///
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

    CommandsFactory::first_io = false;
  }

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
