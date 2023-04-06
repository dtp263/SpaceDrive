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


        // // int *q = (int *)out;

        // // *q = in->Data.leftMotorPower;
        // // q++;
        // // *q = in->Data.rightMotorPower;
        // // q++;

        // StaticJsonDocument<200> doc;
        // doc["left"] = in->Data.leftMotorPower;
        // doc["right"] = in->Data.rightMotorPower;

        // serializeJson(doc, out, DRIVE_PACKET_SIZE);

        
        // // outString.toCharArray(out, DRIVE_PACKET_SIZE);

        // // sprintf(out, "Packet{ Left: %d, Right: %d }", in->Data.leftMotorPower, in->Data.rightMotorPower);
    }

    static DrivePacket Deserialize(String in)
    {
        DrivePacket tmp;
        char *delimeter = ",";
        Serial.print("In: ");
        Serial.print(in);
        Serial.print("\n");

        int index = 0;
        int nextIndex;
        nextIndex = in.indexOf(delimeter,index);
        tmp.Data.leftMotorPower = in.substring(index, nextIndex).toInt();

        index = nextIndex+1;
        nextIndex = in.indexOf(delimeter,index);
        tmp.Data.rightMotorPower = in.substring(index, nextIndex).toInt();

        // tmp.Data.leftMotorPower = atoi(strtok(in, delimeter));
        // // somethingElse = strtod(endptr + 1, &endptr);  // +1 to skip the comma.
        // tmp.Data.rightMotorPower = atoi(strtok(in, delimeter));

        return tmp;
        // Serial.println(in);
        // StaticJsonDocument<200> doc;
        // // Deserialize the JSON document
        // DeserializationError error = deserializeJson(doc, in);

        // // Test if parsing succeeds.
        // if (error) {
        //     Serial.print(F("deserializeJson() failed: "));
        //     Serial.println(error.f_str());
        //     return;
        // }

        // out->Data.leftMotorPower = doc["left"];
        // out->Data.rightMotorPower = doc["right"];

        // // int *q = (int *)in;
        // // out->Data.leftMotorPower = *q;
        // // q++;
        // // out->Data.rightMotorPower = *q;
        // // q++;

        
    }
};

#endif
