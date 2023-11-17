
/* Use the defaults for everything else */
#include_next<FreeRTOSConfig.h>

#ifdef configUSE_TIMERS
#undef configUSE_TIMERS
#endif

#define configUSE_TIMERS                     1

