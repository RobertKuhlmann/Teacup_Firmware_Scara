#ifndef	_DDA_MATHS_H
#define	_DDA_MATHS_H

#include	<stdint.h>

#include	"config_wrapper.h"

// return rounded result of multiplicand * multiplier / divisor
// this version is with quotient and remainder precalculated elsewhere
const int32_t muldivQR(int32_t multiplicand, uint32_t qn, uint32_t rn,
                       uint32_t divisor);

// return rounded result of multiplicand * multiplier / divisor
static int32_t muldiv(int32_t, uint32_t, uint32_t) __attribute__ ((always_inline));
inline int32_t muldiv(int32_t multiplicand, uint32_t multiplier,
                      uint32_t divisor) {
  return muldivQR(multiplicand, multiplier / divisor,
                  multiplier % divisor, divisor);
}

/*
	micrometer distance <=> motor step distance conversions
*/
// Like shown in the patch attached to this post:
// http://forums.reprap.org/read.php?147,89710,130225#msg-130225 ,
// it might be worth pre-calculating muldivQR()'s qn and rn in dda_init()
// as soon as STEPS_PER_M_{XYZE} is no longer a compile-time variable.

static int32_t um_to_steps_x(int32_t) __attribute__ ((always_inline));
inline int32_t um_to_steps_x(int32_t distance) {
    return muldivQR(distance, STEPS_PER_M_X / 1000000UL,
                    STEPS_PER_M_X % 1000000UL, 1000000UL);
}

static int32_t um_to_steps_y(int32_t) __attribute__ ((always_inline));
inline int32_t um_to_steps_y(int32_t distance) {
    return muldivQR(distance, STEPS_PER_M_Y / 1000000UL,
                    STEPS_PER_M_Y % 1000000UL, 1000000UL);
}

#ifdef SCARA_PRINTER
/*
	Scara-type printers need a different calculation, because the ratio
	between xy-coordinates and xy-steps is not constant.
	The start coordinates are needed here, because the steps needed between two points of the Scara-print
	area depend on the coordinates of start- and endpoint.
	You can calculate a step-pair for any cartesian coordinate, relative to bed's zero, but we need to
	calculate the real motor-steps needed to cover the given (cartesian) distance.
*/
void scara_um_to_steps(int32_t pos_x, //actual x_pos
							  int32_t pos_y, //actual y_pos
							  int32_t distance_x, //delta to new position along x-axis in micrometer
							  int32_t distance_y, //delta to new position along y-axis in micrometer
							  int32_t *steps_x,  //steps for Theta-arm
							  int32_t *steps_y);  //steps for Psi-arm 
#endif

static int32_t um_to_steps_z(int32_t) __attribute__ ((always_inline));
inline int32_t um_to_steps_z(int32_t distance) {
    return muldivQR(distance, STEPS_PER_M_Z / 1000000UL,
                    STEPS_PER_M_Z % 1000000UL, 1000000UL);
}

static int32_t um_to_steps_e(int32_t) __attribute__ ((always_inline));
inline int32_t um_to_steps_e(int32_t distance) {
    return muldivQR(distance, STEPS_PER_M_E / 1000000UL,
                    STEPS_PER_M_E % 1000000UL, 1000000UL);
}

// approximate 2D distance
uint32_t approx_distance(uint32_t dx, uint32_t dy);

// approximate 3D distance
uint32_t approx_distance_3(uint32_t dx, uint32_t dy, uint32_t dz);

// integer square root algorithm
uint16_t int_sqrt(uint32_t a);

// integer inverse square root, 12bits precision
uint16_t int_inv_sqrt(uint16_t a);

// this is an ultra-crude pseudo-logarithm routine, such that:
// 2 ^ msbloc(v) >= v
const uint8_t msbloc (uint32_t v);

// Calculates acceleration ramp length in steps.
uint32_t acc_ramp_len(uint32_t feedrate, uint32_t steps_per_m);

// For X axis only, should become obsolete:
#define ACCELERATE_RAMP_LEN(speed) (((speed)*(speed)) / (uint32_t)((7200000.0f * ACCELERATION) / (float)STEPS_PER_M_X))

// Initialization constant for the ramping algorithm.
#define C0 (((uint32_t)((double)F_CPU / sqrt((double)(STEPS_PER_M_X * ACCELERATION / 2000.)))) << 8)

#endif	/* _DDA_MATHS_H */
