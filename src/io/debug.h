#pragma once

#include <stdio.h>

namespace Debug {

    /**
     * Enthält alle Debugvariablen, allesamt zur Compilezeit bekannt.
     */
    namespace Config {

        /**
         * Logging wird nur in die Konsole ausgegeben wenn DO_LOGGING true ist.
         */
        inline constexpr bool DO_LOGGING = true;

        /**
         * Wenn true, gibt eine Warnung aus wenn ein asset geladen werden soll das nicht existiert.
         */
        inline constexpr bool LOG_MISSING_ASSETS = true;

    }

    enum LogLevel {
        INFO = 0,
        DEBUG,
        WARNING, 
        ERROR, 
        FATAL
    };

    /**
     * Druckt den namen eines LogLevels in die Konsole. Wird eigentlich nur von den Log(...) Funktionen gebraucht und sollte so nicht verwendet werden.
     */
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

}