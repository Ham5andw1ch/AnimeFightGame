GCC = gcc
WORKDIR = $(TOP_DIR)
OBJ_DIR = $(WORKDIR)/build

# SQLFLAGS = -lpthread -ldl                                    # With threading and dynamic linking
# SQLFLAGS = -DSQLITE_THREADSAFE=0 -ldl                        # With dynamic linking, but not threading
# SQLFLAGS = -DSQLITE_OMIT_LOAD_EXTENSION -ldl 	               # With threading, but not dynamic linking
SQLFLAGS = -DSQLITE_THREADSAFE=0 -DSQLITE_OMIT_LOAD_EXTENSION # Without threading or dynamic linking

CFLAGS = $(SQLFLAGS) -I$(TOP_DIR)

%.o: %.c
	@if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	$(GCC) $(CFLAGS) -c $^ -o $(OBJ_DIR)/$@

$(SUBDIRS):
	$(MAKE) -C $@ all

.PHONY: $(SUBDIRS) all

all: $(OBS) $(SUBDIRS)
