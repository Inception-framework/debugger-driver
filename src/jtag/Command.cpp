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

uint8_t* Command::get_buffer () {
  return (uint8_t*)this->buffer.data ();
}

uint32_t Command::size () {
  return this->buffer.size ();
}

void Command::add_command (uint32_t tms, uint32_t tdi, uint32_t trst, uint32_t srst) {

  uint8_t byte = 0;

  byte += (tms << 0);
  byte += (tdi << 1);
  byte += (trst << 2);
  byte += (srst << 3);

  this->buffer.push_back (byte);
}


void Command::set_type (COMMAND_TYPE new_type) {
        this->type = new_type;
}

void Command::move_to (tap_state_t state) {

        int i;
        int tms = 0;
        uint8_t byte;

        uint8_t tms_scan = Jtag::tap_get_tms_path(Jtag::current_state, state);

        uint8_t tms_scan_bits = Jtag::tap_get_tms_path_len(Jtag::current_state, state);

        for (i = 0; i < tms_scan_bits; i++) {

                byte = tms_scan & (1u << i) ? (1 << 0) : 0;
                this->buffer.push_back (byte);
        }

        Jtag::current_state = state;
}

void Command::write_ir (uint8_t ir) {

        uint32_t j;
        uint8_t byte;

        for(j=0; j<4; j++) {

            byte = 0;
            byte += ir & (1u << j) ? (1 << 1) : 0;

            if( j == 3 )
              byte += (1 << 0); // set tms

            this->buffer.push_back (byte);
        }

        this->add_command (0, 0, 0, 0);

        Jtag::current_state = TAP_IRPAUSE;
}

void Command::write_dr (uint32_t dr) {

        uint32_t j;
        uint8_t byte;

        for(j=0; j<32; j++) {

            byte = 0;
            byte += dr & (1u << j) ? (1 << 1) : 0;

            if( j == 31 )
              byte += (1 << 0); // set tms

            this->buffer.push_back (byte);
        }

        this->add_command (0, 0, 0, 0);

        Jtag::current_state = TAP_DRPAUSE;
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
