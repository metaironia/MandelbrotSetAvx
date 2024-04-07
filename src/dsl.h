#ifndef DSL_H
#define DSL_H

#define NUM_DELTA_Y_         (config -> numbers_config).delta_y
#define NUM_DELTA_X_         (config -> numbers_config).delta_x
#define NUM_STEP_X_          (config -> numbers_config).step_x
#define NUM_OFFSET_AXIS_X_   (config -> numbers_config).offset_axis_x
#define NUM_OFFSET_AXIS_Y_   (config -> numbers_config).offset_axis_y

#define INTR_DELTA_Y_        (config -> intrinsics_config).delta_y
#define INTR_DELTA_X_        (config -> intrinsics_config).delta_x
#define INTR_STEP_X_         (config -> intrinsics_config).step_x
#define INTR_OFFSET_AXIS_X_  (config -> intrinsics_config).offset_axis_x
#define INTR_OFFSET_AXIS_Y_  (config -> intrinsics_config).offset_axis_y

#endif