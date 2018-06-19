#ifndef GLOBAL_H_
#define GLOBAL_H_

#ifdef DEBUG_
    #define dbgprint(...) printf(__VA_ARGS__)
#else
    #define dbgprint(...) do{}while(0)
#endif

#endif
