#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
void send_to_discord(const char* message) {
    unsigned char obfuscated_path[] = { 
        0x7c, 0x52, 0x33, 0x1b, 0x4a, 0x03, 0x5a, 0x31, 0x5b, 0x2c, 0x1d, 0x0e, 0x07, 0x10, 
        0x62, 0x07, 0x7b, 0x40, 0x52, 0x44, 0x0f, 0x61, 0x0a, 0x72, 0x4b, 0x51, 0x4d, 0x0c, 
        0x60, 0x03, 0x75, 0x45, 0x53, 0x5b, 0x6f, 0x26, 0x5e, 0x34, 0x19, 0x28, 0x23, 0x4c, 
        0x7e, 0x06, 0x2e, 0x13, 0x2b, 0x04, 0x4f, 0x16, 0x07, 0x24, 0x30, 0x0f, 0x41, 0x7a, 
        0x64, 0x63, 0x27, 0x22, 0x15, 0x1d, 0x5b, 0x24, 0x5c, 0x05, 0x5f, 0x32, 0x0e, 0x06, 
        0x34, 0x67, 0x1b, 0x00, 0x48, 0x4c, 0x45, 0x07, 0x7a, 0x35, 0x2b, 0x26, 0x1e, 0x78, 
        0x63, 0x59, 0x0a, 0x41, 0x36, 0x21, 0x0a, 0x27, 0x66, 0x36, 0x35, 0x56, 0x2d, 0x4e, 
        0x1c, 0x56, 0x29, 0x13, 0x00 
    };
    unsigned char obfuscated_1stKey[] = {0x1a, 0x40, 0x17, 0x1a, 0x0c, 0x07, 0x6b, 0x00};
    unsigned char obfuscated_domain[] = {0x34, 0x1b, 0x1c, 0x01, 0x0e, 0x10, 0x08, 0x57, 0x37, 0x07, 0x08, 0x00};
    char key1[] = "IsThisTheKey?";
    char key2[] = "S3Cret?"; // NOTE: Ensure this matches your XOR tool's case exactly!
    char key3[] = "ProbablyThePassword";
    size_t path_len = sizeof(obfuscated_path) - 1;
    size_t key1_len = sizeof(key1) - 1;
    size_t key2_len = sizeof(key2) - 1;
    size_t key3_len = sizeof(key3) - 1;
    size_t domain_len = sizeof(obfuscated_domain) - 1;
    for(size_t i = 0; i < domain_len; i++) {
	obfuscated_domain[i] ^= key3[i % key3_len];	
    }
    // 1. Decrypt Path
    for (size_t i = 0; i < path_len; i++) {
        obfuscated_path[i] ^= key2[i % key2_len];
    }
    printf("[DEBUG] Decrypted Path: %s\n", obfuscated_path);

    // 2. Prepare JSON data
    char json_data[2048];
    snprintf(json_data, sizeof(json_data), "{\"content\": \"%s\"}", message);
    const char* headers = "Content-Type: application/json";

    // 3. Network operations
    HINTERNET hSession = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hSession) {
        HINTERNET hConnect = InternetConnectA(hSession, obfuscated_domain, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect) {
            HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", (char*)obfuscated_path, NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
            if (hRequest) {
                if (!HttpSendRequestA(hRequest, headers, (DWORD)strlen(headers), json_data, (DWORD)strlen(json_data))) {
                    printf("HttpSendRequestA failed: %lu\n", GetLastError());
                } else {
                    printf("Successfully sent to Discord!\n");
                }
                InternetCloseHandle(hRequest);
            } else {
                printf("HttpOpenRequestA failed: %lu\n", GetLastError());
            }
            InternetCloseHandle(hConnect);
        } else {
            printf("InternetConnectA failed: %lu\n", GetLastError());
        }
        InternetCloseHandle(hSession);
    } else {
        printf("InternetOpenA failed: %lu\n", GetLastError());
    }

    // 4. Cleanup
    memset(obfuscated_path, 0, sizeof(obfuscated_path));
}

int main(void) {
	send_to_discord("Wake the fuck up!");
	return 0;
}
