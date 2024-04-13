typedef __builtin_va_list va_list;
/* but you’re not allowed to use the name `va_arg`, technically */
#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_copy __builtin_va_copy
#define va_arg __builtin_va_arg
