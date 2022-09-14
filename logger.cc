#ifndef LOGGER_H_
# include "logger.h"
#endif

#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace stream_internal {
class NullStreambuf : public std::streambuf {
};

class NullOStream : private NullStreambuf, public std::ostream {
public:
    NullOStream()
    : std::ostream(this) {
    }

    NullStreambuf* rdbuf() {
      return this;
    }
};
}  // namespace stream_internal


Logger::LoggerStream::LoggerStream(Logger & logger, bool force)
: logger_(logger), force_(force) {
}

Logger::LoggerStream::~LoggerStream() {
  if (logger_.enabled()) {
    std::cout << std::endl;
  }
}

static stream_internal::NullOStream null_stream;

std::ostream & Logger::LoggerStream::operator()() {
  if (logger_.enabled()) {
    return std::cout;
  } else {
    return null_stream;
  }
}

Logger & Logger::singleton() {
  static Logger sobj;
  return sobj;
}

void Logger::init(bool enable) {
  enabled_ = enable;
}

bool Logger::enabled() {
  return enabled_;
}

#ifdef WIN32
# define localtime_r(_Time, _Tm) localtime_s(_Tm, _Time)
#endif

static std::ostream& operator<< (std::ostream& stream, const tm* tm) {
    return stream << 1900 + tm->tm_year << '-'
        << std::setfill('0') << std::setw(2) << tm->tm_mon + 1 << '-'
        << std::setfill('0') << std::setw(2) << tm->tm_mday << ' '
        << std::setfill('0') << std::setw(2) << tm->tm_hour << ':'
        << std::setfill('0') << std::setw(2) << tm->tm_min << ':'
        << std::setfill('0') << std::setw(2) << tm->tm_sec;
}

static void get_local_time(tm * t) {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  localtime_r(&in_time_t, t);
}

static tm local_time;

Logger::LoggerStream Logger::operator()(bool force) {
  auto s = LoggerStream(*this, force);
  get_local_time(&local_time);
  s() << "[" << &local_time << "] ";
  return s;
}
