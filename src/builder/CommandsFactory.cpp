/**
* @Author: Nassim
* @Date:   2017-03-15T10:15:14+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T14:45:18+01:00
*/
#include "CommandsFactory.h"

#include "CommandsBuilder.h"

#include "jtag/JTAGBuilder.h"
#include "inception/InceptionBuilder.h"

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
      CommandsFactory::builder = new InceptionBuilder();
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

  case RESET:
    cmd = CommandsFactory::builder->reset();
    break;

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
