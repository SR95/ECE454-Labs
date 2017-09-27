#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line
#include "implementation.h"

struct kv netModifications[3];

#define vertical 0
#define horizontal 1
#define rotation 2

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveUp(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {

    // allocate memory for temporary image buffer
    unsigned char *rendered_frame = allocateFrame(width, height);

    // store shifted pixels to temporary buffer
    for (int row = 0; row < (height - offset); row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            int position_buffer_frame = (row + offset) * width * 3 + column * 3;
            rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
            rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
            rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
        }
    }

    // fill left over pixels with white pixels
    for (int row = (height - offset); row < height; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            rendered_frame[position_rendered_frame] = 255;
            rendered_frame[position_rendered_frame + 1] = 255;
            rendered_frame[position_rendered_frame + 2] = 255;
        }
    }

    // copy the temporary buffer back to original frame buffer
    buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return buffer_frame;

}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image left
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveRight(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    
    // allocate memory for temporary image buffer
    unsigned char *rendered_frame = allocateFrame(width, height);

    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        for (int column = offset; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            int position_buffer_frame = row * width * 3 + (column - offset) * 3;
            rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
            rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
            rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
        }
    }

    // fill left over pixels with white pixels
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < offset; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            rendered_frame[position_rendered_frame] = 255;
            rendered_frame[position_rendered_frame + 1] = 255;
            rendered_frame[position_rendered_frame + 2] = 255;
        }
    }

    // copy the temporary buffer back to original frame buffer
    buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return buffer_frame;

}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveDown(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {

    // allocate memory for temporary image buffer
    unsigned char *rendered_frame = allocateFrame(width, height);

    // store shifted pixels to temporary buffer
    for (int row = offset; row < height; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            int position_buffer_frame = (row - offset) * width * 3 + column * 3;
            rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
            rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
            rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
        }
    }

    // fill left over pixels with white pixels
    for (int row = 0; row < offset; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            rendered_frame[position_rendered_frame] = 255;
            rendered_frame[position_rendered_frame + 1] = 255;
            rendered_frame[position_rendered_frame + 2] = 255;
        }
    }

    // copy the temporary buffer back to original frame buffer
    buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return buffer_frame;

}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image right
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveLeft(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {

    // allocate memory for temporary image buffer
    unsigned char *rendered_frame = allocateFrame(width, height);

    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < (width - offset); column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            int position_buffer_frame = row * width * 3 + (column + offset) * 3;
            rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
            rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
            rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
        }
    }

    // fill left over pixels with white pixels
    for (int row = 0; row < height; row++) {
        for (int column = width - offset; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            rendered_frame[position_rendered_frame] = 255;
            rendered_frame[position_rendered_frame + 1] = 255;
            rendered_frame[position_rendered_frame + 2] = 255;
        }
    }

    // copy the temporary buffer back to original frame buffer
    buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return buffer_frame;

}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
unsigned char *processRotateCW(unsigned char *buffer_frame, unsigned width, unsigned height, int rotate_iteration) {

    //Check for 360 degree or multiple
    if (rotate_iteration % 4 == 0) {
    	return buffer_frame;
    }

    //Check for 270 degree or multiple. If so, call CCW function
    if ((rotate_iteration - 3) % 4 == 0 ) {

	buffer_frame = processRotateCCW(buffer_frame, width, height, 1);
	return buffer_frame;
    }

    //Check for 180 degree or multiple
    if ((rotate_iteration - 2) % 4 == 0) {

	// allocate memory for temporary image buffer
    	unsigned char *rendered_frame = allocateFrame(width, height);

        int render_column = width - 1;
        int render_row = height - 1;

        for (int row = 0; row < width; row++) {
            for (int column = 0; column < height; column++) {
                int position_frame_buffer = row * width * 3 + column * 3;
                rendered_frame[render_row * width * 3 + render_column * 3] = buffer_frame[position_frame_buffer];
                rendered_frame[render_row * width * 3 + render_column * 3 + 1] = buffer_frame[position_frame_buffer + 1];
                rendered_frame[render_row * width * 3 + render_column * 3 + 2] = buffer_frame[position_frame_buffer + 2];
                render_column -= 1;
            }
            render_row -= 1;
            render_column = width - 1;
        }

        // copy the temporary buffer back to original frame buffer
        buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

	// free temporary image buffer
    	deallocateFrame(rendered_frame);

    	// return a pointer to the updated image buffer
    	return buffer_frame;
    }

    // allocate memory for temporary image buffer
    unsigned char *rendered_frame = allocateFrame(width, height);

    // store shifted pixels to temporary buffer
    for (int iteration = 0; iteration < rotate_iteration; iteration++) {
        int render_column = width-1;
        int render_row = 0;
	for (int column = 0; column < height; column++) {
        for (int row = 0; row < width; row++) 
             {
                int position_frame_buffer = row * width * 3 + column * 3;
                rendered_frame[render_row * width * 3 + render_column * 3] = buffer_frame[position_frame_buffer];
                rendered_frame[render_row * width * 3 + render_column * 3 + 1] = buffer_frame[position_frame_buffer + 1];
                rendered_frame[render_row * width * 3 + render_column * 3 + 2] = buffer_frame[position_frame_buffer + 2];
                render_column -= 1;
            }
            render_row += 1;
            render_column = width-1;
        }

        // copy the temporary buffer back to original frame buffer
        buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);
    }

    // free temporary image buffer
    deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return buffer_frame;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer counter clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
unsigned char *processRotateCCW(unsigned char *buffer_frame, unsigned width, unsigned height, int rotate_iteration) {

    //Check for 360 degree rotation
    if (rotate_iteration % 4 == 0) {
    	return buffer_frame;
    }

    //Check for 270 degree or multiple. If so, call CCW function
    if ((rotate_iteration - 3) % 4 == 0 ) {

	buffer_frame = processRotateCW(buffer_frame, width, height, 1);
	return buffer_frame;
    }

    //Check for 180 degree rotation, if so do the same as CW 180 degrees
    if ((rotate_iteration - 2) % 4 == 0) {

	// allocate memory for temporary image buffer
    	unsigned char *rendered_frame = allocateFrame(width, height);

        int render_column = width - 1;
        int render_row = height - 1;

        for (int row = 0; row < width; row++) {
            for (int column = 0; column < height; column++) {
                int position_frame_buffer = row * width * 3 + column * 3;
                rendered_frame[render_row * width * 3 + render_column * 3] = buffer_frame[position_frame_buffer];
                rendered_frame[render_row * width * 3 + render_column * 3 + 1] = buffer_frame[position_frame_buffer + 1];
                rendered_frame[render_row * width * 3 + render_column * 3 + 2] = buffer_frame[position_frame_buffer + 2];
                render_column -= 1;
            }
            render_row -= 1;
            render_column = width - 1;
        }

        // copy the temporary buffer back to original frame buffer
        buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

	// free temporary image buffer
    	deallocateFrame(rendered_frame);

    	// return a pointer to the updated image buffer
    	return buffer_frame;
    }

    // allocate memory for temporary image buffer
    unsigned char *rendered_frame = allocateFrame(width, height);

    // store shifted pixels to temporary buffer
    for (int iteration = 0; iteration < rotate_iteration; iteration++) {
        int render_column = 0;
        int render_row = height-1;
	for (int column = 0; column < height; column++) {
        for (int row = 0; row < width; row++) 
             {
                int position_frame_buffer = row * width * 3 + column * 3;
                rendered_frame[render_row * width * 3 + render_column * 3] = buffer_frame[position_frame_buffer];
                rendered_frame[render_row * width * 3 + render_column * 3 + 1] = buffer_frame[position_frame_buffer + 1];
                rendered_frame[render_row * width * 3 + render_column * 3 + 2] = buffer_frame[position_frame_buffer + 2];
                render_column += 1;
            }
            render_row -= 1;
            render_column = 0;
        }

        // copy the temporary buffer back to original frame buffer
        buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);
    }

    // free temporary image buffer
    deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return buffer_frame;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
unsigned char *processMirrorX(unsigned char *buffer_frame, unsigned int width, unsigned int height, int _unused) {

    // allocate memory for temporary image buffer
    unsigned char *rendered_frame = allocateFrame(width, height);

    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * height * 3 + column * 3;
            int position_buffer_frame = (height - row - 1) * height * 3 + column * 3;
            rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
            rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
            rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
        }
    }

    // copy the temporary buffer back to original frame buffer
    buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return buffer_frame;

}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
unsigned char *processMirrorY(unsigned char *buffer_frame, unsigned width, unsigned height, int _unused) {

    // allocate memory for temporary image buffer
    unsigned char *rendered_frame = allocateFrame(width, height);

    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * height * 3 + column * 3;
            int position_buffer_frame = row * height * 3 + (width - column - 1) * 3;
            rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
            rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
            rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
        }
    }

    // copy the temporary buffer back to original frame buffer
    buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return buffer_frame;
}

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          Do not forget to modify the team_name and team member information !!!
 **********************************************************************************************************************/
void print_team_info(){
    // Please modify this field with something interesting
    char team_name[] = "default-name";

    // Please fill in your information
    char student1_first_name[] = "john";
    char student1_last_name[] = "doe";
    char student1_student_number[] = "0000000000";

    // Please fill in your partner's information
    // If yon't have partner, do not modify this
    char student2_first_name[] = "joe";
    char student2_last_name[] = "doe";
    char student2_student_number[] = "0000000001";

    // Printing out team information
    printf("*******************************************************************************************************\n");
    printf("Team Information:\n");
    printf("\tteam_name: %s\n", team_name);
    printf("\tstudent1_first_name: %s\n", student1_first_name);
    printf("\tstudent1_last_name: %s\n", student1_last_name);
    printf("\tstudent1_student_number: %s\n", student1_student_number);
    printf("\tstudent2_first_name: %s\n", student2_first_name);
    printf("\tstudent2_last_name: %s\n", student2_last_name);
    printf("\tstudent2_student_number: %s\n", student2_student_number);
}

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          You can modify anything else in this file
 ***********************************************************************************************************************
 * @param sensor_values - structure stores parsed key value pairs of program instructions
 * @param sensor_values_count - number of valid sensor values parsed from sensor log file or commandline console
 * @param frame_buffer - pointer pointing to a buffer storing the imported  24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param grading_mode - turns off verification and turn on instrumentation
 ***********************************************************************************************************************
 *
 **********************************************************************************************************************/
void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
                           unsigned int width, unsigned int height, bool grading_mode) {
    int processed_frames = 0;
    
    char* sensorKey;
    int sensorValue;

    int sensorValueIdx;

    for (sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {
//        printf("Processing sensor value #%d: %s, %d\n", sensorValueIdx, sensor_values[sensorValueIdx].key,
//               sensor_values[sensorValueIdx].value);

	sensorKey = sensor_values[sensorValueIdx].key;
	sensorValue = sensor_values[sensorValueIdx].value;

        if (!strcmp(sensorKey, "W")) {
            frame_buffer = processMoveUp(frame_buffer, width, height, sensorValue);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensorKey, "A")) {
            frame_buffer = processMoveLeft(frame_buffer, width, height, sensorValue);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensorKey, "S")) {
            frame_buffer = processMoveDown(frame_buffer, width, height, sensorValue);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensorKey, "D")) {
            frame_buffer = processMoveRight(frame_buffer, width, height, sensorValue);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensorKey, "CW")) {
            frame_buffer = processRotateCW(frame_buffer, width, height, sensorValue);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensorKey, "CCW")) {
            frame_buffer = processRotateCCW(frame_buffer, width, height, sensorValue);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensorKey, "MX")) {
            frame_buffer = processMirrorX(frame_buffer, width, height, sensorValue);
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensorKey, "MY")) {
            frame_buffer = processMirrorY(frame_buffer, width, height, sensorValue);
//            printBMP(width, height, frame_buffer);
        }
        processed_frames += 1;
        if (processed_frames % 1 == 0) {
            verifyFrame(frame_buffer, width, height, grading_mode);
        }
    }

    return;
}

/*void updateNetModification(struct kv* netModification, struct kv* currentObject) {
	
	if (netModification->key == NULL) {
		netModification->key = currentObject->key;
		netModification->value = currentObject->value;
		return;
	}

	if(!strcmp(netModification->key, currentObject->key)) {
		netModification->value += currentObject->value;
	}
	else {
		netModification->value -= currentObject->value;

		if (netModification->value == 0) {
			netModification->key = NULL;
		}
		else if (netModification->value < 0)
		{
			netModification->key = currentObject->key;
			netModification->value = netModification->value * -1;
		}
	}
} 

void printCheck() {

	if (netModifications[vertical].key != NULL) {
		if (!strcmp(netModifications[vertical].key,"W")) {
			printf("Need to move up by %d\n",netModifications[vertical].value);
		}
		else if (!strcmp(netModifications[vertical].key,"S")) {
			printf("Need to move down by %d\n",netModifications[vertical].value);
		}
	}
            
	if (netModifications[horizontal].key != NULL) {
        	if (!strcmp(netModifications[horizontal].key,"A")) {
			printf("Need to move left by %d\n",netModifications[horizontal].value);
		}
		else if (!strcmp(netModifications[horizontal].key,"D")) {
			printf("Need to move right by %d\n",netModifications[horizontal].value);
		}
	}

	if (netModifications[rotation].key != NULL) {
        	if (!strcmp(netModifications[rotation].key,"CW")) {
			printf("Need to rotate CW by %d\n",netModifications[rotation].value);
		}
		else if (!strcmp(netModifications[rotation].key,"CCW")) {
			printf("Need to rotate CCW by %d\n",netModifications[rotation].value);
		}
	}
}

void implementation_driver(struct kv* sensor_values, int sensor_values_count, unsigned char * frame_buffer, unsigned int width, unsigned int height, bool grading_mode) {
	
	int processed_frames = 0;

	char* sensorKey;
	int sensorValue;
	int sensorValueIdx;

	for(sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {
		
		sensorKey = sensor_values[sensorValueIdx].key;
		sensorValue = sensor_values[sensorValueIdx].value;
		
		if (!strcmp(sensorKey, "W") || !strcmp(sensorKey, "S")) {
			
			updateNetModification(&netModifications[vertical], &(sensor_values[sensorValueIdx]));
			processed_frames += 1;
		}
		else if (!strcmp(sensorKey, "A") || !strcmp(sensorKey, "D")) {
			updateNetModification(&netModifications[horizontal], &(sensor_values[sensorValueIdx]));
			processed_frames += 1;
		}
		else if (!strcmp(sensorKey, "CW") || !strcmp(sensorKey, "CCW")) {
			updateNetModification(&netModifications[rotation], &(sensor_values[sensorValueIdx]));
			processed_frames += 1;
		}
		else if (!strcmp(sensorKey, "MX") || !strcmp(sensorKey, "MY")) {

			if (netModifications[vertical].key != NULL) {
printCheck();
printf("Vertical Check\n");
				if (!strcmp(netModifications[vertical].key, "W")) {
					frame_buffer = processMoveUp(frame_buffer, width, height, 						netModifications[vertical].value);
					netModifications[vertical].key = NULL;
					netModifications[vertical].value = 0;
				}
				else {
					frame_buffer = processMoveDown(frame_buffer, width, height, 						netModifications[vertical].value);
					netModifications[vertical].key = NULL;
					netModifications[vertical].value = 0;
				}

			}
			
			if (netModifications[horizontal].key != NULL) {
printCheck();
printf("Horizontal Check\n");
				if (!strcmp(netModifications[horizontal].key, "A")) {
					frame_buffer = processMoveLeft(frame_buffer, width, height, 						netModifications[horizontal].value);
					netModifications[horizontal].key = NULL;
					netModifications[horizontal].value = 0;
				}
				else {
					frame_buffer = processMoveRight(frame_buffer, width, height, 						netModifications[horizontal].value);
					netModifications[horizontal].key = NULL;
					netModifications[horizontal].value = 0;
				}

			}

			if (netModifications[rotation].key != NULL) {
printCheck();
printf("Rotation Check\n");
				if (!strcmp(netModifications[rotation].key, "CW")) {
					frame_buffer = processRotateCW(frame_buffer, width, height, 						netModifications[rotation].value);
					netModifications[rotation].key = NULL;
					netModifications[rotation].value = 0;
				}
				else {
					frame_buffer = processRotateCCW(frame_buffer, width, height, 						netModifications[rotation].value);
					netModifications[rotation].key = NULL;
					netModifications[rotation].value = 0;
				}

			}
			
			if (!strcmp(sensorKey, "MX")) {

				frame_buffer = processMirrorX(frame_buffer, width, height, sensorValue);
				processed_frames += 1;
			}
			else if (!strcmp(sensorKey, "MY")) {

				frame_buffer = processMirrorY(frame_buffer, width, height, sensorValue);
				processed_frames += 1;
			}

		}

		if (processed_frames % 5 == 0) {
			if (netModifications[vertical].key != NULL) {
printCheck();
printf("Vertical 2 Check\n");
		
				if (!strcmp(netModifications[vertical].key, "W")) {
					frame_buffer = processMoveUp(frame_buffer, width, height, 						netModifications[vertical].value);
					netModifications[vertical].key = NULL;
					netModifications[vertical].value = 0;
				}
				else {
					frame_buffer = processMoveDown(frame_buffer, width, height, 						netModifications[vertical].value);
					netModifications[vertical].key = NULL;
					netModifications[vertical].value = 0;
				}

			}
			
			if (netModifications[horizontal].key != NULL) {
printCheck();
printf("Horizontal 2 Check\n");
				if (!strcmp(netModifications[horizontal].key, "A")) {
					frame_buffer = processMoveLeft(frame_buffer, width, height, 						netModifications[horizontal].value);
					netModifications[horizontal].key = NULL;
					netModifications[horizontal].value = 0;
				}
				else {
					frame_buffer = processMoveRight(frame_buffer, width, height, 						netModifications[horizontal].value);
					netModifications[horizontal].key = NULL;
					netModifications[horizontal].value = 0;
				}

			}

			if (netModifications[rotation].key != NULL) {
printCheck();
printf("Rotation 2 Check\n");
				if (!strcmp(netModifications[rotation].key, "CW")) {
					frame_buffer = processRotateCW(frame_buffer, width, height, 						netModifications[rotation].value);
					netModifications[rotation].key = NULL;
					netModifications[rotation].value = 0;
				}
				else {
					frame_buffer = processRotateCCW(frame_buffer, width, height, 						netModifications[rotation].value);
					netModifications[rotation].key = NULL;
					netModifications[rotation].value = 0;
				}

			}

			verifyFrame(frame_buffer, width, height, grading_mode);
		}

	}

	return;
}*/
