#include <stdio.h>
#include <string.h>
void print_hex(char *input, size_t len) {
	printf("Hex: ");
	for(int i = 0; i < len; i++){
		printf("%02x ", (unsigned char)input[i]);
	}
	printf("\n");
}
void decrypt(char *output, char *key, size_t message_len, size_t key_len){
        for(int i = 0; i < message_len; i++){
                output[i] = output[i] ^ key[i % key_len];
        }
        printf("Decrypted: %s\n", output);
}

void xor(char *input, char *key) {
	size_t message_len = strlen(input);
	size_t key_len = strlen(key);
	for(int i = 0; i < message_len; i++){
		input[i]= input[i] ^ key[i % key_len];
	}
	print_hex(input, message_len);
	decrypt(input, key, message_len, key_len);
}
int main(void) {
	char input[1024];
	char key[1024];
	printf("Enter string to xor: ");
	fgets(input, sizeof(input), stdin);
	input[strcspn(input, "\n")] = '\0';
	printf("Enter key: ");
	fgets(key, sizeof(key), stdin);
	key[strcspn(key, "\n")] = '\0';
	printf("Original string: %s\n", input);
	xor(input, key);
	
	return 0;
}
