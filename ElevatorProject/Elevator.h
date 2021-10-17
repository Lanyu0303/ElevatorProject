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
	int i;//表示第i座电梯
	int PeopleNum;//电梯内人数
	int CallUp[5];                   //电梯外向上按钮,在第i层有向上的请求
	int CallDown[5];                 //电梯外向下按钮,在第i层有向下的请求
	int CallCar[5];               //电梯内按钮
	int again;						//操作一次后是否继续操作
	int drcl, drcltime, testdoortime; //电梯门参量
	int state;                      //电梯的当前状态(GoingUp，GoingDown，Idle)
	int nextstate;                   //电梯下步状态(GoingUp，GoingDown，Idle)
	int Floor;                      //电梯的当前位置(楼层)
	int d1;                         //值为0，除非人们正在进入和离开电梯；
	int d2;                         //值为0，如果电梯 已经在某层停候30Ot以上；
	int d3;                         //值为0，除非电梯 门正开着又无人迸出电梯；
	struct PeopleInElevator {//电梯里人的队列
		int id;
		struct PeopleInElevator* next;
	};

	struct PeopleInElevator peopleinelevator[MaxFloor + 1] = { (-1,NULL) };//peopleinelevator[i]为电梯中第i层的人
	//struct TimeLine TimeInElevator; //电梯里的时序监控
	int backfirsttime;

	//构造函数
	Elevator();
	void AddTime(int newtime, People people, void(Elevator::*newf)(People people));//增加时间
	void Controler();//控制函数
	void GeneratePeople(People people);//产生新的人
	void GiveupWait(People people);//放弃等待
	void OpenDoor(People people);//开门
	void CloseDoor(People people);//关门
	void PeopleIn(People people);//人进入
	void PeopleOut(People people);//人出去
	void FinishInOut(People people);//结束进出
	void IntoNormalSpeed(People people);//进入正常状态
	void GoBaseFloor(People people);//返回基层
	void ElevatorUp(People people);//电梯上升
	void ElevatorDown(People people);//电梯下降
	void ElevatorStop(People people);//电梯下降
	void ShowFloor(int floor);//在第floor层显示电梯
	void HiddenFloor(int floor);//在第floor层显示电梯
	int GetNextState();//获取下一步的状态
	void AddPeople(int newid, int newfloor);//增加新人
	int DeletePeople(int newfloor);//删除电梯里的人
	int isUp();//电梯是否上升
	int isDown();//电梯是否下降
	
};

