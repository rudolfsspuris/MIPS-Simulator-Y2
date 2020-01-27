#include "reg_file.hpp"
#include <array>
#include <stdint.h>
#include <iostream>

typedef unsigned int uint;

void reg_file::write_reg(const uint& reg, const uint& data){

  if(reg!=0){
    registers[reg] = data;
  }

}

uint reg_file::read_reg(const uint& reg){
    return registers[reg];
}

void reg_file::write_HI(const uint& data){
    if(allow_write_HI){
        hi = data;
        allow_write_HI = false; // disallow writing twice in a row
    }
    else{ //UNPREDICTABLE behaviour
        hi = 666;
    }
}

uint reg_file::read_HI(){
    allow_write_HI = true; // allow writing
    return hi;
}

void reg_file::write_LO(const uint& data){
    if(allow_write_LO){
        lo = data;
        allow_write_LO = false; // disallow writing twice in a row
    }
    else{ //UNPREDICTABLE behaviour
        lo = 666;
    }
}

uint reg_file::read_LO(){
    allow_write_LO = true; // allow writing now that lo has been read
    return lo;
}

void reg_file::initialise(){
    for(int i=0; i < 32; i++){
        registers[i] = 0;
    }
    hi = 0;
    allow_write_HI = true;
    lo = 0;
    allow_write_LO = true;
}

//For testing
void reg_file::print(){
  for(int i = 0; i<32; i++){
    std::cout<<"Register " << i <<": "<< int(registers[i]) << std::endl;
  }
  std::cout<<"Register LO: "<< int(lo) << std::endl;
  std::cout<<"Register HI: "<< int(hi) << std::endl << std::endl;
}
