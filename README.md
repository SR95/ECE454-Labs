# ECE454-Labs

Things changed so far: 
1. Copied all function definitions of operations (W,A,S,D,CW,CCW,MX,MY) from implementation_reference.c into implementation.c
2. Optimized CW and CCW in implementation.c file. Changed to column wise reading and row wise writing. Also took care of cases for multiples of 180, 270 and 360 degrees. For 180 degrees, don't call 90 degrees twice, instead do it in one go. For 270 degrees clockwise, just call anti clockwise with 90 degrees and vice versa. For 360 degrees, don't do anything.
3. Rohan currently working on combining operations together. Done at the end of implementation.c under 3 functions namely updateNetModification, printCheck (for debugging) and implementation_driver (new version). 

Additional Notes:
verifyFrame is currently called after every instruction i.e (processed_frames % 1 == 0) in implementation.c and implementation_reference.c 

Task for Sagnik:
From Jack's slides "Don't allocate temporary buffer, then free it every time. Reuse buffers to achieve higher performance".
If you look at the code for the operations performed, they all alloc and dealloc a new temporary buffer. Maybe create one global (or local) buffer and change that in each of those functions (remember to dealloc it at the end though).
