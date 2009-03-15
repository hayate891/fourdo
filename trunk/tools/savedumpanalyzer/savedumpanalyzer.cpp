// savedumpanalyzer.cpp : Defines the entry point for the console application.

#include <stdio.h>

#include "types.hpp"
#include "registerprinter.hpp"

#define SIZE_REG     (195)
#define SIZE_RAM     (1024 * 1024 * 3)
#define SIZE_ROM1    (1024 * 1024)
#define SIZE_ROM2    (1024 * 1024)

#define OFFSET_DUMP_START ( 0x003CD214 + 64 )
#define OFFSET_REG   ( OFFSET_DUMP_START )
#define OFFSET_RAM   ( OFFSET_REG  + SIZE_REG )
#define OFFSET_ROM1  ( OFFSET_RAM  + SIZE_RAM )
#define OFFSET_ROM2  ( OFFSET_ROM1 + SIZE_ROM1 )
#define OFFSET_NVRAM ( OFFSET_ROM2 + SIZE_ROM2 )

/////////////////////////////////////
// Functions
void printLogo();
void printUsage();
void readInfo(FILE * file);
void readSection(FILE * inputFile, const char* outName, int dataSize );

int main(int argc, char* argv[])
{
	printLogo();
	
	if( argc != 2)
	{
		printUsage();
		return -1;
	}
	
	FILE * pFile;
	fopen_s( &pFile, argv[1],"rb");
	if( pFile == NULL )
	{
		printf("Errors opening file\n");
		return -1;
	}
	
	readInfo( pFile );
		
	fclose (pFile);
	
	return 0;
}

void printLogo()
{
	printf("===============================================\n");
	printf("        Johnny's save dump analyzer            \n");
	printf("===============================================\n");
}

void printUsage()
{
	printf("\n");
	printf("USAGE:    SaveDumpAnalyzer [filename]\n");
	printf("\n");
	printf("This splits a save file into component parts\n");
	printf("\n");
	printf("\n");
}

void readInfo(FILE * file)
{
	/////////////////////////////////////
	// Go to where the registers sit.
	fseek( file, OFFSET_REG, SEEK_SET );
	
	ARM_CoreState reg;
	fread( &reg, sizeof( reg ), 1, file );
	writeRegInfo( &reg );

	/////////////////////////////////////
	// Go to where the RAM sits, and output it
	fseek( file, OFFSET_RAM, SEEK_SET );
	readSection( file, "SDA_RAM.bin", SIZE_RAM );

	/////////////////////////////////////
	// Go to where the ROM1+ROM2 sit.
	fseek( file, OFFSET_ROM1, SEEK_SET );
	readSection( file, "SDA_ROM1.bin", SIZE_ROM1 );

	fseek( file, OFFSET_ROM2, SEEK_SET );
	readSection( file, "SDA_ROM2.bin", SIZE_ROM2 );
}

void readSection(FILE * inputFile, const char* outName, int dataSize )
{
	FILE * outputFile;
	fopen_s( &outputFile, outName, "wb");
	
	if( outputFile == NULL )
		return;

	///////////////////////////////
	// Read RAM contents
	uint8* ramBytes;
	ramBytes = new uint8[dataSize];
	fread( ramBytes, dataSize, 1, inputFile );

	// Swap endianness
	uint8 temp;
	int   x;
	for( x = 0; x < (dataSize/4); x+=4 )
	{
		temp = ramBytes[x];
		ramBytes[x]=ramBytes[x+3];
		ramBytes[x+3]=temp;

		temp = ramBytes[x+1];
		ramBytes[x+1]=ramBytes[x+2];
		ramBytes[x+2]=temp;
	}
	
	///////////////////////////////
	// Output RAM contents to file
	int ret;
	ret = fwrite( ramBytes, 1, dataSize, outputFile );
	
	delete[] ramBytes;
	
	fclose( outputFile );
}