#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  pid_ = pid;
  user_ = LinuxParser::User(pid);
  cmd_ = LinuxParser::Command(pid);
  upTime_ = LinuxParser::UpTime(pid);
  Process::CpuUtilization(pid);
  Process::Ram(pid);
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { return cpuUtil_; }

string Process::Command() { return cmd_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return upTime_; }

bool Process::operator<(Process const& a) const {
  return this->cpuUtil_ < a.cpuUtil_;
}

bool Process::operator>(Process const& a) const {
  return this->cpuUtil_ > a.cpuUtil_;
}

void Process::CpuUtilization(int pid) {
  float process = LinuxParser::ActiveJiffies(pid);
  float processor = LinuxParser::ActiveJiffies();
  cpuUtil_ = process / processor;
}

void Process::Ram(int pid) {
  int kBytes = std::atoi(LinuxParser::Ram(pid).c_str());
  int mBytes = kBytes / 1000;

  ram_ = to_string(mBytes);
}