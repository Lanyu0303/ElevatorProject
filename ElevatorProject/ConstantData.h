#pragma once
#include <windows.h>
#include<bits/stdc++.h>
#include<string.h> 
#include<stdio.h> 
#include<stdlib.h>

//��ʱ����صĳ���
constexpr auto UnitTime = 0.1;        //��λʱ�� 
constexpr auto AccelerateTime = 15;	  //���ݼ���ʱ��
constexpr auto DoorTime = 20;		  //���ݹ���ʱ��
constexpr auto UpTime = 51;			  //��������ʱ��
constexpr auto DownTime = 61;		  //�����½�ʱ��
constexpr auto UpSlowdownTime = 14;   //������������ʱ�� 
constexpr auto DownSlowdownTime = 23; //�����½�����ʱ��
constexpr auto PeopleTime = 25;		  //�˽������ʱ��
constexpr auto CheckDoorTime = 40;		  //����Ƿ����ʱ��
constexpr auto BackTime = 300;		  //����Ƿ�ص���һ��ʱ��
constexpr auto MinGiveupTime = 150;	  //�������ʱ��
constexpr auto MaxGiveupTime = 5000;  //�������ʱ��
constexpr auto MinNextTime = 10;	  //�����һ���˲���ʱ��
constexpr auto MaxNextTime = 100;	  //�����һ���˲���ʱ��

//��ģ����Ŀ��صĳ���
constexpr auto MaxPeople = 10000;	  //ģ��ʱ���������
constexpr auto MaxTime = 10000;		  //ģ��ʱ�����ʱ��
constexpr auto MaxElevatorPeople = 8; //ÿ������������
constexpr auto MaxWaitNumInFloor = 7; //ÿһ������ȴ�����
constexpr auto BaseFloor = 1;		  //¥�Ļ��� 
constexpr auto MaxFloor = 4;		  //¥��������0��4��

//��������
constexpr auto GoingUp = 1;           //����״̬���� 
constexpr auto GoingDown = -1;        //����״̬�½� 
constexpr auto Idle = 0;		      //����״̬�Ⱥ� 

//ģ�⻭ͼ���
constexpr auto FloorHight = 3;		  //ÿһ��¥�ĸ߶�
constexpr auto LeftElevatorX = 6;
constexpr auto LeftElevatorY = 12;
constexpr auto RightElevatorX = 34;
constexpr auto RightElevatorY = 12;