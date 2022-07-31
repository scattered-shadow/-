#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <random>
#include<vector>
using namespace std;

static unsigned char message_buffer[64] = { 0 };//512比特消息分组,一组共64字节
static unsigned int hash_[8] = { 0 };//初始IV：A B C D E F G H
static unsigned int T[64] = { 0 };//64轮的常数T[j]
const int N = 65536;
int sm3_ans[N];
string sm3_in[N];

//生成sm3的随机输入
string strRand(int length) {			
	char tmp;							
	string buffer;						

	random_device rd;					
	default_random_engine random(rd());	

	for (int i = 0; i < length; i++) {
		tmp = random() % 36;	
		if (tmp < 10) {			
			tmp += '0';
		}
		else {				
			tmp -= 10;
			tmp += 'a';
		}
		buffer += tmp;
	}
	return buffer;
}

//16进制输出
void out_hex()
{
	unsigned int i = 0;
	for (i = 0; i < 8; i++)
	{
		printf("%X", hash_[i]);
		printf(" ");
	}
	printf("\n");
}

//将data左移k位
unsigned int data_left(unsigned int data, unsigned int k)
{
	k = k % 32;
	return ((data << k) & 0xFFFFFFFF) | ((data & 0xFFFFFFFF) >> (32 - k));
}

//给64轮常数以及初始IV赋值
int init_SM3()
{
	int i = 0;
	for (i = 0; i < 16; i++)
	{
		T[i] = 0x79cc4519;
	}
	for (i = 16; i < 64; i++)
	{
		T[i] = 0x7a879d8a;
	}
	hash_[0] = 0x7380166f;
	hash_[1] = 0x4914b2b9;
	hash_[2] = 0x172442d7;
	hash_[3] = 0xda8a0600;
	hash_[4] = 0xa96f30bc;
	hash_[5] = 0x163138aa;
	hash_[6] = 0xe38dee4d;
	hash_[7] = 0xb0fb0e4e;
	return 0;
}

//布尔函数
unsigned int FF(unsigned int X, unsigned int Y, unsigned int Z, int j)
{
	unsigned int ret = 0;
	if (0 <= j && j < 16)
	{
		ret = X ^ Y ^ Z;
	}
	else if (16 <= j && j < 64)
	{
		ret = (X & Y) | (X & Z) | (Y & Z);
	}
	return ret;
}
unsigned int GG(unsigned int X, unsigned int Y, unsigned int Z, int j)
{
	unsigned int ret = 0;
	if (0 <= j && j < 16)
	{
		ret = X ^ Y ^ Z;
	}
	else if (16 <= j && j < 64)
	{
		ret = (X & Y) | ((X ^ 0xFFFFFFFF) & Z);
	}
	return ret;
}

//P_0置换和P_1置换
#define P_0(X)X^(data_left(X,9))^(data_left(X,17))
#define P_1(X)X^(data_left(X,15))^(data_left(X,23))

//消息扩展（132个消息字）和压缩函数
int CF(unsigned char* arr)
{
	unsigned int W_0[68];//前68个消息字
	unsigned int W_1[64];//后64个消息字
	unsigned int j;
	unsigned int A, B, C, D, E, F, G, H;//初始IV
	unsigned int SS1, SS2, TT1, TT2;//中间变量
	//将消息组分为16个消息字
	for (j = 0; j < 16; j++)
	{
		W_0[j] = arr[j * 4 + 0] << 24 | arr[j * 4 + 1] << 16 | arr[j * 4 + 2] << 8 | arr[j * 4 + 3];
	}
	//根据前16个消息字扩展W_0和W_1
	for (j = 16; j < 68; j++)
	{
		W_0[j] = P_1(W_0[j - 16] ^ W_0[j - 9] ^ (data_left(W_0[j - 3], 15))) ^ (data_left(W_0[j - 13], 7)) ^ W_0[j - 6];
	}
	for (j = 0; j < 64; j++)
	{
		W_1[j] = W_0[j] ^ W_0[j + 4];
	}
	A = hash_[0];
	B = hash_[1];
	C = hash_[2];
	D = hash_[3];
	E = hash_[4];
	F = hash_[5];
	G = hash_[6];
	H = hash_[7];
	//进行64轮函数压缩
	for (j = 0; j < 64; j++)
	{
		SS1 = data_left(((data_left(A, 12)) + E + (data_left(T[j], j))) & 0xFFFFFFFF, 7);
		SS2 = SS1 ^ (data_left(A, 12));
		TT1 = (FF(A, B, C, j) + D + SS2 + W_1[j]) & 0xFFFFFFFF;
		TT2 = (GG(E, F, G, j) + H + SS1 + W_0[j]) & 0xFFFFFFFF;
		D = C;
		C = data_left(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = data_left(F, 19);
		F = E;
		E = P_0(TT2);
	}
	//与原来的IV异或得到新的IV
	hash_[0] = (A ^ hash_[0]);
	hash_[1] = (B ^ hash_[1]);
	hash_[2] = (C ^ hash_[2]);
	hash_[3] = (D ^ hash_[3]);
	hash_[4] = (E ^ hash_[4]);
	hash_[5] = (F ^ hash_[5]);
	hash_[6] = (G ^ hash_[6]);
	hash_[7] = (H ^ hash_[7]);
	return 0;
}

//消息填充并代入压缩
void Block(const char* msg, unsigned int msg_len)
{
	int i;
	int left = 0;
	unsigned long long total = 0;
	//先将完整的消息组放入压缩函数CF()中
	for (i = 0; i < msg_len / 64; i++)
	{
		memcpy(message_buffer, msg + i * 64, 64);
		CF(message_buffer);
	}
	total = msg_len * 8;//字节长度转化为比特长度
	left = msg_len % 64;
	//消息末尾后填比特"1"，后用0补齐。
	memset(&message_buffer[left], 0, 64 - left);
	memcpy(message_buffer, msg + i * 64, left);
	message_buffer[left] = 0x80;
	//追加64比特的消息比特长度
	if (left <= 55)
	{
		for (i = 0; i < 8; i++)
		{
			message_buffer[56 + i] = (total >> ((7 - i) * 8)) & 0xFF;
		}
		CF(message_buffer);
	}
	else
	{
		CF(message_buffer);
		memset(message_buffer, 0, 64);
		for (i = 0; i < 8; i++)
		{
			message_buffer[56 + i] = (total >> ((7 - i) * 8)) & 0xFF;
		}
		CF(message_buffer);
	}
}

//sm3总函数
int SM3(const char* msg, unsigned int msg_len)
{
	init_SM3();
	Block(msg, msg_len);
	//out_hex();
	return 0;
}
//寻找碰撞
int find_collision(int len)
{
	for (int i = 0; i < len; i++)
	{
		if (sm3_ans[i] == sm3_ans[len])
		{
			return i;
		}
	}
	return -1;
}
//生日攻击
bool attack()
{
	
	for (int i = 0; i < N; i++)
	{
		string str_in = strRand(32);
		const char* str = str_in.data();
		int len = strlen(str);
		SM3(str, len);
		sm3_in[i] = str_in;
		sm3_ans[i] = hash_[0];

		int temp = find_collision(i);
		if (temp!= -1)
		{
			cout << "找到一组碰撞" << endl;
			cout << "明文为：" << sm3_in[temp] << "\t" << sm3_in[i]<<endl;
			cout << "其密文前32位为：" ;
			printf("%X\n", hash_[0]);
			return true;
		}
	}
	cout << "未找到碰撞" << endl;
	return false;
}
void Birthday_attack()
{
	int n = 100, sum = 0;
	
	for (int j = 1; j <= n; j++)
	{
		if (attack())
		{
			sum++;
		}
	}
	cout << "共进行了" << n << "次生日攻击" << endl;
	cout << "攻击成功" << sum << "次" << endl;
}
int main()
{
	Birthday_attack();
	return 0;
}