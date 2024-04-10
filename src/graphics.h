#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <windows.h>

#include "mandelbrot_computation.h"

#define BENCHMARK_RES_(computation)  {                                                                           \
                                         fprintf (benchmark_results, "Benchmark results for " computation "\n"); \
                                         BenchmarkResultPrint (benchmark_results,                                \
                                                               config.benchmark.cycle_start,                     \
                                                               config.benchmark.cycle_end);                      \
                                     }

const int WINDOW_SIZE_X = 800;
const int WINDOW_SIZE_Y = 600;

enum GraphicsFunc {

    GRAPHICS_FUNC_STATUS_OK,
    GRAPHICS_FUNC_STATUS_FAIL
};

GraphicsFunc MandelbrotDraw (void);

inline GraphicsFunc ControlButtonPressCheck (ComputationConfig *config);

GraphicsFunc PixelColorSet (RGBQUAD *videomem,       size_t pixel_x, 
                            size_t   pixel_y,  const size_t iter_num);

#endif