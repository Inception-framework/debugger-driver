
#include "System.h"

#include "TestReport.h"

#include "main.h"

#include <algorithm>
#include <functional>
#include <sstream>

#include "benchmark/Benchmark.h"

#include "drivers/flash/max32550_flash_driver.h"

#include "BinLoader.h"

using namespace std::placeholders;

using namespace jtag;
using namespace flash;

System::System() : is_initialized(false) {

  INFO("Device", "Initializing superspeed device ...");
  fx3 = new Device::USBDevice(0x04B4, 0x00F0, 0);
  fx3->init();

  VERBOSE("Interface", "Starting producer thread ...");
  producer = new Producer(fx3);

  VERBOSE("Decoder", "Starting decoder thread ...");
  decoder = new Decoder(producer);

  producer->add_decoder(decoder);

  idcode = 0;

  ap = NULL;

  flash = new MXFlash(this, 1048576, 0x10000000, 256);
}

System::~System() {}

std::string System::info() {

  if (!is_initialized)
    init_jtag();

  Command *cmd = NULL;
  std::vector<uint32_t> arg;
  uint64_t value = 0;

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::IDCODE, arg);
  producer->synchrone_process(cmd, &value);

  this->idcode = (uint32_t)value;

  std::stringstream info;

  info << "    [*] Avatar gateway version 1.0\r\n";
  info << "            * Author  : Corteggiani Nassim\r\n";
  info << "            * Date    : 2016\r\n";
  info << "            * Company : Maxim Integrated\r\n";
  info << "            * Project : Avatar On Steroids\r\n";

  info << "    [*] Chip information\r\n";
  info << "            * IDCODE  : 0x"<< std::hex << idcode << "\r\n";

  if(flash != NULL)
    info << "            * Flash   : "<< flash->info() <<"\r\n";


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

  Benchmark::start();
  producer->synchrone_process(cmd, &value);
  Benchmark::stop();

  return (uint32_t)value;
}

flash::Flash* System::get_flash() {

  return this->flash;
}

void System::write_reg(uint32_t reg_id, uint32_t value) {

  this->write_u32(0xE000EDF8, value);

  this->write_u32(0xE000EDF4, reg_id |  (1 << 16));
}

uint32_t System::read_reg(uint32_t reg_id) {

  this->write_u32(0xE000EDF4, reg_id);

  return this->read_u32(0xE000EDF8);
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

void System::load_binary_in_sdram(std::string file_path) {

  BinLoader* bin_loader = new BinLoader("/home/noname/workspacees06/stub/SLA/stub.bin");

  try {

    while(true)
    bin_loader->next();

  } catch (std::runtime_error) {

    INFO("BIN_LOADER", "End Of File");
  }

}
