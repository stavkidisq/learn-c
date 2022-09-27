#include <iostream>

/*
    --------------
   | start_frame  | - Стартовый кадр, который должен быть равен 0x01111110.
    --------------
    --------------
   | first_frame  | - Кадр данных.
    --------------
    --------------
   | second_frame | - Кадр данных.
    --------------
    --------------
   | third_frame  | - Кадр данных.
    --------------
    --------------
   | fourth_frame | - Кадр данных.
    --------------
    --------------
   |  fifth_frame | - Кадр данных.
    --------------
    --------------
   |   xor_frame  | - XOR-кадр, который равен функции XOR от всех кадров данных.
    --------------
    --------------
   |  stop_frame  | - Стоповый кадр, который должен быть равен 0x10000001.
    --------------
*/

#define XOR ^
#define DEVICE_DATA_LENGTH 14
#define PACKAGE_SIZE 8

// Данные, взятые с 1-го датчика.
bool frame_from_first_device[] = 
{
    1, 0, 1, 1, 1, 0, 1, 1 , 0, 0, 1, 0, 0, 0
};

// Данные, взятые со 2-го датчика.
bool frame_from_second_device[] =
{
    1, 1, 0, 1, 1, 1, 1, 1 , 0, 0, 1, 0, 1, 0
};

// Данные, взятые с 3-го датчика.
bool frame_from_third_device[] =
{
    1, 0, 0, 0, 1, 0, 1, 1 , 1, 0, 1, 1, 0, 1
};

// Функция отправителя данных.
int sender(bool buff_package[][8], bool frame[DEVICE_DATA_LENGTH]);

// Функция получателя данных.
int receiver(bool package[][8], bool buff_frame[DEVICE_DATA_LENGTH]);

// Функция кодирования данных.
int encoder(bool frame[], bool buff_package[][8]);

// Функция декодирования данных.
int decoder(bool package[][8], bool data_frame[]);

// Функция заполнения кадров с полезной информацией.
int fill_data_frame(bool arr[], bool buff[], int start, int end);

// Функция вывода полезной информации на консоль.
int display_useful_data(bool frame[]);

int main()
{
    // Здесь происходит инициализация массивов, хранящих пакеты данных.
    bool first_package[PACKAGE_SIZE][8] = {};
    bool second_package[PACKAGE_SIZE][8] = {};
    bool third_package[PACKAGE_SIZE][8] = {};
    bool fourth_package[PACKAGE_SIZE][8] =
    {
        { 0, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 1, 1, 1, 1, 1, 0, 1 },
        { 0, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 1, 1 }
    };

    // Обработка информации отправтелем.
    sender(first_package, frame_from_first_device);
    sender(second_package, frame_from_second_device);
    sender(third_package, frame_from_third_device);

    // Инициализация буферов, в которыебудет вкладываться полезная информация.
    bool first_frame[DEVICE_DATA_LENGTH];
    bool second_frame[DEVICE_DATA_LENGTH];
    bool third_frame[DEVICE_DATA_LENGTH];
    bool fourth_frame[DEVICE_DATA_LENGTH];

    // Обработка информации получателем.
    if (receiver(first_package, first_frame) == 0)
    {
        display_useful_data(first_frame);
    }
    else
    {
        printf("An error occurred while transmitting the package.\n");
    }

    if (receiver(second_package, second_frame) == 0)
    {
        display_useful_data(second_frame);
    }
    else
    {
        printf("An error occurred while transmitting the package.\n");
    }

    if (receiver(fourth_package, fourth_frame) == 0)
    {
        display_useful_data(fourth_frame);
    }
    else
    {
        printf("An error occurred while transmitting the package.\n");
    }

    if (receiver(third_package, third_frame) == 0)
    {
        display_useful_data(third_frame);
    }
    else
    {
        printf("An error occurred while transmitting the package.\n");
    }

    return 0;
}

int display_useful_data(bool frame[])
{
    for (int i = 0; i < 14; i++)
        printf("%d", frame[i]);
    printf("\n");

    return 0;
}

int decoder(bool package[][8], bool data_frame[])
{
    // Перемещение по 3 бита полезной информации в data_frame (c 1-го по 4 кадр включительно).
    int k = 0;
    for (int i = 1; i < 5; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            data_frame[k] = package[i][j];
            k++;
        }
    }

    // Перемещение 2 оставшихся бит полезной информации в data_frame (5 кадра)
    for (int i = 0; i < 2; i++)
    {
        data_frame[k] = package[5][i];
        k++;
    }

    return 0;
}

int receiver(bool package[][8], bool buff_frame[DEVICE_DATA_LENGTH])
{
    bool start_frame[8] = { 0, 1, 1, 1, 1, 1, 1, 0 };
    bool stop_frame[8] = { 1, 0, 0, 0, 0, 0, 0, 1 };
    bool xor_frame[8];

    for (int i = 0; i < 8; i++)
        xor_frame[i] = package[1][i] XOR package[2][i] XOR package[3][i] XOR package[4][i] XOR package[5][i];

    // Проверка правильности сартового кадра.
    if (memcmp(package[0], start_frame, 8) < 0)
    {
        perror("The start frame is incorrect.");
        return -1;
    }

    // Проверка правильности XOR-кадра.
    if (memcmp(package[6], xor_frame, 8) < 0)
    {
        perror("The checksum of data packages is incorrect.");
        return -1;
    }

    // Проверка правильности стопового кадра.
    if (memcmp(package[7], stop_frame, 8) < 0)
    {
        perror("The stop frame is incorrect.");
        return -1;
    }

    decoder(package, buff_frame);

    return 0;
}

int sender(bool buff_package[][8], bool frame[DEVICE_DATA_LENGTH])
{
    bool arr[8][8] = {};
    encoder(frame, arr);

    // Перемещение данных в буффер.
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            buff_package[i][j] = arr[i][j];
        }
    }

    return 0;
}

int encoder(bool frame[], bool buff_package[][8])
{
    // Задаются стартовый и стоповый кадры.
    bool start_frame[8] = { 0, 1, 1, 1, 1, 1, 1, 0 };
    bool stop_frame[8] = { 1, 0, 0, 0, 0, 0, 0, 1 };
    
    // Заполнение стартового кадра.
    for (int i = 0; i < 8; i++)
        buff_package[0][i] = start_frame[i];

    // Заполнение каждого кадра полезной информацией.
    fill_data_frame(frame, buff_package[1], 0, 3);
    fill_data_frame(frame, buff_package[2], 3, 6);
    fill_data_frame(frame, buff_package[3], 6, 9);
    fill_data_frame(frame, buff_package[4], 9, 12);
    fill_data_frame(frame, buff_package[5], 12, 14);

    // Заполнение XOR-кадра.
    for (int i = 0; i < 8; i++)
        buff_package[6][i] = buff_package[1][i] XOR buff_package[2][i] XOR buff_package[3][i] XOR buff_package[4][i] XOR buff_package[5][i];

    // Заполнение стопового кадра.
    for (int i = 0; i < 8; i++)
        buff_package[7][i] = stop_frame[i];

    return 0;
}

int fill_data_frame(bool arr[], bool buff[], int start, int end)
{
    // Заполнение кадра мусорными битами.
    for (int i = 0; i < 8; i++)
        buff[i] = i % 2;

    // Заполнение кадра полезными битами.
    int j = 0;
    for (int i = start; i < end; i++, j++)
        buff[j] = arr[i];

    return 0;
}
