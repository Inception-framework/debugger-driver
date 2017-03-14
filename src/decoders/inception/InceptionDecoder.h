/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: InceptionDecoder.h
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

#ifndef INCEPTION_DECODER_H_
#define INCEPTION_DECODER_H_

#include "../../interface/Interface.h"

#include "../../builder/Command.h"
#include "../Decoder.h"

class Producer;

#define _LOG_ALL
#include "../../colored.h"

class InceptionDecoder : public Decoder{

public:
  InceptionDecoder(Producer *new_producer);

  ~InceptionDecoder();

  int32_t process(jtag::Command *cmd, uint64_t *value);

private:

  Producer *producer;
};

#endif /* DECODER_H_ */
