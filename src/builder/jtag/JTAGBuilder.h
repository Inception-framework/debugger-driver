#ifndef JTAG_BUILDER
#define JTAG_BUILDER

#include <cstdint>

#include "../CommandsBuilder.h"
#include "../Jtag.h"

namespace jtag {

// class CommandsBuilder;

class JTAGBuilder : public CommandsBuilder {

public:

	JTAGBuilder();

	~JTAGBuilder();

	jtag::Command* write(uint32_t data, uint32_t address);

	jtag::Command* read(uint32_t address);

	jtag::Command* reset();

	jtag::Command* idcode();

	jtag::Command* init();

private:

	void wait(jtag::Command *cmd, uint32_t cycles);

	void add_command(jtag::Command *cmd, uint32_t tms, uint32_t tdi, uint32_t trst = 0,
									 uint32_t srst = 0);

	void move_to(jtag::Command *cmd, tap_state_t state);

	void write_ir(jtag::Command *cmd, uint8_t ir);

	void write_dr(jtag::Command *cmd, uint8_t RnW, uint8_t address, uint32_t datain);

	void active(jtag::Command *cmd);

	void select(jtag::Command *cmd, uint32_t bank_id);

	bool first_io;
};

}

#endif
