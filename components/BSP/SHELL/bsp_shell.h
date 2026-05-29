#ifndef __BSP_SHELL_H__
#define __BSP_SHELL_H__

#include <stdint.h>
#include "shell.h"
void bsp_shell_init();

#define MAIN_SHELL_EXPORT_CMD(_name,_func,_desc) \
        SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN) | SHELL_CMD_DISABLE_RETURN, _name, _func, _desc);
#endif 