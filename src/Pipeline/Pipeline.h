/*!
    @header Processor pipeline
    The processor pipeline consists of five stages:
        - Instruction Fetch
        - Instruction Decode
        - Execute
        - Memory Access
        - Write Back
    Each stage is described in more detail in their respective
    header file.

    @language c
    @author Jakob Rieck
*/
#ifndef _PIPELINE_H
#define _PIPELINE_H

#include <sys/types.h>

#include "../Instruction/Decode.h"
#include "../Instruction/Opcodes.h"

/*!
    @abstract
        Type of the memory image of the processor
    @discussion
        Data and code are separated to allow for
        pipelining. There is no way to load or otherwise
        access code as data, so there is no way to write
        self-modifying code.
*/
typedef struct memory_image {
    uint32_t * code;
    size_t     code_size;
    uint32_t * data;
    size_t     data_size;
} memory_image_t;

extern memory_image_t memory;
extern uint32_t registers[32];

#include "InstructionFetch.h"
#include "InstructionDecode.h"
#include "Execute.h"
#include "MemoryAccess.h"
#include "WriteBack.h"

#endif // _PIPELINE_H