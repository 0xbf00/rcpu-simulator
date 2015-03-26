/*!
    @header Program loading from disk.
    This header provides two functions that can be used to load a program
    from a user-supplied FILE pointer, or from a path to a file on disk
    
    @language c
    @author Jakob Rieck
*/
#ifndef _PROGRAM_LOADING_H
#define _PROGRAM_LOADING_H

#include <stdio.h>
#include <stdint.h>

/*!
    @abstract Options for program loading
    @discussion
        Two options are available: One for binary loading,
        which refers to binaries consisting of machine instructions
        only, and one for programs distributed in textual form,
        where each line conforms to the grammar specified in the
        functions below.
*/
typedef enum {
    OPT_BINARY = 0,
    OPT_TEXTUAL
} LOAD_OPTION;

/*!
    @abstract
        Loads a program from a user supplied file that has not been opened.

    @see load_program_from_file
*/
int load_program_from_path(const LOAD_OPTION option, const char * input_file,
                           uint32_t ** code_out, uint32_t * size_out);

/*!
    @abstract
        Loads a program from a user supplied file.

    @discussion
        The file is assumed to be readable (not necessarily writable).
        There are two possible program formats: Either it is a binary file
        containing just the instructions encoded as little endian integers,
        or the program is supplied as a text file of the following format

        tag    binary_vector    description

        This input gets transformed and the program is returned.

        The actual output is returned in code_out and size_out.
        An error code is returned from the procedure. (0 on success)

    @param option
        One of the two options: binary or textual.
    @param input
        The input file.
    @param code_out
        Output parameter for the code image. 
        The caller is responsible to release the memory returned in this parameter.
    @param size_out
        Output parameter for the size of the code image.

    @return
        An error code (0 on success)
*/
int load_program_from_file(const LOAD_OPTION option, FILE * input, 
                           uint32_t ** code_out, uint32_t * size_out);

#endif // _PROGRAM_LOADING_H
