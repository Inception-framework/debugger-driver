#include "avatar-jtag.h"

#include "main.h"

using namespace jtag;

Device::USBDevice *fx3 = NULL;

Producer *producer = NULL;

Decoder *decoder = NULL;

void *jtag_init(void) {

  Command *cmd;
  AccessPort *ap;
  std::vector<uint32_t> arg;

  INFO("Device", "Initializing superspeed device ...");
  fx3 = new Device::USBDevice(0x04B4, 0x00F0, 0);
  fx3->init();

  INFO("Interface", "Starting producer thread ...");
  producer = new Producer(fx3);
  producer->start();

  INFO("Decoder", "Starting decoder thread ...");
  decoder = new Decoder(producer);
  decoder->start();

  producer->add_decoder(decoder);

  INFO("Command", "Creating RESET command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::RESET, arg);
  producer->add_cmd_to_queue(cmd);

  INFO("Command", "Creating ACTIVE command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::ACTIVE, arg);
  producer->add_cmd_to_queue(cmd);

  INFO("Command", "Creating Select command ...");
  ap = new AHB_AP();

  arg.push_back(ap->select());

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::SELECT, arg);
  producer->add_cmd_to_queue(cmd);
}

uint64_t jtag_read(void *opaque, uint64_t address, unsigned size) {

  Command *cmd;
  std::vector<uint32_t> arg;

  INFO("Command", "Creating READ_U32 command ...");

  arg.push_back(address);

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::READ_U32, arg);

  producer->add_cmd_to_queue(cmd);

  return 0;
}

void jtag_write(void *opaque, uint64_t address, uint64_t value, unsigned size) {

  Command *cmd;
  std::vector<uint32_t> arg;

  INFO("Command", "Creating WRITE_U32 command ...");

  arg.push_back(value);

  arg.push_back(address);

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::WRITE_U32, arg);

  producer->add_cmd_to_queue(cmd);
}
