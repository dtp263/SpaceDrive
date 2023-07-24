#ifndef DRIVE_PACKET_H
#define DRIVE_PACKET_H

#include <Arduino.h>
#include <ArduinoJson.h>

#define DRIVE_PACKET_SIZE sizeof(struct DrivePacketData) + 16

const char *START_DELIMETER = "X";
const char *DELIMETER = ",";
const char *END_DELIMETER = "$";

struct DrivePacketData
{
    int leftMotorPower;
    int rightMotorPower;
    int direction;
};

class DrivePacket
{
private:

public:
    DrivePacketData Data;

    DrivePacket()
    {
        DrivePacket(0, 0);
    }

    DrivePacket(int leftPower, int rightPower, char direction = 0)
    {
        Data.leftMotorPower = leftPower;
        Data.rightMotorPower = rightPower;
        Data.direction = direction;
    }
    
    static String Serialize(DrivePacket *in)
    {
        char buffer[DRIVE_PACKET_SIZE];
        
        sprintf(
            buffer,
            "%s%s%d%s%d%s%d%s%s", 
            START_DELIMETER,
            DELIMETER,
            in->Data.direction,
            DELIMETER,
            in->Data.leftMotorPower, 
            DELIMETER,
            in->Data.rightMotorPower,
            DELIMETER,
            END_DELIMETER
        );

        return String(buffer);
    }

    // deserialize a string into the DrivePacket values.
    static bool Deserialize(String in, DrivePacket *out)
    {
        int index = 1;
        int nextIndex;

        nextIndex = in.indexOf(DELIMETER,index);
        out->Data.direction = in.substring(index, nextIndex).toInt();

        index = nextIndex+1;
        nextIndex = in.indexOf(DELIMETER,index);
        out->Data.leftMotorPower = in.substring(index, nextIndex).toInt();

        index = nextIndex+1;
        nextIndex = in.indexOf(DELIMETER,index);
        out->Data.rightMotorPower = in.substring(index, nextIndex).toInt();

        return true;
    }
};

#endif
