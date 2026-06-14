/**
 * @file pic_utils.h
 * @brief Image processing utility functions for ESL (Electronic Shelf Label)
 */
#ifndef __PIC_UTILS_H__
#define __PIC_UTILS_H__

#include "lvgl/lvgl.h"

/**
 * @brief Get the width and height of a PNG image
 * @param path Path to the PNG image file
 * @param punWidth Pointer to store the image width
 * @param punHeight Pointer to store the image height
 * @return 0 on success, non-zero error code on failure
 */
int GetPNGWidthHeight(const char* path, unsigned int* punWidth, unsigned int* punHeight);

/**
 * @brief Get the width and height of a JPEG image
 * @param path Path to the JPEG image file
 * @param punWidth Pointer to store the image width
 * @param punHeight Pointer to store the image height
 * @return 0 on success, non-zero error code on failure
 */
int GetJPEGWidthHeight(const char* path, unsigned int *punWidth, unsigned int *punHeight);

/**
 * @brief Get the width and height of an image (supports multiple formats)
 * @param path Path to the image file
 * @param punWidth Pointer to store the image width
 * @param punHeight Pointer to store the image height
 */
void GetPicWidthHeight(const char* path, unsigned int *punWidth, unsigned int *punHeight);

/**
 * @brief Convert a hexadecimal string to a 32-bit unsigned integer
 * @param hex_str Hexadecimal string to convert
 * @return Converted 32-bit unsigned integer
 */
uint32_t hex_to_uint32(const char *hex_str);

/**
 * @brief Convert RGBA color values to a hexadecimal color code
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 * @param a Alpha component (0.0-1.0)
 * @return Hexadecimal color code as a 32-bit unsigned integer
 */
uint32_t rgba_to_hex(int r, int g, int b, float a);

#endif//__PIC_UTILS_H__