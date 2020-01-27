// Array of 32 registers, each 32 bits
#ifndef REG_FILE_HPP
#define REG_FILE_HPP

typedef unsigned int uint;

class reg_file {
public:
    void write_reg(const uint& reg, const uint& data);
    uint read_reg(const uint& reg);

    void write_HI(const uint& data);
    uint read_HI();
    void write_LO(const uint& data);
    uint read_LO();

    void initialise();

    //Testing functions ()
    void print();
    
private:
    uint registers [32];
    uint hi;
    uint lo;
    bool allow_write_HI;
    bool allow_write_LO;
};

#endif
