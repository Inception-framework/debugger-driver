/*
 * Benchmark.h
 *
 *  Created on: Jul 12, 2016
 *      Author: Corteggiani Nassim
 */

#ifndef BENCHMARK_H_
#define BENCHMARK_H_

#include <sstream>
#include <string>
#include <time.h>

class Benchmark {

public:
  Benchmark();

  virtual ~Benchmark();

  static void init(void);

  static void start(void);

  static void stop(void);

  static std::string to_string(void);

  static void inc_nread(void);

  static void inc_nwrite(void);
private:
  static uint32_t get_tick_count(void);

  static struct timespec start_time;

  static struct timespec end_time;

  static struct timespec total_io_time;

  static unsigned long long mem_reads;

  static unsigned long long mem_writes;

  static unsigned int start_tick_count;
};

#endif /* BENCHMARK_H_ */
