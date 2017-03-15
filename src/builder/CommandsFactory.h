/*
 * CommandsFactory.h
 *
 *  Created on: Jul 12, 2016
 *      Author: Nassim Corteggiani
 */

#ifndef JTAG_COMMANDSFACTORY_H_
#define JTAG_COMMANDSFACTORY_H_

#define _LOG_ALL
#include "../colored.h"

#include "Command.h"

#include "CommandsBuilder.h"

#include <iostream>
#include <vector>

typedef enum JTAG_PROTOCOL {
  SWD = 0,
  JTAG = 1,
  INCEPTION = 2,
} JTAG_PROTOCOL;

class CommandsFactory {

public:

  CommandsFactory();

  virtual ~CommandsFactory();

  static jtag::Command *CreateCommand(COMMAND_TYPE type,
                                      std::vector<uint32_t> &argv);

  static void initProtocol(JTAG_PROTOCOL protocol);

private:

  static JTAG_PROTOCOL protocol;

  static jtag::CommandsBuilder* builder;

private:

  static bool check_arg(std::vector<uint32_t> &argv, uint32_t required);

};

#endif /* JTAG_COMMANDSFACTORY_H_ */
