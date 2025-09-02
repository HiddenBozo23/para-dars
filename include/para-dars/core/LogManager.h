#pragma once

#include <string>
#include <vector>

enum class LogType {
    Info,
    Warning,
    Error
};

struct LogEntry {
    LogType type;
    std::string message;
};

class LogManager {
    public:
        static void Log(LogType type, const std::string& msg);
        static std::vector<LogEntry> GetLogs();
        static void Clear();

    private:
        static std::vector<LogEntry> logs;
};