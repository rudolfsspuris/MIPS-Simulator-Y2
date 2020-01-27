//C++ 11
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdint>
#include <stdint.h>
#include "decode.h"
#include "mem_map.hpp"
#include "reg_file.hpp"
#include "PC.hpp"

const uint8_t ret_status_except_arith = 0b00000001;
const uint8_t ret_status_except_memom = 0b00000010;
const uint8_t ret_status_except_instr = 0b00000100;



int main(int argc, char *argv[]){

    //load program binary specified as commandline parameter argv[1]
    if(argv[1] == NULL){ std::cout << "No file specified" << std::endl; return 0; }
    std::string filename = argv[1];
    std::ifstream binaryfile;
    binaryfile.open(filename);
    std::vector<char> instructions;

    if ( binaryfile.is_open() ) {
        // get length of binary
        binaryfile.seekg (0, binaryfile.end);
        int length = binaryfile.tellg();
        binaryfile.seekg (0, binaryfile.beg);

        // read the file 8 bits (1 character) at a time
        char* instr_array;
        instr_array = new char [length];
        binaryfile.read(instr_array, length);

        // copy array of instructions segmented into chars into a vector
        for(int i=0; i < length; i++){
            instructions.push_back(instr_array[i]);
        }
        delete[] instr_array;
        binaryfile.close();
    }
    else{ std::cout << "No file called " << filename << std::endl; }


    // load instructions into memory
    mem_map memory(instructions);

    // Initialise exception and PC
    uint8_t except = 0;
    PC prog_count;

    // create and initialise the register file
    reg_file regist;
    regist.initialise();



    //=============FOR TESTING============
     // for(int i = 0; i<32; i++){
     //   regist.write_reg(i, i);
     // }
     // regist.write_reg(9, 0x80000000);
     // regist.write_reg(10, 1);
     // regist.write_reg(11, 0xFFFFFFFF); // R11 = $t3 = 0xFFFF FFFF overflow testing
     // regist.write_reg(12, 0x7FFFFFFF); // R12 = $t4 = 0x7FFF FFFF signed overflow testing
     // regist.write_reg(13, 0x1FFFFFFF); // R13 = $t5 = 0x1FFF FFFF signed overflow testing
     // regist.write_reg(15, 0x20000000); // R15 = $t7 is the base adress of 0x20000000 for memory reads
     // regist.write_reg(14, 0x30000000); // R14 = $t6 is the base adress of 0x30000000 for reading getc
    //===========END OF TESTING===========





    for(;;){ // run instructions sequentially from memory


      //Read next instruction
      uint cur_intr;

      if(prog_count.getInstr()%4!=0){
        memory.manual_deallocate();   //Deallocate memory
        exit(-11);
      }

      memory.read_val(prog_count.getInstr(), readInstr, cur_intr, except);


      //Checking for exceptions
      if(except==except_r_intsr){
        memory.manual_deallocate();   //Deallocate memory
        exit(-11);
      }
      else if(except==except_r_null){
        memory.manual_deallocate();   //Deallocate memory
        exit(uint8_t(regist.read_reg(2)));
      }

      //Execute instruction
      decode(cur_intr, memory, prog_count, regist, except);

      //increment PC
      prog_count.increment();

      //Checking for exceptions

      if(except == ret_status_except_arith){
        memory.manual_deallocate();   //Deallocate memory
        exit(-10);
      }
      if(except==ret_status_except_memom){
        memory.manual_deallocate();   //Deallocate memory
        exit(-11);
      }
      if(except == ret_status_except_instr){
        memory.manual_deallocate();   //Deallocate memory
        exit(-12);
      }


      //=============FOR TESTING============
      // regist.print(); // Testing output
      //===========END OF TESTING===========

    }


    return 0;
}
