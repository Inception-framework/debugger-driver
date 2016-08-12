/**
 *
 * @author Corteggiani Nassim <nassim.corteggiani@maximintegrated.com>
 * @version 0.0.1
 */
#ifndef __AVATAR_JTAG__
#define __AVATAR_JTAG__

#include <stdint.h>

void *jtag_init(void);

uint64_t jtag_read(void *opaque, uint64_t address, unsigned size);

void jtag_write(void *opaque, uint64_t address, uint64_t value, unsigned size);

#endif
