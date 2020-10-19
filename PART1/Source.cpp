#include <iostream>
#include <windows.h>
#include <vector>
#include <bitset>
#define ENG_ALPH 26

using namespace std;



void DiskBufferization(DWORD DD, vector<string> &buff)
{

	for (int i = 0; i <ENG_ALPH; i++)
	{
		if (((DD>>i)&1) != 0)
		{
			char t = i + 97;
			buff.push_back(string()+t+":"+"\\"+"\0");
			//cout <<(i+1) <<". ���� " << char(i+65) << "://" << endl;
		}
	}
}

void DiskOutput(vector<string> buf)
{
	int j = 0;
	for (vector<string>::iterator i = buf.begin(); i != buf.end(); i++)
		cout <<++j<< ". ���� " << *i << endl;
}

void CopyMove()
{
	char k;
	TCHAR BufE[MAX_PATH + 1];
	TCHAR BufN[MAX_PATH + 1];
	
	BOOL B = 0;
	cout << "����������� � ����������� ������: " << endl;
	cout << "\t1. ����������� �����" << endl;
	cout << "\t2. ����������� �����" << endl;
	cout << "\t3. �����" << endl;
	do {
		cin >> k;
		switch (k)
		{
		case '1':
			cout << "������� ���� �� �������� �����" << endl;
			wcin >> BufE;
			cout << "������� ���� �� ������ ����� �����" << endl;
			wcin >> BufN;
			B = CopyFile(BufE, BufN, FALSE);
			if (B == 0) cerr << "�� ������� ���������� ���� " << GetLastError() << endl;
			else cout << "���� ������� ���������" << endl;
			break;

		case '2':
			cout << "������� ���� �� �������� �����" << endl;
			wcin >> BufE;
			cout << "������� ���� �� ������ ����� �����" << endl;
			wcin >> BufN;
			B = MoveFile(BufE, BufN);
			if (B == 0) B = MoveFileEx(BufE, BufN, MOVEFILE_COPY_ALLOWED| MOVEFILE_REPLACE_EXISTING);
			if (B == 0) cerr << "�� ������� ����������� ����  " << GetLastError() << endl;
			else cout << "���� ������� ���������";
			break;
		case '3':
			cout << "�� ��������� �����\n" << endl;
			break;
		default:
			cout << "������ ������ ���" << endl;
			break;
		}
	} while (k != '1' && k != '2' && k != '3' && B == 0);
}


DWORD AttrsInfo()
{
	DWORD t=NULL;
	char k;
	cout << "������� ������ ��������� (��� ��������), ������� �� ������ ���������� ��� ����� �����: " << endl;
	cout << " \t1. ��������" << endl;
	cout << " \t2. �������" << endl;
	cout << " \t3. ��� ��������������" << endl;
	cout << " \t4. � ��������� ������������� �������" << endl;
	cout << " \t5. ������ �� ������" << endl;
	cout << " \t6. ���������" << endl;
	cout << " \t7. ���������" << endl;
	cout << " \t8. ��� ��������� (������� ��� ������ ��������!)" << endl;
	cout << " \t0. �����" << endl;
	do {
		cin >> k;
		switch (k)
		{
		case '1':
			t = t | FILE_ATTRIBUTE_ARCHIVE;
			break;
		case '2':
			t = t | FILE_ATTRIBUTE_HIDDEN;
			break;
		case '3':
			t = t | FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
			break;
		case '4':
			t = t | FILE_ATTRIBUTE_OFFLINE;
			break;
		case '5':
			t = t | FILE_ATTRIBUTE_READONLY;
			break;
		case '6':
			t = t | FILE_ATTRIBUTE_SYSTEM;
			break;
		case '7':
			t = t | FILE_ATTRIBUTE_TEMPORARY;
			break;
		case '8':
			t = t | FILE_ATTRIBUTE_NORMAL;
			break;
		case '0':
			cout << "�� ��������� �����" << endl;
			break;
		default:
			cout << "������ ������ ���\n" << endl;
			break;
		}
	} while (k != '0');
	
	return t;

}
void FileAttrs(DWORD d)
{
	cout << "���� ����: " << endl;
	if (d & FILE_ATTRIBUTE_ARCHIVE) cout << "\t-��������" << endl;
	if (d & FILE_ATTRIBUTE_COMPRESSED) cout << "\t-������" << endl;
	if (d & FILE_ATTRIBUTE_DIRECTORY) cout << "\t-�������" << endl;
	if (d & FILE_ATTRIBUTE_ENCRYPTED) cout << "\t-�������������" << endl;
	if (d & FILE_ATTRIBUTE_HIDDEN) cout << "\t-�������" << endl;
	if (d & FILE_ATTRIBUTE_NORMAL) cout << "\t-��� ������ ���������" << endl;
	if (d & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) cout << "\t-�� ������������" << endl;
	if (d & FILE_ATTRIBUTE_OFFLINE) cout << "\t-������, �������� ���� ���������� ���������" << endl;
	if (d & FILE_ATTRIBUTE_READONLY) cout << "\t-������ �� ������" << endl;
	if (d & FILE_ATTRIBUTE_REPARSE_POINT) cout << "\t-�� ��������� ������ ��������� ���������" << endl;
	if (d & FILE_ATTRIBUTE_SPARSE_FILE) cout << "\t-�����������" << endl;
	if (d & FILE_ATTRIBUTE_SYSTEM) cout << "\t-���������� �������������" << endl;
	if (d & FILE_ATTRIBUTE_TEMPORARY) cout << "\t-���������� ��������\n" << endl;
}

void FileInf(BY_HANDLE_FILE_INFORMATION t)
{
	SYSTEMTIME st;
	FileAttrs(t.dwFileAttributes);
	BOOL B = 0;
	B = FileTimeToSystemTime(&(t.ftCreationTime), &st);
	if (B != 0)
		cout << "����� � ���� �������� �����: " << st.wDay << "." << st.wMonth << "." << st.wYear << " " << st.wHour+3 << ":" << st.wMinute << endl;
	else cerr<<"�� ������� �������� �/� �������� �����. ������ "<<GetLastError()<<endl;
	B = FileTimeToSystemTime(&(t.ftLastAccessTime), &st);
	if (B!=0)
		cout << "���� ���������� ���������: " << st.wDay << "." << st.wMonth << "." << st.wYear <<" "<< st.wHour+3 << ":" << st.wMinute << endl;
	else cerr << "�� ������� �������� ���� ���������� ���������. ������ " << GetLastError() << endl;
	B = FileTimeToSystemTime(&(t.ftLastWriteTime), &st);
	if (B!=0)
		cout << "���� ��������� �����������: " << st.wDay << "." << st.wMonth << "." << st.wYear << " " << st.wHour+3 << ":" << st.wMinute << endl;
	else cerr << "�� ������� �������� ���� ��������� �����������. ������ " << GetLastError() << endl;
	cout << "�������� ����� �����, �� ������� ��������� ����: " << &(t.dwVolumeSerialNumber) << endl;
	cout << "������� ����� ������� �����: " << &(t.nFileSizeHigh) << endl;
	cout << "������� ����� ������� �����: " << &(t.nFileSizeLow) << endl;
	cout << "���-�� ������ �� ���� � ��: " << &(t.nNumberOfLinks) << endl;
	cout << "������� ����� ����������� 64-������� �������������� �����: " << &(t.nFileIndexHigh) << endl;
	cout << "������� ����� ����������� 64-������� �������������� �����: " << &(t.nFileIndexLow) << endl;
}

void FileT(HANDLE hf)
{
	FILETIME CT, LAT, LWT;
	SYSTEMTIME st;
	BOOL B = 0;
	B = GetFileTime(hf, &CT, &LAT, &LWT);
	B = FileTimeToSystemTime(&CT, &st);
	if (B != 0)
		cout << "����� � ���� �������� �����: " << st.wDay << "." << st.wMonth << "." << st.wYear << " " << st.wHour+3 << ":" << st.wMinute << endl;
	else cerr << "�� ������� �������� �/� �������� �����. ������ " << GetLastError() << endl;
	B = FileTimeToSystemTime(&LAT, &st);
	if (B != 0)
		cout << "���� ���������� ���������: " << st.wDay << "." << st.wMonth << "." << st.wYear << " " << st.wHour+3 << ":" << st.wMinute << endl;
	else cerr << "�� ������� �������� ���� ���������� ���������. ������ " << GetLastError() << endl;
	B = FileTimeToSystemTime(&LWT, &st);
	if (B != 0)
		cout << "���� ��������� �����������: " << st.wDay << "." << st.wMonth << "." << st.wYear << " " << st.wHour+3 << ":" << st.wMinute << endl;

}

void SFileT(HANDLE hf)
{
	SYSTEMTIME mt;
	FILETIME ft;
	BOOL B = 0;
	GetSystemTime(&mt);
	B=SystemTimeToFileTime(&mt, &ft);
	if (B != 0)
	{
		B = SetFileTime(hf, &ft, &ft, &ft);
		if (B == 0)
			cerr << "�� ������� ���������� ������� �����, ��� ����� ����������� � ������. ������ " << GetLastError() << endl;
	}
	else cerr << "�� ������� ���������� ������� �����, ��� ����� ����������� � ������. ������ " << GetLastError() << endl;


}

void FileCharset()
{
	HANDLE fHandle;
	BY_HANDLE_FILE_INFORMATION fInf;
	DWORD w;
	BOOL B = 0;
	TCHAR BufN[MAX_PATH + 1];
	char k;
	cout << "������� ���� �� �������� ����� �������� ���� ������ � ������ � ����������� ����� �� �����" << endl;
	cout << "������: C:\\myfile.txt" << endl;
	wcin >> BufN;
	fHandle = CreateFile(BufN, GENERIC_WRITE,
		NULL, NULL, OPEN_EXISTING, FILE_WRITE_ATTRIBUTES, NULL);
	if (fHandle != INVALID_HANDLE_VALUE) {
		
		
		do {
			cout << "������ � ������ " << endl;
			cout << "\t1. �������� �����" << endl;
			cout << "\t2. �������� �������� �����" << endl;
			cout << "\t3. ���������� � �����" << endl;
			cout << "\t4. ���������� � ��������� �������������� � ������" << endl;
			cout << "\t5. ���������� ������� ����� �������������� � ������" << endl;
			cout << "\t0. �����" << endl;
			cin >> k;
			switch (k)
			{
			case '1':
				system("cls");
				w = GetFileAttributes(BufN);
				FileAttrs(w);
				break;
			case '2':
				system("cls");
				w = AttrsInfo();
				B = SetFileAttributes(BufN, w);
				
				if (B!=0) FileAttrs(w);
				if (B == 0) cerr << "�� ������� ���������� ��������, ���������� ��� ���. ������ "<<GetLastError() << endl;
				break;
			case '3':
				system("cls");
				B = GetFileInformationByHandle(fHandle, &fInf);
				if (B==0)  cerr << "�� ������� �������� ���������� � �����. ������ " << GetLastError() << endl;
				else {
					FileInf(fInf);
				}
				break;
			case '4':
				system("cls");
				FileT(fHandle);
				break;
			case '5':
				system("cls");
				SFileT(fHandle);
				cout << "������� ����� ������������, ��� ����� ��������� �������������� � ������" << endl;
				break;
			case '0':
				cout << "�� ��������� �����" << endl;
				CloseHandle(fHandle);
				break;
			default:
				cout << "������ ������ ���" << endl;
				break;
			}
		} while (k != '0');
	}
	else cout << "�� ������� ������� ����" << endl;
}

void CreationFile()
{
	HANDLE fHandle;
	TCHAR BufN[MAX_PATH + 1];
	cout << "������� ���� �� �������� ����� ������� ���� ������ � ������ � ����������� ����� �� �����" << endl;
	cout << "������: C:\\myfile.txt"<<endl;
	wcin >> BufN;
	fHandle = CreateFile(BufN, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL, NULL, CREATE_NEW, NULL, NULL);
	if (fHandle != INVALID_HANDLE_VALUE)
		cout << "���� ������� ������" << endl;
	else cerr << "��� �������� ����� ��������� ������ " << GetLastError()<<endl;
	CloseHandle(fHandle);
}
void CreationDirectory()
{
	char k;

	TCHAR BufN[MAX_PATH+1];
	BOOL B = 0;
	cout << "������ � ���������: " << endl;
	cout << "\t1. �������� ��������" << endl;
	cout << "\t2. �������� ��������" << endl;
	cout << "\t3. �����" << endl;
	do {
		cin >> k;
		switch (k)
		{
		case '1':
			cout << "������� ���� � �������� ������ ��������" << endl;
			wcin >> BufN;
			B = CreateDirectory(BufN, NULL);
			if (B == 0) cerr << "�� ������� ������� ������� " << GetLastError() << endl;
			else cout << "������� ������ �������!" << endl;
			
			break;
		case '2':
			cout << "������� ���� � �������� ������������� ������� ��������" << endl;
			wcin >> BufN;
			B = RemoveDirectory(BufN);
			if (B == 0) cerr << "�� ������� ������� ������� " << GetLastError() << endl;
			else cout << "������� ����� �������!" << endl;

			break;
		case '3':
			cout << "�� ��������� �����\n" << endl;
			break;
		default:
			cout << "������ ������ ���" << endl;
			break;
		}
	} while (k!='1'&& k!='2'&& k!='3'&& B==0);
	
	
}



void DetailedDriveType(UINT d)
{
	switch (d)
	{
	case DRIVE_UNKNOWN:
	{
		cout << "��� ����������";
	}
	break;
	case DRIVE_NO_ROOT_DIR:
	{
		cout << "�������� ����";
	}
	break;
	case DRIVE_REMOVABLE:
	{
		cout << "�������";
	}
	break;
	case DRIVE_FIXED:
	{
		cout << "�������������";
	}
	break;
	case DRIVE_REMOTE:
	{
		cout << "��������� (�������)";
	}
	break;
	case DRIVE_CDROM:
	{
		cout << "CD-ROM (��������)";
	}
	break;
	case DRIVE_RAMDISK:
	{
		cout << "RAM-����";
	}
	break;
	}
}

void DetailedVolumeInfo(BOOL b, TCHAR* VNB, TCHAR* FS, DWORD SN, DWORD MF, DWORD FSO) {
	if (b != false)
	{

		cout << "��� �������: ";
		for (int i = 0, j = 0; i < MAX_PATH && j!=1; i++)
		{
			if (VNB[i] != '\0')
				wcout << VNB[i];
			else j = 1;
		}
		cout << endl;
		cout << "�������� �������: ";
		for (int i = 0, j = 0; i < MAX_PATH && j != 1; i++)
		{
			if (FS[i] != '\0')
				wcout << FS[i];
			else j = 1;
		}
		cout<< endl;
		cout << "������������ ����� �����: " << MF << endl;
		cout << "����� �������� �������: " <<FSO << endl;
		cout << "�������� ����� �������: " << SN << endl;
	}
	else cout << "\n��������� ������: " << GetLastError() << endl;
}

void DetailedDiskSpaceInfo(BOOL b, DWORD S, DWORD BPS, DWORD FC, DWORD TC) {
	if (b != false)
	{
		cout << "����� �������� � ��������: " << S << endl;
		cout << "����� ���� � �������: " << BPS << endl;
		cout << "��������� ���������: " << FC << endl;
		cout << "����� ���������: " << TC << endl;
		cout << "�������� ����� " <<(long double(BPS)/1024.0/1024.0/1024.)*FC*S << " GB"<<endl;
		cout << "����� �����:" << (long double(BPS) / 1024.0 / 1024.0 / 1024.) * TC * S << " GB" << endl;
	}
	else cout << "\n��������� ������: " << GetLastError() << endl;
}

void DiskFullInformation(LPCTSTR disk)
{
	TCHAR VolumeNameBuf[MAX_PATH+1];
	TCHAR FileSystem[MAX_PATH+1];
	
	DWORD VolumeSerialNumber,MaxFile, FSOpt, Sectors, BytePerSector, freeClusters, totalClusters;

	cout << "\n===================================\n" << endl;
	cout << "��� �����: ";
	UINT d = GetDriveType(disk);
	DetailedDriveType(d);
	cout << endl;
	cout << "\n===================================\n" << endl;
	cout << "���������� � �������: ";

	BOOL b = GetVolumeInformation(disk,
		VolumeNameBuf,
		MAX_PATH,
		&VolumeSerialNumber,
		&MaxFile,
		&FSOpt,
		FileSystem,
		MAX_PATH);
	DetailedVolumeInfo(b, VolumeNameBuf, FileSystem, VolumeSerialNumber, MaxFile, FSOpt);
	cout << endl;
	cout << "\n===================================\n" << endl;

	cout << "�������� �����: ";
	b = GetDiskFreeSpace(disk,
		&Sectors,
		&BytePerSector,
		&freeClusters,
		&totalClusters);
	DetailedDiskSpaceInfo(b, Sectors, BytePerSector,freeClusters, totalClusters);
	cout << endl;
	cout << "\n===================================\n" << endl;

}

void DiskFind(unsigned int i, vector<string> buf)
{

	if (i <= buf.size()) {
		vector<string>::iterator it = buf.begin();
		for (unsigned int k = 1; k != i; k++)
			it++;
		std::wstring stemp = std::wstring((*it).begin(), (*it).end());
		LPCTSTR sw = stemp.c_str();
		DiskFullInformation(sw);
	}
	else cout << "������ ����� ���";
			
}




int main(char* argv[], int argc)
{
	DWORD diskdir;
	char key;
	vector<string> buf = {};
	system("chcp 1251");
	//setlocale(LC_ALL, ".1251");
	system("cls");
	cout.clear();
	cin.clear();
	diskdir = GetLogicalDrives();
	DiskBufferization(diskdir, buf);
	do 
	{
		cout << "\n";
		cout << "�������� ������ ��� ����� ����, ����� �� ����� �� ����������, � ����� ����� Enter:\n" << endl;
		cout << "1. ������ ������"<<endl;
		cout << "2. �������� ���������� � �����" << endl;
		cout << "3. ��������/�������� ���������" << endl;
		cout << "4. �������� ������ � ���������" << endl;
		cout << "5. �����������/����������� ������" << endl;
		cout << "6. ������ � ��������� ��������� ������" << endl;
		cout << "\n ����>>: ";
		cin >> key;
		cout << endl;
		switch(key)
		{

			case '1':
				{
					system("cls");
					DiskOutput(buf);
				}
				break;
			case '2':
				{
					system("cls");
					cout << "\n�������� ������ ��� ����, ����� �� ����� �� ����������, � ����� ����� Enter:\n" << endl;
					DiskOutput(buf);
					cout << "\n ����>>: ";
					unsigned int i;
					cin >> i;
					DiskFind(i, buf);

				}
				
				break;
			case '3':
				system("cls");
				CreationDirectory();
				break;
			case '4':
				system("cls");
				CreationFile();
				break;
			case '5':
				system("cls");
				CopyMove();
				break;
			case '6':
				system("cls");
				FileCharset();
				break;
			case '0':
				system("cls");
				cout << "�������..." << endl;
				break;
			default:
				system("cls");
				cout << "������ ������ �� ����������\n"<<endl;
				break;
	
		}
	} while (key!='0');
	
	return 0;
}