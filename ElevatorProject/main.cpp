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

//ʱ����
typedef struct TimeNode {
	int limittime;
	People person;
	int e;
	int event;
	TimeNode* next;
}*TimeP;//ʱ����ָ��
//����
int id = 1;
int Time = 0;//ʵ��ʱ��
int maxtime = MaxTime;//ʵ�ʵ��������ʱ��
int nexttime = 0;//������һ���˵ļ��ʱ��
TimeNode TimeInElevator[2] = {};//�����������ʱ�����
Elevator* elevator = new Elevator[2]();//��������
People* upqueue = new People[MaxFloor + 1]();//ÿ��¥����������
People* downque = new People[MaxFloor + 1]();//ÿ��¥���½�����
int ExistPeople[MaxPeople] = { 0 };//���Ƿ��ڶ�����
People nullpeo(0);
People* null = &nullpeo;
//��������
int mymin(int a, int b);//ȡ��Сֵ
void gotoxy(short x, short y);//�����ն��й���λ��
void Event(int e, int event,People people);

void Run();//����ģ�����
void PrintElevatorFrame();//������ݿ��
void ShowOthers();//���������Ϣ
void init(Elevator* elevaror, int j);//�Ե���elevator[i]���г�ʼ��
void DisappearWaitingQueue(int floor);//�������ڵȴ��Ķ���
void Addupqueue(People* people, int floor);//������������
void Adddownque(People* people, int floor);//�����½�����
void Showqueue(People* people, int floor);//չʾ����,��һ��Ϊupqueue��downque
void ShowWaitingQueue(int floor);//չʾfloor��ĵȴ�����


int main() {
	//��ʼ������
	init(elevator, 0);
	init(elevator, 1);
	//��ȡģ��ʱ��
	int tempmaxtime = MaxTime;
	cout << "�������������ʱ�䣬��Ҫ����" << MaxTime << endl;
	while (true) {
		cin >> tempmaxtime;
		if (tempmaxtime > MaxTime) {
			cout << "�������ʱ�䷶Χ������������" << endl;
		}
		else {
			maxtime = tempmaxtime;
			break;
		}
	}
	//����ģ��
	Run();
	gotoxy(0, 25);
	return 0;
}

//��������
//����ģ�����
void Run() {
	//��һ����
	elevator[0].GeneratePeople(NULL);
	//ʱ�䲻������
	while (Time < maxtime) {
		//���е��ݲ���
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
		//�����������������¼�������
		if (p == NULL && q == NULL) {
			Time = maxtime;
		}
		//���ʱ�����ϻ����¼�,ִ��
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
		printf("ʱ��Ϊ�� %d", Time);
		Sleep(1000 * UnitTime);
	}
	gotoxy(60, 30);
	cout << "��ʾ����������������˳�" << endl;
	return;
}
int mymin(int a, int b) {
	if (a < b) return a;
	else return b;
}
//�����ն��й���λ��
void gotoxy(short x, short y) {
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return;
}
//������ݿ��
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
//���������Ϣ
void ShowOthers() {
	gotoxy(14, 8);
	printf("����0");
	gotoxy(42, 8);
	printf("����1");
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
//�Ե���elevator[i]���г�ʼ��
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
//�������ڵȴ��Ķ���
void DisappearWaitingQueue(int floor) {
	//�����������ɫ���ñ�����ɫ��дһ�������ȥ���� 0�Ǻ�ɫ 7�ǰ�ɫ
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	ShowWaitingQueue(floor);
	//������������»ָ�������ɫ
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
//������������
void Addupqueue(People* people, int floor) {
	People* temp = &upqueue[floor];
	while (temp->next != null) {
		temp = temp->next;
	}
	temp->next = people;
	people->pre = temp;

}
//�����½�����
void Adddownque(People* people, int floor) {
	People* temp = &downque[floor];
	while (temp->next != null) {
		temp = temp->next;
	}
	temp->next = people;
	people->pre = temp;

}
//չʾ����
void Showqueue(People* people, int floor) {
	People* temp = &people[floor];
	temp = temp->next;
	cout << "��" << floor << "�����Ϊ:" << endl;
	while (temp->Id != null->Id) {
		cout << *temp << endl;
		temp = temp->next;
	}
	return;
}
//չʾfloor��ĵȴ�����
void ShowWaitingQueue(int floor) {
	People* p;
	p = &upqueue[floor];
	p = p->next;
	int j = 0;
	while (p->Id != null->Id)
	{
		gotoxy(RightElevatorX + 26 + j * 6, RightElevatorY - 1 + (4 - p->ArriveFloor) * FloorHight);
		printf(" %d", p->Id);
		printf("��");
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
		printf("��");
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