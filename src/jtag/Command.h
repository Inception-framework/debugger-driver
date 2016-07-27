/*
 * Command.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef JTAG_COMMAND_H_
#define JTAG_COMMAND_H_

#include <stdint.h>
#include <iostream>
#include <vector>

#include "Jtag.h"

typedef enum COMMAND_TYPE {
								RESET     = 0,
								IDCODE    = 1,
								WRITE_U32 = 2,
								READ_U32  = 3,
								SELECT    = 4,
								ACTIVE    = 5,
}COMMAND_TYPE;

static const struct command_name_mapping {
								enum COMMAND_TYPE symbol;
								const char *name;
}
command_name_mapping[] = {
								{ RESET, "RESET", },
								{ READ_U32, "READ U32", },
								{ WRITE_U32, "WRITE U32", },
								{ IDCODE, "IDCODE", },
								{ ACTIVE, "ACTIVE"},
};

namespace jtag {

class Command {
public:
								Command (COMMAND_TYPE type);

								virtual ~Command ();

								void add_command (uint32_t tms, uint32_t tdi, uint32_t trst=0, uint32_t srst=0);

								void move_to (tap_state_t state);

								void write_ir (uint8_t ir);

								void write_dr (uint8_t RnW, uint8_t address, uint32_t datain);

								const char* command_name ();

								uint8_t* get_buffer ();

								uint32_t size ();

								void wait (uint32_t cycles);

private:
								void set_type (COMMAND_TYPE type);

								std::vector<uint8_t> buffer;

								COMMAND_TYPE type;

								static uint32_t tdo;
};

} /* namespace JTAG */

#endif /* JTAG_COMMAND_H_ */
