using System;
using System.IO;
using System.IO.MemoryMappedFiles;
using System.Threading;
using TelemetryConsumer.Models;

namespace TelemetryConsumer;

class Program
{
    private const string MemoryMappedFileName = "acpmf_physics";
    
    static void Main(string[] args)
    {
        Console.Title = "Telemetry Consumer (.NET)";
        Console.WriteLine("[INFO] Searching for Assetto Corsa shared memory block...");

        MemoryMappedFile? mmf = null;

        // Loop until Assetto Corsa opens the shared memory block
        while (mmf == null)
        {
            try
            {
                mmf = MemoryMappedFile.OpenExisting(MemoryMappedFileName);
                Console.ForegroundColor = ConsoleColor.Green;
                Console.WriteLine("[CONNECTED] Hooked into Assetto Corsa physics memory!");
                Console.ResetColor();
            }
            catch (FileNotFoundException)
            {
                Console.WriteLine("[WAITING] acpmf_physics not found. Launch AC or enter session. Retrying in 2s...");
                Thread.Sleep(2000);
            }
        }

        // Proceed to reading once connected
        RunIngestionLoop(mmf);
    }

    static void RunIngestionLoop(MemoryMappedFile mmf)
    {
        using (var accessor = mmf.CreateViewAccessor(0, 0, MemoryMappedFileAccess.Read))
        {
            int lastPacketId = -1; // Initialize with an invalid packet ID
            Console.Clear();

            while (true)
            {
                // Read directly from byte offset 0 into your struct
                accessor.Read<SPageFilePhysics>(0, out var physics);

                // Only process when the physics engine publishes a new tick
                if (physics.packetId != lastPacketId)
                {
                    lastPacketId = physics.packetId;

                    // Render instantaneous telemetry
                    RenderDashboard(in physics);
                }

                // ~60 Hz polling rate (16ms)
                Thread.Sleep(16);
            }
        }
    }

    static unsafe void RenderDashboard(in SPageFilePhysics physics)
    {
        Console.SetCursorPosition(0, 0);
        Console.WriteLine("=================================================");
        Console.WriteLine("          LIVE TELEMETRY STREAM (.NET)           ");
        Console.WriteLine("=================================================");
        Console.WriteLine($"Packet ID : {physics.packetId,-10}");
        Console.WriteLine($"Speed     : {physics.speedKmh,6:F1} km/h");
        Console.WriteLine($"RPM       : {physics.rpms,5} | Gear: {physics.gear - 1}");
        Console.WriteLine($"Throttle  : {physics.gas * 100,5:F1}% | Brake: {physics.brake * 100,5:F1}%");
        Console.WriteLine("-------------------------------------------------");
        Console.WriteLine($"Tire Temp Front-Left : {physics.tyreCoreTemperature[0],5:F1} °C");
        Console.WriteLine($"Tire Temp Front-Right: {physics.tyreCoreTemperature[1],5:F1} °C");
        Console.WriteLine($"Tire Temp Rear-Left  : {physics.tyreCoreTemperature[2],5:F1} °C");
        Console.WriteLine($"Tire Temp Rear-Right : {physics.tyreCoreTemperature[3],5:F1} °C");
        Console.WriteLine("=================================================");
    }
}