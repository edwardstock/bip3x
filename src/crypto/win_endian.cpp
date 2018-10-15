/*!
 * bip39.
 * endian.cpp
 *
 * \date 2018
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#include "win_endian.h"

uint16_t to_little_endian16(uint16_t value) {
    union Endian_Data_16 endian_data;
    union Endian_Data_16 endian_data_copy;
    endian_data_copy.integer_value = value;

    endian_data.char_values[0] = endian_data_copy.char_values[ENDIANNESS_2_BYTE_TYPE_LSB_PLUS_0_INDEX];
    endian_data.char_values[1] = endian_data_copy.char_values[ENDIANNESS_2_BYTE_TYPE_LSB_PLUS_1_INDEX];

    return endian_data.integer_value;
}

uint32_t to_little_endian32(uint32_t value) {
    union Endian_Data_32 endian_data;
    union Endian_Data_32 endian_data_copy;
    endian_data_copy.integer_value = value;

    endian_data.char_values[0] = endian_data_copy.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_0_INDEX];
    endian_data.char_values[1] = endian_data_copy.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_1_INDEX];
    endian_data.char_values[2] = endian_data_copy.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_2_INDEX];
    endian_data.char_values[3] = endian_data_copy.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_3_INDEX];

    return endian_data.integer_value;
}
uint64_t to_little_endian64(uint64_t value) {
    union Endian_Data_64 endian_data;
    union Endian_Data_64 endian_data_copy;
    endian_data_copy.integer_value = value;

    endian_data.char_values[0] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_0_INDEX];
    endian_data.char_values[1] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_1_INDEX];
    endian_data.char_values[2] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_2_INDEX];
    endian_data.char_values[3] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_3_INDEX];
    endian_data.char_values[4] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_4_INDEX];
    endian_data.char_values[5] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_5_INDEX];
    endian_data.char_values[6] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_6_INDEX];
    endian_data.char_values[7] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_7_INDEX];

    return endian_data.integer_value;
}
uint16_t to_big_endian16(uint16_t value) {
    union Endian_Data_16 endian_data;
    union Endian_Data_16 endian_data_copy;
    endian_data_copy.integer_value = value;

    endian_data.char_values[0] = endian_data_copy.char_values[ENDIANNESS_2_BYTE_TYPE_LSB_PLUS_1_INDEX];
    endian_data.char_values[1] = endian_data_copy.char_values[ENDIANNESS_2_BYTE_TYPE_LSB_PLUS_0_INDEX];

    return endian_data.integer_value;
}
uint32_t to_big_endian32(uint32_t value) {
    union Endian_Data_32 endian_data;
    union Endian_Data_32 endian_data_copy;
    endian_data_copy.integer_value = value;

    endian_data.char_values[0] = endian_data_copy.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_3_INDEX];
    endian_data.char_values[1] = endian_data_copy.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_2_INDEX];
    endian_data.char_values[2] = endian_data_copy.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_1_INDEX];
    endian_data.char_values[3] = endian_data_copy.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_0_INDEX];

    return endian_data.integer_value;
}
uint64_t to_big_endian64(uint64_t value) {
    union Endian_Data_64 endian_data;
    union Endian_Data_64 endian_data_copy;
    endian_data_copy.integer_value = value;

    endian_data.char_values[0] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_7_INDEX];
    endian_data.char_values[1] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_6_INDEX];
    endian_data.char_values[2] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_5_INDEX];
    endian_data.char_values[3] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_4_INDEX];
    endian_data.char_values[4] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_3_INDEX];
    endian_data.char_values[5] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_2_INDEX];
    endian_data.char_values[6] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_1_INDEX];
    endian_data.char_values[7] = endian_data_copy.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_0_INDEX];

    return endian_data.integer_value;
}
uint16_t from_little_endian16(uint16_t value) {
    union Endian_Data_16 endian_data;
    union Endian_Data_16 endian_data_copy;
    endian_data_copy.integer_value = value;

    endian_data.char_values[ENDIANNESS_2_BYTE_TYPE_LSB_PLUS_0_INDEX] = endian_data_copy.char_values[0];
    endian_data.char_values[ENDIANNESS_2_BYTE_TYPE_LSB_PLUS_1_INDEX] = endian_data_copy.char_values[1];

    return endian_data.integer_value;
}
uint32_t from_little_endian32(uint32_t value) {
    union Endian_Data_32 endian_data;
    union Endian_Data_32 endian_data_copy;
    endian_data_copy.integer_value = value;

    endian_data.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_0_INDEX] = endian_data_copy.char_values[0];
    endian_data.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_1_INDEX] = endian_data_copy.char_values[1];
    endian_data.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_2_INDEX] = endian_data_copy.char_values[2];
    endian_data.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_3_INDEX] = endian_data_copy.char_values[3];

    return endian_data.integer_value;
}
uint64_t from_little_endian64(uint64_t value) {
    union Endian_Data_64 endian_data;
    union Endian_Data_64 endian_data_copy;
    endian_data_copy.integer_value = value;

    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_0_INDEX] = endian_data_copy.char_values[0];
    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_1_INDEX] = endian_data_copy.char_values[1];
    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_2_INDEX] = endian_data_copy.char_values[2];
    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_3_INDEX] = endian_data_copy.char_values[3];
    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_4_INDEX] = endian_data_copy.char_values[4];
    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_5_INDEX] = endian_data_copy.char_values[5];
    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_6_INDEX] = endian_data_copy.char_values[6];
    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_7_INDEX] = endian_data_copy.char_values[7];

    return endian_data.integer_value;
}
uint16_t from_big_endian16(uint16_t value) {
    union Endian_Data_16 endian_data;
    union Endian_Data_16 endian_data_copy;
    endian_data_copy.integer_value = value;

    endian_data.char_values[ENDIANNESS_2_BYTE_TYPE_LSB_PLUS_0_INDEX] = endian_data_copy.char_values[1];
    endian_data.char_values[ENDIANNESS_2_BYTE_TYPE_LSB_PLUS_1_INDEX] = endian_data_copy.char_values[0];

    return endian_data.integer_value;
}
uint32_t from_big_endian32(uint32_t value) {
    union Endian_Data_32 endian_data;
    union Endian_Data_32 endian_data_copy;
    endian_data_copy.integer_value = value;

    endian_data.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_0_INDEX] = endian_data_copy.char_values[3];
    endian_data.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_1_INDEX] = endian_data_copy.char_values[2];
    endian_data.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_2_INDEX] = endian_data_copy.char_values[1];
    endian_data.char_values[ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_3_INDEX] = endian_data_copy.char_values[0];

    return endian_data.integer_value;
}
uint64_t from_big_endian64(uint64_t value) {
    union Endian_Data_64 endian_data;
    union Endian_Data_64 endian_data_copy;
    endian_data_copy.integer_value = value;

    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_0_INDEX] = endian_data_copy.char_values[7];
    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_1_INDEX] = endian_data_copy.char_values[6];
    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_2_INDEX] = endian_data_copy.char_values[5];
    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_3_INDEX] = endian_data_copy.char_values[4];
    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_4_INDEX] = endian_data_copy.char_values[3];
    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_5_INDEX] = endian_data_copy.char_values[2];
    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_6_INDEX] = endian_data_copy.char_values[1];
    endian_data.char_values[ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_7_INDEX] = endian_data_copy.char_values[0];

    return endian_data.integer_value;
}
