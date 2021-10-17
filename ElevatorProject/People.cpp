#include "People.h"
#include <windows.h>
#include<bits/stdc++.h>
#include<string.h> 
#include<stdio.h> 
#include<stdlib.h>
#include"ConstantData.h"
using  namespace std;
extern People* null;//�Լ������һ��People�Ŀ�ָ��
People::People() {
	Id = -1;               //���û��ı��
	EnterTime = 0;         //����ʱ��
	ArriveFloor = 1;       //�����ڵ�¥��
	TowardsFloor = 2;       //��Ҫȥ�Ĳ�¥
	GiveupTime = 1000;     //�������̵ĵȺ�ʱ��
	pre = next = null;
}
People::People(int num) {
	Id = 0;               //���û��ı��
	EnterTime = 0;         //����ʱ��
	ArriveFloor = 0;       //�����ڵ�¥��
	TowardsFloor = 0;       //��Ҫȥ�Ĳ�¥
	GiveupTime = 0;     //�������̵ĵȺ�ʱ��
	pre = next = null;
}
const People& People::operator=(const People& people) {
	if (this == &people) {
		return *this;
	}
	Id = people.Id;
	EnterTime = people.EnterTime;
	ArriveFloor = people.ArriveFloor;
	TowardsFloor = people.TowardsFloor;
	GiveupTime = people.GiveupTime;
	pre = people.pre;
	next = people.next;
	return people;

}
//ת��Ϊstring����
/*
People::operator string(){
	cout << "People:" << endl;
	cout << "Id:" <<Id<< endl;
	cout << "EnterTime:" << EnterTime << endl;
	cout << "ArriVeFloor:" << ArriVeFloor << endl;
	cout << "TowardsFloor:" << TowardsFloor << endl;
	cout << "GiveupTime:" << GiveupTime << endl;
	cout << "FlagElevator0:" << FlagElevator0 << endl;
	cout << "FlagElevator1" << FlagElevator1 << endl;
}*/
//ʵ��cout<<�����Ϣ
ostream& operator<<(ostream& os, People& people) {
	os << "Id =" << people.Id << ":" << "EnterTime =" << people.EnterTime << ":"
		<< "ArriveFloor =" << people.ArriveFloor << ":"
		<< "TowardsFloor =" << people.TowardsFloor << ":"
		<< "GiveupTime =" << people.GiveupTime << ":"
		<< endl;
	return os;
}
