# include "PC.hpp"

PC::PC(){
  current_instr =   0x10000000;
  next_instr =      0x10000004;
  in_delayed_branch = false;
  disable_outer_increment = false;

}

void PC::increment(){
  if(!disable_outer_increment){
    current_instr = next_instr;
    next_instr +=4;

    in_delayed_branch = false;
  }
  disable_outer_increment = false;

}

void PC::increment_jump(uint instruction){
  if(in_delayed_branch){
    current_instr +=  4;
    next_instr =      instruction;
  }

  current_instr =   next_instr;
  next_instr =      instruction;

  in_delayed_branch = true;
  disable_outer_increment = true;

}

void PC::increment_branch(int offset){
  if(in_delayed_branch){
    current_instr +=  4;
    next_instr +=offset;
  }

  current_instr =   next_instr;
  next_instr +=offset;


  in_delayed_branch = true;
  disable_outer_increment = true;

}

int PC::getInstr(){
  return current_instr;
}
