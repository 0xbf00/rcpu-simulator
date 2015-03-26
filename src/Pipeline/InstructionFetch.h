/*!
    @header Instruction Fetch (IF)
    The instruction fetch stage is responsible for loading
    the next instruction and to increment the program counter.

    @language c
    @author Jakob Rieck
*/
#ifndef _INSTRUCTION_FETCH_H
#define _INSTRUCTION_FETCH_H

#include "Pipeline.h"

/*! 
    @abstract
        Type for result of Instruction Fetch stage.
*/
typedef struct if_result {
    uint32_t n_pc;
    uint32_t inst;
} if_result_t;

/*!
    @abstract
        Execute the first stage of the pipeline.
    
    @return
        Returns a pointer to a structure containing the loaded instruction
        and the new PC, or NULL, if the loaded instruction was HALT.
*/
if_result_t * instruction_fetch();

#endif // _INSTRUCTION_FETCH_H