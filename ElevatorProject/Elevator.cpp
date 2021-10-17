#include "Elevator.h"
extern void gotoxy(short x, short y);
extern People* upqueue;
extern People* downque;
extern People* null;
extern int Time;
extern int id;
extern int maxtime;
extern int nexttime;
extern int ExistPeople[MaxPeople];
extern Elevator* elevator ;
extern int mymin(int a, int b);
typedef struct TimeNode {
	int limittime;
	People person;
	int e;
	int event;
	TimeNode* next;
}*TimeP;//时间轴指针
extern TimeNode TimeInElevator[2]; //两个电梯里的时序控制
extern void Addupqueue(People* people, int floor);//加入上升队列
extern void Adddownque(People* people, int floor);//加入下降队列
extern void Showqueue(People* people, int floor);//展示队列,第一个为upqueue或downque
extern void ShowWaitingQueue(int floor);//展示floor层的等待队列
extern void PrintElevatorFrame();//输出电梯框架
extern void ShowOthers();//输出其他信息
extern void DisappearWaitingQueue(int floor);//消除正在等待的队伍
Elevator::Elevator() {
	i = 0;
	PeopleNum = 0;
	memset(CallUp, 0, sizeof(CallUp));
	memset(CallDown, 0, sizeof(CallDown));
	memset(CallCar, 0, sizeof(CallCar));
	again = 0;
	drcl = 0;
	drcltime = 0;
	testdoortime = 0;
	state = Idle;
	nextstate = Idle;
	Floor = BaseFloor;
	d1 = 0; d2 = 0; d3 = 0;
	backfirsttime = 0;
}
//增加时间节点
void Elevator::AddTime(int newtime, People people, void(Elevator::*newf)(People people)) {
	//建立一个新的时间节点
	TimeP newNode = (TimeP)malloc(sizeof(struct TimeNode));
	newNode->limittime = Time + newtime;
	newNode->person = people;
	newNode->e = i;
	if (newf == &Elevator::GeneratePeople) {
		newNode->event = 1;
	}
	else if (newf == &Elevator::GiveupWait) {
		newNode->event = 2;
	}
	else if (newf == &Elevator::OpenDoor) {
		newNode->event = 3;
	}
	else if (newf == &Elevator::CloseDoor) {
		newNode->event = 4;
	}
	else if (newf == &Elevator::PeopleIn) {
		newNode->event = 5;
	}
	else if (newf == &Elevator::PeopleOut) {
		newNode->event = 6;
	}
	else if (newf == &Elevator::FinishInOut) {
		newNode->event = 7;
	}
	else if (newf == &Elevator::IntoNormalSpeed) {
		newNode->event = 8;
	}
	else if (newf == &Elevator::GoBaseFloor) {
		newNode->event = 9;
	}
	else if (newf == &Elevator::ElevatorUp) {
		newNode->event = 10;
	}
	else if (newf == &Elevator::ElevatorDown) {
		newNode->event = 11;
	}
	else if (newf == &Elevator::ElevatorStop) {
		newNode->event = 12;
	}
	else {
		newNode->event = 0;
	}
	newNode->next = NULL;
	//将这个节点插入到电梯的时间轴里
	TimeP p = &TimeInElevator[i];
	while (p->next != NULL) {
		if (p->next->limittime > newNode->limittime) break;
		p = p->next;
	}
	newNode->next = p->next;
	p->next = newNode;
}
//控制函数
void Elevator::Controler() {
	int flag, j;
	if ( state == Idle) {//如果电梯现在静止
		if ( nextstate == Idle) {//如果下一动作还是静止，就重新获得下一动作
			 nextstate =  GetNextState();
		}
		if ( d1 == 0 &&  d3 == 0 &&  CallCar[ Floor] == 2) {//如果门开着，且没有人进出电梯,且到这层有两个需求而
			 CallCar[ Floor] = 0;//将需求清零
			//可以改写
			flag = 0;
			for (j =  Floor + 1; j <= MaxFloor; j++) {//寻找是否有上升需求
				if ( CallDown[j] ||  CallCar[j] ||  CallUp[j]) {
					flag = 1;
					break;
				}
			}
			for (j =  Floor - 1; j >= 0; j--) {//寻找是否有下降
				if ( CallDown[j] ||  CallCar[j] ||  CallUp[j]) {
					flag = 1;
					break;
				}
			}
			if (flag) {//如果找到请求，直接返回
				return;
			}
			gotoxy(3 + 30 *  i, 4);
			printf("                        ");
			gotoxy(3 + 30 *  i, 4);
			printf("无人请求电梯%d",  i);
			return;
		}
		//如果电梯门开着，没有人进出，并且电梯要上升，并且这层楼有人要上升或者电梯有这层楼请求有一个成立
		if (! d1 && ! d3 &&  nextstate == GoingUp && (( CallUp[ Floor] &&  PeopleNum < MaxElevatorPeople) ||  CallCar[ Floor])){
			 d1 = 1;  d2 = 1;
			if ( drcl) {//如果在关门
				 drcl = 0;//关门被打断
				gotoxy(3 + 30 *  i, 4);
				printf("                        ");
				gotoxy(3 + 30 *  i, 4);
				printf("电梯%d被打断关门",  i);
				AddTime(Time -  drcltime, NULL, &Elevator::OpenDoor);
			}
			else AddTime(DoorTime, NULL, &Elevator::OpenDoor);
			return;
		}
		//如果电梯门开着，没有人进出，并且电梯要下降，并且这层楼有人要上升或者电梯有这层楼请求有一个成立
		else if (! d1 && ! d3 &&  nextstate == GoingDown && (( CallDown[ Floor] &&  PeopleNum < MaxElevatorPeople) ||  CallCar[ Floor])){
			 d1 = 1;  d2 = 1;
			if (drcl) {
				 drcl = 0;
				gotoxy(3 + 30 *  i, 4);
				printf("                        ");
				gotoxy(3 + 30 *  i, 4);
				printf("电梯%d被打断关门",  i);

				AddTime(Time -  drcltime, NULL, &Elevator::OpenDoor);
			}
			else AddTime(DoorTime, NULL, &Elevator::OpenDoor);
			return;
		}
		//电梯门开着，又无人进出，电梯为上升状态，但这层楼有上升队列
		else if ( d3 == 1 &&  nextstate == GoingUp && ( CallUp[ Floor] &&  PeopleNum < MaxElevatorPeople)) {
			 d1 = 1;   //使人进入电梯
			 d3 = 0;   //门还没关，又有人进来
			AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
		}
		//电梯门开着，又无人进出，电梯为下降状态，但这层楼有下降队列
		else if ( d3 == 1 &&  nextstate == GoingDown && ( CallDown[ Floor] &&  PeopleNum < MaxElevatorPeople)) {
			 d1 = 1;	  //使人进入电梯
			 d3 = 0;   //门还没关，又有人进来
			AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
		}
		//门关着并且当前层没有需求，且电梯为上升状态
		else if ( d1 == 0 &&  d3 == 0 &&  drcl == 0 &&  nextstate == GoingUp && !(( CallUp[ Floor] &&  PeopleNum < MaxElevatorPeople) ||  CallCar[ Floor])) {
			int temp =  isUp();
			if (temp == 1) {//考虑上层有没有需求
				 state = GoingUp;
				AddTime(AccelerateTime, NULL, &Elevator::IntoNormalSpeed);
			}
			else {//如果没有需求
				 nextstate = Idle;
				 state = Idle;
				 again = 1;
			}
		}
		//门关着并且当前层没有需求，且电梯为下降状态
		else if ( d1 == 0 &&  d3 == 0 &&  drcl == 0 &&  nextstate == GoingDown && !(( CallDown[ Floor] &&  PeopleNum < MaxElevatorPeople) ||  CallCar[ Floor]))
		{
			int temp =  isDown();
			if (temp == 1) {//考虑下层有没有需求
				 state = GoingDown;
				AddTime(AccelerateTime, NULL, &Elevator::IntoNormalSpeed);
			}
			else {//如果没有需求
				 nextstate = Idle;
				 state = Idle;
				 again = 1;
			}
		}
		else if (! d1 && ! d3 &&  drcl == 0 &&  d2) {//如果什么都没有，返回基层
			 d2 = 0;
			 backfirsttime = Time;
			AddTime(BackTime, NULL, &Elevator::GoBaseFloor);
		}
	}
}
//产生新的人1
void Elevator::GeneratePeople(People people) {
	srand(GetTickCount64());//获得随机种子
	cout << "第" << id << "个人进来" << endl;
	//产生一个新的人
	People* p = new People();
	p->EnterTime = Time;
	p->Id = id++;
	p->GiveupTime = MinGiveupTime + rand() % (MaxGiveupTime - MinGiveupTime + 1);
	p->ArriveFloor = rand() % 5;
	p->next = null;
	p->pre = null;
	nexttime = MinNextTime + rand() % (MaxNextTime - MinNextTime + 1);
	while (true) {
		p->TowardsFloor = rand() % 5;
		if (p->ArriveFloor != p->TowardsFloor) break;
	}
	ExistPeople[id - 1] = 1;
	if (p->TowardsFloor > p->ArriveFloor) {//上升
		cout << "进入上升队列" << endl;
		Addupqueue(p, p->ArriveFloor);
		cout << "现在第" << p->ArriveFloor << "层上升队列为：" << endl;
		Showqueue(upqueue, p->ArriveFloor);
		elevator[0].CallUp[p->ArriveFloor] = 1;
		elevator[1].CallUp[p->ArriveFloor] = 1;
	}
	else {								//下降
		cout << "进入下降队列" << endl;
		Adddownque(p, p->ArriveFloor);
		cout << "现在第" << p->ArriveFloor << "层下降队列为：" << endl;
		Showqueue(downque, p->ArriveFloor);
		elevator[0].CallDown[p->ArriveFloor] = 1;
		elevator[1].CallDown[p->ArriveFloor] = 1;
	}
	AddTime(p->GiveupTime, *p, &Elevator::GiveupWait);
	AddTime(nexttime, *p, &Elevator::GeneratePeople);
	Sleep(10000 * UnitTime);
	system("cls");//清屏操作
	ShowOthers();
	PrintElevatorFrame();
	elevator[0].ShowFloor(elevator[0].Floor);
	elevator[1].ShowFloor(elevator[1].Floor);
	//输出乘客排队图形
	int f;
	for (f = 0; f <= MaxFloor; f++) {
		ShowWaitingQueue(f);
	}
	return;
}
//放弃等待2
void Elevator::GiveupWait(People people) {
	if (ExistPeople[people.Id] == 0) {
		return;
	}
	else if (people.ArriveFloor == elevator[0].Floor && elevator[0].state == Idle && elevator[0].PeopleNum < MaxElevatorPeople)
		return;
	else if (people.ArriveFloor == elevator[1].Floor && elevator[1].state == Idle && elevator[1].PeopleNum < MaxElevatorPeople)
		return;
	DisappearWaitingQueue(people.ArriveFloor);
	cout << "第" << people.Id << "个人等待时间过长，离开了" << endl;
	Sleep(1000 * UnitTime);
	ExistPeople[people.Id - 1] = 0;
	People* p = people.pre;
	People* q = p->next;
	p->next = q->next;
	if (q->next != null) {
		q->next->pre = p;
	}
	ShowWaitingQueue(people.ArriveFloor);
	free(q);
}
//打开门3
void Elevator::OpenDoor(People people) {
	//显示部分
	gotoxy(3 + 30 *  i, 4);
	printf("                        ");
	gotoxy(3 + 30 *  i, 4);
	printf("电梯%d门开了",  i);
	if ( peopleinelevator[ Floor].next) {//如果有人的目标是这一层
		AddTime(PeopleTime, NULL, &Elevator::PeopleOut);
	}
	else if ( nextstate == GoingUp && upqueue[ Floor].next != null &&  PeopleNum < MaxElevatorPeople) {//如果有人要进入这一层
		AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
	}
	else if ( nextstate == GoingDown && downque[ Floor].next != null &&  PeopleNum < MaxElevatorPeople) {//如果有人要进入这一层
		AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
	}
	else {//如果没有进也没有出
		if ( PeopleNum < MaxElevatorPeople) {//
			 CallUp[ Floor] = 0;
		}
		 CallDown[ Floor] = 0;
		 CallCar[ Floor] = 0;
		 d1 = 0;  d3 = 1;   //没人进出，但门是开着的
		 testdoortime = Time;
		AddTime(CheckDoorTime, NULL, &Elevator::IntoNormalSpeed);
	}
}
//关门4
void Elevator::CloseDoor(People people) {
	if (drcl == 0 || Time !=   drcltime + DoorTime) return;
	drcl = 0;
	gotoxy(3 + 30 *   i, 4);
	printf("                        ");
	gotoxy(3 + 30 *   i, 4);
	printf("电梯%d门关了",   i);
}
//人进入5
void Elevator::PeopleIn(People people) {
	if (  nextstate == GoingUp) {
		People* p = upqueue[  Floor].next;
		if (p != null &&   PeopleNum < MaxElevatorPeople) {//如果有人要上，并且电梯没有满人
			HiddenFloor(  Floor);//消除电梯
			DisappearWaitingQueue(  Floor);//消除等待的队列
			gotoxy(3 + 30 *   i, 6);
			printf("                           ");
			gotoxy(3 + 30 *   i, 7);
			printf("                           ");
			gotoxy(3 + 30 *   i, 6);
			printf("乘客%d成功进入电梯%d\n", p->Id,   i);

			ExistPeople[p->Id] = 0;
			upqueue[  Floor].next = p->next;
			  AddPeople(p->Id, p->TowardsFloor);//往电梯中加入新人
			  CallCar[p->TowardsFloor] = 1;
			  ShowFloor(  Floor);
			ShowWaitingQueue(  Floor);
		}
		if (upqueue[  Floor].next != null &&   PeopleNum < MaxElevatorPeople)//如果有人要上，并且电梯没有满人
			AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
		else {//如果有条件不满足
			if (upqueue[  Floor].next != null) {//如果有人要上，并且电梯满人
				People* t = upqueue[  Floor].next;
				gotoxy(3 + 30 *   i, 4);
				printf("                  ");
				gotoxy(3 + 30 *   i, 4);
				printf("电梯%d已满载",   i);
				gotoxy(3 + 30 *   i, 6);
				printf("                           ");
				gotoxy(3 + 30 *   i, 7);
				printf("                           ");
				gotoxy(3 + 30 *   i, 6);
	
			}
			if (upqueue[  Floor].next == null) {//如果没有人要进去
				  CallUp[  Floor] = 0;
			}
			  CallCar[  Floor] = 0;
			  d1 = 0;   d3 = 1;  //没人进出，但门是开着的
			  testdoortime = Time;
			  AddTime(CheckDoorTime, NULL, &Elevator::FinishInOut);
		}
	}
	else if (  nextstate == GoingDown &&   PeopleNum < MaxElevatorPeople) {//如果有人要下，并且电梯没有满人
		People* p = downque[  Floor].next;
		if (p != null &&   PeopleNum < MaxElevatorPeople) {//如果有人要下，并且电梯没有满人
			  HiddenFloor(  Floor);//消除电梯
			DisappearWaitingQueue(  Floor);//消除等待的队列
			gotoxy(3 + 30 *   i, 6);
			printf("                           ");
			gotoxy(3 + 30 *   i, 7);
			printf("                           ");
			gotoxy(3 + 30 *   i, 6);
			printf("乘客%d成功进入电梯%d\n", p->Id,   i);
			PeopleNum++;
			ExistPeople[p->Id] = 0;
			downque[  Floor].next = p->next;
			  AddPeople(p->Id, p->TowardsFloor);//往电梯中加入新人
			int i = p->TowardsFloor;
			  CallCar[p->TowardsFloor] = 1;
			  ShowFloor(  Floor);
			ShowWaitingQueue(  Floor);
		}
		if (downque[  Floor].next != null &&   PeopleNum < MaxElevatorPeople)
			AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
		else {
			if (downque[  Floor].next != null) {//如果电梯满了
				People* t = downque[  Floor].next;
				gotoxy(3 + 30 *   i, 4);
				printf("                        ");
				gotoxy(3 + 30 *   i, 4);
				printf("电梯%d已满载",   i);
				
			}
			if (downque[  Floor].next == null) {//如果没有人要进去
				  CallDown[  Floor] = 0;
			}
			  CallCar[  Floor] = 0;
			  d1 = 0;   d3 = 1;  //没人进出，但门是开着的
			  testdoortime = Time;
			AddTime(CheckDoorTime, NULL, &Elevator::FinishInOut);
		}
	}
}
//人出去6
void Elevator::PeopleOut(People people) {
	if (peopleinelevator[Floor].next) {//如果有人在这层楼下
		HiddenFloor(Floor);
		i = DeletePeople(Floor);
		ShowFloor(Floor);

		gotoxy(3 + 30 * i, 6);
		printf("                           ");
		gotoxy(3 + 30 * i, 7);
		printf("                           ");
		gotoxy(3 + 30 * i, 6);
		printf("乘客%d走出电梯%d", i, i);

	}
	if (peopleinelevator[Floor].next) {//如果有人在这层楼下
		AddTime(PeopleTime, NULL, &Elevator::PeopleOut);
	}
	else if (nextstate == GoingUp && upqueue[Floor].next != null) {//如果有人要往上，并且电梯向上
		AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
	}
	else if (nextstate == GoingDown && downque[Floor].next != null) {//如果有人要往下，并且电梯向下
		AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
	}
	else {
		if (nextstate == GoingUp) {//如果电梯往上，并且楼层没有人要向上
			CallUp[Floor] = 0;
			int flag = 0;
			for (int i = Floor + 1; i <= MaxFloor; i++) {//考虑上面楼层有没有请求
				if (CallCar[i] || CallUp[i] || CallDown[i]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {//如果上面楼层没有请求
				if (downque[Floor].next != NULL) {//如果楼层有人下下
					nextstate = GoingDown;//电梯转为向下
					AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
					return;
				}
			}
		}
		else if (nextstate == GoingDown) {//如果电梯往下，并且楼层没有人要向下
			CallDown[Floor] = 0;
			int flag = 0;
			for (int i = Floor - 1; i >= 0; i--) {//考虑下面楼层是否有请求
				if (CallCar[i] || CallUp[i] || CallDown[i]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {//如果没有请求
				if (upqueue[Floor].next != NULL) {//如果当前楼层有向上的请求
					nextstate = GoingUp;//电梯转为向上
					AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
					return;
				}
			}
		}
		//如果都没有
		CallCar[Floor] = 0;
		d1 = 0; d3 = 1;     ////没人进出，但门是开着的
		testdoortime = Time;
		AddTime(CheckDoorTime, NULL, &Elevator::FinishInOut);
	}
}
//结束进出7
void Elevator::FinishInOut(People people) {
	if (  nextstate == GoingUp) {//如果电梯是上升的
		//如果有人可以进出或者时间限制
		if ((upqueue[  Floor].next != null &&   PeopleNum < MaxElevatorPeople) ||   peopleinelevator[  Floor].next ||   testdoortime + CheckDoorTime != Time)
			return;
		//如果没有人要进入
		if (upqueue[  Floor].next == null) {
			  CallUp[  Floor] = 0;
		}
		  CallCar[  Floor] = 0;
		  d1 = 0;   d3 = 0;
		  drcl = 1;   drcltime = Time;
		gotoxy(3 + 30 *   i, 4);
		printf("                        ");
		gotoxy(3 + 30 *   i, 4);
		printf("电梯%d正在关门",   i);
		AddTime(DoorTime, NULL, &Elevator::CloseDoor);//加入关门
	}
	else if (  nextstate == GoingDown) {//如果电梯是下降的
		//如果有人可以进出或者时间限制
		if ((downque[  Floor].next != null &&   PeopleNum < MaxElevatorPeople) ||   peopleinelevator[  Floor].next ||   testdoortime + CheckDoorTime != Time)
			return;
		//如果没有人要进入
		if (downque[  Floor].next == null) {
			  CallDown[  Floor] = 0;
		}
		  CallCar[  Floor] = 0;
		  d1 = 0;   d3 = 0;
		  drcl = 1;   drcltime = Time;
		gotoxy(3 + 30 *   i, 4);
		printf("                        ");
		gotoxy(3 + 30 *   i, 4);
		printf("电梯%d正在关门",   i);
		AddTime(DoorTime, NULL, &Elevator::CloseDoor);
	}
}
//进入正常状态8
void Elevator::IntoNormalSpeed(People people) {
	if (state == GoingUp) {
		gotoxy(3 + 30 * i, 4);
		printf("                        ");
		gotoxy(3 + 30 * i, 4);
		printf("电梯%d已加速上升！", i);
		AddTime(UpTime, NULL, &Elevator::ElevatorUp);
	}
	else {
		gotoxy(3 + 30 * i, 4);
		printf("                        ");
		gotoxy(3 + 30 * i, 4);
		printf("电梯%d已加速下降！", i);
		AddTime(DownTime, NULL, &Elevator::ElevatorDown);
	}
}
//返回基层9
void Elevator::GoBaseFloor(People people) {
	if (state != Idle || backfirsttime + BackTime != Time)//如果不是到了静止的极限，不执行
		return;
	if (Floor == BaseFloor && d2 == 0) {//如果已经在基层并且没有在基层停留300t以上，不执行
		CallCar[BaseFloor] = 2;
		return;
	}
	else if (Floor == BaseFloor)
		return;
	gotoxy(3 + 30 * i, 4);
	printf("                         ");
	gotoxy(3 + 30 * i, 4);
	printf("无人请求电梯%d!将进入%d层", i, BaseFloor);

	CallCar[BaseFloor] = 2;
}
//电梯上升10
void Elevator::ElevatorUp(People people) {
	Floor++;//电梯层数上升
	gotoxy(3 + 30 * i, 4);
	printf("                        ");
	gotoxy(3 + 30 * i, 4);
	printf("电梯%d正在上升", i);
	HiddenFloor(Floor - 1);//隐藏原有电梯
	ShowFloor(Floor);//绘制新电梯
	if (nextstate == GoingUp && (CallUp[Floor] || CallCar[Floor] || CallDown[Floor])) {
		if (CallUp[Floor] || CallCar[Floor])//如果（这层楼有上升队列或者有到这层楼的请求）并且电梯是上升状态
			AddTime(UpSlowdownTime, NULL, &Elevator::ElevatorStop);//电梯减速
		else {
			int flag = 0;
			for (int i = Floor + 1; i <= MaxFloor; i++) {//考察是否有道上面楼层的请求
				if (CallUp[i] || CallCar[i] || CallDown[i]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {//如果没有，选择停在当前楼层，减速
				AddTime(UpSlowdownTime, NULL ,&Elevator::ElevatorStop);
			}
			else {//如果有，电梯继续上升
				AddTime(UpTime, NULL, &Elevator::ElevatorUp);
			}
		}
	}
	//同理考虑下降形式
	else if (nextstate == GoingDown && (CallUp[Floor] || CallCar[Floor] || CallDown[Floor])) {
		if (CallDown[Floor] || CallCar[Floor])
			AddTime(UpSlowdownTime, NULL, &Elevator::ElevatorStop);
		else {
			int flag = 0;
			for (int i = Floor - 1; i >= 0; i--) {
				if (CallUp[i] || CallCar[i] || CallDown[i]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0)AddTime(UpSlowdownTime, NULL, &Elevator::ElevatorStop);
			else {
				AddTime(UpTime, NULL, &Elevator::ElevatorUp);
			}
		}
	}
	else {
		if (Floor == MaxFloor) {//如果已经到了顶层，不能再上升了
			AddTime(UpSlowdownTime, NULL, &Elevator::ElevatorStop);
		}
		else {
			AddTime(UpTime, NULL, &Elevator::ElevatorUp);
		}
	}
}
//电梯下降11
void Elevator::ElevatorDown(People people) {
	Floor--;
	gotoxy(3 + 30 * i, 4);
	printf("                        ");
	gotoxy(3 + 30 * i, 4);
	printf("电梯%d正在下降", i);
	HiddenFloor(Floor);
	ShowFloor(Floor);
	if (nextstate == GoingUp && (CallUp[Floor] || CallCar[Floor])) {
		AddTime(UpSlowdownTime, NULL, &Elevator::ElevatorStop);
	}
	else if (nextstate == GoingDown && (CallDown[Floor] || CallCar[Floor])) {
		AddTime(UpSlowdownTime, NULL, &Elevator::ElevatorStop);
	}
	else {
		if (Floor == 0) {
			AddTime(DownSlowdownTime, NULL, &Elevator::ElevatorStop);
		}
		else {
			AddTime(DownTime, NULL, &Elevator::ElevatorDown);
		}
	}
}
//电梯暂停12
void Elevator::ElevatorStop(People people) {
	state = Idle;
	d1 = 0; d3 = 0;
	gotoxy(3 + 30 * i, 4);
	printf("                        ");
	gotoxy(3 + 30 * i, 4);
	printf("电梯%d停了，当前层是%d", i, Floor);

}
//在第floor层显示电梯
void Elevator::ShowFloor(int floor) {
	gotoxy(LeftElevatorX + i * 27, LeftElevatorY - 1 + (4 - floor) * FloorHight);
	printf("[");
	//输出电梯上的人,不知道是否可行
	int j = 0;
	for (j = 0; j <= MaxFloor; j++) {
		struct PeopleInElevator* p = &peopleinelevator[j];
		while (p) {
			if (p->id > 0) {
				printf(" %d", p->id);
			}

			p = p->next;
		}
	}
	gotoxy(LeftElevatorX + i * 26 + 24, LeftElevatorY - 1 + (4 - floor) * FloorHight);
	printf("]");
}
//在第floor层隐藏电梯
void Elevator::HiddenFloor(int floor) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);//字体颜色为黑色
	int j;
	for (j = LeftElevatorX + i * 27; j <= LeftElevatorX + i * 26 + 24; j++)
	{
		gotoxy(j, LeftElevatorY - 1 + (4 - floor) * FloorHight);
		printf(" ");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
//获取下一步的状态
int Elevator::GetNextState() {
	int timeUp = -1, timeDown = -1;
	//当前层是否有向上请求
	if (upqueue[Floor].next != null && CallUp[Floor]) {
		timeUp = upqueue[Floor].next->EnterTime;
	}
	//当前层是否有向下请求
	if (downque[Floor].next != null && CallDown[Floor]) {
		timeDown = downque[Floor].next->EnterTime;
	}

	if (timeUp != -1 || timeDown != -1) {//如果有请求
		if (timeUp != -1 && timeDown != -1) {//如果同时有向上向下请求，则选择时间较小的那个
			return timeUp < timeDown ? GoingUp : GoingDown;
		}
		else if (timeUp != -1) {//如果只有向上，则电梯向上
			return GoingUp;
		}
		else if (timeDown != -1) {//如果只有向下，则电梯向下
			return GoingDown;
		}
	}
	else {//如果当前楼层没有请求
		timeUp = maxtime + 1; timeDown = maxtime + 1;//先将时间置为程序最大允许时间
		for (int j = Floor + 1; j <= MaxFloor; j++) {//从上面楼层中选取时间最小的请求
			if (upqueue[j].next != null && CallUp[j]) {
				timeUp = mymin(timeUp, upqueue[j].next->EnterTime);
			}
			if (downque[j].next != null && CallDown[j]) {
				timeUp = mymin(timeUp, downque[j].next->EnterTime);
			}
		}
		for (int j = Floor - 1; j >= 0; j--) {//从下面楼层中选择时间最小的请求
			if (upqueue[j].next != null && CallUp[j]) {
				timeDown = mymin(timeDown, upqueue[j].next->EnterTime);
			}
			if (downque[j].next != null && CallDown[j]) {
				timeDown = mymin(timeDown, downque[j].next->EnterTime);
			}
		}
		if (timeUp != maxtime + 1 || timeDown != maxtime + 1) {//如果上面或者下面有能在程序运行最大时间内的请求
			if (timeUp != maxtime + 1 && timeDown != maxtime + 1) {//如果上下楼层都有，选择最小的那个
				return timeUp < timeDown ? GoingUp : GoingDown;
			}
			else if (timeUp != maxtime + 1) {//如果只有上面楼层，则电梯向上
				return GoingUp;
			}
			else return GoingDown;//如果只有下面楼层，则电梯向下
		}
		else {//如果本层没有请求，上下楼也没有请求
			if (CallCar[BaseFloor] == 2 && BaseFloor > Floor)return GoingUp;//如果基层的电梯请求有两辆，并且楼层在基层下方，就向上
			else if (CallCar[BaseFloor] == 2 && BaseFloor < Floor)return GoingDown;//如果基层的电梯请求有两辆，并且楼层在基层上方，就向下
			return Idle;
		}
	}
}
//往电梯里增加新人
void Elevator::AddPeople(int newid, int newfloor) {
	PeopleInElevator* newNode = (PeopleInElevator*)malloc(sizeof(PeopleInElevator));
	newNode->id = newid;
	newNode->next = peopleinelevator[newfloor].next;
	peopleinelevator[newfloor].next = newNode;
	PeopleNum++;
	return;
}
//删除电梯里的人
int Elevator::DeletePeople(int newfloor) {
	int returnid;
	PeopleInElevator* p = peopleinelevator[newfloor].next;
	peopleinelevator[newfloor].next = p->next;
	PeopleNum--;
	returnid = p->id;
	free(p);
	return returnid;
}
//电梯是否上升
int Elevator::isUp() {
	for (int j = Floor + 1; j <= MaxFloor; j++) {
		if (CallCar[j] || CallUp[j] || CallDown[j])
			return 1;
	}
	return 0;
}
//电梯是否下降
int Elevator::isDown() {
	for (int j = Floor - 1; j >= 0; j--) {
		if (CallCar[j] || CallUp[j] || CallDown[j])
			return 1;
	}
	return 0;
}