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
			//cout <<(i+1) <<". Диск " << char(i+65) << "://" << endl;
		}
	}
}

void DiskOutput(vector<string> buf)
{
	int j = 0;
	for (vector<string>::iterator i = buf.begin(); i != buf.end(); i++)
		cout <<++j<< ". Диск " << *i << endl;
}

void CopyMove()
{
	char k;
	TCHAR BufE[MAX_PATH + 1];
	TCHAR BufN[MAX_PATH + 1];
	
	BOOL B = 0;
	cout << "Копирование и перемещение файлов: " << endl;
	cout << "\t1. Копирование файла" << endl;
	cout << "\t2. Перемещение файла" << endl;
	cout << "\t3. Назад" << endl;
	do {
		cin >> k;
		switch (k)
		{
		case '1':
			cout << "Введите путь до текущего файла" << endl;
			wcin >> BufE;
			cout << "Введите путь до нового файла файла" << endl;
			wcin >> BufN;
			B = CopyFile(BufE, BufN, FALSE);
			if (B == 0) cerr << "Не удалось копировать файл " << GetLastError() << endl;
			else cout << "Файл успешно копирован" << endl;
			break;

		case '2':
			cout << "Введите путь до текущего файла" << endl;
			wcin >> BufE;
			cout << "Введите путь до нового файла файла" << endl;
			wcin >> BufN;
			B = MoveFile(BufE, BufN);
			if (B == 0) B = MoveFileEx(BufE, BufN, MOVEFILE_COPY_ALLOWED| MOVEFILE_REPLACE_EXISTING);
			if (B == 0) cerr << "Не удалось переместить файл  " << GetLastError() << endl;
			else cout << "Файл успешно перемещён";
			break;
		case '3':
			cout << "Вы вернулись назад\n" << endl;
			break;
		default:
			cout << "Такого пункта нет" << endl;
			break;
		}
	} while (k != '1' && k != '2' && k != '3' && B == 0);
}


DWORD AttrsInfo()
{
	DWORD t=NULL;
	char k;
	cout << "Введите номера атрибутов (без пробелов), которые вы хотите установить для этого файла: " << endl;
	cout << " \t1. Архивный" << endl;
	cout << " \t2. Скрытый" << endl;
	cout << " \t3. Без индексирования" << endl;
	cout << " \t4. С физически перемещенными данными" << endl;
	cout << " \t5. Только на чтение" << endl;
	cout << " \t6. Системный" << endl;
	cout << " \t7. Временный" << endl;
	cout << " \t8. Без атрибутов (убирает все другие атрибуты!)" << endl;
	cout << " \t0. Назад" << endl;
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
			cout << "Вы вернулись назад" << endl;
			break;
		default:
			cout << "Такого пункта нет\n" << endl;
			break;
		}
	} while (k != '0');
	
	return t;

}
void FileAttrs(DWORD d)
{
	cout << "Этот файл: " << endl;
	if (d & FILE_ATTRIBUTE_ARCHIVE) cout << "\t-Архивный" << endl;
	if (d & FILE_ATTRIBUTE_COMPRESSED) cout << "\t-Сжатый" << endl;
	if (d & FILE_ATTRIBUTE_DIRECTORY) cout << "\t-Каталог" << endl;
	if (d & FILE_ATTRIBUTE_ENCRYPTED) cout << "\t-Зашифрованный" << endl;
	if (d & FILE_ATTRIBUTE_HIDDEN) cout << "\t-Скрытый" << endl;
	if (d & FILE_ATTRIBUTE_NORMAL) cout << "\t-Без других атрибутов" << endl;
	if (d & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) cout << "\t-Не индексирован" << endl;
	if (d & FILE_ATTRIBUTE_OFFLINE) cout << "\t-Данные, которого были перемещены физически" << endl;
	if (d & FILE_ATTRIBUTE_READONLY) cout << "\t-Только на чтение" << endl;
	if (d & FILE_ATTRIBUTE_REPARSE_POINT) cout << "\t-Со связанной точкой повторной обработки" << endl;
	if (d & FILE_ATTRIBUTE_SPARSE_FILE) cout << "\t-Разреженный" << endl;
	if (d & FILE_ATTRIBUTE_SYSTEM) cout << "\t-Системного использования" << endl;
	if (d & FILE_ATTRIBUTE_TEMPORARY) cout << "\t-Временного хранения\n" << endl;
}

void FileInf(BY_HANDLE_FILE_INFORMATION t)
{
	SYSTEMTIME st;
	FileAttrs(t.dwFileAttributes);
	BOOL B = 0;
	B = FileTimeToSystemTime(&(t.ftCreationTime), &st);
	if (B != 0)
		cout << "Время и дата создания файла: " << st.wDay << "." << st.wMonth << "." << st.wYear << " " << st.wHour+3 << ":" << st.wMinute << endl;
	else cerr<<"Не удалось получить в/д создания файла. Ошибка "<<GetLastError()<<endl;
	B = FileTimeToSystemTime(&(t.ftLastAccessTime), &st);
	if (B!=0)
		cout << "Дата последнего обращения: " << st.wDay << "." << st.wMonth << "." << st.wYear <<" "<< st.wHour+3 << ":" << st.wMinute << endl;
	else cerr << "Не удалось получить дату последнего обращения. Ошибка " << GetLastError() << endl;
	B = FileTimeToSystemTime(&(t.ftLastWriteTime), &st);
	if (B!=0)
		cout << "Дата последней модификации: " << st.wDay << "." << st.wMonth << "." << st.wYear << " " << st.wHour+3 << ":" << st.wMinute << endl;
	else cerr << "Не удалось получить дату последней модификации. Ошибка " << GetLastError() << endl;
	cout << "Серийный номер диска, на котором находится файл: " << &(t.dwVolumeSerialNumber) << endl;
	cout << "Старшее слово размера файла: " << &(t.nFileSizeHigh) << endl;
	cout << "Младшее слово размера файла: " << &(t.nFileSizeLow) << endl;
	cout << "Кол-во ссылок на файл в ФС: " << &(t.nNumberOfLinks) << endl;
	cout << "Старшее слово уникального 64-битного идентификатора файла: " << &(t.nFileIndexHigh) << endl;
	cout << "Младшее слово уникального 64-битного идентификатора файла: " << &(t.nFileIndexLow) << endl;
}

void FileT(HANDLE hf)
{
	FILETIME CT, LAT, LWT;
	SYSTEMTIME st;
	BOOL B = 0;
	B = GetFileTime(hf, &CT, &LAT, &LWT);
	B = FileTimeToSystemTime(&CT, &st);
	if (B != 0)
		cout << "Время и дата создания файла: " << st.wDay << "." << st.wMonth << "." << st.wYear << " " << st.wHour+3 << ":" << st.wMinute << endl;
	else cerr << "Не удалось получить в/д создания файла. Ошибка " << GetLastError() << endl;
	B = FileTimeToSystemTime(&LAT, &st);
	if (B != 0)
		cout << "Дата последнего обращения: " << st.wDay << "." << st.wMonth << "." << st.wYear << " " << st.wHour+3 << ":" << st.wMinute << endl;
	else cerr << "Не удалось получить дату последнего обращения. Ошибка " << GetLastError() << endl;
	B = FileTimeToSystemTime(&LWT, &st);
	if (B != 0)
		cout << "Дата последней модификации: " << st.wDay << "." << st.wMonth << "." << st.wYear << " " << st.wHour+3 << ":" << st.wMinute << endl;

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
			cerr << "Не удалось установить текущее время, как время воздействия с файлом. Ошибка " << GetLastError() << endl;
	}
	else cerr << "Не удалось установить текущее время, как время воздействия с файлом. Ошибка " << GetLastError() << endl;


}

void FileCharset()
{
	HANDLE fHandle;
	BY_HANDLE_FILE_INFORMATION fInf;
	DWORD w;
	BOOL B = 0;
	TCHAR BufN[MAX_PATH + 1];
	char k;
	cout << "Введите путь по которому нужно изменить файл вместе с именем и расширинием файла на конце" << endl;
	cout << "Пример: C:\\myfile.txt" << endl;
	wcin >> BufN;
	fHandle = CreateFile(BufN, GENERIC_WRITE,
		NULL, NULL, OPEN_EXISTING, FILE_WRITE_ATTRIBUTES, NULL);
	if (fHandle != INVALID_HANDLE_VALUE) {
		
		
		do {
			cout << "Работа с файлом " << endl;
			cout << "\t1. Атрибуты файла" << endl;
			cout << "\t2. Изменить атрибуты файла" << endl;
			cout << "\t3. Информация о файле" << endl;
			cout << "\t4. Информация о временном взаимодействии с файлом" << endl;
			cout << "\t5. Установить текущее время взаимодействии с файлом" << endl;
			cout << "\t0. Выйти" << endl;
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
				if (B == 0) cerr << "Не удалось установить атрибуты, попробуйте ещё раз. Ошибка "<<GetLastError() << endl;
				break;
			case '3':
				system("cls");
				B = GetFileInformationByHandle(fHandle, &fInf);
				if (B==0)  cerr << "Не удалось получить информацию о файле. Ошибка " << GetLastError() << endl;
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
				cout << "Текущее время установленно, как новое временное взаимодействие с файлом" << endl;
				break;
			case '0':
				cout << "Вы вернулись назад" << endl;
				CloseHandle(fHandle);
				break;
			default:
				cout << "Такого пункта нет" << endl;
				break;
			}
		} while (k != '0');
	}
	else cout << "Не удалось открыть файл" << endl;
}

void CreationFile()
{
	HANDLE fHandle;
	TCHAR BufN[MAX_PATH + 1];
	cout << "Введите путь по которому нужно создать файл вместе с именем и расширинием файла на конце" << endl;
	cout << "Пример: C:\\myfile.txt"<<endl;
	wcin >> BufN;
	fHandle = CreateFile(BufN, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL, NULL, CREATE_NEW, NULL, NULL);
	if (fHandle != INVALID_HANDLE_VALUE)
		cout << "Файл успешно создан" << endl;
	else cerr << "При создании файла произошла ошибка " << GetLastError()<<endl;
	CloseHandle(fHandle);
}
void CreationDirectory()
{
	char k;

	TCHAR BufN[MAX_PATH+1];
	BOOL B = 0;
	cout << "Работа с каталогом: " << endl;
	cout << "\t1. Создание каталога" << endl;
	cout << "\t2. Удаление каталога" << endl;
	cout << "\t3. Назад" << endl;
	do {
		cin >> k;
		switch (k)
		{
		case '1':
			cout << "Введите путь с указание нового каталога" << endl;
			wcin >> BufN;
			B = CreateDirectory(BufN, NULL);
			if (B == 0) cerr << "Не удалось создать каталог " << GetLastError() << endl;
			else cout << "Каталог создан успешно!" << endl;
			
			break;
		case '2':
			cout << "Введите путь с указание существующего диалога каталога" << endl;
			wcin >> BufN;
			B = RemoveDirectory(BufN);
			if (B == 0) cerr << "Не удалось удалить каталог " << GetLastError() << endl;
			else cout << "Каталог удалён успешно!" << endl;

			break;
		case '3':
			cout << "Вы вернулись назад\n" << endl;
			break;
		default:
			cout << "Такого пункта нет" << endl;
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
		cout << "Тип неизвестен";
	}
	break;
	case DRIVE_NO_ROOT_DIR:
	{
		cout << "Неверный путь";
	}
	break;
	case DRIVE_REMOVABLE:
	{
		cout << "Съемный";
	}
	break;
	case DRIVE_FIXED:
	{
		cout << "Фиксированный";
	}
	break;
	case DRIVE_REMOTE:
	{
		cout << "Удаленный (сетевой)";
	}
	break;
	case DRIVE_CDROM:
	{
		cout << "CD-ROM (дисковод)";
	}
	break;
	case DRIVE_RAMDISK:
	{
		cout << "RAM-диск";
	}
	break;
	}
}

void DetailedVolumeInfo(BOOL b, TCHAR* VNB, TCHAR* FS, DWORD SN, DWORD MF, DWORD FSO) {
	if (b != false)
	{

		cout << "Имя раздела: ";
		for (int i = 0, j = 0; i < MAX_PATH && j!=1; i++)
		{
			if (VNB[i] != '\0')
				wcout << VNB[i];
			else j = 1;
		}
		cout << endl;
		cout << "Файловая система: ";
		for (int i = 0, j = 0; i < MAX_PATH && j != 1; i++)
		{
			if (FS[i] != '\0')
				wcout << FS[i];
			else j = 1;
		}
		cout<< endl;
		cout << "Максимальная длина файла: " << MF << endl;
		cout << "Опции файловой системы: " <<FSO << endl;
		cout << "Серийный номер раздела: " << SN << endl;
	}
	else cout << "\nПроизошла ошибка: " << GetLastError() << endl;
}

void DetailedDiskSpaceInfo(BOOL b, DWORD S, DWORD BPS, DWORD FC, DWORD TC) {
	if (b != false)
	{
		cout << "Число секторов в кластере: " << S << endl;
		cout << "Число байт в секторе: " << BPS << endl;
		cout << "Свободных кластеров: " << FC << endl;
		cout << "Всего кластеров: " << TC << endl;
		cout << "Доступно места " <<(long double(BPS)/1024.0/1024.0/1024.)*FC*S << " GB"<<endl;
		cout << "Всего места:" << (long double(BPS) / 1024.0 / 1024.0 / 1024.) * TC * S << " GB" << endl;
	}
	else cout << "\nПроизошла ошибка: " << GetLastError() << endl;
}

void DiskFullInformation(LPCTSTR disk)
{
	TCHAR VolumeNameBuf[MAX_PATH+1];
	TCHAR FileSystem[MAX_PATH+1];
	
	DWORD VolumeSerialNumber,MaxFile, FSOpt, Sectors, BytePerSector, freeClusters, totalClusters;

	cout << "\n===================================\n" << endl;
	cout << "Тип диска: ";
	UINT d = GetDriveType(disk);
	DetailedDriveType(d);
	cout << endl;
	cout << "\n===================================\n" << endl;
	cout << "Информация о разделе: ";

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

	cout << "Свободно места: ";
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
	else cout << "Такого диска нет";
			
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
		cout << "Выберете нужный вам пункт меню, нажав на цифру на клавиатуре, а затем нажав Enter:\n" << endl;
		cout << "1. Список дисков"<<endl;
		cout << "2. Получить информацию о диске" << endl;
		cout << "3. Создание/удаление каталогов" << endl;
		cout << "4. Создание файлов в каталогах" << endl;
		cout << "5. Копирование/перемещение файлов" << endl;
		cout << "6. Анализ и изменение атрибутов файлов" << endl;
		cout << "\n Ввод>>: ";
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
					cout << "\nВыберете нужный вам диск, нажав на цифру на клавиатуре, а затем нажав Enter:\n" << endl;
					DiskOutput(buf);
					cout << "\n Ввод>>: ";
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
				cout << "Выходим..." << endl;
				break;
			default:
				system("cls");
				cout << "Такого пункта не существует\n"<<endl;
				break;
	
		}
	} while (key!='0');
	
	return 0;
}