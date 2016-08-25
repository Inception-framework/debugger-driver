/**
 *
 * @author Corteggiani Nassim <nassim.corteggiani@maximintegrated.com>
 * @version 0.0.1
 */
#ifndef __SUPERSPEEDJTAG__
#define __SUPERSPEEDJTAG__

#include <stdint.h>

#include "main.h"

typedef struct superspeed_jtag {

  Device::USBDevice *fx3;

  Producer *producer;

  Decoder *decoder;
} SuperspeedJtag;

#if defined(__cplusplus)
extern "C" {
#endif

void *jtag_init(void);

uint64_t jtag_read(void *opaque, uint64_t address, unsigned size);

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
