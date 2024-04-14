CC = gcc
AS = gcc

CCFLAGS_REMOVE_BUILTINS = -nostdlib -nostdinc -fno-unwind-tables -fno-asynchronous-unwind-tables -Wno-builtin-declaration-mismatch -fno-stack-protector

CCFLAGS_LIBRARY = -fPIC 
CCFLAGS = -O
ASFLAGS = -s
LDFLAGS = 

SRC_C = $(wildcard lib/*.c) $(wildcard alloc/*.c)
OBJ_C = $(addprefix $(OBJDIR)/, $(SRC_C:.c=.o))

SRC_S = asm/start.S asm/syscall.S asm/signal.S
OBJ_S = $(addprefix $(OBJDIR)/, $(SRC_S:.S=.o))

OBJDIR = build

EXEC = no_std

SRC_EXAMPLES = examples/hello.c examples/cat.c examples/process.c
EXAMPLES = $(addprefix examples/, hello stat process)

LIBRARY = libnostd.o

exec: $(EXEC)

$(EXEC): $(LIBRARY)
	$(CC) $(CCFLAGS_REMOVE_BUILTINS) $(CCFLAGS) main.c $(LIBRARY) -o $(EXEC)

$(EXAMPLES): $(LIBRARY)
	$(CC) $(CCFLAGS_REMOVE_BUILTINS) $(CCFLAGS) $@.c $(LIBRARY) -o $@


$(OBJDIR)/%.o: %.c
	$(CC) $(CCFLAGS_LIBRARY) $(CCFLAGS_REMOVE_BUILTINS) -c $< -o $@

$(OBJDIR)/%.o: %.S
	$(AS) $(ASFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ_C) $(OBJ_S) $(EXEC) $(LIBRARY) $(EXAMPLES)


$(LIBRARY): $(OBJ_C) $(OBJ_S)
	ld -r $(LDFLAGS) -o $(LIBRARY) $(OBJ_S) $(OBJ_C)

lib: $(LIBRARY)

examples: $(EXAMPLES)

all: lib exec


