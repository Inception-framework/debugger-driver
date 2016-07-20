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

/*
 *  The JTAG-DP register accessed depends on both:
   •       - The Instruction Register (IR) value for the DAP access
   •       - A[3:2] from the address field of the DAP acces
 *
 */
jtag::Command* CommandsFactory::CreateCommand (COMMAND_TYPE type, uint32_t datain, uint32_t address) {

        jtag::Command* cmd = new jtag::Command(type);
        uint32_t data, tar_value, csw_value;

        switch(type) {

        case RESET:
                /* The Test-Logic-Reset state can also be entered synchronously
                   from any state by a sequence of five TCK cycles with DBGTMS HIGH.*/

                for(int i=0; i<5; i++)
                        cmd->add_command(1,0,0,0);

                break;

        case WRITE_U32:

                // Set the correct JTAG-DP
                cmd->move_to(jtag::TAP_IRSHIFT);
                cmd->write_ir(0xA); //1010 = DPACC IR

                // CSW register value
                csw_value = CSW_32BIT | CSW_ADDRINC_OFF | CSW_DBGSWENABLE | CSW_MASTER_DEBUG | CSW_HPROT;

                // tar register value
                tar_value = address & 0xFFFFFFF0;

                // set csw register value
                cmd->move_to(jtag::TAP_DRSHIFT);

                data = DPAP_WRITE | CSW_ADDR;
                cmd->write_dr(data);
                cmd->write_dr(csw_value);

                // set tar register value
                cmd->move_to(jtag::TAP_DRSHIFT);

                data = DPAP_WRITE | TAR_ADDR;
                cmd->write_dr(data);
                cmd->write_dr(tar_value);

                // set DRW register value
                cmd->move_to(jtag::TAP_DRSHIFT);

                data = datain | DRW_ADDR;
                cmd->write_dr(data);
                cmd->write_dr(tar_value);

                cmd->move_to(jtag::TAP_RESET);

                // clock 255 times
                for(int i=0; i<255; i++)
                        cmd->add_command(0,0,0,0);

                break;

        case IDCODE:
                /*
                 * The IDCODE command :
                 *
                 */
                cmd->move_to(jtag::TAP_IRSHIFT);

                cmd->write_ir(0xE); //1110 = IDCODE IR

                /* TODO : solve move_to mistake*/

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
