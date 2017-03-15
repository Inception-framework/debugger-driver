/**
* @Author: Nassim
* @Date:   2017-03-15T10:16:37+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:48:35+01:00
*/



#include "TestReport.h"

TestReport::TestReport(std::string name, std::string description,
                       uint8_t gravity)
    : name(name), description(description), gravity(gravity) {}

TestReport::~TestReport() {}
