#include<cstdint>
#include<iostream>

typedef unsigned int uint; //-correct?
typedef char byte;

int main(){
  // uint8_t a = 0b11111111;
  // uint8_t b = 0b00000001;
  // std::cout<<a<<b<<std::endl;
  //
  // uint i0 = a;
  // uint i1 = b;
  // std::cout<<i0<<i1<<std::endl;

  // std::cin>>a;
  // i0 = a;
  // std::cout<<i0<<std::endl;
  // return 0;

  uint k = 0xFF0FF000;

  uint8_t f = k;       /* 0xef */
  uint8_t g =k >> 8;  /* 0xbe */
  uint8_t h = k >> 16; /* 0xad */
  uint8_t j = k >> 24; /* 0xde */

  uint i0 = f;
  uint i1 = g;
  uint i2 = h;
  uint i3 = j;

  std::cout<<i0<<std::endl<<i1<<std::endl<<i2<<std::endl<<i3<<std::endl<<std::endl;

}
