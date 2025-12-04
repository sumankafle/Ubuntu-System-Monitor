#include "monitor.hpp"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <filesystem>

Monitor::Monitor(int historySize)
    : historySize(historySize) {}

// ---------------- CPU USAGE ----------------
double Monitor::readCpuUsage() {
    std::ifstream statFile("/proc/stat");
    std::string line;
    std::getline(statFile, line);
    std::stringstream ss(line);

    std::string cpu;
    long user, nice, system, idle;
    ss >> cpu >> user >> nice >> system >> idle;

    long total = user + nice + system + idle;
    long totalDiff = total - prevTotal;
    long idleDiff = idle - prevIdle;

    prevTotal = total;
    prevIdle = idle;

    if (totalDiff == 0) return 0.0;

    double usage = 100.0 * (1.0 - (double)idleDiff / totalDiff);

    // Push into history
    cpuHistory.push_back(usage);
    if (cpuHistory.size() > historySize)
        cpuHistory.erase(cpuHistory.begin());

    return usage;
}

// ---------------- MEMORY ----------------
void Monitor::readMemory(long &total, long &avail) {
    std::ifstream memFile("/proc/meminfo");
    std::string key;
    while (memFile >> key) {
        if (key == "MemTotal:") memFile >> total;
        if (key == "MemAvailable:") { memFile >> avail; break; }
        memFile.ignore(256, '\n');
    }
}

// ---------------- UPTIME ----------------
long Monitor::readUptime() {
    std::ifstream upFile("/proc/uptime");
    double up;
    upFile >> up;
    return (long)up;
}

// ---------------- NETWORK ----------------
void Monitor::readNetwork(long &rxRate, long &txRate) {
    std::ifstream netFile("/proc/net/dev");
    std::string line;

    long totalRx = 0, totalTx = 0;

    while (std::getline(netFile, line)) {
        if (line.find(":") == std::string::npos) continue;

        std::stringstream ss(line);
        std::string iface;
        long rx, tx;
        ss >> iface;
        iface.pop_back();  // remove :

        if (iface == "lo") continue;  // skip loopback

        ss >> rx;                  // RX bytes
        for (int i = 0; i < 7; i++) ss >> std::ws; // skip fields
        ss >> tx;                  // TX bytes

        totalRx += rx;
        totalTx += tx;
    }

    rxRate = totalRx - prevRx;
    txRate = totalTx - prevTx;

    prevRx = totalRx;
    prevTx = totalTx;
}

// ---------------- TEMPERATURE ----------------
double Monitor::readTemperature() {
    for (const auto &entry : std::filesystem::directory_iterator("/sys/class/thermal")) {
        auto path = entry.path() / "temp";
        if (std::filesystem::exists(path)) {
            std::ifstream f(path);
            double tempMilli;
            if (f >> tempMilli)
                return tempMilli / 1000.0;
        }
    }
    return 0.0;
}

// ---------------- COMBINED ----------------
SystemStats Monitor::readStats() {
    SystemStats s;

    s.cpuUsage = readCpuUsage();
    readMemory(s.memTotal, s.memAvailable);
    s.uptime = readUptime();
    readNetwork(s.netRxRate, s.netTxRate);
    s.cpuTempC = readTemperature();
    s.cpuHistory = cpuHistory;

    return s;
}
