/*
 * Benchmark.h
 *
 *  Created on: Jul 12, 2016
 *      Author: Corteggiani Nassim
 */

#ifndef BENCHMARK_H_
#define BENCHMARK_H_

#include <time.h>

class Benchmark {

public:
  Benchmark();

  virtual ~Benchmark();

  static void start();

  static void stop();

  static float score();

  static clock_t start_time;

  static clock_t stop_time;
};

#endif /* BENCHMARK_H_ */
