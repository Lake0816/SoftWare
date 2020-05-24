#include <stdio.h>
#include <string.h>
#include "soft_types.h"
#include "config.h"

#if defined(CONFIG_COMMON_CRC)
#include "Crc/soft_crc.h"
#endif

INT main(INT argc, CHAR **args)
{
    FUNCTION_UNUSED_PARA(argc);
    FUNCTION_UNUSED_PARA(args);

#if defined(CONFIG_COMMON_CRC)
    UCHAR string[] = "hello World";
    UCHAR ucCrc = 0;
    USHORT usCrc = 0;
    UINT uiCrc = 0;    

    ucCrc = soft_crc8_positive(string, strlen((CHAR *)string));
    printf("soft_crc8_positive crc %u\n", ucCrc);
    
    ucCrc = soft_crc8_table_positive(string, strlen((CHAR *)string));
    printf("soft_crc8_table_positive crc %u\n", ucCrc);

    ucCrc = soft_crc8_reverse(string, strlen((CHAR *)string));
    printf("soft_crc8_reverse crc %u\n", ucCrc);
    
    ucCrc = soft_crc8_table_reverse(string, strlen((CHAR *)string));
    printf("soft_crc8_table_reverse crc %u\n", ucCrc);
    
    usCrc = soft_crc16_reverse(string, strlen((CHAR *)string));
    printf("soft_crc16_reverse crc %u\n", usCrc);

    usCrc = soft_crc16_table_reverse(string, strlen((CHAR *)string));
    printf("soft_crc16_table_reverse crc %u\n", usCrc);
    
    usCrc = soft_crc16_positive(string, strlen((CHAR *)string));
    printf("soft_crc16_positive crc %u\n", usCrc);

    usCrc = soft_crc16_table_positive(string, strlen((CHAR *)string));
    printf("soft_crc16_table_positive crc %u\n", usCrc);

    uiCrc = soft_crc32_positive(string, strlen((CHAR *)string));
    printf("soft_crc32_positive crc %u\n", uiCrc);
    
    uiCrc = soft_crc32_table_positive(string, strlen((CHAR *)string));
    printf("soft_crc32_table_positive crc %u\n", uiCrc);

    uiCrc = soft_crc32_reverse(string, strlen((CHAR *)string));
    printf("soft_crc32_reverse crc %u\n", uiCrc);
    
    uiCrc = soft_crc32_table_reverse(string, strlen((CHAR *)string));
    printf("soft_crc32_table_reverse crc %u\n", uiCrc);
#endif

    return 0;
}

