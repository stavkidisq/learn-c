#include <stdio.h>
#include <string.h>

#define DATA_LENGTH       256
#define XOR               ^
#define PACKAGE_SIZE      8
#define START_FRAME_INDEX 0
#define XOR_FRAME_INDEX   6
#define STOP_FRAME_INDEX  7

/* Функции для работы с файлами */
int fread(const char*, char[]);

/* Функции для работы с парсингом и преобразованием типов */
int from_str_to_bit_data_stream(char*, bool[]);

/* Функции для проверки кадров передаваемого пакета */
int find_start_frame(bool[]);               // Нахождение стартового кадра
int calculate_checksum(bool[][8], bool[8]); // Вычисление контрольной суммы пакета
int check_data_frames(bool[][8], bool[8]);  // Проверка контрольной суммы пакета
int check_stop_frame(bool[][8]);            // Проверка стопового кадра пакета данных

/* Функции для добавления кадров в буффера пакета данных */
int add_start_frame(bool[], bool[][8], int); // Добавление в буффер стартового кадра
int add_data_frames(bool[], bool[][8], int); // Добавление в буффер кадров, содержащих полезную информацию
int add_xor_frame(bool[], bool[][8], int);   // Добавление контрольной суммы в буффер кадров
int add_stop_frame(bool[], bool[][8], int);  // Нахождение стопового кадра

const bool start_frame[8] = { 0, 1, 1, 1, 1, 1, 1, 0 };
const bool stop_frame[8] = { 1, 0, 0, 0, 0, 0, 0, 1 };

int main()
{
	/* Инициализация нужных переменных */
	char str_data_stream[DATA_LENGTH];
	bool bit_data_stream[DATA_LENGTH];
	bool data_package[PACKAGE_SIZE][8];
	int start_data_package_index = 0;
	int stop_data_package_index = 0;

	/* Чтение битового потока из файла */
	if (fread("in-frame.txt", str_data_stream) < 0)
	{
		printf("An error has occurred in the program by reading file.");
		return -1;
	}

	/* Преобразоввание битового потока из строки в мссив битов */
	from_str_to_bit_data_stream(str_data_stream, bit_data_stream);

	/* Поиск стартового бита */
	if ((start_data_package_index = find_start_frame(bit_data_stream)) < 0)
	{
		printf("The start frame is missing in this bit-stream.");
		return -1;
	}

	printf("%d\n", start_data_package_index);

	/* Инициализация пакета данных */
	add_start_frame(bit_data_stream, data_package, start_data_package_index);
	add_data_frames(bit_data_stream, data_package, start_data_package_index);
	add_xor_frame(bit_data_stream, data_package, start_data_package_index);
	add_stop_frame(bit_data_stream, data_package, start_data_package_index);

	/* Проверка кадров данных на повреждения с использованием контрольной суммы */
	if (check_data_frames(data_package, data_package[6]) < 0)
	{
		printf("The xor frame is incorrect.");
		return -1;
	}

	/* Проверка наличия стопового кадра */
	if (check_stop_frame(data_package) < 0)
	{
		printf("The stop frame is incorrect.");
		return -1;
	}

	printf("package is \n");

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			printf("%d", data_package[i][j]);
		}
		printf("\n");
	}

	return 0;
}

int fread(const char* file_name, char buff_stream[])
{
	FILE* fstream;

	if ((fstream = fopen(file_name, "r")) == NULL)
	{
		perror("fopen error");
		return -1;
	}

	if (fscanf(fstream, "%s", buff_stream) < 0)
	{
		perror("fscanf error");
		return -1;
	}

	fclose(fstream);

	return 0;
}

int from_str_to_bit_data_stream(char* str_stream, bool buff_bit_stream[])
{
	for (int i = 0; str_stream[i] != 0; i++)
		buff_bit_stream[i] = str_stream[i] - '0';

	return 0;
}

int find_start_frame(bool stream[])
{
	int j = 0;
	for (int i = 0; i < 256/8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (stream[i*8 + j] != start_frame[j])
				break;
		}
		if (j == 8)
			return i * j;
	}

	perror("find_start_frame error.");
	return -1;
}

int add_start_frame(bool stream[], bool buff_package[][8], int start_package_index)
{
	for (int i = start_package_index; i < start_package_index + 8; i++)
		buff_package[START_FRAME_INDEX][i - start_package_index] = stream[i];

	return 0;
}

int add_data_frames(bool stream[], bool buff_package[][8], int start_package_index)
{
	int data_frame_index = start_package_index + 8;
	for (int i = 1; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			buff_package[i][j] = stream[data_frame_index];
			data_frame_index++;
		}
	}

	return 0;
}

int add_xor_frame(bool stream[], bool buff_package[][8], int start_package_index)
{
	int xor_frame_index = start_package_index + 48;
	for (int i = xor_frame_index; i < xor_frame_index + 8; i++)
		buff_package[XOR_FRAME_INDEX][i - xor_frame_index] = stream[i];

	return 0;
}

int calculate_checksum(bool buff_package[][8], bool buff_checksum[8])
{
	for (int i = 0; i < 8; i++)
	{
		buff_checksum[i] = buff_package[1][i] XOR buff_package[2][i] XOR
			buff_package[3][i] XOR buff_package[4][i] XOR buff_package[5][i];
	}

	return 0;
}

int check_data_frames(bool buff_package[][8], bool checksum[8])
{
	if (memcmp(buff_package[XOR_FRAME_INDEX], checksum, 8) != 0)
	{
		perror("check_data_frames error");
		return -1;
	}

	return 0;
}

int add_stop_frame(bool stream[], bool buff_package[][8], int start_package_index)
{
	int stop_frame_index = start_package_index + 56;
	for (int i = stop_frame_index; i < stop_frame_index + 8; i++)
		buff_package[STOP_FRAME_INDEX][i - stop_frame_index] = stream[i];

	return 0;
}

int check_stop_frame(bool buff_package[][8])
{
	if (memcmp(buff_package[STOP_FRAME_INDEX], stop_frame, 8) != 0)
	{
		perror("check_stop_frame error");
		return -1;
	}

	return 0;
}
