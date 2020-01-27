#include "mem_map.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include <cstdint>
#include <stdint.h>

typedef unsigned int uint;


//Public functions
void mem_map::read_val (const uint &address, uint8_t funct, uint &value /*output*/, uint8_t &except) const{

  if(address==ADDR_NULL){
    except = except_r_null;
    value = 0;
    return;
  }

  if(funct==readInstr){
    if(address<ADDR_INSTR || address>=ADDR_INSTR_OOB-3){
      except = except_r_intsr;
      value = 0;
      return;
    }
    else{
      value = generate_data(address, funct);
      except = 0;
      return;
    }
  }

  if(funct==readData || funct==readWordLeft0){

    if(address == ADDR_GETC){
      value = readGETC();
      except = 0;
      return;
    }
    else if(address<ADDR_DATA_OOB -3 && address >=ADDR_DATA){
      value = generate_data(address, funct);
      except = 0;
      return;
    }
    else if(address<ADDR_INSTR_OOB -3 && address >=ADDR_INSTR){
      value = generate_data(address, readInstr);
      except = 0;
      return;
    }
    else{
      except = except_r_data;
      value = 0;
      return;
    }
  }

  if(funct==readDataByteSigned){

    if(address == ADDR_GETC){
      uint8_t a = readGETC() >>24;
      int8_t b = a;
      int s = b;
      value = s;
      except = 0;
      return;
    }

    if(address == ADDR_GETC + 1){
      uint8_t a = readGETC() >>16;
      int8_t b = a;
      int s = b;
      value = s;
      except = 0;
      return;
    }

    if(address == ADDR_GETC + 2){
      uint8_t a = readGETC() >>8;
      int8_t b = a;
      int s = b;
      value = s;
      except = 0;
      return;
    }

    if(address == ADDR_GETC + 3){
      uint8_t a = readGETC();
      int8_t b = a;
      int s = b;

      value = s;
      except = 0;
      return;
    }

    if(address<ADDR_DATA_OOB && address>=ADDR_DATA){
      uint8_t s = data[address- ADDR_DATA];
      int8_t u = s;
      int s32 = u;
      uint u32 = s32;
      value = u32;
      return;
    }
    else if(address<ADDR_INSTR_OOB && address >=ADDR_INSTR){
      uint8_t s = instr[address- ADDR_INSTR];
      int8_t u = s;
      int s32 = u;
      uint u32 = s32;
      value = u32;
      return;
    }
    else{
      except = except_r_data;
      value = 0;
      return;
    }
  }

  if(funct==readDataByteUnsigned){

    if(address == ADDR_GETC){
      uint8_t a = readGETC() >>24;
      value = a;
      except = 0;
      return;
    }

    if(address == ADDR_GETC + 1){
      uint8_t a = readGETC() >>16;
      value = a;
      except = 0;
      return;
    }

    if(address == ADDR_GETC + 2){
      uint8_t a = readGETC() >>8;
      value = a;
      except = 0;
      return;
    }

    if(address == ADDR_GETC + 3){
      uint8_t a = readGETC();
      value = a;
      except = 0;
      return;
    }

    if(address<ADDR_DATA_OOB && address>=ADDR_DATA){
      uint8_t s = data[address- ADDR_DATA];
      uint u32 = s;
      value = u32;
      return;
    }
    else if(address<ADDR_INSTR_OOB && address >=ADDR_INSTR){
      uint8_t s = instr[address- ADDR_INSTR];
      uint u32 = s;
      value = u32;
      return;
    }
    else{
      except = except_r_data;
      value = 0;
      return;
    }
  }

  if(funct==readDataHalfSigned){

    if(address == ADDR_GETC){
      uint16_t a = readGETC() >>16;
      int16_t b = a;
      int s = b;
      value = s;
      except = 0;
      return;
    }

    if(address == ADDR_GETC + 2){
      uint16_t a = readGETC();
      int16_t b = a;
      int s = b;
      value = s;
      except = 0;
      return;
    }

    if(address<ADDR_DATA_OOB-1 && address>=ADDR_DATA){
      uint16_t i1 = data[address - ADDR_DATA + 0];
      uint16_t i0 = data[address - ADDR_DATA + 1];

      i1 = i1 << 8;

      uint16_t u16 = i1 + i0;
      int16_t s16 = u16;
      int s32 = s16;
      value = s32;
      return;
    }
    else if(address<ADDR_INSTR_OOB && address >=ADDR_INSTR){
      uint16_t i1 = instr[address - ADDR_INSTR + 0];
      uint16_t i0 = instr[address - ADDR_INSTR + 1];

      i1 = i1 << 8;

      uint16_t u16 = i1 + i0;
      int16_t s16 = u16;
      int s32 = s16;
      value = s32;
      return;
    }

    else{
      except = except_r_data;
      value = 0;
      return;
    }
  }

  if(funct==readDataHalfUnsigned){

    if(address == ADDR_GETC){
      uint16_t a = readGETC() >>16;
      value = a;
      except = 0;
      return;
    }

    if(address == ADDR_GETC + 2){
      uint16_t a = readGETC();
      value = a;
      except = 0;
      return;
    }

    if(address<ADDR_DATA_OOB - 1 && address>=ADDR_DATA){
      uint16_t i1 = data[address - ADDR_DATA + 0];
      uint16_t i0 = data[address - ADDR_DATA + 1];

      i1 = i1 << 8;

      uint16_t u16 = i1 + i0;
      uint u32 = u16;
      value = u32;
      return;
    }
    else if(address<ADDR_INSTR_OOB && address >=ADDR_INSTR){
      uint16_t i1 = instr[address - ADDR_INSTR + 0];
      uint16_t i0 = instr[address - ADDR_INSTR + 1];

      i1 = i1 << 8;

      uint16_t u16 = i1 + i0;
      uint u32 = u16;
      value = u32;
      return;
    }
    else{
      except = except_r_data;
      value = 0;
      return;
    }
  }

  if(funct==readWordLeft1){

    if(address == ADDR_GETC + 1){
      value = readGETC()<<8;
      except = 0;
      return;
    }
    else if(address<ADDR_DATA_OOB-2 && address>=ADDR_DATA){

      uint8_t i2 = data[address - ADDR_DATA + 0];
      uint8_t i1 = data[address - ADDR_DATA + 1];
      uint8_t i0 = data[address - ADDR_DATA + 2];

      uint a = i2<<24;
      uint b = i1<<16;
      uint c = i0<<8;

      value = a+b+c;

      except = 0;
      return;
    }
    else if(address<ADDR_INSTR_OOB && address >=ADDR_INSTR){

      uint8_t i2 = instr[address - ADDR_INSTR + 0];
      uint8_t i1 = instr[address - ADDR_INSTR + 1];
      uint8_t i0 = instr[address - ADDR_INSTR + 2];

      uint a = i2<<24;
      uint b = i1<<16;
      uint c = i0<<8;

      value = a+b+c;

      except = 0;
      return;
    }
    else{
      except = except_r_data;
      value = 0;
      return;
    }

  }

  if(funct==readWordLeft2){

    if(address == ADDR_GETC + 2){
      value = readGETC()<<16;
      except = 0;
      return;
    }
    else if(address<ADDR_DATA_OOB - 1 && address>=ADDR_DATA){
      uint8_t i2 = data[address - ADDR_DATA + 0];
      uint8_t i1 = data[address - ADDR_DATA + 1];

      uint a = i2<<24;
      uint b = i1<<16;

      value = a+b;

      except = 0;
      return;
    }
    else if(address<ADDR_INSTR_OOB && address >=ADDR_INSTR){
      uint8_t i2 = instr[address - ADDR_INSTR + 0];
      uint8_t i1 = instr[address - ADDR_INSTR + 1];

      uint a = i2<<24;
      uint b = i1<<16;

      value = a+b;

      except = 0;
      return;
    }
    else{
      except = except_r_data;
      value = 0;
      return;

    }


  }

  if(funct==readWordLeft3){
    if(address == ADDR_GETC + 3){
      value = readGETC()<<24;
      except = 0;
      return;
    }
    else if(address<ADDR_DATA_OOB && address>=ADDR_DATA){

      uint8_t i2 = data[address - ADDR_DATA + 0];

      uint a = i2<<24;
      value = a;

      except = 0;
      return;
    }
    else if(address<ADDR_INSTR_OOB && address >=ADDR_INSTR){
      uint8_t i2 = instr[address - ADDR_INSTR + 0];

      uint a = i2<<24;
      value = a;

      except = 0;
      return;
    }
    else{
      except = except_r_data;
      value = 0;
      return;
    }
  }

  if(funct==readWordRight0){

    if(address == ADDR_GETC){
      uint a = readGETC()>>24;
      value = a;
      except = 0;
      return;
    }

    else if(address<ADDR_DATA_OOB && address>=ADDR_DATA){

      uint8_t i0 = data[address - ADDR_DATA - 0];

      uint a = i0;
      value = a;

      except = 0;
      return;
    }

    else if(address<ADDR_INSTR_OOB && address >=ADDR_INSTR){
      uint8_t i0 = instr[address - ADDR_INSTR - 0];

      uint a = i0;
      value = a;

      except = 0;
      return;
    }

    else{
      except = except_r_data;
      value = 0;
      return;
    }

  }

  if(funct==readWordRight1){
    if(address == ADDR_GETC + 1){
      uint a = readGETC()>>16;
      value = a;
      except = 0;
      return;
    }

    else if(address<ADDR_DATA_OOB && address>=ADDR_DATA){
      uint8_t i0 = data[address - ADDR_DATA - 1];
      uint8_t i1 = data[address - ADDR_DATA - 0];

      uint a = i0<<8;
      uint b = i1;
      value = a+b;

      except = 0;
      return;
    }
    else if(address<ADDR_INSTR_OOB && address >=ADDR_INSTR){
      uint8_t i0 = instr[address - ADDR_INSTR - 1];
      uint8_t i1 = instr[address - ADDR_INSTR - 0];

      uint a = i0<<8;
      uint b = i1;
      value = a+b;

      except = 0;
      return;
    }


    else{

      except = except_r_data;
      value = 0;
      return;
    }


  }

  if(funct==readWordRight2){

    if(address == ADDR_GETC + 2){
      uint a = readGETC()>>8;
      value = a;
      except = 0;
      return;
    }

    else if(address<ADDR_DATA_OOB && address>=ADDR_DATA){
      uint8_t i0 = data[address - ADDR_DATA - 2];
      uint8_t i1 = data[address - ADDR_DATA - 1];
      uint8_t i2 = data[address - ADDR_DATA - 0];

      uint a = i0<<16;
      uint b = i1<<8;
      uint c = i2;
      value = a+b+c;

      except = 0;
      return;
    }
    else if(address<ADDR_INSTR_OOB && address >=ADDR_INSTR){
      uint8_t i0 = instr[address - ADDR_INSTR - 2];
      uint8_t i1 = instr[address - ADDR_INSTR - 1];
      uint8_t i2 = instr[address - ADDR_INSTR - 0];

      uint a = i0<<16;
      uint b = i1<<8;
      uint c = i2;
      value = a+b+c;

      except = 0;
      return;
    }
    else{
      except = except_r_data;
      value = 0;
      return;

    }
  }

  if(funct==readWordRight3){
    if(address == ADDR_GETC + 3){
      uint a = readGETC();
      value = a;
      except = 0;
      return;
    }

    else if(address<ADDR_DATA_OOB && address>=ADDR_DATA){
      uint8_t i0 = data[address - ADDR_DATA - 3];
      uint8_t i1 = data[address - ADDR_DATA - 2];
      uint8_t i2 = data[address - ADDR_DATA - 1];
      uint8_t i3 = data[address - ADDR_DATA - 0];

      uint a = i0<<24;
      uint b = i1<<16;
      uint c = i2<<8;
      uint d = i3;
      value = a+b+c+d;

      except = 0;
      return;
    }
    else if(address<ADDR_INSTR_OOB && address >=ADDR_INSTR){
      uint8_t i0 = instr[address - ADDR_INSTR - 3];
      uint8_t i1 = instr[address - ADDR_INSTR - 2];
      uint8_t i2 = instr[address - ADDR_INSTR - 1];
      uint8_t i3 = instr[address - ADDR_INSTR - 0];

      uint a = i0<<24;
      uint b = i1<<16;
      uint c = i2<<8;
      uint d = i3;
      value = a+b+c+d;

      except = 0;
      return;
    }


    else{
      except = except_r_data;
      value = 0;
      return;
    }
  }

  else{
    value = 0;
    except = except_false_f;
    return;
  }
}





void mem_map::write_val  (const uint &address, uint8_t funct, const uint value, uint8_t &except){

  if(funct == writeData){
    //Data
    if(address<ADDR_DATA_OOB-3 && address>=ADDR_DATA){

      uint8_t i0/*LSB, ADDR_DATA + 3*/, i1, i2, i3/*MSB, ADDR_DATA + 0*/;

      i0 = value;
      i1 = value>>8;
      i2 = value>>16;
      i3 = value>>24;

      data[address - ADDR_DATA + 3] = i0;
      data[address - ADDR_DATA + 2] = i1;
      data[address - ADDR_DATA + 1] = i2;
      data[address - ADDR_DATA + 0] = i3;

      except = 0;
      return;
    }

    //Output
    else if(address==ADDR_PUTC){
          writePUTC(value);
          return;
    }

    //Error cases
    else if(address<ADDR_INSTR_OOB-3 && address>=ADDR_INSTR){
      except = except_w_instr;
      return;
    }
    else if(address==0){
      except = except_w_NULL;
      return;
    }
    else{
      except = except_w_OOB;
      return;
    }
  }



  else if(funct == writeDataByte){

    if(address>=ADDR_PUTC && address<=ADDR_PUTC + 3){
      if(address%4==0){
        writePUTC(value<<24);
        return;
      }
      if(address%4==1){
        writePUTC(value<<16);
        return;
      }
      if(address%4==2){
        writePUTC(value<<8);
        return;
      }
      if(address%4==3){
        writePUTC(value);
        return;
      }

    }
    else if(address<ADDR_DATA_OOB && address>=ADDR_DATA){
      uint8_t i0 = value;
      data[address - ADDR_DATA] = i0;
      except = 0;
      return;
    }
    else if(address<ADDR_INSTR_OOB && address>=ADDR_INSTR){
      except = except_w_instr;
      return;
    }
    else if(address==0){
      except = except_w_NULL;
      return;
    }
    else{
      except = except_w_OOB;
      return;
    }
  }



  else if(funct == writeDataHalf){
    if(address>=ADDR_PUTC && address<=ADDR_PUTC + 3){
      if(address%4==0){
        writePUTC(value<<16);
        return;
      }
      if(address%4==2){
        writePUTC(value);
        return;
      }
    }

    if(address<ADDR_DATA_OOB - 1 && address>=ADDR_DATA){
      uint8_t i0 = value;
      uint8_t i1 = value>>8;

      data[address - ADDR_DATA + 1] = i0;
      data[address - ADDR_DATA + 0] = i1;

      except = 0;
      return;
    }

    else if(address<ADDR_INSTR_OOB-1 && address>=ADDR_INSTR){
      except = except_w_instr;
      return;
    }
    else if(address==0){
      except = except_w_NULL;
      return;
    }
    else{
      except = except_w_OOB;
      return;
    }


  }


}





mem_map::mem_map (const std::vector<char> &instructions){

  instr = new uint8_t[ADDR_INSTR_L];
  data = new uint8_t[ADDR_DATA_L];

  //Initialise data to 0
  for(int i = 0; i<ADDR_DATA_L; i++){
    data[i]=0;
  }

  //Initialise instructions
  for(int i = 0; i<instructions.size(); i++){
    instr[i] = instructions[i];
  }
  for(int i = instructions.size(); i<ADDR_INSTR_L; i++){
    instr[i] = 0;
  }

}








//Private support functions

uint mem_map::generate_data(const uint &address, uint8_t funct) const{

  uint i0;
  uint i1;
  uint i2;
  uint i3;
  uint output;

  if(funct==readInstr){
    i0 = uint(instr[address - ADDR_INSTR + 3]);
    i1 = uint(instr[address - ADDR_INSTR + 2]);
    i2 = uint(instr[address - ADDR_INSTR + 1]);
    i3 = uint(instr[address - ADDR_INSTR + 0]);

    i1 = i1<<8;
    i2 = i2<<16;
    i3 = i3<<24;

    output = i0 + i1 + i2 + i3;
    return output;
  }

  else if (funct==readData || funct==readWordLeft0 || readWordRight3){
    i0 = uint(data[address - ADDR_DATA + 3]);
    i1 = uint(data[address - ADDR_DATA + 2]);
    i2 = uint(data[address - ADDR_DATA + 1]);
    i3 = uint(data[address - ADDR_DATA + 0]);

    i1 = i1<<8;
    i2 = i2<<16;
    i3 = i3<<24;

    output = i0 + i1 + i2 + i3;
    return output;
  }

}

uint mem_map::readGETC() const{ // If there are no more characters (EOF), the memory read should return -1.
  int8_t a;
  //std::cin>>a; //correctly loading
  //std::cout << "User character input ";
  a = getchar(); // Read the next character in the stdin stream
  int32_t i0 = a; //Conversion to 32 bits with sign exted for a = -1
  return i0;
}

void mem_map::writePUTC(uint out){
  int8_t a = out;
  putchar(int(a));
  return;
}



mem_map::~mem_map(){
  delete[] data;
  delete[] instr;
  //std::cout<<"Destructor for memory called successfully"<<std::endl;
}

void mem_map::manual_deallocate(){ // this causes segfault as data+instr are deallocated by the destructor - useless?
  delete[] data;
  delete[] instr;
  //std::cout<<"Destructor for memory called manually"<<std::endl;
}
