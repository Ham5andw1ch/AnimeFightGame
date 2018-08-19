GCC = gcc
WORKDIR = $(TOP_DIR)
OBJ_DIR = $(WORKDIR)/build
INC_DIR = $(WORKDIR)/include

ifeq ($(shell uname -s), Darwin)
CFLAGS = -g -framework SDL2 -I$(INC_DIR) -DDEBUG_
LFLAGS = -framework SDL2 -lm
else
CFLAGS = -g $(shell sdl2-config --cflags) -I$(INC_DIR) -DDEBUG_
LFLAGS = $(shell sdl2-config --libs) -lm
endif

%.o: %.c
	@if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	$(GCC) $(CFLAGS) -c $^ -o $(OBJ_DIR)/$@

$(SUBDIRS):
	$(MAKE) -C $@ all

.PHONY: $(SUBDIRS) all includes

all: $(OBS) $(SUBDIRS)
