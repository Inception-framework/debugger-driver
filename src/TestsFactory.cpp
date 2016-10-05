#include "TestsFactory.h"

#include "TestReport.h"

#define _LOG_ALL
#include "colored.h"
#include "jtag/Command.h"
#include "System.h"
#include "jtag/CommandsFactory.h"
#include "benchmark/Benchmark.h"

using namespace std;
using namespace jtag;

std::map<enum Test, TestReport *> TestsFactory::testMap = {
    {BENCHMARK_IO,
     new TestReport("BENCHMARK_IO", "Write a random value in memory then read "
                                    "to compare. The test is repeated for all "
                                    "the address space",
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
  default:
    break;
  }

  return report;
}

void TestsFactory::benchmark_io(System* system, TestReport* report) {

  SUCCESS("Benchmark", "Starting IO Benchmark ...");

  Command *cmd = NULL;
  AccessPort *ap = NULL;
  std::vector<uint32_t> arg;
  uint32_t value = 0;

  VERBOSE("Command", "Creating 100 000 WRITE_U32 commands ...");

  Benchmark::init();

  /*
  * Perform 100 000 memory reads through AHB access port and record io access
  * time. At each read value is compared with expected value called oracle.
  * In case of mismatch, the test is aborted and benchmark results displayed.
  */

  uint32_t addr = 0x20000000;
  uint32_t val32 = 0x00000000;

  for (int i = 0; i < 100000 /*65536*/; i++) {

    system->write_u32(val32, addr);

    // Benchmark::start();
    value = system->read_u32(addr);
    // Benchmark::stop();

    // Benchmark::inc_nread();

    if (value != val32) {

      std::stringstream info;

      info << "At test " << i;
      info << " returned value differs from oracle ";
      info << "(0x" << std::hex << val32;
      info << ") : 0x" << std::hex << (uint32_t)value;
      info << ", address 0x" << addr << "\r\n";

      report->success = false;
      report->error = info.str();

      delete ap;
      return;
    }

    // addr += 4;
    val32++;
  }

  // INFO("Benchmark", "%s", (const char *)Benchmark::to_string().c_str());

  delete ap;

  return;
}
