/**
* @Author: Nassim
* @Date:   2017-03-14T12:54:18+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:50:17+01:00
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
