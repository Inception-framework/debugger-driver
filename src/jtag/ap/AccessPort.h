/*
 * Command.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef ACCESS_PORT_H_
#define ACCESS_PORT_H_

#include <stdint.h>
#include <string.h>

namespace jtag {

class AccessPort {
public:
        AccessPort (std::string name);

        virtual ~AccessPort ();

        virtual void select() = 0;

private:
        std::string name;

        static AccessPort* current_ap;
};

} /* namespace JTAG */

#endif /* ACCESS_PORT_H_ */
