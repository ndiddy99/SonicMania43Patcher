#include <Windows.h>
#include <iostream>  
using namespace std;
typedef unsigned int u32;
int main() {
	//u32[] memAddrs={438768,506158,5060D8,4FFA58,4CE7B4,4CE798,36E6E0,36DEE0,239C8C};
	u32 memAddrs[] = {
    0x00838768,
    0x00906158,
    0x009060D8,
    0x008FFA58,
    0x008CE7B4,
    0x008CE798,
    0x0076E6E0,
    0x0076DEE0,
    0x00639C8C,
};
	DWORD width=320; //change to the width you want Mania to render to
	DWORD processID;
	HWND Window = NULL;
	Window=FindWindow(NULL,"Sonic Mania");
	if (!GetWindowThreadProcessId(Window,&processID)) {
		cerr << "couldn't find the Sonic Mania process, " << GetLastError() << endl;
		return 1;
	}
	HANDLE handle=OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	if (!handle) {
		cerr << "couldn't open the Sonic Mania process, " << GetLastError() << endl;
		return 2;
	}
	else 
		cout << "Sonic Mania process found, id "<< processID << endl;
	cout << "patching..." << endl;
	int i;
	for (i=0; i < 9; i++) {
		cout << "patching memory value " <<(i+1) << "of 9..." << endl;
		int memory;
		ReadProcessMemory(handle,(LPVOID)memAddrs[i], &memory,sizeof(DWORD) ,NULL);
		//if (!ReadProcessMemory(handle,(LPVOID)memaddrs[i], &memory,sizeof(DWORD) ,NULL))
		//	cerr << "error accessing memory, " << GetLastError() << endl;
		cout << "old value: "<< memory;
		WriteProcessMemory(handle,(LPVOID)memAddrs[i],&width,sizeof(DWORD),NULL);
		if (!WriteProcessMemory(handle,(LPVOID)memAddrs[i],&width,sizeof(DWORD),NULL))
			cerr << "error writing memory, " << GetLastError() << endl;
		ReadProcessMemory(handle,(LPVOID)memAddrs[i], &memory,sizeof(DWORD) ,NULL);
		cout << ", new value: " << memory << endl;
		memory=0;
	}
	cout << "done patching, exiting..." << endl;
	
	CloseHandle(Window);
	CloseHandle(handle);
	return 0;
}
