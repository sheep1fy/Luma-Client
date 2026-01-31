#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern bool menu_open;
void init_luma_manager(void);
void toggle_luma_menu(void);
void run_luma_tick(void);

#ifdef __cplusplus
}
#endif
