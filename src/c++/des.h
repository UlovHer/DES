#pragma once
#ifndef _DES_H_
void CharToBit(const char input[], int output[], int bits);
void BitToChar(const int intput[], char output[], int bits);
void Xor(int *INA, int *INB, int len);
void IP_Init_Rep(const int input[64], int output[64], const int table[64]);
void E_Extend(const int input[32], int output[48], const int table[48]);
void P_Rep(const int input[32], int output[32], const int table[32]);
void IP_Inv_Rep(const int input[64], int output[64], const int table[64]);
void PC_1(const int input[64], int output[56], const int table[56]);
void PC_2(const int input[56], int output[48], const int table[48]);
void S_Comp(const int input[48], int output[32], const int table[8][4][16]);
void DES_Kernel(const int input[32], int output[32], int subKey[48]);
void RotateL(const int input[28], int output[28], int leftCount);
void subKey_fun(const int input[64], int subKey[16][48]);
void DES_Enc(const char input[8], char inKey[8], int output[64]);
void DES_Dec(const int input[64], char inKey[8], char output[8]);
#define _DES_H_
#endif // !_DES_H_
