/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: Command.h
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

#ifndef JTAG_COMMAND_H_
#define JTAG_COMMAND_H_

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "Jtag.h"
#include "TDO.h"

typedef enum COMMAND_TYPE {
  RESET = 0,
  IDCODE = 1,
  WRITE = 2,
  READ = 3,
  // SELECT = 4,
  // ACTIVE = 5,
  EXIT = 6,
  TRACE = 7,
  UNTRACE = 8,
  SET_AP_AHB_CSW = 9,
  WRITE_MULTI = 10,
  READ_CSW = 11,
  DAP_ABORT = 12,
  CONTROL = 13
} COMMAND_TYPE;

static const struct command_name_mapping {
  enum COMMAND_TYPE symbol;
  const char *name;
} command_name_mapping[] = {
    // {
    //     RESET, "RESET",
    // },
    {
     READ, "READ",
    },
    {
     WRITE, "WRITE",
    },
    {
     IDCODE, "IDCODE",
    },
    // {
    //     ACTIVE, "ACTIVE",
    // },
    // {
    //     SELECT, "SELECT",
    // },
    {EXIT, "EXIT"},
    {EXIT, "TRACE"},
    {EXIT, "UNTRACE"},
    {DAP_ABORT, "DAP_ABORT"},
    {READ_CSW, "READ_CSW"},
    {WRITE_MULTI, "WRITE_MULTI"},
    {CONTROL, "CONTROL"}
};

namespace jtag {

class Command {
public:
  Command(COMMAND_TYPE new_type);

  virtual ~Command();

  const char *command_name(void);

  COMMAND_TYPE get_type(void);

  uint8_t *get_out_buffer(void);

  uint32_t get_out_buffer_size(void);

  uint8_t *get_in_buffer(void);

  uint32_t get_in_buffer_size(void);

  void push_back(uint8_t data_in);

  void push_back(uint32_t data_in);

  uint8_t again(void);

  void add_tdo(uint64_t start, uint64_t end);

  COMMAND_TYPE type;

  TDO tdo;

private:

  void set_type(COMMAND_TYPE type);

  std::vector<uint8_t> out_buffer;

  std::vector<uint8_t> in_buffer;

  uint8_t attempts;
};

} /* namespace JTAG */

#endif /* JTAG_COMMAND_H_ */
