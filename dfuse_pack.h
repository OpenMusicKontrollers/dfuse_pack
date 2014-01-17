/*
 * Copyright (c) 2014 Hanspeter Portner (dev@open-music-kontrollers.ch)
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 * 
 *     2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 * 
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 */

#ifndef DFUSE_PACK_H
#define DFUSE_PACK_H

#include <dfuse_pack.h>

#include <Eina.h>

typedef struct _DfuSe_Entry					DfuSe_Entry;
typedef struct _DfuSe_Entry_Image		DfuSe_Entry_Image;

struct _DfuSe_Entry {
	DfuSe_Target_Prefix tPrefix;
	Eina_List *images;
};

struct _DfuSe_Entry_Image {
	char *path;
	uint8_t *buf;
	uint32_t len;
	DfuSe_Image_Element element;
};

#endif /* DFUSE_PACK_H */
