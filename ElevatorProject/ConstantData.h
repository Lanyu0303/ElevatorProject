#pragma once
#include <windows.h>
#include<bits/stdc++.h>
#include<string.h> 
#include<stdio.h> 
#include<stdlib.h>

//与时间相关的常量
constexpr auto UnitTime = 0.1;        //单位时间 
constexpr auto AccelerateTime = 15;	  //电梯加速时间
constexpr auto DoorTime = 20;		  //电梯关门时间
constexpr auto UpTime = 51;			  //电梯上升时间
constexpr auto DownTime = 61;		  //电梯下降时间
constexpr auto UpSlowdownTime = 14;   //电梯上升减速时间 
constexpr auto DownSlowdownTime = 23; //电梯下降减速时间
constexpr auto PeopleTime = 25;		  //人进入电梯时间
constexpr auto CheckDoorTime = 40;		  //检查是否关门时间
constexpr auto BackTime = 300;		  //检查是否回到第一层时间
constexpr auto MinGiveupTime = 150;	  //最低忍耐时间
constexpr auto MaxGiveupTime = 5000;  //最高忍耐时间
constexpr auto MinNextTime = 10;	  //最低下一个人产生时间
constexpr auto MaxNextTime = 100;	  //最高下一个人产生时间

//与模拟数目相关的常量
constexpr auto MaxPeople = 10000;	  //模拟时的最大人数
constexpr auto MaxTime = 10000;		  //模拟时的最大时间
constexpr auto MaxElevatorPeople = 8; //每层电梯最多人数
constexpr auto MaxWaitNumInFloor = 7; //每一层的最大等待人数
constexpr auto BaseFloor = 1;		  //楼的基层 
constexpr auto MaxFloor = 4;		  //楼的最大层数0到4层

//电梯运行
constexpr auto GoingUp = 1;           //电梯状态上升 
constexpr auto GoingDown = -1;        //电梯状态下降 
constexpr auto Idle = 0;		      //电梯状态等候 

//模拟画图相关
constexpr auto FloorHight = 3;		  //每一层楼的高度
constexpr auto LeftElevatorX = 6;
constexpr auto LeftElevatorY = 12;
constexpr auto RightElevatorX = 34;
constexpr auto RightElevatorY = 12;