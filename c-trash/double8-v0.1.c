#include <stdio.h>
#include <string.h>

#define DATA_LENGTH 256
#define XOR ^

int fread(const char* file_name, char buff[]);
int str2frame(char* str, bool buff_frame[]);
int find_start_frame(bool stream[]);
int fill_start_frame(bool stream[], bool buff[][8], int start_index);
int get_data_frames(bool stream[], bool buff[][8], int start_index);
int get_xor_frame(bool stream[], bool buff[][8], int start_index);
int check_data_frames(bool buff[][8]);
int find_stop_frame(bool stream[], bool buff[][8], int start_index);

int main()
{
	char buff[DATA_LENGTH];
	bool frame[DATA_LENGTH];
	bool package[8][8];

	int startindx = 0;

	if (fread("in-frame.txt", buff) < 0)
	{
		printf("An error has occurred in the program.");
		return -1;
	}

	str2frame(buff, frame);

	if ((startindx = find_start_frame(frame)) < 0)
	{
		printf("An error has occurred in the program (find_frame()).");
		return -1;
	}

	printf("%d\n", startindx);

	fill_start_frame(frame, package, startindx);
	get_data_frames(frame, package, startindx);
	get_xor_frame(frame, package, startindx);
	if (check_data_frames(package) < 0)
	{
		printf("xor is incorrect.");
		return -1;
	}

	find_stop_frame(frame, package, startindx);

	printf("package is \n");

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			printf("%d", package[i][j]);
		}
		printf("\n");
	}

	return 0;
}

int fread(const char* file_name, char buff[])
{
	FILE* fstream;

	if ((fstream = fopen(file_name, "r")) == NULL)
	{
		perror("Couldn't open the file.");
		return -1;
	}

	if (fscanf(fstream, "%s", buff) < 0)
	{
		perror("Couldn't read the file.");
		return -1;
	}

	fclose(fstream);

	return 0;
}

int str2frame(char* str, bool buff_frame[])
{
	for (int i = 0; str[i] != 0; i++)
		buff_frame[i] = str[i] - '0';

	return 0;
}

int find_start_frame(bool stream[])
{
	int start_frame[8] = { 0, 1, 1, 1, 1, 1, 1, 0 };

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

	return -1;
}

int fill_start_frame(bool stream[], bool buff[][8], int start_index)
{
	for (int i = start_index; i < start_index + 8; i++)
	{
		buff[0][i - start_index] = stream[i];
	}

	return 0;
}

int get_data_frames(bool stream[], bool buff[][8], int start_index)
{
	int k = start_index + 8;
	for (int i = 1; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			buff[i][j] = stream[k];
			k++;
		}
	}

	return 0;
}

int get_xor_frame(bool stream[], bool buff[][8], int start_frame)
{
	int k = 0;
	for (int i = start_frame + 48; i < start_frame + 56; i++)
	{
		buff[6][k] = stream[i];
		k++;
	}

	return 0;
}

int check_data_frames(bool buff[][8])
{
	bool checksum[8];
	for (int i = 0; i < 8; i++)
	{
		checksum[i] = buff[1][i] XOR buff[2][i] XOR buff[3][i] XOR
			buff[4][i] XOR buff[5][i];
	}

	printf("xor in function is ");
	for (int i = 0; i < 8; i++)
	{
		printf("%d", checksum[i]);
	}
	printf("\n");

	if (memcmp(buff[6], checksum, 8) != 0)
		return -1;
	
	return 0;
}

int find_stop_frame(bool stream[], bool buff[][8], int start_index)
{
	int k = start_index + 56;
	for (int i = 0; i < 8; i++)
	{
		buff[7][i] = stream[k];
		k++;
	}
	return 0;
}
