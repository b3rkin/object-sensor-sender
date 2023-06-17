

#ifndef _UTIL_H
#define _UTIL_H


struct packetStructure
{
  float values[6];   // sensorArray sensor;
  unsigned long packetTime;   // timestamp packetTime;
};

union packetBuffer
{
  packetStructure packet;
  byte buffer[sizeof(packetStructure)];
};

#endif //_UTIL_H