MPICC = gcc
CFLAGS = -Wall -O2 -fopenmp
TARGET = main
SRCS = main.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(MPICC) $(CFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)
