#include <stdio.h>
#include <string.h>
#include "soft_types.h"
#include "Crc/soft_crc.h"

/* 此文件中实现的都是以正序异或校验 */

#define CRC_16_POLYNOMIALS        0x1021

/* 0x31 */
static UCHAR ucCrc8_table_positive[256] =
{
    0x00,0x31,0x62,0x53,0xc4,0xf5,0xa6,0x97,0xb9,0x88,0xdb,0xea,0x7d,0x4c,0x1f,0x2e,
    0x43,0x72,0x21,0x10,0x87,0xb6,0xe5,0xd4,0xfa,0xcb,0x98,0xa9,0x3e,0x0f,0x5c,0x6d,
    0x86,0xb7,0xe4,0xd5,0x42,0x73,0x20,0x11,0x3f,0x0e,0x5d,0x6c,0xfb,0xca,0x99,0xa8,
    0xc5,0xf4,0xa7,0x96,0x01,0x30,0x63,0x52,0x7c,0x4d,0x1e,0x2f,0xb8,0x89,0xda,0xeb,
    0x3d,0x0c,0x5f,0x6e,0xf9,0xc8,0x9b,0xaa,0x84,0xb5,0xe6,0xd7,0x40,0x71,0x22,0x13,
    0x7e,0x4f,0x1c,0x2d,0xba,0x8b,0xd8,0xe9,0xc7,0xf6,0xa5,0x94,0x03,0x32,0x61,0x50,
    0xbb,0x8a,0xd9,0xe8,0x7f,0x4e,0x1d,0x2c,0x02,0x33,0x60,0x51,0xc6,0xf7,0xa4,0x95,
    0xf8,0xc9,0x9a,0xab,0x3c,0x0d,0x5e,0x6f,0x41,0x70,0x23,0x12,0x85,0xb4,0xe7,0xd6,
    0x7a,0x4b,0x18,0x29,0xbe,0x8f,0xdc,0xed,0xc3,0xf2,0xa1,0x90,0x07,0x36,0x65,0x54,
    0x39,0x08,0x5b,0x6a,0xfd,0xcc,0x9f,0xae,0x80,0xb1,0xe2,0xd3,0x44,0x75,0x26,0x17,
    0xfc,0xcd,0x9e,0xaf,0x38,0x09,0x5a,0x6b,0x45,0x74,0x27,0x16,0x81,0xb0,0xe3,0xd2,
    0xbf,0x8e,0xdd,0xec,0x7b,0x4a,0x19,0x28,0x06,0x37,0x64,0x55,0xc2,0xf3,0xa0,0x91,
    0x47,0x76,0x25,0x14,0x83,0xb2,0xe1,0xd0,0xfe,0xcf,0x9c,0xad,0x3a,0x0b,0x58,0x69,
    0x04,0x35,0x66,0x57,0xc0,0xf1,0xa2,0x93,0xbd,0x8c,0xdf,0xee,0x79,0x48,0x1b,0x2a,
    0xc1,0xf0,0xa3,0x92,0x05,0x34,0x67,0x56,0x78,0x49,0x1a,0x2b,0xbc,0x8d,0xde,0xef,
    0x82,0xb3,0xe0,0xd1,0x46,0x77,0x24,0x15,0x3b,0x0a,0x59,0x68,0xff,0xce,0x9d,0xac
};

static UCHAR ucCrc8_table_reverse[256] = {
    0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41, 
    0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc, 
    0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62, 
    0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff, 
    0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07, 
    0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a, 
    0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24, 
    0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9, 
    0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd, 
    0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50, 
    0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee, 
    0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73, 
    0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b, 
    0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16, 
    0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
    0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35
};

/* 0x1021 */
static USHORT usCrc16_table_positive[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

static USHORT usCrc16_table_reverse[256] = {
    0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
    0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
    0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
    0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
    0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
    0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
    0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
    0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
    0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
    0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
    0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
    0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
    0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
    0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
    0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
    0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
    0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
    0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
    0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
    0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
    0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
    0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
    0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
    0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
    0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
    0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
    0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
    0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
    0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
    0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
    0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
    0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1,0x0f78
};

/* CRC-32-IEEE 802.3 0x04C11DB7 */
static UINT ulCrc32_table_positive[] = {
    0x00000000L, 0x04c11db7L, 0x09823b6eL, 0x0d4326d9L,
    0x130476dcL, 0x17c56b6bL, 0x1a864db2L, 0x1e475005L,
    0x2608edb8L, 0x22c9f00fL, 0x2f8ad6d6L, 0x2b4bcb61L,
    0x350c9b64L, 0x31cd86d3L, 0x3c8ea00aL, 0x384fbdbdL,
    0x4c11db70L, 0x48d0c6c7L, 0x4593e01eL, 0x4152fda9L,
    0x5f15adacL, 0x5bd4b01bL, 0x569796c2L, 0x52568b75L,
    0x6a1936c8L, 0x6ed82b7fL, 0x639b0da6L, 0x675a1011L,
    0x791d4014L, 0x7ddc5da3L, 0x709f7b7aL, 0x745e66cdL,
    0x9823b6e0L, 0x9ce2ab57L, 0x91a18d8eL, 0x95609039L,
    0x8b27c03cL, 0x8fe6dd8bL, 0x82a5fb52L, 0x8664e6e5L,
    0xbe2b5b58L, 0xbaea46efL, 0xb7a96036L, 0xb3687d81L,
    0xad2f2d84L, 0xa9ee3033L, 0xa4ad16eaL, 0xa06c0b5dL,
    0xd4326d90L, 0xd0f37027L, 0xddb056feL, 0xd9714b49L,
    0xc7361b4cL, 0xc3f706fbL, 0xceb42022L, 0xca753d95L,
    0xf23a8028L, 0xf6fb9d9fL, 0xfbb8bb46L, 0xff79a6f1L,
    0xe13ef6f4L, 0xe5ffeb43L, 0xe8bccd9aL, 0xec7dd02dL,
    0x34867077L, 0x30476dc0L, 0x3d044b19L, 0x39c556aeL,
    0x278206abL, 0x23431b1cL, 0x2e003dc5L, 0x2ac12072L,
    0x128e9dcfL, 0x164f8078L, 0x1b0ca6a1L, 0x1fcdbb16L,
    0x018aeb13L, 0x054bf6a4L, 0x0808d07dL, 0x0cc9cdcaL,
    0x7897ab07L, 0x7c56b6b0L, 0x71159069L, 0x75d48ddeL,
    0x6b93dddbL, 0x6f52c06cL, 0x6211e6b5L, 0x66d0fb02L,
    0x5e9f46bfL, 0x5a5e5b08L, 0x571d7dd1L, 0x53dc6066L,
    0x4d9b3063L, 0x495a2dd4L, 0x44190b0dL, 0x40d816baL,
    0xaca5c697L, 0xa864db20L, 0xa527fdf9L, 0xa1e6e04eL,
    0xbfa1b04bL, 0xbb60adfcL, 0xb6238b25L, 0xb2e29692L,
    0x8aad2b2fL, 0x8e6c3698L, 0x832f1041L, 0x87ee0df6L,
    0x99a95df3L, 0x9d684044L, 0x902b669dL, 0x94ea7b2aL,
    0xe0b41de7L, 0xe4750050L, 0xe9362689L, 0xedf73b3eL,
    0xf3b06b3bL, 0xf771768cL, 0xfa325055L, 0xfef34de2L,
    0xc6bcf05fL, 0xc27dede8L, 0xcf3ecb31L, 0xcbffd686L,
    0xd5b88683L, 0xd1799b34L, 0xdc3abdedL, 0xd8fba05aL,
    0x690ce0eeL, 0x6dcdfd59L, 0x608edb80L, 0x644fc637L,
    0x7a089632L, 0x7ec98b85L, 0x738aad5cL, 0x774bb0ebL,
    0x4f040d56L, 0x4bc510e1L, 0x46863638L, 0x42472b8fL,
    0x5c007b8aL, 0x58c1663dL, 0x558240e4L, 0x51435d53L,
    0x251d3b9eL, 0x21dc2629L, 0x2c9f00f0L, 0x285e1d47L,
    0x36194d42L, 0x32d850f5L, 0x3f9b762cL, 0x3b5a6b9bL,
    0x0315d626L, 0x07d4cb91L, 0x0a97ed48L, 0x0e56f0ffL,
    0x1011a0faL, 0x14d0bd4dL, 0x19939b94L, 0x1d528623L,
    0xf12f560eL, 0xf5ee4bb9L, 0xf8ad6d60L, 0xfc6c70d7L,
    0xe22b20d2L, 0xe6ea3d65L, 0xeba91bbcL, 0xef68060bL,
    0xd727bbb6L, 0xd3e6a601L, 0xdea580d8L, 0xda649d6fL,
    0xc423cd6aL, 0xc0e2d0ddL, 0xcda1f604L, 0xc960ebb3L,
    0xbd3e8d7eL, 0xb9ff90c9L, 0xb4bcb610L, 0xb07daba7L,
    0xae3afba2L, 0xaafbe615L, 0xa7b8c0ccL, 0xa379dd7bL,
    0x9b3660c6L, 0x9ff77d71L, 0x92b45ba8L, 0x9675461fL,
    0x8832161aL, 0x8cf30badL, 0x81b02d74L, 0x857130c3L,
    0x5d8a9099L, 0x594b8d2eL, 0x5408abf7L, 0x50c9b640L,
    0x4e8ee645L, 0x4a4ffbf2L, 0x470cdd2bL, 0x43cdc09cL,
    0x7b827d21L, 0x7f436096L, 0x7200464fL, 0x76c15bf8L,
    0x68860bfdL, 0x6c47164aL, 0x61043093L, 0x65c52d24L,
    0x119b4be9L, 0x155a565eL, 0x18197087L, 0x1cd86d30L,
    0x029f3d35L, 0x065e2082L, 0x0b1d065bL, 0x0fdc1becL,
    0x3793a651L, 0x3352bbe6L, 0x3e119d3fL, 0x3ad08088L,
    0x2497d08dL, 0x2056cd3aL, 0x2d15ebe3L, 0x29d4f654L,
    0xc5a92679L, 0xc1683bceL, 0xcc2b1d17L, 0xc8ea00a0L,
    0xd6ad50a5L, 0xd26c4d12L, 0xdf2f6bcbL, 0xdbee767cL,
    0xe3a1cbc1L, 0xe760d676L, 0xea23f0afL, 0xeee2ed18L,
    0xf0a5bd1dL, 0xf464a0aaL, 0xf9278673L, 0xfde69bc4L,
    0x89b8fd09L, 0x8d79e0beL, 0x803ac667L, 0x84fbdbd0L,
    0x9abc8bd5L, 0x9e7d9662L, 0x933eb0bbL, 0x97ffad0cL,
    0xafb010b1L, 0xab710d06L, 0xa6322bdfL, 0xa2f33668L,
    0xbcb4666dL, 0xb8757bdaL, 0xb5365d03L, 0xb1f740b4L
};

/* CRC-32-IEEE 802.3 0xEDB88320 */
static UINT ulCrc32_table_reverse[] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
    0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

/* --------------------------CRC 8 正序--------------------------------*/

/*
    CRC-8       x8+x5+x4+1              0x31（0x131）
    CRC-8       x8+x2+x1+1              0x07（0x107）
    CRC-8       x8+x6+x4+x3+x2+x1       0x5E（0x15E）
    由于多项式的最高为都为1，并且在代码的crc8计算中，最高位也是不使用的，
所以在多项式记录时都去掉了最高位。

    反序异或：计算的多项式从0x31（0011 0001）变成了0x8C (1000 1100) 
*/

/***********************************************************
 * Function Name:   cal_table_positive
 * Description:     创建正序表每个元素的校验值
 * Parameters[in]:  IN UCHAR value
 * Parameters[ou]: 
 * ReturnValue:     UCHAR
 * Author:          liuke
 * Date:            2019.1.4
************************************************************/
UCHAR cal_table_positive(IN UCHAR value)
{
    UCHAR i, crc;
 
    crc = value;
    /* 数据往左移了8位，需要计算8次 */
    for (i=8; i>0; --i)
    { 
        if (crc & 0x80)  /* 判断最高位是否为1 */
        {
            /* 最高位为1，不需要异或，往左移一位，然后与0x31异或 */
            /* 0x31(多项式：x8+x5+x4+1，100110001)，最高位不需要异或，直接去掉 */
            crc = (crc << 1) ^ 0x31;
        }
        else
        {
            /* 最高位为0时，不需要异或，整体数据往左移一位 */
            crc = (crc << 1);
        }
    }
 
    return crc;
}

/***********************************************************
 * Function Name:   create_crc8_table_positive
 * Description:     创建正序表
 * Parameters[in]:  无
 * Parameters[ou]: 
 * ReturnValue:     无
 * Author:          liuke
 * Date:            2019.1.4
************************************************************/
VOID  create_crc8_table_positive(VOID)
{
    USHORT i = 0;
    UCHAR j = 0;

    for (i=0; i<=0xFF; i++)
    {
        if (0 == (i%16))
            printf("\n");

        j = i&0xFF;
        printf("0x%.2x, ", cal_table_positive(j));  /*依次计算每个字节的crc校验值*/
    }
}

/***********************************************************
 * Function Name:   soft_crc8_positive
 * Description:     CRC8计算法（多项式0x31 正序）
 * Parameters[in]:  IN UCHAR *ptr
                    IN UCHAR len
 * Parameters[ou]: 
 * ReturnValue:     校验值
 * Author:          liuke
 * Date:            2019.1.4
***********************************************************/
UCHAR soft_crc8_positive(IN UCHAR *ptr, IN UCHAR len)
{
    UCHAR i = 0; 
    UCHAR ucCrc = 0x00; /* 计算的初始crc值 */ 
 
    while(len--)
    {
        ucCrc ^= *ptr++;  /* 每次先与需要计算的数据异或,计算完指向下一数据 */  
        for (i = 8; i > 0; --i)   /* 下面这段计算过程与计算一个字节crc一样 */  
        { 
            if (ucCrc & 0x80)
                ucCrc = (ucCrc << 1) ^ 0x31;
            else
                ucCrc = (ucCrc << 1);
        }
    }
 
    return ucCrc; 
}

/**********************************************************
 * Function Name:   soft_crc8_table_positive
 * Description:     CRC8查表法（多项式0x31 正序）
 * Parameters[in]:  IN UCHAR *ptr
                    IN UCHAR len
 * Parameters[ou]: 
 * ReturnValue:     校验值
 * Author:          liuke
 * Date:            2019.1.4
***********************************************************/
UCHAR soft_crc8_table_positive(UCHAR *ptr, UCHAR len) 
{
    UCHAR  ucCrc = 0x00;
 
    while (len--)
    {
        ucCrc = ucCrc8_table_positive[ucCrc ^ *ptr++];
    }
    
    return ucCrc;
}

/**********************************************************
 * Function Name:   cal_table_reverse
 * Description:     CRC8逆序（多项式0x31）：计算每个查找表的校验值
 * Parameters[in]:  IN UCHAR value
 * Parameters[ou]: 
 * ReturnValue:     校验值
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
UCHAR cal_table_reverse(IN UCHAR value)
{
    UCHAR i, crc;
 
    crc = value;
    /* 数据往左移了8位，需要计算8次 */
    for (i=8; i>0; --i)
    { 
        if ((crc & 1) != 0)  /* 判断最高位是否为1 */
        {
            /* 最高位为1，不需要异或，往左移一位，然后与0x31异或 */
            /* 计算的多项式从0x31（0011 0001）变成了0x8C (1000 1100) */
            crc = (crc >> 1) ^ 0x8C;        
        }
        else
        {
            /* 最高位为0时，不需要异或，整体数据往左移一位 */
            crc = (crc >> 1);
        }
    }
 
    return crc;
}

/*****************************************************
 * Function Name:   create_crc8_table_reverse
 * Description:     CRC8逆序（多项式0x31）：创建查找表
 * Parameters[in]:  无
 * Parameters[ou]: 
 * ReturnValue:     无
 * Author:          liuke
 * Date:            2019.1.4
*****************************************************/
void  create_crc8_table_reverse(void)
{
    unsigned short i;
    unsigned char j;

    for (i=0; i<=0xFF; i++)
    {
        if (0 == (i % 16))
            printf("\n");

        j = i&0xFF;
        printf("0x%.2x, ", cal_table_reverse(j));  /*依次计算每个字节的crc校验值*/
    }
}

/**********************************************************
 * Function Name:   cal_table_reverse
 * Description:     CRC8逆序（多项式0x31）：计算法校验
 * Parameters[in]:  IN UCHAR *ptr
                    IN UCHAR len
 * Parameters[ou]: 
 * ReturnValue:     校验值
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
UCHAR soft_crc8_reverse(IN UCHAR *ptr, IN UCHAR len)
{
    UCHAR i = 0; 
    UCHAR ucCrc = 0xFF; /* 计算的初始crc值 */ 
 
    while(len--)
    {
        ucCrc ^= *ptr++;  /* 每次先与需要计算的数据异或,计算完指向下一数据 */  
        for (i = 8; i > 0; --i)   /* 下面这段计算过程与计算一个字节crc一样 */  
        { 
            if (ucCrc & 0x01)
                ucCrc = (ucCrc >> 1) ^ 0x8C;
            else
                ucCrc = (ucCrc >> 1);
        }
    }
 
    return ucCrc ^ 0xFF; 
}

/**********************************************************
 * Function Name:   cal_table_reverse
 * Description:     CRC8逆序（多项式0x31）：查表法校验
 * Parameters[in]:  IN UCHAR *ptr
                    IN UCHAR len
 * Parameters[ou]: 
 * ReturnValue:     校验值
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
UCHAR soft_crc8_table_reverse(UCHAR *ptr, UCHAR len) 
{
    UCHAR  ucCrc = 0xFF;
 
    while (len--)
    {
        ucCrc = ucCrc8_table_reverse[ucCrc ^ *ptr++];
    }
    
    return ucCrc ^ 0xFF;
}

/* ---------------------------CRC 16 正序---------------------------- */
/**********************************************************
 * Function Name:   create_crc16_table_positive
 * Description:     CRC16正序（多项式0x1021）：
                        创建表
 * Parameters[in]:  无
 * Parameters[ou]:  无
 * ReturnValue:     无
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
VOID create_crc16_table_positive(VOID)
{
    USHORT c;
    USHORT i = 0;
    USHORT bit = 0;
    
    for(i = 0; i < 256; i++)
    {
        c  = i;
        
        for(bit = 0; bit < 16; bit++)
        {
            if(c & 0x8000)
            {
                c = (c << 1)^(0x1021);
            }
            else
            {
                c =  c << 1;
            }
            
        }
        
        printf("crc16_table_positive %d %04x\n", i, c);
    }
}

/**********************************************************
 * Function Name:   soft_crc16_positive
 * Description:     CRC16正序（多项式0x1021）
                        计算法校验
 * Parameters[in]:  IN UCHAR *pcBuf
                    IN USHORT usLength
 * Parameters[ou]:  无
 * ReturnValue:     校验值
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
USHORT soft_crc16_positive(IN UCHAR *pcBuf, IN USHORT usLength)
{
    USHORT i = 0;
    USHORT usCRC = 0x0000;

    while( usLength-- ) 
    {
        usCRC ^= ((*pcBuf) << 8);
        
        for(i = 0x80; i != 0; i >>= 1) 
        {
            if((usCRC & 0x8000) != 0) 
            {
                usCRC = usCRC << 1;
                usCRC = usCRC ^ 0x1021;
            }
            else
            {
                usCRC = usCRC << 1;
            }
        }
        
        pcBuf++;
    }

    return usCRC;
} 

/**********************************************************
 * Function Name:   soft_crc16_table_positive
 * Description:     CRC16正序（多项式0x1021）
                        查表法校验
 * Parameters[in]:  IN UCHAR *pcBuf
                    IN USHORT usLength
 * Parameters[ou]:  无
 * ReturnValue:     校验值
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
USHORT soft_crc16_table_positive(IN UCHAR *pcBuf, IN ULONG len)
{
    USHORT usCrc = 0;

    while (len-- > 0)
    {
        usCrc = usCrc16_table_positive[(usCrc >> 8 ^ *pcBuf++) & 0xff] ^ (usCrc << 8);
    }
    
    return usCrc;
}

/* -----------------------------------------------CRC 16 逆序-------------------------------------------------- */
/**********************************************************
 * Function Name:   create_crc16_table_reverse
 * Description:     CRC16逆序序（多项式0x1021）
                        创建表
 * Parameters[in]:  无
 * Parameters[ou]:  无
 * ReturnValue:     无
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
VOID create_crc16_table_reverse(VOID)
{
    USHORT c;
    USHORT i = 0;
    USHORT bit = 0;
    
    for(i = 0; i < 256; i++)
    {
        c  = i;
        
        for(bit = 0; bit < 8; bit++)
        {
            if(c & 1)
            {
                c = (c >> 1)^(0x8408);
            }
            else
            {
                c =  c >> 1;
            }
            
        }
        
        printf("crc32_table %d %04x\n", i, c);
    }
}

/* CCITT  0x8408 逆序 */
/**********************************************************
 * Function Name:   soft_crc16_reverse
 * Description:     CRC16逆序序（多项式0x1021）
                        计算法校验
 * Parameters[in]:  IN UCHAR *pcBuf
                    IN USHORT usLength
 * Parameters[ou]:  无
 * ReturnValue:     校验值
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
USHORT soft_crc16_reverse(IN UCHAR *pcBuf, IN USHORT usLength)
{
    USHORT i = 0;
    USHORT usCRC = 0xFFFF;

    while( usLength-- ) 
    {
        usCRC ^= *pcBuf;
        
        for(i = 0x80; i != 0; i >>= 1) 
        {
            if((usCRC & 1) != 0) 
            {
                usCRC = usCRC >> 1;
                usCRC = usCRC ^ 0x8408;
            }
            else
            {
                usCRC = usCRC >> 1;
            }
        }
        
        pcBuf++;
    }

    return usCRC ^ 0xFFFF;
} 

/* CCITT 查表法  0x8408 逆序  */
/**********************************************************
 * Function Name:   soft_crc16_reverse
 * Description:     CRC16逆序序（多项式0x1021）
                        查表法校验
 * Parameters[in]:  IN UCHAR *pcBuf
                    IN USHORT usLength
 * Parameters[ou]:  无
 * ReturnValue:     校验值
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
USHORT soft_crc16_table_reverse(IN UCHAR *pcBuf, IN ULONG len)
{
    USHORT usCrc = 0xFFFF;

    while (len-- > 0)
    {
        usCrc = usCrc16_table_reverse[(usCrc ^ *pcBuf++) & 0xff] ^ (usCrc >> 8);
    }
    
    return usCrc ^ 0xFFFF;
}


/* --------------------------------------------------CRC 32 正序-------------------------------------------------------- */
/*
CRC-32-IEEE 802.3         x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1
                            0x04C11DB7 or 0xEDB88320 (0xDB710641)
CRC-32C (Castagnoli)     x32 + x28 + x27 + x26 + x25 + x23 + x22 + x20 + x19 + x18 + x14 + x13 + x11+ x10 + x9 + x8 + x6 + 1
                            0x1EDC6F41 or 0x82F63B78 (0x05EC76F1)
*/

/**********************************************************
 * Function Name:   create_crc32_table_positive
 * Description:     CRC32正序（多项式0x04c11db7L）
                        创建查找表
 * Parameters[in]:  无
 * Parameters[ou]:  无
 * ReturnValue:     无
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
VOID create_crc32_table_positive(VOID)
{
    UINT c;
    INT i = 0;
    INT bit = 0;
    UINT crc32_table[256];
    
    for(i = 0; i < 256; i++)
    {
        c  = (UINT)i;
        
        for(bit = 0; bit < 32; bit++)
        {
            if(c & 0x80000000)
            {
                c = (c << 1)^(0x04c11db7L);
            }
            else
            {
                c =  c << 1;
            }
            
        }
        crc32_table[i] = c;
        
        printf("crc32_table %d %08x\n", i, c);
    }

    FUNCTION_UNUSED_PARA(crc32_table);

    return;
}

/**********************************************************
 * Function Name:   soft_crc32_positive
 * Description:     CRC32正序（多项式0x04c11db7L）
                        计算法校验
 * Parameters[in]:  IN UCHAR *pcBuf
                    IN USHORT usLength
 * Parameters[ou]:  无
 * ReturnValue:     校验值
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
UINT soft_crc32_positive(IN UCHAR *pcBuf, IN USHORT usLength)
{
    UINT i = 0;
    UINT uiCrc = 0;

    while( usLength-- ) 
    {
        uiCrc ^= ((*pcBuf) << 24);
        
        for(i = 0; i < 8; i++)
        {
            if((uiCrc & 0x80000000) != 0) 
            {
                uiCrc = uiCrc << 1;
                uiCrc = uiCrc ^ 0x04c11db7L;
            }
            else
            {
                uiCrc = uiCrc << 1;
            }
        }
        
        pcBuf++;
    }

    return uiCrc;
} 

/*********************************************************
 * Function Name:   soft_crc32_table_positive
 * Description:     CRC32正序（多项式0x04c11db7L）
                        查表法校验
 * Parameters[in]:  IN UCHAR *pcBuf
                    IN UINT ulsize
 * Parameters[ou]:  无
 * ReturnValue:     校验值
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
UINT soft_crc32_table_positive(IN UCHAR *pcBuf, IN UINT ulsize)
{
    UINT i = 0;
    UINT uiCrc = 0;
 
    for (i = 0; i < ulsize; i++)
    {
        uiCrc = (uiCrc << 8) ^ ulCrc32_table_positive[( uiCrc >> 24 )^ *pcBuf++];   
    }
     
    return uiCrc;
}

/* ---------------------------CRC 32 逆序------------------------------ */
/*********************************************************
 * Function Name:   create_crc32_table_reverse
 * Description:     CRC32逆序（多项式0xEDB88320）
                        创建表
 * Parameters[in]:  无
 * Parameters[ou]:  无
 * ReturnValue:     无
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
VOID create_crc32_table_reverse(VOID)
{
    UINT c;
    INT i = 0;
    INT bit = 0;
    UINT crc32_table[256];
    
    for(i = 0; i < 256; i++)
    {
        c  = (UINT)i;
        
        for(bit = 0; bit < 8; bit++)
        {
            if(c & 1)
            {
                c = (c >> 1)^(0xEDB88320);
            }
            else
            {
                c =  c >> 1;
            }
            
        }
        crc32_table[i] = c;
        
        printf("crc32_table %d %08x\n", i, c);
    }

    FUNCTION_UNUSED_PARA(crc32_table);
}

/**********************************************************
 * Function Name:   soft_crc32_reverse
 * Description:     CRC32逆序（多项式0xEDB88320）
                        计算法校验
 * Parameters[in]:  IN UCHAR *pcBuf
                    IN USHORT usLength
 * Parameters[ou]:  无
 * ReturnValue:     校验值
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
UINT soft_crc32_reverse(IN UCHAR *pcBuf, IN USHORT usLength)
{
    UINT i = 0;
    UINT uiCrc = 0xFFFFFFFF;

    while( usLength-- ) 
    {
        uiCrc ^= *pcBuf;
        
        for(i = 0x80; i != 0; i >>= 1) 
        {
            if((uiCrc & 1) != 0) 
            {
                uiCrc = uiCrc >> 1;
                uiCrc = uiCrc ^ 0xEDB88320;
            }
            else
            {
                uiCrc = uiCrc >> 1;
            }
        }
        
        pcBuf++;
    }

    return uiCrc ^ 0xFFFFFFFF;
} 

/**********************************************************
 * Function Name:   soft_crc32_reverse
 * Description:     CRC32逆序（多项式0xEDB88320）
                        查表法校验
 * Parameters[in]:  IN UCHAR *pcBuf
                    IN USHORT usLength
 * Parameters[ou]:  无
 * ReturnValue:     校验值
 * Author:          liuke
 * Date:            2019.1.4
**********************************************************/
UINT soft_crc32_table_reverse(UCHAR *pcBuf, UINT ulsize)
{
    UINT i = 0;
    UINT uiCrc = 0xFFFFFFFF;
 
    for (i = 0; i < ulsize; i++)
    {
        uiCrc = ulCrc32_table_reverse[(uiCrc ^ pcBuf[i]) & 0xff] ^ (uiCrc >> 8);
    }
     
    return uiCrc ^ 0xFFFFFFFF;
}

