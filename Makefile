CC = gcc
AS = gcc

CCFLAGS_REMOVE_BUILTINS = -nostdlib -nostdinc -fno-unwind-tables -fno-asynchronous-unwind-tables -Wno-builtin-declaration-mismatch -fno-stack-protector

CCFLAGS_LIBRARY = -fPIC -nostdlib -nostdinc -fno-unwind-tables -fno-asynchronous-unwind-tables -Wno-builtin-declaration-mismatch -fno-stack-protector 
CCFLAGS = 
ASFLAGS = -s

SRC_C = lib/syscall.c lib/io.c lib/string.c lib/memcpy.c lib/stat.c alloc/mmap.c alloc/alloc.c 
OBJ_C = $(addprefix $(OBJDIR)/, $(SRC_C:.c=.o))

SRC_S = asm/start.S asm/syscall.S
OBJ_S = $(addprefix $(OBJDIR)/, $(SRC_S:.S=.o))

OBJDIR = build

EXEC = no_std

SRC_EXAMPLES = examples/hello.c examples/cat.c
EXAMPLES = hello cat

LIBRARY = libnostd.o

exec: $(EXEC)

$(EXEC): $(LIBRARY)
	$(CC) $(CCFLAGS_REMOVE_BUILTINS) $(CCFLAGS) main.c $(LIBRARY) -o $(EXEC)

$(EXAMPLES): $(LIBRARY)
	$(CC) $(CCFLAGS_REMOVE_BUILTINS) $(CCFLAGS) examples/$@.c $(LIBRARY) -o $@


$(OBJDIR)/%.o: %.c
	$(CC) $(CCFLAGS_LIBRARY) $(CCFLAGS_REMOVE_BUILTINS) -c $< -o $@

$(OBJDIR)/%.o: %.S
	$(AS) $(ASFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ_C) $(OBJ_S) $(EXEC) $(LIBRARY) $(EXAMPLES)


$(LIBRARY): $(OBJ_C) $(OBJ_S)
	ld -r -o $(LIBRARY) $(OBJ_S) $(OBJ_C)

lib: $(LIBRARY)

examples: $(EXAMPLES)

all: lib exec


