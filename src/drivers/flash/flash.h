/*
* Flash Driver Interface
*/

#ifndef FLASH_H
#define FLASH_H

#include <string>

#include <iostream>
#include <stdint.h>
#include <vector>

namespace flash {

class FlashBank {
public:
  FlashBank(uint32_t base, uint32_t size, uint32_t bank_number)
      : base(base), size(size), bank_number(bank_number){};

  uint32_t base;

  uint32_t size;

  uint32_t bank_number;
};

class Flash {

public:
  Flash(std::string name, uint32_t size, uint32_t base, uint32_t pages_amount);

  ~Flash();

  std::string name;

  virtual void write(uint32_t address, std::vector<uint32_t> buffer) = 0;

  // virtual void erase() = 0;
  //
  // virtual void protect(uint32_t set, uint32_t first, uint32_t last) = 0;
  //
  // virtual void write(std::vector<char> buffer, uint32_t offset,
  //                    uint32_t count) = 0;
  //
  // virtual void read(std::vector<char> buffer, uint32_t offset,
  //                   uint32_t count) = 0;
  //
  // virtual void probe() = 0;

  std::string info();

protected:
  uint32_t size;

  uint32_t base;

  uint32_t pages_amount;

  uint32_t stub_base;

  uint32_t buffer_base;

  std::vector<FlashBank *> banks;
};
}
#endif
