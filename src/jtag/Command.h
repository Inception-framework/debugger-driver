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
  WRITE = 2,
  READ = 3,
  // SELECT = 4,
  // ACTIVE = 5,
  EXIT = 6,
  TRACE = 7,
  UNTRACE = 8,
  SET_AP_AHB_CSW = 9,
  WRITE_MULTI = 10,
  READ_CSW = 11,
  DAP_ABORT = 12,
} COMMAND_TYPE;

static const struct command_name_mapping {
  enum COMMAND_TYPE symbol;
  const char *name;
} command_name_mapping[] = {
    // {
    //     RESET, "RESET",
    // },
    {
        READ, "READ",
    },
    {
        WRITE, "WRITE",
    },
    {
        IDCODE, "IDCODE",
    },
    // {
    //     ACTIVE, "ACTIVE",
    // },
    // {
    //     SELECT, "SELECT",
    // },
    {EXIT, "EXIT"},
    {EXIT, "TRACE"},
    {EXIT, "UNTRACE"},
    {DAP_ABORT, "DAP_ABORT"},
    {READ_CSW, "READ_CSW"},
    {WRITE_MULTI, "WRITE_MULTI"}
};

namespace jtag {

class Command {
public:
  Command(COMMAND_TYPE type);

  virtual ~Command();

  const char *command_name(void);

  COMMAND_TYPE get_type(void);

  uint8_t *get_out_buffer(void);

  uint8_t *get_in_buffer(void);

  void push_back(uint8_t data_in);

  uint32_t size(void);

  uint8_t again(void);

  int8_t decode(uint64_t *value);

  void add_tdo(uint64_t start, uint64_t end);

  COMMAND_TYPE type;

private:
  void set_type(COMMAND_TYPE type);

  std::vector<uint8_t> out_buffer;

  uint8_t ack;

  uint32_t datain;

  uint8_t attempts;

  TDO tdo;

};

} /* namespace JTAG */

#endif /* JTAG_COMMAND_H_ */
