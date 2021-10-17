#pragma once
#include <windows.h>
#include<bits/stdc++.h>
#include<string.h> 
#include<stdio.h> 
#include<stdlib.h>
#include"ConstantData.h"
#include "People.h"
#include "Elevator.h"
using  namespace std;

//时间轴
typedef struct TimeNode {
	int limittime;
	People person;
	int e;
	int event;
	TimeNode* next;
}*TimeP;//时间轴指针
//变量
int id = 1;
int Time = 0;//实际时间
int maxtime = MaxTime;//实际的最大运行时间
int nexttime = 0;//产生下一个人的间隔时间
TimeNode TimeInElevator[2] = {};//两个电梯里的时序控制
Elevator* elevator = new Elevator[2]();//两个电梯
People* upqueue = new People[MaxFloor + 1]();//每层楼的上升队列
People* downque = new People[MaxFloor + 1]();//每层楼的下降队列
int ExistPeople[MaxPeople] = { 0 };//人是否在队列中
People nullpeo(0);
People* null = &nullpeo;
//函数声明
int mymin(int a, int b);//取最小值
void gotoxy(short x, short y);//设置终端中光标的位置
void Event(int e, int event,People people);

void Run();//运行模拟程序
void PrintElevatorFrame();//输出电梯框架
void ShowOthers();//输出其他信息
void init(Elevator* elevaror, int j);//对电梯elevator[i]进行初始化
void DisappearWaitingQueue(int floor);//消除正在等待的队伍
void Addupqueue(People* people, int floor);//加入上升队列
void Adddownque(People* people, int floor);//加入下降队列
void Showqueue(People* people, int floor);//展示队列,第一个为upqueue或downque
void ShowWaitingQueue(int floor);//展示floor层的等待队列


int main() {
	//初始化电梯
	init(elevator, 0);
	init(elevator, 1);
	//获取模拟时间
	int tempmaxtime = MaxTime;
	cout << "请输入最大运行时间，不要超过" << MaxTime << endl;
	while (true) {
		cin >> tempmaxtime;
		if (tempmaxtime > MaxTime) {
			cout << "超出最大时间范围，请重新输入" << endl;
		}
		else {
			maxtime = tempmaxtime;
			break;
		}
	}
	//进行模拟
	Run();
	gotoxy(0, 25);
	return 0;
}

//函数定义
//运行模拟程序
void Run() {
	//第一个人
	elevator[0].GeneratePeople(NULL);
	//时间不断增加
	while (Time < maxtime) {
		//进行电梯操作
		do {
			elevator[0].again = 0;
			elevator[0].Controler();
		} while (elevator[0].again == 1);
		do {
			elevator[1].again = 0;
			elevator[1].Controler();
		} while (elevator[1].again == 1);

		TimeP p = TimeInElevator[0].next;
		TimeP q = TimeInElevator[1].next;
		//做完操作后如果所有事件都结束
		if (p == NULL && q == NULL) {
			Time = maxtime;
		}
		//如果时间轴上还有事件,执行
		while (p && p->limittime <= Time) {
			TimeInElevator[0].next = p->next;
			Event(p->e, p->event,p->person);
			free(p);
			p = TimeInElevator[0].next;
		}
		while (q && q->limittime <= Time) {
			TimeInElevator[1].next = q->next;
			Event(q->e, q->event,q->person);
			free(q);
			q = TimeInElevator[1].next;
		}
		Time++;
		gotoxy(20, 2);
		printf("时间为： %d", Time);
		Sleep(1000 * UnitTime);
	}
	gotoxy(60, 30);
	cout << "演示结束，按下任意键退出" << endl;
	return;
}
int mymin(int a, int b) {
	if (a < b) return a;
	else return b;
}
//设置终端中光标的位置
void gotoxy(short x, short y) {
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return;
}
//输出电梯框架
void PrintElevatorFrame() {
	int j, i, k;
	for (i = 0; i <= 1; i++) {
		for (j = MaxFloor + 1; j >= 0; j--) {
			if (j == MaxFloor) {
				gotoxy(6 + i * 27, 9);
				printf("--------------------------");
			}
			gotoxy(6 + i * 27, 9 + j * FloorHight);
			printf("--------------------------");

		}
		j = MaxFloor + 1;
		for (k = 9; k <= 9 + j * FloorHight; k++) {
			gotoxy(5 + i * 27, k);
			printf("|");
			gotoxy(5 + i * 27 + 26, k);
			printf("|");
		}
	}
}
//输出其他信息
void ShowOthers() {
	gotoxy(14, 8);
	printf("电梯0");
	gotoxy(42, 8);
	printf("电梯1");
	for (int i = 0; i <= MaxFloor; i++) {
		gotoxy(LeftElevatorX - 2, LeftElevatorY - 1 + (4 - i) * FloorHight);
		printf("%d", i);
	}
	/*
	gotoxy(3, 4);
	printf("%s", ElevatorChar[0]);
	gotoxy(33, 4);
	printf("%s", ElevatorChar[1]);
	gotoxy(3, 6);
	printf("%s", PeopleChar6[0]);
	gotoxy(33, 6);
	printf("%s", PeopleChar6[1]);
	gotoxy(3, 7);
	printf("%s", PeopleChar7[0]);
	gotoxy(33, 7);
	printf("%s", PeopleChar7[0]);
	*/
}
//对电梯elevator[i]进行初始化
void init(Elevator* elevaror, int j) {
	elevator[j] = Elevator();
	elevator[j].i = j;
	TimeInElevator[j].e = 1;
	TimeInElevator[j].event = 0;
	TimeInElevator[j].next = NULL;
	TimeInElevator[j].person = NULL;
	TimeInElevator[j].limittime = 0;
	int tempfloor = 0;
	for (tempfloor = 0; tempfloor <= MaxFloor; tempfloor++) {
		elevaror[j].peopleinelevator[tempfloor].id = 0;
		elevaror[j].peopleinelevator[tempfloor].next = NULL;
		upqueue[tempfloor].pre = null;
		upqueue[tempfloor].next = null;
		downque[tempfloor].pre = null;
		downque[tempfloor].next = null;
	}
}
//消除正在等待的队伍
void DisappearWaitingQueue(int floor) {
	//设置字体的颜色，用背景颜色再写一遍就能消去字体 0是黑色 7是白色
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	ShowWaitingQueue(floor);
	//消隐完后再重新恢复字体颜色
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
//加入上升队列
void Addupqueue(People* people, int floor) {
	People* temp = &upqueue[floor];
	while (temp->next != null) {
		temp = temp->next;
	}
	temp->next = people;
	people->pre = temp;

}
//加入下降队列
void Adddownque(People* people, int floor) {
	People* temp = &downque[floor];
	while (temp->next != null) {
		temp = temp->next;
	}
	temp->next = people;
	people->pre = temp;

}
//展示队列
void Showqueue(People* people, int floor) {
	People* temp = &people[floor];
	temp = temp->next;
	cout << "第" << floor << "层队列为:" << endl;
	while (temp->Id != null->Id) {
		cout << *temp << endl;
		temp = temp->next;
	}
	return;
}
//展示floor层的等待队列
void ShowWaitingQueue(int floor) {
	People* p;
	p = &upqueue[floor];
	p = p->next;
	int j = 0;
	while (p->Id != null->Id)
	{
		gotoxy(RightElevatorX + 26 + j * 6, RightElevatorY - 1 + (4 - p->ArriveFloor) * FloorHight);
		printf(" %d", p->Id);
		printf("↑");
		printf("%d", p->TowardsFloor);
		p = p->next;
		j++;
	}
	p = &downque[floor];
	p = p->next;
	while (p->Id != null->Id)
	{
		gotoxy(RightElevatorX + 26 + j * 6, RightElevatorY - 1 + (4 - p->ArriveFloor) * FloorHight);
		printf(" %d", p->Id);
		printf("↓");
		printf("%d", p->TowardsFloor);
		p = p->next;
		j++;
	}
}

void Event(int e, int event,People people) {
	switch (event) {
		case 1:
			elevator[e].GeneratePeople(people);
			break;
		case 2:
			elevator[e].GiveupWait(people);
			break;
		case 3:
			elevator[e].OpenDoor(people);
			break;
		case 4:
			elevator[e].CloseDoor(people);
			break;
		case 5:
			elevator[e].PeopleIn(people);
			break;
		case 6:
			elevator[e].PeopleOut(people);
			break;
		case 7:
			elevator[e].FinishInOut(people);
			break;
		case 8:
			elevator[e].IntoNormalSpeed(people);
			break;
		case 9:
			elevator[e].GoBaseFloor(people);
			break;
		case 10:
			elevator[e].ElevatorUp(people);
			break;
		case 11:
			elevator[e].ElevatorDown(people);
			break;
		case 12:
			elevator[e].ElevatorStop(people);
			break;
		default:
			break;
	}
}