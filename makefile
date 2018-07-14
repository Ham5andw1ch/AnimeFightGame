all: includes main

EXE = test
SUBDIRS = src

TOP_DIR := $(shell git rev-parse --show-toplevel)
include $(TOP_DIR)/makefile.inc

includes:
	@if [ ! -d $(INC_DIR) ]; then mkdir $(INC_DIR); fi
	cp `find src -name *.h` $(INC_DIR)

main: $(SUBDIRS)
	$(GCC) $(OBJ_DIR)/*.o -o $(WORKDIR)/$(EXE) $(LFLAGS) 

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(INC_DIR)/*.h
	rm -f $(WORKDIR)/$(EXE)
