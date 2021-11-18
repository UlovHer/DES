#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "tables.h"
#include "des.h"

/*
 *  ����ת������ I
 *  �� char ��ת��Ϊ��������ʽ
 *  8 * sizeof(char) = 8(λ) 8 ���ַ� 64 λ
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
 *  ����ת������ II
 *  ����������ʽת��Ϊ char ��
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
 *  �����
 *  ������ INA �� INB ���������������ұ����� INA ��
 */
void Xor(int *INA, int *INB, int len)
{
	for (int i = 0; i < len; i++)
	{
		*(INA + i) = *(INA + i) ^ *(INB + i);
	}
}

/*
 *  IP ��ʼ�û�����
 *  IP ���� IP ��ʼ�û�����г�ʼ�û�
 */
void IP_Init_Rep(const int input[64], int output[64], const int table[64])
{
	for (int i = 0; i < 64; i++)
	{
		output[i] = input[table[i] - 1];
	}
}

/*
 *  E ��չ�û�����
 *  ���� E ��չ�������չ
 */
void E_Extend(const int input[32], int output[48], const int table[48])
{
	for (int i = 0; i < 48; i++)
	{
		output[i] = input[table[i] - 1];
	}
}

/*
 *  P �û�����
 *  ���� P �н����û�
 */
void P_Rep(const int input[32], int output[32], const int table[32])
{
	for (int i = 0; i < 32; i++)
	{
		output[i] = input[table[i] - 1];
	}
}

/*
 *  IP ���û�����
 *  IP ���� IP ���û�������û�
 */
void IP_Inv_Rep(const int input[64], int output[64], const int table[64])
{
	for (int i = 0; i < 64; i++)
	{
		output[i] = input[table[i] - 1];
	}
}

/*
 *  �ܳ׵�һ���û�����
 *  �����ܳ׵�һ���û�������û�
 */
void PC_1(const int input[64], int output[56], const int table[56])
{
	for (int i = 0; i < 56; i++)
	{
		output[i] = input[table[i] - 1];
	}
}

/*
 *  �ܳ׵ڶ����û�����
 *  �����ܳ׵ڶ����û�������û�
 */
void PC_2(const int input[56], int output[48], const int table[48])
{
	for (int i = 0; i < 48; i++)
	{
		output[i] = input[table[i] - 1];
	}
}

/*
 *  S ��ѹ������
 *  ���� 8 �� S �н���ѹ��
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
 *  �ֵ�������
 *  DES ���ĵ�������
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
 *  �ܳ�ѭ�����ƺ���
 *  �ܳ��ڲ�ͬ������Ҫ���в�ͬ�����Ʋ���
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
 *  ���ܳ����ɺ���
 *  ���� subKey���ڵ� 1��2��9��16 ��ѭ������ 1 λ��������ѭ������ 2 λ
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
 *  DES ���ܺ���
 *  �������� input ���ܳ� inKey����ȡ 64 λ���������� output
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
	for (int j = 1; j < 16; j++)    //  15 �ֵ���
	{
		for (int k = 0; k < 32; k++)
		{
			l[j][k] = r[j - 1][k];
		}
		DES_Kernel(r[j - 1], r[j], subKeys[j - 1]);
		Xor(r[j], l[j - 1], 32);
	}

	int t = 0;
	for (t = 0; t < 32; t++)        //  �� 16 �ֵ���
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
 *  DES ���ܺ���
 *  ���� 64 λ���������� input ���ܳ� inKey ��ȡ���� output
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
	for (int j = 1; j < 16; j++)    //  15 �ֵ���
	{
		for (int k = 0; k < 32; k++)
		{
			l[j][k] = r[j - 1][k];
		}
		DES_Kernel(r[j - 1], r[j], subKeys[16 - j]);
		Xor(r[j], l[j - 1], 32);
	}

	int t = 0;
	for (t = 0; t < 32; t++)        //  �� 16 �ֵ���
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