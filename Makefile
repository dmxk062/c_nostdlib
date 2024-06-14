CC = gcc
AS = gcc

CCFLAGS_REMOVE_BUILTINS = -nostdlib -nostdinc -fno-unwind-tables -fno-asynchronous-unwind-tables -Wno-builtin-declaration-mismatch -fno-stack-protector -Wall

CCFLAGS_LIBRARY = -fPIC 
CCFLAGS = -Ilib/include/ -include lib/include/types.h 
CCFLAGS_EXE = -Wl,--gc-sections
ASFLAGS = -s
LDFLAGS = 

SRC_C = $(wildcard lib/*.c) $(wildcard lib/structs/*.c) $(wildcard lib/linux/*.c)
OBJ_C = $(addprefix $(OBJDIR)/, $(SRC_C:.c=.o))

SRC_S = asm/start.S asm/syscall.S asm/signal.S
OBJ_S = $(addprefix $(OBJDIR)/, $(SRC_S:.S=.o))

OBJDIR = build

EXEC = no_std


SRC_EXAMPLES = $(wildcard examples/*.c)
EXAMPLES = $(SRC_EXAMPLES:.c=)

BUILDDIRS = build build/lib/linux build/lib/structs build/asm


LIBRARY = libnostd.o

$(BUILDDIRS):
	mkdir -p $(BUILDDIRS)

exec: $(EXEC)

$(EXEC): $(BUILDDIRS) $(LIBRARY)
	$(CC) $(CCFLAGS_REMOVE_BUILTINS) $(CCFLAGS) $(CCFLAGS_EXE) main.c $(LIBRARY) -o $(EXEC)

$(EXAMPLES): $(BUILDDIRS) $(LIBRARY) 
	$(CC) $(CCFLAGS_REMOVE_BUILTINS) $(CCFLAGS) $(CCFLAGS_EXE) $@.c $(LIBRARY) -o $@

$(OBJDIR)/%.o: %.c
	$(CC) $(CCFLAGS_LIBRARY) $(CCFLAGS) $(CCFLAGS_REMOVE_BUILTINS) -c $< -o $@

$(OBJDIR)/%.o: %.S
	$(AS) $(ASFLAGS) -c $< -o $@




$(LIBRARY): $(OBJ_C) $(OBJ_S) $(BUILDDIRS)
	ld -r $(LDFLAGS) -o $(LIBRARY) $(OBJ_S) $(OBJ_C)

lib: $(LIBRARY)

examples: $(EXAMPLES)


all: lib exec examples 

clean:
	rm -rf $(OBJ_C) $(OBJ_S) $(EXEC) $(LIBRARY) $(EXAMPLES) $(BUILDDIRS)

