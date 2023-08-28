#ifndef STRUCTS_HPP
#define STRUCTS_HPP


struct SensorOutput
{
  float values[6];   // sensorArray sensor;
  uint32_t timestamp;   // timestamp packetTime;
};



union PacketBuffer
{
  SensorOutput packet;
  uint8_t buffer[sizeof(SensorOutput)];
};

#endif