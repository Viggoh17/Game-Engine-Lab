#include <Windows.h>
#include <iostream>
using namespace std;

bool IsOnlyInstance(LPCTSTR gameTitle);
void CheckMemory();
DWORD ReadCPUSpeed();
DWORD ReadCPUArchitecture();

int main() 
{
	_int64 lpFreeBytesAvailable = 0;
	_int64 lpTotalNumberOfBytes = 0;
	_int64 lpTotalNumberOfFreeBytes = 0;

	IsOnlyInstance("GameEngineLab");
	int Space = GetDiskFreeSpaceEx(NULL, (PULARGE_INTEGER)&lpFreeBytesAvailable, (PULARGE_INTEGER)&lpTotalNumberOfBytes, (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);
	cout << "Space Available: " << lpFreeBytesAvailable / 1024 << " MB" << endl;

	if (lpFreeBytesAvailable / 1024 >= 300) 
	{
		cout << "There is enough free space." << endl;
	}
	else 
	{
		cout << "There is not enough space." << endl;
	}
	CheckMemory();
	ReadCPUSpeed();
	ReadCPUArchitecture();
	system("pause");
}

bool IsOnlyInstance(LPCTSTR gameTitle) {
	HANDLE handle = CreateMutex(NULL, TRUE, gameTitle);
	if (GetLastError() != ERROR_SUCCESS) 
	{
		HWND hWnd = FindWindow(gameTitle, NULL);
		cout << "Another instance is open." << endl;
		return false;
	}
	return true;
}
void CheckMemory() {
	MEMORYSTATUSEX status;
	GlobalMemoryStatusEx(&status);
	cout << "Current physical RAM: " << status.ullTotalPhys << endl;
	cout << "Current virtual memory " << status.ullAvailVirtual << endl;
}

DWORD ReadCPUSpeed() {
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz = 0;
	DWORD type = REG_DWORD;
	HKEY hKey;
	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
	if (lError == ERROR_SUCCESS) 
	{
		// query the key:
		RegQueryValueEx(hKey, "~MHz", NULL, &type, (LPBYTE)&dwMHz, &BufSize);
		cout << "CPU Speed: " << dwMHz << endl;
	}
	return dwMHz;
}

DWORD ReadCPUArchitecture() {
	
	DWORD BufSize = 260;
	char dwARc[260];
	DWORD type = REG_DWORD;
	HKEY hKey;
	
	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
	DWORD error = 0;
	if (lError == ERROR_SUCCESS) 
	{
		// query the key:
		error = RegQueryValueEx(hKey, "ProcessorNameString", NULL, &type, (LPBYTE)dwARc, &BufSize);
		while (error == ERROR_MORE_DATA) 
		{
			BufSize++;
			error = RegQueryValueEx(hKey, "ProcessorNameString", NULL, &type, (LPBYTE)dwARc, &BufSize);
		}
		cout << dwARc << endl;
	}
	return 0;
}

