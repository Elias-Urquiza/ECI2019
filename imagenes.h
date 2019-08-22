#ifndef __IMAGENES__H__
#define __IMAGENES__H__

#include <stdio.h>
#include "libbmp.h"

BMP *src_img, *src_img2, *dst_img;

void imagenes_abrir(configuracion_t *config);
void imagenes_guardar(configuracion_t *config);
void imagenes_liberar();
void imagenes_flipVertical(buffer_info_t *buffer, BMP *bmp);

#endif /* !__IMAGENES__H__ */
