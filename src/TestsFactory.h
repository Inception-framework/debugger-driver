#ifndef TESTS_FACTORY_H
#define TESTS_FACTORY_H

#include "TestReport.h"

#include <string>
#include <map>
#include "stdint.h"
#include <assert.h>
#include <iterator>

class System;
class TestReport;

enum Test {
  BENCHMARK_IO,
  CHECK_WRITE_U32,
  CHECK_READ_U32,
  CHECK_DEVICE,
  TEST_IDCODE,
  TEST_TRACE
};

// typedef struct EnumIter<BENCHMARK_IO, CHECK_WRITE_U32, CHECK_READ_U32, CHECK_DEVICE>  TestIterator;
//
// ref class Test {
//  public:
//   static const Test BENCHMARK_IO;
//   static const Test CHECK_WRITE_U32;
//   static const Test CHECK_READ_U32;
//   static const Test CHECK_DEVICE;
// };

// namespace std {
// template <> struct iterator_traits<Test> {
//   typedef Test value_type;
//   typedef int difference_type;
//   typedef Test *pointer;
//   typedef Test &reference;
//   typedef std::bidirectional_iterator_tag iterator_category;
// };
// }

class TestsFactory {
public:
  TestsFactory();

  ~TestsFactory();

  static TestReport *CreateTest(enum Test type, System* sys);

private :

  static std::map<enum Test, TestReport*> testMap;

  static void trace(System* sys, TestReport* report);

  static void benchmark_io(System* sys, TestReport* report);

  static void idcode(System* sys, TestReport* report);
};

#endif
