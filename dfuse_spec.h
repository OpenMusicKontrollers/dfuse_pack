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

#ifndef DFUSE_SPEC_H
#define DFUSE_SPEC_H

#include <stdint.h>

typedef struct _DfuSe_Prefix				DfuSe_Prefix;
typedef struct _DfuSe_Target_Prefix	DfuSe_Target_Prefix;
typedef struct _DfuSe_Image_Element	DfuSe_Image_Element;
typedef struct _DfuSe_Suffix				DfuSe_Suffix;

#define DFUSE_PREFIX_SIZE						sizeof(DfuSe_Prefix)
#define DFUSE_TARGET_PREFIX_SIZE		sizeof(DfuSe_Target_Prefix)
#define DFUSE_IMAGE_ELEMENT_SIZE		sizeof(DfuSe_Image_Element)
#define DFUSE_SUFFIX_SIZE						sizeof(DfuSe_Suffix)

struct _DfuSe_Prefix {
	char szSignature [5];		// DfuSE signature, "DfuSe"
	uint8_t bVersion;				// DFU format revision
	uint32_t DFUImageSize;	// total DFU file length in bytes excluding suffix
	uint8_t bTargets;				// number of DFU targets stored in file
} __attribute__((packed));

struct _DfuSe_Target_Prefix {
	char szSignature [6];				// Target signature, "Target"
	uint8_t bAlternateSetting;
	uint32_t bTargetNamed;			// boolean flag whether the target is named or not
	char szTargetName [255];	
	uint32_t dwTargetSize;			// size of the image excluding the target prefix
	uint32_t dwNbElements;			// number of element in the image
} __attribute__((packed));

struct _DfuSe_Image_Element {
	uint32_t dwElementAddress;
	uint32_t dwElementSize;
} __attribute__((packed));

struct _DfuSe_Suffix {
	uint16_t bcdDevice;				// firmware version, 0xFFFF if to be ignored
	uint16_t idProduct;				// product ID, 0xFFFF if to be ignored
	uint16_t idVendor;				// vendor ID, 0xFFFF if to be ignored
	uint16_t bcdDFU;					// DFU specification number, 0x011A for DfuSe
	char ucDfuSignature [3];	// DFU signature
	uint8_t bLength;					// length of DFU suffix itself, 0x10
	uint32_t dwCRC;						// cyclic redundancy check
} __attribute__((packed));

#endif /* DFUSE_SPEC_H */
