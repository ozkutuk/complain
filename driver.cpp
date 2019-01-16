#include "driver.hpp"

#if 0
Driver::Driver() 
  : trace_scanning (false), trace_parsing (false)
{ }

int Driver::parse (const std::string & filename) {
  file = f;
  scan_begin();
  yy::calcxx_parser parser (*this);
  parser.set_debug_level (trace_parsing);
  int res = parser.parse ();
  scan_end();
  return res;
}

void calcxx_driver::error (const std::string& m) {
  std::cerr << m << std::endl;
}
#endif

Driver::Driver() = default;
