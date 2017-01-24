#ifndef WAV
#define WAV
//Not sure what this library does

#include "m_mem.h"

#pragma pack(1)

struct Wav
{
   unsigned int ChunkID;
   unsigned int ChunkSize;
   unsigned int Format;
   unsigned int Subchunk1ID;
   unsigned int Subchunk1Size;
   unsigned short int AudioFormat;
} Header;

struct WavInfo
{
   unsigned short int NumChannels;
   //1 for mono, 2 for stereo
   unsigned int SampleRate, ByteRate;
   unsigned short int BlockAlign;
   unsigned short int BitsPerSample;
   unsigned int Subchunk2ID;
   unsigned int Subchunk2Size;
} InfoHeader;

short int* audioread(char* name)
{

	short int *song;
	FILE *WAV_in = fopen (name, "rb");
	int i, datasize;
	int padding;
	short int* pad;

	if (WAV_in == NULL)
	{
	     printf("\nCannot open file\n");
	      exit(1);
	}

	printf("Reading the wav header...\n");
	fread(&Header, sizeof(Header), 1, WAV_in);
	fread(&InfoHeader, sizeof(InfoHeader), 1, WAV_in);
	
	datasize = InfoHeader.Subchunk2Size;

	padding = 54624;
	pad = m_malloc(padding*sizeof(short int));

	song = m_malloc(datasize);



    if (song == NULL)
     {
    	 printf("out of memory!\n");
     }
     else
     {
    	 printf("Successfully allocated memory for the song\n");

     }

    printf("Reading WAV file...\n");
    for (i=0; i < InfoHeader.Subchunk2Size; i++)
    {
    	printf("%d \n", i);
    	if(i < padding)
    		fread(&pad, padding*sizeof(short int), 1, WAV_in);
    	else
    		fread(&song[i], sizeof(song[i]), 1,  WAV_in);
    }


    fclose(WAV_in);

    printf("Finish reading WAV file...\n");

	return song;
}



#endif
