//
// Created by dwj on 9/10/18.
//

#ifndef IMAGE_CONVERTER_INTERFACE_H
#define IMAGE_CONVERTER_INTERFACE_H

#include <iostream>
#include <jpeglib.h>
#include <csetjmp>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>


struct my_error_mgr {
    struct jpeg_error_mgr pub;  /* "public" fields */

    jmp_buf setjmp_buffer;  /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void) my_error_exit(j_common_ptr cinfo);


GLOBAL(int) get_original_data(std::string const & input_name, std::vector<int> & original_data , unsigned long & height, unsigned long & width,
                              unsigned short & depth);


#endif //IMAGE_CONVERTER_INTERFACE_H
