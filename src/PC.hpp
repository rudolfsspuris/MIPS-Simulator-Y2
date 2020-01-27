#ifndef PC_HPP
#define PC_HPP

typedef unsigned int uint;

class PC{
public:
  PC();
  void increment();
  void increment_jump (uint instruction);
  void increment_branch(int offset);
  int getInstr();


private:
  int current_instr;
  int next_instr;

  bool in_delayed_branch;
  bool disable_outer_increment;
};




#endif
