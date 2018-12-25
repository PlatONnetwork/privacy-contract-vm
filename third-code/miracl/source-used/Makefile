include Makefile.inc

OUTPUT = ../output




INCS = -I./ -I../include -I/usr/include/x86_64-linux-gnu

DEFS = -DMR_UNIX_MT

LIBS = -D/usr/local/lib64 -D/usr/lib64 -D/usr/local/lib

# for miracl.a
BIN = $(OUTPUT)/libmiracl.a

SRC = mrcore.c mrarth0.c mrarth1.c mrarth2.c mralloc.c mrsmall.c mrio1.c mrio2.c mrgcd.c mrjack.c \
mrxgcd.c mrarth3.c mrbits.c mrrand.c mrprime.c mrcrt.c mrscrt.c mrmonty.c mrpower.c mrsroot.c mrcurve.c \
mrfast.c mrshs.c mrshs256.c mrshs512.c mrsha3.c mrfpe.c mraes.c mrgcm.c mrlucas.c mrzzn2.c mrzzn2b.c mrzzn3.c \
mrzzn4.c mrecn2.c mrstrong.c mrbrick.c mrebrick.c mrec2m.c mrgf2m.c mrflash.c mrfrnd.c mrdouble.c mrround.c \
mrbuild.c mrflsh1.c mrpi.c mrflsh2.c mrflsh3.c mrflsh4.c mrmuldv.c

OBJS = $(patsubst %.c,%.o,$(SRC))

$(info "*********")
$(info SRC is $(SRC))
$(info OBJS is $(OBJS))


#gcc -m32 -O2 bmark.c miracl.a -o bmark
BMARK = $(OUTPUT)/bmark
SRC_BMARK = bmark.c
OBJ_BMARK = $(patsubst %.c,%.o,$(SRC_BMARK))

#gcc -m32 -O2 fact.c miracl.a -o fact
FACT = $(OUTPUT)/fact
SRC_FACT = fact.c
OBJ_FACT = $(patsubst %.c,%.o,$(SRC_FACT))

#g++ -m32 -O2 mersenne.cpp big.o miracl.a -o mersenne
MERSENNE = $(OUTPUT)/mersenne
SRC_MERSENNE = mersenne.cpp
OBJ_MERSENNE = $(patsubst %.cpp,%.o,$(SRC_MERSENNE))

#g++ -m32 -O2 brent.cpp big.o zzn.o miracl.a -o brent
BRENT = $(OUTPUT)/brent
SRC_BRENT = brent.cpp big.cpp zzn.cpp
OBJ_BRENT = $(patsubst %.cpp,%.o,$(SRC_BRENT))

#g++ -m32 -O2 sample.cpp flash.o miracl.a -o sample
SAMPLE = $(OUTPUT)/sample
SRC_SAMPLE = sample.cpp flash.cpp
OBJ_SAMPLE = $(patsubst %.cpp,%.o,$(SRC_SAMPLE))

#g++ -m32 -O2 ecsgen.cpp ecn.o big.o miracl.a -o ecsgen
ECSGEN = $(OUTPUT)/ecsgen
SRC_ECSGEN = ecsgen.cpp ecn.cpp big.cpp
OBJ_ECSGEN = $(patsubst %.cpp,%.o,$(SRC_ECSGEN))

#g++ -m32 -O2 ecsign.cpp ecn.o big.o miracl.a -o ecsign
ECSIGN = $(OUTPUT)/ecsign
SRC_ECSIGN = ecsign.cpp ecn.cpp big.cpp
OBJ_ECSIGN = $(patsubst %.cpp,%.o,$(SRC_ECSIGN))

#g++ -m32 -O2 ecsver.cpp ecn.o big.o miracl.a -o ecsver
ECSVER = $(OUTPUT)/ecsver
SRC_ECSVER = ecsver.cpp ecn.cpp big.cpp
OBJ_ECSVER = $(patsubst %.cpp,%.o,$(SRC_ECSVER))

#g++ -m32 -O2 pk-demo.cpp ecn.o big.o miracl.a -o pk-demo
PK_DEMO = $(OUTPUT)/pk-demo
SRC_PK_DEMO = pk-demo.cpp ecn.cpp big.cpp
OBJ_ECSVER = $(patsubst %.cpp,%.o,$(SRC_PK_DEMO))

#g++ -m32 -O2 schoof.cpp polymod.o poly.o ecn.o crt.o zzn.o big.o miracl.a -o schoof
SCHOOF = $(OUTPUT)/schoof
SRC_SCHOOF = curve/schoof.cpp curve/polymod.cpp curve/poly.cpp ecn.cpp crt.cpp zzn.cpp big.cpp
OBJ_SCHOOF = $(patsubst %.cpp,%.o,$(SRC_SCHOOF))



all: release 
debug: $(BIN) $(BMARK) $(FACT) $(MERSEN) $(BRENT) $(SAMPLE) $(ECSGEN) $(ECSIGN) $(ECSVER) $(SCHOOF)
release: $(BIN) $(BMARK) $(FACT) $(MERSEN) $(BRENT) $(SAMPLE) $(ECSGEN) $(ECSIGN) $(ECSVER) $(SCHOOF)

$(BIN): $(OBJS)
	@echo -n "create $@ static library ... "
	@$(AR) $(ARFLAGS) $@ $(OBJS)
	@echo "Done."

$(BMARK): $(OBJ_BMARK)
	@echo -n "Linking $@... "
	@$(CC) -o $@ $(LDFLAGS) $(OBJ_BMARK) $(BIN)
	@echo "Done."

$(FACT): $(OBJ_FACT)
	@echo -n "Linking $@... "
	@$(CC) -o $@ $(LDFLAGS) $(OBJ_FACT) $(BIN)
	@echo "Done."

$(MERSEN): $(OBJ_MERSEN)
	@echo -n "Linking $@... "
	@$(LD) -o $@ $(LDFLAGS) $(OBJ_MERSEN) $(BIN)
	@echo "Done."

$(BRENT): $(OBJ_BRENT)
	@echo -n "Linking $@... "
	@$(LD) -o $@ $(LDFLAGS) $(OBJ_BRENT) $(BIN)
	@echo "Done."

$(SAMPLE): $(OBJ_SAMPLE)
	@echo -n "Linking $@... "
	@$(LD) -o $@ $(LDFLAGS) $(OBJ_SAMPLE) $(BIN)
	@echo "Done."

$(ECSGEN): $(OBJ_ECSGEN)
	@echo -n "Linking $@... "
	@$(LD) -o $@ $(LDFLAGS) $(OBJ_ECSGEN) $(BIN)
	@echo "Done."

$(ECSIGN): $(OBJ_ECSIGN)
	@echo -n "Linking $@... "
	@$(LD) -o $@ $(LDFLAGS) $(OBJ_ECSIGN) $(BIN)
	@echo "Done."

$(ECSVER): $(OBJ_ECSVER)
	@echo -n "Linking $@... "
	@$(LD) -o $@ $(LDFLAGS) $(OBJ_ECSVER) $(BIN)
	@echo "Done."

$(SCHOOF): $(OBJ_SCHOOF)
	@echo -n "Linking $@... "
	@$(LD) -o $@ $(LDFLAGS) $(OBJ_SCHOOF) $(BIN)
	@echo "Done."

.PHONY: clean

clean:
	@echo -n "Cleaning... "
	@$(RM) $(BIN) $(BMARK) $(FACT) $(MERSEN) $(BRENT) $(SAMPLE) $(ECSGEN) $(ECSIGN) $(ECSVER) $(SCHOOF)
	@$(RM) $(OBJS) $(OBJ_BMARK) $(OBJ_FACT) $(OBJ_MERSEN) $(OBJ_BRENT) $(OBJ_SAMPLE) $(OBJ_ECSGEN) $(OBJ_ECSIGN) $(OBJ_ECSVER) $(OBJ_SCHOOF)
	@echo "Done."
