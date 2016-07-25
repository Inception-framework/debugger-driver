/*
 * CommandsFactory.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "CommandsFactory.h"

using namespace std;

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
jtag::Command* CommandsFactory::CreateCommand (COMMAND_TYPE type, vector<uint32_t>& argv) {

        jtag::Command* cmd = new jtag::Command(type);

        switch(type) {

        case SELECT:

                if ( CommandsFactory::check_arg ( argv, 1) == true )
                        CommandsFactory::select (cmd, argv.at(0));
                else
                        ALERT ("CommandsFactory", "SELECT args missing");

                break;

        case RESET:

                for(int i=0; i<5; i++)
                        cmd->add_command(1,0,0,0);

                break;

        case READ_U32:

                if ( check_arg ( argv, 1) == true )
                        CommandsFactory::read_u32 (cmd, argv.at(0));
                else
                        ALERT ("CommandsFactory", "READ_U32 args missing");

                break;

        case WRITE_U32:

                if ( check_arg( argv, 2) == true )
                        CommandsFactory::write_u32 (cmd, argv.at(0), argv.at(0));
                else
                        ALERT ("CommandsFactory", "WRITE_U32 args missing");

                break;

        case IDCODE:

                cmd->move_to(jtag::TAP_IRSHIFT);

                cmd->write_ir(0xE); //1110 = IDCODE IR

                cmd->move_to(jtag::TAP_DRSHIFT);

                for(int i=0; i<32; i++)
                        cmd->add_command(0,0,0,0);

                cmd->move_to(jtag::TAP_RESET);

                break;
        }

        argv.clear ();

        return cmd;

}

bool CommandsFactory::check_arg (vector<uint32_t>& argv, uint32_t required) {

        if( argv.size() != required )
                return false;

        return true;
}

void CommandsFactory::select (jtag::Command* cmd, uint32_t bank_id) {

        uint32_t header;
        uint32_t ap_register[2];

        // Set the correct JTAG-DP
        cmd->move_to(jtag::TAP_IRSHIFT);
        cmd->write_ir(DPACC);   //1010 = DPACC IR

        // set select register value
        cmd->move_to(jtag::TAP_DRSHIFT);

        header = DPAP_WRITE | SEL_ADDR;
        ap_register [0] = header;
        ap_register [1] = bank_id;
        cmd->write_dr(ap_register, 35);

        cmd->move_to(jtag::TAP_RESET);
}

void CommandsFactory::read_u32 (jtag::Command* cmd, uint32_t address) {

        uint32_t header, tar_value, csw_value;

        uint32_t ap_register[2];

        // Set the correct JTAG-DP
        cmd->move_to(jtag::TAP_IRSHIFT);
        cmd->write_ir(APACC);   //1010 = APACC IR

        // CSW register value
        csw_value = CSW_32BIT | CSW_ADDRINC_OFF | CSW_DBGSWENABLE | CSW_MASTER_DEBUG | CSW_HPROT;

        // tar register value
        tar_value = address;

        // set csw register value
        cmd->move_to(jtag::TAP_DRSHIFT);

        header = DPAP_WRITE | CSW_ADDR;
        ap_register [0] = header;
        ap_register [1] = csw_value;

        cmd->write_dr(ap_register, 35);

        // set tar register value
        cmd->move_to(jtag::TAP_DRSHIFT);

        header = DPAP_WRITE | TAR_ADDR;
        ap_register [0] = header;
        ap_register [1] = tar_value;

        cmd->write_dr(ap_register, 35);


        cmd->move_to(jtag::TAP_RESET);

        // clock 255 times
        for(int i=0; i<255; i++)
                cmd->add_command(1,0,0,0);
}

void CommandsFactory::write_u32 (jtag::Command* cmd, uint32_t address, uint32_t datain) {

        uint32_t header, csw_value;
        uint32_t ap_register[2];

        // Set the correct JTAG-DP
        cmd->move_to(jtag::TAP_IRSHIFT);
        cmd->write_ir(APACC); //1011 = APACC IR

        // CSW register value
        csw_value = 0x22000012;//CSW_32BIT | CSW_ADDRINC_OFF | CSW_DBGSWENABLE | CSW_MASTER_DEBUG | CSW_SPROT | CSW_ADDRINC_SINGLE | CSW_SPIDEN;
        printf("CSW = 0x%08x", csw_value);

        // set csw register value
        cmd->move_to(jtag::TAP_DRSHIFT);

        header = DPAP_WRITE | CSW_ADDR;
        ap_register [0] = header;
        ap_register [1] = csw_value;

        cmd->write_dr(ap_register, 35);

        // set tar register value
        cmd->move_to(jtag::TAP_DRSHIFT);

        header = DPAP_WRITE | TAR_ADDR;
        ap_register [0] = header;
        ap_register [1] = address;

        cmd->write_dr(ap_register, 35);

        // set DRW register value
        cmd->move_to(jtag::TAP_DRSHIFT);

        header = DRW_ADDR;
        ap_register [0] = header;
        ap_register [1] = datain;

        cmd->write_dr(ap_register, 35);

        cmd->move_to(jtag::TAP_RESET);

        // clock 255 times
        for(int i=0; i<255; i++)
                cmd->add_command(1,0,0,0);
}
