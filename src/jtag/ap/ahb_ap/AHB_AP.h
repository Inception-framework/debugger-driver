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

namespace jtag {

class AHB_AP {
public:
        AHB_AP (std::string name);

        virtual ~AHB_AP ();

        virtual void select() = 0;

private:
        std::string name;

        static AccessPort* current_ap;
};

} /* namespace JTAG */

#endif /* AHB_AP_H_ */
