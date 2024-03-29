#ifndef DRIVE_PACKET_H
#define DRIVE_PACKET_H

#include <Arduino.h>
#include <ArduinoJson.h>

#define DRIVE_PACKET_SIZE sizeof(struct DrivePacketData) + 16

const char *START_DELIMITER = "X";
const char *DELIMITER = ",";
const char *END_DELIMITER = "$";

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
            START_DELIMITER,
            DELIMITER,
            in->Data.direction,
            DELIMITER,
            in->Data.leftMotorPower, 
            DELIMITER,
            in->Data.rightMotorPower,
            DELIMITER,
            END_DELIMITER
        );

        return String(buffer);
    }

    // deserialize a char pointer into two ints delineated by a comma and return a DrivePacket
    static bool Deserialize(String in, DrivePacket *out)
    {
        int index = 1;
        int nextIndex;

        nextIndex = in.indexOf(DELIMITER,index);
        out->Data.direction = in.substring(index, nextIndex).toInt();

        index = nextIndex+1;
        nextIndex = in.indexOf(DELIMITER,index);
        out->Data.leftMotorPower = in.substring(index, nextIndex).toInt();

        index = nextIndex+1;
        nextIndex = in.indexOf(DELIMITER,index);
        out->Data.rightMotorPower = in.substring(index, nextIndex).toInt();

        return true;
    }
};

#endif
