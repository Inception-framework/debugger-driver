/*
 * Command.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef ACCESS_PORT_H_
#define ACCESS_PORT_H_

#include <stdint.h>
#include <string>

namespace jtag {

class AccessPort {
public:
        AccessPort (std::string name);

        virtual ~AccessPort ();

        virtual uint32_t select() = 0;

        static void select(AccessPort* ap);

protected:
        uint32_t select_register;

private:
        static AccessPort* current_ap;

        std::string name;
};

} /* namespace JTAG */

#endif /* ACCESS_PORT_H_ */
