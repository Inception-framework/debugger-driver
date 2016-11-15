#include "flash.h"

#include <iostream>
#include <sstream>

namespace flash {

Flash::Flash(std::string name, uint32_t size, uint32_t base,
             uint32_t pages_amount)
    : size(size), base(base), pages_amount(pages_amount) {

  FlashBank *bank = NULL;

  uint32_t bank_base = 0;
  uint32_t bank_size = size / pages_amount;

  for (auto i = 0; i < pages_amount; i++) {

    bank_base += (i * bank_size);

    bank = new FlashBank(bank_base, bank_size, i);

    this->banks.push_back(bank);
  }
}

Flash::~Flash() {}

std::string Flash::info() {

  std::stringstream stdout;

  stdout << "\r\n\t\t Base address     : 0x" << std::hex << base;
  stdout << "\r\n\t\t Size address     : 0x" << std::hex << size;
  stdout << "\r\n\t\t Amount of Pages  : 0x" << std::hex << pages_amount;

  return stdout.str();
}
}
