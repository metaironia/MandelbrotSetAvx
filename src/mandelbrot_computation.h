#ifndef MANDELBROT_COMPUTATION_H
#define MANDELBROT_COMPUTATION_H

#include <windows.h>

#include "graphics.h"

enum ComputationFunc {

    COMPUTATION_FUNC_STATUS_OK,
    COMPUTATION_FUNC_STATUS_FAIL
};

const float DELTA_X = 3.f / 800;
const float DELTA_Y = 3.f / 800;

const float OFFSET_AXIS_X = -2.f;
const float OFFSET_AXIS_Y = -1.f;

const float MANDELBROT_RADIUS_SQUARE = 100.f;

const int MAX_COMPUTATION_NUM = 256;

ComputationFunc MandelbrotComputeNoSIMD (RGBQUAD *videomem);

#endif