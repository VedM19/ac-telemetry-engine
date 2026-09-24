#pragma once

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

class ACTelemetryReader
{
public:
    ACTelemetryReader() = default;

    bool connect();

    void disconnect();

    const SPageFilePhysics *fetchLatestFrame();

    ~ACTelemetryReader();

private:
    void *mappingHandle_ = nullptr;
    const SPageFilePhysics *physicsView_ = nullptr;
};
