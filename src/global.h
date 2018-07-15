#ifndef GLOBAL_H_
#define GLOBAL_H_

#ifdef DEBUG_
    #ifndef PERR_
        #define PERR_
    #endif
    #define dbgprint(...) printf(__VA_ARGS__)
#else
    #define dbgprint(...) do{}while(0)
#endif

#ifdef PERR_
    #define errprint(...) fprintf(stderr, __VA_ARGS__)
#else
    #define errprint(...) do{}while(0)
#endif

#endif
