#include "TDO.h"

namespace jtag {

TDO::TDO() {}

TDO::~TDO() {}

void TDO::add(uint32_t begin, uint32_t end) {

  this->push_back(new Pair{begin, end});
}

} /* namespace JTAG */
