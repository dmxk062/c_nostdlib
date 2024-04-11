CC = gcc
AS = gcc

CCFLAGS = -nostdlib -fno-unwind-tables -fno-asynchronous-unwind-tables -Wno-builtin-declaration-mismatch
ASFLAGS = -s

SRC_C = lib/syscall.c lib/io.c lib/string.c main.c
OBJ_C = $(addprefix $(OBJDIR)/, $(SRC_C:.c=.o))

SRC_S = asm/start.S
OBJ_S = $(addprefix $(OBJDIR)/, $(SRC_S:.S=.o))

OBJDIR = build

EXEC = no_std

all: $(EXEC)

$(EXEC): $(OBJ_C) $(OBJ_S)
	$(CC) $(CCFLAGS) $(OBJ_C) $(OBJ_S) -o $(EXEC)

$(OBJDIR)/%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.S
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_C) $(OBJ_S) $(EXEC)

