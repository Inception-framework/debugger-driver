#include "Benchmark.h"

#include "stdio.h"

clock_t Benchmark::start_time;
clock_t Benchmark::stop_time;

Benchmark::Benchmark() {}

Benchmark::~Benchmark() {}

void Benchmark::start() { start_time = clock(); }

void Benchmark::stop() {
  stop_time = clock();
  score();
}

float Benchmark::score() {

  printf("Time to process : %02f",
         1.0 * (stop_time - start_time) / CLOCKS_PER_SEC);
}
