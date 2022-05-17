#include"funcs.h"
#include<stdio.h>
#include<windows.h>

void findFile(char* pathName) {
	char currFile[MAX_PATH]; // ��ʱ�洢ÿ���ļ���
	memset(currFile, 0, MAX_PATH);
	sprintf(currFile, "%s\\*.*", pathName);
	_WIN32_FIND_DATAA findData;
	HANDLE hFile = FindFirstFile(currFile, &findData);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	int ret = 0;
	while (1) {
		memset(currFile, 0, MAX_PATH);
		sprintf(currFile, "%s\\%s", pathName, findData.cFileName);
		// ����ļ�����--�ļ������ļ��У�
		if (findData.cFileName[0] == '.'); //�������ļ��в����д���
		else if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))  //�������ͨ�ļ��У��ݹ����findFile����
			findFile(currFile);
		else //���򣬴���ǰ�ļ�(��ȫ����������ӡ�ļ���)
			deCode(currFile);

		ret = FindNextFile(hFile, &findData);
		if (!ret)
			break;
	}
}

void deCode(char* pathFile) {
	//�򿪴������ļ����������ܺ��ļ�
	FILE* fpSrc = fopen(pathFile, "rb"); //ֻ���ֽ���
	char buff[MAX_PATH];
	memset(buff, 0, MAX_PATH);
	//ȥ��.exe��׺
	if (dropEXE(pathFile, buff) != 0) //������ط�0����˵����ǰ�ļ���.exe��β�������н��ܴ���
		return;
	FILE* fpDst = fopen(buff, "wb"); //ֻд�ֽ���
	if (fpSrc == NULL || fpDst == NULL)
		return;
	//�Ե����ֽ�ѭ����ȡ�������ļ����ݣ���д������ļ���
	char currByte;
	while (1) {
		int count = fread(&currByte, 1, 1, fpSrc);
		if (count < 1) //û����
			break;
		currByte ^= 0x66; //�������ܷ�
		fwrite(&currByte, 1, 1, fpDst); //д������ļ�
	}
	fclose(fpSrc);
	fclose(fpDst);
	printf("Congrat! %s recoveried successfully!\n", buff);
	remove(pathFile);
}

int dropEXE(char* fpSrc, char* fpDst) {
	int n = strlen(fpSrc);
	if (n < 4)
		return 1;
	char check[5];
	for (int i = 0; i < 4; ++i)
		check[i] = *(fpSrc + n - 4 + i);
	check[4] = '\0';
	if (strcmp(check, ".exe") != 0) {
		printf("sorry, %s is not a .exe file, recovery failed!\n", fpSrc);
		return 1;
	}
	strncpy(fpDst, fpSrc, n - 4);
	return 0;
}