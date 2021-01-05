#ifndef KEY_VALUE_PARSER
#define KEY_VALUE_PARSER

#include <fstream>
#include <string>

#include "linux_parser.h"
using std::string;

namespace KeyValueParser {
template <typename T>
T GetValueByKey(string const &key, string const &fileName) {
  string line;
  string k;
  T value;
  std::ifstream stream(LinuxParser::kProcDirectory + fileName);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> k >> value) {
        if (k == key) {
          return value;
        }
      }
    }
  };

  return value;
}

template <typename T>
T GetValue(string const &fileName) {
  string line;
  T value;
  std::ifstream stream(LinuxParser::kProcDirectory + fileName);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }

  return value;
};

template <typename T>
T GetValue(string const &fileName, int const k) {
  string line, skip;
  T value;
  std::ifstream stream(LinuxParser::kProcDirectory + fileName);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    // skip k times
    for (int i = 0; i < k; ++i) {
      linestream >> skip;
    }
    linestream >> value;
  }

  return value;
};
};  // namespace KeyValueParser

#endif