#include <Windows.h>
#include <iostream>  
using namespace std;
int main() {
	DWORD memAddrs[] = {
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
	DWORD width=320; //change to the width you want Mania to render to (default is 424)
	DWORD processID;
	HWND window;
	if (!WinExec("SonicMania.exe",SW_SHOWNORMAL)) {
		cerr << "couldn't find the Sonic Mania executable, make sure this program is in your Sonic Mania folder" << GetLastError << endl;
		return 1;
	}
	else 
		cout << "waiting for Sonic Mania to start";
	while (!GetWindowThreadProcessId(window,&processID)) {
		cout << ".";
		window=FindWindow(NULL,"Sonic Mania");
		Sleep(250);
	}
	cout << endl;
	HANDLE handle=OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	if (!handle) {
		cerr << "couldn't open the Sonic Mania process, " << GetLastError() << endl;
		return 2;
	}
	else 
		cout << "Sonic Mania process found, id "<< processID << endl;
	//cout << "waiting for Sonic Mania to initialize..." <<endl;
	//Sleep(5000);
	cout << "patching..." << endl;
	int i;
	for (i=0; i < 9; i++) {
		cout << "patching memory value " <<(i+1) << " of 9..." << endl;
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
	cout << "done patching" << endl;

	cout << "making Mania fullscreen..." << endl;
	keybd_event(VK_MENU, 0, 0, 0);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,0);
	keybd_event(VK_MENU,0,KEYEVENTF_KEYUP,0);
	
	CloseHandle(window);
	CloseHandle(handle);
	//Sleep(20000);
	return 0;
}
