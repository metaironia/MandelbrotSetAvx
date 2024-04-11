#ifndef MANDELBROT_COMPUTATION_H
#define MANDELBROT_COMPUTATION_H

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>

#define FOR_ACCUM  for (size_t i = 0; i < ACCUM_NUM; i++)

enum ComputationFunc {

    COMPUTATION_FUNC_STATUS_OK,
    COMPUTATION_FUNC_STATUS_FAIL
};

const int BENCHMARK_COMPUTE_TIMES = 1000;

const int ACCUM_NUM = 4;

const double DEFAULT_DELTA_X = 3.f / 800;
const double DEFAULT_DELTA_Y = 3.f / 800;
const double DEFAULT_STEP_X  = DEFAULT_DELTA_X * ACCUM_NUM;

const double DEFAULT_OFFSET_AXIS_X = -2.f;
const double DEFAULT_OFFSET_AXIS_Y = -1.f;

const double MANDELBROT_RADIUS_SQUARE = 100.f;
const int    MAX_COMPUTATION_NUM      = 256;

const __m256d INTR_MAND_RAD_SQ = _mm256_set1_pd (MANDELBROT_RADIUS_SQUARE);

const __m256d INTR_CONST_ONE = _mm256_set1_pd (1.0);
const __m256d INTR_CONST_TWO = _mm256_set1_pd (2.0);
const __m256d INTR_CONST_TEN = _mm256_set1_pd (10.0);
const __m256d INTR_0_TO_3    = _mm256_setr_pd (0.0, 1.0, 2.0, 3.0);


struct ComputationConfig {

    struct {

        __m256d delta_x;
        __m256d delta_y;
        __m256d step_x;
        __m256d offset_axis_x;
        __m256d offset_axis_y;
    } intrinsics_config;

    struct {

        double delta_x;
        double delta_y;
        double step_x;
        double offset_axis_x;
        double offset_axis_y;
    } numbers_config;

    struct {
        int64_t cycle_start;
        int64_t cycle_end;
    } benchmark;
};

ComputationFunc BenchmarkResultPrint (FILE *benchmark_result, const int64_t cycle_start,
                                                              const int64_t cycle_end);

ComputationFunc ConfigCtor (ComputationConfig *config);

ComputationFunc ConfigDataCtor (ComputationConfig *config);

ComputationFunc ConfigDataDtor (ComputationConfig *config);

ComputationFunc ConfigDtor (ComputationConfig *config);

ComputationFunc MandelbrotComputeSillyNoSIMD (RGBQUAD *videomem, ComputationConfig *config);

ComputationFunc MandelbrotComputeSensibleNoSIMD (RGBQUAD *videomem, ComputationConfig *config);

ComputationFunc MandelbrotComputeSIMD (RGBQUAD *videomem, ComputationConfig *config);

#endif