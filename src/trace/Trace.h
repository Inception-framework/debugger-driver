/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: Trace.h
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

#ifndef TRACE_H
#define TRACE_H

#include <chrono>
#include <future>

#include "../device/Device.h"

class Trace {

public:
  Trace(Device::USBDevice *device);

  ~Trace();

  void start();

  void stop();

  void run();

private:
  // std::future task;
  
  bool stopped;

  Device::USBDevice *device;
};

#endif
