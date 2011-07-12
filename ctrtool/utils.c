#include <stdio.h>
#include <string.h>
#include "utils.h"


u32 align(u32 offset, u32 alignment)
{
	u32 mask = ~(alignment-1);

	return (offset + (alignment-1)) & mask;
}

u64 getle64(const u8* p)
{
	u64 n = p[0];

	n |= (u64)p[1]<<8;
	n |= (u64)p[2]<<16;
	n |= (u64)p[3]<<24;
	n |= (u64)p[4]<<32;
	n |= (u64)p[5]<<40;
	n |= (u64)p[6]<<48;
	n |= (u64)p[7]<<56;
	return n;
}

u32 getle32(const u8* p)
{
	return (p[0]<<0) | (p[1]<<8) | (p[2]<<16) | (p[3]<<24);
}

u32 getle16(const u8* p)
{
	return (p[0]<<0) | (p[1]<<8);
}


void readkeyfile(u8* key, const char* keyfname)
{
	FILE* f = fopen(keyfname, "rb");
	u32 keysize = 0;

	if (0 == f)
	{
		fprintf(stdout, "Error opening key file\n");
		goto clean;
	}

	fseek(f, 0, SEEK_END);
	keysize = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (keysize != 16)
	{
		fprintf(stdout, "Error key size mismatch, got %d, expected %d\n", keysize, 16);
		goto clean;
	}

	if (16 != fread(key, 1, 16, f))
	{
		fprintf(stdout, "Error reading key file\n");
		goto clean;
	}

clean:
	if (f)
		fclose(f);
}


void memdump(FILE* fout, const char* prefix, const u8* data, u32 size)
{
	u32 i;
	u32 prefixlen = strlen(prefix);
	u32 offs = 0;
	u32 line = 0;
	while(size)
	{
		u32 max = 16;

		if (max > size)
			max = size;

		if (line==0)
			fprintf(fout, "%s", prefix);
		else
			fprintf(fout, "%*s", prefixlen, "");


		for(i=0; i<max; i++)
			fprintf(fout, "%02X", data[offs+i]);
		fprintf(fout, "\n");
		line++;
		size -= max;
		offs += max;
	}
}