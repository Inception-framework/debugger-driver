#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "jtag/ap/AccessPort.h"
#include "main.h"

#include "TestsFactory.h"
#include "drivers/flash/flash.h"

namespace flash {
class Flash;
}

class System {

public:
  System();

  ~System();

  std::string info();

  uint32_t read_u32(uint32_t address);

  void write_u32(uint32_t value, uint32_t address);

  void check_up(void);

  void check(Test test);
  // void read_chunck(uint32_t address, std::vector<char> buffer);
  // void write_chunck(uint32_t address, std::vector<char> buffer);

  void synchrone_process(jtag::Command *command, uint64_t *value);

  void write_reg(uint32_t reg_id, uint32_t value);

  uint32_t read_reg(uint32_t reg_id);

  // void execute(uint32_t address);

  flash::Flash* get_flash();

  std::string idcode;

  bool halted;

private:
  Device::USBDevice *fx3;

  Producer *producer;

  Decoder *decoder;

  void init_jtag(void);

  bool is_initialized;

  jtag::AccessPort *ap;

  void select(void);

  void load_binary_in_sdram(std::string file_path);

  flash::Flash *flash;
};

#endif
