//
// Created by dwj on 9/11/18.
//

#include "interface.h"


using std::vector;
using std::endl;
using std::string;

METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
    my_error_ptr myerr = (my_error_ptr)cinfo->err;
    (*cinfo->err->output_message) (cinfo);
    longjmp(myerr->setjmp_buffer, 1);
}


GLOBAL(int) get_original_data(string const & input_name, vector<int> & original_data , unsigned long & height, unsigned long & width,
                              unsigned short & depth)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;
    FILE * infile;

    if ((infile = fopen(input_name.c_str(), "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", input_name.c_str());
        return 0;
    }
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return 0;
    }
    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, infile);

    //Get image information
    (void)jpeg_read_header(&cinfo, TRUE);



    (void)jpeg_start_decompress(&cinfo);




    width = cinfo.output_width; //width of the image
    height = cinfo.output_height; //height of the image
    depth = cinfo.output_components; //depth of the image

    unsigned long line_stride = width*depth;//line stride of the image
    unsigned long size = line_stride*height; //height of the image




    original_data.resize(size,0);


    JSAMPARRAY buffer;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, width*depth, 1);

    int offset = 0;
    while(cinfo.output_scanline < height)
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        int i=0;

        //Out put the image channel by channel
        while (i<width*depth)
        {
            original_data[i+offset]=buffer[0][i];
            i++;
        }

        offset += line_stride;
    }




    (void)jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return 1;
}