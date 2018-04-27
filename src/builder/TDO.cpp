/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: TDO.cpp
    @Last modified by:   noname
    @Last modified time: 16-Mar-2017
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

#include "builder/TDO.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#define _LOG_ALL
#include "colored.h"

namespace jtag {

TDO::TDO() {}

TDO::~TDO() {

  for (unsigned int i = 0; i < size(); i++)
    delete at(i);
}

void TDO::add(uint32_t begin, uint32_t end) {

  push_back(new Pair{begin, end});
}

} /* namespace JTAG */
