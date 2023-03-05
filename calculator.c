#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <gmp.h>

bool is_float(char str[])
{
    size_t len = strlen(str);
    bool decimal_point = false;

    for (int i = 0; i < len; i++)
    {
        if (str[i] == '-')
        {
            if (i != 0)
            {
                // - can only in the first position
                return false;
            }
        }
        else if (str[i] == '.')
        {
            if (decimal_point)
            {
                // decimal point can only appear once
                return false;
            }
            decimal_point = true;
        }
        else if (!isdigit(str[i]))
        {
            // every char must be digit
            return false;
        }
    }

    // one digit at least
    if (!isdigit(str[len - 1]) && !isdigit(str[0]))
    {
        return false;
    }

    if (decimal_point)
    {
        return true;
    }
    return false;
}

bool is_number(char str[])
{
    size_t len = strlen(str);
    bool decimal_point = false;

    for (int i = 0; i < len; i++)
    {
        if (str[i] == '-')
        {
            if (i != 0)
            {
                // - can only in the first position
                return false;
            }
        }
        else if (str[i] == '.')
        {
            if (decimal_point)
            {
                // decimal point can only appear once
                return false;
            }
            decimal_point = true;
        }
        else if (!isdigit(str[i]))
        {
            // every char must be digit
            return false;
        }
    }

    // one digit at least
    if (!isdigit(str[len - 1]) && !isdigit(str[0]))
    {
        return false;
    }

    return true;
}

void add(char num1[], char num2[], char answer[])
{
    size_t len1 = strlen(num1);
    size_t len2 = strlen(num2);

    size_t max_len;
    if (len1 > len2)
    {
        max_len = len1;
    }
    else
    {
        max_len = len2;
    }

    int carry = 0;
    for (size_t i = 0; i < max_len; i++)
    {
        int digit1;
        if (i < len1)
        {
            digit1 = num1[len1 - i - 1] - '0';
        }
        else
        {
            digit1 = 0;
        }
        int digit2;
        if (i < len2)
        {
            digit2 = num2[len2 - i - 1] - '0';
        }
        else
        {
            digit2 = 0;
        }

        int sum = digit1 + digit2 + carry;
        carry = sum / 10;

        answer[max_len - i - 1] = (char)(sum % 10 + '0');
    }

    if (carry != 0)
    {
        // shift right
        for (size_t i = max_len; i > 0; i--)
        {
            answer[i] = answer[i - 1];
        }
        answer[0] = (char)(carry + '0');
        // null terminator
        answer[max_len + 1] = '\0';
    }
    else
    {
        // null terminator
        answer[max_len] = '\0';
    }
}

void multiply(char num1[], char num2[], char answer[])
{
    size_t len1 = strlen(num1);
    size_t len2 = strlen(num2);
    size_t max_len = len1 + len2;

    int digits[max_len];
    for (size_t i = 0; i < max_len; i++)
    {
        digits[i] = 0;
    }

    for (size_t i = 0; i < len1; i++)
    {
        int carry = 0;
        int digit1 = num1[len1 - i - 1] - '0';

        for (size_t j = 0; j < len2; j++)
        {
            int digit2 = num2[len2 - j - 1] - '0';
            int product = digit1 * digit2 + carry + digits[i + j];

            carry = product / 10;
            digits[i + j] = product % 10;
        }

        if (carry > 0)
        {
            digits[i + len2] += carry;
        }
    }

    // remove leading zeroes
    size_t i = max_len - 1;
    while (i > 0 && digits[i] == 0)
    {
        i--;
    }

    // copy digits to answer in reverse order
    for (size_t j = 0; j <= i; j++)
    {
        answer[j] = (char)(digits[i - j] + '0');
    }

    answer[i + 1] = '\0';
}

void subtract(char num1_str[], char num2_str[], char answer[])
{
    // init
    mpz_t num1, num2, result;
    mpz_init_set_str(num1, num1_str, 10);
    mpz_init_set_str(num2, num2_str, 10);
    mpz_init(result);

    // sub & print
    mpz_sub(result, num1, num2);
    gmp_sprintf(answer, "%Zd", result);

    // clear
    mpz_clear(num1);
    mpz_clear(num2);
    mpz_clear(result);
}

void add_float(float num1, float num2, char *result)
{
    sprintf(result, "%.2f", num1 + num2);
}

void subtract_float(float num1, float num2, char *result)
{
    sprintf(result, "%.2f", num1 - num2);
}

void multiply_float(float num1, float num2, char *result)
{
    sprintf(result, "%.2f", num1 * num2);
}

void divide_float(float num1, float num2, char *quotient, char *remainder)
{
    if (num2 == 0)
    {
        printf("A number cannot be divided by zero.\n");
        return;
    }
    sprintf(quotient, "%.2f", num1 / num2);
    sprintf(remainder, "%.2f", fmod(num1, num2));
}

int main()
{
    while (true)
    {
        char input[200];
        printf("Enter an expression (q to quit): ");
        fgets(input, sizeof(input), stdin);

        if (strcmp(input, "q\n") == 0)
        { // If the user enters 'q', exit the loop
            printf("Quiting...\n");
            break;
        }

        char num1_str[100], num2_str[100], op;
        int i;

        // get 1st num
        for (i = 0; input[i] != ' '; i++)
        {
            num1_str[i] = input[i];
        }
        num1_str[i] = '\0';
        i++; // skip space

        // get operator
        op = input[i];
        i++; // skip operator
        i++; // skip space

        // get 2nd num
        int j;
        for (j = 0; input[i] != '\n'; i++, j++)
        {
            num2_str[j] = input[i];
        }
        num2_str[j] = '\0';

        // check format
        if (!is_number(num1_str) || !is_number(num2_str))
        {
            printf("Wrong number format.\n");
            continue;
        }

        if (is_float(num1_str) || is_float(num2_str))
        {
            float num1, num2;
            char op;

            // parse input
            if (sscanf(input, "%f %c %f", &num1, &op, &num2) != 3)
            {
                printf("Invalid input format.\n");
                continue;
            }

            if (op == '+')
            {
                char answer[101];
                add_float(num1, num2, answer);
                printf("%.2f + %.2f = %s\n", num1, num2, answer);
            }
            else if (op == '-')
            {
                char answer[101];
                subtract_float(num1, num2, answer);
                printf("%.2f - %.2f = %s\n", num1, num2, answer);
            }
            else if (op == '*')
            {
                char answer[202];
                multiply_float(num1, num2, answer);
                printf("%.2f * %.2f = %s\n", num1, num2, answer);
            }
            else if (op == '/')
            {
                char quotient_str[101], remainder_str[101];
                divide_float(num1, num2, quotient_str, remainder_str);
                printf("%.2f / %.2f = %s with remainder %s\n", num1, num2, quotient_str, remainder_str);
            }
            else
            {
                printf("Invalid operator '%c'\n", op);
                continue;
            }
            continue;
        }

        if (op == '+')
        {
            char answer[101];
            add(num1_str, num2_str, answer);
            printf("%s + %s = %s\n", num1_str, num2_str, answer);
        }
        else if (op == '-')
        {
            char answer[101];
            subtract(num1_str, num2_str, answer);
            printf("%s - %s = %s\n", num1_str, num2_str, answer);
        }
        else if (op == '*')
        {
            char answer[202];
            multiply(num1_str, num2_str, answer);
            printf("%s * %s = %s\n", num1_str, num2_str, answer);
        }
        else if (op == '/')
        {
            mpz_t num1, num2, quotient, remainder;
            mpz_inits(num1, num2, quotient, remainder, NULL);

            mpz_set_str(num1, num1_str, 10);
            mpz_set_str(num2, num2_str, 10);

            if (mpz_cmp_si(num2, 0) == 0)
            {
                printf("A number cannot be divided by zero.\n");
                continue;
            }

            mpz_tdiv_qr(quotient, remainder, num1, num2);

            char quotient_str[101], remainder_str[101];
            mpz_get_str(quotient_str, 10, quotient);
            mpz_get_str(remainder_str, 10, remainder);
            printf("%s / %s = %s with remainder %s\n", num1_str, num2_str, quotient_str, remainder_str);
            mpz_clears(num1, num2, quotient, remainder, NULL);
        }
        else
        {
            printf("Invalid operator.'%c'\n", op);
            continue;
        }
    }
    return 0;
}
