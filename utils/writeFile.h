#pragma once 
#include <iostream>
#include <vector>
#include <fstream>

#include "ospcommon/math/vec.h"

#include "zfp.h"

static int compress(float* array, int nx, int ny, int nz, float tolerance, std::string file);

void writeRaw(std::vector<float> voxels, std::string fileDir, ospcommon::math::vec3i dims)
{
    // each 4^3 block is compressed using zfp
    int bx = dims.x / 4;
    int by = dims.y / 4;
    int bz = dims.z / 4;

    std::vector<float> sub_volume(4 * 4 * 4, 0);

    for(int z = 0; z < bz; z++){
        int z_index[] = {z * 4, z * 4 + 1, z * 4 + 2, z * 4 + 3};
        for(int y = 0; y < by; y++){
            int y_index[] = {y * 4, y * 4 + 1, y * 4 + 2, y * 4 + 3};
            for(int x = 0; x < bx; x++){
                int x_index[] = {x * 4, x * 4 + 1, x * 4 + 2, x * 4 + 3};
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
                uint result = compress_3(sub_volume.data(), fileDir);
            }
        }
    }

}


static uint compress_3(float *array, const std::string file)
{
    uint status = 0;
    zfp_stream* zfp;   /* compressed stream */
    zfp_encode_block_float_3(zfp, array);

    return status;
}


// /* compress or decompress array */
// static int compress(float* array, int nx, int ny, int nz, float tolerance, std::string file)
// {
//     int status = 0;    /* return value: 0 = success */
//     zfp_type type;     /* array scalar type */
//     zfp_field* field;  /* array meta data */
//     zfp_stream* zfp;   /* compressed stream */
//     void* buffer;      /* storage for compressed stream */
//     size_t bufsize;    /* byte size of compressed buffer */
//     bitstream* stream; /* bit stream to write to or read from */
//     size_t zfpsize;    /* byte size of compressed stream */

//     /* allocate meta data for the 3D array a[nz][ny][nx] */
//     type = zfp_type_float;
//     field = zfp_field_3d(array, type, nx, ny, nz);

//     /* allocate meta data for a compressed stream */
//     zfp = zfp_stream_open(NULL);

//     /* set compression mode and parameters via one of three functions */
//     /*  zfp_stream_set_rate(zfp, rate, type, 3, 0); */
//     /*  zfp_stream_set_precision(zfp, precision); */
//     zfp_stream_set_accuracy(zfp, tolerance);

//     /* allocate buffer for compressed data */
//     bufsize = zfp_stream_maximum_size(zfp, field);
//     buffer = malloc(bufsize);

//     // std::cout << "buffer size " << bufsize << std::endl;

//     /* associate bit stream with allocated buffer */
//     stream = stream_open(buffer, bufsize);
//     zfp_stream_set_bit_stream(zfp, stream);
//     zfp_stream_rewind(zfp);

//     /* compress or decompress entire array */

//     /* compress array and output compressed stream */
//     zfpsize = zfp_compress(zfp, field);

//     if (!zfpsize) {
//         fprintf(stderr, "compression failed\n");
//         status = 1;
//     }
//     else
//     {
//         FILE *f = fopen(file.c_str(), "ab");
//         // outFile.write(reinterpret_cast<char*>(&buffer[0]), sizeof(buffer)*sizeof(float));
//         // std::cout << "size of array after compress " << zfpsize << std::endl;
//         fwrite(buffer, 1, zfpsize, f);
//         fclose(f);
//         // fwrite(buffer, 1, zfpsize, stdout);
//     }


//     /* clean up */
//     zfp_field_free(field);
//     zfp_stream_close(zfp);
//     stream_close(stream);
//     free(buffer);
//     // free(array);

//     return status;
// }