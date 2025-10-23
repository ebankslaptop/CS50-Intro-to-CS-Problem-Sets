#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_valid_key(const char *key);
char encrypt_char(char plaintext_char, const char *key);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    const char *key = argv[1];

    if (!is_valid_key(key))
    {
        printf("Key must contain 26 unique alphabetic characters.\n");
        return 1;
    }

    char plaintext[1024];
    printf("plaintext: ");
    if (fgets(plaintext, sizeof(plaintext), stdin) == NULL)
    {
        return 1;
    }

    size_t len = strlen(plaintext);
    if (len > 0 && plaintext[len - 1] == '\n') 
    {
        plaintext[len - 1] = '\0';
    }

    printf("ciphertext: ");
    for (int i = 0; plaintext[i] != '\0'; i++)
    {
        char encrypted = encrypt_char(plaintext[i], key);
        printf("%c", encrypted);
    }
    printf("\n");

    return 0;
}

int is_valid_key(const char *key)
{
    if (strlen(key) != 26)
    {
        return 0;
    }

    int seen[26] = {0};

    for (int i = 0; i < 26; i++)
    {
        char c = key[i];

        if (!isalpha(c))
        {
            return 0;
        }

        int index = toupper(c) - 'A';

        if (seen[index] == 1)
        {
            return 0;
        }
        seen[index] = 1;
    }

    return 1;
}

char encrypt_char(char plaintext_char, const char *key)
{
    if (!isalpha(plaintext_char))
    {
        return plaintext_char;
    }

    int index;
    if (isupper(plaintext_char))
    {
        index = plaintext_char - 'A';
        return toupper(key[index]);
    }
    else
    {
        index = plaintext_char - 'a';
        return tolower(key[index]);
    }
}
