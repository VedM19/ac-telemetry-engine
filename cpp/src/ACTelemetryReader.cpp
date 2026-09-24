#include "ACTelemetryReader.h"
#include <windows.h>

bool ACTelemetryReader::connect()
{
    mappingHandle_ = OpenFileMappingA(FILE_MAP_READ, FALSE, "Local\\acpmf_physics");
    physicsView_ = static_cast<SPageFilePhysics*>(MapViewOfFile(mappingHandle_, FILE_MAP_READ, 0, 0, 0));
    if (mappingHandle_ != nullptr && physicsView_ != nullptr) {
        return true;
    }
    return false;
}

void ACTelemetryReader::disconnect()
{
    if (physicsView_ != nullptr) {
        UnmapViewOfFile(physicsView_);
        physicsView_ = nullptr;
    }
    if (mappingHandle_ != nullptr) {
        CloseHandle(mappingHandle_);
        mappingHandle_ = nullptr;
    }
}

const SPageFilePhysics* ACTelemetryReader::fetchLatestFrame()
{
    if (physicsView_ != nullptr) {
        return physicsView_;
    }
    return nullptr;
}

ACTelemetryReader::~ACTelemetryReader()
{
    disconnect();
}
