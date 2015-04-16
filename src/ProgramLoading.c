#include "ProgramLoading.h"

#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>

#include <assert.h>

#ifdef __linux
#include <malloc.h>
#endif

/*
    Returns the filesize of the file fp,
    or -1 if an error occurred.
*/
static size_t file_getsize(FILE *fp) {
    struct stat s;
    int ret = fstat(fileno(fp), &s);

    assert((ret != -1) && s.st_size >= 0);
    return (size_t) s.st_size;
}

static int isbinary(int c)
{
    return c == '0' || c == '1';
}

/*
    Reads one instruction from a file that contains
    a textual representation of the input.
*/
static uint32_t read_instruction_from_file(FILE * input)
{
    char currentChar;
    uint32_t result = 0;

    // Skip the first number
    while (isdigit(currentChar = fgetc(input)))
        ;
    ungetc(currentChar, stdin);

    // Skip whitespace between number and bit vector
    while (isspace(currentChar = fgetc(input)))
        ;
    ungetc(currentChar, stdin);

    while (isbinary(currentChar = fgetc(input))) {
        result = (result << 1) + (currentChar - '0');
    }

    while (('\n' != (currentChar = fgetc(input))) 
        && (EOF  !=  currentChar));

    return result;
}

int load_program_from_path(const LOAD_OPTION option, const char * input_file,
                           uint32_t ** code_out, size_t * size_out)
{
    FILE *fp = fopen(input_file, "rb");
    if (fp == NULL)
        return 1;

    const int retVal = load_program_from_file(option, fp, code_out, size_out);

    fclose(fp);
    return retVal;
}

int load_program_from_file(const LOAD_OPTION option, FILE * input, 
                           uint32_t ** code_out, size_t * size_out)
{
    if (option == OPT_BINARY) {
        const size_t filesize = file_getsize(input);

        // Our program needs to be aligned to a 4 byte boundary
        if (filesize % sizeof(uint32_t) != 0)
            return 1;

        uint32_t * code = malloc(filesize);
        if (code == NULL)
            return 1;

        const size_t readCount = fread(code, 1, filesize, input);
        if (readCount != filesize)
            return 1;

        *size_out = filesize;
        *code_out = code;

        return 0;
    } else { // OPTION == OPT_TEXTUAL
        uint32_t cinst = 0;
        uint32_t ninst = 1000;

        uint32_t * code = malloc(ninst * sizeof(uint32_t));
        if (code == NULL)
            return 1;

        while (!feof(input)) {
            // Enlarge input buffer, iff necessary
            if (cinst == ninst) {
                code = realloc(code, ninst * 2 * sizeof(uint32_t));
                if (code == NULL)
                    return 1;
                ninst *= 2;
            }

            // Read one instruction line and store it into the buffer
            const uint32_t inst = read_instruction_from_file(input);
            code[cinst] = inst;
            cinst++;
        }

        // Resize buffer, because we may not need all the memory we allocated
        code = realloc(code, cinst * sizeof(uint32_t));
        if (code == NULL)
            return 1;

        *code_out = code;
        *size_out = cinst;

        return 0;
    }
}
