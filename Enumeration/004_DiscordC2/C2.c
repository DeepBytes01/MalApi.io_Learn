#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <string.h>
#pragma comment(lib, "wininet.lib")

// Function to send a message to your Discord Webhook
void send_to_discord(const char* message) {
    // Starting point -- w/o this, servers will identify you as a bot
	/*
	HINTERNET InternetOpenA(
  		[in] LPCSTR lpszAgent, ==Mozilla/5.0
  		[in] DWORD  dwAccessType, ==INTERNET_OPEN_TYPE_DIRECT == tells windows to connect directly to the web, ignoring local proxy
  		[in] LPCSTR lpszProxy,   ==  NULL
 		 [in] LPCSTR lpszProxyBypass,  == NULL
 		 [in] DWORD  dwFlags == 0
	);
	*/
    HINTERNET hSession = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hSession) {
	// This creates a session SPECIFICALLY for one website
	/*
	HINTERNET InternetConnectA(
	 	 [in] HINTERNET     hInternet, , ==  hSession  == takes the InternetOpenA we created
 		 [in] LPCSTR        lpszServerName, == discord.com == domain only
 		 [in] INTERNET_PORT nServerPort, == INTERNET_DEFAULT_HTTPS_PORT == port 443. port 80 will refuse.
 		 [in] LPCSTR        lpszUserName, == NULL
 		 [in] LPCSTR        lpszPassword, == NULL
 		 [in] DWORD         dwService, == INTERNET_SERVICE_HTTP = even though we use https, the service type is still http.
 		 [in] DWORD         dwFlags, == 0
 		 [in] DWORD_PTR     dwContext == 0
	);
	*/
        HINTERNET hConnect = InternetConnectA(hSession, "discord.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect) {

            // Your Webhook path (the part after discord.com)
            const char* path = "/api/webhooks/";
	    //  request communication.
		/*
		HINTERNET HttpOpenRequestA( 
 			 [in] HINTERNET hConnect, == hConnect == takes  our InternetConnectA we created
 			 [in] LPCSTR    lpszVerb, == POST == to give something
 			 [in] LPCSTR    lpszObjectName, == path == the path variable, could be directly hardcoded BAD PRACTICE
 			 [in] LPCSTR    lpszVersion, == NULL
 			 [in] LPCSTR    lpszReferrer, == NULL
 			 [in] LPCSTR    *lplpszAcceptTypes, NULL
 			 [in] DWORD     dwFlags, INTERNET_FLAG_SECURE ==  It tells Windows to encrypt the data with SSL/TLS.
 			 [in] DWORD_PTR dwContext == 0
		); 
		*/
            HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", path, NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
            
            if (hRequest) {
                // Formatting the message as JSON for Discord
                char json_data[2048];
                snprintf(json_data, sizeof(json_data), "{\"content\": \"%s\"}", message);
                
		// tells were sending a json format
                const char* headers = "Content-Type: application/json";
		/*
		BOOL HttpSendRequestA(
 			 [in] HINTERNET hRequest, == hRequest == takes our hRequest we made
 			 [in] LPCSTR    lpszHeaders, == headers == takes the content type format json
 			 [in] DWORD     dwHeadersLength, == sizeof(headers)
 			 [in] LPVOID    lpOptional, == json_data == the actual data to be sent
 			 [in] DWORD     dwOptionalLength == sizeof(json_data)
		);
		*/
                HttpSendRequestA(hRequest, headers, strlen(headers), json_data, strlen(json_data));
                InternetCloseHandle(hRequest);
            }
            InternetCloseHandle(hConnect);
        }
        InternetCloseHandle(hSession);
    }
}
int main(void) {
	char message[1900];
	printf("Enter your message: ");
	fgets(message, sizeof(message), stdin);
	message[strcspn(message, "\n")] = '\0';
	send_to_discord(message);
	return 0;
}
