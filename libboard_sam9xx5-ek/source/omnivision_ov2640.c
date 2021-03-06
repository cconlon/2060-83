/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2011, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include "board.h"

/*----------------------------------------------------------------------------
 *        Globle variables
 *----------------------------------------------------------------------------*/

/* 176x144 */
const struct ov_reg ov2640_yuv_qcif[]= {
{0xff, 0x01},{0x12, 0x80},
{0xff, 0x00},{0x2c, 0xff},{0x2e, 0xdf},
{0xff, 0x01},{0x3c, 0x32},{0x11, 0x00},{0x09, 0x02},
{0x04, 0x28},{0x13, 0xe5},{0x14, 0x48},{0x2c, 0x0c},{0x33, 0x78},{0x3a, 0x33},{0x3b, 0xfb},{0x3e, 0x00},{0x43, 0x11},
{0x16, 0x10},{0x39, 0x02},{0x35, 0x88},{0x22, 0x0a},{0x37, 0x40},{0x23, 0x00},{0x34, 0xa0},{0x36, 0x1a},{0x06, 0x02},
{0x07, 0xc0},{0x0d, 0xb7},{0x0e, 0x01},{0x4c, 0x00},{0x4a, 0x81},{0x21, 0x99},{0x24, 0x3a},{0x25, 0x32},{0x26, 0x82},
{0x5c, 0x00},{0x63, 0x00},{0x5d, 0x55},{0x5e, 0x7d},{0x5f, 0x7d},{0x60, 0x55},{0x61, 0x70},{0x62, 0x80},{0x7c, 0x05},
{0x20, 0x80},{0x28, 0x30},{0x6c, 0x00},{0x6d, 0x80},{0x6e, 0x00},{0x70, 0x02},{0x71, 0x94},{0x73, 0xc1},{0x3d, 0x34},
{0x5a, 0x57},{0x4f, 0xbb},{0x50, 0x9c},
{0xff, 0x00},{0xe5, 0x7f},{0xf9, 0xc0},{0x41, 0x24},{0xe0, 0x14},{0x76, 0xff},
{0x33, 0xa0},{0x42, 0x20},{0x43, 0x18},{0x4c, 0x00},{0x87, 0xd0},{0x88, 0x3f},{0xd7, 0x03},{0xd9, 0x10},{0xd3, 0x82},
{0xc8, 0x08},{0xc9, 0x80},{0x7c, 0x00},{0x7d, 0x02},{0x7c, 0x03},{0x7d, 0x48},{0x7d, 0x48},{0x7c, 0x08},{0x7d, 0x20},
{0x7d, 0x10},{0x7d, 0x0e},{0x90, 0x00},{0x91, 0x0e},{0x91, 0x1a},{0x91, 0x31},{0x91, 0x5a},{0x91, 0x69},{0x91, 0x75},
{0x91, 0x7e},{0x91, 0x88},{0x91, 0x8f},{0x91, 0x96},{0x91, 0xa3},{0x91, 0xaf},{0x91, 0xc4},{0x91, 0xd7},{0x91, 0xe8},
{0x91, 0x20},{0x92, 0x00},{0x93, 0x06},{0x93, 0xe3},{0x93, 0x05},{0x93, 0x05},{0x93, 0x00},{0x93, 0x02},{0x93, 0x00},
{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x96, 0x00},{0x97, 0x08},{0x97, 0x19},
{0x97, 0x02},{0x97, 0x0c},{0x97, 0x24},{0x97, 0x30},{0x97, 0x28},{0x97, 0x26},{0x97, 0x02},{0x97, 0x98},{0x97, 0x80},
{0x97, 0x00},{0x97, 0x00},{0xc3, 0xed},{0xa4, 0x00},{0xa8, 0x00},{0xc5, 0x11},{0xc6, 0x51},{0xbf, 0x80},{0xc7, 0x10},
{0xb6, 0x66},{0xb8, 0xa5},{0xb7, 0x64},{0xb9, 0x7c},{0xb3, 0xaf},{0xb4, 0x97},{0xb5, 0xff},{0xb0, 0xc5},{0xb1, 0x94},
{0xb2, 0x0f},{0xc4, 0x5c},{0xc0, 0xc8},{0xc1, 0x96},{0x86, 0x1d},{0x50, 0x00},{0x51, 0x90},{0x52, 0x18},{0x53, 0x00},
{0x54, 0x00},{0x55, 0x88},{0x57, 0x00},{0x5a, 0x90},{0x5b, 0x18},{0x5c, 0x05},{0xc3, 0xed},{0x7f, 0x00},{0xda, 0x04},
{0xe5, 0x1f},{0xe1, 0x67},{0xe0, 0x00},{0xdd, 0xff},{0x05, 0x00},
{0xff, 0x01},{0x11, 0x01},
{0xff, 0x01},{0x12, 0x40},
{0x17, 0x11},{0x18, 0x43},{0x19, 0x00},{0x1a, 0x4b},{0x32, 0x09},{0x4f, 0xca},{0x50, 0xa8},{0x5a, 0x23},{0x6d, 0x00},
{0x3d, 0x38},{0x39, 0x12},{0x35, 0xda},{0x22, 0x1a},{0x37, 0xc3},{0x23, 0x00},{0x34, 0xc0},{0x36, 0x1a},{0x06, 0x88},
{0x07, 0xc0},{0x0d, 0x87},{0x0e, 0x41},{0x4c, 0x00},{0x48, 0x00},{0x5B, 0x00},{0x42, 0x03},
{0xff, 0x00},{0xe0, 0x04},
{0xc0, 0x64},{0xc1, 0x4B},{0x8c, 0x00}, // Xiao: H 0x64*8 = 800, V 0x4B*8 = 600
{0x86, 0x1D},
{0xd3, 0x82},{0xe0, 0x00},
{0xff, 0x00},
{0xc0, 0x64},{0xc1, 0x4B},{0x8c, 0x00}, // Xiao: H 0x64*8 = 800, V 0x4B*8 = 600
{0x86, 0x3D},
{0x50, 0x89},
{0x51, 0xC8},{0x52, 0x96},{0x53, 0x00},{0x54, 0x00},{0x55, 0x00}, // Xiao: HSIZE 0xC8(200)*4 = 800, VSIZE 0x96(150)*4 = 600
{0x5a, 0x2C},{0x5b, 0x26},{0x5c, 0x00}, // Xiao: ZW 0x2C(44)*4 = 176, ZH 0x26(38)*4 = 144
{0xd3, 0x04},
{0xFF, 0x00},{0xE0, 0x04},{0xE1, 0x67},{0xD7, 0x01},{0xDA, 0x00},{0xD3, 0x82},{0xE0, 0x00},
{0xFF, 0xFF}
};

/* 352*288 */
const struct ov_reg ov2640_yuv_cif[]= {
{0xff, 0x01},{0x12, 0x80},
{0xff, 0x00},{0x2c, 0xff},{0x2e, 0xdf},
{0xff, 0x01},{0x3c, 0x32},{0x11, 0x00},{0x09, 0x02},
{0x04, 0x28},{0x13, 0xe5},{0x14, 0x48},{0x2c, 0x0c},{0x33, 0x78},{0x3a, 0x33},{0x3b, 0xfb},{0x3e, 0x00},{0x43, 0x11},
{0x16, 0x10},{0x39, 0x02},{0x35, 0x88},{0x22, 0x0a},{0x37, 0x40},{0x23, 0x00},{0x34, 0xa0},{0x36, 0x1a},{0x06, 0x02},
{0x07, 0xc0},{0x0d, 0xb7},{0x0e, 0x01},{0x4c, 0x00},{0x4a, 0x81},{0x21, 0x99},{0x24, 0x3a},{0x25, 0x32},{0x26, 0x82},
{0x5c, 0x00},{0x63, 0x00},{0x5d, 0x55},{0x5e, 0x7d},{0x5f, 0x7d},{0x60, 0x55},{0x61, 0x70},{0x62, 0x80},{0x7c, 0x05},
{0x20, 0x80},{0x28, 0x30},{0x6c, 0x00},{0x6d, 0x80},{0x6e, 0x00},{0x70, 0x02},{0x71, 0x94},{0x73, 0xc1},{0x3d, 0x34},
{0x5a, 0x57},{0x4f, 0xbb},{0x50, 0x9c},
{0xff, 0x00},{0xe5, 0x7f},{0xf9, 0xc0},{0x41, 0x24},{0xe0, 0x14},{0x76, 0xff},
{0x33, 0xa0},{0x42, 0x20},{0x43, 0x18},{0x4c, 0x00},{0x87, 0xd0},{0x88, 0x3f},{0xd7, 0x03},{0xd9, 0x10},{0xd3, 0x82},
{0xc8, 0x08},{0xc9, 0x80},{0x7c, 0x00},{0x7d, 0x02},{0x7c, 0x03},{0x7d, 0x48},{0x7d, 0x48},{0x7c, 0x08},{0x7d, 0x20},
{0x7d, 0x10},{0x7d, 0x0e},{0x90, 0x00},{0x91, 0x0e},{0x91, 0x1a},{0x91, 0x31},{0x91, 0x5a},{0x91, 0x69},{0x91, 0x75},
{0x91, 0x7e},{0x91, 0x88},{0x91, 0x8f},{0x91, 0x96},{0x91, 0xa3},{0x91, 0xaf},{0x91, 0xc4},{0x91, 0xd7},{0x91, 0xe8},
{0x91, 0x20},{0x92, 0x00},{0x93, 0x06},{0x93, 0xe3},{0x93, 0x05},{0x93, 0x05},{0x93, 0x00},{0x93, 0x02},{0x93, 0x00},
{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x96, 0x00},{0x97, 0x08},{0x97, 0x19},
{0x97, 0x02},{0x97, 0x0c},{0x97, 0x24},{0x97, 0x30},{0x97, 0x28},{0x97, 0x26},{0x97, 0x02},{0x97, 0x98},{0x97, 0x80},
{0x97, 0x00},{0x97, 0x00},{0xc3, 0xed},{0xa4, 0x00},{0xa8, 0x00},{0xc5, 0x11},{0xc6, 0x51},{0xbf, 0x80},{0xc7, 0x10},
{0xb6, 0x66},{0xb8, 0xa5},{0xb7, 0x64},{0xb9, 0x7c},{0xb3, 0xaf},{0xb4, 0x97},{0xb5, 0xff},{0xb0, 0xc5},{0xb1, 0x94},
{0xb2, 0x0f},{0xc4, 0x5c},{0xc0, 0xc8},{0xc1, 0x96},{0x86, 0x1d},{0x50, 0x00},{0x51, 0x90},{0x52, 0x18},{0x53, 0x00},
{0x54, 0x00},{0x55, 0x88},{0x57, 0x00},{0x5a, 0x90},{0x5b, 0x18},{0x5c, 0x05},{0xc3, 0xed},{0x7f, 0x00},{0xda, 0x04},
{0xe5, 0x1f},{0xe1, 0x67},{0xe0, 0x00},{0xdd, 0xff},{0x05, 0x00},
{0xff, 0x01},{0x11, 0x01},
{0xff, 0x01},{0x12, 0x40},
{0x17, 0x11},{0x18, 0x43},{0x19, 0x00},{0x1a, 0x4b},{0x32, 0x09},{0x4f, 0xca},{0x50, 0xa8},{0x5a, 0x23},{0x6d, 0x00},
{0x3d, 0x38},{0x39, 0x12},{0x35, 0xda},{0x22, 0x1a},{0x37, 0xc3},{0x23, 0x00},{0x34, 0xc0},{0x36, 0x1a},{0x06, 0x88},
{0x07, 0xc0},{0x0d, 0x87},{0x0e, 0x41},{0x4c, 0x00},{0x48, 0x00},{0x5B, 0x00},{0x42, 0x03},
{0xff, 0x00},{0xe0, 0x04},
{0xc0, 0x64},{0xc1, 0x4B},{0x8c, 0x00}, // Xiao: H 0x64*8 = 800, V 0x4B*8 = 600
{0x86, 0x1D},
{0xd3, 0x82},{0xe0, 0x00},
{0xff, 0x00},
{0xc0, 0x64},{0xc1, 0x4B},{0x8c, 0x00}, // Xiao: H 0x64*8 = 800, V 0x4B*8 = 600
{0x86, 0x3D},
{0x50, 0x89},
{0x51, 0xC8},{0x52, 0x96},{0x53, 0x00},{0x54, 0x00},{0x55, 0x00}, // Xiao: HSIZE 0xC8(200)*4 = 800, VSIZE 0x96(150)*4 = 600
{0x5a, 0x58},{0x5b, 0x4C},{0x5c, 0x00}, // Xiao: ZW 0x58(88)*4 = 352, ZH 0x4C(76)*4 = 288
{0xd3, 0x04},
{0xFF, 0x00},{0xE0, 0x04},{0xE1, 0x67},{0xD7, 0x01},{0xDA, 0x00},{0xD3, 0x82},{0xE0, 0x00},
{0xFF, 0xFF}
};

/* 320*240 */
const struct ov_reg ov2640_yuv_qvga[]= {
{0xff, 0x01},{0x12, 0x80},
{0xff, 0x00},{0x2c, 0xff},{0x2e, 0xdf},
{0xff, 0x01},{0x3c, 0x32},{0x11, 0x00},{0x09, 0x02},
{0x04, 0x28},{0x13, 0xe5},{0x14, 0x48},{0x2c, 0x0c},{0x33, 0x78},{0x3a, 0x33},{0x3b, 0xfb},{0x3e, 0x00},{0x43, 0x11},
{0x16, 0x10},{0x39, 0x02},{0x35, 0x88},{0x22, 0x0a},{0x37, 0x40},{0x23, 0x00},{0x34, 0xa0},{0x36, 0x1a},{0x06, 0x02},
{0x07, 0xc0},{0x0d, 0xb7},{0x0e, 0x01},{0x4c, 0x00},{0x4a, 0x81},{0x21, 0x99},{0x24, 0x3a},{0x25, 0x32},{0x26, 0x82},
{0x5c, 0x00},{0x63, 0x00},{0x5d, 0x55},{0x5e, 0x7d},{0x5f, 0x7d},{0x60, 0x55},{0x61, 0x70},{0x62, 0x80},{0x7c, 0x05},
{0x20, 0x80},{0x28, 0x30},{0x6c, 0x00},{0x6d, 0x80},{0x6e, 0x00},{0x70, 0x02},{0x71, 0x94},{0x73, 0xc1},{0x3d, 0x34},
{0x5a, 0x57},{0x4f, 0xbb},{0x50, 0x9c},
{0xff, 0x00},{0xe5, 0x7f},{0xf9, 0xc0},{0x41, 0x24},{0xe0, 0x14},{0x76, 0xff},
{0x33, 0xa0},{0x42, 0x20},{0x43, 0x18},{0x4c, 0x00},{0x87, 0xd0},{0x88, 0x3f},{0xd7, 0x03},{0xd9, 0x10},{0xd3, 0x82},
{0xc8, 0x08},{0xc9, 0x80},{0x7c, 0x00},{0x7d, 0x02},{0x7c, 0x03},{0x7d, 0x48},{0x7d, 0x48},{0x7c, 0x08},{0x7d, 0x20},
{0x7d, 0x10},{0x7d, 0x0e},{0x90, 0x00},{0x91, 0x0e},{0x91, 0x1a},{0x91, 0x31},{0x91, 0x5a},{0x91, 0x69},{0x91, 0x75},
{0x91, 0x7e},{0x91, 0x88},{0x91, 0x8f},{0x91, 0x96},{0x91, 0xa3},{0x91, 0xaf},{0x91, 0xc4},{0x91, 0xd7},{0x91, 0xe8},
{0x91, 0x20},{0x92, 0x00},{0x93, 0x06},{0x93, 0xe3},{0x93, 0x05},{0x93, 0x05},{0x93, 0x00},{0x93, 0x02},{0x93, 0x00},
{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x96, 0x00},{0x97, 0x08},{0x97, 0x19},
{0x97, 0x02},{0x97, 0x0c},{0x97, 0x24},{0x97, 0x30},{0x97, 0x28},{0x97, 0x26},{0x97, 0x02},{0x97, 0x98},{0x97, 0x80},
{0x97, 0x00},{0x97, 0x00},{0xc3, 0xed},{0xa4, 0x00},{0xa8, 0x00},{0xc5, 0x11},{0xc6, 0x51},{0xbf, 0x80},{0xc7, 0x10},
{0xb6, 0x66},{0xb8, 0xa5},{0xb7, 0x64},{0xb9, 0x7c},{0xb3, 0xaf},{0xb4, 0x97},{0xb5, 0xff},{0xb0, 0xc5},{0xb1, 0x94},
{0xb2, 0x0f},{0xc4, 0x5c},{0xc0, 0xc8},{0xc1, 0x96},{0x86, 0x1d},{0x50, 0x00},{0x51, 0x90},{0x52, 0x18},{0x53, 0x00},
{0x54, 0x00},{0x55, 0x88},{0x57, 0x00},{0x5a, 0x90},{0x5b, 0x18},{0x5c, 0x05},{0xc3, 0xed},{0x7f, 0x00},{0xda, 0x04},
{0xe5, 0x1f},{0xe1, 0x67},{0xe0, 0x00},{0xdd, 0xff},{0x05, 0x00},
{0xff, 0x01},{0x11, 0x01},
{0xff, 0x01},{0x12, 0x40},
{0x17, 0x11},{0x18, 0x43},{0x19, 0x00},{0x1a, 0x4b},{0x32, 0x09},{0x4f, 0xca},{0x50, 0xa8},{0x5a, 0x23},{0x6d, 0x00},
{0x3d, 0x38},{0x39, 0x12},{0x35, 0xda},{0x22, 0x1a},{0x37, 0xc3},{0x23, 0x00},{0x34, 0xc0},{0x36, 0x1a},{0x06, 0x88},
{0x07, 0xc0},{0x0d, 0x87},{0x0e, 0x41},{0x4c, 0x00},{0x48, 0x00},{0x5B, 0x00},{0x42, 0x03},
{0xff, 0x00},{0xe0, 0x04},
{0xc0, 0x64},{0xc1, 0x4B},{0x8c, 0x00},{0x86, 0x1D},{0xd3, 0x82},{0xe0, 0x00},
{0xff, 0x00},
{0xc0, 0x64},{0xc1, 0x4B},{0x8c, 0x00}, // Xiao: HSIZE 0x64*8 = 800, VSIZE 0x4b*8 = 600
{0x86, 0x3D},
{0x50, 0x89}, // LP_DP, V_DIV 1, H_DIV 1
{0x51, 0xC8},{0x52, 0x96},{0x53, 0x00},{0x54, 0x00},{0x55, 0x00}, // Xiao: HSIZE 0xC8(200)*4 = 800, VSIZE 0x96(150)*4 = 600
{0x5a, 0x50},{0x5b, 0x3C},{0x5c, 0x00}, // Xiao: ZMOW 0x50(80)*4 = 320, ZMOH 0x3C(60)*4 = 240
{0xd3, 0x04},
{0xFF, 0x00},{0xE0, 0x04},{0xE1, 0x67},{0xD7, 0x01},{0xDA, 0x00},{0xD3, 0x82},{0xE0, 0x00},
{0xFF, 0xFF}
};

/* 1280x1024 */
const struct ov_reg ov2640_yuv_sxga[]= {
{0xff, 0x01},{0x12, 0x80},
{0xff, 0x00},{0x2c, 0xff},{0x2e, 0xdf},
{0xff, 0x01},{0x3c, 0x32},{0x11, 0x00},{0x09, 0x02},
{0x04, 0x28},{0x13, 0xe5},{0x14, 0x48},{0x2c, 0x0c},{0x33, 0x78},{0x3a, 0x33},{0x3b, 0xfb},{0x3e, 0x00},{0x43, 0x11},
{0x16, 0x10},{0x39, 0x02},{0x35, 0x88},{0x22, 0x0a},{0x37, 0x40},{0x23, 0x00},{0x34, 0xa0},{0x36, 0x1a},{0x06, 0x02},
{0x07, 0xc0},{0x0d, 0xb7},{0x0e, 0x01},{0x4c, 0x00},{0x4a, 0x81},{0x21, 0x99},{0x24, 0x3a},{0x25, 0x32},{0x26, 0x82},
{0x5c, 0x00},{0x63, 0x00},{0x5d, 0x55},{0x5e, 0x7d},{0x5f, 0x7d},{0x60, 0x55},{0x61, 0x70},{0x62, 0x80},{0x7c, 0x05},
{0x20, 0x80},{0x28, 0x30},{0x6c, 0x00},{0x6d, 0x80},{0x6e, 0x00},{0x70, 0x02},{0x71, 0x94},{0x73, 0xc1},{0x3d, 0x34},
{0x5a, 0x57},{0x4f, 0xbb},{0x50, 0x9c},
{0xff, 0x00},{0xe5, 0x7f},{0xf9, 0xc0},{0x41, 0x24},{0xe0, 0x14},{0x76, 0xff},
{0x33, 0xa0},{0x42, 0x20},{0x43, 0x18},{0x4c, 0x00},{0x87, 0xd0},{0x88, 0x3f},{0xd7, 0x03},{0xd9, 0x10},{0xd3, 0x82},
{0xc8, 0x08},{0xc9, 0x80},{0x7c, 0x00},{0x7d, 0x02},{0x7c, 0x03},{0x7d, 0x48},{0x7d, 0x48},{0x7c, 0x08},{0x7d, 0x20},
{0x7d, 0x10},{0x7d, 0x0e},{0x90, 0x00},{0x91, 0x0e},{0x91, 0x1a},{0x91, 0x31},{0x91, 0x5a},{0x91, 0x69},{0x91, 0x75},
{0x91, 0x7e},{0x91, 0x88},{0x91, 0x8f},{0x91, 0x96},{0x91, 0xa3},{0x91, 0xaf},{0x91, 0xc4},{0x91, 0xd7},{0x91, 0xe8},
{0x91, 0x20},{0x92, 0x00},{0x93, 0x06},{0x93, 0xe3},{0x93, 0x05},{0x93, 0x05},{0x93, 0x00},{0x93, 0x02},{0x93, 0x00},
{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x96, 0x00},{0x97, 0x08},{0x97, 0x19},
{0x97, 0x02},{0x97, 0x0c},{0x97, 0x24},{0x97, 0x30},{0x97, 0x28},{0x97, 0x26},{0x97, 0x02},{0x97, 0x98},{0x97, 0x80},
{0x97, 0x00},{0x97, 0x00},{0xc3, 0xed},{0xa4, 0x00},{0xa8, 0x00},{0xc5, 0x11},{0xc6, 0x51},{0xbf, 0x80},{0xc7, 0x10},
{0xb6, 0x66},{0xb8, 0xa5},{0xb7, 0x64},{0xb9, 0x7c},{0xb3, 0xaf},{0xb4, 0x97},{0xb5, 0xff},{0xb0, 0xc5},{0xb1, 0x94},
{0xb2, 0x0f},{0xc4, 0x5c},{0xc0, 0xc8},{0xc1, 0x96},{0x86, 0x1d},{0x50, 0x00},{0x51, 0x90},{0x52, 0x18},{0x53, 0x00},
{0x54, 0x00},{0x55, 0x88},{0x57, 0x00},{0x5a, 0x90},{0x5b, 0x18},{0x5c, 0x05},{0xc3, 0xed},{0x7f, 0x00},{0xda, 0x04},
{0xe5, 0x1f},{0xe1, 0x67},{0xe0, 0x00},{0xdd, 0xff},{0x05, 0x00},
{0xff, 0x01},{0x11, 0x00},
{0xff, 0x00},
{0xc0, 0xC8},{0xc1, 0x96},{0x8c, 0x00}, // Xiao: HSIZE 0xC8*8 = 1600, VSIZE 0x96*8 = 1200
{0x86, 0x3D},
{0x50, 0x00},
{0x51, 0x90},{0x52, 0x2C},{0x53, 0x00},{0x54, 0x00},{0x55, 0x88}, // Xiao: HSIZE 0x190(400)*4 = 1600, VSIZE 0x12C(300)*4 = 1200
{0x5a, 0x40},{0x5b, 0x00},{0x5c, 0x05}, // Xiao: ZMOW 0x140(320)*4 = 1280, ZMOW 0x100(256)*4 = 1024
{0xd3, 0x82},
{0xFF, 0x00},{0xE0, 0x04},{0xE1, 0x67},{0xD7, 0x01},{0xDA, 0x00},{0xD3, 0x82},{0xE0, 0x00},
{0xFF, 0xFF}
};

/* 640x480 */
const struct ov_reg ov2640_yuv_vga[]= {
{0xff, 0x01},{0x12, 0x80},
{0xff, 0x00},{0x2c, 0xff},{0x2e, 0xdf},
{0xff, 0x01},{0x3c, 0x32},{0x11, 0x00},{0x09, 0x02},
{0x04, 0x28},{0x13, 0xe5},{0x14, 0x48},{0x2c, 0x0c},{0x33, 0x78},{0x3a, 0x33},{0x3b, 0xfb},{0x3e, 0x00},{0x43, 0x11},
{0x16, 0x10},{0x39, 0x02},{0x35, 0x88},{0x22, 0x0a},{0x37, 0x40},{0x23, 0x00},{0x34, 0xa0},{0x36, 0x1a},{0x06, 0x02},
{0x07, 0xc0},{0x0d, 0xb7},{0x0e, 0x01},{0x4c, 0x00},{0x4a, 0x81},{0x21, 0x99},{0x24, 0x3a},{0x25, 0x32},{0x26, 0x82},
{0x5c, 0x00},{0x63, 0x00},{0x5d, 0x55},{0x5e, 0x7d},{0x5f, 0x7d},{0x60, 0x55},{0x61, 0x70},{0x62, 0x80},{0x7c, 0x05},
{0x20, 0x80},{0x28, 0x30},{0x6c, 0x00},{0x6d, 0x80},{0x6e, 0x00},{0x70, 0x02},{0x71, 0x94},{0x73, 0xc1},{0x3d, 0x34},
{0x5a, 0x57},{0x4f, 0xbb},{0x50, 0x9c},
{0xff, 0x00},{0xe5, 0x7f},{0xf9, 0xc0},{0x41, 0x24},{0xe0, 0x14},{0x76, 0xff},
{0x33, 0xa0},{0x42, 0x20},{0x43, 0x18},{0x4c, 0x00},{0x87, 0xd0},{0x88, 0x3f},{0xd7, 0x03},{0xd9, 0x10},{0xd3, 0x82},
{0xc8, 0x08},{0xc9, 0x80},{0x7c, 0x00},{0x7d, 0x02},{0x7c, 0x03},{0x7d, 0x48},{0x7d, 0x48},{0x7c, 0x08},{0x7d, 0x20},
{0x7d, 0x10},{0x7d, 0x0e},{0x90, 0x00},{0x91, 0x0e},{0x91, 0x1a},{0x91, 0x31},{0x91, 0x5a},{0x91, 0x69},{0x91, 0x75},
{0x91, 0x7e},{0x91, 0x88},{0x91, 0x8f},{0x91, 0x96},{0x91, 0xa3},{0x91, 0xaf},{0x91, 0xc4},{0x91, 0xd7},{0x91, 0xe8},
{0x91, 0x20},{0x92, 0x00},{0x93, 0x06},{0x93, 0xe3},{0x93, 0x05},{0x93, 0x05},{0x93, 0x00},{0x93, 0x02},{0x93, 0x00},
{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x93, 0x00},{0x96, 0x00},{0x97, 0x08},{0x97, 0x19},
{0x97, 0x02},{0x97, 0x0c},{0x97, 0x24},{0x97, 0x30},{0x97, 0x28},{0x97, 0x26},{0x97, 0x02},{0x97, 0x98},{0x97, 0x80},
{0x97, 0x00},{0x97, 0x00},{0xc3, 0xed},{0xa4, 0x00},{0xa8, 0x00},{0xc5, 0x11},{0xc6, 0x51},{0xbf, 0x80},{0xc7, 0x10},
{0xb6, 0x66},{0xb8, 0xa5},{0xb7, 0x64},{0xb9, 0x7c},{0xb3, 0xaf},{0xb4, 0x97},{0xb5, 0xff},{0xb0, 0xc5},{0xb1, 0x94},
{0xb2, 0x0f},{0xc4, 0x5c},{0xc0, 0xc8},{0xc1, 0x96},{0x86, 0x1d},{0x50, 0x00},{0x51, 0x90},{0x52, 0x18},{0x53, 0x00},
{0x54, 0x00},{0x55, 0x88},{0x57, 0x00},{0x5a, 0x90},{0x5b, 0x18},{0x5c, 0x05},{0xc3, 0xed},{0x7f, 0x00},{0xda, 0x04},
{0xe5, 0x1f},{0xe1, 0x67},{0xe0, 0x00},{0xdd, 0xff},{0x05, 0x00},
{0xff, 0x01},{0x11, 0x01},
{0xff, 0x01},{0x12, 0x40},
{0x17, 0x11},{0x18, 0x43},{0x19, 0x00},{0x1a, 0x4b},{0x32, 0x09},{0x4f, 0xca},{0x50, 0xa8},{0x5a, 0x23},{0x6d, 0x00},
{0x3d, 0x38},{0x39, 0x12},{0x35, 0xda},{0x22, 0x1a},{0x37, 0xc3},{0x23, 0x00},{0x34, 0xc0},{0x36, 0x1a},{0x06, 0x88},
{0x07, 0xc0},{0x0d, 0x87},{0x0e, 0x41},{0x4c, 0x00},{0x48, 0x00},{0x5B, 0x00},{0x42, 0x03},
{0xff, 0x00},{0xe0, 0x04},
{0xc0, 0x64},{0xc1, 0x4B},{0x8c, 0x00}, // Xiao: H 0x64*8 = 800, V 0x4B*8 = 600
{0x86, 0x1D},
{0xd3, 0x82},{0xe0, 0x00},
{0xff, 0x00},
{0xc0, 0x64},{0xc1, 0x4B},{0x8c, 0x00}, // Xiao: H 0x64*8 = 800, V 0x4B*8 = 600
{0x86, 0x3D},
{0x50, 0x00},
{0x51, 0xC8},{0x52, 0x96},{0x53, 0x00},{0x54, 0x00},{0x55, 0x00}, // Xiao: H 0xC8(200)*4 = 800, V 0x96(150)*4 = 600
{0x5a, 0xA0},{0x5b, 0x78},{0x5c, 0x00}, // Xiao: ZW 0xa0(160)*4 = 640, ZH 0x78(120)*4 = 480
{0xd3, 0x04},
{0xFF, 0x00},{0xE0, 0x04},{0xE1, 0x67},{0xD7, 0x01},{0xDA, 0x00},{0xD3, 0x82},{0xE0, 0x00},
{0xFF, 0xFF}
};

/*----------------------------------------------------------------------------
 *        Defines
 *----------------------------------------------------------------------------*/

/** Slave address of OMNIVISION chips. */
#define OV_CAPTOR_ADDRESS   (0x60 >> 1)

/** terminating list entry for register in configuration file */
#define OV_REG_TERM 0xff
/** terminating list entry for value in configuration file */
#define OV_VAL_TERM 0xff

/** Array of image sizes supported by OV264x */
static const struct capture_size ov264x_sizes[] = {
//  {width, height}
    /** QVGA */
    { 320, 240 },
    /** CIF */
    { 352, 288 },
    /** VGA */
    { 640, 480 },
    /** SXGA */
    {1280, 1024 },
    /** QCIF */
    { 176, 144 }
};

 
/*----------------------------------------------------------------------------
 *        Local Functions
 *----------------------------------------------------------------------------*/
/**
 *  \brief Reset using ctrl signal
 *
 *  \param pCtrl1 PIO interface
 */

static void ov_reset(const Pin *pCtrl1)
{
  volatile unsigned int i;

  if(pCtrl1->id == 0)
      return;

  pCtrl1->pio->PIO_CODR = pCtrl1->mask;
  for(i=0; i<6000; i++ );
  pCtrl1->pio->PIO_SODR = pCtrl1->mask;
  for(i=0; i<6000; i++ );
}

/**
 *  \brief Read PID and VER
 *
 *  \param pTwid TWI interface
 *
 *  \return VER | (PID<<8)
 */
static unsigned short ov_id(Twid *pTwid)
{
    uint8_t id=0;
    uint8_t ver=0;

    // OV_PID
    ov_read_reg(pTwid, 0x0A, &id);
    TRACE_INFO("PID  = 0x%X\n\r", id);

    // OV_VER
    ov_read_reg(pTwid, 0x0B, &ver);
    TRACE_INFO("VER  = 0x%X\n\r", ver);

    return((unsigned short)(id <<8) | ver);
}

/**
 *  \brief Read Manufacturer
 *
 *  \param pTwid TWI interface
 *  \return 0 if error; 1 if the good captor is present
 */
static uint8_t ov_Manufacturer(Twid *pTwid)
{
    uint8_t midh=0;
    uint8_t midl=0;
    unsigned status = 0;

    // OV_MIDH
    ov_read_reg(pTwid, 0x1C, &midh);
    TRACE_DEBUG("MIDH = 0x%X\n\r", midh);

    // OV_MIDL
    ov_read_reg(pTwid, 0x1D, &midl);
    TRACE_DEBUG("MIDL = 0x%X\n\r", midl);

    if(( midh == 0x7F) && (midl == 0xA2)) {
        status = 1;
    }
    return(status);
}

/**
 *  \brief ov_TestWrite
 *  \param pTwid TWI interface
 *  \return 1 if  the write is correct; 0 otherwise
 */
static uint8_t ov_TestWrite(Twid *pTwid)
{
    uint8_t value=0;
    uint8_t oldvalue=0;

    // OV_BLUE
    ov_read_reg(pTwid, 0x01, &oldvalue);
    ov_write_reg(pTwid, 0x01, 0xAD);
    ov_read_reg(pTwid, 0x01, &value);
    if( value != 0xAD ) {
        return(0);
    }
    // return old value
    ov_write_reg(pTwid, 0x01, oldvalue);
    ov_read_reg(pTwid, 0x01, &value);
    if( value != oldvalue ) {
        return(0);
    }
    return(1);
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
/**
 *  \brief Read a value from a register in an OV9650 sensor device.
 *  \param pTwid TWI interface
 *  \param reg Register to be read
 *  \param pData Data read
 *  \return 0 if no error, otherwize TWID_ERROR_BUSY
 */
uint8_t ov_read_reg(Twid *pTwid, uint8_t reg, uint8_t *pData)
{
    uint8_t status;

    status = TWID_Write( pTwid, OV_CAPTOR_ADDRESS, 0, 0, &reg, 1, 0);
    status |= TWID_Read( pTwid, OV_CAPTOR_ADDRESS, 0, 0, pData, 1, 0);
    //status = TWID_Read(pTwid, OV_CAPTOR_ADDRESS, reg, 1, pData, 1, 0);
    if( status != 0 ) {
        TRACE_ERROR("ov_read_reg pb");
    }
    return status;
}
/**
 *  \brief Write a value to a register in an OV9650 sensor device.
 *  \param pTwid TWI interface
 *  \param reg Register to be write
 *  \param val Value to be writte
 *  \return 0 if no error, otherwize TWID_ERROR_BUSY
 */
uint8_t ov_write_reg(Twid *pTwid, uint8_t reg, uint8_t val)
{
    uint8_t status;

    status = TWID_Write(pTwid, OV_CAPTOR_ADDRESS, reg, 1, &val, 1, 0);
    if( status != 0 ) {
        TRACE_ERROR("ov_write_reg pb");
    }

    return status;
}

/**
 *  \brief Initialize a list of OV registers.
 *  The list of registers is terminated by the pair of values { OV_REG_TERM, OV_VAL_TERM }.
 *  \param pTwid TWI interface
 *  \param pReglist Register list to be written
 *  \return 0 if no error, otherwize TWID_ERROR_BUSY
 */
uint32_t ov_write_regs(Twid *pTwid, const struct ov_reg* pReglist)
{
    int err;
    int size=0;
    const struct ov_reg *pNext = pReglist;
    unsigned int i=0;

    TRACE_DEBUG("ov_write_regs:");
    while (!((pNext->reg == OV_REG_TERM) && (pNext->val == OV_VAL_TERM))) {
        err = ov_write_reg(pTwid, pNext->reg, pNext->val);
        TRACE_DEBUG_WP("+(%d) ", size);
        size++;

        //delay(1);
        for(i=0; i<6000; i++ ) {
            //*(unsigned int*)0x20400000 = 0;
        }

        if (err == TWID_ERROR_BUSY){
            TRACE_ERROR("ov_write_regs: TWI ERROR\n\r");
            return err;
        }
        pNext++;
    }
    TRACE_DEBUG_WP("\n\r");
    return 0;
}

/**
 *  \brief Dump all register
 *  \param pTwid TWI interface
 *  \param ovType Sensor type
 */
void ov_DumpRegisters(Twid *pTwid, uint8_t ovType)
{
    int i;
    uint8_t value;
    uint8_t regNum;

    switch(ovType){

        case BOARD_OV9655:
            regNum = 0xDA;
        break;

        case BOARD_OV2640:
            regNum = 0x5C;
        break;

        default:
            printf("Omnivision type %d not supported. \r\n", ovType);
            return ;
        //break;
    }
    TRACE_INFO_WP("Dump all camera register\n\r");
    for(i = 0; i <= regNum; i++) {
        value = 0;
        ov_read_reg(pTwid, i, &value);
        printf("[0x%02x]=0x%02x ", i, value);
        if( ((i+1)%5) == 0 ) {
            printf("\n\r");
        }
    }
    printf("\n\r");
}

/**
 *  \brief Sequence For correct operation of the sensor.
 *  \param pTwid TWI interface
 *  \param pCtrl1 Ctrl1 signal
 *  \return 1 if initialization ok, otherwise 0
 */
uint8_t ov_init(Twid *pTwid, const Pin *pCtrl1)
{
    unsigned short id=0;
    unsigned short bdId;

    bdId = 0x26;
    ov_reset(pCtrl1);
    id = ov_id(pTwid);
    if( (id>>8) == bdId ) {
        TRACE_DEBUG("ID and PID OK\n\r");
        if( ov_Manufacturer(pTwid) == 1 ) {
            TRACE_DEBUG("Manufacturer OK\n\r");
            if( ov_TestWrite(pTwid) == 1 ) {
                return 1;
            }
            else {
                TRACE_ERROR("Problem captor: bad write\n\r");
            }
        }
        else {
            TRACE_ERROR("Problem captor: bad Manufacturer\n\r");
        }
    }
    else {
        TRACE_ERROR("Problem captor: bad PID\n\r");
    }
    TRACE_INFO("Problem: captor not responding\n\r");
    return 0;
}

/**
 *  \brief Power control using ctrl2 signal
 *
 *  \param pCtrl2 PIO interface
 *  \param powerType on, off
 */

void ov_pwd(const Pin *pCtrl2, uint8_t powerType)
{
    if(pCtrl2->id == 0)
        return;

    if(powerType == OV_CTRL2_POWER_OFF)
        pCtrl2->pio->PIO_CODR = pCtrl2->mask;
    else if (powerType == OV_CTRL2_POWER_ON)
        pCtrl2->pio->PIO_SODR = pCtrl2->mask;
}

/**
 *  \brief Configure the OV2640 for a specified image size, pixel format, and frame period.
 *
 *  \note Supported image sizes: 320x240, 352x288, 640x480, 1280x1024
 *
 *  \param pTwid TWI interface
 *  \param width Image width
 *  \param heigth Image Height
 */

void ov_configure(Twid *pTwid, unsigned int width, unsigned int heigth)
{
    const struct ov_reg *reg_conf;
    unsigned char goodCaptureSize = 0;
    unsigned char i;

    TRACE_DEBUG("ov264x_configure\n\r");
    for( i=0; i<sizeof(ov264x_sizes); i++ ) {
        if( ov264x_sizes[i].width == width ) {
            if( ov264x_sizes[i].height != heigth ) {
                TRACE_ERROR("ov264x_configure vsize not define\n\r");
            }
            else {
                goodCaptureSize = 1;
                break;
            }
        }
    }

    if( goodCaptureSize == 0 ) {
        TRACE_ERROR("Problem size\n\r");
        //while(1);
        return;
    }

    // Default value
    reg_conf = ov2640_yuv_vga;
   
    // common register initialization
    switch(width) {
        case 1280: //SXGA
            TRACE_DEBUG("SXGA\n\r");
            reg_conf = ov2640_yuv_sxga;
        break;
        case 640: //VGA
            TRACE_DEBUG("VGA\n\r");
            reg_conf = ov2640_yuv_vga;
        break;
        case 352: //CIF
            TRACE_DEBUG("CIF\n\r");
            reg_conf = ov2640_yuv_cif;
        break;
        case 320: //QVGA
            TRACE_DEBUG("QVGA\n\r");
            reg_conf = ov2640_yuv_qvga;
        break;
        case 176: //QCIF
            TRACE_DEBUG("QCIF\n\r");
            reg_conf = ov2640_yuv_qcif;
        break;

        default:  
            TRACE_DEBUG("ov264x_configure problem\n\r");
        break;
    }
    ov_write_regs(pTwid, reg_conf);
}

