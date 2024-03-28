#include "../lib/TXLib.h"

#include <assert.h>
#include <math.h>

#include "mandelbrot_computation.h"
#include "graphics.h"

GraphicsFunc MandelbrotDraw (void) {

    txCreateWindow (WINDOW_SIZE_X, WINDOW_SIZE_Y);

    RGBQUAD *vmem_buffer = txVideoMemory();
    assert (vmem_buffer);

    Win32::_fpreset();
    txBegin();

    MandelbrotComputeNoSIMD (vmem_buffer);

    txEnd();

    return GRAPHICS_FUNC_STATUS_OK;
}

GraphicsFunc PixelColorSet (RGBQUAD *videomem, size_t pixel_x, 
                            size_t pixel_y,    const size_t iter_num) {

    assert (videomem);

    size_t curr_pixel_pos = (pixel_y) * WINDOW_SIZE_X + pixel_x;
//fprintf (stderr, "set pixel %zu\n", curr_pixel_pos);
    RGBQUAD *current_pixel = videomem + curr_pixel_pos;

    if (iter_num >= MAX_COMPUTATION_NUM) {

        *current_pixel = {};
        return GRAPHICS_FUNC_STATUS_OK;
    }

    //current_pixel -> rgbBlue  = (BYTE) (sqrt (iter_num) * 256); 
    current_pixel -> rgbGreen = (BYTE) (sqrt (sqrt (iter_num)) * 256);
    //current_pixel -> rgbRed   = (BYTE) (iter_num * 256); 

    return GRAPHICS_FUNC_STATUS_OK;
}