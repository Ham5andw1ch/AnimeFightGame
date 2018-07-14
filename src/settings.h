#ifndef SETTINGS_H_
#define SETTINGS_H_

#define MaxJoysticks 10
#define ButtonCount 8
#define PlayerCount 2

#define QueueLength 30
#define QueueTolerance 3
#define FiveLimit 10
#define FPS 60

#define DefaultKeybinds {           \
    {                               \
        { BIND_HAT, 0, 0, 1 },      \
        { BIND_HAT, 0, 0, 4 },      \
        { BIND_HAT, 0, 0, 8 },      \
        { BIND_HAT, 0, 0, 2 },      \
        { BIND_BUTTON, 0, 2, 0 },   \
        { BIND_BUTTON, 0, 3, 0 },   \
        { BIND_BUTTON, 0, 5, 0 },   \
        { BIND_BUTTON, 0, 0, 0 },   \
    },                              \
    {                               \
        { BIND_HAT, 1, 0, 1 },      \
        { BIND_HAT, 1, 0, 4 },      \
        { BIND_HAT, 1, 0, 8 },      \
        { BIND_HAT, 1, 0, 2 },      \
        { BIND_BUTTON, 1, 2, 0 },   \
        { BIND_BUTTON, 1, 3, 0 },   \
        { BIND_BUTTON, 1, 5, 0 },   \
        { BIND_BUTTON, 1, 0, 0 },   \
    }                               \
} 

#endif
