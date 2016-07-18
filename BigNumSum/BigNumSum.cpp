// BigNumSum.cpp : Defines the entry point for the console application.
//
//20160715
//大数加法器-Zephyr

/*
基本思路，化而治之
123456789
*	  211
---------
123456789
+1234567890
+24691357800
------------
=26049382479
*/
#include "stdafx.h"
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <string>

using namespace std;

const int ConstMaxLength = 10;
const int ConstMaxPos = ConstMaxLength - 1;


struct SBigNumberVec
{
	//Just long long int? come on, I can do it invinite 

	int mpVec[ConstMaxLength];
	int mLength;
	int mLastPos;

	SBigNumberVec()
	{
		//mpVec = new int[ConstMaxLength];
		Zero();
	}

	~SBigNumberVec()
	{
		if (NULL != mpVec)
		{
			//delete mpVec;
		}
	}

	const int GetLength()
	{
		return mLength;
	}

	const int GetLastPos()
	{
		return mLastPos;
	}



	int *GetVec()
	{
		return mpVec;
	}

	void OutPut()
	{
		for (int j = mLastPos; j <= ConstMaxPos; ++j)//从不为零的数位开始输出结果
		{
			cout << mpVec[j];
		}
		if (0 == mLength)
		{
			cout << "0";
		}
		cout << endl;
	}

	void Copy(SBigNumberVec *srcVec)
	{
		Zero();
		for (int j = ConstMaxPos; j >= srcVec->GetLastPos(); --j)
		{
			mpVec[j] = srcVec->GetVec()[j];
		}
		CalculateLength();
	}

	void Zero()
	{
		for (int j = 0; j <= ConstMaxPos; ++j)
		{
			mpVec[j] = 0;
		}
		mLength = 0;
		mLastPos = ConstMaxPos;
	}

	void GetInputBigNumber()
	{
		Zero();
		string strInputNum;
		cin >> strInputNum;
		SetBigNumber(strInputNum);
	}

	void SetBigNumber(string inStr)
	{
		Zero();
		mLength = inStr.length();
		mLastPos = ConstMaxLength - mLength;
		char c;
		for (int i = 0; i < mLength; ++i)
		{
			c = inStr[i];
			mpVec[mLastPos + i] = atoi(&c);
		}
	}

	void Increase()
	{
		mpVec[ConstMaxPos]++;
		for (int j = mLength; j > 0; --j)
		{
			if (mpVec[j] > 10)
			{
				mpVec[j] -= 10;
				++mpVec[j - 1];
			}
			else
				break;
		}
		CalculateLength();
	}


	//相加的时候向前提了k个位置
	void AddSum(SBigNumberVec * inVec, int k = 0)
	{
		//有漏洞，但可以把ConstMaxPos设大
		for (int j = ConstMaxPos - k; j >= inVec->GetLastPos() - k; --j)
		{
			mpVec[j] += inVec->GetVec()[j + k];
			if (mpVec[j] >= 10)
			{
				if (j >= ConstMaxLength)
					assert("Over flow!");
				if (mpVec[j] < 0 || mpVec[j] > 19)
					assert("Oh no");
				mpVec[j] -= 10;
				if (mpVec[j] > 8)
					assert("Max 9+9-10 = 8");
				++mpVec[j - 1];
			}
		}
		CalculateLength();
	}

	void Multi(int iMultiNum)//十进制乘法
	{
		if (iMultiNum < 0 || iMultiNum > 9)
			assert("Wrong Number");

		if (0 == iMultiNum || 0 == mLength)
		{
			Zero();
			return;
		}
		int t = 0;
		int j = 0;
		for (j = ConstMaxPos; j >= mLastPos; --j)
		{
			mpVec[j] *= iMultiNum;
			if (t)
			{
				mpVec[j] += t;
				t = 0;
			}
			//进位
			if (mpVec[j] > 10)
			{
				t = mpVec[j] / 10;
				mpVec[j] = mpVec[j] % 10;
			}
		}
		if (t)
		{
			mpVec[j] += t;
			t = 0;
		}
		CalculateLength();
	}

	void MultiX(SBigNumberVec *inVec)//大数相乘
	{
		SBigNumberVec oriVec;
		oriVec.Copy(this);
		for (int j = ConstMaxPos; j >= inVec->GetLastPos(); j--)
		{
			SBigNumberVec tmpVec;
			tmpVec.Copy(&oriVec);
			tmpVec.Multi(inVec->mpVec[j]);
			int k = ConstMaxPos - j;
			if (0 == k)
				Copy(&tmpVec); //第一位
			else
				AddSum(&tmpVec, k);
		}
		CalculateLength();
	}
private:
	void CalculateLength()
	{
		int i = 0;
		while (mpVec[i] <= 0 && i <= ConstMaxPos)
			++i;
		mLength = ConstMaxLength - i;
		mLastPos = i;
	}
};



int main(){

	SBigNumberVec totalVec, tempVec;

	int nInputNumber;
	cout << "Input the count of input numbers:" << endl;
	cin >> nInputNumber;

	do
	{
		
		totalVec.SetBigNumber("1");
		for (int arr_i = 0; arr_i < nInputNumber; arr_i++)
		{
			cout << "Input the #" << arr_i + 1 << " big number:" << endl;
			tempVec.GetInputBigNumber();
			//totalVec.AddSum(&tempVec);
			totalVec.MultiX(&tempVec);
		}
		cout << "The Final Result:" << endl;
		totalVec.OutPut();
	} while (0 != nInputNumber);

	return 0;
}