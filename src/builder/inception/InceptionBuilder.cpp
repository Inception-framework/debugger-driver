/**
* @Author: Nassim
* @Date:   2017-03-15T10:36:58+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T15:11:34+01:00
*/



#include "InceptionBuilder.h"

#include "../Jtag.h"
#include "../Command.h"

namespace jtag {

InceptionBuilder::InceptionBuilder()  {}
InceptionBuilder::~InceptionBuilder() {}


jtag::Command* InceptionBuilder::write(uint32_t data, uint32_t address) {

  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::WRITE);

  VERBOSE("InceptionBuilder","Write command at address 0x%08x", address);

  cmd->push_back(0x14); //Write a word of 32bits

  cmd->push_back(0x00); //Only once
  cmd->push_back(0x00);
  cmd->push_back(0x01);

  for( auto i=3; i>=0; i--)
    cmd->push_back(address >> (8*i));

  for( auto i=3; i>=0; i--)
    cmd->push_back(data >> (8*i));

  return cmd;
}

jtag::Command* InceptionBuilder::read(uint32_t address) {

  VERBOSE("InceptionBuilder","Read command at address 0x%08x", address);

  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::WRITE);

  cmd->push_back(0x24); //Read a word of 32bits

  cmd->push_back(0x00); //Only once
  cmd->push_back(0x00);
  cmd->push_back(0x01);

  for( auto i=3; i>=0; i--)
    cmd->push_back(address >> (8*i));

  return cmd;
}

jtag::Command* InceptionBuilder::reset() {

  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::RESET);

  cmd->push_back(0x30);
  cmd->push_back(0x00);
  cmd->push_back(0x00);
  cmd->push_back(0x00);

  return cmd;
}

jtag::Command* InceptionBuilder::idcode() {

    jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::IDCODE);

    cmd->push_back(0x40);
    cmd->push_back(0x00);
    cmd->push_back(0x00);
    cmd->push_back(0x00);

    return cmd;
}

jtag::Command* InceptionBuilder::init() {

}

}
