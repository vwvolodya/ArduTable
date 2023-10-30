#include "Storage.h"
#include <EEPROM.h>

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