/*
 * CommandsFactory.cpp
 *
 *  Created on: Jul 12, 2016
 *  @author Corteggiani Nassim <nassim.corteggiani@maximintegrated.com>
 */

#include "CommandsFactory.h"

#include "CommandsBuilder.h"

#include "JTAGBuilder.h"

#include <assert.h>


using namespace std;
using namespace jtag;

CommandsBuilder* CommandsFactory::builder = NULL;

enum JTAG_PROTOCOL CommandsFactory::protocol = JTAG;

CommandsFactory::CommandsFactory() {}

void CommandsFactory::initProtocol(JTAG_PROTOCOL new_protocol) {

  switch(new_protocol) {
    case SWD:
      // builder = new SWDBuilder();
      break;
    case JTAG:
      CommandsFactory::builder = new JTAGBuilder();
      break;
    case INCEPTION:
      // builder = new InceptionBuilder();
      break;
    default:
      throw std::runtime_error("CommandsFactory initialized with an unknown protocol ... \n");
  }

  builder->init();
}

CommandsFactory::~CommandsFactory() {}

jtag::Command *CommandsFactory::CreateCommand(COMMAND_TYPE type,
                                              vector<uint32_t> &argv) {

  assert (CommandsFactory::builder && "You must init a protocol before using the CreateCommand method");

  jtag::Command* cmd = NULL;

  switch (type) {

  case READ:
    if (check_arg(argv, 1) == true) {
      cmd = CommandsFactory::builder->read(argv.at(0));
    } else
      ALERT("CommandsFactory", "READ args missing");

    break;

  case WRITE:
    if (check_arg(argv, 2) == true) {
      cmd = CommandsFactory::builder->write(argv.at(1), argv.at(0));
    } else
      ALERT("CommandsFactory", "WRITE args missing");

    break;

  case IDCODE:
    cmd = CommandsFactory::builder->idcode();

  case EXIT:
    break;
  }

  argv.clear();

  return cmd;
}

bool CommandsFactory::check_arg(vector<uint32_t> &argv, uint32_t required_size) {

  if (argv.size() != required_size)
    return false;

  return true;
}
