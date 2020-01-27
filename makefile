simulator: mem_map.o PC.o reg_file.o decode.o simulator.o
	mkdir -p bin
	g++ src/mem_map.o src/PC.o src/reg_file.o src/decode.o src/simulator.o -std=c++11 -o bin/mips_simulator

mem_map.o: src/mem_map.cpp src/mem_map.hpp
	g++ -c -std=c++11 src/mem_map.cpp -o src/mem_map.o

PC.o: src/PC.cpp src/PC.hpp
	g++ -c -std=c++11 src/PC.cpp -o src/PC.o

reg_file.o: src/reg_file.cpp src/reg_file.hpp
	g++ -c -std=c++11 src/reg_file.cpp -o src/reg_file.o

decode.o: src/decode.cpp src/decode.h
	g++ -c -std=c++11 src/decode.cpp -o src/decode.o

simulator.o: src/simulator.cpp
	g++ -c -std=c++11 src/simulator.cpp -o src/simulator.o

run:
	./bin/mips_simulator

clean:
	rm src/*.o
	rm bin/mips_simulator

testbench:
