#pragma once
double luma_game_get_x();
double luma_game_get_y();
double luma_game_get_z();
void   luma_game_set_coords(double x, double y, double z);
int    luma_game_get_ping();
void   luma_game_set_ping(int ms);
bool   luma_game_is_auto_sprint();
void   luma_game_set_auto_sprint(bool v);
