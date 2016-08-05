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

class TDO : public std::vector<Pair *> {
public:
  TDO();

  virtual ~TDO();

  void add(uint32_t begin, uint32_t end);
};

} /* namespace JTAG */

#endif /* TDO_H_ */
