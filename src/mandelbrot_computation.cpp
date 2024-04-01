#include <windows.h>
#include <assert.h>

#include "graphics.h"
#include "mandelbrot_computation.h"

#define FOR_ACCUM  for (size_t i = 0; i < ACCUM_NUM; i++)
ComputationFunc MandelbrotComputeSillyNoSIMD (RGBQUAD *videomem) {

    assert (videomem);

    float y_0 = OFFSET_AXIS_Y;

    for (size_t y_pixel = 0; y_pixel < WINDOW_SIZE_Y; y_pixel++, y_0 += DELTA_Y) {
        
        float x_0 = OFFSET_AXIS_X;
            
        for (size_t x_pixel = 0; x_pixel < WINDOW_SIZE_X; x_pixel++, x_0 += DELTA_X) {

            float x = x_0;
            float y = y_0;

            size_t iter_num = 0;

            for (; iter_num <= MAX_COMPUTATION_NUM; iter_num++) {

                float curr_x_sq = x * x;
                float curr_y_sq = y * y;
                float curr_xy   = x * y;

                if ((curr_x_sq + curr_y_sq) > MANDELBROT_RADIUS_SQUARE)
                    break;
 
                x = curr_x_sq - curr_y_sq + x_0;
                y = 2 * curr_xy           + y_0; 
            }

            PixelColorSet (videomem, x_pixel, y_pixel, iter_num);
        }
    }

    return COMPUTATION_FUNC_STATUS_OK;
}

ComputationFunc MandelbrotComputeSensibleNoSIMD (RGBQUAD *videomem) {

    assert (videomem);

    float y_0 = OFFSET_AXIS_Y;

    for (size_t y_pixel = 0; y_pixel < WINDOW_SIZE_Y; y_pixel++, y_0 += DELTA_Y) {
        
        float x_0[ACCUM_NUM] = {OFFSET_AXIS_X,               OFFSET_AXIS_X + DELTA_X, 
                                OFFSET_AXIS_X + DELTA_X * 2, OFFSET_AXIS_X + DELTA_X * 3};
            
        for (size_t x_pixel = 0; x_pixel < WINDOW_SIZE_X; x_pixel += ACCUM_NUM) {

            float x[ACCUM_NUM] = {};
            float y[ACCUM_NUM] = {};

            FOR_ACCUM
                x[i] = x_0[i];

            FOR_ACCUM
                y[i] = y_0;

            size_t iter_num[ACCUM_NUM] = {};

            int pixel_mask           = 0;
            int is_dot_in[ACCUM_NUM] = {};

            FOR_ACCUM
                is_dot_in[i] = 1;

            for (size_t curr_iter = 0; curr_iter <= MAX_COMPUTATION_NUM; curr_iter++) {
                
                float curr_x_sq[ACCUM_NUM] = {};
                float curr_y_sq[ACCUM_NUM] = {};
                float curr_xy[ACCUM_NUM]   = {}; 

                FOR_ACCUM
                    curr_x_sq[i] = x[i] * x[i];
                
                FOR_ACCUM
                    curr_y_sq[i] = y[i] * y[i];
                
                FOR_ACCUM
                    curr_xy[i] = x[i] * y[i];

                FOR_ACCUM
                    if ((curr_x_sq[i] + curr_y_sq[i]) > MANDELBROT_RADIUS_SQUARE)
                        is_dot_in[i] = 0;
                
                FOR_ACCUM
                    pixel_mask |= (is_dot_in[i] << i);

                if (!pixel_mask)
                    break;

                FOR_ACCUM
                    x[i] = curr_x_sq[i] - curr_y_sq[i] + x_0[i];
                
                FOR_ACCUM
                    y[i] = 2 * curr_xy[i] + y_0; 

                FOR_ACCUM
                    iter_num[i] += is_dot_in[i];
            }

            FOR_ACCUM
                PixelColorSet (videomem, x_pixel + i, y_pixel, iter_num[i]);

            FOR_ACCUM
                x_0[i] += ACCUM_NUM * DELTA_X;
        }
    }

    return COMPUTATION_FUNC_STATUS_OK;
}