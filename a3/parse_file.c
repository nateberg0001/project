#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parse_file.h"

/*
 * Helper function which fills the channel_pcm attributes of wav_info.
 * Stereo case: PCM samples are read in LRLR order into left and right channel PCM.
 * Mono case: PCM samples only fill the left channel PCM.
 */
void fill_pcm(WAV_INFO *wav_info, FILE *fp) {
    
    wav_info->left_channel_pcm = malloc(sizeof(double) * wav_info->num_samples);

    // Only malloc right channel if audio is stereo (will not be used otherwise)
    if (wav_info->num_channels == 2) {
        wav_info->right_channel_pcm = malloc(sizeof(double) * wav_info->num_samples);
    }    

    // Now, read audio data (starts at bit 44 of audio file)
    int i = 0;
    int j = 0;
    short sample;
    fseek(fp, 44, SEEK_SET);
    
    while (fread(&sample, (wav_info->bit_depth/8), 1, fp) != 0) {
        if (i % 2 == 0) {
            // Left channel case
            (wav_info->left_channel_pcm)[j] = (double) sample;

        } else {
            // Right channel case
            (wav_info->right_channel_pcm)[j] = (double) sample;
            j++;
        }

        // Fill right channel only if audio has 2 channels
        if (wav_info->num_channels == 2) {
            i++;
        }
        else { // Have to move through left_channel_pcm
            j++;
        }
    }
}

/*
 * Return a WAV_INFO struct containing metadata and PCM data of a .wav file
 */
WAV_INFO *parse_file(char *filename) {

    FILE *fp = fopen(filename, "rb"); 
    if (fp == NULL) {
        perror("Error reading file");
        exit(1);
    }

    int name_length = strlen(filename);
    char file_ex[5] = {filename[name_length - 4], filename[name_length - 3], filename[name_length - 2], filename[name_length - 1], '\0'};

    if (strcmp(file_ex, ".wav") != 0) {
        printf("File is not .wav\n");
        exit(1);
    }

    // Need (1) number of channels (bits 22-23), (2) sample rate (bits 24-27), (3) bit depth (bits 34-35)
    WAV_INFO *wav_info = malloc(sizeof(WAV_INFO)); 

    // Reading metadata
    if (fseek(fp, 0, SEEK_SET) == -1) {
        perror("Error finding start of file:");
        exit(1);
    }
    for (int i = 0; i <= 10; i++) {
        if (fread(&((wav_info->metadata)[i]), sizeof(int), 1, fp) == 0) {
            printf("Error reading metadata.\n");
            exit(1);
        }
    }    

    // Reading number of channels
    if (fseek(fp, 22, SEEK_SET) == -1) {
        perror("File smaller than .wav header (num_channels)");
        exit(1);
    }
    if (fread(&(wav_info->num_channels), sizeof(short), 1, fp) == 0 ||  wav_info->num_channels > 2) {
        printf("Error reading number of channels. Ensure the file does not have more than 2 audio channels\n");
        exit(1);
    }
    
    // Reading sample rate
    if (fseek(fp, 24, SEEK_SET) == -1) {
        perror("File smaller than .wav header (sample_rate)");
        exit(1);
    }
    if (fread(&(wav_info->sample_rate), sizeof(int), 1, fp) == 0) {
        printf("Error reading sample rate.\n");
        exit(1);
    }

    // Reading bit depth
    if (fseek(fp, 34, SEEK_SET) == -1) {
        perror("File smaller than .wav header (bit_depth)");
        exit(1);
    }
    if (fread(&(wav_info->bit_depth), 2, 1, fp) == 0) {
        printf("Error reading bit depth.\n");
        exit(1);
    }
    if (wav_info->bit_depth != 16) {
        printf("Unfortunately, only 16 bit audio data is currently supported. Please try again with a file with 16 bit audio data.\n");
    }

    // Reading number of samples
    if (fseek(fp, 40, SEEK_SET) == -1) {
        perror("File smaller than .wav header (subchunk2_size)");
        exit(1);
    }
    if (fread(&(wav_info->pcm_size), sizeof(int), 1, fp) == 0) {
        printf("Error reading subchunk2 size.\n");
        exit(1);
    }

    wav_info->num_samples = wav_info->pcm_size / wav_info->num_channels;

    printf("Channels: %d\n", wav_info->num_channels);
    printf("Sample rate: %d\n", wav_info->sample_rate);
    printf("Bit depth: %d\n", wav_info->bit_depth);

    if (wav_info->num_channels == 1) {
        printf("Number of samples (over %d channel): %d\n", wav_info->num_channels, wav_info->pcm_size);
        printf("Mono audio detected - file will be converted to dual channel split mono.\n");
    } else {
        printf("Number of samples (over %d channels): %d\n", wav_info->num_channels, wav_info->pcm_size);
    }
    
    printf("Number of samples per channel: %d\n", wav_info->num_samples);

    // Now, fill the pcm data arrays
    fill_pcm(wav_info, fp);    

    printf("File parsing complete.\n");

    fclose(fp);

    return wav_info;

}