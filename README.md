### What is this?

Whenever you use C, a supposedly low level language, on a modern computer, a ton of things are already taken care of by the runtime. 
Even if you do not use the standard library directly, the compiler still does a lot of things in the background, most of them useful.

My goal here is to see how far I could get without the standard library and while using the minimum amount of compiler features. 
To start with assembly and raw syscalls and try to make a C-library-ish thing out of this.

### Goals

- implement most common syscalls and their structures as functions to better understand how this works internally in Linux
- write as much of the code as possible at a fairly low level to be able to understand exactly what I am doing
- not use the standard library or system headers at all


### What do I have?
- very basic allocator/memory management
- basic string formatting using my own `fmt()` function
- wrappers for a couple basic syscalls
- very basic library functions, e.g. `strcmp()` and `memcpy()`

### Things I don't want to do:
- Implement a full C standard library. This is just a hobby project and nobody should actually use this.
- Make functions I have compliant with some standard. This is intentionally different.
- Fix all issues. This is mainly about me trying to find out how things work under the hood and not trying to write a viable library.
- Make this cross platform. amd64 on linux with gcc is all I want to work on, mainly because I want to learn all the lower level things.


### What compiler/stdlib things do I still need to get rid of?

- ~~va_args: I'd really like to replace GCC's builtin implementation with my own~~ I've decided to not use va_args for formatting functions at all, I prefer using a list that gives me *some* type safety

### Known issues:
- Environment handling leaks memory. `setenv()` and `unsetenv()` do not free old values yet, this will probably take a rewrite of the allocator.
This is not that bad for now, since whenever you `setenv()` you're not far away from an `execve()` anyways
- The memory allocator is less than robust and can get fragmented, values might not get freed correctly and the internal data structure can get corrupted
- *Nothing* is thread safe. That would be too much work for me right now, especially as I won't need threads here.
- I do not set `errno` for *anything* yet

### Differences to "regular" C

I tried to do a few things differently since I am no longer limited by the regular C conventions:
#### Different/New types: 

I decided to use more idiomatic naming for most types. `u8` and `i64` are nicer to read and understand than `unsigned char` and `long int`. 
This is also why i generally use `bool` for functions that have two possible return values only.

#### Slightly different semantics

I try to avoid automatic allocations for a couple reasons:
- No need to remember whether something needs to be freed
- I can use the stack or static memory instead if needed
- More robust error checking since functions can return an integer for the number of bytes written instead of a `char*` that can only indicate an error using `NULL`
for example: `fmt()` returns `-3` if it managed to fully format the string, but didn't have enough space for a null terminator. This allows the caller to proceed as normal if it does not need the terminator
