namespace TelemetryConsumer.Models;

using System.Runtime.InteropServices;
[StructLayout(LayoutKind.Sequential, Pack = 4)]
public unsafe struct SPageFilePhysics
{
    public int packetId;
    public float gas;
    public float brake;
    public float fuel;
    public int gear;
    public int rpms;
    public float steerAngle;
    public float speedKmh;
    public fixed float velocity[3];
    public fixed float accG[3];
    public fixed float wheelSlip[4];
    public fixed float wheelLoad[4];
    public fixed float wheelsPressure[4];
    public fixed float wheelAngularSpeed[4];
    public fixed float tyreWear[4];
    public fixed float tyreDirtyLevel[4];
    public fixed float tyreCoreTemperature[4];
    public fixed float camberRAD[4];
    public fixed float suspensionTravel[4];
    public float drs;
    public float tc;
    public float heading;
    public float pitch;
    public float roll;
    public float cgHeight;
    public fixed float carDamage[5];
    public int numberOfTyresOut;
    public int pitLimiterOn;
    public float abs;
    public float kersCharge;
    public float kersInput;
    public int autoShifterOn;
    public fixed float rideHeight[2];
    public float turboBoost;
    public float ballast;
    public float airDensity;
}