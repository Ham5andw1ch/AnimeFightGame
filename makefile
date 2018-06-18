all: main

EXE = test
SUBDIRS = src

TOP_DIR := $(shell git rev-parse --show-toplevel)
include $(TOP_DIR)/makefile.inc

main: $(SUBDIRS)
	$(GCC) $(OBJ_DIR)/*.o -o $(WORKDIR)/$(EXE) $(LFLAGS) 

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(WORKDIR)/$(EXE)