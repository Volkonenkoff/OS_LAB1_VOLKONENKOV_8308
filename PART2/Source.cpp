#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <Windows.h>

#pragma comment(lib, "winmm.lib")


using namespace std;


DWORD cl_part, ovr_opers;

HANDLE BaseFileH, CopiedFileH;
OVERLAPPED* Bovr, * Covr;
CHAR** inBuffer;
DWORD Recs, Read, Write;
DWORD fileSize;



VOID WINAPI EndReadFunc(DWORD error, DWORD countOfBytes, LPOVERLAPPED pOvr);
VOID WINAPI EndWriteFunc(DWORD error, DWORD countOfBytes, LPOVERLAPPED pOvr);



void cl_menu(int s)
{

	switch (s)
	{
	case 0:
		cl_part = 512;
		break;

	case 1:
		cl_part = 1024;
		break;

	case 2:
		cl_part = 2048;
		break;

	case 3:
		cl_part = 4096;
		break;

	case 4:
		cl_part = 8192;
		break;

	case 5:
		cl_part = 16384;
		break;

	case 6:
		cl_part = 32768;
		break;

	case 7:
		cl_part = 65536;
		break;

	default:
		cout << "Такого варианта нет, попробуйте еще раз" << endl;
		break;
	}



}

void ops_menu(int s)
{
	switch (s)
	{
	case 0:
		ovr_opers = 1;
		break;

	case 1:
		ovr_opers = 2;
		break;

	case 2:
		ovr_opers = 4;
		break;

	case 3:
		ovr_opers = 8;
		break;

	case 4:
		ovr_opers = 12;
		break;

	case 5:
		ovr_opers = 16;
		break;

	default:
		cout << "Такого варианта нет, попробуйте еще раз" << endl;
		break;

	}

}

int main()
{
	int s, o = 0, c = 0;
	TCHAR bname[MAX_PATH + 1] = {};
	TCHAR cname[MAX_PATH + 1] = {};
	string filen = "F.pdf\0";
	string temp;
	fstream res;
	DWORD t1, t2;
	BOOL B;
	res.open("result.txt", ios::out | ios::app);
	setlocale(LC_ALL, "Russian");
	do {

		copy(filen.begin(), filen.end(), bname);
		BaseFileH = CreateFile(bname, GENERIC_READ, 0,
			NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);
		temp = "NF" + to_string(c) + to_string(o) + ".pdf\0";
		copy(temp.begin(), temp.end(), cname);
		CopiedFileH = CreateFile(cname, GENERIC_WRITE, 0,
			NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);

		if (BaseFileH != INVALID_HANDLE_VALUE && CopiedFileH != INVALID_HANDLE_VALUE)
		{

			LARGE_INTEGER curPos;
			cl_menu(c);
			ops_menu(o);

			cout << "Выбранный размер блока: " << cl_part << endl;
			cout << "Выбранное число операций: " << ovr_opers << endl;
			cout << "=========================================" << endl;



			cout << "=========================================" << endl;
			fileSize = GetFileSize(BaseFileH, NULL);
			cout << "\nРазмер файла: " << fileSize / 1024 + (fileSize % 1024 > 0 ? 1 : 0) << endl;
			Recs = fileSize / cl_part + (fileSize % cl_part > 0 ? 1 : 0);
			cout << "Число записей: " << Recs << endl;
			cout << "=========================================" << endl;



			curPos.QuadPart = 0;
			inBuffer = new CHAR * [ovr_opers];
			for (DWORD k = 0; k < ovr_opers; k++)
				inBuffer[k] = new CHAR[cl_part];
			Bovr = new OVERLAPPED[ovr_opers];
			Covr = new OVERLAPPED[ovr_opers];
			t1 = timeGetTime();

			for (DWORD i = 0; i < ovr_opers; i++)
			{
				Bovr[i].hEvent = (HANDLE)i;
				Covr[i].hEvent = (HANDLE)i;
				Bovr[i].Offset = curPos.LowPart;
				Bovr[i].OffsetHigh = curPos.HighPart;
				if (curPos.QuadPart < fileSize)
				{
					B = ReadFileEx(BaseFileH, inBuffer[i], cl_part, &Bovr[i], EndReadFunc);
					if (B == 0) cout << "Ошибка при чтении из базового файла :" << GetLastError() << endl;
				}
				curPos.QuadPart = curPos.QuadPart + (LONGLONG)cl_part;
			}

			Read = 0;

			while (Read < Recs)
				SleepEx(INFINITE, TRUE);





			SetFilePointer(CopiedFileH, fileSize, NULL, FILE_BEGIN);
			SetEndOfFile(CopiedFileH);
			t2 = timeGetTime();
			cout << "=========================================" << endl;
			cout << "\nВремя копирования: ~" << t2 - t1 << "ms" << endl;
			cout << "=========================================" << endl;
			cout << "=========================================" << endl;
			cout << "=========================================" << endl;
			res << (t2 - t1) / 1000.0 << " ";
			CloseHandle(BaseFileH);
			CloseHandle(CopiedFileH);

			delete[] Bovr;
			Bovr = nullptr;
			delete[] Covr;
			Covr = nullptr;
			for (int i = 0; i < ovr_opers; i++)
				delete inBuffer[i];
			delete[] inBuffer;
			inBuffer = nullptr;
			cout << "Подождите..." << endl;
			SleepEx(5000, TRUE);
		}
		else
			cout << "Ошибка при открытии файла" << endl;
		cout << "\nДля продолжения введите - 1, для выхода - 0\n" << endl;
		cout << "Ввод>> : [";
		cin >> s;
		if (s == 1) {
			if (o + 1 == 6) { c++; o = 0; res << "\n"; }
			else { o++; }
		}
	} while (s && c < 8 && o < 6);
	res.close();
	return 0;

}





VOID CALLBACK EndReadFunc(DWORD error, DWORD countOfBytes, LPOVERLAPPED pOvr)

{
	if (error == 0)
	{
		BOOL B = 0;
		LARGE_INTEGER curPosIn;
		Read++;
		DWORD k = (DWORD)(pOvr->hEvent);
		curPosIn.LowPart = Bovr[k].Offset;
		curPosIn.HighPart = Bovr[k].OffsetHigh;
		Covr[k].Offset = Bovr[k].Offset;
		Covr[k].OffsetHigh = Bovr[k].OffsetHigh;
		B = WriteFileEx(CopiedFileH, inBuffer[k], cl_part, &Covr[k], EndWriteFunc);
		if (B == 0) cout << "Ошибка при записи в файл :" << GetLastError() << endl;
		curPosIn.QuadPart += cl_part * (LONGLONG)(ovr_opers);
		Bovr[k].Offset = curPosIn.LowPart;
		Bovr[k].OffsetHigh = curPosIn.HighPart;
	}
	
	return;

}



VOID CALLBACK EndWriteFunc(DWORD error, DWORD countOfBytes, LPOVERLAPPED pOvr)

{
	if (error == 0) {
		LARGE_INTEGER curPos;
		BOOL B = 0;
		Write++;
		DWORD k = (DWORD)(pOvr->hEvent);
		curPos.LowPart = Bovr[k].Offset;
		curPos.HighPart = Bovr[k].OffsetHigh;
		if (curPos.QuadPart < fileSize)
		{
			B = ReadFileEx(BaseFileH, inBuffer[k], cl_part, &Bovr[k], EndReadFunc);
			if (B == 0) cout << "Ошибка при чтении из базового файла :" << GetLastError() << endl;
		}
	}
	
	return;

}