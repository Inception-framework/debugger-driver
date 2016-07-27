/*
 * Command_test.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "Command.h"

#include "../device/Device.h"
#include "../interface/Interface.h"
#include "../interface/Producer.h"

namespace jtag {

class CommandTest {

public:
  static void test() { test_move_to(); }

private:
  static void test_move_to() {

    INFO("Device", "Initializing superspeed device ...");
    Device::USBDevice *fx3 = new Device::USBDevice(0x04B4, 0x00F0, 0);
    fx3->init();

    INFO("Interface", "Starting producer thread ...");
    Interface *producer = new Producer(fx3);
    producer->start();

    INFO("Command", "Creating RESET command ...");
    Command *cmd = new Command(READ_U32);

    cmd->move_to(jtag::TAP_DRPAUSE);

    cmd->move_to(jtag::TAP_DRSHIFT);

    cmd->move_to(jtag::TAP_IRPAUSE);

    cmd->move_to(jtag::TAP_IRSHIFT);

    // cmd->move_to (jtag::TAP_DRSHIFT);

    producer->add_cmd_to_queue(cmd);
    producer->process_jtag_queue();

    INFO("Interface", "Shutting down interfaces ...");
    producer->stop();

    INFO("Device", "Closing device connection ...");
    fx3->quit();
  }
};

} /* namespace JTAG */
