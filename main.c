/*
 * main.c
 *
 *  Created on: Jan 11, 2017
 *      Author: EE113D
 */

#include <stdio.h>
#include <stdlib.h>
#include "L138_LCDK_aic3106_init.h"
#include "m_mem.h"

#pragma pack(1)

#define SAMPLELEN 1600

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

int i, datasize;

int padding = 54624;

short int* audioread(char* name)
{

	short int *song;
	FILE *WAV_in = fopen (name, "rb");

	if (WAV_in == NULL)
	{
	     printf("\nCannot open file\n");
	      exit(1);
	}

	printf("Reading the wav header...\n");
	fread(&Header, sizeof(Header), 1, WAV_in);
	fread(&InfoHeader, sizeof(InfoHeader), 1, WAV_in);

	datasize = InfoHeader.Subchunk2Size;

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

    fread(&song, sizeof(short int), (datasize/sizeof(short int)),  WAV_in);

    fclose(WAV_in);

    printf("Finish reading WAV file...\n");

	return song;
}

int counter;
volatile int record;
volatile int reset;

int16_t left_sample;
int16_t sample[SAMPLELEN];

short int *lamb;

int index;

interrupt void interrupt4(void)
{

	if(index < InfoHeader.Subchunk2Size/(InfoHeader.BitsPerSample/8))
		output_left_sample(lamb[index]);
	else
		output_left_sample(0);

	index += 2;

//	if (record ==1)
//	{
//		if(counter < SAMPLELEN)
//		{
//			sample[counter] = input_left_sample();
//			counter = (counter+1)%SAMPLELEN;
//		}
//		output_left_sample(input_left_sample());
//	}
//
//	else{
//			output_left_sample(0);
//		}
//	return;
}




int main()
{
	record = 0;
	reset = 0;
	counter = 0;

	lamb = audioread("C:\\Users\\EE113D\\MyProjects\\LittleLamb\\test.wav");


	L138_initialise_intr(FS_44100_HZ,ADC_GAIN_24DB,DAC_ATTEN_0DB,LCDK_MIC_INPUT);

	while(1)
	{

		if(reset == 1)
			{
				record = 0;
				reset = 0;
			}

	}

}
