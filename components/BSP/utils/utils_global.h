#ifndef _UTILS_GLOBAL_H_
#define _UTILS_GLOBAL_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MIN(n,m) (((n)<(m)) ? (n) : (m))

uint32_t GetSysTickMs(void);

#endif