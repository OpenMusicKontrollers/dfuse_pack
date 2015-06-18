/*
 * Copyright (c) 2015 Hanspeter Portner (dev@open-music-kontrollers.ch)
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the Artistic License 2.0 as published by
 * The Perl Foundation.
 *
 * This source is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * Artistic License 2.0 for more details.
 *
 * You should have received a copy of the Artistic License 2.0
 * along the source as a COPYING file. If not, obtain it from
 * http://www.perlfoundation.org/artistic_license_2_0.
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
