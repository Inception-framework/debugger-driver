#include "TDO.h"

namespace jtag {

TDO::TDO() {}

TDO::~TDO() {

  for (int i = 0; i < this->size(); i++)
    delete this->at(i);
}

void TDO::add(uint32_t begin, uint32_t end) {

  this->push_back(new Pair{begin, end});
}

} /* namespace JTAG */
