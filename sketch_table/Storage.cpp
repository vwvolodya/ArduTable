#include "Storage.h"
#include <EEPROM.h>
#include <Arduino.h>

Storage::Storage(unsigned int address){
  this->address = address;
}
void Storage::set(int number){
  EEPROM.write(this->address, number);
  this->init();
}

int Storage::get(){
  EEPROM.read(this->address);
}

void Storage::init(){
  EEPROM.write(this->address, 0);   //ensuring there is no rubbish after init. Just 0
}

void Storage::setLong(long number){
  byte four = (number & 0xFF);
  byte three = ((number >> 8) & 0xFF);
  byte two = ((number >> 16) & 0xFF);
  byte one = ((number >> 24) & 0xFF);
  
  EEPROM.write(this->address, four);
  EEPROM.write(this->address + 1, three);
  EEPROM.write(this->address + 2, two);
  EEPROM.write(this->address + 3, one);
}

long Storage::getLong(){
  long four = EEPROM.read(this->address);
  long three = EEPROM.read(this->address + 1);
  long two = EEPROM.read(this->address + 2);
  long one = EEPROM.read(this->address + 3);
  
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}