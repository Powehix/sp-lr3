#include <windows.h>
#include <iostream>
using namespace std;

void ComputerName();
void TimeInformation();
void UserInformation();
void Directories();
void DiskInformation();
void DriveType(char *temp);
void DiskFreeSpace(char *temp);

int main()
{
	setlocale(LC_ALL, "Russian");   //для корректного вывода языка, используемоего пользователем (3-е задание)

	//1 задание: определить имя компьютера (GetComputerName)
	ComputerName();

	//2 задание: определить локальное время и часовой пояс (GetLocalTime, GetTimeZoneInformation)
	TimeInformation();

	//3 задание: определить имя и язык текущего пользователя (GetUserDefaultLangID, VerLanguageName, GetUserName)
	UserInformation();

	//4 задание: определить текущую директорию, директорию ОС, системную директорию (GetCurrentDirectory, GetWindowsDirectory, GetSystemDirectory)
	Directories();

	//5 задание: определить список логических дисков, их типы и свободное место на диске в байтах (GetLogicalDrivers, GetLogicalDrivesString, GetDriveType, GetDiskFreeSpace)
	DiskInformation();

	system("PAUSE");
	return 0;
}


void ComputerName()
{
	char buffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = sizeof(buffer);
	GetComputerName(buffer, &size);
	cout << "Computer name: " << buffer << endl;
}

void TimeInformation()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	cout << "Local time: " << st.wHour << ":" << st.wMinute << endl;    //выводит текущее время (часы и минуты)

	TIME_ZONE_INFORMATION tzi;
	GetTimeZoneInformation(&tzi);
	cout << "Timezone: " << tzi.Bias / 60 << endl;   //выводит смещение (разницу) в минутах между UTC и местным временем (деление на 60 для определения часов)
}

void UserInformation()
{
	char buffer[256];
	DWORD size = sizeof(buffer);
	GetUserName(buffer, &size);
	cout << "User name: " << buffer << endl;

	VerLanguageName(GetUserDefaultLangID(), buffer, 256);
	cout << "User default language: " << buffer << endl;
}

void Directories()
{
	char buffer[256];
	GetCurrentDirectory(256, buffer);
	cout << "Current directory: " << buffer << endl;

	GetWindowsDirectory(buffer, 256);
	cout << "Windows directory: " << buffer << endl;

	GetSystemDirectory(buffer, 256);
	cout << "System directory: " << buffer << endl;
}

void DiskInformation()
{
	DWORD drives = GetLogicalDrives();
	unsigned int mask = 1;
	cout << "Available disk drives (1st way): ";

	for (int i = 0; i <= 31; i++)
	{
		if (drives & mask)
			cout << char(65 + i) << " ";   //выводит символ в соответствии с таблицей ASCII
		mask <<= 1;
	}

	char buffer[256];
	GetLogicalDriveStrings(256, buffer);
	cout << "\nAvailable disk drives (2nd way): \n";

	char *temp = buffer;
	while (*temp)
	{
		cout << temp;
		DriveType(temp);   //определяет тип диска
		DiskFreeSpace(temp);   //определяет свободное место на диске в байтах
		temp = &temp[strlen(temp) + 1];   //переносит указатель
	}
}

void DriveType(char *temp) 
{
	unsigned int type = GetDriveType(temp);
	switch (type)
	{
	case DRIVE_UNKNOWN:   //0
		cout << " DRIVE_UNKNOWN ";
		break;
	case DRIVE_NO_ROOT_DIR:   //1
		cout << " DRIVE_NO_ROOT_DIR ";
		break;
	case DRIVE_REMOVABLE:   //2
		cout << " DRIVE_REMOVABLE ";
		break;
	case DRIVE_FIXED:   //3
		cout << " DRIVE_FIXED ";
		break;
	case DRIVE_REMOTE:   //4
		cout << " DRIVE_REMOTE ";
		break;
	case DRIVE_CDROM:   //5
		cout << " DRIVE_CDROM ";
		break;
	case DRIVE_RAMDISK:   //6
		cout << " DRIVE_RAMDISK ";
		break;
	default: " Unknown value!";
	}
}

void DiskFreeSpace(char *temp)
{
	DWORD  spc, bps, nofc, tnoc;   //переменные SectorsPerCluster, BytesPerSector, NumberOfFreeClusters и TotalNumberOfClusters в сокращенном виде
	GetDiskFreeSpace(temp, &spc, &bps, &nofc, &tnoc);
	cout << "(Free space: " << (UINT64)spc * (UINT64)bps * (UINT64)nofc << ")" << endl;
}