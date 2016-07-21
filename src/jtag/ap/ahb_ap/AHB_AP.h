/*
 * AHB_AP.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef AHB_AP_H_
#define AHB_AP_H_

#include <stdint.h>
#include <string.h>

#include "../AccessPort.h"

namespace jtag {

class AHB_AP : public AccessPort {
public:
        AHB_AP ();

        virtual ~AHB_AP ();

        uint32_t select();
};

} /* namespace JTAG */

#endif /* AHB_AP_H_ */
