#include "monitor.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <algorithm>

void drawCpuGraph(const std::vector<double>& hist) {
    double maxVal = 100.0;
    int height = 10;

    std::cout << "\nCPU Usage Graph:\n\n";

    for (int row = height; row >= 0; --row) {
        for (double v : hist) {
            if (v >= (row * (maxVal / height)))
                std::cout << "█";
            else
                std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::string(hist.size(), '-') << "\n";
}

int main() {
    Monitor m(60); // 60 samples graph width

    std::ofstream logfile("system_stats.log", std::ios::out | std::ios::app);

    while (true) {
        SystemStats s = m.readStats();

        // --- Clear terminal ---
        system("clear");

        // --- Display graph + summary in terminal ---
        std::cout << "===== Linux System Monitor =====\n";
        std::cout << "CPU Usage:       " << s.cpuUsage << " %\n";
        std::cout << "Memory Used:     "
                  << (s.memTotal - s.memAvailable) / 1024 << " MB / "
                  << s.memTotal / 1024 << " MB\n";
        std::cout << "Uptime:          " << s.uptime << " sec\n";
        std::cout << "Net RX/TX:       "
                  << s.netRxRate << " B/s  |  "
                  << s.netTxRate << " B/s\n";
        std::cout << "CPU Temp:        " << s.cpuTempC << " °C\n";
        drawCpuGraph(s.cpuHistory);

        // --- Log only text data to file ---
        if (logfile.is_open()) {
            logfile << "CPU: " << s.cpuUsage << "%, "
                    << "Mem: " << (s.memTotal - s.memAvailable)/1024
                    << "/" << s.memTotal/1024 << " MB, "
                    << "Uptime: " << s.uptime << " sec, "
                    << "Net RX/TX: " << s.netRxRate << "/" << s.netTxRate
                    << " B/s, Temp: " << s.cpuTempC << " C\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}
