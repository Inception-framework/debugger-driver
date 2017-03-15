/**
* @Author: Nassim
* @Date:   2017-03-15T11:05:17+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T14:28:18+01:00
*/



#include "System.h"

bool DEBUG = false;
bool DEBUG2 = false;

#include "test/TestReport.h"
#include "test/TestsFactory.h"

#include "builder/CommandsFactory.h"

#include "main.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <thread>

#include "benchmark/Benchmark.h"

#include "drivers/flash/max32550_flash_driver.h"

#include "binutils/BinLoader.h"

using namespace std::placeholders;

using namespace jtag;
using namespace flash;

System::System() : halted(false) {

  WARNING("SYSTEM", "Please connect the jtag device first and then \
  the trace device.");

  INFO("Device", "Initializing jtag device ...");
  fx3_jtag = new Device::USBDevice(0x0B6A, 0x0001, 0);
  fx3_jtag->init();

  INFO("Device", "Initializing trace device ...");
  // fx3_trace = new Device::USBDevice(0x04b4, 0x00f3, 0);
  // fx3_trace->init();

  VERBOSE("Interface", "Starting producer...");
  producer = new Producer(fx3_jtag);

  VERBOSE("Decoder", "Starting decoder...");
  decoder = new Decoder(producer);

  producer->add_decoder(decoder);

  VERBOSE("Trace", "Starting trace...");
  // trace = new Trace(fx3_trace);
  // trace->run();

  reset();

  idcode = 0;

  flash = new MXFlash(this, 1048576, 0x10000000, 256);
}

System::~System() {}

void System::reset() {

  std::vector<uint32_t> arg;

  CommandsFactory::initProtocol(JTAG_PROTOCOL::INCEPTION);

  Command *cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::RESET, arg);

  producer->synchrone_process(cmd, (uint32_t)NULL);
}

void System::stop() {

  // trace->stop();

  fx3_jtag->close();

  // fx3_trace->close();
}

std::string System::info() {

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
  info << "            * HALTED  : "<< halted << "\r\n";

  if(flash != NULL)
    info << "            * Flash   : "<< flash->info() <<"\r\n";

  return info.str();
}

void System::synchrone_process(Command* command, uint64_t *value) {

  this->producer->synchrone_process(command, value);
}

void System::write_u32(uint32_t value, uint32_t address) {

  Command *cmd = NULL;
  std::vector<uint32_t> arg;
  uint64_t retval;

  arg.push_back(value);
  arg.push_back(address);

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::WRITE, arg);
  producer->synchrone_process(cmd, &retval);
}

uint32_t System::read_u32(uint32_t address) {

  uint64_t value;
  Command *cmd = NULL;
  std::vector<uint32_t> arg;

  arg.push_back(address);

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::READ, arg);

  Benchmark::start();
  producer->synchrone_process(cmd, &value);
  Benchmark::stop();

  return (uint32_t)value;
}

flash::Flash* System::get_flash() {

  return this->flash;
}

void System::halt() {

  if(!halted) {

    this->write_u32(0xE000EDF0, 0xA05F0003);
    halted = true;
  }

}

void System::write_reg(uint32_t reg_id, uint32_t value) {

  this->write_u32(value, 0xE000EDF8);//DCRDR
  // std::this_thread::sleep_for(std::chrono::milliseconds(100));

  this->write_u32(reg_id | 0x00010000, 0xE000EDF4);//DCRSR
  // std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

uint32_t System::read_reg(uint32_t reg_id) {

  this->write_u32(reg_id, 0xE000EDF4);
  // std::this_thread::sleep_for(std::chrono::milliseconds(100));

  uint32_t retVal =  this->read_u32(0xE000EDF8);

  return retVal;
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

  stop();
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
