#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "main.h"
#include "jtag/ap/AccessPort.h"

#include "TestsFactory.h"

class System {

  public :

    System();

    ~System();

    std::string info();

    uint32_t read_u32(uint32_t address);

    void write_u32(uint32_t value, uint32_t address);

    void check_up(void);

    void check(Test test);
    // void read_chunck(uint32_t address, std::vector<char> buffer);
    // void write_chunck(uint32_t address, std::vector<char> buffer);

    void synchrone_process(jtag::Command* command, uint64_t *value);

  private :

    Device::USBDevice *fx3;

    Producer *producer;

    Decoder *decoder;

    void init_jtag(void);

    bool is_initialized;

    jtag::AccessPort *ap;

    void select(void);

    void load_binary_in_sdram(std::string file_path);
};

#endif
