## What is this?

Whenever you use C, a supposedly low level language, on a modern computer, a ton of things are already taken care of by the runtime. 
Even if you do not use the standard library directly, the compiler still does a lot of things in the background, most of them useful.

My goal here is to see how far I could get without the standard library and while using the minimum amount of compiler features. 
To start with assembly and raw syscalls and try to make a C-library-ish thing out of this. Ideally while coming up with my own ideas and not copying what C libraries do right now.

### Goals
- [x] simple syscall wrappers for a lot of things and basic utility functions:
    - [x] string formatting and formatted printing
- [x] basic memory allocator:
    - [ ] improve safety
- [ ] understand kernel interal data structures better and how to wrap them for userspace
- [ ] come up with my own ways of doing things instead of copying a regular libc


### Non-Goals
- Cross platform/architecture support. This will stay amd64/Linux. This applies to structure layout, type size and syscall numbers
- Make this safe/fast/very stable. I want to experiment more than I want to learn an actually viable piece of software. No one should use this except to have fun.
- Make it possible to compile existing C libraries or programs against this. I want to keep my own semantics and not imitate a POSIX libc


## Differences to "regular" C

### Type names

I'm using `i64`, `u64` and other more descriptive and shorter type names instead of `long int` and `unsigned long int`.
This is mainly for readability but also to make things like structure layouts easier to see.


### Error handling

##### Bad
Right now there are a few ways to return errors from a function in C, none of them that great:
- Return `NULL` on error. This is not possible if a function could return `0` and does not have a good way to communicate *what* happened outside of setting `errno`, which relies on a single global symbol.
- Return a negative number on error and a positive one on success. Does not work if negative return values are possible or if the return value is anything but an integer. This also sacrifices half of the size of a variable for error handling.

##### What i do
Return a success value and put output into a pointer argument. This is actually not that bad, but not as nice syntactically, since
it does not allow you to assign to the result. I use this in a few places where it makes sense, but try to not overuse it. 
I.e. I do not use it when the return value has any other purpose than to communicate an error
```c
struct SomeStruct var;
errno_t err = some_function(&var);
if (err) {
    handle_error(err);
} else {
    do_stuff(var);
}
```

Return some structure containing a success flag and a value/errno union. This is what I settled one for most things. Generally these end up being custom types of some kind. I decided to use the macros `DEFRESULT` and `RESULT` for nicer syntax:
```c
RESULT(i64) new_value = function(void);
if (!new_value.success) {
    handle_error(new_value.errno)
} else {
    do_thing(new_value.value)
}
```

This is not that bad syntactically and also communicates the return type quite well. It also makes it as easy to return an error from
a function if an error occured in a function it cast as with the other way

