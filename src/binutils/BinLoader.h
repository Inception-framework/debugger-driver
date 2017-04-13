/**
* @Author: Nassim
* @Date:   2017-03-15T10:57:27+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:51:25+01:00
*/



#ifndef BINLOADER_H
#define BINLOADER_H

#include <string>
#include <fstream>
#include <stdint.h>
#define _LOG_ALL
#include "../colored.h"
#include <stdexcept>
#include <stdexcept>

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
