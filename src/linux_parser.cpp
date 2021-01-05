#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "key_value_parser.h"

using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  float memoryTotal, memoryFree;

  memoryTotal =
      KeyValueParser::GetValueByKey<float>("MemTotal:", kMeminfoFilename);
  memoryFree =
      KeyValueParser::GetValueByKey<float>("MemFree:", kMeminfoFilename);

  return 1 - memoryFree / memoryTotal;
}

long LinuxParser::UpTime() {
  long upTime = KeyValueParser::GetValue<long>(kUptimeFilename);

  return upTime;
}

long LinuxParser::Jiffies() {
  vector<string> jiffies = CpuUtilization();
  long totalJiffies = 0;
  for (string jiffies : jiffies) {
    try {
      totalJiffies += stol(jiffies);
    } catch (const std::exception& e) {
      totalJiffies += 0;
    }
  }

  return totalJiffies;
}

long LinuxParser::ActiveJiffies(int pid) {
  long activeJiffies = 0;
  string utime, stime, line, skip;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    // Skip the first fourteen values
    for (int i = 0; i < 13; ++i) {
      linestream >> skip;
    }
    linestream >> utime >> stime;
  }
  try {
    activeJiffies = stol(utime) + stol(stime);
  } catch (const std::exception& e) {
    activeJiffies += 0;
  }

  return activeJiffies;
}

long LinuxParser::ActiveJiffies() {
  long activeJiffies = 0;
  activeJiffies = Jiffies() - IdleJiffies();

  return activeJiffies;
}

long LinuxParser::IdleJiffies() {
  vector<string> jiffies = CpuUtilization();
  long idleJiffies = 0;
  long idle = std::stoi(jiffies[3]);
  long ioWait = std::stoi(jiffies[4]);
  idleJiffies = idle + ioWait;

  return idleJiffies;
}

vector<string> LinuxParser::CpuUtilization() {
  vector<string> timers;
  string timer;
  string line;
  string skip;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> skip;
    while (!linestream.eof()) {
      linestream >> timer;
      timers.push_back(timer);
    }
  }

  return timers;
}

int LinuxParser::TotalProcesses() {
  int totalProcesses =
      KeyValueParser::GetValueByKey<int>("processes", kStatFilename);

  return totalProcesses;
}

int LinuxParser::RunningProcesses() {
  int runningProcesses =
      KeyValueParser::GetValueByKey<int>("procs_running", kStatFilename);

  return runningProcesses;
}

string LinuxParser::Command(int pid) {
  string cmd{};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmd);
  }
  return cmd;
}

string LinuxParser::Ram(int pid) {
  string ram = KeyValueParser::GetValueByKey<string>(
      "VmSize:", to_string(pid) + kStatusFilename);

  return ram;
}

string LinuxParser::Uid(int pid) {
  string uid = KeyValueParser::GetValueByKey<string>(
      "Uid:", to_string(pid) + kStatusFilename);

  return uid;
}

string LinuxParser::User(int pid) {
  string line, skip;
  string tmpUser, tmpUid;
  string user{"N/A"}, uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (!stream.eof()) {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> tmpUser >> skip >> tmpUid;

      if (uid == tmpUid) {
        user = tmpUser;
        return user;
      }
    }
  }

  return user;
}

long LinuxParser::UpTime(int pid) {
  long ticks =
      KeyValueParser::GetValue<long>(to_string(pid) + kStatFilename, 14);

  return ticks;
}