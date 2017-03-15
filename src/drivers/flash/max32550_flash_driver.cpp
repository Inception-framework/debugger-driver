/***************************************************************************
*   Copyright (C) 2012 by Maxim Integrated                                *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
*********************************maxim_write_code**************************/

#include "max32550_flash_driver.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "../../builder/Command.h"
#include "../../builder/CommandsFactory.h"

using namespace jtag;
using namespace std;

namespace flash {

std::vector<uint32_t> MXFlash::stub = {
    0xDFF84C40, 0xD0F80080, 0xB8F1000F, 0x00F01E80, 0x47684745, 0x3FF4F6AF,
    0x226002F1, 0x040257F8, 0x048BC4F8, 0x3080A568, 0x45F00105, 0xA560D4F8,
    0x088018F0, 0x010F7FF4, 0xFAAF8F42, 0x28BF00F1, 0x08074760, 0x013B0BB1,
    0xFFF7DCBF, 0x00BE00BF, /* pFLASH_CTRL_BASE: */
    0x00000040              /* .word 0x40000000 (value will be overwritten) */
};

uint32_t MXFlash::flash_ctrl_addr = 0x40029000;

MXFlash::MXFlash(System *sys, uint32_t size, uint32_t base,
                 uint32_t pages_amount)
    : Flash("MX 32550 Nand Flash", size, base, pages_amount), sys(sys),
      size(size), base(base), pages_amount(pages_amount), stub_loaded(false) {

  MXFlash::stub[sizeof(MXFlash::stub) - 4] = base;

  this->clk = 54;

  this->stub_base = 0x20000000;

  uint32_t addr = this->stub_base + this->stub.size();
  addr += (4 - (addr % 4));

  this->buffer_base = addr;
}

MXFlash::~MXFlash() {}

std::string MXFlash::info(void) {

  std::stringstream stdout;

  stdout << "MAX 32550 Flash : " << Flash::info();

  return stdout.str();
}

bool MXFlash::is_flash_busy(void) {

  uint32_t flash_ctrl_value = sys->read_u32(MXFlash::flash_ctrl_addr + FLSH_CN);

  if (flash_ctrl_value & (FLSH_CN_PEND | FLSH_CN_ERASE_CODE_MASK | FLSH_CN_PGE |
                          FLSH_CN_ME | FLSH_CN_WR))
    return true;

  // if ((flash_ctrl_value & 0x048800E0) == 0) {
  //   INFO("FLASH", "Still busy, waiting... CTRL REG = 0x%08x", flash_ctrl_value);
  //   return false;
  // }

  return true;
}

void MXFlash::write_u128_word() {}

void MXFlash::write_u32_word() {}

void MXFlash::write(uint32_t address, std::vector<uint32_t> buffer) {

  /*Attempt to write using the stub*/
  // load_stub();

  // load_buffer(buffer);

  set_stub_args(address, this->buffer_base, 0x10000000, buffer.size()/4);

  load_buffer(buffer);

  INFO("REGISTER", "R0 = 0x%08x", sys->read_reg(0));
  // INFO("REGISTER", "R1 = 0x%08x", sys->read_reg(1));
  // INFO("REGISTER", "R2 = 0x%08x", sys->read_reg(2));
  // INFO("REGISTER", "R3 = 0x%08x", sys->read_reg(3));

  // load_buffer(buffer);

  // sys->execute(0x20000000);

  // set_clk();
  //
  // unlock();
  //
  // clear_interrupts();
  //
  //
  // while (!is_flash_busy())
  //   ;

  /*Attempt to write using 128bits mode*/
  // write_u128_word(address, buffer);

  /*Attempt to write using 32bits mode*/
  // write_u32_word(address, buffer);

  // lock();
}

void MXFlash::clear_interrupts(void) {

  uint32_t int_reg = sys->read_u32(MXFlash::flash_ctrl_addr + FLSH_INT);

  if ((~0xFFFFFCFF & int_reg) != 0) {
    ALERT("FLASH", "Interrupt Register = 0x%08x", int_reg);
    throw runtime_error("Flash interrupt...");
  }

  sys->write_u32(0x00000000, MXFlash::flash_ctrl_addr + FLSH_INT);
}

void MXFlash::set_clk(void) {

  sys->write_u32(this->clk, MXFlash::flash_ctrl_addr + FLSH_CLKDIV);
}

void MXFlash::lock(void) {

  uint32_t ctrl_val = sys->read_u32(MXFlash::flash_ctrl_addr + FLSH_CN);

  if ((ctrl_val & FLSH_CN_UNLOCK_MASK) == FLSH_CN_UNLOCK_VALUE)
    return;

  ctrl_val &= ~FLSH_CN_UNLOCK_MASK;

  sys->write_u32(ctrl_val, MXFlash::flash_ctrl_addr + FLSH_CN);
}

void MXFlash::unlock(void) {

  uint32_t ctrl_val = sys->read_u32(MXFlash::flash_ctrl_addr + FLSH_CN);

  ctrl_val &= ~FLSH_CN_UNLOCK_MASK;
  ctrl_val |= FLSH_CN_UNLOCK_VALUE;

  if((ctrl_val & FLSH_CN_UNLOCK_MASK) == FLSH_CN_UNLOCK_VALUE)
    return;

  sys->write_u32(ctrl_val, MXFlash::flash_ctrl_addr + FLSH_CN);

  uint32_t real_value = sys->read_u32(MXFlash::flash_ctrl_addr + FLSH_CN);

  if ((real_value & FLSH_CN_UNLOCK_MASK) != FLSH_CN_UNLOCK_VALUE) {
    ALERT("FLASH UNLOCK", "FLASH CTRL REG = 0x%08x", ctrl_val);
    ALERT("FLASH UNLOCK", "FLASH LOCK REG = 0x%08x",
          real_value & FLSH_CN_UNLOCK_MASK);
    throw runtime_error("Unable to unlock the flash");
  }
}

void MXFlash::load_stub(void) {

  uint32_t address = this->stub_base;
  uint32_t read;

  INFO("FLASH", "Loading stub driver into SRAM memory at address 0X%08x", this->stub_base);

  for (auto const &word : stub) {

    sys->write_u32(word, address);

    read = sys->read_u32(address);

    if (word != read) {
      ALERT("FLASH", "Expected value 0x%08x at 0x%08x but read 0x%08x", word,
            address, read);
      throw std::runtime_error("Unable to load stub. SRAM write failed");
    }

    address += 4;
  }

  stub_loaded = true;
}

void MXFlash::load_buffer(std::vector<uint32_t> buffer) {

  uint32_t read;
  uint32_t address = this->stub_base;

  INFO("FLASH", "Loading buffer into SRAM memory at address 0X%08x", this->buffer_base);

  for (auto const &word : buffer) {

    sys->write_u32(word, address);

    read = sys->read_u32(address);

    if (word != read) {
      ALERT("FLASH", "Expected value 0x%08x at 0x%08x but read 0x%08x", word,
            address, read);
      throw std::runtime_error("Unable to load buffer. SRAM write failed");
    }

    break;
    address += 4;
  }
}

void MXFlash::set_stub_args(uint32_t src_addr, uint32_t src_size,
                            uint32_t dst_addr, uint32_t dst_size) {

  sys->write_reg(0, 0x3);

  // sys->write_reg(1, 0x3);
  //
  // sys->write_reg(2, dst_addr);
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  //
  // sys->write_reg(3, dst_size);
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}
}
