/**
* @Author: Nassim
* @Date:   2016-09-15T10:10:04+02:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:51:35+01:00
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

  static long double time(void) {return total_io_time.tv_sec;}

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
