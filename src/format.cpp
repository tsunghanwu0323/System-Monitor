#include "format.h"

#include <iostream>
#include <string>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hour = 0;
  int min = 0;
  int sec = 0;

  hour = seconds / 3600;
  seconds %= 3600;
  min = seconds / 60;
  seconds %= 60;
  sec = seconds;

  return Format(hour) + ":" + Format(min) + ":" + Format(sec);
}

string Format::Format(int time) {
  string time_string = std::to_string(time);

  return string(2 - time_string.length(), '0') + time_string;
}