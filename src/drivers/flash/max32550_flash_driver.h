#ifndef MAX32550_FLASH_H
#define MAX32550_FLASH_H

#define FLSH_ADDR 0x000
#define FLSH_CLKDIV 0x004
#define FLSH_CN 0x008
#define PR1E_ADDR 0x00C
#define PR2S_ADDR 0x010
#define PR2E_ADDR 0x014
#define PR3S_ADDR 0x018
#define PR3E_ADDR 0x01C
#define FLSH_MD 0x020
#define FLSH_INT 0x024
#define FLSH_DATA0 0x030
#define FLSH_DATA1 0x034
#define FLSH_DATA2 0x038
#define FLSH_DATA3 0x03C

#define FLSH_INT_AF 0x00000002

#define FLSH_CN_UNLOCK_MASK 0xF0000000
#define FLSH_CN_UNLOCK_VALUE 0x20000000

#define FLSH_CN_PEND 0x01000000
#define FLSH_CN_M3_WRITE_ENABLE 0x00020000

#define FLSH_CN_ERASE_CODE_MASK 0x0000FF00
#define FLSH_CN_ERASE_CODE_PGE 0x00005500
#define FLSH_CN_ERASE_CODE_ME 0x0000AA00

#define FLSH_CN_PGE 0x00000004
#define FLSH_CN_ME 0x00000002
#define FLSH_CN_WR 0x00000001

#include "flash.h"

#include "../../System.h"

#include <iostream>
#include <stdint.h>
#include <vector>

class System;

namespace flash {

class MXFlash : public Flash {

public:
  MXFlash(System *sys, uint32_t size, uint32_t base, uint32_t pages_amount);

  ~MXFlash();

  uint64_t target_write(uint32_t address, uint32_t value);

  void write(uint32_t address, std::vector<uint32_t> buffer);

  void read(std::vector<char> buffer, uint32_t offset, uint32_t count);

  void load_stub();

protected:
  uint32_t base;

  uint32_t pages_amount;

  uint32_t clk;

  uint32_t size;

  System *sys;

  static std::vector<uint32_t> stub;

  static uint32_t flash_ctrl_addr;

  std::string info(void);

  bool stub_loaded;

  void set_stub_args(uint32_t src_addr, uint32_t src_size, uint32_t dst_addr,
                     uint32_t dst_size);

  bool is_flash_busy(void);

  void unlock(void);

  void lock(void);

  void set_clk(void);

  void clear_interrupts(void);

  // void write(uint32_t address, std::vector<uint32_t> buffer);

  void write_u128_word();

  void write_u32_word();

  void load_buffer(std::vector<uint32_t> buffer);

  // bool flash_lock();
  //
  // bool flash_unlock();
};
}

#endif
