GCC = gcc
WORKDIR = $(TOP_DIR)
OBJ_DIR = $(WORKDIR)/build
INC_DIR = $(WORKDIR)/include

CFLAGS = -g -I$(INC_DIR) 
#-DDEBUG_
LFLAGS = -lm

ifeq ($(OS),Windows_NT)
	CFLAGS += -DWIN32 -std=c99 -I$(SDL_DIR)/include
	LFLAGS += -L$(SDL_DIR)/lib -lSDL2 -lSDL2main
	UNIX = 0
else ifeq ($(shell uname -s), Darwin)
	CFLAGS += -DMACOSX
	LFLAGS += -framework SDL2
	UNIX = 1
else
	CFLAGS += $(shell sdl2-config --cflags) -DLINUX
	LFLAGS += $(shell sdl2-config --libs)
	UNIX = 1
endif

ifeq ($(UNIX),1)
%.o: %.c
	@if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	$(GCC) $(CFLAGS) -c $^ -o $(OBJ_DIR)/$@
else
%.o: %.c
	echo $^
	if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	$(GCC) $(CFLAGS) -c $^ -o $(OBJ_DIR)/$@
endif

$(SUBDIRS):
	$(MAKE) -C $@ all

.PHONY: $(SUBDIRS) all includes

all: $(OBS) $(SUBDIRS)
