OBJS = User.o FrontEnd.o BackEnd.o QueryResult.o main.o
CXX = g++
DEBUG = -g
OPTI = -O0
CXXFLAGS = -Wall -Wextra -c $(DEBUG) $(OPTI)
LFLAGS = -lboost_program_options $(DEBUG) $(OPTI)

main: $(OBJS) 
	$(CXX) $(LFLAGS) $(OBJS) -o main

.PHONY: clean
clean:
	rm -f *.o main

BackEnd.o: BackEnd.cpp BackEnd.hpp QueryResult.hpp User.hpp

FrontEnd.o: FrontEnd.cpp FrontEnd.hpp QueryResult.hpp User.hpp BackEnd.hpp

User.o: User.cpp User.hpp

QueryResult.o: QueryResult.cpp QueryResult.hpp

main.o: main.cpp
