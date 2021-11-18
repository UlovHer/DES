#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "des.h"

int main()
{
	char plaintext[9] = { 0 };
	char key[9] = { 0 };
	char resultStr[9];
	int ciphertext_bit[64] = { 0 };
	char ciphertext[9];
	std::cout << "please input plaintext(8bytes)" << std::endl;
	scanf("%s", plaintext);
	std::cout << "please input key(8bytes)" << std::endl;
	scanf("%s", key);

	std::cout << "*****************Encription*****************" << std::endl;
	DES_Enc(plaintext, key, ciphertext_bit);
	BitToChar(ciphertext_bit, ciphertext, 8);
	std::cout << "ciphertext bit is: " << ciphertext_bit << std::endl;
	std::cout << "ciphertext is: " << ciphertext << std::endl;

	std::cout << "*****************Decryption*****************" << std::endl;
	DES_Dec(ciphertext_bit, key, plaintext);
	std::cout << "plaintext is: " << plaintext << std::endl;

	system("pause");

	return 0;
}