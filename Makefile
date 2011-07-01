OBJS = User.o FrontEnd.o BackEnd.o QueryResult.o main.o
CXX = g++
DEBUG = -g
OPTI = -O0
CXXFLAGS = -Wall -Wextra -c $(DEBUG) $(OPTI)
LFLAGS = -lboost_program_options -lboost_filesystem -lmysqlcppconn $(DEBUG) $(OPTI)

adworks: $(OBJS) 
	$(CXX) $(LFLAGS) $(OBJS) -o adworks

.PHONY: clean
clean:
	rm -f *.o adworks

BackEnd.o: BackEnd.cpp BackEnd.hpp QueryResult.hpp User.hpp

FrontEnd.o: FrontEnd.cpp FrontEnd.hpp QueryResult.hpp User.hpp BackEnd.hpp

User.o: User.cpp User.hpp

QueryResult.o: QueryResult.cpp QueryResult.hpp

main.o: main.cpp
