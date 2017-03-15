/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: flash.h
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

#ifndef FLASH_H
#define FLASH_H

#include <string>

#include <iostream>
#include <stdint.h>
#include <vector>

namespace flash {

class FlashBank {
public:
  FlashBank(uint32_t base, uint32_t size, uint32_t bank_number)
      : base(base), size(size), bank_number(bank_number){};

  uint32_t base;

  uint32_t size;

  uint32_t bank_number;
};

class Flash {

public:
  Flash(std::string name, uint32_t size, uint32_t base, uint32_t pages_amount);

  ~Flash();

  std::string name;

  virtual void write(uint32_t address, std::vector<uint32_t> buffer) = 0;

  // virtual void erase() = 0;
  //
  // virtual void protect(uint32_t set, uint32_t first, uint32_t last) = 0;
  //
  // virtual void write(std::vector<char> buffer, uint32_t offset,
  //                    uint32_t count) = 0;
  //
  // virtual void read(std::vector<char> buffer, uint32_t offset,
  //                   uint32_t count) = 0;
  //
  // virtual void probe() = 0;

  std::string info();

protected:
  uint32_t size;

  uint32_t base;

  uint32_t pages_amount;

  uint32_t stub_base;

  uint32_t buffer_base;

  std::vector<FlashBank *> banks;
};
}
#endif
