/**
* @Author: Nassim
* @Date:   2016-09-15T10:10:04+02:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:51:27+01:00
*/



#include "Benchmark.h"

#include "stdio.h"
#include <iostream>

struct timespec Benchmark::start_time;
struct timespec Benchmark::end_time;
struct timespec Benchmark::total_io_time = {0, 0};
unsigned long long Benchmark::mem_reads = 0;
unsigned long long Benchmark::mem_writes = 0;
uint32_t Benchmark::start_tick_count;

Benchmark::Benchmark() {}

Benchmark::~Benchmark() {}

void Benchmark::init() { start_tick_count = get_tick_count(); }

void Benchmark::start() { clock_gettime(CLOCK_MONOTONIC, &start_time); }

void Benchmark::inc_nread() { mem_reads++; }

void Benchmark::inc_nwrite() { mem_writes++; }

uint32_t Benchmark::get_tick_count(void) {

  struct timespec ts;

  clock_gettime(CLOCK_MONOTONIC, &ts);

  return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}

void Benchmark::stop() {

  clock_gettime(CLOCK_MONOTONIC, &end_time);

  total_io_time.tv_sec += end_time.tv_sec - start_time.tv_sec;
  total_io_time.tv_nsec += end_time.tv_nsec - start_time.tv_nsec;

  if (total_io_time.tv_nsec > 1000000000) {
    total_io_time.tv_sec++;
    total_io_time.tv_nsec -= 1000000000;
  }

  if (total_io_time.tv_nsec < 0) {
    total_io_time.tv_sec--;
    total_io_time.tv_nsec += 1000000000;
  }
}

std::string Benchmark::to_string() {

  uint32_t running_time = get_tick_count() - start_tick_count;

  std::ostringstream stream;

  stream << "Recorded information : \r\n"
         << "    [*] Time \r\n"
         << "        - Total running time :  " << running_time << "\r\n"
         << "        - Total IO time      :  " << total_io_time.tv_sec << "."
         << total_io_time.tv_nsec << "\r\n"
         << "    [*] IO \r\n"
         << "        - Number of reads    :  " << mem_reads << "\r\n"
         << "        - Number of writes   :  " << mem_writes << "\r\n";

  // std::cout << stream.str() << std::endl;

  return stream.str();
}
