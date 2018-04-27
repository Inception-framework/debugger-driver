/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: CommandsFactory.h
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

#ifndef JTAG_COMMANDSFACTORY_H_
#define JTAG_COMMANDSFACTORY_H_

#define _LOG_ALL
#include "../colored.h"

#include "Command.h"

#include "CommandsBuilder.h"

#include <iostream>
#include <vector>
#include <stdexcept>

typedef enum JTAG_PROTOCOL {
  SWD = 0,
  JTAG = 1,
  INCEPTION = 2,
  FILEB = 3,
} JTAG_PROTOCOL;

class CommandsFactory {

public:
  CommandsFactory();

  virtual ~CommandsFactory();

  static jtag::Command *CreateCommand(COMMAND_TYPE type,
                                      std::vector<uint32_t> &argv);

  static void initProtocol(JTAG_PROTOCOL protocol);

private:
  static JTAG_PROTOCOL protocol;

  static jtag::CommandsBuilder *builder;

private:
  static bool check_arg(std::vector<uint32_t> &argv, uint32_t required);
};

#endif /* JTAG_COMMANDSFACTORY_H_ */
