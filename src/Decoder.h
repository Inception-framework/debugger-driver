/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: Decoder.h
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

#ifndef DECODER_H_
#define DECODER_H_

#include "interface/Interface.h"

#include "builder/Command.h"

class Producer;

#include <mutex>
#include <queue>
#include <thread>

#define _LOG_ALL
#include "colored.h"

class Decoder /*: public Interface*/ {

public:
  Decoder(Producer *producer);

  virtual ~Decoder();

  void start();

  void stop();

  /*
  * Thread safe method use by Consumer object to assign TDO outputs to the
  * expected Command object.
  * The Decoder checks JTAG ack and ask the Producer to send the command again
  * if a wait response occurs
  */
  void add_cmd_to_queue(jtag::Command *cmd);

  int32_t process_jtag_queue(uint64_t *);

  int32_t process(jtag::Command *cmd, uint64_t *value);

private:
  bool check_ack(uint8_t data);

  std::queue<jtag::Command *> queue;

  std::thread task;

  bool is_running;

  Producer *producer;

  std::mutex locker;
};

#endif /* DECODER_H_ */
