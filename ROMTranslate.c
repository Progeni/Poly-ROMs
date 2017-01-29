/*
	TRANSLATE.C
	-----------
	Copyright (c) 2017 by Andrew Trotman 
	Released on the BSD Licence

	On the Poly 1 the address and data buses are not connected directly
	to the pins of the ROMs, they go through buffers and line driuvers
	that do not connect the pins in sequential order.  This program is
	supposed to translate the ROMs into order so they can be viewed
	sequentialy.
*/

#include <stdio.h>
#include <stdlib.h>

/*
	TRANSLATE_DATA()
	----------------
*/
unsigned char translate_data(unsigned char val)
{
unsigned char b1, b2, b3, b4, b5, b6, b7, b8;
unsigned char new_byte;

b1 = (val & 0x01) ? 1 : 0;
b2 = (val & 0x02) ? 1 : 0;
b3 = (val & 0x04) ? 1 : 0;
b4 = (val & 0x08) ? 1 : 0;
b5 = (val & 0x10) ? 1 : 0;
b6 = (val & 0x20) ? 1 : 0;
b7 = (val & 0x40) ? 1 : 0;
b8 = (val & 0x80) ? 1 : 0;

new_byte = 
b4 << 7 |
b5 << 6 |
b3 << 5 |
b6 << 4 |
b2 << 3 |
b7	<< 2 |
b1 << 1 |
b8 << 0 ;

return new_byte;
}

/*
	TRANSLATE_ADDRESS()
	-------------------
*/
unsigned long translate_address(unsigned long val)
{
unsigned long b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11;
unsigned long new_address;

b0  = (val & 0x01) ? 1 : 0;
b1  = (val & 0x02) ? 1 : 0;
b2  = (val & 0x04) ? 1 : 0;
b3  = (val & 0x08) ? 1 : 0;
b4  = (val & 0x10) ? 1 : 0;
b5  = (val & 0x20) ? 1 : 0;
b6  = (val & 0x40) ? 1 : 0;
b7  = (val & 0x80) ? 1 : 0;
b8  = (val & 0x100) ? 1 : 0;
b9  = (val & 0x200) ? 1 : 0;
b10 = (val & 0x400) ? 1 : 0;
b11 = (val & 0x800) ? 1 : 0;

new_address = 
b11 << 0  |
b9  << 1  |
b7  << 2  |
b6  << 3  |
b5  << 4  |
b3  << 5  |
b1  << 6  |
b0  << 7  |
b2  << 8  |
b4  << 9  |
b8  << 10 |
b10 << 11 ;

return new_address;
}

/*
	MAIN()
	------
*/
int main(int argc, char *argv[])
{
static unsigned char rom[4096];
unsigned long byte, new_address;
unsigned char val, rom_byte;
FILE *fp, *outfile;

if (argc != 3)
	exit(printf("Usage:%s <2532ROMFile> <outfile>", argv[0]));

if ((fp = fopen(argv[1], "rb")) == NULL)
	exit(printf("Can't open 2532ROMFile %s\n", argv[1]));

fread(rom, sizeof(rom), 1, fp);

outfile = fopen(argv[2], "wb");

for (byte = 0; byte < sizeof(rom); byte++)
	{
	new_address = translate_address(byte);
	rom_byte = rom[new_address];
	val = translate_data(rom_byte);
	fwrite(&val, 1, 1, outfile);
	}

fclose(outfile);
fclose(fp);

return 0;
}
