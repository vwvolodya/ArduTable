#ifndef MY_STORAGE_H
#define MY_STORAGE_H
#include <EEPROM.h>
class Storage{
  private:
  unsigned int address;

  public:
  Storage(unsigned int address);
  void set(int number);
  int get();
  void init();

  void setLong(long number);
  long getLong();

};
#endif