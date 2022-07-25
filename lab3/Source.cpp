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
	setlocale(LC_ALL, "Russian");   //��� ����������� ������ �����, �������������� ������������� (3-� �������)

	//1 �������: ���������� ��� ���������� (GetComputerName)
	ComputerName();

	//2 �������: ���������� ��������� ����� � ������� ���� (GetLocalTime, GetTimeZoneInformation)
	TimeInformation();

	//3 �������: ���������� ��� � ���� �������� ������������ (GetUserDefaultLangID, VerLanguageName, GetUserName)
	UserInformation();

	//4 �������: ���������� ������� ����������, ���������� ��, ��������� ���������� (GetCurrentDirectory, GetWindowsDirectory, GetSystemDirectory)
	Directories();

	//5 �������: ���������� ������ ���������� ������, �� ���� � ��������� ����� �� ����� � ������ (GetLogicalDrivers, GetLogicalDrivesString, GetDriveType, GetDiskFreeSpace)
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
	cout << "Local time: " << st.wHour << ":" << st.wMinute << endl;    //������� ������� ����� (���� � ������)

	TIME_ZONE_INFORMATION tzi;
	GetTimeZoneInformation(&tzi);
	cout << "Timezone: " << tzi.Bias / 60 << endl;   //������� �������� (�������) � ������� ����� UTC � ������� �������� (������� �� 60 ��� ����������� �����)
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
			cout << char(65 + i) << " ";   //������� ������ � ������������ � �������� ASCII
		mask <<= 1;
	}

	char buffer[256];
	GetLogicalDriveStrings(256, buffer);
	cout << "\nAvailable disk drives (2nd way): \n";

	char *temp = buffer;
	while (*temp)
	{
		cout << temp;
		DriveType(temp);   //���������� ��� �����
		DiskFreeSpace(temp);   //���������� ��������� ����� �� ����� � ������
		temp = &temp[strlen(temp) + 1];   //��������� ���������
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
	DWORD  spc, bps, nofc, tnoc;   //���������� SectorsPerCluster, BytesPerSector, NumberOfFreeClusters � TotalNumberOfClusters � ����������� ����
	GetDiskFreeSpace(temp, &spc, &bps, &nofc, &tnoc);
	cout << "(Free space: " << (UINT64)spc * (UINT64)bps * (UINT64)nofc << ")" << endl;
}