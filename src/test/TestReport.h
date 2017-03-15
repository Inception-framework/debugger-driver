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
