#ifndef MANDELBROT_COMPUTATION_H
#define MANDELBROT_COMPUTATION_H

#include <windows.h>
#include <immintrin.h>

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
const int   MAX_COMPUTATION_NUM      = 256;

const int ACCUM_NUM = 4;

const __m256d INTR_MAND_RAD_SQ = _mm256_set1_pd (MANDELBROT_RADIUS_SQUARE);

const __m256d INTR_DELTA_X = _mm256_set1_pd (DELTA_X);
const __m256d INTR_DELTA_Y = _mm256_set1_pd (DELTA_Y);

const __m256d INTR_CONST_TWO = _mm256_set1_pd (2.0);
const __m256d INTR_0_TO_3    = _mm256_setr_pd (0.0, 1.0, 2.0, 3.0);
const __m256d INTR_STEP_X    = _mm256_mul_pd (INTR_DELTA_X, _mm256_set1_pd (ACCUM_NUM));

ComputationFunc MandelbrotComputeSillyNoSIMD (RGBQUAD *videomem);

ComputationFunc MandelbrotComputeSensibleNoSIMD (RGBQUAD *videomem);

ComputationFunc MandelbrotComputeSIMD (RGBQUAD *videomem);

#endif