CC=g++
CFLAGS=-Wall -std=c++11 -g -fPIC -O0
OUTPUT=libeqew.a
OBJS=EQEW.o $(addsuffix .o,$(basename $(wildcard liboauthcpp/*.cpp)))
#LIBS=liboauthcpp/*.o
all: $(OBJS) $(LIBS)
	ar rcs $(OUTPUT) $(OBJS)
#CCellDefinition.o:CCellDefinition.cpp CCellDefinition.hpp
#	$(CC) $(CFLAGS) -c $< -I/home/carrot031/libtarpp
#CCellDefinition.o:CCell.cpp CCell.hpp
#	$(CC) $(CFLAGS) -c $< -I/home/carrot031/libtarpp
%.o:%.cpp %.hpp
	$(CC) $(CFLAGS) -c $< $(INCLUDE)
%.a:
	make -C $(shell dirname $@)
#Size.o:Size.cpp Size.hpp
#	$(CC) -std=c++11 -c Size.cpp
#Point.o:Point.cpp Point.hpp
#	$(CC) -std=c++11 -c Point.cpp

clean:
	rm -f $(OBJS) $(OUTPUT)

