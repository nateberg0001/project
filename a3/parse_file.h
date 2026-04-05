typedef struct WAVE_METADATA {
    short num_channels;
    int sample_rate;
    short bit_depth;
    unsigned int num_samples; // Size of the PCM for one channel
    double *left_channel_pcm;
    double *right_channel_pcm;
    unsigned int pcm_size; // Total size of the PCM for all channels
    int metadata[11]; // 44 bytes of metadata for access when creating the new file
} WAV_INFO;

void fill_pcm(WAV_INFO *wav_info, FILE *fp);
WAV_INFO *parse_file(char *filename);
