#include "TestReport.h"

TestReport::TestReport(std::string name, std::string description,
                       uint8_t gravity)
    : name(name), description(description), gravity(gravity) {}

TestReport::~TestReport() {}
