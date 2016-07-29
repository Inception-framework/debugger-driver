/*
 * Jtag.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef JTAG_H_
#define JTAG_H_

#include <stdint.h>

#include "../colored.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))

namespace jtag {

/*
 * The DPACC scan chain is used to access the CTRL/STAT, SELECT and RDBUFF DP
 * registers
 * The APACC scan chain is used to access registers in the currently-selected AP
 * register bank
 */

#define JTAG_OK 0xb010
#define JTAG_WAIT 0xb001

/**
 * Defines JTAG Test Access Port states.
 *
 * These definitions were gleaned from the ARM7TDMI-S Technical
 * Reference Manual and validated against several other ARM core
 * technical manuals.
 */
typedef enum tap_state {
  TAP_INVALID = -1,
  /* Proper ARM recommended numbers */
  TAP_DREXIT2 = 0x0,
  TAP_DREXIT1 = 0x1,
  TAP_DRSHIFT = 0x2,
  TAP_DRPAUSE = 0x3,
  TAP_IRSELECT = 0x4,
  TAP_DRUPDATE = 0x5,
  TAP_DRCAPTURE = 0x6,
  TAP_DRSELECT = 0x7,
  TAP_IREXIT2 = 0x8,
  TAP_IREXIT1 = 0x9,
  TAP_IRSHIFT = 0xa,
  TAP_IRPAUSE = 0xb,
  TAP_IDLE = 0xc,
  TAP_IRUPDATE = 0xd,
  TAP_IRCAPTURE = 0xe,
  TAP_RESET = 0x0f,
} tap_state_t;

#define APACC 0xB
#define DPACC 0xA

#define CSW_ADDR (0x00)
#define TAR_ADDR (0x04)
#define DRW_ADDR (0x0C)
#define SEL_ADDR (0x08)
#define CTRL_STAT (0x04)

#define DPAP_WRITE 0
#define DPAP_READ 1

/* Fields of the MEM-AP's CSW register */
#define CSW_8BIT 0
#define CSW_16BIT 1
#define CSW_32BIT 2
#define CSW_ADDRINC_MASK (3UL << 4)
#define CSW_ADDRINC_OFF 0UL
#define CSW_ADDRINC_SINGLE (1UL << 4)
#define CSW_ADDRINC_PACKED (2UL << 4)
#define CSW_DEVICE_EN (1UL << 6)
#define CSW_TRIN_PROG (1UL << 7)
#define CSW_SPIDEN (1UL << 23)
/* 30:24 - implementation-defined! */
#define CSW_HPROT (1UL << 25)        /* ? */
#define CSW_MASTER_DEBUG (1UL << 29) /* ? */
#define CSW_SPROT (1UL << 30)
#define CSW_DBGSWENABLE (1UL << 31)

typedef enum mem_ap_reg {
  /* MEM-AP register addresses */
  MEM_AP_REG_CSW = 0x00,
  MEM_AP_REG_TAR = 0x04,
  MEM_AP_REG_TAR64 = 0x08, /* RW: Large Physical Address Extension */
  MEM_AP_REG_DRW = 0x0C,   /* RW: Data Read/Write register */
  MEM_AP_REG_BD0 = 0x10,   /* RW: Banked Data register 0-3 */
  MEM_AP_REG_BD1 = 0x14,
  MEM_AP_REG_BD2 = 0x18,
  MEM_AP_REG_BD3 = 0x1C,
  MEM_AP_REG_MBT = 0x20,    /* --: Memory Barrier Transfer register */
  MEM_AP_REG_BASE64 = 0xF0, /* RO: Debug Base Address (LA) register */
  MEM_AP_REG_CFG = 0xF4,    /* RO: Configuration register */
  MEM_AP_REG_BASE = 0xF8,   /* RO: Debug Base Address register */
} MEM_AP_REG;

/* tap_move[i][j]: tap movement command to go from state i to state j
 * encodings of i and j are what tap_move_ndx() reports.
 *
 */
struct tms_sequences {
  uint8_t bits;
  uint8_t bit_count;
};

/*
 * These macros allow us to specify TMS state transitions by bits rather than
 * hex bytes.
 * Read the bits from LSBit first to MSBit last (right-to-left).
 */
#define HEX__(n) 0x##n##LU

#define B8__(x)                                                                \
  ((((x)&0x0000000FLU) ? (1 << 0) : 0) + (((x)&0x000000F0LU) ? (1 << 1) : 0) + \
   (((x)&0x00000F00LU) ? (1 << 2) : 0) + (((x)&0x0000F000LU) ? (1 << 3) : 0) + \
   (((x)&0x000F0000LU) ? (1 << 4) : 0) + (((x)&0x00F00000LU) ? (1 << 5) : 0) + \
   (((x)&0x0F000000LU) ? (1 << 6) : 0) + (((x)&0xF0000000LU) ? (1 << 7) : 0))

#define B8(bits, count)                                                        \
  { ((uint8_t)B8__(HEX__(bits))), (count) }

typedef const struct tms_sequences tms_table[6][6];

const struct tms_sequences tms_seqs[6][6] = {
    /* to state: */
    /*	RESET		     IDLE			      DRSHIFT			    DRPAUSE					IRSHIFT					IRPAUSE */ /* from state: */
    {B8(1111111, 7), B8(0000000, 7), B8(0010, 4), B8(01010, 5), B8(0011011, 7),
     B8(0010110, 7)}, /* RESET */
    {B8(1111111, 7), B8(0000000, 7), B8(001, 3), B8(0101, 4), B8(0011, 4),
     B8(01011, 5)}, /* IDLE */
    {B8(1111111, 7), B8(011, 3), B8(00111, 5), B8(00111, 2), B8(001111, 6),
     B8(0101111, 7)}, /* DRSHIFT */
    {B8(1111111, 7), B8(011, 3), B8(00111, 5), B8(0, 1), B8(001111, 6),
     B8(0101111, 7)}, /* DRPAUSE */
    {B8(1111111, 7), B8(011, 3), B8(00111, 5), B8(010111, 6), B8(001111, 6),
     B8(01, 2)}, /* IRSHIFT */
    {B8(1111111, 7), B8(011, 3), B8(00111, 5), B8(010111, 6), B8(001111, 6),
     B8(0, 1)} /* IRPAUSE */
};

static const struct name_mapping {
  enum tap_state symbol;
  const char *name;
} tap_name_mapping[] = {
    {
        TAP_RESET, "RESET",
    },
    {
        TAP_IDLE, "RUN/IDLE",
    },
    {
        TAP_DRSELECT, "DRSELECT",
    },
    {
        TAP_DRCAPTURE, "DRCAPTURE",
    },
    {
        TAP_DRSHIFT, "DRSHIFT",
    },
    {
        TAP_DREXIT1, "DREXIT1",
    },
    {
        TAP_DRPAUSE, "DRPAUSE",
    },
    {
        TAP_DREXIT2, "DREXIT2",
    },
    {
        TAP_DRUPDATE, "DRUPDATE",
    },
    {
        TAP_IRSELECT, "IRSELECT",
    },
    {
        TAP_IRCAPTURE, "IRCAPTURE",
    },
    {
        TAP_IRSHIFT, "IRSHIFT",
    },
    {
        TAP_IREXIT1, "IREXIT1",
    },
    {
        TAP_IRPAUSE, "IRPAUSE",
    },
    {
        TAP_IREXIT2, "IREXIT2",
    },
    {
        TAP_IRUPDATE, "IRUPDATE",
    },
    {
        TAP_IDLE, "IDLE",
    },
};

class Jtag {

public:
  Jtag();

  virtual ~Jtag();

  static int tap_get_tms_path(tap_state_t from, tap_state_t to);

  static int tap_get_tms_path_len(tap_state_t from, tap_state_t to);

  static tap_state_t current_state;

  static const char *tap_state_name(tap_state_t state);

private:
  static int tap_move_ndx(tap_state_t astate);
};

} /* namespace JTAG */

#endif /* JTAG_H_ */
