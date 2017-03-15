/**
* @Author: Nassim
* @Date:   2016-11-21T15:47:35+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:49:57+01:00
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

  int8_t decode(uint64_t *value, bool idcode);

  void resize(uint32_t new_size);

  uint8_t* tdo_ptr();

private:

  int8_t decode_recursivly(uint32_t position, uint64_t *value);

  TDO_PACKET *to_int(uint32_t position);

  bool check_ack(uint8_t ack);

  std::vector<uint8_t> data;

};

} /* namespace JTAG */

#endif /* TDO_H_ */
