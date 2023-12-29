
/* Use the defaults for everything else */
#include_next<FreeRTOSConfig.h>

#ifdef configUSE_TIME_SLICING
#undef configUSE_TIME_SLICING
#endif
#define configUSE_TIME_SLICING                  1


#ifdef configIDLE_SHOULD_YIELD
#undef configIDLE_SHOULD_YIELD
#endif
#define configIDLE_SHOULD_YIELD                 0
