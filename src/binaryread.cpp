#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdint>

int main(){
    std::ifstream binaryfile;
    binaryfile.open("binary.bin");
    std::vector<char> instructions;

    if (binaryfile) {
        // get length of file
        binaryfile.seekg (0, binaryfile.end);
        int length = binaryfile.tellg();
        binaryfile.seekg (0, binaryfile.beg);

        char* instr_array;
        instr_array = new char [length];
        // read the file 8 bits (1 character) at a time
        binaryfile.read(instr_array, length);

        // copy instructions array into a vector
        for(int i=0; i < length; i++){
            instructions.push_back(instr_array[i]);
        }
        delete[] instr_array;
    }
    // console output
    for(int i=0; i < instructions.size(); i++){
        std::cout << "char "<< i << ": " << instructions[i] << std::endl;
    }

    binaryfile.close();
    return 0;
}
