#include "System.h"

#include "TestReport.h"

#include "main.h"

#include <algorithm>
#include <functional>
#include <sstream>

using namespace std::placeholders;

using namespace jtag;

System::System() : is_initialized(false) {

  INFO("Device", "Initializing superspeed device ...");
  fx3 = new Device::USBDevice(0x04B4, 0x00F0, 0);
  fx3->init();

  VERBOSE("Interface", "Starting producer thread ...");
  producer = new Producer(fx3);

  VERBOSE("Decoder", "Starting decoder thread ...");
  decoder = new Decoder(producer);

  producer->add_decoder(decoder);

  ap = NULL;
}

System::~System() {}

std::string System::info() {

  // Command *cmd = NULL;
  std::vector<uint32_t> arg;
  // uint64_t value = 0;

  std::stringstream info;

  info << "    [*] Avatar gateway version 1.0\r\n";
  info << "            * Author  : Corteggiani Nassim\r\n";
  info << "            * Date    : 2016\r\n";
  info << "            * Company : Maxim Integrated\r\n";
  info << "            * Project : Avatar On Steroids\r\n";

  // cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::IDCODE, arg);
  // producer->synchrone_process(cmd, &value);

  return info.str();
}

void System::synchrone_process(Command* command, uint64_t *value) {

  if (!is_initialized)
    init_jtag();

  this->producer->synchrone_process(command, value);
}

void System::init_jtag() {

  if (is_initialized)
    return;

  is_initialized = true;

  Command *cmd = NULL;
  std::vector<uint32_t> arg;
  uint64_t value = 0;

  VERBOSE("Command", "Creating RESET command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::RESET, arg);
  producer->synchrone_process(cmd, &value);

  VERBOSE("Command", "Creating ACTIVE command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::ACTIVE, arg);
  producer->synchrone_process(cmd, &value);
}

void System::write_u32(uint32_t value, uint32_t address) {

  Command *cmd = NULL;
  std::vector<uint32_t> arg;
  uint64_t retval;

  if (!is_initialized)
    init_jtag();

  if(ap == NULL)
    select();

  arg.push_back(value);
  arg.push_back(address);

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::WRITE_U32, arg);
  producer->synchrone_process(cmd, &retval);
}

void System::select() {

  Command *cmd = NULL;
  std::vector<uint32_t> arg;
  uint64_t value = 0;

  if (ap == NULL) {

    VERBOSE("Command", "Creating Select command ...");
    ap = new AHB_AP();

    arg.push_back(ap->select());

    cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::SELECT, arg);
    producer->synchrone_process(cmd, &value);
  }
}

uint32_t System::read_u32(uint32_t address) {

  uint64_t value;
  Command *cmd = NULL;
  std::vector<uint32_t> arg;

  if (!is_initialized)
    init_jtag();

  if(ap == NULL)
    select();

  arg.push_back(address);

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::READ_U32, arg);
  producer->synchrone_process(cmd, &value);

  return (uint32_t)value;
}

/*
* Start all the tests
*/
void System::check_up() {
  // std::for_each(Test_Begin, Test_End, std::bind(&System::check, this, _1));
  // foreach (Test test in Enum.GetValues(typeof(Test))) { this->check(test); }

  for (auto i = 0; i < CHECK_DEVICE; i++) {
    this->check(static_cast<Test>(i));
  }
}

void System::check(Test test) {

  TestReport *report;

  std::stringstream info;

  report = TestsFactory::CreateTest(test, this);
  info << "    Checking " << report->name << "\r\n";

  info << "    Status         : " << report->success << "\r\n";
  info << "    Description    : " << report->description << "\r\n";
  info << "    Error          : " << report->error << "\r\n";
  info << "    Gravity        : " << std::hex << report->gravity << "\r\n";

  if(report->success)
    SUCCESS(report->name.c_str(), "passed !");
  else
    ALERT(report->name.c_str(), "failed !");

  INFO(report->name.c_str(), "%s", info.str().c_str());

}
