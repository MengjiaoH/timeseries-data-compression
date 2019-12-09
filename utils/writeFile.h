#pragma once 
#include <iostream>
#include <vector>
#include <fstream>

#include "ospcommon/math/vec.h"

#include "zfp.h"

static int compress_3(float* array, int nx, int ny, int nz, float tolerance, std::string file);
static int compress_4(float* array, float tolerance, std::string file);

void writeRaw_4(std::vector<std::vector<float>> volumes, std::string fileDir, ospcommon::math::vec3i dims)
{
    //std::cout << "volume size = "  << volumes.size() << std::endl;
    //std::cout << "debug 0 in writeRaw 4 " << std::endl;
    int bx = dims.x / 4;
    int by = dims.y / 4;
    int bz = dims.z / 4;

    std::vector<float> sub_volume(4 * 4 * 4 * 4, 0);

    for(int z = 0; z < bz; z++){
        int z_index[4] = {z * 4, z * 4 + 1, z * 4 + 2, z * 4 + 3};
        for(int y = 0; y < by; y++){
            int y_index[4] = {y * 4, y * 4 + 1, y * 4 + 2, y * 4 + 3};
            for(int x = 0; x < bx; x++){
                int x_index[4] = {x * 4, x * 4 + 1, x * 4 + 2, x * 4 + 3};
                for(int t = 0; t < 4; t++){
                    //std::cout << "t = " << t << std::endl;
                    for(int zz = 0; zz < 4; zz++){
                        for(int yy = 0; yy < 4; yy++){
                            for(int xx = 0; xx < 4; xx++){
                                int index = z_index[zz] * dims.x * dims.y + y_index[yy] * dims.x + x_index[xx];
                                int sub_index = zz * 4 * 4 + yy * 4 + xx + t * 64;
                                //if(t == 3){
                                    //std::cout << "index = " << index << " subindex = " << sub_index << std::endl;
                                //}
                                //std::cout << "debug 1 in write raw 4" << std::endl;
                                sub_volume[sub_index] = volumes[t][index];
                                //std::cout << "debug 2 in write raw 4" << std::endl;
                            }
                        }
                    }

                    //std::cout << "t = " << t << std::endl;
                }
                //std::cout << "debug 1 in write raw 4" << std::endl;
                int result = compress_4(sub_volume.data(), 1e-3, fileDir);
            }
        }
    }
    //std::cout << "debug end in wrtie raw 4 " << std::endl;
} 


void writeRaw_3(std::vector<float> voxels, std::string fileDir, ospcommon::math::vec3i dims)
{
    // each 4^3 block is compressed using zfp
    int bx = dims.x / 4;
    int by = dims.y / 4;
    int bz = dims.z / 4;

    std::vector<float> sub_volume(4 * 4 * 4, 0);

    for(int z = 0; z < bz; z++){
        int z_index[4] = {z * 4, z * 4 + 1, z * 4 + 2, z * 4 + 3};
        for(int y = 0; y < by; y++){
            int y_index[4] = {y * 4, y * 4 + 1, y * 4 + 2, y * 4 + 3};
            for(int x = 0; x < bx; x++){
                int x_index[4] = {x * 4, x * 4 + 1, x * 4 + 2, x * 4 + 3};
                for(int zz = 0; zz < 4; zz++){
                    for(int yy = 0; yy < 4; yy++){
                        for(int xx = 0; xx < 4; xx++){
                            int index = z_index[zz] * dims.x * dims.y + y_index[yy] * dims.x + x_index[xx];
                            int sub_index = zz * 4 * 4 + yy * 4 + xx;
                            sub_volume[sub_index] = voxels[index];
                        }
                    }
                }
                // compress sub_volume and append to file
                // int result = compress(sub_volume.data(), 4, 4, 4, 1e-3, fileDir);
                int result = compress_3(sub_volume.data(), 4, 4, 4, 1e-3, fileDir);
            }
        }
    }

}

 /* compress or decompress array */
 static int compress_3(float* array, int nx, int ny, int nz, float tolerance, std::string file)
 {
    int status = 0;    /* return value: 0 = success */
    //zfp_type type;     /* array scalar type */
    //zfp_field* field;  /* array meta data */
    void* buffer;      /* storage for compressed stream */
    size_t bufsize;    /* byte size of compressed buffer */
    bitstream* stream; /* bit stream to write to or read from */
    size_t zfpsize;    /* byte size of compressed stream */

    zfp_stream* zfp;   /* compressed stream */
     /* allocate meta data for the 3D array a[nz][ny][nx] */
     //type = zfp_type_float;
     //field = zfp_field_3d(array, type, nx, ny, nz);

     /* allocate meta data for a compressed stream */
     zfp = zfp_stream_open(NULL);

     /* set compression mode and parameters via one of three functions */
    //zfp_stream_set_rate(zfp, rate, type, 3, 0); 
    //zfp_stream_set_precision(zfp, precision); 
     zfp_stream_set_accuracy(zfp, tolerance);

     /* allocate buffer for compressed data */
     //bufsize = zfp_stream_maximum_size(zfp, field);
     bufsize = 300;
     buffer = malloc(bufsize);

      //std::cout << "buffer size " << bufsize << std::endl;

     /* associate bit stream with allocated buffer */
     stream = stream_open(buffer, bufsize);
     zfp_stream_set_bit_stream(zfp, stream);
     zfp_stream_rewind(zfp);

     /* compress or decompress entire array */

     /* compress array and output compressed stream */
     //zfpsize = zfp_compress(zfp, field);

    uint size = zfp_encode_block_float_3(zfp, array);
    zfpsize = zfp_stream_flush(zfp);
    //std::cout << "size = " << size << " zfpsize = " << zfpsize << std::endl;
    if (!size) {
        fprintf(stderr, "zfpsize %d compression failed\n", zfpsize);
        status = 1;
    }
    else
    {
        FILE *f = fopen(file.c_str(), "ab");
        fwrite(buffer, 1, size/8, f);
        fclose(f);
     }


     /* clean up */
     //zfp_field_free(field);
     zfp_stream_close(zfp);
     stream_close(stream);
     free(buffer);
     // free(array);

     return status;
 }

static int compress_4(float* array, float tolerance, std::string file)
{
    int status = 0;
    void* buffer;      /* storage for compressed stream */
    size_t bufsize;    /* byte size of compressed buffer */
    bitstream* stream; /* bit stream to write to or read from */
    size_t zfpsize;    /* byte size of compressed stream */
    zfp_type type;     /* array scalar type */
    zfp_field* field;  /* array meta data */
    
    zfp_stream* zfp;   /* compressed stream */
    /* allocate meta data for the 3D array a[nz][ny][nx] */
    type = zfp_type_float;
    field = zfp_field_4d(array, type, 4, 4, 4, 4);
    
    zfp = zfp_stream_open(NULL);

    /* set compression mode and parameters via one of three functions */
    //zfp_stream_set_rate(zfp, rate, type, 3, 0); 
    //zfp_stream_set_precision(zfp, precision); 
    zfp_stream_set_accuracy(zfp, tolerance);

    /* allocate buffer for compressed data */
    bufsize = zfp_stream_maximum_size(zfp, field);
    //bufsize = 1200;
    buffer = malloc(bufsize);

    //std::cout << "buffer size " << bufsize << std::endl;

    /* associate bit stream with allocated buffer */
    stream = stream_open(buffer, bufsize);
    zfp_stream_set_bit_stream(zfp, stream);
    zfp_stream_rewind(zfp);

    /* compress or decompress entire array */

    /* compress array and output compressed stream */
    //zfpsize = zfp_compress(zfp, field);

    uint size = zfp_encode_block_float_4(zfp, array);
    zfpsize = zfp_stream_flush(zfp);
    //std::cout << "size = " << size << " zfpsize = " << zfpsize << std::endl;
    if (!size) {
        fprintf(stderr, "zfpsize %d compression failed\n", zfpsize);
        status = 1;
    }
    else
    {
        FILE *f = fopen(file.c_str(), "ab");
        fwrite(buffer, 1, size/8, f);
        fclose(f);
    }


    /* clean up */
    //zfp_field_free(field);
    zfp_stream_close(zfp);
    stream_close(stream);
    free(buffer);

    return status;
}
