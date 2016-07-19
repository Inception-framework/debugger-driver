/*
 * CommandsFactory.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "CommandsFactory.h"

CommandsFactory::CommandsFactory () {

}

CommandsFactory::~CommandsFactory () {
}

jtag::Command* CommandsFactory::CreateCommand (COMMAND_TYPE type, int address) {

        jtag::Command* cmd = new jtag::Command(type);

        switch(type) {

        case RESET:
                /* The Test-Logic-Reset state can also be entered synchronously
                   from any state by a sequence of five TCK cycles with DBGTMS HIGH.*/

                for(int i=0; i<5; i++)
                        cmd->add_command(1,0,0,0);

                break;
//
//    case READ_BYTE:
//
//      cmd->move_state(STATE_IR);
//
//      cmd->write_ir(APACC);
//
//      cmd->set_type(JTAG::READ_BYTE);
//
//    break;
//
//    case READ_WORD:
//
//    cmd->set_type(JTAG::READ_WORD);
//
//    break;
//
//    case READ_HALFWORD:
//
//    cmd->set_type(JTAG::READ_HALFWORD);
//
//    break;
//
//    case WRITE_BYTE:
//
//    cmd->set_type(JTAG::WRITE_BYTE);
//
//    break;
//
//    case WRITE_WORD:
//
//    cmd->set_type(JTAG::WRITE_WORD);
//
//    break;
//
//    case WRITE_HALFWORD:
//
//    cmd->set_type(JTAG::WRITE_HALFWORD);
//
//    break;
//
        case IDCODE:
                /*
                 * The IDCODE command :
                 *
                 */
                cmd->move_to(jtag::TAP_IRSHIFT);

                cmd->write_ir(0xE); //1110 = IDCODE IR

                //cmd->move_to(jtag::TAP_DRSHIFT);
                cmd->add_command(0,0,0,0);
                cmd->add_command(1,0,0,0);
                cmd->add_command(1,0,0,0);
                cmd->add_command(1,0,0,0);
                cmd->add_command(0,0,0,0);
                cmd->add_command(0,0,0,0);

                for(int i=0; i<32; i++)
                        cmd->add_command(0,0,0,0);

                cmd->add_command(1,0,0,0);
                cmd->add_command(0,0,0,0);
                cmd->add_command(1,0,0,0);
                cmd->add_command(1,0,0,0);
                cmd->add_command(1,0,0,0);

                break;
        }

        return cmd;

}
