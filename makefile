all: includes main

EXE = fightgame
SUBDIRS = src

TOP_DIR := $(shell git rev-parse --show-toplevel)
include $(TOP_DIR)/makefile.inc

ifeq ($(UNIX),1)
includes:
	@if [ ! -d $(INC_DIR) ]; then mkdir $(INC_DIR); fi
	cp `find src -name *.h` $(INC_DIR)
else ifeq ($(UNIX),0)
includes:
	if not exist "$(INC_DIR)" mkdir "$(INC_DIR)"
	for /F %%G in ('dir /S /B *.h') do copy %%G "$(INC_DIR)"
else
includes:
	echo UNIX not set
endif

main: $(SUBDIRS)
	$(GCC) $(OBJ_DIR)/*.o -o $(WORKDIR)/$(EXE) $(LFLAGS) 

ifeq ($(UNIX),1)
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(INC_DIR)/*.h
	rm -f $(WORKDIR)/$(EXE)
else
clean:
	del /F $(OBJ_DIR)/*.o
	del /F $(OBJ_DIR)/*.h
	del /F $(WORKDIR)/$(EXE)
endif
