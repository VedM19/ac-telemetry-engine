#include "ACTelemetryReader.h"
#include <windows.h>
#include <chrono>
#include <iostream>
#include <thread>


// Enable ANSI escape codes for terminal output when not in VScode's integrated terminal
void enableVTMode() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    // Enable Virtual Terminal Processing for ANSI codes
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

// Hide the blinking terminal cursor
void hideCursor() {
    std::cout << "\x1b[?25l" << std::flush;
}

// Restore the terminal cursor on program exit
void showCursor() {
    std::cout << "\x1b[?25h" << std::flush;
}

int main()
{
    enableVTMode();
    ACTelemetryReader reader;

    if (!reader.connect()) {
        std::cerr << "Failed to connect to Assetto Corsa telemetry.\n";
        return 1;
    }

    int lastPacketId = -1;
    int packetId = -1;
    LARGE_INTEGER frequency, lastTimestamp, now;
    QueryPerformanceFrequency(&frequency); // how many ticks equal one second on your machine
    QueryPerformanceCounter(&lastTimestamp);

    while (true) {
        const SPageFilePhysics* frame = reader.fetchLatestFrame();


        if (frame != nullptr) {
            packetId = frame->packetId;
        }
        else {
            std::cout << "\r\33[K"; // Clear the line
            std::cout << "Failed to fetch latest frame.\n";
        }

        std::vector<double> deltas;
        

        if (frame != nullptr && packetId != lastPacketId) {
            QueryPerformanceCounter(&now);
            double deltaMs = (double)(now.QuadPart - lastTimestamp.QuadPart) * 1000.0 / frequency.QuadPart;  // Convert to milliseconds
            deltas.push_back(deltaMs);
            lastTimestamp = now;

            hideCursor();
            std::cout << "\r\33[KTelemetry frame available. Read live fields here.\n";
            std::cout << "\r\33[KSpeed: " << frame->speedKmh << " km/h\n";
            std::cout << "\r\33[KRPMs: " << frame->rpms << ".\n";
            std::cout << "\r\33[KGear: " << frame->gear << ".\n";
            std::cout << "\r\33[KGas: " << frame->gas << ".\n";
            std::cout << "\r\33[KBrake: " << frame->brake << ".\n";
            std::cout << "\r\33[KFuel: " << frame->fuel << "\n";             
            std::cout << "\r\33[7A"; // Move cursor up to overwrite previous lines
            lastPacketId = packetId;
        } else {
            std::cout << "\r\33[K"; // Clear the line
            std::cout << "Telemetry frame unavailable. Connect or retry here.\n";
            break; // Exit the loop if no frame is available
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    showCursor();
    reader.disconnect();
    return 0;
}
