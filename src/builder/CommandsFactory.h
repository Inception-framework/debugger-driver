/**
* @Author: Nassim
* @Date:   2017-03-15T10:14:13+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:50:08+01:00
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
