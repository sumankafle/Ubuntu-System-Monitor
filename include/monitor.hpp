#pragma once
#include <string>
#include <vector>

struct SystemStats {
    double cpuUsage;
    long memTotal;
    long memAvailable;
    long uptime;
    long netRxRate;
    long netTxRate;
    double cpuTempC;
    std::vector<double> cpuHistory;
};

class Monitor {
public:
    Monitor(int historySize = 50);
    SystemStats readStats();

private:
    int historySize;
    std::vector<double> cpuHistory;

    long prevTotal = 0, prevIdle = 0;
    long prevRx = 0, prevTx = 0;

    double readCpuUsage();
    void readMemory(long &total, long &avail);
    long readUptime();
    void readNetwork(long &rxRate, long &txRate);
    double readTemperature();
};
