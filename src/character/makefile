SUBDIRS = $(shell ls -p | grep /)
OBS = $(addsuffix .o, $(basename $(wildcard *.c)))

TOP_DIR := $(shell git rev-parse --show-toplevel)
include $(TOP_DIR)/makefile.inc
