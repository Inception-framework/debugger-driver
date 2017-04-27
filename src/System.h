/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: System.h
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

#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

#include "trace/Trace.h"
#include "main.h"

#include "test/TestsFactory.h"
#include "drivers/flash/flash.h"

#include "decoders/Decoder.h"
#include "decoders/inception/InceptionDecoder.h"
#include "decoders/jtag/JTAGDecoder.h"

namespace flash {
class Flash;
}

class System {

public:
  System();

  ~System();

  void select_protocol(JTAG_PROTOCOL protocol);

  void stop();

  std::string info();

  void reset();

  uint32_t read_u32(uint32_t address);

  void write_u32(uint32_t value, uint32_t address);

  void check_up(void);

  void check(Test test);
  // void read_chunck(uint32_t address, std::vector<char> buffer);
  // void write_chunck(uint32_t address, std::vector<char> buffer);

  void synchrone_process(jtag::Command *command, uint64_t *value);

  void write_reg(uint32_t reg_id, uint32_t value);

  uint32_t read_reg(uint32_t reg_id);

  // void execute(uint32_t address);

  flash::Flash *get_flash();

  uint32_t idcode;

  bool halted;

  void load_binary_in_sdram(std::string file_path, uint32_t address);

private:

  JTAG_PROTOCOL protocol;

  Device::USBDevice *fx3_jtag;



  Producer *producer;

  Decoder *decoder;

  flash::Flash *flash;

  void halt();
};

#endif
