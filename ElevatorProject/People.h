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
	int Id;                //���û��ı��
	int EnterTime;         //����ʱ��
	int ArriveFloor;       //�����ڵ�¥��
	int TowardsFloor;       //��Ҫȥ�Ĳ�¥
	int GiveupTime;        //�������̵ĵȺ�ʱ��
	People* pre;			//��һ����
	People* next;			//��һ����

	People();//���캯��
	People(int num);//ת������
	const People& operator=(const People& people);
	//operator string();//ת��Ϊ�ַ�������
	friend ostream& operator << (ostream& os, People& people);
};

