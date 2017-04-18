/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: superspeedjtag.h
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

#ifndef __SUPERSPEEDJTAG__
#define __SUPERSPEEDJTAG__

#include <stdint.h>

#include "main.h"

#if defined(__cplusplus)
extern "C" {
#endif

void *jtag_init(void);

int32_t jtag_read(void *opaque, uint64_t address, uint64_t *value,
                  unsigned size);

uint64_t jtag_read_u32(void *opaque, uint64_t address);

void jtag_write(void *opaque, uint64_t address, uint64_t value, unsigned size);

void benchmark_start(void);

void benchmark_stop(void);

void benchmark_to_string(void);

void benckmark_inc_nread(void);

void benckmark_inc_nwrite(void);

void load_binary_in_sdram(void *opaque, char* file_path, uint32_t address);

#if defined(__cplusplus)
}
#endif

#endif
