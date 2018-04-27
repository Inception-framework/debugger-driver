/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: JTAGBuilder.h
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

#ifndef JTAG_BUILDER
#define JTAG_BUILDER

#include <cstdint>

#include "../CommandsBuilder.h"
#include "../Jtag.h"

namespace jtag {

// class CommandsBuilder;

class JTAGBuilder : public CommandsBuilder {

public:
  JTAGBuilder();

  ~JTAGBuilder();

  jtag::Command *write(uint32_t addres, uint32_t data);

  jtag::Command *read(uint32_t address);

  jtag::Command *reset();

  jtag::Command *idcode();

  jtag::Command *init();

  jtag::Command *control() {};

private:
  void wait(jtag::Command *cmd, uint32_t cycles);

  void add_command(jtag::Command *cmd, uint32_t tms, uint32_t tdi,
                   uint32_t trst = 0, uint32_t srst = 0);

  void move_to(jtag::Command *cmd, tap_state_t state);

  void write_ir(jtag::Command *cmd, uint8_t ir);

  void write_dr(jtag::Command *cmd, uint8_t RnW, uint8_t address,
                uint32_t datain);

  void active(jtag::Command *cmd);

  void select(jtag::Command *cmd, uint32_t bank_id);

  void print_dr(uint8_t RnW, uint8_t address, uint64_t datain);

  bool first_io;
};
}

#endif
