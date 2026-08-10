# ac-telemetry-engine
Low-latency C++ telemetry ingestion engine and PyTorch ML analytics pipeline for Assetto Corsa

## 📹 Live Memory Reader Demo

![Live Telemetry Demo](docs/readerdemo.gif)

*The C++ reader sampling Assetto Corsa shared memory at high frequency without dropping frames.*

## 🛠️ Tech Stack & Architecture
- **Language:** C++17
- **APIs:** Windows Shared Memory (`MapViewOfFile`, `OpenFileMappingA`)
- **Build System:** CMake