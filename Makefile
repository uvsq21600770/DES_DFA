PROJECT_NAME = DES_DFA_M1
PROJECT_NAME_M2 = DES_DFA_M2
CC=g++ -O3
PROJECT_SRC = .
vpath %.cpp $(PROJECT_SRC)

########################################################################
SRCS = DES_DFA_M1.cpp
SRCS += DifDistrTab.cpp

SRCS_M2 = DES_DFA_M2.cpp

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

M2: $(PROJECT_NAME_M2)

$(PROJECT_NAME_M2): $(SRCS_M2)
	$(CC) $(INCLUDE) $(CFLAGS) $^ -o $@ $(LDFLAGS)

run:
	./$(PROJECT_NAME)

run2:
	./$(PROJECT_NAME_M2)

%.o: %.cpp
	$(CC) $< -c $(INCLUDE) $(CFLAGS)

clean:
	rm -f *.o $(PROJECT_NAME)
