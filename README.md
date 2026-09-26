# AC Telemetry Engine

Windows shared-memory telemetry readers for Assetto Corsa, implemented in C++17 and .NET 8/C#.

The readers attach to Assetto Corsa's `acpmf_physics` shared-memory block and expose live physics telemetry, including speed, RPM, gear, throttle, brake, fuel, and tyre temperatures.

## Demo

![Live C++ telemetry reader](docs/readerdemo.gif)

*The native C++ reader sampling Assetto Corsa shared memory and rendering live telemetry.*

## Implementations

| Reader | Language | Status | Notes |
| --- | --- | --- | --- |
| Native reader | C++17 | Working | Direct Windows shared-memory reader with terminal output. |
| Managed consumer | .NET 8 / C# | Working | Maps the physics block into a packed C# struct and renders a live terminal dashboard. |

## .NET telemetry consumer

The C# consumer connects to Assetto Corsa's `acpmf_physics` memory-mapped file, waits for new physics packets, and displays live telemetry in the terminal.

### Requirements

- Windows
- .NET 8 SDK
- Assetto Corsa running in a session

### Run

```powershell
dotnet run --project dotnet/TelemetryConsumer
```

## Architecture direction

The current readers intentionally keep acquisition and terminal rendering together while the shared-memory boundary is being validated. The next iteration will separate input from output through an SPSC (single-producer, single-consumer) lock-free ring buffer:

```text
Assetto Corsa shared memory
        ↓
input reader / producer
        ↓
SPSC lock-free ring buffer
        ↓
output consumer(s)
```

This keeps ingestion isolated from downstream work and will support independent outputs such as terminal dashboards, recording, analytics, and visualizations.

## Repository layout

```text
cpp/       Native C++ telemetry reader and ring-buffer experiments
dotnet/    .NET 8/C# telemetry consumer
docs/      Demo media
```

## Status

Active experiment and learning project. The shared-memory readers are functional; producer/consumer decoupling and SPSC ring-buffer integration are planned.
