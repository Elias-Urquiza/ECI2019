/* ************************************************************************* */
/* Organizacion del Computador II                                            */
/*                                                                           */
/*             Biblioteca de funciones para operar imagenes BMP              */
/*                                                                           */
/*   Esta biblioteca permite crear, abrir, modificar y guardar archivos en   */
/*   formato bmp de forma sencilla. Soporta solamente archivos con header de */
/*   versiones info_header (40 bytes) y info_v5_header (124 bytes). Para la  */
/*   primera imagenes de 24 bits (BGR) y la segunda imagenes de 32 (ABGR).   */
/*                                                                           */
/*   bmp.h : headers de la biblioteca                                        */
/*   bmp.c : codigo fuente de la biblioteca                                  */
/*   example.c : ejemplos de uso de la biblioteca                            */
/*               $ gcc example.c bmp.c -o example                            */
/* ************************************************************************* */

#ifndef __BMP__H__
#define __BMP__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define size_BITMAPCOREHEADER   12
#define size_OS21XBITMAPHEADER  12
#define size_OS22XBITMAPHEADER  64
#define size_BITMAPINFOHEADER   40 // supported
#define size_BITMAPV2INFOHEADER 52
#define size_BITMAPV3INFOHEADER 56
#define size_BITMAPV4HEADER     108
#define size_BITMAPV5HEADER     124 // supported

typedef struct buffer_info_t
{
    int width, height, row_size;
    unsigned char *bytes;
    unsigned int tipo;
} buffer_info_t;

typedef struct bgra_t {
    unsigned char b, g, r, a;
} __attribute__((packed)) bgra_t;

typedef enum e_BITMAPTYPE {
BITMAPINFOHEADER,
BITMAPV3INFOHEADER,
BITMAPV5HEADER
} BITMAPTYPE;

typedef enum {
  LCS_GM_ABS_COLORIMETRIC = 0x00000008,
  LCS_GM_BUSINESS = 0x00000001,
  LCS_GM_GRAPHICS = 0x00000002,
  LCS_GM_IMAGES = 0x00000004
} BMPGamutMappingIntent;

typedef  enum {
  LCS_CALIBRATED_RGB = 0x00000000,
  LCS_sRGB = 0x73524742,
  LCS_WINDOWS_COLOR_SPACE = 0x57696E20
} BMPLogicalColorSpace;

typedef  enum {
  BI_RGB = 0x0000,
  BI_RLE8 = 0x0001,
  BI_RLE4 = 0x0002,
  BI_BITFIELDS = 0x0003,
  BI_JPEG = 0x0004,
  BI_PNG = 0x0005,
  BI_CMYK = 0x000B,
  BI_CMYKRLE8 = 0x000C,
  BI_CMYKRLE4 = 0x000D
} BMPCompression;

typedef struct __attribute__((packed)) s_CIEXYZ {
  uint32_t ciexyzX; // FXPT2DOT30
  uint32_t ciexyzY; // FXPT2DOT30
  uint32_t ciexyzZ; // FXPT2DOT30
} CIEXYZ,*LPCIEXYZ;

typedef struct __attribute__((packed)) s_CIEXYZTRIPLE {
  CIEXYZ ciexyzRed;
  CIEXYZ ciexyzGreen;
  CIEXYZ ciexyzBlue;
} CIEXYZTRIPLE,*LPCIEXYZTRIPLE;

typedef struct __attribute__((packed)) s_BITMAPFILEHEADER {
  char  bfType[2];
  uint32_t  bfSize;
  uint16_t  bfReserved1;
  uint16_t  bfReserved2;
  uint32_t bfOffBits;
} BMPFH;

typedef struct __attribute__((packed)) s_BITMAPINFOHEADER {
  uint32_t biSize;
  uint32_t biWidth;
  uint32_t biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  uint32_t biXPelsPerMeter;
  uint32_t biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
} BMPIH;

typedef struct __attribute__((packed)) s_BITMAPV3INFOHEADER {
  uint32_t bv3iSize;
  uint32_t bv3iWidth;
  uint32_t bv3iHeight;
  uint16_t bv3iPlanes;
  uint16_t bv3iBitCount;
  uint32_t bv3iCompression;
  uint32_t bv3iSizeImage;
  uint32_t bv3iXPelsPerMeter;
  uint32_t bv3iYPelsPerMeter;
  uint32_t bv3iClrUsed;
  uint32_t bv3iClrImportant;
  uint32_t bv3iRedMask;
  uint32_t bv3iGreenMask;
  uint32_t bv3iBlueMask;
  uint32_t bv3iAlphaMask;
} BMPV3IH;

typedef struct __attribute__((packed)) s_BITMAPV5HEADER{
  uint32_t bV5Size;
  uint32_t bV5Width;
  uint32_t bV5Height;
  uint16_t bV5Planes;
  uint16_t bV5BitCount;
  uint32_t bV5Compression;
  uint32_t bV5SizeImage;
  uint32_t bV5XPelsPerMeter;
  uint32_t bV5YPelsPerMeter;
  uint32_t bV5ClrUsed;
  uint32_t bV5ClrImportant;
  uint32_t bV5RedMask;
  uint32_t bV5GreenMask;
  uint32_t bV5BlueMask;
  uint32_t bV5AlphaMask;
  uint32_t bV5CSType;
  CIEXYZTRIPLE bV5Endpoints;
  uint32_t bV5GammaRed;
  uint32_t bV5GammaGreen;
  uint32_t bV5GammaBlue;
  uint32_t bV5Intent;
  uint32_t bV5ProfileData;
  uint32_t bV5ProfileSize;
  uint32_t bV5Reserved;
} BMPV5H;

typedef struct s_BMP { BMPFH* fh; void* ih; uint8_t* data; } BMP;

/** get_BMPIH: creates an info header with default parameters**/
BMPIH* get_BMPIH(uint32_t width, uint32_t height);

/** get_BMPv5H: creates a V5 info header with default parameters**/
BMPV5H* get_BMPV5H(uint32_t width, uint32_t height);

/** bmp_create: Creates a bmp using info headers(40B or 124B), a 1 in init_data initializes the data **/
BMP* bmp_create(void* info_header, int init_data);

/** bmp_copy: Copies a bmp, and copies it's data if copy_data is 1 **/
BMP* bmp_copy(BMP* img, int copy_data);

/** creates an empty bmp of width x height **/
BMP* bmp_new(int width, int height);

/** opens and starts reading a bmp **/
BMP* bmp_read(char* src);

/** saves a bmp **/
int bmp_save(char* dst, BMP* img);

/** deletes a bmp **/
void bmp_delete(BMP* img);

/** Obtains the width of a bmp **/
uint32_t bmp_width(BMP* img);

/** obtains the height of the bmp **/
uint32_t bmp_height(BMP* img);

/** bmp_bytes_per_row: obtiene el ancho en bytes de cada linea del bmp **/
/** bmp_bytes_per_row: Obtains the width in Bytes of every row of the bmp **/ 
uint32_t bmp_bytes_per_row(BMP* img);

/** Obtains the bitcount of the bmp **/
uint16_t bmp_bit_count(BMP* img);

/** Obtains compression code of the bmp **/
uint32_t bmp_compression(BMP* img);

/** Obtains the data part of the bmp (the pixels) **/
uint8_t* bmp_data(BMP* img);

/** Converts the data from the 24 bit to 32 bit bmp format **/
void bmp_convert_24_to_32_bpp(BMP *img);

/** Converts data between 8 and 32 bit formats **/
void bmp_convert_8_to_32_bpp (BMP *img);
void bmp_convert_32_to_8_bpp (BMP *img);

void setear_buffer(buffer_info_t *buffer, BMP *bmp);

#endif /* __BMP__H__ */
