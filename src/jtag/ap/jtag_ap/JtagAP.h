/*
 * JtagAP.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef JTAG_AP_H_
#define JTAG_AP_H_

#include <stdint.h>
#include <string.h>

#include "../AccessPort.h"

namespace jtag {

class JtagAP : public AccessPort {
public:
        JtagAP ();

        virtual ~JtagAP ();

        uint32_t select();
};

} /* namespace JTAG */

#endif /* JTAG_AP_H_ */
