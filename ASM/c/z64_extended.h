#ifndef Z64_EXTENDED_H
#define Z64_EXTENDED_H

#include "z64.h"

/* dram addresses */
#define z64_camera_view_addr	0x801DB0CD

/* data */
#define z64_camera_view			(*(uint8_t*)	z64_camera_view_addr)

#endif