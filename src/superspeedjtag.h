/**
* @Author: Nassim
* @Date:   2017-01-04T11:07:29+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:49:21+01:00
*/



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

#if defined(__cplusplus)
}
#endif

#endif
