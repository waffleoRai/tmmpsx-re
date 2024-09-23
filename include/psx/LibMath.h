#ifndef PSXSDK_LIBMATH_H
#define PSXSDK_LIBMATH_H

#include "psx/PSXTypes.h"

//From LIBMATH.H

#define _ABS(x)		((x) < 0 ? -(x) : (x))
#define fabs(x)		_ABS(x)

extern int math_errno;
extern int math_err_point;

#ifdef __cplusplus
extern "C" {
#endif

extern double Math_sqrt(double);

#ifdef __cplusplus
}
#endif

#endif