#include "ACTelemetryReader.h"

bool ACTelemetryReader::connect()
{
    /**
     * TODO: Implement this step
     *
     * WHAT to do:
     * - Include the appropriate Windows header in this source file when you are
     *   ready to implement the real logic.
     * - Call OpenFileMapping to open "Local\\acpmef_physics".
     * - Call MapViewOfFile to obtain a read-only pointer to the physics page.
     * - Store the returned HANDLE-like value in mappingHandle_ and the mapped
     *   page pointer in physicsView_.
     *
     * WHY:
     * - OpenFileMapping locates an existing named shared-memory mapping created
     *   by Assetto Corsa.
     * - MapViewOfFile makes that shared memory readable from this process.
     *
     * MSDN parameters to study:
     * - OpenFileMapping: dwDesiredAccess, bInheritHandle, lpName.
     * - MapViewOfFile: hFileMappingObject, dwDesiredAccess,
     *   dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap.
     */
    // TODO: Implement this step
    return false;
}

void ACTelemetryReader::disconnect()
{
    /**
     * TODO: Implement this step
     *
     * WHAT to do:
     * - If physicsView_ is not null, call UnmapViewOfFile with the mapped base
     *   address.
     * - If mappingHandle_ is not null, call CloseHandle with the mapping handle.
     * - Reset physicsView_ and mappingHandle_ after releasing them.
     *
     * WHY:
     * - The mapped view and mapping handle are separate OS resources.
     * - Releasing them prevents leaks and prevents later reads through an
     *   invalid mapped-memory pointer.
     *
     * MSDN parameters to study:
     * - UnmapViewOfFile: lpBaseAddress.
     * - CloseHandle: hObject.
     */
    // TODO: Implement this step
}

const SPageFilePhysics* ACTelemetryReader::fetchLatestFrame()
{
    /**
     * TODO: Implement this step
     *
     * WHAT to do:
     * - Return physicsView_ when a shared-memory view is currently mapped.
     * - Return nullptr when connect() has not succeeded.
     *
     * WHY:
     * - The simulator updates the shared page in place, so the mapped pointer is
     *   your live view of the latest physics frame.
     * - Returning nullptr for disconnected state makes the polling loop handle
     *   startup and simulator-not-running cases explicitly.
     *
     * Logic topics to study:
     * - Pointer lifetime after MapViewOfFile and UnmapViewOfFile.
     * - Using packetId to detect new frames or repeated frames.
     * - Avoiding reads while disconnected.
     */
    // TODO: Implement this step
    return nullptr;
}

ACTelemetryReader::~ACTelemetryReader()
{
    /**
     * TODO: Implement this step
     *
     * WHAT to do:
     * - Call disconnect() from here after disconnect() is implemented.
     *
     * WHY:
     * - This makes ACTelemetryReader own its Windows resources using RAII.
     * - Callers should not have to remember cleanup on every exit path.
     *
     * Design topics to study:
     * - Destructors and noexcept expectations.
     * - Idempotent cleanup functions.
     */
    // TODO: Implement this step
}
