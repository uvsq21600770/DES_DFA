PROJECT_NAME = RnD
CC=g++ -O3
PROJECT_SRC = .
vpath %.cpp $(PROJECT_SRC)

########################################################################
SRCS = RnDBeta.cpp
SRCS += DifDistrTab.cpp

########################################################################
INC_DIRS = Include
INCLUDE = $(addprefix -I ,$(INC_DIRS))
########################################################################

CFLAGS= -Wall \
       -pedantic \
	   -g


LDFLAGS=

########################################################################

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(SRCS)
	$(CC) $(INCLUDE) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) $< -c $(INCLUDE) $(CFLAGS)

clean:
	rm -f *.o $(PROJECT_NAME)
