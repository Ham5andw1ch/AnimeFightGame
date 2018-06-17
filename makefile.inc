GCC = gcc
WORKDIR = $(TOP_DIR)
OBJ_DIR = $(WORKDIR)/build

CFLAGS = -g $(shell sdl2-config --cflags)
LFLAGS = $(shell sdl2-config --libs) -lm

%.o: %.c
	@if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	$(GCC) $(CFLAGS) -c $^ -o $(OBJ_DIR)/$@

$(SUBDIRS):
	$(MAKE) -C $@ all

.PHONY: $(SUBDIRS) all

all: $(OBS) $(SUBDIRS)
