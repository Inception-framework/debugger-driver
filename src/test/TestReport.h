/**
* @Author: Nassim
* @Date:   2017-03-15T10:16:37+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:48:36+01:00
*/



#ifndef TEST_REPORT_H
#define TEST_REPORT_H

#include <string>
#include <stdint.h>

class TestReport {
public:

  TestReport(std::string name, std::string description, uint8_t gravity);

  ~TestReport();

  std::string name;

  bool success;

  std::string description;

  std::string error;

  uint8_t gravity;
};

#endif
