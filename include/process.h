#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  bool operator>(Process const& a) const;

 private:
  int pid_{0};
  string user_{""};
  string cmd_{""};
  float cpuUtil_{0.0};
  string ram_{""};
  long upTime_{0};

  void CpuUtilization(int pid);
  void Ram(int pid);
};

#endif