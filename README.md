# dfuse\_pack

## A DfuSe packer (USB device firmware upgrade STMicroelectronics extension)

This command line utility lets you pack raw binary firmware images into a DfuSe image container to be downloaded by any compatible flasher. The ideal companion to extend your automated build and release infrastructure based on DfuSe containers.

### Dependencies

- [eina](http://docs.enlightenment.org/auto/eina/eina_main.html)
- [cmake](http://www.cmake.org/)

### Build / install

	mkdir build
	cd build
	cmake ..
	make
	sudo make install

### Arguments

- mandatory (type)
	- **-o** output image (string)
	- **-a** target alternate setting (decimal 8-bit integer)
	- **-i** image raw binary (string)
- optional (type) [default]
	- **-n** target name (string) ['\0']
	- **-m** image memory target (hexadecimal 32-bit integer) [0x00000000]
	- **-f** firmware version (hexadecimal 16-bit integer) [0xffff]
	- **-p** product ID (hexadecimal 16-bit integer) [0xdf11]
	- **-v** vendor ID (hexadecimal 16-bit integer) [0x0483]
- help
	- **-h**

### Usage

You need to provide at least one alternate setting with at least one image and an output for the program to succeed. Some command line arguments can occur multiple times ([-a], [-n], [-m], [-i]) and some must be given in the correct order: The memory target [-m] must precede its associated image file [-i]. The latter two and the target name [-n] must precede their associated alternate setting [-a].

A minimal working example using all the defaults for omitted arguments:

	dfuse_pack \
		-o firmware.dfu \
			-i image.bin \
		-a 0

A more complex one based on two images for alternate setting 0 and a single image for alternate setting 1:

	dfuse_pack \
		-f 0x0100 \
		-p 0xdf11 \
		-v 0x0483 \
		-o firmware.dfu \
			-n "INTERNAL FLASH of my awesome app v0.1.0" \
			-m 0x08000000 -i boot.bin \
			-m 0x08005000 -i code.bin \
		-a 0 \
			-n "OPTION BYTES of my awesome app v0.1.0" \
			-m 0x1ffff8000 -i options.bin \
		-a 1

### License

Released under zlib/libpng license.

Copyright (c) 2014 Hanspeter Portner (dev@open-music-kontrollers.ch)

### References

DfuSe file specification: [DfuSe USB device firmware upgrade STMicroelectronics extension](http://www.st.com/web/catalog/tools/FM147/CL1794/SC961/SS1533/PF257916)

DfuSe compatible open source flasher: [dfu-util](http://dfu-util.gnumonks.org/)
