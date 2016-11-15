#ifndef BINLOADER_H
#define BINLOADER_H

#include <string>
#include <fstream>
#include <stdint.h>
#define _LOG_ALL
#include "colored.h"

class BinLoader{

  public:

    BinLoader(std::string source);

    ~BinLoader();

    uint32_t next();

  private:

    std::ifstream ifs;

    std::string source;

};

#endif
