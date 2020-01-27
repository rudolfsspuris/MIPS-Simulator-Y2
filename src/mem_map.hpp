#include <cstdint>
#include <vector>
#include <stdint.h>

#ifndef MEM_MAP_HPP
#define MEM_MAP_HPP


typedef unsigned int uint;

//Constant definitions in case needed
const uint ADDR_NULL      = 0x00000000;    //Jumping to this address means the Binary has finished execution.
const uint ADDR_NULL_L    = 0x00000004;

const uint ADDR_INSTR     = 0x10000000;    //Executable memory. The Binary should be loaded here.
const uint ADDR_INSTR_L   = 0x01000000;
const uint ADDR_INSTR_OOB = 0x11000000;

const uint ADDR_DATA      = 0x20000000;    //Read-write data area. Should be zero-initialised.
const uint ADDR_DATA_L    = 0x04000000;
const uint ADDR_DATA_OOB  = 0x24000000;

const uint ADDR_GETC      = 0x30000000;    //Location of memory mapped input. Read-only.
const uint ADDR_GETC_L    = 0x00000004;

const uint ADDR_PUTC      = 0x30000004;
const uint ADDR_PUTC_L    = 0x00000008;    //Location of memory mapped output. Write-only.

//Exception codes for possible debugging
const uint8_t except_r_null   = 0b00000001;   //Reading address 0
const uint8_t except_r_intsr  = 0b00000010;   //Attempt to read instructions, address is OOB
const uint8_t except_r_data   = 0b00000100;   //Attempt to read data, address is OOB

const uint8_t except_w_instr  = 0b00001000;   //Attempt to write data to Instr
const uint8_t except_w_NULL   = 0b00010000;   //Attempt to write data to NULL (might not cause exception, check SPEC)
const uint8_t except_w_OOB    = 0b00100000;   //Attempt to write data OOB anywhere except Instr or 0

const uint8_t except_unknown  = 0b10000000;   //Something weeeirds going on?
const uint8_t except_false_f  = 0b11111111;   //Function code specified is unknown, return 0,

//Function codes
const uint8_t readInstr             = 0b00000001;   //Read instruction memory, return 32bit value
const uint8_t readData              = 0b00000010;   //Read data memory, return value

const uint8_t readDataByteSigned    = 0b00001000;   //Read data memory, return value
const uint8_t readDataByteUnsigned  = 0b00010000;   //Read data memory, return value

const uint8_t readDataHalfSigned    = 0b00100000;   //Read data memory, return value
const uint8_t readDataHalfUnsigned  = 0b01000000;   //Read data memory, return value

const uint8_t writeData             = 0b00000001;   //Write data to memory address specified
const uint8_t writeDataByte         = 0b00000010;
const uint8_t writeDataHalf         = 0b00000100;

const uint8_t readWordLeft0        = 0b11111110;
const uint8_t readWordLeft1        = 0b11111101;
const uint8_t readWordLeft2        = 0b11111011;
const uint8_t readWordLeft3        = 0b11110111;

const uint8_t readWordRight0        = 0b01111111;
const uint8_t readWordRight1        = 0b10111111;
const uint8_t readWordRight2        = 0b11011111;
const uint8_t readWordRight3        = 0b11101111;




class mem_map{

public:
  //Access Functions
  void read_val   (const uint &address, uint8_t funct, uint &value /*output*/, uint8_t &except) const;
  void write_val  (const uint &address, uint8_t funct, const uint value, uint8_t &except);

  //Initialisation Functions
  mem_map (const std::vector<char> &instructions);
  ~mem_map();
  void manual_deallocate();


private:

  uint8_t* instr;
  uint8_t* data;

  //Support functions
  uint generate_data(const uint &address, uint8_t funct) const;
  uint readGETC() const;
  void writePUTC(uint out);

};


#endif
