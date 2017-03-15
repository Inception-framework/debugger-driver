/**
* @Author: Nassim
* @Date:   2017-03-15T10:36:41+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T12:15:11+01:00
*/

#ifndef InceptionBuilder_h
#define InceptionBuilder_h

#include <stdio.h>
#include <cstdint>

#include "../CommandsBuilder.h"
#include "../Jtag.h"

namespace jtag {

class InceptionBuilder : public CommandsBuilder {
private:
public:
    InceptionBuilder();
    ~InceptionBuilder();

    jtag::Command* write(uint32_t data, uint32_t address);

  	jtag::Command* read(uint32_t address);

  	jtag::Command* reset();

  	jtag::Command* idcode();

  	jtag::Command* init();
protected:

};

}

#endif /* InceptionBuilder_h */
