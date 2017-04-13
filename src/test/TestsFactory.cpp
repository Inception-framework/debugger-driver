/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: TestsFactory.cpp
    @Last modified by:   Corteggiani
    @Last modified time: 15-Mar-2017
    @License: GPLv3

    Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
    Copyright (C) 2017 Corteggiani Nassim <Corteggiani>

*
*    This program is free software: you can redistribute it and/or modify      *
*    it under the terms of the GNU General Public License as published by      *
*    the Free Software Foundation, either version 3 of the License, or         *
*    (at your option) any later version.                                       *
*    This program is distributed in the hope that it will be useful,           *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*    GNU General Public License for more details.                              *
*    You should have received a copy of the GNU General Public License         *
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
*                                                                              *
*                                                                              *
********************************************************************************/

#include "../test/TestsFactory.h"

#include "../test/TestReport.h"

#define _LOG_ALL
#include "../colored.h"
#include "../builder/Command.h"
#include "../System.h"
#include "../builder/CommandsFactory.h"
#include "../benchmark/Benchmark.h"

#include <time.h>
#include <chrono>
#include <thread>

using namespace std;
using namespace jtag;

std::map<enum Test, TestReport *> TestsFactory::testMap = {
    {BENCHMARK_IO,
     new TestReport("BENCHMARK_IO", "Write a random value in memory then read "
                                    "to compare. The test is repeated for all "
                                    "the address space",
                    0)},
    {TEST_TRACE,
     new TestReport("TRACE", "Test if the trace mode exist and can be enabled",
                    0)},
    {CHECK_WRITE_U32,
     new TestReport("CHECK_WRITE_U32", "Perform a 32bits write at a valid "
                                       "random address and read the same "
                                       "address to compare",
                    0)},
    {CHECK_READ_U32,
     new TestReport("CHECK_READ_U32", "Read at a valid random address", 0)},
    {CHECK_DEVICE, new TestReport("CHECK_DEVICE",
                                  "Device check-up (Availability/Speed/)", 0)},
    {TEST_FLASH, new TestReport("CHECK_Flash", "Flash check-up", 0)},
    {TEST_AHB_AP_SCS,
     new TestReport("TEST_AHB_AP_SCS",
                    "Same test than run_test.sv : HALT, AMBA Write, RESUME",
                    0)},
    {TEST_AHB_AP_CSW,
     new TestReport("TEST_AHB_AP_CSW",
                    "Read Write and compare on AHB_AP CSW register", 0)},
};

TestsFactory::TestsFactory() {}

TestsFactory::~TestsFactory() {}

TestReport *TestsFactory::CreateTest(Test type, System *system) {

  TestReport *report = TestsFactory::testMap.at(type);

  switch (type) {
  case BENCHMARK_IO:

    TestsFactory::benchmark_io(system, report);
    report->gravity = 1;
    break;
  case CHECK_WRITE_U32:

    report->gravity = 0;
    break;
  case CHECK_READ_U32:

    report->gravity = 0;
    break;
  case CHECK_DEVICE:

    report->gravity = 0;
    break;
  case TEST_TRACE:

    TestsFactory::trace(system, report);
    report->gravity = 1;
    break;
  case TEST_FLASH:

    TestsFactory::flash(system, report);
    report->gravity = 1;
    break;

  case TEST_AHB_AP_SCS:

    TestsFactory::ahb_ap_scs(system, report);
    report->gravity = 1;
    break;

  case TEST_AHB_AP_CSW:

    TestsFactory::ahb_ap_csw(system, report);
    report->gravity = 1;
    break;

  default:
    break;
  }

  return report;
}

void TestsFactory::trace(System *sys, TestReport *report) {

  Command *cmd;
  uint64_t value;
  std::vector<uint32_t> arg;

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::TRACE, arg);
  sys->synchrone_process(cmd, &value);

  report->success = true;
}

void TestsFactory::idcode(System *system, TestReport *report) {

  Command *cmd;
  uint64_t value;
  std::vector<uint32_t> arg;

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::IDCODE, arg);
  system->synchrone_process(cmd, &value);

  report->success = true;
  // report->error = info.str();
}

void TestsFactory::flash(System *system, TestReport *report) {

  report->success = true;

  std::vector<uint32_t> buffer = {0x0ABABABA, 0x0ABABABA, 0x0ABABABA};

  system->get_flash()->write(0x10000000, buffer);
}

/*
* Perform 100 000 memory reads through AHB access port and record io access
* time. At each read value is compared with expected value called oracle.
* In case of mismatch, the test is aborted and benchmark results displayed.
*/
void TestsFactory::benchmark_io(System *system, TestReport *report) {

  SUCCESS("Benchmark", "Starting IO Benchmark ...");

  uint64_t value;

  VERBOSE("Command", "Creating 100 000 WRITE_U32 commands ...");

  Benchmark::init();

  srand(time(NULL));

  uint32_t addr = 0x2000C000;
  uint32_t val32 = rand() % 0xFFFFFFFF;

  for (int i = 0; i < /*100000*/ 65536; i++) {

    system->write_u32(val32, addr);

    // std::this_thread::sleep_for(std::chrono::milliseconds(2));
    // for(auto j=0;j<10000;j++);

    value = system->read_u32(addr);

    VERBOSE("Benchmark", "Wrote 0x%8x then read 0x%8x", val32, value);

    //std::this_thread::sleep_for(std::chrono::milliseconds(2));

    // for(auto j=0;j<10000;j++);

    Benchmark::inc_nread();

    if (value != val32) {

      std::stringstream info;

      info << "At test " << i;
      info << " returned value differs from oracle ";
      info << "(0x" << std::hex << val32;
      info << ") : 0x" << std::hex << (uint32_t)value;
      info << ", address 0x" << addr << "\r\n";

      report->success = false;
      report->error = info.str();

      return;
    }

    // if( (addr + 4) < 0x20010000)
      // addr += 4;
    val32 = rand() % 0xFFFFFFFF;
    // val32++;
  }

  INFO("Benchmark", "%s", (const char *)Benchmark::to_string().c_str());

  report->success = true;

  return;
}

void TestsFactory::ahb_ap_csw(System *system, TestReport *report) {

  Command *cmd;
  uint64_t value = 0;
  std::vector<uint32_t> arg;

  // cmd = CommandsFactory::CreateCommand(COMMAND_TYPE:JTAG_:ABORT, arg);
  // system->synchrone_process(cmd, &value);

  arg.push_back(0x23000042);
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::SET_AP_AHB_CSW, arg);
  system->synchrone_process(cmd, &value);

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::READ_CSW, arg);
  system->synchrone_process(cmd, &value);

  if (value == 0x23000042)
    report->success = true;
  else {

    std::stringstream info;

    info << "At test " << 0;
    info << " returned value differs from oracle ";
    info << "(0x" << std::hex << 0x23000042;
    info << ") : 0x" << std::hex << value;
    info << ", address AP_CSW_ADDR"
         << "\r\n";

    report->success = false;
    report->error = info.str();
  }
}

void TestsFactory::ahb_ap_scs(System *system, TestReport *report) {

  Command *cmd;
  uint64_t value = 0, regVal = 0;
  std::vector<uint32_t> arg;

  // cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::SELECT, arg);
  // system->synchrone_process(cmd, &value);

  // cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::DAP_ABORT, arg);
  // system->synchrone_process(cmd, &value);

  // system->write_u32(0xA05F0001, 0xE000EDF0);
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  system->write_u32(0xA05F0003, 0xE000EDF0);
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // arg.push_back(0x23000052);
  // cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::SET_AP_AHB_CSW, arg);
  // system->synchrone_process(cmd, &value);

  // std::vector<uint32_t> buffer = {0x0ABABABA, 0x0ABABABA, 0x0ABABABA};
  //
  // CommandsFactory::write_multi(cmd, 0x20000200, &buffer);

  // system->write_u32(0x00DDFFFF, 0x20000200);

  // value = system->read_u32(0x20000200);

  system->write_reg(0, 3);

  regVal = system->read_reg(0);

  ALERT("REG", "0x%08x", regVal);

  if (regVal == 3)
    report->success = true;
  else {

    std::stringstream info;

    info << "At test " << 0;
    info << " returned value differs from oracle ";
    info << "(0x" << std::hex << 3;
    info << ") : 0x" << std::hex << regVal;
    info << ", address 0x" << 0x20000200 << "\r\n";
    info << " REG = " << std::hex << regVal << "\r\n";

    report->success = false;
    report->error = info.str();
  }

  // system->write_u32(0x20000240, 0xE000EDF8);
  // system->write_u32(0x0001000F, 0xE000EDF4);
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // do {
  //
  //   value = system->read_u32(0xE000EDF0);
  // } while( value & (1UL << 16));
  //
  // system->write_u32(0xA05F0001, 0xE000EDF0);
}
