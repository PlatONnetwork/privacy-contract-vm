CC = gcc
CXX = g++
AR = ar
LD = g++
MAKE = make
CP = cp -fp
RM = rm -f
MD = mkdir -p
MV = mv -f
RANLIB = ranlib
ARFLAGS = rc

debug: CFLAGS = -c -g -Wall -DDEBUG -m64 -fPIC
debug: CXXFLAGS = -c -g -Wall -DDEBUG -std=c++11  -m64 -fPIC
debug: LDFLAGS =

release: CFLAGS = -c -O2 -g -Wall -DNDEBUG  -m64 -fPIC
release: CXXFLAGS = -c -O2 -g -Wall -DNDEBUG -std=c++11  -m64 -fPIC
release: LDFLAGS =

.c.o:
	@echo -n "gcc Compiling $<... "
	@$(CC) $(CFLAGS) $(INCS) $(DEFS) -o $@ $<
	@echo "Done."

.cc.o:
	@echo -n "Compiling $<... "
	@$(CXX) $(CXXFLAGS) $(INCS) $(DEFS) -o $@ $<
	@echo "Done."

.cpp.o:
	@echo -n "Compiling $<... "
	@$(CXX) $(CXXFLAGS) $(INCS) $(DEFS) -o $@ $<
	@echo "Done."

.cxx.o:
	@echo -n "Compiling $<... "
	@$(CXX) $(CXXFLAGS) $(INCS) $(DEFS) -o $@ $<
	@echo "Done."
