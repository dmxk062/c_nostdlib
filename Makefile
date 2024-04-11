CC = gcc
AS = gcc

CCFLAGS_REMOVE_BUILTINS = -nostdlib -fno-unwind-tables -fno-asynchronous-unwind-tables -Wno-builtin-declaration-mismatch -fno-stack-protector
CCFLAGS = 
ASFLAGS = -s

SRC_C = lib/syscall.c lib/io.c lib/string.c alloc/mmap.c alloc/alloc.c main.c 
OBJ_C = $(addprefix $(OBJDIR)/, $(SRC_C:.c=.o))

SRC_S = asm/start.S
OBJ_S = $(addprefix $(OBJDIR)/, $(SRC_S:.S=.o))

OBJDIR = build

EXEC = no_std

all: $(EXEC)

$(EXEC): $(OBJ_C) $(OBJ_S)
	$(CC) $(CCFLAGS) $(CCFLAGS_REMOVE_BUILTINS) $(OBJ_C) $(OBJ_S) -o $(EXEC)

$(OBJDIR)/%.o: %.c
	$(CC) $(CCFLAGS) $(CCFLAGS_REMOVE_BUILTINS) -c $< -o $@

$(OBJDIR)/%.o: %.S
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_C) $(OBJ_S) $(EXEC)

