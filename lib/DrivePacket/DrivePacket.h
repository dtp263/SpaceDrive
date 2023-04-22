#ifndef DRIVE_PACKET_H
#define DRIVE_PACKET_H

#include <Arduino.h>
#include <ArduinoJson.h>

#define DRIVE_PACKET_SIZE 24

struct DrivePacketData
{
    int leftMotorPower;
    int rightMotorPower;
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

    DrivePacket(int leftPower, int rightPower)
    {
        Data.leftMotorPower = leftPower;
        Data.rightMotorPower = rightPower;
    }

    static String Serialize(DrivePacket *in)
    {
        char buffer[DRIVE_PACKET_SIZE];
        
        sprintf(
            buffer,
            "%d,%d", 
            in->Data.leftMotorPower, 
            in->Data.rightMotorPower
        );

        return String(buffer);
    }

    static DrivePacket Deserialize(String in)
    {
        DrivePacket tmp;
        char *delimeter = ",";
        // Serial.print("In: ");
        // Serial.print(in);
        // Serial.print("\n");

        int index = 0;
        int nextIndex;
        nextIndex = in.indexOf(delimeter,index);
        tmp.Data.leftMotorPower = in.substring(index, nextIndex).toInt();

        index = nextIndex+1;
        nextIndex = in.indexOf(delimeter,index);
        tmp.Data.rightMotorPower = in.substring(index, nextIndex).toInt();

        return tmp;
    }
};

#endif
