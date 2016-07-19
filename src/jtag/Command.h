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
								RESET=0,
								READ_BYTE=1,
								READ_WORD=2,
								READ_HALFWORD =3,
								WRITE_BYTE=4,
								WRITE_WORD=5,
								WRITE_HALFWORD=6,
								IDCODE=7
}COMMAND_TYPE;

static const struct command_name_mapping {
								enum COMMAND_TYPE symbol;
								const char *name;
}
command_name_mapping[] = {
								{ RESET, "RESET", },
								{ READ_BYTE, "READ_BYTE", },
								{ READ_WORD, "READ_WORD", },
								{ READ_HALFWORD, "READ_HALFWORD", },
								{ WRITE_BYTE, "WRITE_BYTE", },
								{ WRITE_WORD, "WRITE_WORD", },
								{ WRITE_HALFWORD, "WRITE_HALFWORD", },
								{ IDCODE, "IDCODE", },
};

namespace jtag {

class Command {
public:
								Command (COMMAND_TYPE type);

								virtual ~Command ();

								void add_command (uint32_t tms, uint32_t tdi, uint32_t trst=0, uint32_t srst=0);

								void move_to (tap_state_t state);

								void write_ir (uint8_t ir);

								const char* command_name ();

								uint8_t* get_buffer ();

								uint32_t size ();

private:
								void set_type (COMMAND_TYPE type);

								std::vector<uint8_t> buffer;

								COMMAND_TYPE type;

};

} /* namespace JTAG */

#endif /* JTAG_COMMAND_H_ */
