#include "para-dars/core/LogManager.h"

std::vector<LogEntry> LogManager::logs;  // static members must appear in cpp

void LogManager::Log(LogType type, const std::string& msg) {
    logs.push_back({type, msg});
}

std::vector<LogEntry> LogManager::GetLogs() {
    return logs;
}

void LogManager::Clear() {
    logs.clear();
}