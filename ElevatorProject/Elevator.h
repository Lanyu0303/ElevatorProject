#pragma once
#include "People.h"
#include <windows.h>
#include<bits/stdc++.h>
#include<string.h> 
#include<stdio.h> 
#include<stdlib.h>
#include"ConstantData.h"
using  namespace std;



class Elevator{
public:
	int i;//��ʾ��i������
	int PeopleNum;//����������
	int CallUp[5];                   //���������ϰ�ť,�ڵ�i�������ϵ�����
	int CallDown[5];                 //���������°�ť,�ڵ�i�������µ�����
	int CallCar[5];               //�����ڰ�ť
	int again;						//����һ�κ��Ƿ��������
	int drcl, drcltime, testdoortime; //�����Ų���
	int state;                      //���ݵĵ�ǰ״̬(GoingUp��GoingDown��Idle)
	int nextstate;                   //�����²�״̬(GoingUp��GoingDown��Idle)
	int Floor;                      //���ݵĵ�ǰλ��(¥��)
	int d1;                         //ֵΪ0�������������ڽ�����뿪���ݣ�
	int d2;                         //ֵΪ0��������� �Ѿ���ĳ��ͣ��30Ot���ϣ�
	int d3;                         //ֵΪ0�����ǵ��� �������������˱ų����ݣ�
	struct PeopleInElevator {//�������˵Ķ���
		int id;
		struct PeopleInElevator* next;
	};

	struct PeopleInElevator peopleinelevator[MaxFloor + 1] = { (-1,NULL) };//peopleinelevator[i]Ϊ�����е�i�����
	//struct TimeLine TimeInElevator; //�������ʱ����
	int backfirsttime;

	//���캯��
	Elevator();
	void AddTime(int newtime, People people, void(Elevator::*newf)(People people));//����ʱ��
	void Controler();//���ƺ���
	void GeneratePeople(People people);//�����µ���
	void GiveupWait(People people);//�����ȴ�
	void OpenDoor(People people);//����
	void CloseDoor(People people);//����
	void PeopleIn(People people);//�˽���
	void PeopleOut(People people);//�˳�ȥ
	void FinishInOut(People people);//��������
	void IntoNormalSpeed(People people);//��������״̬
	void GoBaseFloor(People people);//���ػ���
	void ElevatorUp(People people);//��������
	void ElevatorDown(People people);//�����½�
	void ElevatorStop(People people);//�����½�
	void ShowFloor(int floor);//�ڵ�floor����ʾ����
	void HiddenFloor(int floor);//�ڵ�floor����ʾ����
	int GetNextState();//��ȡ��һ����״̬
	void AddPeople(int newid, int newfloor);//��������
	int DeletePeople(int newfloor);//ɾ�����������
	int isUp();//�����Ƿ�����
	int isDown();//�����Ƿ��½�
	
};

