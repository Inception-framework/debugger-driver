/*
* Author : Corteggiani Nassim
* Contact: nassim.corteggiani@maximintegrated.com
*/
#ifndef COMMANDS_BUILDER_H
#define COMMANDS_BUILDER_H

#include <cstdint>

namespace jtag {

class Command;

/*
* This class is an interface for JTAG commands builder
*/
class CommandsBuilder {

public:

	CommandsBuilder() {};

	~CommandsBuilder() {};

	virtual Command* write (uint32_t data, uint32_t address) = 0;

	virtual Command* read  (uint32_t address) = 0;

	virtual Command* reset () = 0;

	virtual Command* idcode () = 0;

	virtual Command* init() = 0;

};

}

#endif
