/*
*
* Copyright 2015 Rockchip Electronics Co. LTD
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#define MODULE_TAG "hal_h264d_rkv_reg"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_type.h"
#include "mpp_err.h"
#include "mpp_mem.h"

#include "h264d_log.h"
#include "hal_h264d_global.h"
#include "hal_h264d_rkv_pkt.h"
#include "hal_h264d_rkv_reg.h"

const RK_U8 H264_Cabac_table[] = {
    0x14, 0xf1, 0x02, 0x36, 0x03, 0x4a, 0x14, 0xf1, 0x02, 0x36, 0x03, 0x4a, 0xe4, 0x7f, 0xe9, 0x68,
    0xfa, 0x35, 0xff, 0x36, 0x07, 0x33, 0x17, 0x21, 0x17, 0x02, 0x15, 0x00, 0x01, 0x09, 0x00, 0x31,
    0xdb, 0x76, 0x05, 0x39, 0xf3, 0x4e, 0xf5, 0x41, 0x01, 0x3e, 0x0c, 0x31, 0xfc, 0x49, 0x11, 0x32,
    0x12, 0x40, 0x09, 0x2b, 0x1d, 0x00, 0x1a, 0x43, 0x10, 0x5a, 0x09, 0x68, 0xd2, 0x7f, 0xec, 0x68,
    0x01, 0x43, 0xf3, 0x4e, 0xf5, 0x41, 0x01, 0x3e, 0xfa, 0x56, 0xef, 0x5f, 0xfa, 0x3d, 0x09, 0x2d,
    0xfd, 0x45, 0xfa, 0x51, 0xf5, 0x60, 0x06, 0x37, 0x07, 0x43, 0xfb, 0x56, 0x02, 0x58, 0x00, 0x3a,
    0xfd, 0x4c, 0xf6, 0x5e, 0x05, 0x36, 0x04, 0x45, 0xfd, 0x51, 0x00, 0x58, 0xf9, 0x43, 0xfb, 0x4a,
    0xfc, 0x4a, 0xfb, 0x50, 0xf9, 0x48, 0x01, 0x3a, 0x00, 0x29, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x3f,
    0xf7, 0x53, 0x04, 0x56, 0x00, 0x61, 0xf9, 0x48, 0x0d, 0x29, 0x03, 0x3e, 0x00, 0x2d, 0xfc, 0x4e,
    0xfd, 0x60, 0xe5, 0x7e, 0xe4, 0x62, 0xe7, 0x65, 0xe9, 0x43, 0xe4, 0x52, 0xec, 0x5e, 0xf0, 0x53,
    0xea, 0x6e, 0xeb, 0x5b, 0xee, 0x66, 0xf3, 0x5d, 0xe3, 0x7f, 0xf9, 0x5c, 0xfb, 0x59, 0xf9, 0x60,
    0xf3, 0x6c, 0xfd, 0x2e, 0xff, 0x41, 0xff, 0x39, 0xf7, 0x5d, 0xfd, 0x4a, 0xf7, 0x5c, 0xf8, 0x57,
    0xe9, 0x7e, 0x05, 0x36, 0x06, 0x3c, 0x06, 0x3b, 0x06, 0x45, 0xff, 0x30, 0x00, 0x44, 0xfc, 0x45,
    0xf8, 0x58, 0xfe, 0x55, 0xfa, 0x4e, 0xff, 0x4b, 0xf9, 0x4d, 0x02, 0x36, 0x05, 0x32, 0xfd, 0x44,
    0x01, 0x32, 0x06, 0x2a, 0xfc, 0x51, 0x01, 0x3f, 0xfc, 0x46, 0x00, 0x43, 0x02, 0x39, 0xfe, 0x4c,
    0x0b, 0x23, 0x04, 0x40, 0x01, 0x3d, 0x0b, 0x23, 0x12, 0x19, 0x0c, 0x18, 0x0d, 0x1d, 0x0d, 0x24,
    0xf6, 0x5d, 0xf9, 0x49, 0xfe, 0x49, 0x0d, 0x2e, 0x09, 0x31, 0xf9, 0x64, 0x09, 0x35, 0x02, 0x35,
    0x05, 0x35, 0xfe, 0x3d, 0x00, 0x38, 0x00, 0x38, 0xf3, 0x3f, 0xfb, 0x3c, 0xff, 0x3e, 0x04, 0x39,
    0xfa, 0x45, 0x04, 0x39, 0x0e, 0x27, 0x04, 0x33, 0x0d, 0x44, 0x03, 0x40, 0x01, 0x3d, 0x09, 0x3f,
    0x07, 0x32, 0x10, 0x27, 0x05, 0x2c, 0x04, 0x34, 0x0b, 0x30, 0xfb, 0x3c, 0xff, 0x3b, 0x00, 0x3b,
    0x16, 0x21, 0x05, 0x2c, 0x0e, 0x2b, 0xff, 0x4e, 0x00, 0x3c, 0x09, 0x45, 0x0b, 0x1c, 0x02, 0x28,
    0x03, 0x2c, 0x00, 0x31, 0x00, 0x2e, 0x02, 0x2c, 0x02, 0x33, 0x00, 0x2f, 0x04, 0x27, 0x02, 0x3e,
    0x06, 0x2e, 0x00, 0x36, 0x03, 0x36, 0x02, 0x3a, 0x04, 0x3f, 0x06, 0x33, 0x06, 0x39, 0x07, 0x35,
    0x06, 0x34, 0x06, 0x37, 0x0b, 0x2d, 0x0e, 0x24, 0x08, 0x35, 0xff, 0x52, 0x07, 0x37, 0xfd, 0x4e,
    0x0f, 0x2e, 0x16, 0x1f, 0xff, 0x54, 0x19, 0x07, 0x1e, 0xf9, 0x1c, 0x03, 0x1c, 0x04, 0x20, 0x00,
    0x22, 0xff, 0x1e, 0x06, 0x1e, 0x06, 0x20, 0x09, 0x1f, 0x13, 0x1a, 0x1b, 0x1a, 0x1e, 0x25, 0x14,
    0x1c, 0x22, 0x11, 0x46, 0x01, 0x43, 0x05, 0x3b, 0x09, 0x43, 0x10, 0x1e, 0x12, 0x20, 0x12, 0x23,
    0x16, 0x1d, 0x18, 0x1f, 0x17, 0x26, 0x12, 0x2b, 0x14, 0x29, 0x0b, 0x3f, 0x09, 0x3b, 0x09, 0x40,
    0xff, 0x5e, 0xfe, 0x59, 0xf7, 0x6c, 0xfa, 0x4c, 0xfe, 0x2c, 0x00, 0x2d, 0x00, 0x34, 0xfd, 0x40,
    0xfe, 0x3b, 0xfc, 0x46, 0xfc, 0x4b, 0xf8, 0x52, 0xef, 0x66, 0xf7, 0x4d, 0x03, 0x18, 0x00, 0x2a,
    0x00, 0x30, 0x00, 0x37, 0xfa, 0x3b, 0xf9, 0x47, 0xf4, 0x53, 0xf5, 0x57, 0xe2, 0x77, 0x01, 0x3a,
    0xfd, 0x1d, 0xff, 0x24, 0x01, 0x26, 0x02, 0x2b, 0xfa, 0x37, 0x00, 0x3a, 0x00, 0x40, 0xfd, 0x4a,
    0xf6, 0x5a, 0x00, 0x46, 0xfc, 0x1d, 0x05, 0x1f, 0x07, 0x2a, 0x01, 0x3b, 0xfe, 0x3a, 0xfd, 0x48,
    0xfd, 0x51, 0xf5, 0x61, 0x00, 0x3a, 0x08, 0x05, 0x0a, 0x0e, 0x0e, 0x12, 0x0d, 0x1b, 0x02, 0x28,
    0x00, 0x3a, 0xfd, 0x46, 0xfa, 0x4f, 0xf8, 0x55, 0x00, 0x00, 0xf3, 0x6a, 0xf0, 0x6a, 0xf6, 0x57,
    0xeb, 0x72, 0xee, 0x6e, 0xf2, 0x62, 0xea, 0x6e, 0xeb, 0x6a, 0xee, 0x67, 0xeb, 0x6b, 0xe9, 0x6c,
    0xe6, 0x70, 0xf6, 0x60, 0xf4, 0x5f, 0xfb, 0x5b, 0xf7, 0x5d, 0xea, 0x5e, 0xfb, 0x56, 0x09, 0x43,
    0xfc, 0x50, 0xf6, 0x55, 0xff, 0x46, 0x07, 0x3c, 0x09, 0x3a, 0x05, 0x3d, 0x0c, 0x32, 0x0f, 0x32,
    0x12, 0x31, 0x11, 0x36, 0x0a, 0x29, 0x07, 0x2e, 0xff, 0x33, 0x07, 0x31, 0x08, 0x34, 0x09, 0x29,
    0x06, 0x2f, 0x02, 0x37, 0x0d, 0x29, 0x0a, 0x2c, 0x06, 0x32, 0x05, 0x35, 0x0d, 0x31, 0x04, 0x3f,
    0x06, 0x40, 0xfe, 0x45, 0xfe, 0x3b, 0x06, 0x46, 0x0a, 0x2c, 0x09, 0x1f, 0x0c, 0x2b, 0x03, 0x35,
    0x0e, 0x22, 0x0a, 0x26, 0xfd, 0x34, 0x0d, 0x28, 0x11, 0x20, 0x07, 0x2c, 0x07, 0x26, 0x0d, 0x32,
    0x0a, 0x39, 0x1a, 0x2b, 0x0e, 0x0b, 0x0b, 0x0e, 0x09, 0x0b, 0x12, 0x0b, 0x15, 0x09, 0x17, 0xfe,
    0x20, 0xf1, 0x20, 0xf1, 0x22, 0xeb, 0x27, 0xe9, 0x2a, 0xdf, 0x29, 0xe1, 0x2e, 0xe4, 0x26, 0xf4,
    0x15, 0x1d, 0x2d, 0xe8, 0x35, 0xd3, 0x30, 0xe6, 0x41, 0xd5, 0x2b, 0xed, 0x27, 0xf6, 0x1e, 0x09,
    0x12, 0x1a, 0x14, 0x1b, 0x00, 0x39, 0xf2, 0x52, 0xfb, 0x4b, 0xed, 0x61, 0xdd, 0x7d, 0x1b, 0x00,
    0x1c, 0x00, 0x1f, 0xfc, 0x1b, 0x06, 0x22, 0x08, 0x1e, 0x0a, 0x18, 0x16, 0x21, 0x13, 0x16, 0x20,
    0x1a, 0x1f, 0x15, 0x29, 0x1a, 0x2c, 0x17, 0x2f, 0x10, 0x41, 0x0e, 0x47, 0x08, 0x3c, 0x06, 0x3f,
    0x11, 0x41, 0x15, 0x18, 0x17, 0x14, 0x1a, 0x17, 0x1b, 0x20, 0x1c, 0x17, 0x1c, 0x18, 0x17, 0x28,
    0x18, 0x20, 0x1c, 0x1d, 0x17, 0x2a, 0x13, 0x39, 0x16, 0x35, 0x16, 0x3d, 0x0b, 0x56, 0x0c, 0x28,
    0x0b, 0x33, 0x0e, 0x3b, 0xfc, 0x4f, 0xf9, 0x47, 0xfb, 0x45, 0xf7, 0x46, 0xf8, 0x42, 0xf6, 0x44,
    0xed, 0x49, 0xf4, 0x45, 0xf0, 0x46, 0xf1, 0x43, 0xec, 0x3e, 0xed, 0x46, 0xf0, 0x42, 0xea, 0x41,
    0xec, 0x3f, 0x09, 0xfe, 0x1a, 0xf7, 0x21, 0xf7, 0x27, 0xf9, 0x29, 0xfe, 0x2d, 0x03, 0x31, 0x09,
    0x2d, 0x1b, 0x24, 0x3b, 0xfa, 0x42, 0xf9, 0x23, 0xf9, 0x2a, 0xf8, 0x2d, 0xfb, 0x30, 0xf4, 0x38,
    0xfa, 0x3c, 0xfb, 0x3e, 0xf8, 0x42, 0xf8, 0x4c, 0xfb, 0x55, 0xfa, 0x51, 0xf6, 0x4d, 0xf9, 0x51,
    0xef, 0x50, 0xee, 0x49, 0xfc, 0x4a, 0xf6, 0x53, 0xf7, 0x47, 0xf7, 0x43, 0xff, 0x3d, 0xf8, 0x42,
    0xf2, 0x42, 0x00, 0x3b, 0x02, 0x3b, 0x15, 0xf3, 0x21, 0xf2, 0x27, 0xf9, 0x2e, 0xfe, 0x33, 0x02,
    0x3c, 0x06, 0x3d, 0x11, 0x37, 0x22, 0x2a, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x14, 0xf1, 0x02, 0x36, 0x03, 0x4a, 0x14, 0xf1, 0x02, 0x36, 0x03, 0x4a, 0xe4, 0x7f, 0xe9, 0x68,
    0xfa, 0x35, 0xff, 0x36, 0x07, 0x33, 0x16, 0x19, 0x22, 0x00, 0x10, 0x00, 0xfe, 0x09, 0x04, 0x29,
    0xe3, 0x76, 0x02, 0x41, 0xfa, 0x47, 0xf3, 0x4f, 0x05, 0x34, 0x09, 0x32, 0xfd, 0x46, 0x0a, 0x36,
    0x1a, 0x22, 0x13, 0x16, 0x28, 0x00, 0x39, 0x02, 0x29, 0x24, 0x1a, 0x45, 0xd3, 0x7f, 0xf1, 0x65,
    0xfc, 0x4c, 0xfa, 0x47, 0xf3, 0x4f, 0x05, 0x34, 0x06, 0x45, 0xf3, 0x5a, 0x00, 0x34, 0x08, 0x2b,
    0xfe, 0x45, 0xfb, 0x52, 0xf6, 0x60, 0x02, 0x3b, 0x02, 0x4b, 0xfd, 0x57, 0xfd, 0x64, 0x01, 0x38,
    0xfd, 0x4a, 0xfa, 0x55, 0x00, 0x3b, 0xfd, 0x51, 0xf9, 0x56, 0xfb, 0x5f, 0xff, 0x42, 0xff, 0x4d,
    0x01, 0x46, 0xfe, 0x56, 0xfb, 0x48, 0x00, 0x3d, 0x00, 0x29, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x3f,
    0xf7, 0x53, 0x04, 0x56, 0x00, 0x61, 0xf9, 0x48, 0x0d, 0x29, 0x03, 0x3e, 0x0d, 0x0f, 0x07, 0x33,
    0x02, 0x50, 0xd9, 0x7f, 0xee, 0x5b, 0xef, 0x60, 0xe6, 0x51, 0xdd, 0x62, 0xe8, 0x66, 0xe9, 0x61,
    0xe5, 0x77, 0xe8, 0x63, 0xeb, 0x6e, 0xee, 0x66, 0xdc, 0x7f, 0x00, 0x50, 0xfb, 0x59, 0xf9, 0x5e,
    0xfc, 0x5c, 0x00, 0x27, 0x00, 0x41, 0xf1, 0x54, 0xdd, 0x7f, 0xfe, 0x49, 0xf4, 0x68, 0xf7, 0x5b,
    0xe1, 0x7f, 0x03, 0x37, 0x07, 0x38, 0x07, 0x37, 0x08, 0x3d, 0xfd, 0x35, 0x00, 0x44, 0xf9, 0x4a,
    0xf7, 0x58, 0xf3, 0x67, 0xf3, 0x5b, 0xf7, 0x59, 0xf2, 0x5c, 0xf8, 0x4c, 0xf4, 0x57, 0xe9, 0x6e,
    0xe8, 0x69, 0xf6, 0x4e, 0xec, 0x70, 0xef, 0x63, 0xb2, 0x7f, 0xba, 0x7f, 0xce, 0x7f, 0xd2, 0x7f,
    0xfc, 0x42, 0xfb, 0x4e, 0xfc, 0x47, 0xf8, 0x48, 0x02, 0x3b, 0xff, 0x37, 0xf9, 0x46, 0xfa, 0x4b,
    0xf8, 0x59, 0xde, 0x77, 0xfd, 0x4b, 0x20, 0x14, 0x1e, 0x16, 0xd4, 0x7f, 0x00, 0x36, 0xfb, 0x3d,
    0x00, 0x3a, 0xff, 0x3c, 0xfd, 0x3d, 0xf8, 0x43, 0xe7, 0x54, 0xf2, 0x4a, 0xfb, 0x41, 0x05, 0x34,
    0x02, 0x39, 0x00, 0x3d, 0xf7, 0x45, 0xf5, 0x46, 0x12, 0x37, 0xfc, 0x47, 0x00, 0x3a, 0x07, 0x3d,
    0x09, 0x29, 0x12, 0x19, 0x09, 0x20, 0x05, 0x2b, 0x09, 0x2f, 0x00, 0x2c, 0x00, 0x33, 0x02, 0x2e,
    0x13, 0x26, 0xfc, 0x42, 0x0f, 0x26, 0x0c, 0x2a, 0x09, 0x22, 0x00, 0x59, 0x04, 0x2d, 0x0a, 0x1c,
    0x0a, 0x1f, 0x21, 0xf5, 0x34, 0xd5, 0x12, 0x0f, 0x1c, 0x00, 0x23, 0xea, 0x26, 0xe7, 0x22, 0x00,
    0x27, 0xee, 0x20, 0xf4, 0x66, 0xa2, 0x00, 0x00, 0x38, 0xf1, 0x21, 0xfc, 0x1d, 0x0a, 0x25, 0xfb,
    0x33, 0xe3, 0x27, 0xf7, 0x34, 0xde, 0x45, 0xc6, 0x43, 0xc1, 0x2c, 0xfb, 0x20, 0x07, 0x37, 0xe3,
    0x20, 0x01, 0x00, 0x00, 0x1b, 0x24, 0x21, 0xe7, 0x22, 0xe2, 0x24, 0xe4, 0x26, 0xe4, 0x26, 0xe5,
    0x22, 0xee, 0x23, 0xf0, 0x22, 0xf2, 0x20, 0xf8, 0x25, 0xfa, 0x23, 0x00, 0x1e, 0x0a, 0x1c, 0x12,
    0x1a, 0x19, 0x1d, 0x29, 0x00, 0x4b, 0x02, 0x48, 0x08, 0x4d, 0x0e, 0x23, 0x12, 0x1f, 0x11, 0x23,
    0x15, 0x1e, 0x11, 0x2d, 0x14, 0x2a, 0x12, 0x2d, 0x1b, 0x1a, 0x10, 0x36, 0x07, 0x42, 0x10, 0x38,
    0x0b, 0x49, 0x0a, 0x43, 0xf6, 0x74, 0xe9, 0x70, 0xf1, 0x47, 0xf9, 0x3d, 0x00, 0x35, 0xfb, 0x42,
    0xf5, 0x4d, 0xf7, 0x50, 0xf7, 0x54, 0xf6, 0x57, 0xde, 0x7f, 0xeb, 0x65, 0xfd, 0x27, 0xfb, 0x35,
    0xf9, 0x3d, 0xf5, 0x4b, 0xf1, 0x4d, 0xef, 0x5b, 0xe7, 0x6b, 0xe7, 0x6f, 0xe4, 0x7a, 0xf5, 0x4c,
    0xf6, 0x2c, 0xf6, 0x34, 0xf6, 0x39, 0xf7, 0x3a, 0xf0, 0x48, 0xf9, 0x45, 0xfc, 0x45, 0xfb, 0x4a,
    0xf7, 0x56, 0x02, 0x42, 0xf7, 0x22, 0x01, 0x20, 0x0b, 0x1f, 0x05, 0x34, 0xfe, 0x37, 0xfe, 0x43,
    0x00, 0x49, 0xf8, 0x59, 0x03, 0x34, 0x07, 0x04, 0x0a, 0x08, 0x11, 0x08, 0x10, 0x13, 0x03, 0x25,
    0xff, 0x3d, 0xfb, 0x49, 0xff, 0x46, 0xfc, 0x4e, 0x00, 0x00, 0xeb, 0x7e, 0xe9, 0x7c, 0xec, 0x6e,
    0xe6, 0x7e, 0xe7, 0x7c, 0xef, 0x69, 0xe5, 0x79, 0xe5, 0x75, 0xef, 0x66, 0xe6, 0x75, 0xe5, 0x74,
    0xdf, 0x7a, 0xf6, 0x5f, 0xf2, 0x64, 0xf8, 0x5f, 0xef, 0x6f, 0xe4, 0x72, 0xfa, 0x59, 0xfe, 0x50,
    0xfc, 0x52, 0xf7, 0x55, 0xf8, 0x51, 0xff, 0x48, 0x05, 0x40, 0x01, 0x43, 0x09, 0x38, 0x00, 0x45,
    0x01, 0x45, 0x07, 0x45, 0xf9, 0x45, 0xfa, 0x43, 0xf0, 0x4d, 0xfe, 0x40, 0x02, 0x3d, 0xfa, 0x43,
    0xfd, 0x40, 0x02, 0x39, 0xfd, 0x41, 0xfd, 0x42, 0x00, 0x3e, 0x09, 0x33, 0xff, 0x42, 0xfe, 0x47,
    0xfe, 0x4b, 0xff, 0x46, 0xf7, 0x48, 0x0e, 0x3c, 0x10, 0x25, 0x00, 0x2f, 0x12, 0x23, 0x0b, 0x25,
    0x0c, 0x29, 0x0a, 0x29, 0x02, 0x30, 0x0c, 0x29, 0x0d, 0x29, 0x00, 0x3b, 0x03, 0x32, 0x13, 0x28,
    0x03, 0x42, 0x12, 0x32, 0x13, 0xfa, 0x12, 0xfa, 0x0e, 0x00, 0x1a, 0xf4, 0x1f, 0xf0, 0x21, 0xe7,
    0x21, 0xea, 0x25, 0xe4, 0x27, 0xe2, 0x2a, 0xe2, 0x2f, 0xd6, 0x2d, 0xdc, 0x31, 0xde, 0x29, 0xef,
    0x20, 0x09, 0x45, 0xb9, 0x3f, 0xc1, 0x42, 0xc0, 0x4d, 0xb6, 0x36, 0xd9, 0x34, 0xdd, 0x29, 0xf6,
    0x24, 0x00, 0x28, 0xff, 0x1e, 0x0e, 0x1c, 0x1a, 0x17, 0x25, 0x0c, 0x37, 0x0b, 0x41, 0x25, 0xdf,
    0x27, 0xdc, 0x28, 0xdb, 0x26, 0xe2, 0x2e, 0xdf, 0x2a, 0xe2, 0x28, 0xe8, 0x31, 0xe3, 0x26, 0xf4,
    0x28, 0xf6, 0x26, 0xfd, 0x2e, 0xfb, 0x1f, 0x14, 0x1d, 0x1e, 0x19, 0x2c, 0x0c, 0x30, 0x0b, 0x31,
    0x1a, 0x2d, 0x16, 0x16, 0x17, 0x16, 0x1b, 0x15, 0x21, 0x14, 0x1a, 0x1c, 0x1e, 0x18, 0x1b, 0x22,
    0x12, 0x2a, 0x19, 0x27, 0x12, 0x32, 0x0c, 0x46, 0x15, 0x36, 0x0e, 0x47, 0x0b, 0x53, 0x19, 0x20,
    0x15, 0x31, 0x15, 0x36, 0xfb, 0x55, 0xfa, 0x51, 0xf6, 0x4d, 0xf9, 0x51, 0xef, 0x50, 0xee, 0x49,
    0xfc, 0x4a, 0xf6, 0x53, 0xf7, 0x47, 0xf7, 0x43, 0xff, 0x3d, 0xf8, 0x42, 0xf2, 0x42, 0x00, 0x3b,
    0x02, 0x3b, 0x11, 0xf6, 0x20, 0xf3, 0x2a, 0xf7, 0x31, 0xfb, 0x35, 0x00, 0x40, 0x03, 0x44, 0x0a,
    0x42, 0x1b, 0x2f, 0x39, 0xfb, 0x47, 0x00, 0x18, 0xff, 0x24, 0xfe, 0x2a, 0xfe, 0x34, 0xf7, 0x39,
    0xfa, 0x3f, 0xfc, 0x41, 0xfc, 0x43, 0xf9, 0x52, 0xfd, 0x51, 0xfd, 0x4c, 0xf9, 0x48, 0xfa, 0x4e,
    0xf4, 0x48, 0xf2, 0x44, 0xfd, 0x46, 0xfa, 0x4c, 0xfb, 0x42, 0xfb, 0x3e, 0x00, 0x39, 0xfc, 0x3d,
    0xf7, 0x3c, 0x01, 0x36, 0x02, 0x3a, 0x11, 0xf6, 0x20, 0xf3, 0x2a, 0xf7, 0x31, 0xfb, 0x35, 0x00,
    0x40, 0x03, 0x44, 0x0a, 0x42, 0x1b, 0x2f, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x14, 0xf1, 0x02, 0x36, 0x03, 0x4a, 0x14, 0xf1, 0x02, 0x36, 0x03, 0x4a, 0xe4, 0x7f, 0xe9, 0x68,
    0xfa, 0x35, 0xff, 0x36, 0x07, 0x33, 0x1d, 0x10, 0x19, 0x00, 0x0e, 0x00, 0xf6, 0x33, 0xfd, 0x3e,
    0xe5, 0x63, 0x1a, 0x10, 0xfc, 0x55, 0xe8, 0x66, 0x05, 0x39, 0x06, 0x39, 0xef, 0x49, 0x0e, 0x39,
    0x14, 0x28, 0x14, 0x0a, 0x1d, 0x00, 0x36, 0x00, 0x25, 0x2a, 0x0c, 0x61, 0xe0, 0x7f, 0xea, 0x75,
    0xfe, 0x4a, 0xfc, 0x55, 0xe8, 0x66, 0x05, 0x39, 0xfa, 0x5d, 0xf2, 0x58, 0xfa, 0x2c, 0x04, 0x37,
    0xf5, 0x59, 0xf1, 0x67, 0xeb, 0x74, 0x13, 0x39, 0x14, 0x3a, 0x04, 0x54, 0x06, 0x60, 0x01, 0x3f,
    0xfb, 0x55, 0xf3, 0x6a, 0x05, 0x3f, 0x06, 0x4b, 0xfd, 0x5a, 0xff, 0x65, 0x03, 0x37, 0xfc, 0x4f,
    0xfe, 0x4b, 0xf4, 0x61, 0xf9, 0x32, 0x01, 0x3c, 0x00, 0x29, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x3f,
    0xf7, 0x53, 0x04, 0x56, 0x00, 0x61, 0xf9, 0x48, 0x0d, 0x29, 0x03, 0x3e, 0x07, 0x22, 0xf7, 0x58,
    0xec, 0x7f, 0xdc, 0x7f, 0xef, 0x5b, 0xf2, 0x5f, 0xe7, 0x54, 0xe7, 0x56, 0xf4, 0x59, 0xef, 0x5b,
    0xe1, 0x7f, 0xf2, 0x4c, 0xee, 0x67, 0xf3, 0x5a, 0xdb, 0x7f, 0x0b, 0x50, 0x05, 0x4c, 0x02, 0x54,
    0x05, 0x4e, 0xfa, 0x37, 0x04, 0x3d, 0xf2, 0x53, 0xdb, 0x7f, 0xfb, 0x4f, 0xf5, 0x68, 0xf5, 0x5b,
    0xe2, 0x7f, 0x00, 0x41, 0xfe, 0x4f, 0x00, 0x48, 0xfc, 0x5c, 0xfa, 0x38, 0x03, 0x44, 0xf8, 0x47,
    0xf3, 0x62, 0xfc, 0x56, 0xf4, 0x58, 0xfb, 0x52, 0xfd, 0x48, 0xfc, 0x43, 0xf8, 0x48, 0xf0, 0x59,
    0xf7, 0x45, 0xff, 0x3b, 0x05, 0x42, 0x04, 0x39, 0xfc, 0x47, 0xfe, 0x47, 0x02, 0x3a, 0xff, 0x4a,
    0xfc, 0x2c, 0xff, 0x45, 0x00, 0x3e, 0xf9, 0x33, 0xfc, 0x2f, 0xfa, 0x2a, 0xfd, 0x29, 0xfa, 0x35,
    0x08, 0x4c, 0xf7, 0x4e, 0xf5, 0x53, 0x09, 0x34, 0x00, 0x43, 0xfb, 0x5a, 0x01, 0x43, 0xf1, 0x48,
    0xfb, 0x4b, 0xf8, 0x50, 0xeb, 0x53, 0xeb, 0x40, 0xf3, 0x1f, 0xe7, 0x40, 0xe3, 0x5e, 0x09, 0x4b,
    0x11, 0x3f, 0xf8, 0x4a, 0xfb, 0x23, 0xfe, 0x1b, 0x0d, 0x5b, 0x03, 0x41, 0xf9, 0x45, 0x08, 0x4d,
    0xf6, 0x42, 0x03, 0x3e, 0xfd, 0x44, 0xec, 0x51, 0x00, 0x1e, 0x01, 0x07, 0xfd, 0x17, 0xeb, 0x4a,
    0x10, 0x42, 0xe9, 0x7c, 0x11, 0x25, 0x2c, 0xee, 0x32, 0xde, 0xea, 0x7f, 0x04, 0x27, 0x00, 0x2a,
    0x07, 0x22, 0x0b, 0x1d, 0x08, 0x1f, 0x06, 0x25, 0x07, 0x2a, 0x03, 0x28, 0x08, 0x21, 0x0d, 0x2b,
    0x0d, 0x24, 0x04, 0x2f, 0x03, 0x37, 0x02, 0x3a, 0x06, 0x3c, 0x08, 0x2c, 0x0b, 0x2c, 0x0e, 0x2a,
    0x07, 0x30, 0x04, 0x38, 0x04, 0x34, 0x0d, 0x25, 0x09, 0x31, 0x13, 0x3a, 0x0a, 0x30, 0x0c, 0x2d,
    0x00, 0x45, 0x14, 0x21, 0x08, 0x3f, 0x23, 0xee, 0x21, 0xe7, 0x1c, 0xfd, 0x18, 0x0a, 0x1b, 0x00,
    0x22, 0xf2, 0x34, 0xd4, 0x27, 0xe8, 0x13, 0x11, 0x1f, 0x19, 0x24, 0x1d, 0x18, 0x21, 0x22, 0x0f,
    0x1e, 0x14, 0x16, 0x49, 0x14, 0x22, 0x13, 0x1f, 0x1b, 0x2c, 0x13, 0x10, 0x0f, 0x24, 0x0f, 0x24,
    0x15, 0x1c, 0x19, 0x15, 0x1e, 0x14, 0x1f, 0x0c, 0x1b, 0x10, 0x18, 0x2a, 0x00, 0x5d, 0x0e, 0x38,
    0x0f, 0x39, 0x1a, 0x26, 0xe8, 0x7f, 0xe8, 0x73, 0xea, 0x52, 0xf7, 0x3e, 0x00, 0x35, 0x00, 0x3b,
    0xf2, 0x55, 0xf3, 0x59, 0xf3, 0x5e, 0xf5, 0x5c, 0xe3, 0x7f, 0xeb, 0x64, 0xf2, 0x39, 0xf4, 0x43,
    0xf5, 0x47, 0xf6, 0x4d, 0xeb, 0x55, 0xf0, 0x58, 0xe9, 0x68, 0xf1, 0x62, 0xdb, 0x7f, 0xf6, 0x52,
    0xf8, 0x30, 0xf8, 0x3d, 0xf8, 0x42, 0xf9, 0x46, 0xf2, 0x4b, 0xf6, 0x4f, 0xf7, 0x53, 0xf4, 0x5c,
    0xee, 0x6c, 0xfc, 0x4f, 0xea, 0x45, 0xf0, 0x4b, 0xfe, 0x3a, 0x01, 0x3a, 0xf3, 0x4e, 0xf7, 0x53,
    0xfc, 0x51, 0xf3, 0x63, 0xf3, 0x51, 0xfa, 0x26, 0xf3, 0x3e, 0xfa, 0x3a, 0xfe, 0x3b, 0xf0, 0x49,
    0xf6, 0x4c, 0xf3, 0x56, 0xf7, 0x53, 0xf6, 0x57, 0x00, 0x00, 0xea, 0x7f, 0xe7, 0x7f, 0xe7, 0x78,
    0xe5, 0x7f, 0xed, 0x72, 0xe9, 0x75, 0xe7, 0x76, 0xe6, 0x75, 0xe8, 0x71, 0xe4, 0x76, 0xe1, 0x78,
    0xdb, 0x7c, 0xf6, 0x5e, 0xf1, 0x66, 0xf6, 0x63, 0xf3, 0x6a, 0xce, 0x7f, 0xfb, 0x5c, 0x11, 0x39,
    0xfb, 0x56, 0xf3, 0x5e, 0xf4, 0x5b, 0xfe, 0x4d, 0x00, 0x47, 0xff, 0x49, 0x04, 0x40, 0xf9, 0x51,
    0x05, 0x40, 0x0f, 0x39, 0x01, 0x43, 0x00, 0x44, 0xf6, 0x43, 0x01, 0x44, 0x00, 0x4d, 0x02, 0x40,
    0x00, 0x44, 0xfb, 0x4e, 0x07, 0x37, 0x05, 0x3b, 0x02, 0x41, 0x0e, 0x36, 0x0f, 0x2c, 0x05, 0x3c,
    0x02, 0x46, 0xfe, 0x4c, 0xee, 0x56, 0x0c, 0x46, 0x05, 0x40, 0xf4, 0x46, 0x0b, 0x37, 0x05, 0x38,
    0x00, 0x45, 0x02, 0x41, 0xfa, 0x4a, 0x05, 0x36, 0x07, 0x36, 0xfa, 0x4c, 0xf5, 0x52, 0xfe, 0x4d,
    0xfe, 0x4d, 0x19, 0x2a, 0x11, 0xf3, 0x10, 0xf7, 0x11, 0xf4, 0x1b, 0xeb, 0x25, 0xe2, 0x29, 0xd8,
    0x2a, 0xd7, 0x30, 0xd1, 0x27, 0xe0, 0x2e, 0xd8, 0x34, 0xcd, 0x2e, 0xd7, 0x34, 0xd9, 0x2b, 0xed,
    0x20, 0x0b, 0x3d, 0xc9, 0x38, 0xd2, 0x3e, 0xce, 0x51, 0xbd, 0x2d, 0xec, 0x23, 0xfe, 0x1c, 0x0f,
    0x22, 0x01, 0x27, 0x01, 0x1e, 0x11, 0x14, 0x26, 0x12, 0x2d, 0x0f, 0x36, 0x00, 0x4f, 0x24, 0xf0,
    0x25, 0xf2, 0x25, 0xef, 0x20, 0x01, 0x22, 0x0f, 0x1d, 0x0f, 0x18, 0x19, 0x22, 0x16, 0x1f, 0x10,
    0x23, 0x12, 0x1f, 0x1c, 0x21, 0x29, 0x24, 0x1c, 0x1b, 0x2f, 0x15, 0x3e, 0x12, 0x1f, 0x13, 0x1a,
    0x24, 0x18, 0x18, 0x17, 0x1b, 0x10, 0x18, 0x1e, 0x1f, 0x1d, 0x16, 0x29, 0x16, 0x2a, 0x10, 0x3c,
    0x0f, 0x34, 0x0e, 0x3c, 0x03, 0x4e, 0xf0, 0x7b, 0x15, 0x35, 0x16, 0x38, 0x19, 0x3d, 0x15, 0x21,
    0x13, 0x32, 0x11, 0x3d, 0xfd, 0x4e, 0xf8, 0x4a, 0xf7, 0x48, 0xf6, 0x48, 0xee, 0x4b, 0xf4, 0x47,
    0xf5, 0x3f, 0xfb, 0x46, 0xef, 0x4b, 0xf2, 0x48, 0xf0, 0x43, 0xf8, 0x35, 0xf2, 0x3b, 0xf7, 0x34,
    0xf5, 0x44, 0x09, 0xfe, 0x1e, 0xf6, 0x1f, 0xfc, 0x21, 0xff, 0x21, 0x07, 0x1f, 0x0c, 0x25, 0x17,
    0x1f, 0x26, 0x14, 0x40, 0xf7, 0x47, 0xf9, 0x25, 0xf8, 0x2c, 0xf5, 0x31, 0xf6, 0x38, 0xf4, 0x3b,
    0xf8, 0x3f, 0xf7, 0x43, 0xfa, 0x44, 0xf6, 0x4f, 0xfd, 0x4e, 0xf8, 0x4a, 0xf7, 0x48, 0xf6, 0x48,
    0xee, 0x4b, 0xf4, 0x47, 0xf5, 0x3f, 0xfb, 0x46, 0xef, 0x4b, 0xf2, 0x48, 0xf0, 0x43, 0xf8, 0x35,
    0xf2, 0x3b, 0xf7, 0x34, 0xf5, 0x44, 0x09, 0xfe, 0x1e, 0xf6, 0x1f, 0xfc, 0x21, 0xff, 0x21, 0x07,
    0x1f, 0x0c, 0x25, 0x17, 0x1f, 0x26, 0x14, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x14, 0xf1, 0x02, 0x36, 0x03, 0x4a, 0x14, 0xf1, 0x02, 0x36, 0x03, 0x4a, 0xe4, 0x7f, 0xe9, 0x68,
    0xfa, 0x35, 0xff, 0x36, 0x07, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x3f,
    0xf7, 0x53, 0x04, 0x56, 0x00, 0x61, 0xf9, 0x48, 0x0d, 0x29, 0x03, 0x3e, 0x00, 0x0b, 0x01, 0x37,
    0x00, 0x45, 0xef, 0x7f, 0xf3, 0x66, 0x00, 0x52, 0xf9, 0x4a, 0xeb, 0x6b, 0xe5, 0x7f, 0xe1, 0x7f,
    0xe8, 0x7f, 0xee, 0x5f, 0xe5, 0x7f, 0xeb, 0x72, 0xe2, 0x7f, 0xef, 0x7b, 0xf4, 0x73, 0xf0, 0x7a,
    0xf5, 0x73, 0xf4, 0x3f, 0xfe, 0x44, 0xf1, 0x54, 0xf3, 0x68, 0xfd, 0x46, 0xf8, 0x5d, 0xf6, 0x5a,
    0xe2, 0x7f, 0xff, 0x4a, 0xfa, 0x61, 0xf9, 0x5b, 0xec, 0x7f, 0xfc, 0x38, 0xfb, 0x52, 0xf9, 0x4c,
    0xea, 0x7d, 0xf9, 0x5d, 0xf5, 0x57, 0xfd, 0x4d, 0xfb, 0x47, 0xfc, 0x3f, 0xfc, 0x44, 0xf4, 0x54,
    0xf9, 0x3e, 0xf9, 0x41, 0x08, 0x3d, 0x05, 0x38, 0xfe, 0x42, 0x01, 0x40, 0x00, 0x3d, 0xfe, 0x4e,
    0x01, 0x32, 0x07, 0x34, 0x0a, 0x23, 0x00, 0x2c, 0x0b, 0x26, 0x01, 0x2d, 0x00, 0x2e, 0x05, 0x2c,
    0x1f, 0x11, 0x01, 0x33, 0x07, 0x32, 0x1c, 0x13, 0x10, 0x21, 0x0e, 0x3e, 0xf3, 0x6c, 0xf1, 0x64,
    0xf3, 0x65, 0xf3, 0x5b, 0xf4, 0x5e, 0xf6, 0x58, 0xf0, 0x54, 0xf6, 0x56, 0xf9, 0x53, 0xf3, 0x57,
    0xed, 0x5e, 0x01, 0x46, 0x00, 0x48, 0xfb, 0x4a, 0x12, 0x3b, 0xf8, 0x66, 0xf1, 0x64, 0x00, 0x5f,
    0xfc, 0x4b, 0x02, 0x48, 0xf5, 0x4b, 0xfd, 0x47, 0x0f, 0x2e, 0xf3, 0x45, 0x00, 0x3e, 0x00, 0x41,
    0x15, 0x25, 0xf1, 0x48, 0x09, 0x39, 0x10, 0x36, 0x00, 0x3e, 0x0c, 0x48, 0x18, 0x00, 0x0f, 0x09,
    0x08, 0x19, 0x0d, 0x12, 0x0f, 0x09, 0x0d, 0x13, 0x0a, 0x25, 0x0c, 0x12, 0x06, 0x1d, 0x14, 0x21,
    0x0f, 0x1e, 0x04, 0x2d, 0x01, 0x3a, 0x00, 0x3e, 0x07, 0x3d, 0x0c, 0x26, 0x0b, 0x2d, 0x0f, 0x27,
    0x0b, 0x2a, 0x0d, 0x2c, 0x10, 0x2d, 0x0c, 0x29, 0x0a, 0x31, 0x1e, 0x22, 0x12, 0x2a, 0x0a, 0x37,
    0x11, 0x33, 0x11, 0x2e, 0x00, 0x59, 0x1a, 0xed, 0x16, 0xef, 0x1a, 0xef, 0x1e, 0xe7, 0x1c, 0xec,
    0x21, 0xe9, 0x25, 0xe5, 0x21, 0xe9, 0x28, 0xe4, 0x26, 0xef, 0x21, 0xf5, 0x28, 0xf1, 0x29, 0xfa,
    0x26, 0x01, 0x29, 0x11, 0x1e, 0xfa, 0x1b, 0x03, 0x1a, 0x16, 0x25, 0xf0, 0x23, 0xfc, 0x26, 0xf8,
    0x26, 0xfd, 0x25, 0x03, 0x26, 0x05, 0x2a, 0x00, 0x23, 0x10, 0x27, 0x16, 0x0e, 0x30, 0x1b, 0x25,
    0x15, 0x3c, 0x0c, 0x44, 0x02, 0x61, 0xfd, 0x47, 0xfa, 0x2a, 0xfb, 0x32, 0xfd, 0x36, 0xfe, 0x3e,
    0x00, 0x3a, 0x01, 0x3f, 0xfe, 0x48, 0xff, 0x4a, 0xf7, 0x5b, 0xfb, 0x43, 0xfb, 0x1b, 0xfd, 0x27,
    0xfe, 0x2c, 0x00, 0x2e, 0xf0, 0x40, 0xf8, 0x44, 0xf6, 0x4e, 0xfa, 0x4d, 0xf6, 0x56, 0xf4, 0x5c,
    0xf1, 0x37, 0xf6, 0x3c, 0xfa, 0x3e, 0xfc, 0x41, 0xf4, 0x49, 0xf8, 0x4c, 0xf9, 0x50, 0xf7, 0x58,
    0xef, 0x6e, 0xf5, 0x61, 0xec, 0x54, 0xf5, 0x4f, 0xfa, 0x49, 0xfc, 0x4a, 0xf3, 0x56, 0xf3, 0x60,
    0xf5, 0x61, 0xed, 0x75, 0xf8, 0x4e, 0xfb, 0x21, 0xfc, 0x30, 0xfe, 0x35, 0xfd, 0x3e, 0xf3, 0x47,
    0xf6, 0x4f, 0xf4, 0x56, 0xf3, 0x5a, 0xf2, 0x61, 0x00, 0x00, 0xfa, 0x5d, 0xfa, 0x54, 0xf8, 0x4f,
    0x00, 0x42, 0xff, 0x47, 0x00, 0x3e, 0xfe, 0x3c, 0xfe, 0x3b, 0xfb, 0x4b, 0xfd, 0x3e, 0xfc, 0x3a,
    0xf7, 0x42, 0xff, 0x4f, 0x00, 0x47, 0x03, 0x44, 0x0a, 0x2c, 0xf9, 0x3e, 0x0f, 0x24, 0x0e, 0x28,
    0x10, 0x1b, 0x0c, 0x1d, 0x01, 0x2c, 0x14, 0x24, 0x12, 0x20, 0x05, 0x2a, 0x01, 0x30, 0x0a, 0x3e,
    0x11, 0x2e, 0x09, 0x40, 0xf4, 0x68, 0xf5, 0x61, 0xf0, 0x60, 0xf9, 0x58, 0xf8, 0x55, 0xf9, 0x55,
    0xf7, 0x55, 0xf3, 0x58, 0x04, 0x42, 0xfd, 0x4d, 0xfd, 0x4c, 0xfa, 0x4c, 0x0a, 0x3a, 0xff, 0x4c,
    0xff, 0x53, 0xf9, 0x63, 0xf2, 0x5f, 0x02, 0x5f, 0x00, 0x4c, 0xfb, 0x4a, 0x00, 0x46, 0xf5, 0x4b,
    0x01, 0x44, 0x00, 0x41, 0xf2, 0x49, 0x03, 0x3e, 0x04, 0x3e, 0xff, 0x44, 0xf3, 0x4b, 0x0b, 0x37,
    0x05, 0x40, 0x0c, 0x46, 0x0f, 0x06, 0x06, 0x13, 0x07, 0x10, 0x0c, 0x0e, 0x12, 0x0d, 0x0d, 0x0b,
    0x0d, 0x0f, 0x0f, 0x10, 0x0c, 0x17, 0x0d, 0x17, 0x0f, 0x14, 0x0e, 0x1a, 0x0e, 0x2c, 0x11, 0x28,
    0x11, 0x2f, 0x18, 0x11, 0x15, 0x15, 0x19, 0x16, 0x1f, 0x1b, 0x16, 0x1d, 0x13, 0x23, 0x0e, 0x32,
    0x0a, 0x39, 0x07, 0x3f, 0xfe, 0x4d, 0xfc, 0x52, 0xfd, 0x5e, 0x09, 0x45, 0xf4, 0x6d, 0x24, 0xdd,
    0x24, 0xde, 0x20, 0xe6, 0x25, 0xe2, 0x2c, 0xe0, 0x22, 0xee, 0x22, 0xf1, 0x28, 0xf1, 0x21, 0xf9,
    0x23, 0xfb, 0x21, 0x00, 0x26, 0x02, 0x21, 0x0d, 0x17, 0x23, 0x0d, 0x3a, 0x1d, 0xfd, 0x1a, 0x00,
    0x16, 0x1e, 0x1f, 0xf9, 0x23, 0xf1, 0x22, 0xfd, 0x22, 0x03, 0x24, 0xff, 0x22, 0x05, 0x20, 0x0b,
    0x23, 0x05, 0x22, 0x0c, 0x27, 0x0b, 0x1e, 0x1d, 0x22, 0x1a, 0x1d, 0x27, 0x13, 0x42, 0x1f, 0x15,
    0x1f, 0x1f, 0x19, 0x32, 0xef, 0x78, 0xec, 0x70, 0xee, 0x72, 0xf5, 0x55, 0xf1, 0x5c, 0xf2, 0x59,
    0xe6, 0x47, 0xf1, 0x51, 0xf2, 0x50, 0x00, 0x44, 0xf2, 0x46, 0xe8, 0x38, 0xe9, 0x44, 0xe8, 0x32,
    0xf5, 0x4a, 0x17, 0xf3, 0x1a, 0xf3, 0x28, 0xf1, 0x31, 0xf2, 0x2c, 0x03, 0x2d, 0x06, 0x2c, 0x22,
    0x21, 0x36, 0x13, 0x52, 0xfd, 0x4b, 0xff, 0x17, 0x01, 0x22, 0x01, 0x2b, 0x00, 0x36, 0xfe, 0x37,
    0x00, 0x3d, 0x01, 0x40, 0x00, 0x44, 0xf7, 0x5c, 0xf2, 0x6a, 0xf3, 0x61, 0xf1, 0x5a, 0xf4, 0x5a,
    0xee, 0x58, 0xf6, 0x49, 0xf7, 0x4f, 0xf2, 0x56, 0xf6, 0x49, 0xf6, 0x46, 0xf6, 0x45, 0xfb, 0x42,
    0xf7, 0x40, 0xfb, 0x3a, 0x02, 0x3b, 0x15, 0xf6, 0x18, 0xf5, 0x1c, 0xf8, 0x1c, 0xff, 0x1d, 0x03,
    0x1d, 0x09, 0x23, 0x14, 0x1d, 0x24, 0x0e, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


/*!
***********************************************************************
* \brief
*    init
***********************************************************************
*/
//extern "C"
MPP_RET rkv_h264d_init(void *hal, MppHalCfg *cfg)
{
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264dHalCtx_t *p_hal = (H264dHalCtx_t *)hal;

    INP_CHECK(ret, ctx, NULL == p_hal);

    FunctionIn(p_hal->logctx.parr[RUN_HAL]);
    p_hal->regs = (void *)mpp_calloc(H264_RkvRegs_t, 1);
    p_hal->pkts = (void *)mpp_calloc(H264dRkvPkt_t, 1);
    MEM_CHECK(ret, p_hal->regs && p_hal->pkts);
    FUN_CHECK(ret = alloc_fifo_packet(&p_hal->logctx, (H264dRkvPkt_t *)p_hal->pkts));

    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
    (void)cfg;
__RETURN:
    return MPP_OK;
__FAILED:
    rkv_h264d_deinit(hal);

    return ret;
}
/*!
***********************************************************************
* \brief
*    deinit
***********************************************************************
*/
//extern "C"
MPP_RET rkv_h264d_deinit(void *hal)
{
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264dHalCtx_t *p_hal = (H264dHalCtx_t *)hal;

    INP_CHECK(ret, ctx, NULL == p_hal);
    FunctionIn(p_hal->logctx.parr[RUN_HAL]);

    free_fifo_packet((H264dRkvPkt_t *)p_hal->pkts);
    MPP_FREE(p_hal->regs);
    MPP_FREE(p_hal->pkts);

    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
__RETURN:
    return ret = MPP_OK;
}
/*!
***********************************************************************
* \brief
*    generate register
***********************************************************************
*/
//extern "C"
MPP_RET rkv_h264d_gen_regs(void *hal, HalTaskInfo *task)
{
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264dHalCtx_t *p_hal = (H264dHalCtx_t *)hal;
    H264dRkvPkt_t *pkts  = (H264dRkvPkt_t *)p_hal->pkts;
    INP_CHECK(ret, ctx, NULL == p_hal);
    FunctionIn(p_hal->logctx.parr[RUN_HAL]);

    explain_input_buffer(hal, &task->dec);
    prepare_spspps_packet(hal, &pkts->spspps);
    prepare_framerps_packet(hal, &pkts->rps);
    prepare_scanlist_packet(hal, &pkts->scanlist);
    prepare_stream_packet(hal, &pkts->strm);
    generate_regs(p_hal, &pkts->reg);

    mpp_log("++++++++++ hal_h264_decoder, g_framecnt=%d \n", p_hal->g_framecnt++);
    ((HalDecTask*)&task->dec)->valid = 0;
    FunctionOut(p_hal->logctx.parr[RUN_HAL]);

__RETURN:
    return ret = MPP_OK;
}
/*!
***********************************************************************
* \brief h
*    start hard
***********************************************************************
*/
//extern "C"
MPP_RET rkv_h264d_start(void *hal, HalTaskInfo *task)
{
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264dHalCtx_t *p_hal = (H264dHalCtx_t *)hal;

    INP_CHECK(ret, ctx, NULL == p_hal);
    FunctionIn(p_hal->logctx.parr[RUN_HAL]);





    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
    (void)task;
__RETURN:
    return ret = MPP_OK;
}
/*!
***********************************************************************
* \brief
*    wait hard
***********************************************************************
*/
//extern "C"
MPP_RET rkv_h264d_wait(void *hal, HalTaskInfo *task)
{
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264dHalCtx_t *p_hal = (H264dHalCtx_t *)hal;

    INP_CHECK(ret, ctx, NULL == p_hal);
    FunctionIn(p_hal->logctx.parr[RUN_HAL]);





    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
    (void)task;
__RETURN:
    return ret = MPP_OK;
}
/*!
***********************************************************************
* \brief
*    reset
***********************************************************************
*/
//extern "C"
MPP_RET rkv_h264d_reset(void *hal)
{
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264dHalCtx_t *p_hal = (H264dHalCtx_t *)hal;

    INP_CHECK(ret, ctx, NULL == p_hal);
    FunctionIn(p_hal->logctx.parr[RUN_HAL]);

    memset(&p_hal->regs, 0, sizeof(H264_RkvRegs_t));
    reset_fifo_packet(p_hal->pkts);

    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
__RETURN:
    return ret = MPP_OK;
}
/*!
***********************************************************************
* \brief
*    flush
***********************************************************************
*/
//extern "C"
MPP_RET rkv_h264d_flush(void *hal)
{
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264dHalCtx_t *p_hal = (H264dHalCtx_t *)hal;

    INP_CHECK(ret, ctx, NULL == p_hal);
    FunctionIn(p_hal->logctx.parr[RUN_HAL]);





    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
__RETURN:
    return ret = MPP_OK;
}
/*!
***********************************************************************
* \brief
*    control
***********************************************************************
*/
//extern "C"
MPP_RET rkv_h264d_control(void *hal, RK_S32 cmd_type, void *param)
{
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264dHalCtx_t *p_hal = (H264dHalCtx_t *)hal;

    INP_CHECK(ret, ctx, NULL == p_hal);
    FunctionIn(p_hal->logctx.parr[RUN_HAL]);





    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
    (void)hal;
    (void)cmd_type;
    (void)param;
__RETURN:
    return ret = MPP_OK;
}