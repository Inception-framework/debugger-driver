/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: JTAGBuilder.h
    @Last modified by:   Corteggiani
    @Last modified time: 21-Mar-2017
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



#ifndef FILE_BUILDER_H
#define FILE_BUILDER_H

#include "../CommandsBuilder.h"
#include "../Jtag.h"

namespace jtag {

class FileBuilder : public CommandsBuilder {

public:
  FileBuilder();

  ~FileBuilder();

  jtag::Command *write(uint32_t addres, uint32_t data);

  jtag::Command *read(uint32_t address);

  jtag::Command *reset();

  jtag::Command *idcode();

  jtag::Command *init();

private:

  void process(jtag::Command* cmd);

  void process_inputs(char* buffer, unsigned size, jtag::Command* cmd);

  void add_command(jtag::Command *cmd, uint32_t tms, uint32_t tdi,
                                uint32_t trst, uint32_t srst);
};
}

#endif
