#pragma once
#include <windows.h>
#include<bits/stdc++.h>
#include<string.h> 
#include<stdio.h> 
#include<stdlib.h>
#include"ConstantData.h"
using  namespace std;

class People{
public:
	int Id;                //该用户的编号
	int EnterTime;         //进入时间
	int ArriveFloor;       //他所在的楼层
	int TowardsFloor;       //他要去哪层楼
	int GiveupTime;        //他能容忍的等候时间
	People* pre;			//上一个人
	People* next;			//下一个人

	People();//构造函数
	People(int num);//转化函数
	const People& operator=(const People& people);
	//operator string();//转化为字符串类型
	friend ostream& operator << (ostream& os, People& people);
};

