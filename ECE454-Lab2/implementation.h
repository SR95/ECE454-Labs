#ifndef ECE454_LAB2_IMPLEMENTATION_H
#define ECE454_LAB2_IMPLEMENTATION_H

// WARNING: DO NOT REMOVE this prototype
void print_team_info();

// WARNING: DO NOT REMOVE this prototype
void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
                                     unsigned int width, unsigned int height, bool grading_mode);

unsigned char *processRotateCW(unsigned char *buffer_frame, unsigned width, unsigned height, int rotate_iteration);
unsigned char *processRotateCCW(unsigned char *buffer_frame, unsigned width, unsigned height, int rotate_iteration);

#endif //ECE454_LAB2_IMPLEMENTATION_H
