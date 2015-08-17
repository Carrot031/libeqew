CC=g++
CFLAGS=-Wall -std=c++11 -g -fPIC -O0
OUTPUT=libeqew.a
BUILDDIR = build/$(ARCH)
OBJS=EQEW.o  $(addsuffix .o,$(basename $(wildcard liboauthcpp/*.cpp))) $(addsuffix .o,$(basename $(wildcard json11/*.cpp)))
ARCH=$(shell arch)
#LIBS=liboauthcpp/*.o
all: $(OUTPUT)
$(OUTPUT) : $(OBJS) $(LIBS)
	mkdir -p $(BUILDDIR)
	ar rcs $(BUILDDIR)/$(OUTPUT) $(OBJS)
#CCellDefinition.o:CCellDefinition.cpp CCellDefinition.hpp
#	$(CC) $(CFLAGS) -c $< -I/home/carrot031/libtarpp
#CCellDefinition.o:CCell.cpp CCell.hpp
#	$(CC) $(CFLAGS) -c $< -I/home/carrot031/libtarpp
%.o:%.cpp %.hpp
	$(CC) $(CFLAGS) -c $< $(INCLUDE) -o $@
%.a:
	make -C $(shell dirname $@)
#Size.o:Size.cpp Size.hpp
#	$(CC) -std=c++11 -c Size.cpp
#Point.o:Point.cpp Point.hpp
#	$(CC) -std=c++11 -c Point.cpp

clean:cleanobjs
	rm -rf $(OUTPUT) $(BUILDDIR)
cleanobjs:
	rm -rf $(OBJS)

