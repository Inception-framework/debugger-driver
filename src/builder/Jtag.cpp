/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: Jtag.cpp
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

#include "Jtag.h"

namespace jtag {

tap_state_t Jtag::current_state = TAP_RESET;

Jtag::Jtag() {}

Jtag::~Jtag() {}

int Jtag::tap_get_tms_path(tap_state_t from, tap_state_t to) {
  return tms_seqs[tap_move_ndx(from)][tap_move_ndx(to)].bits;
}

int Jtag::tap_get_tms_path_len(tap_state_t from, tap_state_t to) {
  return tms_seqs[tap_move_ndx(from)][tap_move_ndx(to)].bit_count;
}

int Jtag::tap_move_ndx(tap_state_t state) {

  int ndx = -1;

  switch (state) {
  case TAP_RESET:
    ndx = 0;
    break;
  case TAP_IDLE:
    ndx = 1;
    break;
  case TAP_DRSHIFT:
    ndx = 2;
    break;
  case TAP_DRPAUSE:
    ndx = 3;
    break;
  case TAP_IRSHIFT:
    ndx = 4;
    break;
  case TAP_IRPAUSE:
    ndx = 5;
    break;
  default:
    ALERT("main", "Requested state not found in tap_move_ndx()");
  }

  return ndx;
}

const char *Jtag::tap_state_name(tap_state_t state) {

  unsigned int i;

  for (i = 0; i < ARRAY_SIZE(tap_name_mapping); i++) {
    if (tap_name_mapping[i].symbol == state)
      return tap_name_mapping[i].name;
  }
  return "???";
}
}
