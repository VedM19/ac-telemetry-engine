#pragma once

/**
 * Assetto Corsa physics shared-memory page layout.
 *
 * This struct is intended to mirror the beginning of Assetto Corsa's
 * "Local\\acpmef_physics" shared-memory block. When you implement the reader,
 * the bytes returned by MapViewOfFile will be interpreted as this layout.
 *
 * Learning TODO:
 * - Verify the complete official Assetto Corsa shared-memory layout before
 *   using this in production.
 * - Confirm field order, field sizes, array lengths, packing, and alignment.
 * - Look up how C++ struct layout and padding work on MSVC for C-compatible
 *   telemetry structs.
 */

 #pragma pack(push, 4)
struct SPageFilePhysics
{
    int packetId = 0;
    float gas = 0;
    float brake = 0;
    float fuel = 0;
    int gear = 0;
    int rpms = 0;
    float steerAngle = 0;
    float speedKmh = 0;
    float velocity[3];
    float accG[3];
    float wheelSlip[4];
    float wheelLoad[4];
    float wheelsPressure[4];
    float wheelAngularSpeed[4];
    float tyreWear[4];
    float tyreDirtyLevel[4];
    float tyreCoreTemperature[4];
    float camberRAD[4];
    float suspensionTravel[4];
    float drs = 0;
    float tc = 0;
    float heading = 0;
    float pitch = 0;
    float roll = 0;
    float cgHeight;
    float carDamage[5];
    int numberOfTyresOut = 0;
    int pitLimiterOn = 0;
    float abs = 0;
    float kersCharge = 0;
    float kersInput = 0;
    int autoShifterOn = 0;
    float rideHeight[2];
    float turboBoost = 0;
    float ballast = 0;
    float airDensity = 0;
};
#pragma pack(pop)


/**
 * Stubbed RAII-style reader for Assetto Corsa physics telemetry.
 *
 * This class is intentionally incomplete. It is a learning scaffold for
 * opening a Windows file mapping object, mapping a view of shared memory, and
 * exposing the mapped bytes as an SPageFilePhysics pointer.
 *
 * Learning TODO:
 * - Study the Windows shared-memory APIs in MSDN:
 *   OpenFileMapping, MapViewOfFile, UnmapViewOfFile, and CloseHandle.
 * - Understand HANDLE lifetime ownership and when mapped pointers become
 *   invalid.
 */
class ACTelemetryReader
{
public:
    ACTelemetryReader() = default;

    /**
     * Connect to Assetto Corsa's physics shared-memory mapping.
     *
     * WHAT to implement:
     * - Open the named Windows file mapping object for Assetto Corsa physics:
     *   "Local\\acpmef_physics".
     * - Map a read-only view of that mapping into this process.
     * - Store both the mapping handle and mapped pointer in the private members.
     *
     * WHY this is needed:
     * - Assetto Corsa publishes telemetry through a named shared-memory region.
     * - Your process must first acquire a HANDLE to that mapping, then map it
     *   into its own virtual address space before it can read telemetry fields.
     *
     * MSDN parameters to look up:
     * - OpenFileMapping: dwDesiredAccess, bInheritHandle, lpName.
     * - MapViewOfFile: hFileMappingObject, dwDesiredAccess,
     *   dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap.
     *
     * TODO:
     * - Decide what access flags are appropriate for read-only telemetry.
     * - Decide how to report failure when the simulator is not running.
     */
    bool connect();

    /**
     * Disconnect from the Assetto Corsa physics shared-memory mapping.
     *
     * WHAT to implement:
     * - If a view is currently mapped, unmap it.
     * - If a mapping handle is currently open, close it.
     * - Reset stored members so later calls cannot accidentally use stale state.
     *
     * WHY this is needed:
     * - MapViewOfFile and OpenFileMapping acquire OS resources that must be
     *   released.
     * - A pointer returned by MapViewOfFile is only valid until UnmapViewOfFile
     *   succeeds.
     *
     * MSDN parameters to look up:
     * - UnmapViewOfFile: lpBaseAddress.
     * - CloseHandle: hObject.
     *
     * TODO:
     * - Handle repeated disconnect calls safely.
     * - Decide whether cleanup failures should be logged, asserted, or ignored.
     */
    void disconnect();

    /**
     * Return the latest mapped physics frame.
     *
     * WHAT to implement:
     * - Return a typed pointer to the currently mapped shared-memory block.
     * - If no view is mapped, return nullptr.
     *
     * WHY this is needed:
     * - Assetto Corsa updates the shared-memory page in place.
     * - Reading through this pointer gives your process access to the most
     *   recent telemetry values without copying the entire page.
     *
     * Logic details to look up:
     * - How to safely reinterpret a mapped byte address as a C++ struct pointer.
     * - Whether volatile, atomic snapshots, or packetId comparison are useful
     *   for detecting partial updates in your polling loop.
     *
     * TODO:
     * - Add any safety checks you want before exposing the mapped pointer.
     */
    const SPageFilePhysics *fetchLatestFrame();

    /**
     * RAII cleanup destructor.
     *
     * WHAT to implement:
     * - Call disconnect() so mapped views and handles are released when the
     *   reader object goes out of scope.
     *
     * WHY this is needed:
     * - RAII keeps resource ownership local and makes early returns or
     *   exceptions less likely to leak Windows handles.
     *
     * TODO:
     * - Keep destructor cleanup minimal and noexcept-friendly.
     */
    ~ACTelemetryReader();

private:
    void *mappingHandle_ = nullptr;
    const SPageFilePhysics *physicsView_ = nullptr;
};
