IGNORE = character/ audio/ test.o
SUBDIRS = $(filter-out $(IGNORE), $(shell ls -p | grep /))
OBS = $(filter-out $(IGNORE), $(addsuffix .o, $(basename $(wildcard *.c))))

TOP_DIR := $(shell git rev-parse --show-toplevel)
include $(TOP_DIR)/makefile.inc
