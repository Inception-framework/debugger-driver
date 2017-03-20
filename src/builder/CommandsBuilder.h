/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: CommandsBuilder.h
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

#ifndef COMMANDS_BUILDER_H
#define COMMANDS_BUILDER_H

#include <cstdint>

namespace jtag {

class Command;

/*
* This class is an interface for JTAG commands builder
*/
class CommandsBuilder {

public:
  CommandsBuilder(){};

  ~CommandsBuilder(){};

  virtual Command *write(uint32_t address, uint32_t data) = 0;

  virtual Command *read(uint32_t address) = 0;

  virtual Command *reset() = 0;

  virtual Command *idcode() = 0;

  virtual Command *init() = 0;
};
}

#endif
