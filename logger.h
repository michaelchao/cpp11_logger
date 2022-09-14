#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <ostream>


#define logger() Logger::singleton()()() // << stuff1 << stuff2;
#define logger_f() Logger::singleton()(true)() // << stuff1 << stuff2;

class Logger {
public:
  class LoggerStream {
  public:
    LoggerStream(Logger & logger, bool force);

    ~LoggerStream();

    std::ostream & operator()();

  private:
    Logger & logger_;
    bool force_ {false};
  };
public:
  static Logger & singleton();

  void init(bool enable);

  bool enabled();

  LoggerStream operator()(bool force = false);

private:
  bool enabled_ {false};
};

#endif
