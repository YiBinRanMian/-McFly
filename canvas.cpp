
#include "stdafx.h"
#include <stdio.h>
#include <time.h>
#include <io.h>
#include <ctype.h>
#include <stdlib.h>
#include "mmsystem.h"//��������ͷ�ļ�
#pragma comment(lib,"winmm.lib")//��������ͷ�ļ���

struct BULLET
{
	bool exist;
};

struct GOLD
{
	bool exist;
};

HINSTANCE hInst;
HBITMAP bg,fly,cannon,over,begin1,begin2,begin3,begin4,drop,eject,die,bullet,bat,boom,fire,glide,gold,printd,process1,process2,plane,bomb,heart;
HDC		hdc,mdc,Background,Fly,Cannon,Droping,Eject,Die,Bullet,Bat,Fire,Glide,Gold,Printd,Process1,Process2,Plane,Bomb,Heart;
HWND	hWnd;
DWORD	tPre,tNow;

float	x=0;//x��ʼ����
float   y=9450;//y��ʼ����
float   vx=10;//vx��ʼ�ٶ�
float   vy=100;//vy��ʼ�ٶ�
float	gy=-1;//ģ������
float	ay=40;//����y����ٶ�
int		man_life=2;//��������ֵ
int     bex=0,bey=0;//������������
int		Bat_x=0;//���������
int     Plane_x=0;//�ɻ�������
int Bat_y=0;//����������
int		bomb_x=0,bomb_y=0;//ը����������
int		i;//�ӵ����
int     j=0;//������
int		p;//�������
int		bcount=0;//�ӵ�����
int		judge=2;//��ʼ2������0����Ϸ1�����������л�-1
int     judge2=1;//��ʼ�����ж���ͣλ��
int		num=0;//���С����䡢�����ӵ������䶯��
int		num_cannon=0;//����&&�Ƕ��ж�
int		num_bat=0;//����
int		num_boom=0;//��ը
int		num_gold=0;//���
int		beginx,beginy;//��ʼ�����������
int		vbeginx,vbeginy;//�����ͣ����
int		sway=1;//���ڰڶ�
int		flag1=0;/*��ֵΪ1��������*/
int		flag2=1;/*��ֵΪ1���Ըı��ʼ�ٶ�*/
int		flag3=0;/*��ֵΪ1�������䡢�����ӵ�*/
int		flag4=1;/*��ֵΪ1��������Bat_y�����*/
int		flag5=1;/*��ֵΪ1��ʾ�������*/
int		flag6=0;/*��ֵΪ1���Բ�����Ч*/
int		flag7=0;/*��ֵΪ1���Է����ӵ�*/
int		flag8=0;//��ֵΪ1���Ի���
int		flag9=0;//��ֵΪ1��ʾ�ڿ�������Ϊ0��ʾ��������2��ʾ��ը������
int		flag10[3]={0,0,0};//����Ƿ񱻳�
int		flag11=1;//����bgm
int     flag12=1;//���Ʒɻ�x����
int		flag13=0;//����ը��
int		flag14=0;//����ը���ͽ�ҽ������
int     flag15=0;//�������а���
int		flag16=0;//�����߷����ж��Rank�����
int		distance=0;//��¼���о���
int		bestscore=0;//��߷�
float	power=100;//��¼ʣ������
float   proc;//�������ٷ���
int		vbullet_x[8],vbullet_y[8];//�ӵ�x��y����ٶ�
int     vgold_x[3],vgold_y[3];//���x��y���ٶ�
int		bat_time=0;//������������ʱ��
int		plane_time=0;//���Ʒɻ�����ʱ��
int		gametime=0;//��Ϸ��ʼʱ�����
int     bat_life=3;//��������
int		n[5];//��ʾ����
int     h;//�������λ��
bool    isover;//�ж���Ϸ����
BULLET	b[8];  //�ӵ�
GOLD	golds[3];//���
char    buf[50];//��Ч���

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void				handleKey();
void				Main(HDC hdc);
void				main_eject(HDC hdc);
void				main_fire(HDC hdc);
void				main_bullet(HDC hdc);
void				main_fly(HDC hdc);
void				main_drop(HDC hdc);
void				main_die(HDC hdc);
void				main_bat(HDC hdc);
void				main_boom(HDC hdc);
void				main_glide(HDC hdc);
void				main_gold(HDC hdc);
void				main_plane(HDC hdc);
void				main_bomb(HDC hdc);
void				main_background(HDC hdc);
void				Over(HDC hdc);
void				vCannon(HDC hdc);
void				vBegin(HDC hdc);
void				Printproc(HDC hdc,float power);
void				Printheart(HDC hdc,int man_life);
void				Printdis(HDC hdc,int distance);
void				Shot(int n);
void				Rank(int distance);
int					change(char* filename);
void				CheckDie1(float y);//�жϴ��մ�������
bool				CheckDie2(int x,int y);//�жϴ�������������
void				CheckDie3(int x,int y,int i);//�ж���������
bool				CheckDie4(int x,int y,int j);//�жϽ�ұ���
bool				CheckDie5(int x,int y);//�жϴ�����ը������

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	MyRegisterClass(hInstance);
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}
	while( msg.message!=WM_QUIT )
    {
        if( PeekMessage( &msg, NULL, 0,0 ,PM_REMOVE) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		else
		{
			tNow = GetTickCount();
			if(tNow-tPre>=5)
				handleKey();
			if(judge==2)		//��ʼ����
				vBegin(hdc);
			else if(judge==0 && tNow-tPre >= 40)		//���ڽ���
				vCannon(hdc);
			else if(judge==1 && tNow-tPre >= 40)	//��Ϸ����
			{
				if(flag2==1/*��ֵΪ1���Ըı��ʼ�ٶ�*/)
					Shot(num_cannon);
				Main(hdc);
			}
			else if(isover == true&&judge==-1)		//��������
			{
				Over(hdc);
			}
		}
	}
	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "canvas";
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HBITMAP bmp;
	hInst = hInstance;

	hWnd = CreateWindow("canvas", "Window" , WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	MoveWindow(hWnd,10,10,1280,580,true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hdc = GetDC(hWnd);
	mdc = CreateCompatibleDC(hdc);
	Background = CreateCompatibleDC(hdc);
    Fly=CreateCompatibleDC(hdc);
	Cannon=CreateCompatibleDC(hdc);
	Droping=CreateCompatibleDC(hdc);
	Die=CreateCompatibleDC(hdc);
	Eject=CreateCompatibleDC(hdc);
	Bat=CreateCompatibleDC(hdc);
	Bullet=CreateCompatibleDC(hdc);
	Fire=CreateCompatibleDC(hdc);
	Glide=CreateCompatibleDC(hdc);
	Printd=CreateCompatibleDC(hdc);
	Gold=CreateCompatibleDC(hdc);
	Process1=CreateCompatibleDC(hdc);
	Process2=CreateCompatibleDC(hdc);
	Plane=CreateCompatibleDC(hdc);
	Bomb=CreateCompatibleDC(hdc);
	Heart=CreateCompatibleDC(hdc);
	bmp = CreateCompatibleBitmap(hdc,1000,6704);
	SelectObject(mdc,bmp);

	bg = (HBITMAP)LoadImage(NULL,"bg3.bmp",IMAGE_BITMAP,1280,10000,LR_LOADFROMFILE);
	fly = (HBITMAP)LoadImage(NULL,"fx.bmp",IMAGE_BITMAP,500,500,LR_LOADFROMFILE);
	cannon=(HBITMAP)LoadImage(NULL,"dapao.bmp",IMAGE_BITMAP,3600,800,LR_LOADFROMFILE);
	over=(HBITMAP)LoadImage(NULL,"cp.bmp",IMAGE_BITMAP,1280,580,LR_LOADFROMFILE);
	begin1=(HBITMAP)LoadImage(NULL,"begin1.bmp",IMAGE_BITMAP,1280,580,LR_LOADFROMFILE);
	begin2=(HBITMAP)LoadImage(NULL,"begin2.bmp",IMAGE_BITMAP,1280,580,LR_LOADFROMFILE);
	begin3=(HBITMAP)LoadImage(NULL,"begin3.bmp",IMAGE_BITMAP,1280,580,LR_LOADFROMFILE);
	begin4=(HBITMAP)LoadImage(NULL,"begin4.bmp",IMAGE_BITMAP,1280,580,LR_LOADFROMFILE);
	drop=(HBITMAP)LoadImage(NULL,"xialuo.bmp",IMAGE_BITMAP,500,500,LR_LOADFROMFILE);
	eject=(HBITMAP)LoadImage(NULL,"penshe.bmp",IMAGE_BITMAP,500,500,LR_LOADFROMFILE);
	die=(HBITMAP)LoadImage(NULL,"daodi.bmp",IMAGE_BITMAP,500,250,LR_LOADFROMFILE);
	bullet=(HBITMAP)LoadImage(NULL,"BULLET1.bmp",IMAGE_BITMAP,30,60,LR_LOADFROMFILE);
	bat=(HBITMAP)LoadImage(NULL,"bianfu.bmp",IMAGE_BITMAP,1200,250,LR_LOADFROMFILE);
	boom=(HBITMAP)LoadImage(NULL,"boom.bmp",IMAGE_BITMAP,1600,400,LR_LOADFROMFILE);
	fire=(HBITMAP)LoadImage(NULL,"AK.bmp",IMAGE_BITMAP,500,500,LR_LOADFROMFILE);
	glide=(HBITMAP)LoadImage(NULL,"slowd.bmp",IMAGE_BITMAP,500,250,LR_LOADFROMFILE);
	gold=(HBITMAP)LoadImage(NULL,"gold.bmp",IMAGE_BITMAP,1008,130,LR_LOADFROMFILE);
	printd=(HBITMAP)LoadImage(NULL,"shuzi.bmp",IMAGE_BITMAP,320,96,LR_LOADFROMFILE);
	process1=(HBITMAP)LoadImage(NULL,"jindu.bmp",IMAGE_BITMAP,442,30,LR_LOADFROMFILE);
	process2=(HBITMAP)LoadImage(NULL,"jindu.bmp",IMAGE_BITMAP,442,30,LR_LOADFROMFILE);
	plane=(HBITMAP)LoadImage(NULL,"feiji.bmp",IMAGE_BITMAP,256,86,LR_LOADFROMFILE);
	bomb=(HBITMAP)LoadImage(NULL,"zhadan.bmp",IMAGE_BITMAP,80,40,LR_LOADFROMFILE);
	heart=(HBITMAP)LoadImage(NULL,"heart.bmp",IMAGE_BITMAP,28,48,LR_LOADFROMFILE);
	return TRUE;
}

void vBegin(HDC hdc)													//��ʼ
{
	if(flag6==1/*��ֵΪ1���Բ�����Ч*/)
	{
		mciSendString("open login.wav type avivideo alias login",buf,strlen(buf),NULL);
	    mciSendString("play login repeat",buf,strlen(buf),NULL);
	}
	if(judge2==1)
	{
		SelectObject(mdc,begin1);
		BitBlt(hdc,0,0,1280,580,mdc,0,0,SRCCOPY);
	}
	else if(judge2==2)
	{
		SelectObject(mdc,begin2);
		BitBlt(hdc,0,0,1280,580,mdc,0,0,SRCCOPY);
	}
	else if(judge2==3)
	{
		SelectObject(mdc,begin3);
		BitBlt(hdc,0,0,1280,580,mdc,0,0,SRCCOPY);
	}
	else if(judge2==4)
	{
		SelectObject(mdc,begin4);
		BitBlt(hdc,0,0,1280,580,mdc,0,0,SRCCOPY);
		if(flag15==1)
		{
			Rank(0);
			Printdis(mdc,bestscore*10);
		}
	}
}

void Main(HDC hdc)
{
	srand((unsigned)time(NULL));										//�����
	gametime++;
	tPre = GetTickCount();
	main_background(mdc);												//������
	Printdis(mdc,distance);												//������
	Printproc(hdc,power);												//������������
	Printheart(mdc,man_life);
	x += vx;															//�����趨
	distance+=vx;

	if(flag1==1/*��ֵΪ1��������*/ && flag3==1/*��ֵΪ1�������䡢�����ӵ�*/ &&flag7==0)//��������
		main_eject(hdc);
	else if(flag7==1/*��ֵΪ1���Է����ӵ�*/)							//���ӵ�����
	{
		main_fire(hdc);
		if(bcount!=0 && flag3==1/*��ֵΪ1�������䡢�����ӵ�*/)
			main_bullet(mdc);
		main_gold(mdc);													//���
		main_bomb(mdc);													//ը��
	}
	else if(vy>0 && flag7==0)												//��������
	{
		main_fly(hdc);
		if(bcount!=0 && flag3==1/*��ֵΪ1�������䡢�����ӵ�*/)
			main_bullet(mdc);
		main_gold(mdc);													//���
		main_bomb(mdc);													//ը��
	}
	else if(vy<=0 && isover==false && flag7==0&&flag8==0)				//���䶯��
	{
		main_drop(hdc);
		if(bcount!=0 && flag3==1/*��ֵΪ1�������䡢�����ӵ�*/)
			main_bullet(mdc);
		main_gold(mdc);													//���
		main_bomb(mdc);													//ը��
	}
	else if(vy<=0 && isover==false && flag7==0&&flag8==1)				//����
	{
		main_glide(hdc);
		main_bullet(mdc);
		main_gold(mdc);													//���
		main_bomb(mdc);													//ը��
	}
	else if(isover==true)												//����
		main_die(hdc);
	
	if(bat_time<=300 && flag5==1/*��ֵΪ1��ʾ�������*/)				//����
	    main_bat(mdc);
	else if(flag5==0)													//����ը
		main_boom(mdc);
	bat_time++;
	if(bat_time==300)
	{
		bat_life=3;
        bat_time=0;
	    Bat_x=0;
		flag4=1;/*��ֵΪ1��������Bat_y�����*/
	}
	
	if(plane_time<=200 && flag12==1)									//�ɻ�
	    main_plane(mdc);
	plane_time++;
	if(plane_time==200)
	{
		flag12=1;
		plane_time=0;
		Plane_x=0;
	}

	BitBlt(hdc,0,0,1280,580,mdc,0,0,SRCCOPY);
	
	if(x==1280)															//���������ѭ��
		x = 0;
		
	num++;																//�زĶ�������
	if(num==2)
		num=0;
	
	CheckDie1(y);														//�ж����ﴥ������
	
			
}

void main_background(HDC hdc)											  //����
{
	SelectObject(Background,bg);
	BitBlt(mdc,1280-x,0,x,580,Background,0,y,SRCCOPY);
	BitBlt(mdc,0,0,1280-x,580,Background,x,y,SRCCOPY);
	if(gametime>500 && gametime<1500 && (int)x%80==0)													//�ٶ�����
		vx=80;
	else if((int)x%40==0 && gametime>=1500 && gametime <2500)
		vx=40;
	else if((int)x%20==0 && gametime>=2500)
		vx=20;
	
}

void Printproc(HDC hdc,float power)										  //������
{
	SelectObject(Process1,process1);
	SelectObject(Process2,process2);
	BitBlt(hdc,1020,0,221,15,Process2,221,15,SRCAND);
	BitBlt(hdc,1020,0,221,15,Process2,221,0,SRCPAINT);
	proc=power/100;
	BitBlt(hdc,1020,0,200*proc,15,Process2,0,15,SRCAND);
	BitBlt(hdc,1020,0,200*proc,15,Process2,0,0,SRCPAINT);
}

void main_plane(HDC hdc)												   //�ɻ�
{
	if(gametime>200)
	{
		SelectObject(Plane,plane); 
		BitBlt(mdc,1200-Plane_x,0,128,86,Plane,128,0,SRCAND);
		BitBlt(mdc,1200-Plane_x,0,128,86,Plane,0,0,SRCPAINT);
		Plane_x+=20;
		if(Plane_x>3000)
			flag12=0;
	}
}

void main_eject(HDC hdc)
{
	SelectObject(Eject,eject);											    //����
	vy+=ay;
	distance+=vx;
	y-=vy;
	BitBlt(hdc,150,180,250,250,Eject,num*250,250,SRCAND);
	BitBlt(hdc,150,180,250,250,Eject,num*250,0,SRCPAINT);
	flag1=0;/*��ֵΪ1��������*/
	Sleep(50);
}

void main_fire(HDC hdc)
{
	SelectObject(Fire,fire);												//����
	BitBlt(hdc,140,170,250,250,Fire,num*250,250,SRCAND);
	BitBlt(hdc,140,170,250,250,Fire,num*250,0,SRCPAINT);
	Sleep(100);
	flag7=0;
	y-=vy;
}

void main_drop(HDC hdc)
{
	SelectObject(Droping,drop);												//����
	flag3=1;/*��ֵΪ1�������䡢�����ӵ�*/
	BitBlt(hdc,150,180,250,250,Droping,num*250,250,SRCAND);
	BitBlt(hdc,150,180,250,250,Droping,num*250,0,SRCPAINT);
	Sleep(50);
	vy=-15;
	y-=vy;
}

void main_fly(HDC hdc)
{
	SelectObject(Fly,fly);													//����
	BitBlt(hdc,150,180,250,250,Fly,num*250,250,SRCAND);
	BitBlt(hdc,150,180,250,250,Fly,num*250,0,SRCPAINT);
	Sleep(50);
	vy+=gy;
	y-=vy;
}

void main_die(HDC hdc)
{
	SelectObject(Die,die);													//����
	BitBlt(hdc,150,330,250,250,Die,250,0,SRCAND);
	BitBlt(hdc,150,330,250,250,Die,0,0,SRCPAINT);
	Sleep(2000);
	if(flag6==1/*��ֵΪ1���Բ�����Ч*/)
		PlaySound("over.wav", NULL, SND_FILENAME | SND_ASYNC);
	Sleep(3000);
	judge=-1;
}

void main_bullet(HDC hdc)
{
	SelectObject(Bullet,bullet);											//�ӵ�
	for(i=0;i<8;i++)
	{
		if(b[i].exist)
		{
			BitBlt(hdc,380+vbullet_x[i],280+vbullet_y[i],30,30,Bullet,0,30,SRCAND);
			BitBlt(hdc,380+vbullet_x[i],280+vbullet_y[i],30,30,Bullet,0,0,SRCPAINT);
			CheckDie3(380+vbullet_x[i],280+vbullet_y[i],i);
			vbullet_x[i]+=30;
			vbullet_y[i]+=vy/2;
				
			if(vbullet_x[i] > 1020||vbullet_y[i]>230)
			{
				bcount--;
				b[i].exist = false;
				vbullet_x[i]=0;
				vbullet_y[i]=0;
			}
		}
	}	
}

void main_gold(HDC hdc)														//���
{
	if(gametime>200)
	{
		SelectObject(Gold,gold);
		if((1200-Plane_x)==800 || (1200-Plane_x)==900 || (1200-Plane_x)==1000)//���ƽ�����
		{
			for(j=0;j<3;j++)
			{
		  		if(!golds[j].exist)
				{
					golds[j].exist = true;
		
				    break;
			    }
			}
			flag14=1;
		}
		for(j=0;j<3;j++)
		{
			if(golds[j].exist)
			{
				if(flag10[j]==0&&!CheckDie4(1200-Plane_x-vgold_x[j],vgold_y[j],j))
				{
					BitBlt(hdc,1200-Plane_x-vgold_x[j],vgold_y[j],63,65,Gold,63*num_gold,65,SRCAND);
					BitBlt(hdc,1200-Plane_x-vgold_x[j],vgold_y[j],63,65,Gold,63*num_gold,0,SRCPAINT);
				}
				vgold_x[j]+=(vx-23);
				vgold_y[j]+=vy;				
				if(1200-Plane_x-vgold_x[j]<0)
				{
					vgold_x[j]=0;
					vgold_y[j]=0;
					flag10[j]=0;
					golds[j].exist=false;
				}
			}
		}
		num_gold++;
		if(num_gold==16)
			num_gold=0;
	}
}

void main_bomb(HDC hdc)														//ը��
{
	if(gametime>200)
	{
		SelectObject(Bomb,bomb);
		if((1200-Plane_x)==1200)											//����ը�����
		{
			flag13=1;
			flag14=0;
		}
		if(flag13==1&&!CheckDie5(1200-Plane_x-bomb_x,bomb_y))
		{
			BitBlt(hdc,1200-Plane_x-bomb_x,bomb_y,40,40,Bomb,40,0,SRCAND);
			BitBlt(hdc,1200-Plane_x-bomb_x,bomb_y,40,40,Bomb,0,0,SRCPAINT);
			bomb_x+=(vx-20);
			bomb_y+=vy+5;				
			if(bomb_y>580 || 1200-Plane_x-bomb_x<0)
			{
				bomb_x=0;
				bomb_y=0;
				flag13=0;
			}
		}
	}
}

void main_bat(HDC hdc)														//����
{
	if(gametime>300 && !CheckDie2(1200-Bat_x,Bat_y))
	{
		if(flag4==1/*��ֵΪ1��������Bat_y�����*/)
			Bat_y=rand()%480;
		flag4=0;
		SelectObject(Bat,bat); 
		BitBlt(mdc,1200-Bat_x,Bat_y,200,125,Bat,200*num_bat,125,SRCAND);
		BitBlt(mdc,1200-Bat_x,Bat_y,200,125,Bat,200*num_bat,0,SRCPAINT);
		if(flag8==0)
			Bat_y+=vy/2;
		else if(flag8==1)
			Bat_y+=(vy*3)/2;
		Bat_x+=vx/5;
    	num_bat++;
		if(num_bat==3)
			num_bat=0;
																			//�ж����ﴥ��������
	}
}

void main_boom(HDC hdc)														//��ը
{
	SelectObject(Bat,boom);
	BitBlt(mdc,bex+20,bey-20,200,200,Bat,200*num_boom,200,SRCAND);
	BitBlt(mdc,bex+20,bey-20,200,200,Bat,200*num_boom,0,SRCPAINT);
	num_boom++;
	if(num_boom==8)
	{
		num_boom=0;
		flag5=1;/*��ֵΪ1��ʾ�������*/
	}
	bat_life=3;
}

void main_glide(HDC hdc)													//����
{
	SelectObject(Glide,glide);
	BitBlt(hdc,150,115,250,250,Glide,250,0,SRCAND);
	BitBlt(hdc,150,115,250,250,Glide,0,0,SRCPAINT);
	Sleep(50);
	vy=-5;
	y-=vy;
	flag8=0;
}

void Printdis(HDC hdc,int distance)											//����
{
	SelectObject(Printd,printd); 
	n[0]=(distance/10)%10;
	n[1]=(distance/10)/10%10;
	n[2]=(distance/10)/100%10;
	n[3]=(distance/10)/1000%10;
	n[4]=(distance/10)/10000%10;
	for(h=4;h>=0;h--)
	{
		BitBlt(mdc,850+70*(5-h),480,32,48,Printd,32*n[h],48,SRCAND);
		BitBlt(mdc,850+70*(5-h),480,32,48,Printd,32*n[h],0,SRCPAINT);
	}
	if(isover && flag16==0)
	{
		Rank(distance/10);
		flag16=1;
	}
}

void Printheart(HDC hdc,int man_life)										//����
{
	SelectObject(Heart,heart);
	for(h=0;h<man_life;h++)
	{
		BitBlt(mdc,h*30,0,28,24,Heart,0,24,SRCAND);
		BitBlt(mdc,h*30,0,28,24,Heart,0,0,SRCPAINT);
	}
}
void CheckDie1(float y)														//�жϴ��մ�������
{
	if(y>9470)
	{
		flag9=0;
		isover=true;
		if(flag6==1/*��ֵΪ1���Բ�����Ч*/)
		{
			PlaySound("die.wav", NULL, SND_FILENAME | SND_ASYNC);
		}
	}
}

bool CheckDie2(int x,int y)													//�жϴ�������������
{
     if(x>=160 && x<=250 && y>=180 && y<=330)
	 {
		man_life--;
		if(man_life==0)
			isover=true;	 
		flag9=1;
		return true;
	 }
	 else
		 return false;
}

void CheckDie3(int x,int y,int i)											//�ж���������
{
	if(x>=1280-Bat_x&&x<=1320-Bat_x&&y>=Bat_y+30&&y<=Bat_y+90)
	{
		bat_life--;
		if(flag6==1/*��ֵΪ1���Բ�����Ч*/ && bat_life!=0)
		{
			mciSendString("close bathit",buf,strlen(buf),NULL);
			mciSendString("open bathit.wav type avivideo alias bathit",buf,strlen(buf),NULL);
			mciSendString("play bathit",buf,strlen(buf),NULL);
		}		
		bcount--;
		b[i].exist = false;
		vbullet_x[i]=0;
		vbullet_y[i]=0;
		if(bat_life==0)
		{
			power+=8;
			flag5=0;/*��ֵΪ1��ʾ�������*/
			bex=1200-Bat_x;
			bey=Bat_y;
			Bat_x=1500;
			if(flag6==1/*��ֵΪ1���Բ�����Ч*/)
			{
				mciSendString("close batdie",buf,strlen(buf),NULL);
				mciSendString("open batdie.wav type avivideo alias batdie",buf,strlen(buf),NULL);
				mciSendString("play batdie",buf,strlen(buf),NULL);				
			}
		}
	}
}

bool CheckDie4(int x,int y,int j)											//�жϽ�ұ���
{	
	if(x>=160&&x<=250&&y>=180&&y<=330)
	{
		if(flag6==1/*��ֵΪ1���Բ�����Ч*/)
		{
			mciSendString("close coins",buf,strlen(buf),NULL);
			mciSendString("open coins.wav type avivideo alias coins",buf,strlen(buf),NULL);
			mciSendString("play coins",buf,strlen(buf),NULL);
		}
		flag10[j]=1;
		power+=1;
		return true;	
	}
	else
		return false;
}

bool CheckDie5(int x,int y)													//�жϴ�����ը������
{
	if(x>=160&&x<=250&&y>=180&&y<=330)
	{
		flag13=0;
		flag9=2;
		if(flag6==1/*��ֵΪ1���Բ�����Ч*/)
		{
			mciSendString("close boom",buf,strlen(buf),NULL);
			mciSendString("open boom.wav type avivideo alias boom",buf,strlen(buf),NULL);
			mciSendString("play boom",buf,strlen(buf),NULL);
		}
		man_life--;
		if(man_life==0)
			isover=true;
		return true;	
	}
	else
		return false;
}

void vCannon(HDC hdc)														//���ڽ���
{
	SelectObject(Cannon,cannon);
	main_background(mdc);
	BitBlt(mdc,30,215,300,400,Cannon,num_cannon*300,400,SRCAND);
	BitBlt(mdc,30,215,300,400,Cannon,num_cannon*300,0,SRCPAINT);
	BitBlt(hdc,0,0,1280,580,mdc,0,0,SRCCOPY);
	tPre = GetTickCount();
	Sleep(100);
	num_cannon+=sway;
	if(num_cannon==11)
		sway=-1;
	else if(num_cannon==0)
		sway=1;
	judge=0;
}

void Shot(int n)															//�Ƕ��жϺ���
{
	n/=2;
	switch(n)
	{
		case 0:
			vx=80;
			vy=50;
			break;
		case 1:
			vx=40;
			vy=60;
			break;
		case 2:
			vx=40;
			vy=80;
			break;
		case 3:
			vx=20;
			vy=90;
			break;
		case 4:
			vx=10;
			vy=110;
			break;
		case 5:
			vx=5;
			vy=130;
			break;
	}	
	flag2=0;/*��ֵΪ1���Ըı��ʼ�ٶ�*/
}

void Over(HDC hdc)															//��������
{
	SelectObject(mdc,over);
	BitBlt(hdc,0,0,1280,580,mdc,0,0,SRCCOPY);	
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{   
		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_UP:
				if(power>0)
				flag1=1;/*��ֵΪ1��������*/
				if(flag3==1/*��ֵΪ1�������䡢�����ӵ�*/)
					power-=5;
				if(power<0)
				{
					flag1=0;/*��ֵΪ1��������*/
					power=0;
				}
				if(flag6==1/*��ֵΪ1���Բ�����Ч*/)
					PlaySound("fire.wav", NULL, SND_FILENAME | SND_ASYNC);
				break;
			}
			if(GetKeyState(VK_ESCAPE)<0)
				PostQuitMessage(0);
			break;

		case  WM_LBUTTONDOWN:
			beginx=LOWORD(lParam);
			beginy=HIWORD(lParam);
			if(beginx>=525 && beginx<=750 && beginy>=195 && beginy<=255)
				judge=0;
			if(beginx>=525 && beginx<=750 && beginy>=275 && beginy<=335)
				flag6=1;/*��ֵΪ1���Բ�����Ч*/
			if(beginx>=525 && beginx<=750 && beginy>=355 && beginy<=415)
				flag15=1;
			break;

		case WM_MOUSEMOVE:
			vbeginx=LOWORD(lParam);
			vbeginy=HIWORD(lParam);
			if(vbeginx>=525 && vbeginx<=750 && vbeginy>=195 && vbeginy<=255)
				judge2=2;
			else if(vbeginx>=525 && vbeginx<=750 && vbeginy>=275 && vbeginy<=335)
				judge2=3;
			else if(vbeginx>=525 && vbeginx<=750 && vbeginy>=355 && vbeginy<=415)
				judge2=4;
			else
				judge2=1;
			break;

		case WM_DESTROY:			
			DeleteDC(mdc);
			DeleteDC(Background);
			DeleteDC(Fly);
			DeleteDC(Cannon);
			DeleteDC(Droping);
			DeleteDC(Eject);
			DeleteDC(Die);
			DeleteDC(Bullet);
			DeleteDC(Bat);
			DeleteDC(Fire);
			DeleteDC(Glide);
			DeleteDC(Gold);
			DeleteDC(Printd);
			DeleteDC(Process1);
			DeleteDC(Process2);
			DeleteDC(Plane);
			DeleteDC(Bomb);
			DeleteDC(Heart);
			DeleteObject(bg);
			DeleteObject(fly);
			DeleteObject(over);
			DeleteObject(begin1);
			DeleteObject(begin2);
			DeleteObject(begin3);
			DeleteObject(begin4);
			DeleteObject(cannon);
			DeleteObject(drop);
			DeleteObject(eject);
			DeleteObject(die);
			DeleteObject(bullet);
			DeleteObject(bat);
			DeleteObject(fire);
			DeleteObject(glide);
			DeleteObject(gold);
			DeleteObject(printd);
			DeleteObject(process1);
			DeleteObject(process2);
			DeleteObject(plane);
			DeleteObject(bomb);
			DeleteObject(heart);
			ReleaseDC(hWnd,hdc);
			PostQuitMessage(0);
			break;

		default:							
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

void handleKey()
{
	if(::GetKeyState(VK_SPACE)<0)
	{
		judge=1;
		if(flag6==1/*��ֵΪ1���Բ�����Ч*/)
			PlaySound("rocket.wav", NULL, SND_FILENAME | SND_ASYNC);
	}
	if(::GetKeyState(VK_RIGHT)<0)
	{
	    for(i=0;i<8;i++)
		{
		  	if(!b[i].exist)
			{
				b[i].exist = true;
			    bcount++;
				if(flag6==1)
					PlaySound("ak47.wav", NULL, SND_FILENAME | SND_ASYNC);
			    break;
		    }
		}
		if(flag3==1/*��ֵΪ1�������䡢�����ӵ�*/)
			flag7=1;
	}
	if(::GetKeyState(VK_DOWN)<0)
	{
		flag8=1;
	}
}

int change(char* filename)
{
	if(filename[4]>='0' && filename[4]<='9')
		return (filename[0]-48)*10000+(filename[1]-48)*1000+(filename[2]-48)*100+(filename[3]-48)*10+filename[4];
	else if(filename[3]>='0' && filename[3]<='9')
		return (filename[0]-48)*1000+(filename[1]-48)*100+(filename[2]-48)*10+(filename[3]-48);
	else if(filename[2]>='0' && filename[2]<='9')
		return (filename[0]-48)*100+(filename[1]-48)*10+(filename[2]-48);
	else if(filename[1]>='0' && filename[1]<='9')
		return (filename[0]-48)*10+(filename[1]-48);
	else if(filename[0]>='0' && filename[0]<='9')
		return (filename[0]-48);
	else
		return 0;
}

void Rank(int distance)						//��߷�
{
	FILE *fp=NULL;
	char rank[]="rank.txt";
	char dis[6];
	int result;
	char Distance[6];

	itoa(distance,dis,10);
	result=(access(rank,0)==0);

	if(result)
	{
		fp=fopen(rank,"a");
		if(distance!=0)
		{
			fputs(dis,fp);
			fputc('\n',fp);
		}
		fclose(fp);
	}
	else
	{
		fp=fopen(rank,"w");
		if(distance!=0)
		{
			fputs(dis,fp);
			fputc('\n',fp);
		}
		fclose(fp);
	}

	fp=fopen(rank,"r");

	while(fgets(Distance,7,fp)!=NULL)
	{
		if(change(Distance)>bestscore)
			bestscore=change(Distance);
	}

	fclose(fp);
}
