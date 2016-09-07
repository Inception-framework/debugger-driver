/*
 * Command.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef TDO_H_
#define TDO_H_

#include <iostream>
#include <stdint.h>
#include <vector>

namespace jtag {

typedef struct pair {
  uint32_t begin;
  uint32_t end;
} Pair;

typedef struct tdo_packet {
  uint8_t ack;
  uint64_t data_in;
} TDO_PACKET;


class TDO : public std::vector<Pair *> {
public:
  TDO();

  virtual ~TDO();

  void add(uint32_t begin, uint32_t end);

  int8_t decode(uint64_t *value);

  void resize(uint32_t new_size);

  uint8_t* tdo_ptr();

private:

  int8_t decode_recursivly(uint32_t position, uint64_t *value);

  TDO_PACKET *tdo_to_int(uint32_t position);

  bool check_ack(uint8_t ack);

  std::vector<uint8_t> data;

};

} /* namespace JTAG */

#endif /* TDO_H_ */
