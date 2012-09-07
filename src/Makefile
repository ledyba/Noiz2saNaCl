# Noiz2sa makefile(MinGW 2.0.0)
# $Id: Makefile,v 1.4 2003/08/10 09:00:05 kenta Exp $

BULLETML=

NACL_TOOLCHAIN_ROOT=$(NACL_SDK_ROOT)/toolchain/linux_x86_newlib
NACL_TOOLCHAIN_ROOT32=$(NACL_TOOLCHAIN_ROOT)/i686-nacl/usr
NACL_TOOLCHAIN_ROOT64=$(NACL_TOOLCHAIN_ROOT)/x86_64-nacl/usr
NACL_GCC_PREFIX=$(NACL_TOOLCHAIN_ROOT)/bin/x86_64-nacl-

NAME   = noiz2sa
RM     = rm -f
CC     = $(NACL_GCC_PREFIX)gcc -c
LINKER     = $(NACL_GCC_PREFIX)g++
CXX    = $(NACL_GCC_PREFIX)g++ -c

O32      = o32
O64      = o64
NEXE_64        = ../$(NAME)_64.nexe
NEXE_32        = ../$(NAME)_32.nexe

all: $(NEXE_64) $(NEXE_32)

NEXE32_CFLAGS = `$(NACL_TOOLCHAIN_ROOT32)/bin/sdl-config --cflags` -m32
NEXE64_CFLAGS = `$(NACL_TOOLCHAIN_ROOT64)/bin/sdl-config --cflags` -m64
NEXE32_LDFLAGS = `$(NACL_TOOLCHAIN_ROOT32)/bin/sdl-config --libs`
NEXE64_LDFLAGS = `$(NACL_TOOLCHAIN_ROOT64)/bin/sdl-config --libs`
NEXE32_LIBS=-lbulletml32
NEXE64_LIBS=-lbulletml64

MORE_CFLAGS = -O3

CFLAGS  = $(MORE_CFLAGS)
CPPFLAGS  = $(MORE_CFLAGS) -I. -I$(BULLETML) -I$(BULLETML)/src

LDFLAGS        =  -L. -L$(BULLETML)/src
LIBS=-lSDL_mixer -lSDL -lnosys -lvorbis -lvorbisfile -lvorbisenc -lvorbis -logg -lstdc++ -lppapi -lppapi_cpp -lm

OBJS_32 =	$(NAME).$(O32) ship.$(O32) shot.$(O32) frag.$(O32) bonus.$(O32) \
	foe.$(O32) foecommand.$(O32) barragemanager.$(O32) attractmanager.$(O32) \
	background.$(O32) letterrender.$(O32) \
	screen.$(O32) clrtbl.$(O32) vector.$(O32) degutil.$(O32) rand.$(O32) soundmanager.$(O32) \
	nativeclient.$(O32)

OBJS_64 =	$(NAME).$(O64) ship.$(O64) shot.$(O64) frag.$(O64) bonus.$(O64) \
	foe.$(O64) foecommand.$(O64) barragemanager.$(O64) attractmanager.$(O64) \
	background.$(O64) letterrender.$(O64) \
	screen.$(O64) clrtbl.$(O64) vector.$(O64) degutil.$(O64) rand.$(O64) soundmanager.$(O64) \
	nativeclient.$(O64)

$(NEXE_32): $(OBJS_32) 
	$(LINKER) $(NEXE32_CFLAGS) $(CFLAGS) -o $(NEXE_32) $(OBJS_32) $(NEXE32_LDFLAGS) $(LDFLAGS) $(NEXE32_LIBS) $(LIBS) 
$(NEXE_64): $(OBJS_64) 
	$(LINKER) $(NEXE64_CFLAGS) $(CFLAGS) -o $(NEXE_64) $(OBJS_64) $(NEXE64_LDFLAGS) $(LDFLAGS) $(NEXE64_LIBS) $(LIBS)

clean:
	$(RM) $(NEXE_32) $(NEXE_64) *.$(O32) *.$(O64)


.SUFFIXES:	.c .cc .$(O32) .$(O64)
.c.$(O32):
	$(CC) $(NEXE32_CFLAGS) $(CFLAGS) $(CPPFLAGS) $< -o $@
.cc.$(O32):
	$(CXX) $(NEXE32_CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $< -o $@

.c.$(O64):
	$(CC) $(NEXE64_CFLAGS) $(CFLAGS) $(CPPFLAGS) $< -o $@
.cc.$(O64):
	$(CXX) $(NEXE64_CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $< -o $@

