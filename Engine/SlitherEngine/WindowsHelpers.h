#pragma once

void OutputMessage(const char* message, ...);
char* LoadCompleteFile(const char* filename, long* length);
double GetSystemTime();
double GetSystemTimeSinceGameStart();