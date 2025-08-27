#include "debug.h"

void Debug::PrintLogLevel(Debug::LogLevel level) {

    static const char *names[] = {"[INFO]", "[DEBUG]", "[WARNING]", "[ERROR]", "[FATAL]"};
    printf(names[level]);

}