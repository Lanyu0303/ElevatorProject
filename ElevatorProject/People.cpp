#include "People.h"
#include <windows.h>
#include<bits/stdc++.h>
#include<string.h> 
#include<stdio.h> 
#include<stdlib.h>
#include"ConstantData.h"
using  namespace std;
extern People* null;//自己定义的一个People的空指针
People::People() {
	Id = -1;               //该用户的编号
	EnterTime = 0;         //进入时间
	ArriveFloor = 1;       //他所在的楼层
	TowardsFloor = 2;       //他要去哪层楼
	GiveupTime = 1000;     //他能容忍的等候时间
	pre = next = null;
}
People::People(int num) {
	Id = 0;               //该用户的编号
	EnterTime = 0;         //进入时间
	ArriveFloor = 0;       //他所在的楼层
	TowardsFloor = 0;       //他要去哪层楼
	GiveupTime = 0;     //他能容忍的等候时间
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
//转化为string类型
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
//实现cout<<输出信息
ostream& operator<<(ostream& os, People& people) {
	os << "Id =" << people.Id << ":" << "EnterTime =" << people.EnterTime << ":"
		<< "ArriveFloor =" << people.ArriveFloor << ":"
		<< "TowardsFloor =" << people.TowardsFloor << ":"
		<< "GiveupTime =" << people.GiveupTime << ":"
		<< endl;
	return os;
}
