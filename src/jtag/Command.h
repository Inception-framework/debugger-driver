/*
 * Command.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef JTAG_COMMAND_H_
#define JTAG_COMMAND_H_

#include <iostream>
#include <stdint.h>
#include <vector>

#include "Jtag.h"
#include "TDO.h"

typedef enum COMMAND_TYPE {
  RESET = 0,
  IDCODE = 1,
  WRITE_U32 = 2,
  READ_U32 = 3,
  SELECT = 4,
  ACTIVE = 5,
  EXIT = 6,
} COMMAND_TYPE;

static const struct command_name_mapping {
  enum COMMAND_TYPE symbol;
  const char *name;
} command_name_mapping[] = {
    {
        RESET, "RESET",
    },
    {
        READ_U32, "READ U32",
    },
    {
        WRITE_U32, "WRITE U32",
    },
    {
        IDCODE, "IDCODE",
    },
    {
        ACTIVE, "ACTIVE",
    },
    {
        SELECT, "SELECT",
    },
    {EXIT, "EXIT"},
};

namespace jtag {

class Command {
public:
  Command(COMMAND_TYPE type);

  virtual ~Command();

  void add_command(uint32_t tms, uint32_t tdi, uint32_t trst = 0,
                   uint32_t srst = 0);

  void move_to(tap_state_t state);

  void write_ir(uint8_t ir);

  void write_dr(uint8_t RnW, uint8_t address, uint32_t datain);

  const char *command_name();

  COMMAND_TYPE get_type();

  uint8_t *get_out_buffer();

  uint8_t *get_in_buffer();

  uint64_t *get_answers();

  uint32_t size();

  void wait(uint32_t cycles);

  int32_t again();

  TDO *get_tdo();

  COMMAND_TYPE type;

  void add_answer(uint64_t answer);

private:
  void set_type(COMMAND_TYPE type);

  std::vector<uint8_t> out_buffer;

  std::vector<uint8_t> in_buffer;

  std::vector<uint64_t> answers;

  uint32_t attempts;

  TDO tdo;
};

} /* namespace JTAG */

#endif /* JTAG_COMMAND_H_ */
