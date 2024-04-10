CC = gcc
AS = gcc

CCFLAGS = -nostdlib -fno-unwind-tables -fno-asynchronous-unwind-tables -Wno-builtin-declaration-mismatch
ASFLAGS = -s

SRC_C = syscall.c main.c io.c
OBJ_C = $(SRC_C:.c=.o)

SRC_S = _start.S
OBJ_S = $(SRC_S:.S=.o)

EXEC = no_std

all: $(EXEC)

$(EXEC): $(OBJ_C) $(OBJ_S)
	$(CC) $(CCFLAGS) $(OBJ_C) $(OBJ_S) -o $(EXEC)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_C) $(OBJ_S) $(EXEC)

