#ifndef __SOFT_CRC_H__
#define __SOFT_CRC_H__

/*
    CRC-8       x8+x5+x4+1              0x31（0x131）
    CRC-8       x8+x2+x1+1              0x07（0x107）
    CRC-8       x8+x6+x4+x3+x2+x1       0x5E（0x15E）
    由于多项式的最高为都为1，并且在代码的crc8计算中，最高位也是不使用的，
所以在多项式记录时都去掉了最高位。

    反序异或：计算的多项式从0x31（0011 0001）变成了0x8C (1000 1100) 
*/

/* 0x31 正序 */
extern UCHAR soft_crc8_positive(UCHAR *ptr, UCHAR len);

/* 0x31 查表法 正序 */
extern UCHAR soft_crc8_table_positive(UCHAR *ptr, UCHAR len);

/* 0x8C 逆序 */
extern UCHAR soft_crc8_reverse(UCHAR *ptr, UCHAR len);

/* 0x8C 查表法 逆序 */
extern UCHAR soft_crc8_table_reverse(UCHAR *ptr, UCHAR len);

/*
    CRC16_CCITT：多项式x16+x12+x5+1（0x1021），初始值0x0000，低位在前，高位在后，结果与0x0000异或

    CRC16_CCITT_FALSE：多项式x16+x12+x5+1（0x1021），初始值0xFFFF，低位在后，高位在前，结果与0x0000异或

    CRC16_XMODEM：多项式x16+x12+x5+1（0x1021），初始值0x0000，低位在后，高位在前，结果与0x0000异或

    CRC16_X25：多项式x16+x12+x5+1（0x1021），初始值0x0000，低位在前，高位在后，结果与0xFFFF异或

    CRC16_MODBUS：多项式x16+x15+x2+1（0x8005），初始值0xFFFF，低位在前，高位在后，结果与0x0000异或

    CRC16_IBM：多项式x16+x15+x2+1（0x8005），初始值0x0000，低位在前，高位在后，结果与0x0000异或

    CRC16_MAXIM：多项式x16+x15+x2+1（0x8005），初始值0x0000，低位在前，高位在后，结果与0xFFFF异或

    CRC16_USB：多项式x16+x15+x2+1（0x8005），初始值0xFFFF，低位在前，高位在后，结果与0xFFFF异或
*/

/* CCITT  0x1021 正序 */
extern USHORT soft_crc16_positive(UCHAR *pcBuf, USHORT usLength);

/* CCITT 查表法 0x1021 正序 */
extern USHORT soft_crc16_table_positive(UCHAR *pcBuf, ULONG len);

/* CCITT  0x8408 逆序 */
extern USHORT soft_crc16_reverse(UCHAR *pcBuf, USHORT usLength);

/* CCITT 查表法  0x8408 逆序  */
extern USHORT soft_crc16_table_reverse(UCHAR *pcBuf, ULONG len);


/*
CRC-32-IEEE 802.3         x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1
                            0x04C11DB7 or 0xEDB88320 (0xDB710641)
CRC-32C (Castagnoli)     x32 + x28 + x27 + x26 + x25 + x23 + x22 + x20 + x19 + x18 + x14 + x13 + x11+ x10 + x9 + x8 + x6 + 1
                            0x1EDC6F41 or 0x82F63B78 (0x05EC76F1)
*/

/* CRC-32-IEEE 802.3  0x04c11db7L 正序 */
extern UINT soft_crc32_positive(UCHAR *pcBuf, USHORT usLength);

/* CRC-32-IEEE 802.3  查表法: 0x04c11db7L 正序 */
extern UINT soft_crc32_table_positive(UCHAR *pcBuf, UINT ulsize);

/* CRC-32-IEEE 802.3  0xEDB88320 逆序 */
extern UINT soft_crc32_reverse(UCHAR *pcBuf, USHORT usLength);

/* 查表法: 0xEDB88320 逆序 */
extern UINT soft_crc32_table_reverse(UCHAR *pcBuf, UINT ulsize);

#endif
