GCC = gcc
WORKDIR = $(TOP_DIR)
OBJ_DIR = $(WORKDIR)/build
INC_DIR = $(WORKDIR)/include

CFLAGS = -g $(shell sdl2-config --cflags) -I$(INC_DIR)
LFLAGS = $(shell sdl2-config --libs) -lm

%.o: %.c
	@if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	$(GCC) $(CFLAGS) -c $^ -o $(OBJ_DIR)/$@

$(SUBDIRS):
	$(MAKE) -C $@ all

.PHONY: $(SUBDIRS) all includes

all: includes $(OBS) $(SUBDIRS)

includes:
	@if [ ! -d $(INC_DIR) ]; then mkdir $(INC_DIR); fi
	@if [ -e *.h ]; then cp -f *.h $(INC_DIR); fi
