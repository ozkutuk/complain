#pragma once

#include <string>
#include <unordered_map>

#include "llvm/IR/Value.h"

class Driver {
public:
  Driver();
  virtual ~Driver() = default;

  std::unordered_map<std::string, llvm::Value *> globals;

  // Handling the scanner.
  // void scan_begin();
  // void scan_end();
  // bool trace_scanning;

  // Run the parser on file F.
  // Return 0 on success.
  // int parse(const std::string & filename);

  // The name of the file being parsed.
  // Used later to pass the file name to the location tracker.
  // std::string file;

  // Whether parser traces should be generated.
  // bool trace_parsing;
  
  // Error handling.
  // void error(const yy::location& l, const std::string& m);
  // void error(const std::string& m);
};
