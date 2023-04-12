#include "unity.h"
#include <Wire.h>
 #include <SPI.h>
#include "../lib/DrivePacket/DrivePacket.h"

// unity testing docs: https://github.com/ThrowTheSwitch/Unity

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_function_should_serialize_and_deserialize_without_error(void) {
  // more test stuff
  DrivePacket testPacket = DrivePacket(33, 42);
  char *buffer;
  String serializedString = DrivePacket::Serialize(&testPacket);

  Serial.println(buffer);
  serializedString.toCharArray(buffer, DRIVE_PACKET_SIZE);
  TEST_ASSERT_EQUAL_STRING(buffer, "33,42");

  DrivePacket resultPacket;
  resultPacket = DrivePacket::Deserialize(String(buffer));
  

  TEST_ASSERT_EQUAL(testPacket.Data.leftMotorPower, resultPacket.Data.leftMotorPower);
  TEST_ASSERT_EQUAL(testPacket.Data.rightMotorPower, resultPacket.Data.rightMotorPower);
}

int runDrivePacketTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_serialize_and_deserialize_without_error);
  return UNITY_END();
}

// WARNING!!! PLEASE REMOVE UNNECESSARY MAIN IMPLEMENTATIONS //

/**
  * For native dev-platform or for some embedded frameworks
  */
int main(void) {
  return runDrivePacketTests();
}


/**
  * For Arduino framework
  */
void setup() {
  // Wait ~2 seconds before the Unity test runner
  // establishes connection with a board Serial interface
  delay(2000);

  runDrivePacketTests();
}
void loop() {}

/**
  * For ESP-IDF framework
  */
void app_main() {
  runDrivePacketTests();
}