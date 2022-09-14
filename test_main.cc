#ifndef LOGGER_H_
# include "logger.h"
#endif

int main() {
  Logger::singleton().init(true);
  logger() << 123;
  logger() << "1111";
  return 0;
}