/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: System.cpp
    @Last modified by:   noname
    @Last modified time: 16-Mar-2017
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


#include "benchmark/Benchmark.h"

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
