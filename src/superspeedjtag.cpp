#include "superspeedjtag.h"

#include "benchmark/Benchmark.h"
#include <string>

using namespace jtag;

void benchmark_start() { Benchmark::start(); }

void benchmark_stop() { Benchmark::stop(); }

void benckmark_inc_nread() { Benchmark::inc_nread(); }

void benckmark_inc_nwrite() { Benchmark::inc_nwrite(); }

void benchmark_to_string() {

  std::string str = Benchmark::to_string();

  const char *c_str = str.c_str();

  INFO("Benchmark", "%s", c_str);
}

void *jtag_init(void) {

  Command *cmd;
  AccessPort *ap;
  std::vector<uint32_t> arg;
  uint64_t value;

  INFO("Device", "Initializing superspeed device ...");
  Device::USBDevice *fx3 = new Device::USBDevice(0x04B4, 0x00F0, 0);
  fx3->init();

  INFO("Interface", "Starting producer thread ...");
  Producer *producer = new Producer(fx3);
  // producer->start();

  INFO("Decoder", "Starting decoder thread ...");
  Decoder *decoder = new Decoder(producer);

  producer->add_decoder(decoder);

  // INFO("Command", "Creating RESET command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::RESET, arg);
  // producer->add_cmd_to_queue(cmd);
  producer->synchrone_process(cmd, &value);

  // INFO("Command", "Creating ACTIVE command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::ACTIVE, arg);
  // producer->add_cmd_to_queue(cmd);
  producer->synchrone_process(cmd, &value);

  // INFO("Command", "Creating Select command ...");
  ap = new AHB_AP();
  arg.push_back(ap->select());
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::SELECT, arg);
  // producer->add_cmd_to_queue(cmd);
  producer->synchrone_process(cmd, &value);

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::IDCODE, arg);
  // producer->add_cmd_to_queue(cmd);
  producer->synchrone_process(cmd, &value);

  SuperspeedJtag *sj = new SuperspeedJtag{fx3, producer, decoder};

  Benchmark::init();

  INFO("Device", "Avatar ready !");

  return (void *)sj;
}

int32_t jtag_read(void *opaque, uint64_t address, uint64_t *value,
                  unsigned size) {

  Command *cmd;
  std::vector<uint32_t> arg;

  SuperspeedJtag *sj = (SuperspeedJtag *)opaque;

  // INFO("Command", "Creating READ_U32 command ...");

  arg.push_back((uint32_t)address);

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::READ_U32, arg);

  // sj->producer->add_cmd_to_queue(cmd);
  sj->producer->synchrone_process(cmd, value);

  // while (sj->decoder->process_jtag_queue(value) == -1)
  //   ;

  return 0;
}

void jtag_write(void *opaque, uint64_t address, uint64_t value, unsigned size) {

  Command *cmd;
  std::vector<uint32_t> arg;

  SuperspeedJtag *sj = (SuperspeedJtag *)opaque;

  // INFO("Command", "Creating WRITE_U32 command ...");

  arg.push_back((uint32_t)value);
  arg.push_back((uint32_t)address);

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::WRITE_U32, arg);

  // sj->producer->add_cmd_to_queue(cmd);
  sj->producer->synchrone_process(cmd, &value);
}
