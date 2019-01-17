################################################################################
#Makefile Linux
#Set DEBUG to 1 to compile in debug mode : $make DEBUG=1
################################################################################

PROG	= canardage
ARG		= 127.0.0.1 5650 0

CC		= g++
RM		= rm

SRCD	= src
INCD	= inc

CFLAGS	= -O2 -I$(INCD)
BFLAGS	=
LDFLAGS = -std=c++0x
LOG		= logfile_*

DEBUG	= 2
ifeq ($(DEBUG),2)
    LDFLAGS += -g -Wall -Wextra -Wformat=2 -Wunreachable-code -Wpedantic -Wno-unused-parameter
else
    LDFLAGS += -DNDEBUG
endif

SRC		= $(wildcard $(SRCD)/**/*.cpp $(SRCD)/*.cpp)
OBJS	= $(patsubst %.cpp, %.o, $(SRC))

.PHONY: clean

all: $(PROG) _clean


$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(BFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<


_clean:
	$(RM) -f #$(SRCD)/*.o $(SRCD)/*/*.o

clean:
	$(RM) -f $(SRCD)/*.o $(SRCD)/*/*.o $(LOG)

valg:
	valgrind --tool=memcheck --leak-check=full --leak-resolution=high \
	--show-reachable=yes ./$(PROG) $(ARG)