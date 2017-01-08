/*
 * Copyright (c) 2015-2016 Hanspeter Portner (dev@open-music-kontrollers.ch)
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

#include <portable_endian.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <inttypes.h>

#include <dfuse_spec.h>
#include <dfuse_pack.h>

static const uint32_t crc32_table[256] = {
  0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
  0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
  0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
  0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
  0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
  0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
  0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
  0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
  0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
  0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
  0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
  0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
  0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
  0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
  0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
  0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
  0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
  0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
  0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
  0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
  0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
  0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
  0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
  0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
  0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
  0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
  0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
  0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
  0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
  0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
  0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
  0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
  0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
  0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
  0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
  0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
  0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
  0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
  0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
  0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
  0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
  0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
  0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

static uint32_t
crc32_update_no_xor (uint32_t crc, uint8_t *buf, size_t len)
{
	size_t i;
	
	for(i=0; i<len; i++)
    crc = crc32_table[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);

  return crc;
}

static DfuSe_Entry *
new_entry(void)
{
	DfuSe_Entry *entry = calloc(1, sizeof(DfuSe_Entry));

	strcpy(entry->tPrefix.szSignature, "Target");
	entry->tPrefix.bAlternateSetting = 0x0;
	entry->tPrefix.bTargetNamed = htole32(0x00000000);
	entry->tPrefix.szTargetName[0] = '\0';
	entry->tPrefix.dwTargetSize = htole32(0x00000000);
	entry->tPrefix.dwNbElements = htole32(0x00000000);

	return entry;
}

static DfuSe_Entry_Image *
new_image(void)
{
	DfuSe_Entry_Image *image = calloc(1, sizeof(DfuSe_Entry_Image));

	return image;
}

#define LIST_FOREACH(LIST, ITERATOR, DATA) \
	for((ITERATOR) = (LIST), (DATA) = (ITERATOR) ? (ITERATOR)->data : NULL; \
			(ITERATOR); \
			(ITERATOR) = (ITERATOR)->next, (DATA) = (ITERATOR) ? (ITERATOR)->data : NULL)

#define LIST_FREE(LIST, HEAD, DATA) \
	for((HEAD) = (LIST), (DATA) = (LIST) ? (LIST)->data : NULL; \
			(LIST); \
			(LIST) = (LIST)->next, (DATA) = (LIST) ? (LIST)->data : NULL, free(HEAD))

static DfuSe_List *
list_append(DfuSe_List *list, void *data)
{
	DfuSe_List *next = malloc(sizeof(DfuSe_List));
	if(next)
	{
		next->next = NULL;
		next->data = data;

		if(list)
		{
			DfuSe_List *tail = NULL;
			for(tail = list; tail->next; tail = tail->next)
				;

			tail->next = next;
		}
		else
		{
			list = next;
		}
	}

	return list;
}

static size_t
list_count(DfuSe_List *list)
{
	size_t count = 0;

	DfuSe_List *itr = NULL;
	for(itr = list; itr; itr = itr->next)
		count++;

	return count;
}

int
main (int argc, char **argv)
{
	DfuSe_List *targets = NULL;
	DfuSe_List *l, *m;
	DfuSe_Entry *entry = NULL;
	DfuSe_Entry_Image *image = NULL;
	uint16_t version = 0xffff;
	uint16_t product = 0xdf11;
	uint16_t vendor = 0x0483;
	char *out = NULL;
	FILE *dfuse = NULL;

#define FAIL(...) \
({ \
	fprintf(stderr, "FAIL: "__VA_ARGS__); \
	goto cleanup; \
})

	int c;
	while( (c = getopt (argc, argv, "a:i:m:n:f:p:v:o:h")) != -1 )
		switch(c)
		{
			case 'a':
			{
				uint8_t asetting;
				if(!sscanf(optarg, "%"SCNu8, &asetting))
					FAIL("invalid alternate setting '%s'\n", optarg);
				entry = entry ? entry : new_entry();
				entry->tPrefix.bAlternateSetting = asetting;
				if(!entry->images)
					FAIL("at least one image [-i] needed for altsetting %"PRIu8"\n", entry->tPrefix.bAlternateSetting);
				targets = list_append(targets, entry);
				entry = NULL;
				break;
			}
			case 'n':
				entry = entry ? entry : new_entry();
				entry->tPrefix.bTargetNamed = htole32(0x00000001);
				strcpy(entry->tPrefix.szTargetName, optarg);
				break;
			case 'm':
			{
				uint32_t address;
				if(!sscanf(optarg, "0x%08"SCNx32, &address) && !sscanf(optarg, "%08"SCNx32, &address))
						FAIL("invalid memory target '%s'\n", optarg);
				entry = entry ? entry : new_entry();
				image = image ? image : new_image();
				image->element.dwElementAddress = htole32(address);
				break;
			}
			case 'i':
				entry = entry ? entry : new_entry();
				image = image ? image : new_image();
				image->path = strdup(optarg);
				entry->images = list_append(entry->images, image);
				image = NULL;
				break;
			case 'f':
				if(!sscanf(optarg, "0x%04"SCNx16, &version) && !sscanf(optarg, "%04"SCNx16, &version))
						FAIL("invalid version number '%s'\n", optarg);
				break;
			case 'p':
				if(!sscanf(optarg, "0x%04"SCNx16, &product) && !sscanf(optarg, "%04"SCNx16, &product))
						FAIL("invalid product ID '%s'\n", optarg);
				break;
			case 'v':
				if(!sscanf(optarg, "0x%04"SCNx16, &vendor) && !sscanf(optarg, "%04"SCNx16, &vendor))
						FAIL("invalid vendor ID '%s'\n", optarg);
				break;
			case 'o':
				out = strdup(optarg);
				break;
			case 'h':
				printf("usage: %s \n\t{\n\t\t[-n TARGET_NAME = '\\0']\n\t\t{\n\t\t\t[-m MEMORY_TARGET = 0x00000000]\n\t\t\t-i INPUT_IMAGE\n\t\t}+\n\t\t-a ALT_SETTING\n\t}+\n\t[-f FIRMWARE_VERSION = 0xffff]\n\t[-p PRODUCT_ID = 0xdf11]\n\t[-v VENDOR_ID = 0x0483]\n\t[-h]\n\t-o OUTPUT_IMAGE\n\n", argv[0]);
				goto cleanup;
			case '?':
				if( (optopt == 'a') || (optopt == 'i') || (optopt == 'm') || (optopt == 'n') ||
						(optopt == 'f') || (optopt == 'p') || (optopt == 'v') || (optopt == 'o') )
					FAIL("Option `-%c' requires an argument.\n", optopt);
				else if(isprint(optopt))
					FAIL("Unknown option `-%c'.\n", optopt);
				else
					FAIL("Unknown option character `\\x%x'.\n", optopt);
			default:
				goto cleanup;
		}

	if(!targets || !out)
		FAIL("at least one target [-a] and an output [-o] needed\n");

	uint32_t DFUImageSize = DFUSE_TARGET_PREFIX_SIZE;

	LIST_FOREACH(targets, l, entry)
	{
		uint32_t dwTargetSize = 0UL;

		LIST_FOREACH(entry->images, m, image)
		{
			// load image
			FILE *raw = NULL;
			if(!(raw = fopen (image->path, "rb")))
				FAIL("could not open image file '%s'\n", image->path);
			fseek (raw, 0UL, SEEK_END);
			image->len = ftell (raw);
			fseek (raw, 0UL, SEEK_SET);
			image->buf = calloc (image->len, sizeof(uint8_t));
			fread (image->buf, sizeof(uint8_t), image->len, raw);
			if(raw)
				fclose(raw);
				
			// update target image element
			image->element.dwElementSize = htole32(image->len);

			dwTargetSize += DFUSE_IMAGE_ELEMENT_SIZE + image->len;
		}
			
		// update target image prefix
		entry->tPrefix.dwNbElements = htole32(list_count(entry->images));
		entry->tPrefix.dwTargetSize = htole32(dwTargetSize);

		DFUImageSize += dwTargetSize;
	}

	DfuSe_Prefix prefix = {
		.szSignature = {'D', 'f', 'u', 'S', 'e'},
		.bVersion = 0x01,
		.DFUImageSize = htole32(DFUImageSize),
		.bTargets = list_count(targets)
	};

	DfuSe_Suffix suffix = {
		.bcdDevice = htole16(version),
		.idProduct = htole16(product),
		.idVendor = htole16(vendor),
		.bcdDFU = htole16(0x011a),
		.ucDfuSignature = {'U', 'F', 'D'},
		.bLength = 0x10
		//.dwCRC is calculated followingly
	};

	uint32_t crc = 0xffffffffUL;
	crc = crc32_update_no_xor (crc, (uint8_t *)&prefix, DFUSE_PREFIX_SIZE);
	LIST_FOREACH(targets, l, entry)
	{
		crc = crc32_update_no_xor (crc, (uint8_t *)&entry->tPrefix, DFUSE_TARGET_PREFIX_SIZE);
		LIST_FOREACH(entry->images, m, image)
		{
			crc = crc32_update_no_xor (crc, (uint8_t *)&image->element, DFUSE_IMAGE_ELEMENT_SIZE);
			crc = crc32_update_no_xor (crc, image->buf, image->len);
		}
	}
	crc = crc32_update_no_xor (crc, (uint8_t *)&suffix, DFUSE_SUFFIX_SIZE-4); // exclusive CRC itself

	// update suffix
	suffix.dwCRC = htole32(crc);

	// write container to file
	printf("Output container: %s\n", out);
	printf("Firmware version: 0x%04"PRIx16"\n", version);
	printf("Product ID: 0x%04"PRIx16"\n", product);
	printf("Vendor ID: 0x%04"PRIx16"\n", vendor);
	printf("Targets: %"PRIuPTR"\n", list_count(targets));
	printf("CRC: 0x%04"PRIx32"\n\n", crc);

	if(!(dfuse = fopen(out, "wb")))
		FAIL("could not open output file '%s'\n", out);
	fwrite (&prefix, DFUSE_PREFIX_SIZE, 1, dfuse);
	LIST_FOREACH(targets, l, entry)
	{
		printf("Target\n");
		printf("\tAltsetting: %"PRIu8"\n", entry->tPrefix.bAlternateSetting);
		printf("\tImages: %"PRIuPTR"\n", list_count(entry->images));
		printf("\tName: %s\n\n", entry->tPrefix.szTargetName);

		fwrite (&entry->tPrefix, DFUSE_TARGET_PREFIX_SIZE, 1, dfuse);
		LIST_FOREACH(entry->images, m, image)
		{
			printf("\tImage\n");
			printf("\t\tPath: %s\n", image->path);
			printf("\t\tAddress: 0x%08"PRIx32"\n", image->element.dwElementAddress);
			printf("\t\tSize: %"PRIu32" bytes\n\n", image->len);

			fwrite (&image->element, DFUSE_IMAGE_ELEMENT_SIZE, 1, dfuse);
			fwrite (image->buf, sizeof(uint8_t), image->len, dfuse);
		}
	}
	fwrite (&suffix, DFUSE_SUFFIX_SIZE, 1, dfuse);

cleanup:

	if(dfuse)
		fclose(dfuse);

	if(targets)
		LIST_FREE(targets, l, entry)
		{
			if(entry->images)
				LIST_FREE(entry->images, m, image)
				{
					if(image->path)
						free(image->path);
					if(image->buf)
						free(image->buf);
					free(image);
				}
				free(entry);
		}

	if(out)
		free(out);

	return 0;
}
