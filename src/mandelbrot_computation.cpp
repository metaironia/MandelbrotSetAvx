#include <stdio.h>

#include <windows.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <immintrin.h>

#include "graphics.h"
#include "mandelbrot_computation.h"
#include "dsl.h"

ComputationFunc BenchmarkResultPrint (FILE *benchmark_result, const int64_t cycle_start,
                                                              const int64_t cycle_end) {

    const int64_t cycles_whole_computation = cycle_end - cycle_start;

    fprintf (benchmark_result, "Computations repeated %d times,\n"
                               "Initial CPU cycle:          %lld\n"
                               "End CPU cycle:              %lld\n"
                               "Cycles to compute all:      %lld\n"
                               "Cycles per one computation: %lg\n\n",
                               BENCHMARK_COMPUTE_TIMES,
                               cycle_start, cycle_end,
                               cycles_whole_computation, 
                               ((double) cycles_whole_computation) / BENCHMARK_COMPUTE_TIMES);

    return COMPUTATION_FUNC_STATUS_OK;
}

ComputationFunc ConfigCtor (ComputationConfig *config) {

    assert (config);

    ConfigDataCtor (config);

    return COMPUTATION_FUNC_STATUS_OK;
}

ComputationFunc ConfigDataCtor (ComputationConfig *config) {

    assert (config);

    (config -> numbers_config).delta_x          = DEFAULT_DELTA_X; 
    (config -> numbers_config).delta_y          = DEFAULT_DELTA_Y;
    (config -> numbers_config).offset_axis_x    = DEFAULT_OFFSET_AXIS_X;
    (config -> numbers_config).offset_axis_y    = DEFAULT_OFFSET_AXIS_Y;
    (config -> numbers_config).step_x           = DEFAULT_STEP_X;

    (config -> intrinsics_config).delta_x       = _mm256_set1_pd (DEFAULT_DELTA_X); 
    (config -> intrinsics_config).delta_y       = _mm256_set1_pd (DEFAULT_DELTA_Y);
    (config -> intrinsics_config).offset_axis_x = _mm256_set1_pd (DEFAULT_OFFSET_AXIS_X);
    (config -> intrinsics_config).offset_axis_y = _mm256_set1_pd (DEFAULT_OFFSET_AXIS_Y);
    (config -> intrinsics_config).step_x        = _mm256_set1_pd (DEFAULT_STEP_X);

    (config -> benchmark).cycle_start           = 0;
    (config -> benchmark).cycle_end             = 0;

    return COMPUTATION_FUNC_STATUS_OK;
}

ComputationFunc ConfigDataDtor (ComputationConfig *config) {
    
    assert (config);

    memset (config, 0, sizeof (ComputationConfig));

    return COMPUTATION_FUNC_STATUS_OK;
}

ComputationFunc ConfigDtor (ComputationConfig *config) {

    assert (config);

    ConfigDataDtor (config);

    return COMPUTATION_FUNC_STATUS_OK;
}

ComputationFunc MandelbrotComputeSillyNoSIMD (RGBQUAD *videomem, ComputationConfig *config) {

    assert (videomem);
    assert (config);

    double y_0 = NUM_OFFSET_AXIS_Y_;

#ifdef BENCHMARK
    (config -> benchmark).cycle_start = _rdtsc();

    for (size_t bench_iter = 0; bench_iter < BENCHMARK_COMPUTE_TIMES; bench_iter++)
#endif

    for (volatile size_t y_pixel = 0; y_pixel < WINDOW_SIZE_Y; y_pixel++, y_0 += NUM_DELTA_Y_) {
        
        double x_0 = NUM_OFFSET_AXIS_X_;
            
        for (volatile size_t x_pixel = 0; x_pixel < WINDOW_SIZE_X; x_pixel++, x_0 += NUM_DELTA_X_) {

            double x = x_0;
            double y = y_0;

            volatile size_t iter_num = 0;

            for (; iter_num <= MAX_COMPUTATION_NUM; iter_num++) {

                double curr_x_sq = x * x;
                double curr_y_sq = y * y;
                double curr_xy   = x * y;

                if ((curr_x_sq + curr_y_sq) > MANDELBROT_RADIUS_SQUARE)
                    break;
 
                x = curr_x_sq - curr_y_sq + x_0;
                y = 2 * curr_xy           + y_0; 
            }

#ifndef BENCHMARK
            PixelColorSet (videomem, x_pixel, y_pixel, iter_num);
#endif
        }
    }

#ifdef BENCHMARK
    (config -> benchmark).cycle_end = _rdtsc();
#endif

    return COMPUTATION_FUNC_STATUS_OK;
}

ComputationFunc MandelbrotComputeSensibleNoSIMD (RGBQUAD *videomem, ComputationConfig *config) {

    assert (videomem);
    assert (config);

    double y_0[ACCUM_NUM] = {NUM_OFFSET_AXIS_Y_, NUM_OFFSET_AXIS_Y_,
                             NUM_OFFSET_AXIS_Y_, NUM_OFFSET_AXIS_Y_};

#ifdef BENCHMARK
    (config -> benchmark).cycle_start = _rdtsc();

    for (size_t bench_iter = 0; bench_iter < BENCHMARK_COMPUTE_TIMES; bench_iter++)
#endif

    for (volatile size_t y_pixel = 0; y_pixel < WINDOW_SIZE_Y; y_pixel++) {
        
        double x_0[ACCUM_NUM] = {NUM_OFFSET_AXIS_X_,                    
                                 NUM_OFFSET_AXIS_X_ + NUM_DELTA_X_, 
                                 NUM_OFFSET_AXIS_X_ + NUM_DELTA_X_ * 2, 
                                 NUM_OFFSET_AXIS_X_ + NUM_DELTA_X_ * 3};
            
        for (volatile size_t x_pixel = 0; x_pixel < WINDOW_SIZE_X; x_pixel += ACCUM_NUM) {

            double x[ACCUM_NUM] = {};
            double y[ACCUM_NUM] = {};

            FOR_ACCUM
                x[i] = x_0[i];

            FOR_ACCUM
                y[i] = y_0[i];

            int is_dot_in[ACCUM_NUM] = {};

            FOR_ACCUM
                is_dot_in[i] = 1;

            volatile size_t iter_num[ACCUM_NUM]  = {};

            for (size_t curr_iter = 0; curr_iter <= MAX_COMPUTATION_NUM; curr_iter++) {
                
                double curr_x_sq[ACCUM_NUM] = {};
                double curr_y_sq[ACCUM_NUM] = {};
                double curr_xy[ACCUM_NUM]   = {}; 
                double radius_sq[ACCUM_NUM] = {};

                FOR_ACCUM
                    curr_x_sq[i] = x[i] * x[i];
                
                FOR_ACCUM
                    curr_y_sq[i] = y[i] * y[i];
                
                FOR_ACCUM
                    curr_xy[i] = x[i] * y[i];

                FOR_ACCUM
                    radius_sq[i] = curr_y_sq[i] + curr_x_sq[i];

                FOR_ACCUM
                    if (radius_sq[i] > MANDELBROT_RADIUS_SQUARE)
                        is_dot_in[i] = 0;
                
                int pixel_mask = 0;

                FOR_ACCUM
                    pixel_mask |= (is_dot_in[i] << i);

                if (!pixel_mask)
                    break;

                FOR_ACCUM
                    x[i] = curr_x_sq[i] - curr_y_sq[i] + x_0[i];
                
                FOR_ACCUM
                    y[i] = 2 * curr_xy[i] + y_0[i]; 

                FOR_ACCUM
                    iter_num[i] += is_dot_in[i];
            }

#ifndef BENCHMARK
            FOR_ACCUM
                PixelColorSet (videomem, x_pixel + i, y_pixel, iter_num[i]);
#endif

            FOR_ACCUM
                x_0[i] += NUM_STEP_X_;
        }

        FOR_ACCUM
            y_0[i] += NUM_DELTA_Y_;
    }

#ifdef BENCHMARK
    (config -> benchmark).cycle_end = _rdtsc();
#endif

    return COMPUTATION_FUNC_STATUS_OK;
}

ComputationFunc MandelbrotComputeSIMD (RGBQUAD *videomem, ComputationConfig *config) {

    assert (videomem);
    assert (config);

    __m256d y_0 = INTR_OFFSET_AXIS_Y_;

#ifdef BENCHMARK
    (config -> benchmark).cycle_start = _rdtsc();

    for (size_t bench_iter = 0; bench_iter < BENCHMARK_COMPUTE_TIMES; bench_iter++)
#endif

    for (volatile size_t y_pixel = 0; y_pixel < WINDOW_SIZE_Y; y_pixel++) {
        
        __m256d x_0 = _mm256_add_pd (INTR_OFFSET_AXIS_X_, _mm256_mul_pd (INTR_DELTA_X_, INTR_0_TO_3));

        for (volatile size_t x_pixel = 0; x_pixel < WINDOW_SIZE_X; x_pixel += ACCUM_NUM) {

            __m256d x = x_0;
            __m256d y = y_0;

            volatile __m256d iter_num = _mm256_setzero_pd();

            for (size_t curr_iter = 0; curr_iter <= MAX_COMPUTATION_NUM; curr_iter++) {
                
                __m256d curr_x_sq      = _mm256_mul_pd (x, x);
                __m256d curr_y_sq      = _mm256_mul_pd (y, y);
                __m256d curr_xy        = _mm256_mul_pd (x, y); 
                __m256d curr_x_sq_y_sq = _mm256_add_pd (curr_x_sq, curr_y_sq);

                __m256d is_dot_in = _mm256_cmp_pd (curr_x_sq_y_sq, INTR_MAND_RAD_SQ, _CMP_LT_OQ);

                if (!_mm256_movemask_pd (is_dot_in))
                    break;

                x = _mm256_add_pd (_mm256_sub_pd (curr_x_sq, curr_y_sq),    x_0);      
                y = _mm256_add_pd (_mm256_mul_pd (INTR_CONST_TWO, curr_xy), y_0);

                __m256d sum_mask = _mm256_blendv_pd (_mm256_setzero_pd(), INTR_CONST_ONE, is_dot_in); 
                        iter_num = _mm256_add_pd    (sum_mask, iter_num);
            }

#ifndef BENCHMARK
            double *pixel_iter_num = (double *) (&iter_num); 

            FOR_ACCUM
                PixelColorSet (videomem, x_pixel + i, y_pixel, (size_t) pixel_iter_num[i]);
#endif
            x_0 = _mm256_add_pd (x_0, INTR_STEP_X_);
        }

        y_0 = _mm256_add_pd (y_0, INTR_DELTA_Y_);
    }

#ifdef BENCHMARK
    (config -> benchmark).cycle_end = _rdtsc();
#endif

    return COMPUTATION_FUNC_STATUS_OK;
}