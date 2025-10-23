#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long long cardNumber = get_long("Number: ");
    int totalSum = 0, cardLength = 0;
    long long firstTwoDigits = 0;

    for (long long tempNumber = cardNumber; tempNumber > 0; tempNumber /= 10, cardLength++)
    {
        int digit = tempNumber % 10;
        
        if (cardLength % 2 != 0)
        {
            int doubledDigit = digit * 2;
            totalSum += (doubledDigit % 10) + (doubledDigit / 10);
        }
        else
        {
            totalSum += digit;
        }

        if (tempNumber < 100 && tempNumber >= 10)
        {
            firstTwoDigits = tempNumber;
        }
    }

    if (totalSum % 10 != 0)
    {
        printf("INVALID\n");
    }
    else if (cardLength == 15 && (firstTwoDigits == 34 || firstTwoDigits == 37))
    {
        printf("AMEX\n");
    }
    else if (cardLength == 16 && (firstTwoDigits >= 51 && firstTwoDigits <= 55))
    {
        printf("MASTERCARD\n");
    }
    else if ((cardLength == 13 || cardLength == 16) && (firstTwoDigits / 10 == 4))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
    
    return 0;
}
