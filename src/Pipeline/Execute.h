/*!
    @header Execute stage (EX)
    Being the third stage of the pipeline, the purpose
    of this stage is to actually compute the results of
    arithmetic instructions, compute addresses of store
    and load instructions and re-calculate the flag, iff
    necessary.

    @language c
    @author Jakob Rieck
*/
#ifndef _EXECUTE_H
#define _EXECUTE_H

#include "Pipeline.h"

/*!
    @abstract
        Type for result of execute pipeline stage.
*/
typedef struct ex_result {
    uint32_t n_pc;
    uint32_t inst;

    // 1 iff inst is branch and branch is taken, 
    // otherwise 0.
    uint32_t branch_taken;

    // address for LDR / STR operations or 
    // result of arithmetic or logical instruction
    uint32_t result;

    // See description in id_result_t
    uint32_t io_op;
} ex_result_t;

// Forward declarations
struct id_result;
typedef struct id_result id_result_t;

/*!
    @abstract
        Execute the third stage of the pipeline.

    @param in
        The result of the previous stage.
        This parameter can be NULL. In that case,
        no calculation is performed and NULL is
        returned.

    @return
        A pointer to a structure containing the
        results of this stage or NULL, if no
        input was available.
*/
ex_result_t * execute(const id_result_t * const in);

#endif // _EXECUTE_H
