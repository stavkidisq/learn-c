#include <iostream>

bool frame_from_first_device[] = 
{
    1, 0, 1, 1, 1, 0, 1, 1 , 0, 0, 1, 0, 0, 0
};

bool frame_from_second_device[] =
{
    1, 1, 0, 1, 1, 1, 1, 1 , 0, 0, 1, 0, 1, 0
};

bool frame_from_third_device[] =
{
    1, 0, 0, 0, 1, 0, 1, 1 , 1, 0, 1, 1, 0, 1
};

int sender(bool buff[][8], bool frames[14]);
int receiver(bool package[][8], bool frames[14]);

int encoder(bool arr[], bool buff[][8]);
int decoder(bool package[][8], bool data_frame[]);

int fill_data_frame(bool arr[], bool buff[], int start, int end);

int main()
{
    bool first_package[8][8] = {};
    bool second_package[8][8] = {};
    bool third_package[8][8] = {};

    bool fourth_package[8][8] = {
        { 0, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 0, 1 },
        { 0, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 1 }
    };

    sender(first_package, frame_from_first_device);
    sender(second_package, frame_from_second_device);
    sender(third_package, frame_from_third_device);

    bool first_frame[14];
    bool second_frame[14];
    bool third_frame[14];
    bool fourth_frame[14];

    receiver(first_package, first_frame);
    receiver(second_package, second_frame);
    receiver(third_package, third_frame);
    receiver(fourth_package, fourth_frame);

    return 0;
}

int decoder(bool package[][8], bool data_frame[])
{
    int k = 0;
    for (int i = 1; i < 5; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            data_frame[k] = package[i][j];
            k++;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        data_frame[k] = package[5][i];
        k++;
    }

    return 0;
}

int receiver(bool package[][8], bool frames[14])
{
    bool start_frame[8] = { 0, 1, 1, 1, 1, 1, 1, 0 };
    bool stop_frame[8] = { 1, 0, 0, 0, 0, 0, 0, 1 };
    bool xor_frame[8];

    for (int i = 0; i < 8; i++)
        xor_frame[i] = package[1][i] ^ package[2][i] ^ package[3][i] ^ package[4][i] ^ package[5][i];

    if (memcmp(package[0], start_frame, 8) < 0)
    {
        perror("Start frame don't match!");
        return -1;
    }

    if (memcmp(package[6], xor_frame, 8) < 0)
    {
        perror("Xor don't match!");
        return -1;
    }

    if (memcmp(package[7], stop_frame, 8) < 0)
    {
        perror("Stop frame don't match!");
        return -1;
    }

    decoder(package, frames);
    return 0;
}

int sender(bool buff[][8], bool frames[14])
{
    bool arr[8][8] = {};
    int result = encoder(frames, arr);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            buff[i][j] = arr[i][j];
        }
    }

    return 0;
}

int encoder(bool arr[], bool buff[][8])
{
    bool start_frame[8] = { 0, 1, 1, 1, 1, 1, 1, 0 };
    bool stop_frame[8] = { 1, 0, 0, 0, 0, 0, 0, 1 };
    
    for (int i = 0; i < 8; i++)
        buff[0][i] = start_frame[i];

    fill_data_frame(arr, buff[1], 0, 3);
    fill_data_frame(arr, buff[2], 3, 6);
    fill_data_frame(arr, buff[3], 6, 9);
    fill_data_frame(arr, buff[4], 9, 12);
    fill_data_frame(arr, buff[5], 12, 14);

    for (int i = 0; i < 8; i++)
        buff[6][i] = buff[1][i] ^ buff[2][i] ^ buff[3][i] ^ buff[4][i] ^ buff[5][i];

    for (int i = 0; i < 8; i++)
        buff[7][i] = stop_frame[i];

    return 0;
}

int fill_data_frame(bool arr[], bool buff[], int start, int end)
{
    for (int i = 0; i < 8; i++)
        buff[i] = i % 2;

    int j = 0;
    for (int i = start; i < end; i++, j++)
    {
        buff[j] = arr[i];
    }

    return 0;
}
