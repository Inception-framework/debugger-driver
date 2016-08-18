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

extern void *jtag_init(void);

extern uint64_t jtag_read(void *opaque, uint64_t address, unsigned size);

extern void jtag_write(void *opaque, uint64_t address, uint64_t value,
                       unsigned size);

#if defined(__cplusplus)
}
#endif

#endif
