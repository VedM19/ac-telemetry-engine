#include "ACTelemetryReader.h"

#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    ACTelemetryReader reader;

    /**
     * TODO: Implement this step
     *
     * WHAT to do:
     * - Call reader.connect() once before entering the polling loop.
     * - If it fails, decide whether to retry, exit, or keep waiting for Assetto
     *   Corsa to start.
     *
     * WHY:
     * - The shared-memory mapping exists only while the simulator is publishing
     *   telemetry.
     * - Connection failure is normal when Assetto Corsa is not running.
     *
     * Logic topics to study:
     * - Startup retry policies for external processes.
     * - How to surface connection status in a console telemetry tool.
     */
    if (!reader.connect()) {
        std::cerr << "Failed to connect to Assetto Corsa telemetry.\n";
        return 1;
    }

    int lastPacketId = -1;
    int packetId = -1;

    while (true) {
        /**
         * TODO: Implement this step
         *
         * WHAT to do:
         * - Call reader.fetchLatestFrame().
         * - If the returned pointer is not null, read fields such as speedKmh,
         *   rpms, gear, gas, brake, and fuel.
         * - Print or log the values you want to inspect.
         *
         * WHY:
         * - fetchLatestFrame() is the handoff point between Windows shared
         *   memory mechanics and your telemetry-processing logic.
         * - Keeping reads in the loop lets you observe live simulator updates.
         *
         * Logic topics to study:
         * - Using packetId to avoid printing duplicate frames.
         * - Formatting high-frequency console output without overwhelming the
         *   terminal.
         * - Separating data acquisition from later analytics code.
         */
        const SPageFilePhysics* frame = reader.fetchLatestFrame();


        if (frame != nullptr) {
            packetId = frame->packetId;
        }
        else {
            std::cout << "Failed to fetch latest frame.\n";
        }
        

        if (frame != nullptr && packetId != lastPacketId) {
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
            std::cout << "Telemetry frame unavailable. Connect or retry here.\n";
            break; // Exit the loop if no frame is available
        }

        /**
         * TODO: Implement this step
         *
         * WHAT to do:
         * - Sleep for roughly 16 milliseconds to approximate a 60Hz polling
         *   interval.
         *
         * WHY:
         * - A telemetry reader usually should not busy-spin at full CPU speed.
         * - 60Hz is a reasonable first polling cadence for learning and console
         *   debugging.
         *
         * Standard library APIs to study:
         * - std::this_thread::sleep_for.
         * - std::chrono::milliseconds.
         */
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    /**
     * TODO: Implement this step
     *
     * WHAT to do:
     * - Decide how your loop should exit cleanly.
     * - On exit, rely on ACTelemetryReader's destructor or call disconnect()
     *   explicitly before returning.
     *
     * WHY:
     * - Long-running telemetry tools eventually need signal handling, a quit
     *   key, or integration with a larger application lifecycle.
     */
    reader.disconnect();
    return 0;
}
