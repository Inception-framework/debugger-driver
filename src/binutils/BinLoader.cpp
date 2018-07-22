/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: System.cpp
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

#include "binutils/BinLoader.h"
#include <string>
#include <fstream>

BinLoader::BinLoader(std::string source) : ifs(source.c_str(), std::ifstream::binary) {

  source = source;

  if (! ifs.is_open()) {
    ALERT("BinLoader", "Unable to open Binary file : %s ", source.c_str());
    throw std::runtime_error("Unable to open binary file\n");
  }

  ifs.seekg (0, ifs.beg);
}

BinLoader::~BinLoader() {
  ifs.close();
}

uint32_t BinLoader::next() {

  uint32_t val;

  if( ! ifs.eof() )
    ifs.read((char*) (&val), sizeof(val));
  else
    throw std::runtime_error("End of binary file\n");

  INFO("BIN_LOADED", "0x%08x", val);

  return val;
}

// int main(int argc, char** argv) {
//
//   BinLoader* bin_loader = new BinLoader("/home/noname/workspacees06/stub/SLA/stub.bin");
//
//   try {
//
//     while(true)
//       bin_loader->next();
//
//   } catch (std::runtime_error) {
//
//     INFO("BIN_LOADER", "End Of File");
//   }
// }
