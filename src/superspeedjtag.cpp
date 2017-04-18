/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: superspeedjtag.cpp
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

  //INFO("Init", "%s", sys->info().c_str());

  return (void *)sys;
}

void load_binary_in_sdram(void *opaque, char* file_path, uint32_t address) {

  std::string str(file_path);

  if (opaque == NULL)
    return;

  System *sys = (System *)opaque;

  sys->load_binary_in_sdram(str, address);
}

uint64_t jtag_read_u32(void *opaque, uint64_t address) {

  if (opaque == NULL)
    return 0;

  System *sys = (System *)opaque;

  return sys->read_u32(address);
}

int32_t jtag_read(void *opaque, uint64_t address, uint64_t *value,
                  unsigned size) {

  if (opaque == NULL)
    return 0;

  System *sys = (System *)opaque;

  return sys->read_u32(address);
}

void jtag_write(void *opaque, uint64_t address, uint64_t value, unsigned size) {

  if (opaque == NULL)
    return;

  System *sys = (System *)opaque;

  sys->write_u32(value, address);
}
