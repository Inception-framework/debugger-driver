/*
 * CommandsFactory.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef JTAG_COMMANDSFACTORY_H_
#define JTAG_COMMANDSFACTORY_H_

#include <iostream>

#include "Command.h"

class CommandsFactory {
public:
        CommandsFactory ();
        virtual ~CommandsFactory ();

        static jtag::Command* CreateCommand (COMMAND_TYPE type, int address);
};

#endif /* JTAG_COMMANDSFACTORY_H_ */
