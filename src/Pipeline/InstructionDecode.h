/*!
    @header Instruction Decode (ID)
    The instruction decode stage is responsible for decoding
    the operands- and result register of the specified instruction.
    Furthermore, the actual contents are fetched from the registry bank.

    @language c
    @author Jakob Rieck
*/
#ifndef _INSTRUCTION_DECODE_H
#define _INSTRUCTION_DECODE_H

#include "Pipeline.h"

/*! 
    @abstract
        Type for result of Instruction Decode stage.
*/
typedef struct id_result {
    uint32_t n_pc;
    uint32_t inst;

    uint32_t op1; // Contents of first register parameter
    uint32_t op2; // Can either be register contents or an immediate value
    
    // Register index in the case of inst == LOAD
    // else the value to store
    uint32_t io_op;
} id_result_t;

// Forward declarations
struct if_result;
typedef struct if_result if_result_t;

/*!
    @abstract
        Execute the second stage of the pipeline.

    @param in
        The output of the previous pipeline stage.
        The memory for this input is released upon completion.

    @result
        Returns a pointer to a structure containing the results
        of this stage. If the input was NULL, the output is also
        NULL and the function returns immediately.
*/
id_result_t * instruction_decode(const if_result_t * const in);

#endif // _INSTRUCTION_DECODE_H
