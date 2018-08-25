GCC= gcc
CFLAGS= -g
LFLAGS =

EXE = test
OBS = $(addsuffix .o, $(basename $(wildcard *.c)))

%.o: %.c
	$(GCC) $(CFLAGS) -c $^

$(EXE): $(OBS)
	$(GCC) $(OBS) -o $(EXE) $(LFLAGS) 

clean:
	rm -f $(OBS)
	rm -f $(EXE)
