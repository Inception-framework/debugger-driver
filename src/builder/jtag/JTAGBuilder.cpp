/*
 * JTAGBuilder.cpp
 *
 *  Created on: Jul 12, 2016
 *  @author Corteggiani Nassim <nassim.corteggiani@maximintegrated.com>
 */

#include "JTAGBuilder.h"

#include "../Jtag.h"
#include "../Command.h"

using namespace std;

namespace jtag {

JTAGBuilder::JTAGBuilder() : first_io(true) {}

JTAGBuilder::~JTAGBuilder() {}

jtag::Command* JTAGBuilder::reset() { return init(); }


jtag::Command* JTAGBuilder::init() {

  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::RESET);

  for (int i = 0; i < 5; i++)
    add_command(cmd, 1, 0, 0, 0);

  JTAGBuilder::active(cmd);

  JTAGBuilder::select(cmd, 0);

  return cmd;
}

/*
* Implementation of Commands from the CommandsBuilder interface
*/
jtag::Command* JTAGBuilder::write(uint32_t datain, uint32_t address) {

  INFO("JTAGBuilder","Write command at address 0x%08x", address);

  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::WRITE);

  if (first_io == true) {

      // Set the correct JTAG-DP
      move_to(cmd, jtag::TAP_IRSHIFT);
      write_ir(cmd, APACC); // 1011 = APACC IR

      // CSW register value
      // csw_value = CSW_32BIT | CSW_ADDRINC_OFF | CSW_MASTER_DEBUG| CSW_DBGSWENABLE | CSW_HPROT;

      // set csw register value
      move_to(cmd, jtag::TAP_DRSHIFT);
      write_dr(cmd, DPAP_WRITE, CSW_ADDR, CSW_32BIT | CSW_ADDRINC_OFF | CSW_MASTER_DEBUG | CSW_HPROT);
      move_to(cmd, jtag::TAP_IDLE);
      for (int i = 0; i < 20; i++)
        add_command(cmd, 0, 0, 0, 0);
  }

  // set tar register value
  move_to(cmd, jtag::TAP_DRSHIFT);
  write_dr(cmd, DPAP_WRITE, TAR_ADDR, address);
  move_to(cmd, jtag::TAP_IDLE);
  for (int i = 0; i < 20; i++)
    add_command(cmd, 0, 0, 0, 0);

  // set DRW register value
  move_to(cmd, jtag::TAP_DRSHIFT);
  write_dr(cmd, DPAP_WRITE, DRW_ADDR, datain);
  move_to(cmd, jtag::TAP_IDLE);
  for (int i = 0; i < 20; i++)
    add_command(cmd, 0, 0, 0, 0);

  return cmd;
}

jtag::Command* JTAGBuilder::read(uint32_t address) {

  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::READ);

  // uint32_t header, csw_value;

  // Set the correct JTAG-DP
  if (first_io == true) {

    // SUCCESS("JTAGBuilder", "First IO here");

    move_to(cmd, jtag::TAP_IRSHIFT);
    write_ir(cmd, APACC); // 1011 = APACC IR

    // set csw register value
    move_to(cmd, jtag::TAP_DRSHIFT);
    write_dr(cmd, DPAP_WRITE, CSW_ADDR, CSW_32BIT | CSW_ADDRINC_OFF | CSW_MASTER_DEBUG | CSW_HPROT);
    move_to(cmd, jtag::TAP_IDLE);
    for (int i = 0; i < 20; i++)
      add_command(cmd, 0, 0, 0, 0);
  }

  // set tar register value
  move_to(cmd, jtag::TAP_DRSHIFT);
  write_dr(cmd, DPAP_WRITE, TAR_ADDR, address);
  move_to(cmd, jtag::TAP_IDLE);
  for (int i = 0; i < 20; i++)
    add_command(cmd, 0, 0, 0, 0);

  // set drw register value
  move_to(cmd, jtag::TAP_DRSHIFT);
  write_dr(cmd, DPAP_READ, DRW_ADDR, 0);
  move_to(cmd, jtag::TAP_IDLE);
  for (int i = 0; i < 20; i++)
    add_command(cmd, 0, 0, 0, 0);

  // set drw register value
  move_to(cmd, jtag::TAP_DRSHIFT);
  write_dr(cmd, DPAP_READ, DRW_ADDR, 0);
  move_to(cmd, jtag::TAP_IDLE);
  for (int i = 0; i < 20; i++)
    add_command(cmd, 0, 0, 0, 0);

  return cmd;
}

jtag::Command* JTAGBuilder::idcode() {

  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::IDCODE);

  move_to(cmd, jtag::TAP_IRSHIFT);

  write_ir(cmd, 0xE); // 1110 = IDCODE IR

  move_to(cmd, jtag::TAP_DRSHIFT);
  write_dr(cmd, DPAP_READ, 0, 0);

  move_to(cmd, jtag::TAP_IDLE);

  return cmd;
}

void JTAGBuilder::active(jtag::Command *cmd) {

  move_to(cmd, jtag::TAP_IRSHIFT);

  write_ir(cmd, DPACC);

  move_to(cmd, jtag::TAP_DRSHIFT);

  write_dr(cmd, DPAP_WRITE, CTRL_STAT, 0x50000000);

  for (int i = 0; i < 30; i++)
    add_command(cmd, 0, 0, 0, 0);

  move_to(cmd, jtag::TAP_IDLE);
}

void JTAGBuilder::select(jtag::Command *cmd, uint32_t bank_id) {

  uint32_t sel = (bank_id << 24);
  sel += (0x0 << 4);

  // Set the correct JTAG-DP
  move_to(cmd, jtag::TAP_IRSHIFT);
  write_ir(cmd, DPACC); // 1010 = DPACC IR

  // set select register value
  move_to(cmd, jtag::TAP_DRSHIFT);

  write_dr(cmd, DPAP_WRITE, SEL_ADDR, sel);

  move_to(cmd, jtag::TAP_IDLE);
}

void JTAGBuilder::add_command(jtag::Command *cmd, uint32_t tms, uint32_t tdi, uint32_t trst,
                          uint32_t srst) {

  uint8_t byte = 0;

  byte += (tms << 0);
  byte += (tdi << 1);
  byte += (trst << 2);
  byte += (srst << 3);

  cmd->push_back(byte);
}

void JTAGBuilder::move_to(jtag::Command *cmd, tap_state_t state) {

  int i;
  uint8_t byte;

  // printf("From %s to %s ", Jtag::tap_state_name(Jtag::current_state),
  //        Jtag::tap_state_name(state));

  uint8_t tms_scan = Jtag::tap_get_tms_path(Jtag::current_state, state);

  uint8_t tms_scan_bits =
      Jtag::tap_get_tms_path_len(Jtag::current_state, state);

  for (i = 0; i < tms_scan_bits; i++) {

    byte = tms_scan & (1u << i) ? (1 << 0) : 0;
    cmd->push_back(byte);

    // printf("%01x ", byte);
  }
  // printf("\n\n");

  Jtag::current_state = state;
}

void JTAGBuilder::write_ir(jtag::Command *cmd, uint8_t ir) {

  uint32_t j;
  uint8_t byte;

  for (j = 0; j < 4; j++) {

    byte = ir & (1u << j) ? (1 << 1) : 0;

    if (j == 3)
      byte += (1 << 0); // set tms

    cmd->push_back(byte);
  }

  cmd->push_back(0);

  Jtag::current_state = TAP_IRPAUSE;
}

void JTAGBuilder::wait(jtag::Command *cmd, uint32_t cycles) {

  switch (Jtag::current_state) {
  case TAP_DREXIT2:
  case TAP_DREXIT1:
  case TAP_DRSHIFT:
  case TAP_DRPAUSE:
  case TAP_DRUPDATE:
  case TAP_DRCAPTURE:
  case TAP_DRSELECT:
    move_to(cmd, TAP_DRPAUSE);
    break;

  case TAP_IRSELECT:
  case TAP_IREXIT2:
  case TAP_IREXIT1:
  case TAP_IRSHIFT:
  case TAP_IRPAUSE:
  case TAP_IRUPDATE:
  case TAP_IRCAPTURE:
    move_to(cmd, TAP_IRPAUSE);
    break;

  default:
    return;
  }

  for (unsigned int i = 0; i < cycles; i++)
    cmd->push_back(0);
}

void JTAGBuilder::write_dr(jtag::Command *cmd, uint8_t RnW, uint8_t address, uint32_t datain) {

  uint32_t j;
  uint8_t byte;
  uint32_t pos = cmd->size();

  byte = ((address >> 1) & 0x6) | (RnW & 0x1);

  for (j = 0; j < 3; j++)
    cmd->push_back(byte & (1u << j) ? (1 << 1) : 0);

  for (j = 0; j < 31; j++)
    cmd->push_back(datain & (1u << j) ? (1 << 1) : 0);

  cmd->push_back(datain & (1u << 31) ? (1 << 1) | (1 << 0)
                                                 : (1 << 0));

  cmd->push_back(0);

  Jtag::current_state = TAP_DRPAUSE;

  cmd->add_tdo(pos, (pos + 34));
}

}
