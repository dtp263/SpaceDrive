#include <Arduino.h>
#include <DualMotorController.h>
#include <DrivePacket.h>
#include <Wire.h>
#include <SPI.h>

#include<SoftwareSerial.h>
SoftwareSerial mySUART(2, 3); //mySUART(SRX, STX)
DualMotorController motorController = DualMotorController(5, 6);

DrivePacket drivePacket;
char *drivePacketBuffer = (char*)malloc(DRIVE_PACKET_SIZE);

void setup() {
  

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Start I2C communication with the Multiplexer
  // Wire.begin();
    
  // motorController.Setup();

  Serial.begin(9600);
  mySUART.begin(9600);

}


void loop() {
  if (mySUART.available()) {
    Serial.println("uart is available... reading bytes.");
    // char data_rcvd = mySUART.read();   // read one byte from serial buffer and save to data_rcvd
    

    // if(data_rcvd == '1') digitalWrite(LED_BUILTIN, HIGH);  // switch LED On
    // if(data_rcvd == '0') digitalWrite(LED_BUILTIN, LOW);   // switch LED Off

    // char buffer[DRIVE_PACKET_SIZE];
    // char *drivePacketBuffer;
    mySUART.setTimeout(2000);
    int returnBytes = mySUART.readBytes(drivePacketBuffer, DRIVE_PACKET_SIZE);
    Serial.println("finished reading bytes.");

    Serial.println("deserializing...");
    Serial.println(drivePacketBuffer);
    drivePacket = DrivePacket::Deserialize(String(drivePacketBuffer));
    Serial.println("finished deserializing.");

    // int returnBytes = mySUART.readBytes(buffer, DRIVE_PACKET_SIZE);
    


    if(drivePacket.Data.leftMotorPower >= 20) digitalWrite(LED_BUILTIN, HIGH);  // switch LED On
    if(drivePacket.Data.leftMotorPower < 20) digitalWrite(LED_BUILTIN, LOW);   // switch LED Off

    Serial.println(drivePacket.Data.leftMotorPower);
    if (returnBytes == DRIVE_PACKET_SIZE) {
      Serial.println("true");
    } else {
      Serial.println("false");
      Serial.println(returnBytes);
    }
    delay(1000);
  } else {
    Serial.println("no uart available");
    delay(1000);
  }

  
  

  // motorController.WritePowerToMotorAsPercentage(0, 0);
}
