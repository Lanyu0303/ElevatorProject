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
}*TimeP;//ʱ����ָ��
extern TimeNode TimeInElevator[2]; //�����������ʱ�����
extern void Addupqueue(People* people, int floor);//������������
extern void Adddownque(People* people, int floor);//�����½�����
extern void Showqueue(People* people, int floor);//չʾ����,��һ��Ϊupqueue��downque
extern void ShowWaitingQueue(int floor);//չʾfloor��ĵȴ�����
extern void PrintElevatorFrame();//������ݿ��
extern void ShowOthers();//���������Ϣ
extern void DisappearWaitingQueue(int floor);//�������ڵȴ��Ķ���
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
//����ʱ��ڵ�
void Elevator::AddTime(int newtime, People people, void(Elevator::*newf)(People people)) {
	//����һ���µ�ʱ��ڵ�
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
	//������ڵ���뵽���ݵ�ʱ������
	TimeP p = &TimeInElevator[i];
	while (p->next != NULL) {
		if (p->next->limittime > newNode->limittime) break;
		p = p->next;
	}
	newNode->next = p->next;
	p->next = newNode;
}
//���ƺ���
void Elevator::Controler() {
	int flag, j;
	if ( state == Idle) {//����������ھ�ֹ
		if ( nextstate == Idle) {//�����һ�������Ǿ�ֹ�������»����һ����
			 nextstate =  GetNextState();
		}
		if ( d1 == 0 &&  d3 == 0 &&  CallCar[ Floor] == 2) {//����ſ��ţ���û���˽�������,�ҵ���������������
			 CallCar[ Floor] = 0;//����������
			//���Ը�д
			flag = 0;
			for (j =  Floor + 1; j <= MaxFloor; j++) {//Ѱ���Ƿ�����������
				if ( CallDown[j] ||  CallCar[j] ||  CallUp[j]) {
					flag = 1;
					break;
				}
			}
			for (j =  Floor - 1; j >= 0; j--) {//Ѱ���Ƿ����½�
				if ( CallDown[j] ||  CallCar[j] ||  CallUp[j]) {
					flag = 1;
					break;
				}
			}
			if (flag) {//����ҵ�����ֱ�ӷ���
				return;
			}
			gotoxy(3 + 30 *  i, 4);
			printf("                        ");
			gotoxy(3 + 30 *  i, 4);
			printf("�����������%d",  i);
			return;
		}
		//��������ſ��ţ�û���˽��������ҵ���Ҫ�������������¥����Ҫ�������ߵ��������¥������һ������
		if (! d1 && ! d3 &&  nextstate == GoingUp && (( CallUp[ Floor] &&  PeopleNum < MaxElevatorPeople) ||  CallCar[ Floor])){
			 d1 = 1;  d2 = 1;
			if ( drcl) {//����ڹ���
				 drcl = 0;//���ű����
				gotoxy(3 + 30 *  i, 4);
				printf("                        ");
				gotoxy(3 + 30 *  i, 4);
				printf("����%d����Ϲ���",  i);
				AddTime(Time -  drcltime, NULL, &Elevator::OpenDoor);
			}
			else AddTime(DoorTime, NULL, &Elevator::OpenDoor);
			return;
		}
		//��������ſ��ţ�û���˽��������ҵ���Ҫ�½����������¥����Ҫ�������ߵ��������¥������һ������
		else if (! d1 && ! d3 &&  nextstate == GoingDown && (( CallDown[ Floor] &&  PeopleNum < MaxElevatorPeople) ||  CallCar[ Floor])){
			 d1 = 1;  d2 = 1;
			if (drcl) {
				 drcl = 0;
				gotoxy(3 + 30 *  i, 4);
				printf("                        ");
				gotoxy(3 + 30 *  i, 4);
				printf("����%d����Ϲ���",  i);

				AddTime(Time -  drcltime, NULL, &Elevator::OpenDoor);
			}
			else AddTime(DoorTime, NULL, &Elevator::OpenDoor);
			return;
		}
		//�����ſ��ţ������˽���������Ϊ����״̬�������¥����������
		else if ( d3 == 1 &&  nextstate == GoingUp && ( CallUp[ Floor] &&  PeopleNum < MaxElevatorPeople)) {
			 d1 = 1;   //ʹ�˽������
			 d3 = 0;   //�Ż�û�أ������˽���
			AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
		}
		//�����ſ��ţ������˽���������Ϊ�½�״̬�������¥���½�����
		else if ( d3 == 1 &&  nextstate == GoingDown && ( CallDown[ Floor] &&  PeopleNum < MaxElevatorPeople)) {
			 d1 = 1;	  //ʹ�˽������
			 d3 = 0;   //�Ż�û�أ������˽���
			AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
		}
		//�Ź��Ų��ҵ�ǰ��û�������ҵ���Ϊ����״̬
		else if ( d1 == 0 &&  d3 == 0 &&  drcl == 0 &&  nextstate == GoingUp && !(( CallUp[ Floor] &&  PeopleNum < MaxElevatorPeople) ||  CallCar[ Floor])) {
			int temp =  isUp();
			if (temp == 1) {//�����ϲ���û������
				 state = GoingUp;
				AddTime(AccelerateTime, NULL, &Elevator::IntoNormalSpeed);
			}
			else {//���û������
				 nextstate = Idle;
				 state = Idle;
				 again = 1;
			}
		}
		//�Ź��Ų��ҵ�ǰ��û�������ҵ���Ϊ�½�״̬
		else if ( d1 == 0 &&  d3 == 0 &&  drcl == 0 &&  nextstate == GoingDown && !(( CallDown[ Floor] &&  PeopleNum < MaxElevatorPeople) ||  CallCar[ Floor]))
		{
			int temp =  isDown();
			if (temp == 1) {//�����²���û������
				 state = GoingDown;
				AddTime(AccelerateTime, NULL, &Elevator::IntoNormalSpeed);
			}
			else {//���û������
				 nextstate = Idle;
				 state = Idle;
				 again = 1;
			}
		}
		else if (! d1 && ! d3 &&  drcl == 0 &&  d2) {//���ʲô��û�У����ػ���
			 d2 = 0;
			 backfirsttime = Time;
			AddTime(BackTime, NULL, &Elevator::GoBaseFloor);
		}
	}
}
//�����µ���1
void Elevator::GeneratePeople(People people) {
	srand(GetTickCount64());//����������
	cout << "��" << id << "���˽���" << endl;
	//����һ���µ���
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
	if (p->TowardsFloor > p->ArriveFloor) {//����
		cout << "������������" << endl;
		Addupqueue(p, p->ArriveFloor);
		cout << "���ڵ�" << p->ArriveFloor << "����������Ϊ��" << endl;
		Showqueue(upqueue, p->ArriveFloor);
		elevator[0].CallUp[p->ArriveFloor] = 1;
		elevator[1].CallUp[p->ArriveFloor] = 1;
	}
	else {								//�½�
		cout << "�����½�����" << endl;
		Adddownque(p, p->ArriveFloor);
		cout << "���ڵ�" << p->ArriveFloor << "���½�����Ϊ��" << endl;
		Showqueue(downque, p->ArriveFloor);
		elevator[0].CallDown[p->ArriveFloor] = 1;
		elevator[1].CallDown[p->ArriveFloor] = 1;
	}
	AddTime(p->GiveupTime, *p, &Elevator::GiveupWait);
	AddTime(nexttime, *p, &Elevator::GeneratePeople);
	Sleep(10000 * UnitTime);
	system("cls");//��������
	ShowOthers();
	PrintElevatorFrame();
	elevator[0].ShowFloor(elevator[0].Floor);
	elevator[1].ShowFloor(elevator[1].Floor);
	//����˿��Ŷ�ͼ��
	int f;
	for (f = 0; f <= MaxFloor; f++) {
		ShowWaitingQueue(f);
	}
	return;
}
//�����ȴ�2
void Elevator::GiveupWait(People people) {
	if (ExistPeople[people.Id] == 0) {
		return;
	}
	else if (people.ArriveFloor == elevator[0].Floor && elevator[0].state == Idle && elevator[0].PeopleNum < MaxElevatorPeople)
		return;
	else if (people.ArriveFloor == elevator[1].Floor && elevator[1].state == Idle && elevator[1].PeopleNum < MaxElevatorPeople)
		return;
	DisappearWaitingQueue(people.ArriveFloor);
	cout << "��" << people.Id << "���˵ȴ�ʱ��������뿪��" << endl;
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
//����3
void Elevator::OpenDoor(People people) {
	//��ʾ����
	gotoxy(3 + 30 *  i, 4);
	printf("                        ");
	gotoxy(3 + 30 *  i, 4);
	printf("����%d�ſ���",  i);
	if ( peopleinelevator[ Floor].next) {//������˵�Ŀ������һ��
		AddTime(PeopleTime, NULL, &Elevator::PeopleOut);
	}
	else if ( nextstate == GoingUp && upqueue[ Floor].next != null &&  PeopleNum < MaxElevatorPeople) {//�������Ҫ������һ��
		AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
	}
	else if ( nextstate == GoingDown && downque[ Floor].next != null &&  PeopleNum < MaxElevatorPeople) {//�������Ҫ������һ��
		AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
	}
	else {//���û�н�Ҳû�г�
		if ( PeopleNum < MaxElevatorPeople) {//
			 CallUp[ Floor] = 0;
		}
		 CallDown[ Floor] = 0;
		 CallCar[ Floor] = 0;
		 d1 = 0;  d3 = 1;   //û�˽����������ǿ��ŵ�
		 testdoortime = Time;
		AddTime(CheckDoorTime, NULL, &Elevator::IntoNormalSpeed);
	}
}
//����4
void Elevator::CloseDoor(People people) {
	if (drcl == 0 || Time !=   drcltime + DoorTime) return;
	drcl = 0;
	gotoxy(3 + 30 *   i, 4);
	printf("                        ");
	gotoxy(3 + 30 *   i, 4);
	printf("����%d�Ź���",   i);
}
//�˽���5
void Elevator::PeopleIn(People people) {
	if (  nextstate == GoingUp) {
		People* p = upqueue[  Floor].next;
		if (p != null &&   PeopleNum < MaxElevatorPeople) {//�������Ҫ�ϣ����ҵ���û������
			HiddenFloor(  Floor);//��������
			DisappearWaitingQueue(  Floor);//�����ȴ��Ķ���
			gotoxy(3 + 30 *   i, 6);
			printf("                           ");
			gotoxy(3 + 30 *   i, 7);
			printf("                           ");
			gotoxy(3 + 30 *   i, 6);
			printf("�˿�%d�ɹ��������%d\n", p->Id,   i);

			ExistPeople[p->Id] = 0;
			upqueue[  Floor].next = p->next;
			  AddPeople(p->Id, p->TowardsFloor);//�������м�������
			  CallCar[p->TowardsFloor] = 1;
			  ShowFloor(  Floor);
			ShowWaitingQueue(  Floor);
		}
		if (upqueue[  Floor].next != null &&   PeopleNum < MaxElevatorPeople)//�������Ҫ�ϣ����ҵ���û������
			AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
		else {//���������������
			if (upqueue[  Floor].next != null) {//�������Ҫ�ϣ����ҵ�������
				People* t = upqueue[  Floor].next;
				gotoxy(3 + 30 *   i, 4);
				printf("                  ");
				gotoxy(3 + 30 *   i, 4);
				printf("����%d������",   i);
				gotoxy(3 + 30 *   i, 6);
				printf("                           ");
				gotoxy(3 + 30 *   i, 7);
				printf("                           ");
				gotoxy(3 + 30 *   i, 6);
	
			}
			if (upqueue[  Floor].next == null) {//���û����Ҫ��ȥ
				  CallUp[  Floor] = 0;
			}
			  CallCar[  Floor] = 0;
			  d1 = 0;   d3 = 1;  //û�˽����������ǿ��ŵ�
			  testdoortime = Time;
			  AddTime(CheckDoorTime, NULL, &Elevator::FinishInOut);
		}
	}
	else if (  nextstate == GoingDown &&   PeopleNum < MaxElevatorPeople) {//�������Ҫ�£����ҵ���û������
		People* p = downque[  Floor].next;
		if (p != null &&   PeopleNum < MaxElevatorPeople) {//�������Ҫ�£����ҵ���û������
			  HiddenFloor(  Floor);//��������
			DisappearWaitingQueue(  Floor);//�����ȴ��Ķ���
			gotoxy(3 + 30 *   i, 6);
			printf("                           ");
			gotoxy(3 + 30 *   i, 7);
			printf("                           ");
			gotoxy(3 + 30 *   i, 6);
			printf("�˿�%d�ɹ��������%d\n", p->Id,   i);
			PeopleNum++;
			ExistPeople[p->Id] = 0;
			downque[  Floor].next = p->next;
			  AddPeople(p->Id, p->TowardsFloor);//�������м�������
			int i = p->TowardsFloor;
			  CallCar[p->TowardsFloor] = 1;
			  ShowFloor(  Floor);
			ShowWaitingQueue(  Floor);
		}
		if (downque[  Floor].next != null &&   PeopleNum < MaxElevatorPeople)
			AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
		else {
			if (downque[  Floor].next != null) {//�����������
				People* t = downque[  Floor].next;
				gotoxy(3 + 30 *   i, 4);
				printf("                        ");
				gotoxy(3 + 30 *   i, 4);
				printf("����%d������",   i);
				
			}
			if (downque[  Floor].next == null) {//���û����Ҫ��ȥ
				  CallDown[  Floor] = 0;
			}
			  CallCar[  Floor] = 0;
			  d1 = 0;   d3 = 1;  //û�˽����������ǿ��ŵ�
			  testdoortime = Time;
			AddTime(CheckDoorTime, NULL, &Elevator::FinishInOut);
		}
	}
}
//�˳�ȥ6
void Elevator::PeopleOut(People people) {
	if (peopleinelevator[Floor].next) {//������������¥��
		HiddenFloor(Floor);
		i = DeletePeople(Floor);
		ShowFloor(Floor);

		gotoxy(3 + 30 * i, 6);
		printf("                           ");
		gotoxy(3 + 30 * i, 7);
		printf("                           ");
		gotoxy(3 + 30 * i, 6);
		printf("�˿�%d�߳�����%d", i, i);

	}
	if (peopleinelevator[Floor].next) {//������������¥��
		AddTime(PeopleTime, NULL, &Elevator::PeopleOut);
	}
	else if (nextstate == GoingUp && upqueue[Floor].next != null) {//�������Ҫ���ϣ����ҵ�������
		AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
	}
	else if (nextstate == GoingDown && downque[Floor].next != null) {//�������Ҫ���£����ҵ�������
		AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
	}
	else {
		if (nextstate == GoingUp) {//����������ϣ�����¥��û����Ҫ����
			CallUp[Floor] = 0;
			int flag = 0;
			for (int i = Floor + 1; i <= MaxFloor; i++) {//��������¥����û������
				if (CallCar[i] || CallUp[i] || CallDown[i]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {//�������¥��û������
				if (downque[Floor].next != NULL) {//���¥����������
					nextstate = GoingDown;//����תΪ����
					AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
					return;
				}
			}
		}
		else if (nextstate == GoingDown) {//����������£�����¥��û����Ҫ����
			CallDown[Floor] = 0;
			int flag = 0;
			for (int i = Floor - 1; i >= 0; i--) {//��������¥���Ƿ�������
				if (CallCar[i] || CallUp[i] || CallDown[i]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {//���û������
				if (upqueue[Floor].next != NULL) {//�����ǰ¥�������ϵ�����
					nextstate = GoingUp;//����תΪ����
					AddTime(PeopleTime, NULL, &Elevator::PeopleIn);
					return;
				}
			}
		}
		//�����û��
		CallCar[Floor] = 0;
		d1 = 0; d3 = 1;     ////û�˽����������ǿ��ŵ�
		testdoortime = Time;
		AddTime(CheckDoorTime, NULL, &Elevator::FinishInOut);
	}
}
//��������7
void Elevator::FinishInOut(People people) {
	if (  nextstate == GoingUp) {//���������������
		//������˿��Խ�������ʱ������
		if ((upqueue[  Floor].next != null &&   PeopleNum < MaxElevatorPeople) ||   peopleinelevator[  Floor].next ||   testdoortime + CheckDoorTime != Time)
			return;
		//���û����Ҫ����
		if (upqueue[  Floor].next == null) {
			  CallUp[  Floor] = 0;
		}
		  CallCar[  Floor] = 0;
		  d1 = 0;   d3 = 0;
		  drcl = 1;   drcltime = Time;
		gotoxy(3 + 30 *   i, 4);
		printf("                        ");
		gotoxy(3 + 30 *   i, 4);
		printf("����%d���ڹ���",   i);
		AddTime(DoorTime, NULL, &Elevator::CloseDoor);//�������
	}
	else if (  nextstate == GoingDown) {//����������½���
		//������˿��Խ�������ʱ������
		if ((downque[  Floor].next != null &&   PeopleNum < MaxElevatorPeople) ||   peopleinelevator[  Floor].next ||   testdoortime + CheckDoorTime != Time)
			return;
		//���û����Ҫ����
		if (downque[  Floor].next == null) {
			  CallDown[  Floor] = 0;
		}
		  CallCar[  Floor] = 0;
		  d1 = 0;   d3 = 0;
		  drcl = 1;   drcltime = Time;
		gotoxy(3 + 30 *   i, 4);
		printf("                        ");
		gotoxy(3 + 30 *   i, 4);
		printf("����%d���ڹ���",   i);
		AddTime(DoorTime, NULL, &Elevator::CloseDoor);
	}
}
//��������״̬8
void Elevator::IntoNormalSpeed(People people) {
	if (state == GoingUp) {
		gotoxy(3 + 30 * i, 4);
		printf("                        ");
		gotoxy(3 + 30 * i, 4);
		printf("����%d�Ѽ���������", i);
		AddTime(UpTime, NULL, &Elevator::ElevatorUp);
	}
	else {
		gotoxy(3 + 30 * i, 4);
		printf("                        ");
		gotoxy(3 + 30 * i, 4);
		printf("����%d�Ѽ����½���", i);
		AddTime(DownTime, NULL, &Elevator::ElevatorDown);
	}
}
//���ػ���9
void Elevator::GoBaseFloor(People people) {
	if (state != Idle || backfirsttime + BackTime != Time)//������ǵ��˾�ֹ�ļ��ޣ���ִ��
		return;
	if (Floor == BaseFloor && d2 == 0) {//����Ѿ��ڻ��㲢��û���ڻ���ͣ��300t���ϣ���ִ��
		CallCar[BaseFloor] = 2;
		return;
	}
	else if (Floor == BaseFloor)
		return;
	gotoxy(3 + 30 * i, 4);
	printf("                         ");
	gotoxy(3 + 30 * i, 4);
	printf("�����������%d!������%d��", i, BaseFloor);

	CallCar[BaseFloor] = 2;
}
//��������10
void Elevator::ElevatorUp(People people) {
	Floor++;//���ݲ�������
	gotoxy(3 + 30 * i, 4);
	printf("                        ");
	gotoxy(3 + 30 * i, 4);
	printf("����%d��������", i);
	HiddenFloor(Floor - 1);//����ԭ�е���
	ShowFloor(Floor);//�����µ���
	if (nextstate == GoingUp && (CallUp[Floor] || CallCar[Floor] || CallDown[Floor])) {
		if (CallUp[Floor] || CallCar[Floor])//��������¥���������л����е����¥�����󣩲��ҵ���������״̬
			AddTime(UpSlowdownTime, NULL, &Elevator::ElevatorStop);//���ݼ���
		else {
			int flag = 0;
			for (int i = Floor + 1; i <= MaxFloor; i++) {//�����Ƿ��е�����¥�������
				if (CallUp[i] || CallCar[i] || CallDown[i]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {//���û�У�ѡ��ͣ�ڵ�ǰ¥�㣬����
				AddTime(UpSlowdownTime, NULL ,&Elevator::ElevatorStop);
			}
			else {//����У����ݼ�������
				AddTime(UpTime, NULL, &Elevator::ElevatorUp);
			}
		}
	}
	//ͬ�����½���ʽ
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
		if (Floor == MaxFloor) {//����Ѿ����˶��㣬������������
			AddTime(UpSlowdownTime, NULL, &Elevator::ElevatorStop);
		}
		else {
			AddTime(UpTime, NULL, &Elevator::ElevatorUp);
		}
	}
}
//�����½�11
void Elevator::ElevatorDown(People people) {
	Floor--;
	gotoxy(3 + 30 * i, 4);
	printf("                        ");
	gotoxy(3 + 30 * i, 4);
	printf("����%d�����½�", i);
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
//������ͣ12
void Elevator::ElevatorStop(People people) {
	state = Idle;
	d1 = 0; d3 = 0;
	gotoxy(3 + 30 * i, 4);
	printf("                        ");
	gotoxy(3 + 30 * i, 4);
	printf("����%dͣ�ˣ���ǰ����%d", i, Floor);

}
//�ڵ�floor����ʾ����
void Elevator::ShowFloor(int floor) {
	gotoxy(LeftElevatorX + i * 27, LeftElevatorY - 1 + (4 - floor) * FloorHight);
	printf("[");
	//��������ϵ���,��֪���Ƿ����
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
//�ڵ�floor�����ص���
void Elevator::HiddenFloor(int floor) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);//������ɫΪ��ɫ
	int j;
	for (j = LeftElevatorX + i * 27; j <= LeftElevatorX + i * 26 + 24; j++)
	{
		gotoxy(j, LeftElevatorY - 1 + (4 - floor) * FloorHight);
		printf(" ");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
//��ȡ��һ����״̬
int Elevator::GetNextState() {
	int timeUp = -1, timeDown = -1;
	//��ǰ���Ƿ�����������
	if (upqueue[Floor].next != null && CallUp[Floor]) {
		timeUp = upqueue[Floor].next->EnterTime;
	}
	//��ǰ���Ƿ�����������
	if (downque[Floor].next != null && CallDown[Floor]) {
		timeDown = downque[Floor].next->EnterTime;
	}

	if (timeUp != -1 || timeDown != -1) {//���������
		if (timeUp != -1 && timeDown != -1) {//���ͬʱ����������������ѡ��ʱ���С���Ǹ�
			return timeUp < timeDown ? GoingUp : GoingDown;
		}
		else if (timeUp != -1) {//���ֻ�����ϣ����������
			return GoingUp;
		}
		else if (timeDown != -1) {//���ֻ�����£����������
			return GoingDown;
		}
	}
	else {//�����ǰ¥��û������
		timeUp = maxtime + 1; timeDown = maxtime + 1;//�Ƚ�ʱ����Ϊ�����������ʱ��
		for (int j = Floor + 1; j <= MaxFloor; j++) {//������¥����ѡȡʱ����С������
			if (upqueue[j].next != null && CallUp[j]) {
				timeUp = mymin(timeUp, upqueue[j].next->EnterTime);
			}
			if (downque[j].next != null && CallDown[j]) {
				timeUp = mymin(timeUp, downque[j].next->EnterTime);
			}
		}
		for (int j = Floor - 1; j >= 0; j--) {//������¥����ѡ��ʱ����С������
			if (upqueue[j].next != null && CallUp[j]) {
				timeDown = mymin(timeDown, upqueue[j].next->EnterTime);
			}
			if (downque[j].next != null && CallDown[j]) {
				timeDown = mymin(timeDown, downque[j].next->EnterTime);
			}
		}
		if (timeUp != maxtime + 1 || timeDown != maxtime + 1) {//�������������������ڳ����������ʱ���ڵ�����
			if (timeUp != maxtime + 1 && timeDown != maxtime + 1) {//�������¥�㶼�У�ѡ����С���Ǹ�
				return timeUp < timeDown ? GoingUp : GoingDown;
			}
			else if (timeUp != maxtime + 1) {//���ֻ������¥�㣬���������
				return GoingUp;
			}
			else return GoingDown;//���ֻ������¥�㣬���������
		}
		else {//�������û����������¥Ҳû������
			if (CallCar[BaseFloor] == 2 && BaseFloor > Floor)return GoingUp;//�������ĵ�������������������¥���ڻ����·���������
			else if (CallCar[BaseFloor] == 2 && BaseFloor < Floor)return GoingDown;//�������ĵ�������������������¥���ڻ����Ϸ���������
			return Idle;
		}
	}
}
//����������������
void Elevator::AddPeople(int newid, int newfloor) {
	PeopleInElevator* newNode = (PeopleInElevator*)malloc(sizeof(PeopleInElevator));
	newNode->id = newid;
	newNode->next = peopleinelevator[newfloor].next;
	peopleinelevator[newfloor].next = newNode;
	PeopleNum++;
	return;
}
//ɾ�����������
int Elevator::DeletePeople(int newfloor) {
	int returnid;
	PeopleInElevator* p = peopleinelevator[newfloor].next;
	peopleinelevator[newfloor].next = p->next;
	PeopleNum--;
	returnid = p->id;
	free(p);
	return returnid;
}
//�����Ƿ�����
int Elevator::isUp() {
	for (int j = Floor + 1; j <= MaxFloor; j++) {
		if (CallCar[j] || CallUp[j] || CallDown[j])
			return 1;
	}
	return 0;
}
//�����Ƿ��½�
int Elevator::isDown() {
	for (int j = Floor - 1; j >= 0; j--) {
		if (CallCar[j] || CallUp[j] || CallDown[j])
			return 1;
	}
	return 0;
}