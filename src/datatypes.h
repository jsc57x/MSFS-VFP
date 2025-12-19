#pragma once

//general
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long i64;

typedef float f32;
typedef double f64;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// subject-specific
struct AircraftState
{
    double latitude;
    double longitude;
    double altitude;
    double heading;
    double bank;
    double pitch;
    double speed;
};