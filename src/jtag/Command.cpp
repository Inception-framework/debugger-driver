/*
 * Command.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "Command.h"

namespace jtag {


Command::Command (COMMAND_TYPE type) {

        this->type = type;

}

Command::~Command () {
        // TODO Auto-generated destructor stub
}

void Command::add_command (uint32_t tms, uint32_t tdi, uint32_t trst, uint32_t srst) {

  uint8_t byte = 0;

  byte += (tms << 0);
  byte += (tdi << 1);
  byte += (trst << 2);
  byte += (srst << 3);

  this->buffer.push_back(byte);
}


void Command::set_type (COMMAND_TYPE new_type) {
        this->type = new_type;
}

void Command::move_to (tap_state_t state) {

        int i;
        int tms = 0;
        int bit;

        uint8_t tms_scan = Jtag::tap_get_tms_path(Jtag::current_state, state);

        uint8_t tms_scan_bits = Jtag::tap_get_tms_path_len(Jtag::current_state, state);

        for (i = 0; i < tms_scan_bits; i++) {

                bit = (tms_scan >> i) & 1 ? (1 << 0) : 0;
                this->buffer.push_back(tms);
        }

}

void Command::write_ir (uint16_t ir) {

        uint32_t j;

        for(j=0; j<16; j++) //For each bit as byte
                this->buffer.push_back( ir & (1u << j) ? (1 << 1) : 0);

}

const char* Command::command_name()
{

	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(command_name_mapping); i++) {
		if (command_name_mapping[i].symbol == this->type)
			return command_name_mapping[i].name;
	}
	return "???";
}

} /* namespace JTAG */
