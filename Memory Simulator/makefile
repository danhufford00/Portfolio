
CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11

scc: scc.o inpost.o mystack.o
	$(CXX) $(CXXFLAGS) -o scc $^

simplesim: simplesim_main.o simplesim.o
	$(CXX) $(CXXFLAGS) -o simplesim $^


scc.o: scc.cpp sml.h inpost.h
inpost.o: inpost.cpp inpost.h mystack.h
mystack.o: mystack.cpp mystack.h
simplesim_main.o: simplesim_main.cpp simplesim.h
simplesim.o: simplesim.cpp simplesim.h sml.h

clean:
	rm -f scc simplesim *.o
