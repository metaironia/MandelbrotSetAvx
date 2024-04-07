#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <windows.h>

const int WINDOW_SIZE_X = 800;
const int WINDOW_SIZE_Y = 600;

enum GraphicsFunc {

    GRAPHICS_FUNC_STATUS_OK,
    GRAPHICS_FUNC_STATUS_FAIL
};

GraphicsFunc MandelbrotDraw (void);

GraphicsFunc PixelColorSet (RGBQUAD *videomem,       size_t pixel_x, 
                            size_t   pixel_y,  const size_t iter_num);

#endif