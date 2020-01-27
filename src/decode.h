#include "mem_map.hpp"
#include "reg_file.hpp"
#include "PC.hpp"

void decode(uint instruction, mem_map & memory, PC & prog_count, reg_file & regist, uint8_t &ret_status);
