#pragma once

#include <stdio.h>

namespace Debug {

    namespace Config {

        inline constexpr bool DO_LOGGING = true;

    }

    enum LogLevel {
        INFO = 0,
        DEBUG,
        WARNING, 
        ERROR, 
        FATAL
    };

    void PrintLogLevel(LogLevel level);

    /**
     * Loggt eine Nachricht, wenn Debug::Config::DO_LOGGING true ist
     */
    template<typename... Formatters>
    void Log(LogLevel level, const char *message, Formatters... f) {

        if (!Config::DO_LOGGING)
            return;

        PrintLogLevel(level);
        printf(" ");
        printf(message, f...);
        printf("\n");

    }

    /**
     * Loggt eine Nachricht, wenn Debug::Config::DO_LOGGING true ist
     */
    template<typename... Formatters>
    void Log(const char *message, Formatters... f) {

        if (!Config::DO_LOGGING)
            return;

        PrintLogLevel(LogLevel::INFO);
        printf(" ");
        printf(message, f...);
        printf("\n");

    }

    /**
     * Erzeugt immer Debug-Output, auch wenn Debug::Config::DO_LOGGING false ist
     */
    template<typename... Formatters>
    void Debug(const char *message, Formatters... f) {

        PrintLogLevel(LogLevel::DEBUG);
        printf(" ");
        printf(message, f...);
        printf("\n");

    }

    #undef PRINT_LOG_LEVEL

}