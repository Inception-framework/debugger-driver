/**
* @Author: Nassim
* @Date:   2017-03-15T11:01:37+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:49:16+01:00
*/



#include "superspeedjtag.h"

#include "benchmark/Benchmark.h"
#include "System.h"

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

    System *sys = new System();

    INFO("Init", "%s", sys->info().c_str());

    return (void *)sys;
}

uint64_t jtag_read_u32(void *opaque, uint64_t address) {

    if(opaque == NULL)
      return 0;

    System *sys = (System *)opaque;

    return sys->read_u32(address);
}

int32_t jtag_read(void *opaque, uint64_t address, uint64_t *value,
                  unsigned size) {

  if(opaque == NULL)
    return 0;

  System *sys = (System *)opaque;

  return sys->read_u32(address);
}

void jtag_write(void *opaque, uint64_t address, uint64_t value, unsigned size) {

  if(opaque == NULL)
    return;

  System *sys = (System *)opaque;

  sys->write_u32(value, address);
}
