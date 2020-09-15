#define NUM_ARGS 2

static char __argvbuf0[] = {"exe"};
static char __argvbuf1[] = {"-v"};

static int __argc  = NUM_ARGS;
static char * __argv[NUM_ARGS] = { __argvbuf0, __argvbuf1 };

void __iar_argc_argv(void)
{
    // setup R0 (argc) and R1 (argv) arguments to main (see cmain.s)
    asm volatile ("MOVS R1, %0" : : "r" (__argv));
    asm volatile ("MOVS R0, %0" : : "r" (__argc));
}
