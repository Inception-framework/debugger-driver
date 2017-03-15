/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: Trace.cpp
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

#include "Trace.h"

Trace::Trace(Device::USBDevice *dev) : stopped(false), device(dev) {}

void Trace::start() { std::async(&Trace::run, this); }

void Trace::stop() {

  stopped = true;

  // task.get();
}

void Trace::run() {

  uint32_t size = 8;

  uint8_t buffer[8] = {0};

  // buffer[0] = 4;
  // device->download((uint8_t*)&buffer, &size);
  // buffer[0] = 0;

  while (true) {

    INFO("Trace", "Waiting Trace information");

    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    device->upload((uint8_t *)&buffer, &size);

    INFO("TRACE", "Received %d B", size);

    for (auto i = 0; i < size; i++) {
      printf("%d", buffer[i]);
    }
    printf("\n\r");
  }
}
