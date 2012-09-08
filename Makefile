# Noiz2sa makefile(MinGW 2.0.0)
# $Id: Makefile,v 1.4 2003/08/10 09:00:05 kenta Exp $

BULLETML=./bulletml

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
NEXE_64        = $(NAME)_64.nexe
NEXE_32        = $(NAME)_32.nexe

subdirs=./bulletml/src

all:DELBIN $(subdirs) $(NEXE_64) $(NEXE_32)

$(subdirs): FORCE
	make -C $@ $(MAKECMDGOALS)
FORCE:
	
DELBIN:
	rm -f *.nexe

NEXE32_CFLAGS = `$(NACL_TOOLCHAIN_ROOT32)/bin/sdl-config --cflags` -m32
NEXE64_CFLAGS = `$(NACL_TOOLCHAIN_ROOT64)/bin/sdl-config --cflags` -m64
NEXE32_LDFLAGS = `$(NACL_TOOLCHAIN_ROOT32)/bin/sdl-config --libs`
NEXE64_LDFLAGS = `$(NACL_TOOLCHAIN_ROOT64)/bin/sdl-config --libs`
NEXE32_LIBS=-lbulletml32
NEXE64_LIBS=-lbulletml64

MORE_CFLAGS = -O3 -std=gnu99 -g

CFLAGS  = $(MORE_CFLAGS)
CPPFLAGS  = $(MORE_CFLAGS) -I./src -I$(BULLETML) -I$(BULLETML)/src

LDFLAGS        =  -L. -L$(BULLETML)/src -g
LIBS=-lSDL_mixer -lSDL -lnosys -lvorbis -lvorbisfile -lvorbisenc -lvorbis -logg -lstdc++ -lppapi -lppapi_cpp -lm

storage/file_data.c: packFile.py
	python $< > $@

OBJS_32 =	src/$(NAME).$(O32) src/ship.$(O32) src/shot.$(O32) src/frag.$(O32) src/bonus.$(O32) \
	src/foe.$(O32) src/foecommand.$(O32) src/barragemanager.$(O32) src/attractmanager.$(O32) \
	src/background.$(O32) src/letterrender.$(O32) \
	src/screen.$(O32) src/clrtbl.$(O32) src/vector.$(O32) src/degutil.$(O32) src/rand.$(O32) src/soundmanager.$(O32) \
	nacl/entrypoint.$(O32)\
	nacl/storage.$(O32) \
	nacl/file_data.$(O32)

OBJS_64 =	src/$(NAME).$(O64) src/ship.$(O64) src/shot.$(O64) src/frag.$(O64) src/bonus.$(O64) \
	src/foe.$(O64) src/foecommand.$(O64) src/barragemanager.$(O64) src/attractmanager.$(O64) \
	src/background.$(O64) src/letterrender.$(O64) \
	src/screen.$(O64) src/clrtbl.$(O64) src/vector.$(O64) src/degutil.$(O64) src/rand.$(O64) src/soundmanager.$(O64) \
	nacl/entrypoint.$(O64)\
	nacl/storage.$(O64) \
	nacl/file_data.$(O64)

$(NEXE_32): $(OBJS_32) 
	$(LINKER) $(NEXE32_CFLAGS) $(CFLAGS) -o $(NEXE_32) $(OBJS_32) $(NEXE32_LDFLAGS) $(LDFLAGS) $(NEXE32_LIBS) $(LIBS) 
$(NEXE_64): $(OBJS_64) 
	$(LINKER) $(NEXE64_CFLAGS) $(CFLAGS) -o $(NEXE_64) $(OBJS_64) $(NEXE64_LDFLAGS) $(LDFLAGS) $(NEXE64_LIBS) $(LIBS)

clean:
	make -C bulletml/src/ clean
	$(RM) $(NEXE_32) $(NEXE_64) src/*.$(O32) src/*.$(O64)


.SUFFIXES:	.c .cc .$(O32) .$(O64)
.c.$(O32):
	$(CC) $(NEXE32_CFLAGS) $(CFLAGS) $(CPPFLAGS) $< -o $@
.cc.$(O32):
	$(CXX) $(NEXE32_CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $< -o $@

.c.$(O64):
	$(CC) $(NEXE64_CFLAGS) $(CFLAGS) $(CPPFLAGS) $< -o $@
.cc.$(O64):
	$(CXX) $(NEXE64_CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $< -o $@

