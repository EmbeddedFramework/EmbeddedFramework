
/* Use the defaults for everything else */
#include_next<FreeRTOSConfig.h>

#ifdef configUSE_TICK_HOOK
#undef configUSE_TICK_HOOK
#endif

#define configUSE_TICK_HOOK                     1

