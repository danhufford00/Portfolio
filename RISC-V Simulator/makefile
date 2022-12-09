CXX = g++
CXXFLAGS = -g -ansi -pedantic -Wall -Werror -std=c++14

rv32i: main.o hex.o memory.o rv32i_decode.o rv32i_hart.o registerfile.o cpu_single_hart.o
	$(CXX) $(CXXFLAGS) -o rv32i $^

main.o: main.cpp hex.h memory.h rv32i_decode.h rv32i_hart.h registerfile.h cpu_single_hart.h

hex.o: hex.cpp hex.h

memory.o: memory.cpp memory.h

rv32i.decode.o: rv32i_decode.cpp rv32i_decode.h

rv32i.hart.o: rv32i_hart.cpp rv32i_hart.h

registerfile.o: registerfile.cpp registerfile.h

cpu.single.hart.o: cpu_single_hart.cpp cpu_single_hart.h

clean:
	rm -f rv32i *.o *.run
