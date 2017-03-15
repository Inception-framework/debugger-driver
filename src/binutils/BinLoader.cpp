/**
* @Author: Nassim
* @Date:   2017-03-15T10:17:27+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:51:23+01:00
*/



#include "BinLoader.h"
#include <string>
#include <fstream>

BinLoader::BinLoader(std::string source) : ifs(source.c_str(), std::ifstream::binary) {

  source = source;

  if (! ifs.is_open()) {
    ALERT("BinLoader", "Unable to open Binary file : %s ", source.c_str());
    throw std::runtime_error("Unable to open binary file\n");
  }

  ifs.seekg (0, ifs.beg);
}

BinLoader::~BinLoader() {
  ifs.close();
}

uint32_t BinLoader::next() {

  uint32_t val;

  if( ! ifs.eof() )
    ifs.read((char*) (&val), sizeof(val));
  else
    throw std::runtime_error("End of binary file\n");

  INFO("BIN_LOADED", "0x%08x", val);

  return val;
}

// int main(int argc, char** argv) {
//
//   BinLoader* bin_loader = new BinLoader("/home/noname/workspacees06/stub/SLA/stub.bin");
//
//   try {
//
//     while(true)
//       bin_loader->next();
//
//   } catch (std::runtime_error) {
//
//     INFO("BIN_LOADER", "End Of File");
//   }
// }
