/*
 * CommandsFactory.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef JTAG_COMMANDSFACTORY_H_
#define JTAG_COMMANDSFACTORY_H_

#define _LOG_ALL
#include "../colored.h"

#include "Command.h"

#include <iostream>
#include <vector>

class CommandsFactory {
public:
  CommandsFactory();
  virtual ~CommandsFactory();

  static jtag::Command *CreateCommand(COMMAND_TYPE type,
                                      std::vector<uint32_t> &argv);

private:
  static bool check_arg(std::vector<uint32_t> &argv, uint32_t required);

  static void select(jtag::Command *cmd, uint32_t bank_id);

  static void read_u32(jtag::Command *cmd, uint32_t address);

  static void write_u32(jtag::Command *cmd, uint32_t address, uint32_t data);

  static bool first_io;

};

#endif /* JTAG_COMMANDSFACTORY_H_ */
