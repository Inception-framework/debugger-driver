/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: flash.cpp
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

#include "flash.h"

#include <iostream>
#include <sstream>

namespace flash {

Flash::Flash(std::string name, uint32_t size, uint32_t base,
             uint32_t pages_amount)
    : size(size), base(base), pages_amount(pages_amount) {

  FlashBank *bank = NULL;

  uint32_t bank_base = 0;
  uint32_t bank_size = size / pages_amount;

  for (auto i = 0; i < pages_amount; i++) {

    bank_base += (i * bank_size);

    bank = new FlashBank(bank_base, bank_size, i);

    this->banks.push_back(bank);
  }
}

Flash::~Flash() {}

std::string Flash::info() {

  std::stringstream stdout;

  stdout << "\r\n\t\t Base address     : 0x" << std::hex << base;
  stdout << "\r\n\t\t Size address     : 0x" << std::hex << size;
  stdout << "\r\n\t\t Amount of Pages  : 0x" << std::hex << pages_amount;

  return stdout.str();
}
}
