#include <windows.h>
#include <assert.h>

#include "graphics.h"
#include "mandelbrot_computation.h"

ComputationFunc MandelbrotComputeNoSIMD (RGBQUAD *videomem) {

    assert (videomem);

    float y_0 = OFFSET_AXIS_Y;

    for (size_t y_pixel = 0; y_pixel < WINDOW_SIZE_Y; y_pixel++,
                                                      y_0 += DELTA_Y) {
        
        float x_0 = OFFSET_AXIS_X;
            
        for (size_t x_pixel = 0; x_pixel < WINDOW_SIZE_X; x_pixel++,
                                                          x_0 += DELTA_X) {

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