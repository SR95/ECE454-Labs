#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line
#include "implementation.h"

struct kv netModifications[5];
struct kv simplifiedInstructions[25];

#define vertical 0
#define horizontal 1
#define rotation 2
#define mirrorx 3
#define mirrory 4

#define translation 5
#define mirror 6
#define nooperation 7

unsigned char* rendered_frame;
unsigned char* rendered_frame_sparse_matrix;
unsigned char* sparseMatrix;
	
int sparseMatrixParameters[12];
/*Element 0 is original topX (representing the top row bound)
  Element 1 is original bottomX (representing the bottom row bound)
  Element 2 is original leftY (representing the left column bound)
  Element 3 is original rightY (representing the right column bound)
  Element 4 is changing topX (initial value is same as original topX)
  Element 5 is changing bottomX (initial value is same as original bottomX)
  Element 6 is changing leftY (initial value is same as original leftY)
  Element 7 is changing rightY (initial value is same as original rightY)
  Element 8 is changing sparse matrix width
  Element 9 is changing sparse matrix height
  Element 10 is original sparse matrix width
  Element 11 is original sparse matrix height
*/

// Declariations
unsigned char *processMoveUp(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char *processMoveLeft(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char *processMoveDown(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char *processMoveRight(unsigned char *buffer_frame, unsigned width, unsigned height, int offset);
unsigned char *processRotateCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                                        int rotate_iteration);
unsigned char *processRotateCCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                                        int rotate_iteration);

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

    // handle negative offsets
    if (offset < 0){
        return processMoveDown(buffer_frame, width, height, offset * -1);
    }


    /****************** MEMCPY VERSION ********************/
    unsigned char *sourcePtr, *destPtr, *tempPtr;
    // store shifted pixels to temporary buffer
    //printf("buffer address: %p, %p\n", buffer_frame, tempPtr);
    for (int row = 0; row < (height - offset); row++)
    {
	    destPtr = buffer_frame + (row*width*3);
  	    sourcePtr = destPtr + (offset*width*3);
  	    memcpy(destPtr, sourcePtr, width*3);
    }

    // fill left over pixels with white pixels
    for (int row = (height - offset); row < height; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            buffer_frame[position_rendered_frame] = 255;
            buffer_frame[position_rendered_frame + 1] = 255;
            buffer_frame[position_rendered_frame + 2] = 255;
        }
    }

    // copy the temporary buffer back to original frame buffer
    //buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

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
     
    // handle negative offsets
    if (offset < 0){
        return processMoveLeft(buffer_frame, width, height, offset * -1);
    }

    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        for (int column = width - 1; column >= offset; column--) {
            int position_rendered_frame = row * width * 3 + column * 3;
            int position_buffer_frame = row * width * 3 + (column - offset) * 3;
            buffer_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
            buffer_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
            buffer_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
        }
    }

    // fill left over pixels with white pixels
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < offset; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            buffer_frame[position_rendered_frame] = 255;
            buffer_frame[position_rendered_frame + 1] = 255;
            buffer_frame[position_rendered_frame + 2] = 255;
        }
    }

    // copy the temporary buffer back to original frame buffer
    //buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

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

    // handle negative offsets
    if (offset < 0){
        return processMoveUp(buffer_frame, width, height, offset * -1);
    }

    unsigned char *sourcePtr, *destPtr, *tempPtr;
    
    for (int row = height - 1; row >= offset; row--) {

	    sourcePtr = buffer_frame + ((row - offset) * width * 3);
	    destPtr = buffer_frame + (row * width * 3);
	    memcpy(destPtr,sourcePtr, width*3);
    }
    
 
    /***************************************************** 
    // Older version: store shifted pixels to temporary buffer
    for (int row = height - 1; row >= offset; row--) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            int position_buffer_frame = (row - offset) * width * 3 + column * 3;
            buffer_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
            buffer_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
            buffer_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
        }
    }
    /*****************************************************/

    // fill left over pixels with white pixels
    for (int row = 0; row < offset; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            buffer_frame[position_rendered_frame] = 255;
            buffer_frame[position_rendered_frame + 1] = 255;
            buffer_frame[position_rendered_frame + 2] = 255;
        }
    }

    // copy the temporary buffer back to original frame buffer
    //buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

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

    // handle negative offsets
    if (offset < 0){
        return processMoveRight(buffer_frame, width, height, offset * -1);
    }

    // Implementing memcpy version, currently not working
    /*unsigned char *sourcePtr, *destPtr, *tempPtr;

    for (int row = 0; row < height; row++) {
	    //printf("Source Ptr: %p\n",sourcePtr);
	    //printf("Dest Ptr: %p\n",destPtr);
  	    sourcePtr = buffer_frame + (row * width * 3) + offset;
	    destPtr = buffer_frame + (row * width * 3);
            memcpy(destPtr,sourcePtr,((width - offset - 1) * 3));
       
    }*/


    // Original: store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < (width - offset); column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            int position_buffer_frame = row * width * 3 + (column + offset) * 3;
            buffer_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
            buffer_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
            buffer_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
        }
    }

    // fill left over pixels with white pixels
    for (int row = 0; row < height; row++) {
        for (int column = width - offset; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            buffer_frame[position_rendered_frame] = 255;
            buffer_frame[position_rendered_frame + 1] = 255;
            buffer_frame[position_rendered_frame + 2] = 255;
        }
    }

    // copy the temporary buffer back to original frame buffer
    //buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

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
	
    // handle negative offsets
    if (rotate_iteration < 0){
        return processRotateCCW(buffer_frame, width, height, rotate_iteration * -1);
    }

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

        int render_column = width - 1;
        int render_row = height - 1;

        for (int row = 0; row < height; row++) {
            for (int column = 0; column < width; column++) {
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

    	// return a pointer to the updated image buffer
    	return buffer_frame;
    }


    //This is done for the sparse matrix. If not using sparse matrix, still okay
    //because input image is considered to be square at all times
    int newWidth = height;
    int newHeight = width;

    // store shifted pixels to temporary buffer

        int render_column = height-1;
        int render_row = 0;
	for (int column = 0; column < width; column++) {
        for (int row = 0; row < height; row++) 
             {  //printf("(%d,%d) goes to (%d,%d)\n",row,column,render_row,render_column);
                int position_frame_buffer = row * width * 3 + column * 3;
                rendered_frame[render_row * newWidth * 3 + render_column * 3] = buffer_frame[position_frame_buffer];
                rendered_frame[render_row * newWidth * 3 + render_column * 3 + 1] = buffer_frame[position_frame_buffer + 1];
                rendered_frame[render_row * newWidth * 3 + render_column * 3 + 2] = buffer_frame[position_frame_buffer + 2];
                render_column -= 1;
            }
            render_row += 1;
            render_column = height-1;
        }

        // copy the temporary buffer back to original frame buffer
        buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);
	
    //Only do this for sparse matrix
    sparseMatrixParameters[8] = newWidth;
    sparseMatrixParameters[9] = newHeight;
    //printf("New Width: %d\nNew Height: %d\n", sparseMatrixParameters[8], sparseMatrixParameters[9]);

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

    if (rotate_iteration < 0){
        // handle negative offsets
        // rotating 90 degrees counter clockwise in opposite direction is equal to 90 degrees in cw direction
        return processRotateCW(buffer_frame, width, height, rotate_iteration * -1);
    } 

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

        int render_column = width - 1;
        int render_row = height - 1;

        for (int row = 0; row < height; row++) {
            for (int column = 0; column < width; column++) {
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

    	// return a pointer to the updated image buffer
    	return buffer_frame;
    }

    int newWidth = height;
    int newHeight = width;

    // store shifted pixels to temporary buffer
        int render_column = 0;
        int render_row = newHeight-1;
	for (int column = 0; column < width; column++) {
        for (int row = 0; row < height; row++) 
             {
		//printf("(%d,%d) goes to (%d,%d)\n",row,column,render_row,render_column);
                int position_frame_buffer = row * width * 3 + column * 3;
                rendered_frame[render_row * newWidth * 3 + render_column * 3] = buffer_frame[position_frame_buffer];
                rendered_frame[render_row * newWidth * 3 + render_column * 3 + 1] = buffer_frame[position_frame_buffer + 1];
                rendered_frame[render_row * newWidth * 3 + render_column * 3 + 2] = buffer_frame[position_frame_buffer + 2];
                render_column += 1;
            }
            render_row -= 1;
            render_column = 0;
        }

        // copy the temporary buffer back to original frame buffer
        buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    //Only do this for sparse matrix
    sparseMatrixParameters[8] = newWidth;
    sparseMatrixParameters[9] = newHeight;
    //printf("New Width: %d\nNew Height: %d\n", sparseMatrixParameters[8], sparseMatrixParameters[9]);

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

    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            int position_buffer_frame = (height - row - 1) * width * 3 + column * 3;
            rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
            rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
            rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
        }
    }

    // copy the temporary buffer back to original frame buffer
    buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

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

    // store shifted pixels to temporary buffer
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            int position_buffer_frame = row * width * 3 + (width - column - 1) * 3;
            rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
            rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
            rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
        }
    }

    // copy the temporary buffer back to original frame buffer
    buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

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

void updateNetModification(struct kv* netModification, struct kv* currentObject) {
	
	if (netModification->key == NULL) {

		if (currentObject-> value > 0) {
			netModification->key = currentObject->key;
			netModification->value = currentObject->value;
		}
		else {
			netModification->value = currentObject->value * -1;

			if (!strcmp(currentObject->key, "W"))
				netModification->key = "S";
			else if (!strcmp(currentObject->key, "S"))
				netModification->key = "W";
			else if (!strcmp(currentObject->key, "A"))
				netModification->key = "D";
			else if (!strcmp(currentObject->key, "D"))
				netModification->key = "A";
			else if (!strcmp(currentObject->key, "CW"))
				netModification->key = "CCW";
			else if (!strcmp(currentObject->key, "CCW"))
				netModification->key = "CW";
			else if (!strcmp(currentObject->key, "MX")) {
				netModification->key = "MX";
				netModification->value = 1;			
			}
			else if (!strcmp(currentObject->key, "MY")) {
				netModification->key = "MY";
				netModification->value = 1;			
			}
		}
		
		return;
	}

	if(!strcmp(netModification->key, currentObject->key)) {
		
		//Handle Mirror separately		
		if(!strcmp(netModification->key, "MX") || !strcmp(netModification->key, "MY")) {
			if (netModification->value == 1) {
				netModification->value = 0;
				netModification->key = NULL;
			}			

			return;
		}

		netModification->value += currentObject->value;
	}
	else {	
		//All other cases
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

	if (netModifications[mirrorx].key != NULL) {
		printf("Need to mirror X by %d\n",netModifications[mirrorx].value);
	}

	if (netModifications[mirrory].key != NULL) {
		printf("Need to mirror Y by %d\n",netModifications[mirrorx].value);
	}

}

void findSparseMatrix (unsigned char * frame_buffer, int width, int height)
{
	int row, column;

	int topX = height - 1;
	int bottomX = 0;
	int leftY = width - 1;
	int rightY = 0;

	int R,G,B;

	int frame_buffer_index;

	for (row = 0; row < width; row++)
		for (column = 0; column < height; column++) {
	
			frame_buffer_index = (row * width * 3) + (column * 3);
			R = frame_buffer[frame_buffer_index];
			G = frame_buffer[frame_buffer_index + 1];
			B = frame_buffer[frame_buffer_index + 2];
			
			if (R != 255 || G != 255 || B != 255) {
				if (row < topX)
					topX = row;
				if (row > bottomX)
					bottomX = row;
				if (column < leftY)
					leftY = column;
				if (column > rightY)
					rightY = column;
			} 

	}

	//Alloc sparseMatrix and fill it up with values
	int sparseWidth, sparseHeight; 
	sparseWidth = rightY - leftY + 1;
	sparseHeight = bottomX - topX + 1;
	
	sparseMatrixParameters[0] = topX;
	sparseMatrixParameters[1] = bottomX;
	sparseMatrixParameters[2] = leftY;
	sparseMatrixParameters[3] = rightY;
	sparseMatrixParameters[4] = topX;
	sparseMatrixParameters[5] = bottomX;
	sparseMatrixParameters[6] = leftY;
	sparseMatrixParameters[7] = rightY;
	sparseMatrixParameters[8] = sparseWidth;
	sparseMatrixParameters[9] = sparseHeight;
	sparseMatrixParameters[10] = sparseWidth;
	sparseMatrixParameters[11] = sparseHeight;

	//Don't forget to deallocate after use!
	sparseMatrix = allocateFrame(sparseWidth, sparseHeight);

	for (row = topX; row <= bottomX; row++) {
		for (column = leftY; column <= rightY; column++) {

			int sourceIndex = row * width * 3 + column * 3;
			int destIndex = ((row - topX) * sparseWidth * 3) + ((column - leftY) * 3);

			sparseMatrix[destIndex] = frame_buffer[sourceIndex];
			sparseMatrix[destIndex + 1] = frame_buffer[sourceIndex + 1];
			sparseMatrix[destIndex + 2] = frame_buffer[sourceIndex + 2];
		}
	}

}

unsigned char* drawSparseMatrixInImage (unsigned char* frame_buffer, int width, int height) {

	/*Done in two for loops as of now to deal with the event in which
	  the original and new sparseMatrix locations overlap with each other
	  This can be optimized later
	*/

	int originalTopX = sparseMatrixParameters[0];
	int originalBottomX = sparseMatrixParameters[1];
	int originalLeftY = sparseMatrixParameters[2];
	int originalRightY = sparseMatrixParameters[3];
	int newTopX = sparseMatrixParameters[4];
	int newBottomX = sparseMatrixParameters[5];
	int newLeftY = sparseMatrixParameters[6];
	int newRightY = sparseMatrixParameters[7];	
	int newSparseWidth = sparseMatrixParameters[8];
	int newSparseHeight = sparseMatrixParameters[9];
	int originalSparseWidth = sparseMatrixParameters[10];
	int originalSparseHeight = sparseMatrixParameters[11];

	unsigned char *sparseMatrixPtr, *drawPixelPtr;

	for (int row = 0; row < originalSparseHeight; row++) {
		drawPixelPtr = frame_buffer + ((row + originalTopX) * width * 3) + (originalLeftY * 3);
		memset(drawPixelPtr, 255, originalSparseWidth*3);
	}

	for (int row = 0; row < newSparseHeight; row++) {

		sparseMatrixPtr = sparseMatrix + (row * newSparseWidth * 3);
		drawPixelPtr = frame_buffer + ((row + newTopX) * width * 3) + (newLeftY * 3);
		memcpy(drawPixelPtr, sparseMatrixPtr, newSparseWidth*3);
	}

	return frame_buffer;
	
}

void updateSparseMatrixParameters(int operationType, char* operation, int magnitude, int width, int height) {

	if (operationType == translation) {

		if (!strcmp(operation, "W")) {
			sparseMatrixParameters[4] -= magnitude;
			sparseMatrixParameters[5] -= magnitude;
		}
		else if (!strcmp(operation, "S")) {
			sparseMatrixParameters[4] += magnitude;
			sparseMatrixParameters[5] += magnitude;
		}
		else if (!strcmp(operation, "A")) {
			sparseMatrixParameters[6] -= magnitude;
			sparseMatrixParameters[7] -= magnitude;
		}
		else if (!strcmp(operation, "D")) {
			sparseMatrixParameters[6] += magnitude;
			sparseMatrixParameters[7] += magnitude;
		}		

	}
	else if (operationType == rotation) {

		int topX = sparseMatrixParameters[4];
		int bottomX = sparseMatrixParameters[5];
		int leftY = sparseMatrixParameters[6];
		int rightY = sparseMatrixParameters[7];

		if ( (!strcmp(operation, "CW") && ((magnitude - 1) % 4 == 0))
			|| (!strcmp(operation, "CCW") && ((magnitude - 3) % 4 == 0))) {

			//Rotation CW 90 degrees or CCW 270 degrees
			sparseMatrixParameters[4] = leftY;
			sparseMatrixParameters[5] = rightY;
			sparseMatrixParameters[6] = width - 1 - bottomX;
			sparseMatrixParameters[7] = width - 1 - topX;
		}
		else if ( (!strcmp(operation, "CW") && ((magnitude - 2) % 4 == 0))
			|| (!strcmp(operation, "CCW") && ((magnitude - 2) % 4 == 0))) {
			
			//Rotation CW 180 degrees or CCW 180
			
			sparseMatrixParameters[4] = height - 1 - bottomX;
			sparseMatrixParameters[5] = height - 1 - topX;
			sparseMatrixParameters[6] = width - 1 - rightY;
			sparseMatrixParameters[7] = width - 1 - leftY;
		}
		else if ( (!strcmp(operation, "CW") && ((magnitude - 3) % 4 == 0))
			|| (!strcmp(operation, "CCW") && ((magnitude - 1) % 4 == 0))) {
			
			//Rotation CW 270 degrees or CCW 90
			
			sparseMatrixParameters[4] = height - 1 - rightY;
			sparseMatrixParameters[5] = height - 1 - leftY;
			sparseMatrixParameters[6] = topX;
			sparseMatrixParameters[7] = bottomX;
		}
		
	} //end of rotation
	else if (operationType == mirror) {

		int topX = sparseMatrixParameters[4];
		int bottomX = sparseMatrixParameters[5];
		int leftY = sparseMatrixParameters[6];
		int rightY = sparseMatrixParameters[7];
		
		if (!strcmp(operation, "MX")) {
			sparseMatrixParameters[4] = height - 1 - bottomX;
			sparseMatrixParameters[5] = height - 1 - topX;
		}
		else if (!strcmp(operation, "MY")) {
			sparseMatrixParameters[6] = width - 1 - rightY;
			sparseMatrixParameters[7] = width - 1 - leftY;
		}

	}

}

void implementation_driver(struct kv* sensor_values, int sensor_values_count, unsigned char * frame_buffer, unsigned int width, unsigned int height, bool grading_mode) {
	
	rendered_frame = allocateFrame(width, height);

	int processed_frames = 0;

	char* sensorKey;
	int sensorValue;
	int sensorValueIdx;

	int lastOperation, currentOperation;

	findSparseMatrix(frame_buffer, width, height);

	netModifications[vertical].key = NULL;
	netModifications[horizontal].key = NULL;
	netModifications[rotation].key = NULL;
	netModifications[mirrorx].key = NULL;
	netModifications[mirrory].key = NULL;

	netModifications[vertical].value = 0;
	netModifications[horizontal].value = 0;
	netModifications[rotation].value = 0;
	netModifications[mirrorx].value = 0;
	netModifications[mirrory].value = 0;

	//Ignoring the last few instructions if not an exact multiple of 25
	if (sensor_values_count / 25 > 1) {
		sensor_values_count -= sensor_values_count % 25;
	}

	for(sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {

		if(sensorValueIdx == 0)
			lastOperation = nooperation;

		sensorKey = sensor_values[sensorValueIdx].key;
		sensorValue = sensor_values[sensorValueIdx].value;	

		if (!strcmp(sensorKey, "W") || !strcmp(sensorKey, "S")) {
			currentOperation = translation;
			
			if(lastOperation != nooperation && currentOperation != lastOperation) {

				//Perform rotation if any
				if (netModifications[rotation].key != NULL) {
					updateSparseMatrixParameters(rotation, netModifications[rotation].key, netModifications[rotation].value, width, height);		
	
					if (!strcmp(netModifications[rotation].key, "CW")) {
						sparseMatrix = processRotateCW(sparseMatrix, sparseMatrixParameters[8],sparseMatrixParameters[9],netModifications[rotation].value);
					}
					else {
						sparseMatrix = processRotateCCW(sparseMatrix, sparseMatrixParameters[8],sparseMatrixParameters[9],netModifications[rotation].value);
					}
		
					netModifications[rotation].key = NULL;
					netModifications[rotation].value = 0;

				}

				//Perform mirrorx if any
				if (netModifications[mirrorx].key != NULL) {
					updateSparseMatrixParameters(mirror, netModifications[mirrorx].key, netModifications[mirrorx].value, width, height);
					sparseMatrix = processMirrorX(sparseMatrix, sparseMatrixParameters[8], sparseMatrixParameters[9], netModifications[mirrorx].value);
					netModifications[mirrorx].key = NULL;
					netModifications[mirrorx].value = 0;
				}

				//Perform mirrory if any
				if (netModifications[mirrory].key != NULL) {
					updateSparseMatrixParameters(mirror, netModifications[mirrory].key, netModifications[mirrory].value, width, height);
					sparseMatrix = processMirrorY(sparseMatrix, sparseMatrixParameters[8], sparseMatrixParameters[9], netModifications[mirrory].value);
					netModifications[mirrory].key = NULL;
					netModifications[mirrory].value = 0;
					
				}

			}

			updateNetModification(&netModifications[vertical], &(sensor_values[sensorValueIdx]));
			processed_frames += 1;
			lastOperation = currentOperation;

		}
		else if (!strcmp(sensorKey, "A") || !strcmp(sensorKey, "D")) {
			currentOperation = translation;

			if(lastOperation != nooperation && currentOperation != lastOperation) {

				//Perform rotation if any
				if (netModifications[rotation].key != NULL) {

					updateSparseMatrixParameters(rotation, netModifications[rotation].key, netModifications[rotation].value, width, height);

					if (!strcmp(netModifications[rotation].key, "CW")) {
						sparseMatrix = processRotateCW(sparseMatrix, sparseMatrixParameters[8],sparseMatrixParameters[9],netModifications[rotation].value);
					}
					else {
						sparseMatrix = processRotateCCW(sparseMatrix, sparseMatrixParameters[8],sparseMatrixParameters[9],netModifications[rotation].value);
					}

					netModifications[rotation].key = NULL;
					netModifications[rotation].value = 0;
				}

				//Perform mirrorx if any
				if (netModifications[mirrorx].key != NULL) {
					updateSparseMatrixParameters(mirror, netModifications[mirrorx].key, netModifications[mirrorx].value, width, height);
					sparseMatrix = processMirrorX(sparseMatrix, sparseMatrixParameters[8], sparseMatrixParameters[9], netModifications[mirrorx].value);
					netModifications[mirrorx].key = NULL;
					netModifications[mirrorx].value = 0;
				}

				//Perform mirrory if any
				if (netModifications[mirrory].key != NULL) {
					updateSparseMatrixParameters(mirror, netModifications[mirrory].key, netModifications[mirrory].value, width, height);
					sparseMatrix = processMirrorY(sparseMatrix, sparseMatrixParameters[8], sparseMatrixParameters[9], netModifications[mirrory].value);
					netModifications[mirrory].key = NULL;
					netModifications[mirrory].value = 0;
					
				}

			}

			updateNetModification(&netModifications[horizontal], &(sensor_values[sensorValueIdx]));
			processed_frames += 1;
			lastOperation = currentOperation;

		}
		else if (!strcmp(sensorKey, "CW") || !strcmp(sensorKey, "CCW")) {
			currentOperation = rotation;

			if(lastOperation != nooperation && currentOperation != lastOperation) {
				//Perform translation if any
				if (netModifications[vertical].key != NULL) {
					updateSparseMatrixParameters(translation, netModifications[vertical].key, netModifications[vertical].value, width, height);
					netModifications[vertical].key = NULL;
					netModifications[vertical].value = 0;
				}
			
			        if (netModifications[horizontal].key != NULL) {
					updateSparseMatrixParameters(translation, netModifications[horizontal].key, netModifications[horizontal].value, width, height);
					netModifications[horizontal].key = NULL;
					netModifications[horizontal].value = 0;
				}

				//Perform mirrorx if any
				if (netModifications[mirrorx].key != NULL) {
					updateSparseMatrixParameters(mirror, netModifications[mirrorx].key, netModifications[mirrorx].value, width, height);
					sparseMatrix = processMirrorX(sparseMatrix, sparseMatrixParameters[8], sparseMatrixParameters[9], netModifications[mirrorx].value);
					netModifications[mirrorx].key = NULL;
					netModifications[mirrorx].value = 0;
					
				}

				//Perform mirrory if any
				if (netModifications[mirrory].key != NULL) {
					updateSparseMatrixParameters(mirror, netModifications[mirrory].key, netModifications[mirrory].value, width, height);
					sparseMatrix = processMirrorY(sparseMatrix, sparseMatrixParameters[8], sparseMatrixParameters[9], netModifications[mirrory].value);
					netModifications[mirrory].key = NULL;
					netModifications[mirrory].value = 0;
					
				}

			}

			updateNetModification(&netModifications[rotation], &(sensor_values[sensorValueIdx]));
			processed_frames += 1;
			lastOperation = currentOperation;

		}
		else if (!strcmp(sensorKey, "MX") || !strcmp(sensorKey, "MY")) {
			currentOperation = mirror;

			if(lastOperation != nooperation && currentOperation != lastOperation) {

				//Perform translation if any
				if (netModifications[vertical].key != NULL) {
					updateSparseMatrixParameters(translation, netModifications[vertical].key, netModifications[vertical].value, width, height);
					netModifications[vertical].key = NULL;
					netModifications[vertical].value = 0;
				}
			
			        if (netModifications[horizontal].key != NULL) {
					updateSparseMatrixParameters(translation, netModifications[horizontal].key, netModifications[horizontal].value, width, height);
					netModifications[horizontal].key = NULL;
					netModifications[horizontal].value = 0;
				}

				//Perform rotation if any
				if (netModifications[rotation].key != NULL) {

					updateSparseMatrixParameters(rotation, netModifications[rotation].key, netModifications[rotation].value, width, height);

					if (!strcmp(netModifications[rotation].key, "CW")) {
						sparseMatrix = processRotateCW(sparseMatrix, sparseMatrixParameters[8],sparseMatrixParameters[9],netModifications[rotation].value);	
					}
					else {
						sparseMatrix = processRotateCCW(sparseMatrix, sparseMatrixParameters[8],sparseMatrixParameters[9],netModifications[rotation].value);
					}
					netModifications[rotation].key = NULL;
					netModifications[rotation].value = 0;
				}

			}

			if (!strcmp(sensorKey, "MX")) {
				updateNetModification(&netModifications[mirrorx], &(sensor_values[sensorValueIdx]));
				
			}
			else if (!strcmp(sensorKey, "MY")) {
				updateNetModification(&netModifications[mirrory], &(sensor_values[sensorValueIdx]));
			}
			
			processed_frames += 1;
			lastOperation = currentOperation;

		}

		if (processed_frames % 25 == 0) {
			//Perform translation if any
			if (netModifications[vertical].key != NULL) {
					updateSparseMatrixParameters(translation, netModifications[vertical].key, netModifications[vertical].value, width, height);
					netModifications[vertical].key = NULL;
					netModifications[vertical].value = 0;
			}
			
			if (netModifications[horizontal].key != NULL) {
					updateSparseMatrixParameters(translation, netModifications[horizontal].key, netModifications[horizontal].value, width, height);
					netModifications[horizontal].key = NULL;
					netModifications[horizontal].value = 0;
			}

			//Perform rotation if any
			if (netModifications[rotation].key != NULL) {
			
				updateSparseMatrixParameters(rotation, netModifications[rotation].key, netModifications[rotation].value, width, height);

				if (!strcmp(netModifications[rotation].key, "CW")) {
					sparseMatrix = processRotateCW(sparseMatrix, sparseMatrixParameters[8],sparseMatrixParameters[9],netModifications[rotation].value);
				}
				else {
					sparseMatrix = processRotateCCW(sparseMatrix, sparseMatrixParameters[8],sparseMatrixParameters[9],netModifications[rotation].value);
				}

				netModifications[rotation].key = NULL;
				netModifications[rotation].value = 0;

			}

			//Perform mirrorx if any
			if (netModifications[mirrorx].key != NULL) {
				updateSparseMatrixParameters(mirror, netModifications[mirrorx].key, netModifications[mirrorx].value, width, height);
					sparseMatrix = processMirrorX(sparseMatrix, sparseMatrixParameters[8], sparseMatrixParameters[9], netModifications[mirrorx].value);
				netModifications[mirrorx].key = NULL;
				netModifications[mirrorx].value = 0;
					
			}

			//Perform mirrory if any
			if (netModifications[mirrory].key != NULL) {
				updateSparseMatrixParameters(mirror, netModifications[mirrory].key, netModifications[mirrory].value, width, height);
					sparseMatrix = processMirrorY(sparseMatrix, sparseMatrixParameters[8], sparseMatrixParameters[9], netModifications[mirrory].value);
				netModifications[mirrory].key = NULL;
				netModifications[mirrory].value = 0;
					
			}
			
			frame_buffer = drawSparseMatrixInImage (frame_buffer, width, height);

			sparseMatrixParameters[0] = sparseMatrixParameters[4];
			sparseMatrixParameters[1] = sparseMatrixParameters[5];
			sparseMatrixParameters[2] = sparseMatrixParameters[6];
			sparseMatrixParameters[3] = sparseMatrixParameters[7];
			sparseMatrixParameters[10] = sparseMatrixParameters[8];
			sparseMatrixParameters[11] = sparseMatrixParameters[9];

			verifyFrame(frame_buffer, width, height, grading_mode);
		}

	}

	deallocateFrame(rendered_frame);
	deallocateFrame(sparseMatrix);

	return;
}
