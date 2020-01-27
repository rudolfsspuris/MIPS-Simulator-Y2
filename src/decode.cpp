#include "mem_map.hpp"
#include "reg_file.hpp"
#include "PC.hpp"
#include <string>
#include <iostream>

typedef unsigned int uint;

#define INSTR_OPCODE     0b11111100000000000000000000000000    //ALL
#define INSTR_RS         0b00000011111000000000000000000000    //R and I
#define INSTR_RT         0b00000000000111110000000000000000    //R and I
#define INSTR_RD         0b00000000000000001111100000000000    //R
#define INSTR_SHAMT      0b00000000000000000000011111000000    //R
#define INSTR_FUNCT      0b00000000000000000000000000111111    //R
#define INSTR_IMMEDIATE  0b00000000000000001111111111111111    //I
#define INSTR_ADDRESS    0b00000011111111111111111111111111    //J
#define SIGN             0b10000000000000000000000000000000


const uint8_t except_overflow   = 0b00000001;
// const uint8_t readData          = 0b00000010;   //Read data memory, return value
// const uint8_t writeData         = 0b00000100;   //Write data to memory address specified

// const uint8_t except_r_null   = 0b00000001;   //Reading address 0
// const uint8_t except_r_intsr  = 0b00000010;   //Attempt to read instructions, address is OOB
// const uint8_t except_r_data   = 0b00000100;   //Attempt to read data, address is OOB

const uint8_t ret_status_except_arith = 0b00000001;
const uint8_t ret_status_except_memom = 0b00000010;
const uint8_t ret_status_except_instr = 0b00000100;




#include<cstdint>
#include<vector>

void decode(uint instruction, mem_map & memory, PC & prog_count, reg_file & regist, uint8_t &ret_status){

    uint32_t rs = (instruction & INSTR_RS) >> 21;
    uint32_t rt = (instruction & INSTR_RT) >> 16;
    uint32_t rd = (instruction & INSTR_RD) >> 11;
    uint32_t sa = (instruction & INSTR_SHAMT) >> 6;

    //////std::cout<<"Decoding the instruction"<<std::endl;
    switch(instruction & INSTR_OPCODE){



    case 0b00000000000000000000000000000000 :{//R-type


      switch(instruction & INSTR_FUNCT){
        case 0b00000000000000000000000000000000:{//AND, NOP, SLL



          if(instruction == 0b000000000000000000000000000000){//NOP

            //std::cout<<"Executing NOP"<<std::endl<<std::endl;
            regist.write_reg(rd, (regist.read_reg(rt) << sa ));

            break;
          }



          else{ // Shift Left Logical

            //std::cout<<"Executing SHIFT LEFT LOGICAL"<<std::endl<<std::endl;
            regist.write_reg(rd, (regist.read_reg(rt) << sa ));
            break;
          }
        }


        case 0b00000000000000000000000000100000:{ //Add Word

          //std::cout<<"Executing ADD WORD"<<std::endl<<std::endl;

          uint a = regist.read_reg(rs);
          uint b = regist.read_reg(rt);

          if(int(a) < 0 && int(b) < 0){           // if both negative
            uint c = a + b;

            if(int(c) < 0){                        // Result negative->no overflow
              regist.write_reg(rd, c);
              ret_status = 0;
              break;
            }
            else{                                   // Result positive->overflow
              ret_status = except_overflow;
              break;
            }
          }
          else if(int(a) > 0 && int(b) > 0){      // if both positive
            uint c = a + b;
            if(int(c) > 0){                        // Result positive->no overflow
              regist.write_reg(rd, c);
              ret_status = 0;
              break;
            }
            else{                                   // Result negative->overflow
              ret_status = except_overflow;
              break;
            }
          }
          else{
            uint c = a + b;
            regist.write_reg(rd, c);
            ret_status = 0;
            break;
          }

        }


        case 0b00000000000000000000000000100001:{//ADDU: Add Unsigned Word

          //std::cout<<"Executing ADD WORD UNSIGNED: R"<<rd<<" = R"<<rs<<" + R"<<rt<<std::endl<<std::endl;

          uint a = regist.read_reg(rs);
          uint b = regist.read_reg(rt);
          regist.write_reg(rd, a + b); //C++ addition is 'unsigned word' addition without overflow - good for ADDU
          break;
        }


        case 0b00000000000000000000000000100100:{//And

          //std::cout<<"Executing AND WORD"<<std::endl<<std::endl;

          uint a = regist.read_reg(rs);
          uint b = regist.read_reg(rt);
          uint AandB = a & b;
          regist.write_reg(rd, AandB);
          break;
        }


        case 0b00000000000000000000000000011010:{//Divide Word

          //std::cout<<"Executing DIVIDE WORD"<<std::endl<<std::endl;

          int a = regist.read_reg(rs);
          int b = regist.read_reg(rt);

          if(b==0){ // Result is not unpredictable;
            regist.write_HI(666);
            regist.write_LO(666);
            break;
          }

          int div = a/b;
          int rem = a%b;

          regist.write_HI(rem);
          regist.write_LO(div);

          break;
        }


        case 0b00000000000000000000000000011011:{//Divide Unsigned Word

          //std::cout<<"Executing DIVIDE UNSIGNED WORD"<<std::endl<<std::endl;

          uint a = regist.read_reg(rs);
          uint b = regist.read_reg(rt);
          if(b==0){
            regist.write_HI(666);
            regist.write_LO(666);
            break;
          }
          uint div = a/b;
          uint rem = a%b;

          regist.write_HI(rem);
          regist.write_LO(div);

          break;
        }


        case 0b00000000000000000000000000010000:{//Move from HI Register

          //std::cout<<"Executing MOVE FROM HI REGISTER"<<std::endl<<std::endl;

          regist.write_reg(rd, regist.read_HI());
          break;
        }


        case 0b00000000000000000000000000010010:{//Move From LO Register

          //std::cout<<"Executing MOVE FROM LO REGISTER"<<std::endl<<std::endl;

          regist.write_reg(rd, regist.read_LO());
          break;
        }


        case 0b00000000000000000000000000010001:{//Move to Hi Register

          //std::cout<<"Executing MOVE TO HI REGISTER"<<std::endl<<std::endl;

          regist.write_HI(regist.read_reg(rs));
          break;

        }


        case 0b00000000000000000000000000010011:{//Move to LO Register

          //std::cout<<"Executing MOVE TO LO REGISTER"<<std::endl<<std::endl;

          regist.write_LO(regist.read_reg(rs));
          break;
        }


        case 0b00000000000000000000000000011000:{//Multiply Word
          //std::cout<<"Executing MULTIPLY WORD"<<std::endl<<std::endl;

          long int a = int(regist.read_reg(rs));
          long int b = int(regist.read_reg(rt));

          long int result = a*b;


          uint to_hi = result>>32;
          uint to_lo = result;

          regist.write_HI(to_hi);
          regist.write_LO(to_lo);
          break;
        }


        case 0b00000000000000000000000000011001:{//Multiply Unsigned Word
          //std::cout<<"Executing MULTIPLY WORD UNSIGNED"<<std::endl<<std::endl;

          unsigned long int a = regist.read_reg(rs);
          unsigned long int b = regist.read_reg(rt);

          long int result = a*b;
          ////std::cout<<"result: "<< result << std::endl<<std::endl;

          uint to_hi = result>>32;
          uint to_lo = result;
          ////std::cout<<"to HI: "<< to_hi << std::endl<<std::endl;
          ////std::cout<<"to LO: "<< to_lo << std::endl<<std::endl;

          regist.write_HI(to_hi);
          regist.write_LO(to_lo);
          break;
        }



        case 0b00000000000000000000000000100101:{//Or

          //std::cout<<"Executing OR"<<std::endl<<std::endl;
          regist.write_reg(rd, regist.read_reg(rs) | regist.read_reg(rt));
          break;
        }



        case 0b00000000000000000000000000000100:{//Shift Word Left Logical Variable

          //std::cout<<"Executing SHIFT WORD LEFT LOGICAL VARIABLE"<<std::endl<<std::endl;
          regist.write_reg(rd, (regist.read_reg(rt) << (regist.read_reg(rs) & 0b00000000000000000000000000011111)));
          break;
        }



        case 0b00000000000000000000000000101010:{//Set on Less Than

          //std::cout<<"Executing SET ON LESS THAN"<<std::endl<<std::endl;
          int a = regist.read_reg(rs);
          int b = regist.read_reg(rt);

          if(a<b){
            regist.write_reg(rd, 0b00000000000000000000000000000001);
            break;
          }
          else{
            regist.write_reg(rd, 0b00000000000000000000000000000000);
            break;
          }
          ////std::cout<<"ERROR IN Executing SET ON LESS THAN"<<std::endl;
          break;
        }



        case 0b00000000000000000000000000101011:{//Set on Less Than Unsigned

          //std::cout<<"Executing SET ON LESS THAN"<<std::endl<<std::endl;
          uint a = regist.read_reg(rs);
          uint b = regist.read_reg(rt);

          if(a<b){
            regist.write_reg(rd, 0b00000000000000000000000000000001);
            break;
          }
          else{
            regist.write_reg(rd, 0b00000000000000000000000000000000);
            break;
          }
          break;

        }



        case 0b00000000000000000000000000000011:{//Shift Word Right Arithmetic

          //std::cout<<"Executing SHIFT WORD RIGHT ARITHMETIC"<<std::endl<<std::endl;

          int a = regist.read_reg(rt);
          regist.write_reg(rd, a >> sa);
          break;
        }



        case 0b00000000000000000000000000000111:{//Shift Word Right Arithmetic Variable

          //std::cout<<"Executing SHIFT WORD RIGHT ARITHMETIC VARIABLE"<<std::endl<<std::endl;

          int a = regist.read_reg(rt);
          regist.write_reg(rd, a >> (regist.read_reg(rs) & 0b00000000000000000000000000011111));
          break;
        }



        case 0b00000000000000000000000000000010:{//Shift Word Right Logical

          //std::cout<<"Executing SHIFT WORD RIGHT LOGICAL"<<std::endl<<std::endl;

          uint a = regist.read_reg(rt);
          regist.write_reg(rd, a >> sa);
          break;
        }



        case 0b00000000000000000000000000000110:{//Shift Word Right Logical Variable

          //std::cout<<"Executing SHIFT WORD RIGHT LOGICAL VARIABLE"<<std::endl<<std::endl;

          uint a = regist.read_reg(rt);
          regist.write_reg(rd, a >> (regist.read_reg(rs) & 0b00000000000000000000000000011111));

          break;
        }



        case 0b00000000000000000000000000100010:{//Subtract Word

          //std::cout<<"Executing SUBTRACT WORD"<<std::endl<<std::endl;

          uint a = regist.read_reg(rs);
          uint b = regist.read_reg(rt);
          uint c = a - b;

          if( int(a) < int(b) ) {       // if a < b, a- b < 0
            if(int(c) < 0){             // no overflow
                regist.write_reg(rd, c);
                ret_status = 0;
                break;
            }
            else{                       // overflow
                ret_status = except_overflow;
                break;
            }
          }
          else if( int(a) > int(b) ){   // if a > b, a-b > 0
            if(int(c) > 0){             // no overflow
                regist.write_reg(rd, c);
                ret_status = 0;
                break;
              }
              else{                       // overflow
                ret_status = except_overflow;
                break;
              }
          }

          else{                         // if a=b, no overflow
            regist.write_reg(rd, c);
            ret_status = 0;
            break;
          }
        }



        case 0b00000000000000000000000000100011:{//Subtract Unsigned Word

          //std::cout<<"Executing SUBTRACT UNSIGNED WORD"<<std::endl<<std::endl;
          uint a = regist.read_reg(rs);
          uint b = regist.read_reg(rt);
          regist.write_reg(rd, a - b);
          break;
        }



        case 0b00000000000000000000000000100110:{//Exclusive OR

          //std::cout<<"Executing EXCLUSIVE OR"<<std::endl<<std::endl;
          regist.write_reg(rd, regist.read_reg(rs) ^ regist.read_reg(rt));
          break;

        }


        case 0b00000000000000000000000000001001:{//Jump and Link Register

          //std::cout<<"Executing JUMP & LINK REGISTER"<<std::endl<<std::endl;
          if(rs == rd){
            //UNDEFINED BEHAVIOUR
            break;
          }

          uint a = regist.read_reg(rs);
          regist.write_reg(rd, prog_count.getInstr() + 8);

          prog_count.increment_jump(a);
          break;
        }


        case 0b00000000000000000000000000001000:{//Jump Register

          //std::cout<<"Executing JUMP REGISTER"<<std::endl<<std::endl;

          uint a = regist.read_reg(rs);
          prog_count.increment_jump(a);
          break;
        }


        default:
          ret_status =  ret_status_except_instr;


      }
      break;
    }


    case 0b00100000000000000000000000000000: {//Add Immediate Word

      //std::cout<<"Executing ADD IMMEDIATE WORD"<<std::endl<<std::endl;


      int16_t tmp = (instruction & INSTR_IMMEDIATE);
      int tmp2 = tmp; //sign extend;
      uint imm = tmp2; //uint for catching exceptions

      uint a = regist.read_reg(rs);

      if(int(a) < 0 && int(imm) < 0){         //if both negative
        uint c = a + imm;

        if(int(c) < 0){                         ////result should be negative
          regist.write_reg(rt, c);
          ret_status = 0;
          break;
        }
        else{                                     //Overflow
          ret_status = except_overflow;
          break;
        }
      }
      else if(int(a) > 0 && int(imm) > 0){      //if both positive
        uint c = a + imm;
        if(int(c) > 0){                           //result should be positive
          regist.write_reg(rt, c);
          ret_status = 0;
          break;
        }
        else{
          ret_status = except_overflow;
          break;
        }
      }
      else{
        int c = a + imm;
        regist.write_reg(rt, c);
        ret_status = 0;
        break;
      }

      break;
    }



    case 0b00100100000000000000000000000000: {//Add Immediate Word Unsigned

      //std::cout<<"Executing ADD IMMEDIATE WORD UNSIGNED"<<std::endl<<std::endl;


      uint a = regist.read_reg(rs);


      int16_t tmp = (instruction & INSTR_IMMEDIATE);
      int tmp2 = tmp; //sign extend;
      uint32_t imm = tmp2; //uint for unsigned addition
      uint32_t c = a+imm;

      regist.write_reg(rt, c); //C++ addition is 'unsigned word' addition without overflow - good for ADDU

      break;
    }



    case 0b00110000000000000000000000000000: {//And Immediate

      //std::cout<<"Executing AND IMMEDIATE"<<std::endl<<std::endl;

      uint16_t tmp = (instruction & INSTR_IMMEDIATE);
      uint imm = tmp; //zero extend??
      imm = imm & 0b00000000000000001111111111111111; // for safety I guess?

      regist.write_reg(rt, regist.read_reg(rs) & imm);
      break;
    }



    case 0b00110100000000000000000000000000: {//Or Immediate

      //std::cout<<"Executing OR IMMEDIATE"<<std::endl<<std::endl;

      uint16_t tmp = (instruction & INSTR_IMMEDIATE);
      uint imm = tmp; //zero extend??
      imm = imm & 0b00000000000000001111111111111111; // for safety I guess?

      regist.write_reg(rt, regist.read_reg(rs) | imm);
      break;

    }



    case 0b00111000000000000000000000000000: { //Exclusive Or Immediate

      //std::cout<<"Executing EXCLUSIVE OR IMMEDIATE"<<std::endl<<std::endl;

      uint16_t tmp = (instruction & INSTR_IMMEDIATE);
      uint imm = tmp; //zero extend??
      imm = imm & 0b00000000000000001111111111111111; // for safety I guess?

      regist.write_reg(rt, regist.read_reg(rs) ^ imm);
      break;

      break;
    }



    case 0b00010000000000000000000000000000: {//Branch on equal
      uint16_t  a = instruction;
      int16_t   b = a;
      int       c = b;
      c = c << 2;

      if(int(regist.read_reg(rs))==int(regist.read_reg(rt))){
        prog_count.increment_branch(c);
      }
      break;

    }



    case 0b00000100000000000000000000000000: {//Branches
      switch(instruction & INSTR_RT){
        case 0b00000000000000010000000000000000: {//Branch on Greater Than or Equal to Zero
          // //std::cout<<"Executing BRANCH on LESS THAN ZERO"<<std::endl<<std::endl;

          uint16_t  a = instruction;
          int16_t   b = a;
          int       c = b;
          c = c << 2;

          if(int(regist.read_reg(rs))>=0){
            prog_count.increment_branch(c);
          }
          break;
        }

        case 0b00000000000100010000000000000000: { //Branch on Greater Than or Equal to Zero and Link
          uint16_t  a = instruction;
          int16_t   b = a;
          int       c = b;
          c = c << 2;

          if(rs == 31){
            break;
            //GPR 31 must not be used for the source register rs, because such an
            //instruction does not have the same effect when
            //reexecuted. The result of executing such an instruction is UNPREDICTABLE
          }

          regist.write_reg(31, prog_count.getInstr() + 8);


          if(int(regist.read_reg(rs))>=0){
            prog_count.increment_branch(c);
          }
          break;
        }

        case 0b00000000000000000000000000000000:{//Branch on Less Than Zero
          ////std::cout<<"Executing BRANCH on LESS THAN ZERO"<<std::endl<<std::endl;

          uint16_t  a = instruction;
          int16_t   b = a;
          int       c = b;
          c = c << 2;

          if(int(regist.read_reg(rs))<0){
            prog_count.increment_branch(c);
          }
          break;


        }

        case 0b00000000000100000000000000000000: {//Branch on Less Than Zero and Link
          ////std::cout<<"Executing BRANCH on LESS THAN ZERO AND LINK"<<std::endl<<std::endl;

          uint16_t  a = instruction;
          int16_t   b = a;
          int       c = b;
          c = c << 2;

          if(rs == 31){
            break;
            //GPR 31 must not be used for the source register rs, because such an
            //instruction does not have the same effect when
            //reexecuted. The result of executing such an instruction is UNPREDICTABLE
          }

          regist.write_reg(31, prog_count.getInstr() + 8);


          if(int(regist.read_reg(rs))<0){
            prog_count.increment_branch(c);
          }
          break;
        }

        default: {
          ret_status =  ret_status_except_instr;
          break;
        }
      }

      break;
    }



    case 0b00011100000000000000000000000000: {//Branch on Greater Than Zero
      //std::cout<<"Executing BRANCH on GREATER THAN ZERO"<<std::endl<<std::endl;

      uint16_t  a = instruction;
      int16_t   b = a;
      int       c = b;
      c = c << 2;

      if(int(regist.read_reg(rs))>0){
        prog_count.increment_branch(c);
      }
      break;

    }



    case 0b00011000000000000000000000000000: {//Branch on Less Than or Equal to Zero
        //std::cout<<"Executing BRANCH on NOT EQUAL TO ZERO"<<std::endl<<std::endl;
      uint16_t  a = instruction;
      int16_t   b = a;
      int       c = b;
      c = c << 2;

      if(int(regist.read_reg(rs))<=0){
        prog_count.increment_branch(c);
      }
      break;
    }



    case 0b00010100000000000000000000000000: { //Branch on Not Equal
      //std::cout<<"Executing BRANCH on NOT EQUAL"<<std::endl<<std::endl;


      uint16_t  a = instruction;
      int16_t   b = a;
      int       c = b;
      c = c << 2;

      if(int(regist.read_reg(rs))!=int(regist.read_reg(rt))){
        prog_count.increment_branch(c);
      }
      break;

    }



    case 0b00101000000000000000000000000000: {//Set on Less Than Immediate
      //std::cout<<"Executing SET ON LESS THAN IMMEDIATE"<<std::endl<<std::endl;


      // Casting from one type to another: http://www.cplusplus.com/doc/tutorial/typecasting/
      int16_t tmp = (instruction & INSTR_IMMEDIATE); // signed 16 bit immediate
      int32_t im = tmp; // interpret as signed 32 bit - sign extension?   - YES, GOOD

      uint32_t tmp2 = regist.read_reg(rs); // unsigned 32 bit int
      int32_t source = tmp2; // interpret as signed 32 bit

      if(source < im) {     regist.write_reg(rt,1); }
      else {                regist.write_reg(rt,0); }

      break;
    }



    case 0b00101100000000000000000000000000: {//Set on Less Than Immediate Unsigned
      //std::cout<<"Executing SET ON LESS THAN"<<std::endl<<std::endl;

      int16_t tmp = (instruction & INSTR_IMMEDIATE); // signed 16 bit immediate
      int32_t tmp2 = tmp; // signed 32 bit - sign extension?
      uint32_t im = tmp2; // interpret as unsigned 32 bit

      uint32_t source = regist.read_reg(rs); // interpret as unsigned 32 bit int

      if(source < im) {     regist.write_reg(rt,1); }
      else {                regist.write_reg(rt,0); }

      break;
    }






    case 0b00001000000000000000000000000000: {//Jump
      //std::cout<<"Executing JUMP"<<std::endl<<std::endl;

      uint a = instruction & INSTR_ADDRESS;
      a = a<<2;
      uint b = prog_count.getInstr() + 4;
      b = b & 0xF0000000;
      a = a+b;
      prog_count.increment_jump(a);
      break;
    }



    case 0b00001100000000000000000000000000: {//Jump and link
      //std::cout<<"Executing JUMP & LINK"<<std::endl<<std::endl;

      regist.write_reg(31, prog_count.getInstr() + 8);

      uint a = instruction & INSTR_ADDRESS;
      a = a<<2;
      uint b = prog_count.getInstr() + 4;
      b = b & 0xF0000000;
      a = a+b;
      prog_count.increment_jump(a);
      break;
    }






    case 0b10000000000000000000000000000000: {//Load byte
      //std::cout << "Executing LOAD BYTE" << std::endl;
      int16_t offset = (instruction & INSTR_IMMEDIATE);  //Can be negative, need to test if negative ->
      uint address = regist.read_reg(rs) + int(offset);                        //-> sign extention works//
      uint data;
      uint8_t except;
      memory.read_val(address, readDataByteSigned, data, except);

      if(except==except_r_data){
        ret_status = ret_status_except_memom;
        break;
      }
      if(except==except_r_null){
        ret_status = ret_status_except_memom;
        break;
      }

      regist.write_reg(rt, data);
      ret_status = 0;
      break;
    }



    case 0b10010000000000000000000000000000: {//Load byte Unsigned
      //std::cout << "Executing LOAD BYTE UNSIGNED" << std::endl;
      int16_t offset = (instruction & INSTR_IMMEDIATE);  //Can be negative, need to test if negative ->
      uint address = regist.read_reg(rs) + int(offset);                        //-> sign extention works//
      uint data;
      uint8_t except;
      memory.read_val(address, readDataByteUnsigned, data, except);

      if(except==except_r_data){
        ret_status = ret_status_except_memom;
        break;
      }
      if(except==except_r_null){
        ret_status = ret_status_except_memom;
        break;
      }

      regist.write_reg(rt, data);
      ret_status = 0;
      break;

      break;
    }



    case 0b10000100000000000000000000000000: {//Load Halfword
      //std::cout << "Executing LOAD HALF WORD" << std::endl;
      int16_t offset = (instruction & INSTR_IMMEDIATE);  //Can be negative, need to test if negative ->
      uint address = regist.read_reg(rs) + offset;                        //-> sign extention works//
      if(address%2!=0){
        ret_status=ret_status_except_memom;
        break;
      }
      uint data;
      uint8_t except;
      memory.read_val(address, readDataHalfSigned, data, except);

      if(except==except_r_data){
        ret_status = ret_status_except_memom;
        break;
      }
      if(except==except_r_null){
        ret_status = ret_status_except_memom;
        break;
      }

      regist.write_reg(rt, data);
      ret_status = 0;
      break;

    }



    case 0b10010100000000000000000000000000: {//Load Halfword Unsigned
      //std::cout << "Executing LOAD HALF WORD UNSIGNED" << std::endl;
      int16_t offset = (instruction & INSTR_IMMEDIATE);  //Can be negative, need to test if negative ->
      uint address = regist.read_reg(rs) + offset;                        //-> sign extention works//
      if(address%2!=0){
        ret_status=ret_status_except_memom;
        break;
      }
      uint data;
      uint8_t except;
      memory.read_val(address, readDataHalfUnsigned, data, except);

      if(except==except_r_data){
        ret_status = ret_status_except_memom;
        break;
      }
      if(except==except_r_null){
        ret_status = ret_status_except_memom;
        break;
      }

      regist.write_reg(rt, data);
      ret_status = 0;
      break;
    }



    case 0b00111100000000000000000000000000: {//Load Upper Immediate
      //std::cout << "Executing LUP ";

      uint immediate = instruction & INSTR_IMMEDIATE;
      immediate = immediate << 16;

      regist.write_reg(rt, immediate);
      break;
    }



    case 0b10001100000000000000000000000000: {//LOAD WORD

      //std::cout << "Executing LW: Loading word ";
      int16_t offset = (instruction & INSTR_IMMEDIATE);  //Can be negative, need to test if negative ->
      uint address = regist.read_reg(rs) + offset;                        //-> sign extension works//
      ////std::cout << "from address " << address << std::endl;

      if(address%4!=0){
        ret_status=ret_status_except_memom;
        break;
      }
      uint data;
      uint8_t except;
      memory.read_val(address, readData, data, except);

      if(except==except_r_data){
        ret_status = ret_status_except_memom;
        break;
      }
      if(except==except_r_null){
        ret_status = ret_status_except_memom;
        break;
      }

      regist.write_reg(rt, data);
      ret_status = 0;
      break;


      //SHOULD WE IMPLEMENT:
          //TLB Refill, TLB Invalid, Bus Error?
    }



    case 0b10001000000000000000000000000000: {//Load Word Left
      ////std::cout << "Executing LOAD WORD LEFT" << std::endl;
      int16_t offset = (instruction & INSTR_IMMEDIATE);  //Can be negative, need to test if negative ->
      uint address = regist.read_reg(rs) + offset;                        //-> sign extention works//
      uint8_t except;
      uint data;


      if(address%4==0){
        memory.read_val(address, readWordLeft0, data, except);

        if(except==except_r_data){
          ret_status = ret_status_except_memom;
          break;
        }
        if(except==except_r_null){
          ret_status = ret_status_except_memom;
          break;
        }
        regist.write_reg(rt, data);
        ret_status = 0;
        break;
      }


      if(address%4==1){
        memory.read_val(address, readWordLeft1, data, except);

        if(except==except_r_data){
          ret_status = ret_status_except_memom;
          break;
        }
        if(except==except_r_null){
          ret_status = ret_status_except_memom;
          break;
        }

        uint a = (regist.read_reg(rt)) & 0x000000FF;
        data = a + data;
        regist.write_reg(rt, data);
        ret_status = 0;
        break;

      }


      if(address%4==2){
        memory.read_val(address, readWordLeft2, data, except);

        if(except==except_r_data){
          ret_status = ret_status_except_memom;
          break;
        }
        if(except==except_r_null){
          ret_status = ret_status_except_memom;
          break;
        }

        uint a = (regist.read_reg(rt)) & 0x0000FFFF;
        data = a + data;
        regist.write_reg(rt, data);
        ret_status = 0;
        break;
      }


      if(address%4==3){
        memory.read_val(address, readWordLeft3, data, except);

        if(except==except_r_data){
          ret_status = ret_status_except_memom;
          break;
        }
        if(except==except_r_null){
          ret_status = ret_status_except_memom;
          break;
        }

        uint a = (regist.read_reg(rt)) & 0x00FFFFFF;
        data = a + data;
        regist.write_reg(rt, data);
        ret_status = 0;
        break;
      }


      break;
    }



    case 0b10011000000000000000000000000000: {//Load Word Right
      ////std::cout << "Executing LOAD WORD RIGHT" << std::endl;
      int16_t offset = (instruction & INSTR_IMMEDIATE);  //Can be negative, need to test if negative ->
      uint address = regist.read_reg(rs) + offset;                        //-> sign extention works//
      uint8_t except;
      uint data;

      if(address%4 == 0){
        memory.read_val(address, readWordRight0, data, except);

        if(except==except_r_data){
          ret_status = ret_status_except_memom;
          break;
        }
        if(except==except_r_null){
          ret_status = ret_status_except_memom;
          break;
        }

        uint a = (regist.read_reg(rt)) & 0xFFFFFF00;
        data = a + data;

        regist.write_reg(rt, data);
        ret_status = 0;
        break;

      }


      if(address%4 == 1){
        memory.read_val(address, readWordRight1, data, except);

        if(except==except_r_data){
          ret_status = ret_status_except_memom;
          break;
        }
        if(except==except_r_null){
          ret_status = ret_status_except_memom;
          break;
        }

        uint a = (regist.read_reg(rt)) & 0xFFFF0000;
        data = a + data;

        regist.write_reg(rt, data);
        ret_status = 0;
        break;
      }


      if(address%4 == 2){
        memory.read_val(address, readWordRight2, data, except);

        if(except==except_r_data){
          ret_status = ret_status_except_memom;
          break;
        }
        if(except==except_r_null){
          ret_status = ret_status_except_memom;
          break;
        }

        uint a = (regist.read_reg(rt)) & 0xFF000000;
        data = a + data;

        regist.write_reg(rt, data);
        ret_status = 0;
        break;
      }


      if(address%4 == 3){

        memory.read_val(address, readWordRight3, data, except);

        if(except==except_r_data){
          ret_status = ret_status_except_memom;
          break;
        }
        if(except==except_r_null){
          ret_status = ret_status_except_memom;
          break;
        }

        regist.write_reg(rt, data);
        ret_status = 0;
        break;

      }

      break;
    }



    case 0b10100000000000000000000000000000: {//Store Byte
      //////std::cout << "Executing STORE BYTE" << std::endl;
      int16_t offset = (instruction & INSTR_IMMEDIATE);  //Can be negative, need to test if negative ->
      uint address = regist.read_reg(rs) + offset;                        //-> sign extention works//

      uint8_t except;
      memory.write_val(address, writeDataByte, regist.read_reg(rt), except);

      if(except==except_w_instr){
        ret_status = ret_status_except_memom;
        break;
      }
      else if(except==except_w_NULL){
        ret_status = ret_status_except_memom;
        break;
      }
      else if(except==except_w_OOB){
        ret_status = ret_status_except_memom;
        break;
      }

      ret_status = 0;
      break;
    }



    case 0b10100100000000000000000000000000: {//Store Halfword
      // ////std::cout << "Executing STORE HALFWORD" << std::endl;
      int16_t offset = (instruction & INSTR_IMMEDIATE);  //Can be negative, need to test if negative ->
      uint address = regist.read_reg(rs) + offset;                        //-> sign extention works//
      if(address%2!=0){
        ret_status=ret_status_except_memom;
        break;
      }

      uint8_t except;
      memory.write_val(address, writeDataHalf, regist.read_reg(rt), except);

      if(except==except_w_instr){
        ret_status = ret_status_except_memom;
        break;
      }
      else if(except==except_w_NULL){
        ret_status = ret_status_except_memom;
        break;
      }
      else if(except==except_w_OOB){
        ret_status = ret_status_except_memom;
        break;
      }

      ret_status = 0;
      break;
    }



    case 0b10101100000000000000000000000000: {//Store Word
      //////std::cout << "Executing SW" << std::endl;
      int16_t offset = (instruction & INSTR_IMMEDIATE);  //Can be negative, need to test if negative ->
      uint address = regist.read_reg(rs) + offset;                        //-> sign extention works//
      if(address%4!=0){
        ret_status=ret_status_except_memom;
        break;
      }

      uint8_t except;
      memory.write_val(address, writeData, regist.read_reg(rt), except);

      if(except==except_w_instr){
        ret_status = ret_status_except_memom;
        break;
      }
      else if(except==except_w_NULL){
        ret_status = ret_status_except_memom;
        break;
      }
      else if(except==except_w_OOB){
        ret_status = ret_status_except_memom;
        break;
      }

      ret_status = 0;
      break;
    }


    default: ret_status =  0b00000100;


  }


}
