#pragma once
#include "std.h"

extern u8 PS2_LOCKS;

//extern void (*PS2_MAKE_CB) (u8);
//extern void (*PS2_BREAK_CB) (u8);
//extern void (*PS2_TYPED_CB) (u8);
//extern void (*PS2_ASCII_CB) (u8);

#include <PS2_Keys.h>

extern u8 PS2_KEY_DOWN[KEY_NUM_KEYS];

u16 PS2_INIT();

#define PS2_INT_RETURN_OS 1
#define PS2_INT_RETURN_DUMP 2
int PS2_INT();