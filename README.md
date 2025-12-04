# 🖥️ Ubuntu System Monitor

**Author:** Suman  
**Date Created:** 2022/02/21 (semester break, at haw gitlab)

A lightweight **terminal-based system monitor** written in **C++** that displays real-time CPU usage, memory, network throughput, uptime, and CPU temperature.  
It includes an **ASCII graph** for CPU usage and logs text stats to a file for later analysis.

---

## Features

- ✅ Real-time **CPU usage graph** (ASCII)  
- ✅ Memory usage display (used / total)  
- ✅ System uptime  
- ✅ Network throughput monitoring (RX/TX in bytes/sec)  
- ✅ CPU temperature monitoring (via `/sys/class/thermal/`)  
- ✅ Logs text stats to `system_stats.log`  
- ✅ Works on Linux (tested on Ubuntu)  
- ✅ Easy build with **Makefile** (no CMake required)  

---

## Folder Structure

```text
system_monitor/
├── .gitignore
├── include/
│   └── monitor.hpp
├── src/
│   ├── main.cpp
│   └── monitor.cpp
└── Makefile
````

---

## Dependencies

* Linux system (access to `/proc` filesystem)
* C++17 compatible compiler (e.g., `g++`)
* Standard C++ library only (no external libraries)
* Terminal that supports ANSI/ASCII characters

---

## Build & Run

### 1. Build

```bash
make
```

### 2. Run

```bash
make run
```

This opens a **live terminal view** showing:

* CPU Usage (with ASCII graph)
* Memory usage
* Network RX/TX
* CPU temperature
* System uptime

### 3. Clean Build Artifacts

```bash
make clean
```

### 4. View Log While Running

Open another terminal:

```bash
tail -f system_stats.log
```

The log file continuously records **text stats** for later analysis.

---

## Example Output

```text
===== Linux System Monitor =====
CPU Usage:       12 %
Memory Used:     3200 MB / 8000 MB
Uptime:          3600 sec
Net RX/TX:       1024 B/s | 512 B/s
CPU Temp:        45.2 °C

CPU Usage Graph:

█ █    █  █   █
█ █    █  █   █
---------------
```

---

## How It Works

1. **CPU Usage**: reads `/proc/stat` and calculates percentage usage.
2. **Memory**: reads `/proc/meminfo`.
3. **Uptime**: reads `/proc/uptime`.
4. **Network Throughput**: reads `/proc/net/dev` for RX/TX bytes.
5. **CPU Temperature**: reads `/sys/class/thermal/thermal_zone*/temp`.
6. **ASCII Graph**: maintains a history of CPU usage samples and prints them vertically.
7. **Logging**: text stats are appended to `system_stats.log`.

---

## Makefile Targets

| Target       | Description                                |
| ------------ | ------------------------------------------ |
| `make`       | Build the project                          |
| `make run`   | Build and run the monitor                  |
| `make clean` | Delete build artifacts (`obj/` and `bin/`) |

---


