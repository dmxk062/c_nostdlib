### What is this?

Whenever you use C, a supposedly low level language, on a modern computer, a ton of things are already taken care of by the runtime. 
Even if you do not use the standard library directly, the compiler still does a lot of things in the background, most of them useful.

My goal here was to see how far I could get without the standard library and while using the minimum amount of compiler features. 
To start with assembly and raw syscalls and try to make a C-library-ish thing out of this.


### What do I have?
- very basic allocator/memory management
- basic string formatting using my own `fmt()` function
- wrappers for a couple basic syscalls
- very basic library functions, e.g. `strcmp()` and `memcpy()`

### Things i don't want to do:
- Implement a full C standard library. This is just a hobby project and nobody should actually use this.
- Make functions I have compliant with some standard. This is intentionally different.
- Fix all issues. This is mainly about me trying to find out how things work under the hood and not trying to write a viable library.
- Make this cross platform. amd64 on linux with gcc is all I want to work on, mainly because I want to learn all the lower level things.


### What compiler/stdlib things do I still need to get rid of?

- va_args: I'd really like to replace GCC's builtin implementation with my own

### Known issues:
- Environment handling leaks memory. `setenv()` and `unsetenv()` do not free old values yet, this will probably take a rewrite of the allocator.
This is not that bad for now, since whenever you `setenv()` you're not far away from an `execve()` anyways
- The memory allocator is less than robust and can get fragmented, values might not get freed correctly and the internal data structure can get corrupted
- *Nothing* is thread safe. That would be too much work for me right now, especially as I won't need threads here.


