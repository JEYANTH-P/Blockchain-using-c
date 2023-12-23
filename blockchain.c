
#include<stdio.h>
#include<stdlib.h>

char * encrypt(char* str, int size, int key);
char * decrypt(char* str, int size, int key);

char * encrypt(char* str, int size, int key) {
    int i;
    char *temp = (char *)malloc(sizeof(char) * (size + 1));

    for (i = 0; i < size; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            temp[i] = (char)(((str[i] - 'a' + key) % 26 + 26) % 26 + 'a');
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
            temp[i] = (char)(((str[i] - 'A' + key) % 26 + 26) % 26 + 'A');
        } else {
            temp[i] = str[i];
        }
    }

    temp[i] = '\0';
    return temp;
}

char * decrypt(char* str, int size, int key) {
    int i;
    char *temp2 = (char *)malloc(sizeof(char) * (size + 1));

    for (i = 0; i < size; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            temp2[i] = (char)(((str[i] - 'a' - key + 26) % 26 + 26) % 26 + 'a');
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
            temp2[i] = (char)(((str[i] - 'A' - key + 26) % 26 + 26) % 26 + 'A');
        } else {
            temp2[i] = str[i];
        }
    }

    temp2[i] = '\0';
    return temp2;
}

int main() {
    char str[] = "aBcz";
    char *ptr = encrypt(str, 4, -6000000);
    printf("Encrypted: %s\n", ptr);

    char *ptr1 = decrypt(ptr, 4, -6000000);
    printf("Decrypted: %s\n", ptr1);

    free(ptr);
    free(ptr1);

    return 0;
}
