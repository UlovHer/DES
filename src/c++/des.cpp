#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "tables.h"
#include "des.h"

/*
 *  类型转换函数 I
 *  将 char 型转化为二进制形式
 *  8 * sizeof(char) = 8(位) 8 个字符 64 位
 */
void CharToBit(const char input[], int output[], int bits)
{
	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			output[7 * (j + 1) - i + j] = (input[j] >> i) & 1;
		}
	}
}

/*
 *  类型转换函数 II
 *  将二进制形式转化为 char 型
 */
void BitToChar(const int intput[], char output[], int bits)
{
	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			output[j] = output[j] * 2 + intput[i + 8 * j];
		}
	}
}

/*
 *  异或函数
 *  将数组 INA 和 INB 进行异或操作，并且保存在 INA 中
 */
void Xor(int *INA, int *INB, int len)
{
	for (int i = 0; i < len; i++)
	{
		*(INA + i) = *(INA + i) ^ *(INB + i);
	}
}

/*
 *  IP 初始置换函数
 *  IP 根据 IP 初始置换表进行初始置换
 */
void IP_Init_Rep(const int input[64], int output[64], const int table[64])
{
	for (int i = 0; i < 64; i++)
	{
		output[i] = input[table[i] - 1];
	}
}

/*
 *  E 扩展置换函数
 *  根据 E 扩展表进行扩展
 */
void E_Extend(const int input[32], int output[48], const int table[48])
{
	for (int i = 0; i < 48; i++)
	{
		output[i] = input[table[i] - 1];
	}
}

/*
 *  P 置换函数
 *  根据 P 盒进行置换
 */
void P_Rep(const int input[32], int output[32], const int table[32])
{
	for (int i = 0; i < 32; i++)
	{
		output[i] = input[table[i] - 1];
	}
}

/*
 *  IP 逆置换函数
 *  IP 根据 IP 逆置换表进行置换
 */
void IP_Inv_Rep(const int input[64], int output[64], const int table[64])
{
	for (int i = 0; i < 64; i++)
	{
		output[i] = input[table[i] - 1];
	}
}

/*
 *  密匙第一次置换函数
 *  根据密匙第一次置换表进行置换
 */
void PC_1(const int input[64], int output[56], const int table[56])
{
	for (int i = 0; i < 56; i++)
	{
		output[i] = input[table[i] - 1];
	}
}

/*
 *  密匙第二次置换函数
 *  根据密匙第二次置换表进行置换
 */
void PC_2(const int input[56], int output[48], const int table[48])
{
	for (int i = 0; i < 48; i++)
	{
		output[i] = input[table[i] - 1];
	}
}

/*
 *  S 盒压缩函数
 *  根据 8 个 S 盒进行压缩
 */
void S_Comp(const int input[48], int output[32], const int table[8][4][16])
{
	int INT[8];
	for (int i = 0, j = 0; i < 48; i = i + 6)
	{
		INT[j] = table[j][(input[i] << 1)
			+ (input[i + 5])][(input[i + 1] << 3)
			+ (input[i + 2] << 2)
			+ (input[i + 3] << 1)
			+ (input[i + 4])];
		j++;
	}
	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			output[3 * (j + 1) - i + j] = (INT[j] >> i) & 1;
		}
	}
}

/*
 *  轮迭代函数
 *  DES 核心迭代部分
 */
void DES_Kernel(const int input[32], int output[32], int subKey[48])
{
	int len = 48;
	int temp0[48] = { 0 };
	int temp1[32] = { 0 };
	E_Extend(input, temp0, E_Table);
	Xor(temp0, subKey, len);
	S_Comp(temp0, temp1, S_Box);
	P_Rep(temp1, output, P_Table);
}

/*
 *  密匙循环左移函数
 *  密匙在不同轮数都要进行不同的左移操作
 */
void RotateL(const int input[28], int output[28], int leftCount)
{
	int len = 28;
	for (int i = 0; i < len; i++)
	{
		output[i] = input[(i + leftCount) % len];
	}
}

/*
 *  子密匙生成函数
 *  生成 subKey，在第 1、2、9、16 轮循环左移 1 位，其他轮循环左移 2 位
 */
void subKey_fun(const int input[64], int subKey[16][48])
{
	int loop0 = 1, loop1 = 2;
	int c[28], d[28];
	int pc_1[56] = { 0 };
	int pc_2[16][56] = { 0 };
	int rotatel_c[16][28] = { 0 };
	int rotatel_d[16][28] = { 0 };

	PC_1(input, pc_1, PC1_Table);
	for (int i = 0; i < 28; i++)
	{
		c[i] = pc_1[i];
		d[i] = pc_1[i + 28];
	}

	int leftCount = 0;
	for (int i = 1; i < 17; i++)
	{
		if (i == 1 || i == 2 || i == 9 || i == 16)
		{
			leftCount += loop0;
			RotateL(c, rotatel_c[i - 1], leftCount);
			RotateL(d, rotatel_d[i - 1], leftCount);
		}
		else
		{
			leftCount += loop1;
			RotateL(c, rotatel_c[i - 1], leftCount);
			RotateL(d, rotatel_d[i - 1], leftCount);
		}
	}

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 28; j++)
		{
			pc_2[i][j] = rotatel_c[i][j];
			pc_2[i][j + 28] = rotatel_d[i][j];
		}
	}

	for (int i = 0; i < 16; i++)
	{
		PC_2(pc_2[i], subKey[i], PC2_Table);
	}
}

/*
 *  DES 加密函数
 *  传入明文 input 和密匙 inKey，获取 64 位二进制密文 output
 */
void DES_Enc(const char input[8], char inKey[8], int output[64])
{
	int ip[64] = { 0 };
	int output_1[64] = { 0 };
	int subKeys[16][48];
	int chartobit[64] = { 0 };
	int key[64];
	int l[17][32], r[17][32];

	CharToBit(input, chartobit, 8);
	IP_Init_Rep(chartobit, ip, IP_Init_Table);
	CharToBit(inKey, key, 8);
	subKey_fun(key, subKeys);

	for (int i = 0; i < 32; i++)
	{
		l[0][i] = ip[i];
		r[0][i] = ip[32 + i];
	}
	for (int j = 1; j < 16; j++)    //  15 轮迭代
	{
		for (int k = 0; k < 32; k++)
		{
			l[j][k] = r[j - 1][k];
		}
		DES_Kernel(r[j - 1], r[j], subKeys[j - 1]);
		Xor(r[j], l[j - 1], 32);
	}

	int t = 0;
	for (t = 0; t < 32; t++)        //  第 16 轮迭代
	{
		r[16][t] = r[15][t];
	}
	DES_Kernel(r[15], l[16], subKeys[15]);
	Xor(l[16], l[15], 32);

	for (t = 0; t < 32; t++)
	{
		output_1[t] = l[16][t];
		output_1[32 + t] = r[16][t];
	}
	IP_Inv_Rep(output_1, output, IPR_Table);
}

/*
 *  DES 解密函数
 *  传入 64 位二进制密文 input 和密匙 inKey 获取明文 output
 */
void DES_Dec(const int input[64], char inKey[8], char output[8])
{
	int ip[64] = { 0 };
	int output_1[64] = { 0 };
	int output_2[64] = { 0 };
	int subKeys[16][48];
	int key[64];
	int l[17][32], r[17][32];

	IP_Init_Rep(input, ip, IP_Init_Table);
	CharToBit(inKey, key, 8);
	subKey_fun(key, subKeys);
	for (int i = 0; i < 32; i++)
	{
		l[0][i] = ip[i];
		r[0][i] = ip[32 + i];
	}
	for (int j = 1; j < 16; j++)    //  15 轮迭代
	{
		for (int k = 0; k < 32; k++)
		{
			l[j][k] = r[j - 1][k];
		}
		DES_Kernel(r[j - 1], r[j], subKeys[16 - j]);
		Xor(r[j], l[j - 1], 32);
	}

	int t = 0;
	for (t = 0; t < 32; t++)        //  第 16 轮迭代
	{
		r[16][t] = r[15][t];
	}
	DES_Kernel(r[15], l[16], subKeys[0]);
	Xor(l[16], l[15], 32);

	for (t = 0; t < 32; t++)
	{
		output_1[t] = l[16][t];
		output_1[32 + t] = r[16][t];
	}
	IP_Inv_Rep(output_1, output_2, IPR_Table);
	BitToChar(output_2, output, 8);
}