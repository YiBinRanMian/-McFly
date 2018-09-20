
#include "stdafx.h"
#include <stdio.h>
#include <time.h>
#include <io.h>
#include <ctype.h>
#include <stdlib.h>
#include "mmsystem.h"//导入声音头文件
#pragma comment(lib,"winmm.lib")//导入声音头文件库

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

float	x=0;//x初始坐标
float   y=9450;//y初始坐标
float   vx=10;//vx初始速度
float   vy=100;//vy初始速度
float	gy=-1;//模拟重力
float	ay=40;//喷射y轴加速度
int		man_life=2;//川普生命值
int     bex=0,bey=0;//蝙蝠死亡坐标
int		Bat_x=0;//蝙蝠横坐标
int     Plane_x=0;//飞机横坐标
int Bat_y=0;//蝙蝠纵坐标
int		bomb_x=0,bomb_y=0;//炸弹横纵坐标
int		i;//子弹相关
int     j=0;//金币相关
int		p;//生命相关
int		bcount=0;//子弹数量
int		judge=2;//开始2、开炮0、游戏1、结束画面切换-1
int     judge2=1;//开始界面判断悬停位置
int		num=0;//飞行、喷射、发射子弹、下落动作
int		num_cannon=0;//开炮&&角度判断
int		num_bat=0;//蝙蝠
int		num_boom=0;//爆炸
int		num_gold=0;//金币
int		beginx,beginy;//开始界面鼠标坐标
int		vbeginx,vbeginy;//鼠标悬停坐标
int		sway=1;//大炮摆动
int		flag1=0;/*数值为1可以喷射*/
int		flag2=1;/*数值为1可以改变初始速度*/
int		flag3=0;/*数值为1可以喷射、发射子弹*/
int		flag4=1;/*数值为1可以生成Bat_y随机数*/
int		flag5=1;/*数值为1表示蝙蝠活着*/
int		flag6=0;/*数值为1可以播放音效*/
int		flag7=0;/*数值为1可以发射子弹*/
int		flag8=0;//数值为1可以滑翔
int		flag9=0;//数值为1表示在空中死亡为0表示触地死亡2表示碰炸弹死亡
int		flag10[3]={0,0,0};//金币是否被吃
int		flag11=1;//播放bgm
int     flag12=1;//控制飞机x坐标
int		flag13=0;//控制炸弹
int		flag14=0;//控制炸弹和金币交替出现
int     flag15=0;//控制排行榜功能
int		flag16=0;//点击最高分运行多次Rank的情况
int		distance=0;//记录飞行距离
int		bestscore=0;//最高分
float	power=100;//记录剩余能量
float   proc;//进度条百分数
int		vbullet_x[8],vbullet_y[8];//子弹x和y轴的速度
int     vgold_x[3],vgold_y[3];//金币x和y轴速度
int		bat_time=0;//控制蝙蝠生成时间
int		plane_time=0;//控制飞机生成时间
int		gametime=0;//游戏开始时间计算
int     bat_life=3;//蝙蝠生命
int		n[5];//显示距离
int     h;//数字输出位数
bool    isover;//判断游戏结束
BULLET	b[8];  //子弹
GOLD	golds[3];//金币
char    buf[50];//音效相关

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
void				CheckDie1(float y);//判断川普触地死亡
bool				CheckDie2(int x,int y);//判断川普碰蝙蝠死亡
void				CheckDie3(int x,int y,int i);//判断蝙蝠死亡
bool				CheckDie4(int x,int y,int j);//判断金币被吃
bool				CheckDie5(int x,int y);//判断川普碰炸弹死亡

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
			if(judge==2)		//初始界面
				vBegin(hdc);
			else if(judge==0 && tNow-tPre >= 40)		//开炮界面
				vCannon(hdc);
			else if(judge==1 && tNow-tPre >= 40)	//游戏界面
			{
				if(flag2==1/*数值为1可以改变初始速度*/)
					Shot(num_cannon);
				Main(hdc);
			}
			else if(isover == true&&judge==-1)		//结束界面
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

void vBegin(HDC hdc)													//开始
{
	if(flag6==1/*数值为1可以播放音效*/)
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
	srand((unsigned)time(NULL));										//随机数
	gametime++;
	tPre = GetTickCount();
	main_background(mdc);												//贴背景
	Printdis(mdc,distance);												//贴距离
	Printproc(hdc,power);												//贴能量进度条
	Printheart(mdc,man_life);
	x += vx;															//距离设定
	distance+=vx;

	if(flag1==1/*数值为1可以喷射*/ && flag3==1/*数值为1可以喷射、发射子弹*/ &&flag7==0)//空中喷射
		main_eject(hdc);
	else if(flag7==1/*数值为1可以发射子弹*/)							//发子弹动作
	{
		main_fire(hdc);
		if(bcount!=0 && flag3==1/*数值为1可以喷射、发射子弹*/)
			main_bullet(mdc);
		main_gold(mdc);													//金币
		main_bomb(mdc);													//炸弹
	}
	else if(vy>0 && flag7==0)												//上升动作
	{
		main_fly(hdc);
		if(bcount!=0 && flag3==1/*数值为1可以喷射、发射子弹*/)
			main_bullet(mdc);
		main_gold(mdc);													//金币
		main_bomb(mdc);													//炸弹
	}
	else if(vy<=0 && isover==false && flag7==0&&flag8==0)				//下落动作
	{
		main_drop(hdc);
		if(bcount!=0 && flag3==1/*数值为1可以喷射、发射子弹*/)
			main_bullet(mdc);
		main_gold(mdc);													//金币
		main_bomb(mdc);													//炸弹
	}
	else if(vy<=0 && isover==false && flag7==0&&flag8==1)				//滑翔
	{
		main_glide(hdc);
		main_bullet(mdc);
		main_gold(mdc);													//金币
		main_bomb(mdc);													//炸弹
	}
	else if(isover==true)												//死亡
		main_die(hdc);
	
	if(bat_time<=300 && flag5==1/*数值为1表示蝙蝠活着*/)				//蝙蝠
	    main_bat(mdc);
	else if(flag5==0)													//蝙蝠爆炸
		main_boom(mdc);
	bat_time++;
	if(bat_time==300)
	{
		bat_life=3;
        bat_time=0;
	    Bat_x=0;
		flag4=1;/*数值为1可以生成Bat_y随机数*/
	}
	
	if(plane_time<=200 && flag12==1)									//飞机
	    main_plane(mdc);
	plane_time++;
	if(plane_time==200)
	{
		flag12=1;
		plane_time=0;
		Plane_x=0;
	}

	BitBlt(hdc,0,0,1280,580,mdc,0,0,SRCCOPY);
	
	if(x==1280)															//背景横向的循环
		x = 0;
		
	num++;																//素材动作控制
	if(num==2)
		num=0;
	
	CheckDie1(y);														//判断人物触地死亡
	
			
}

void main_background(HDC hdc)											  //背景
{
	SelectObject(Background,bg);
	BitBlt(mdc,1280-x,0,x,580,Background,0,y,SRCCOPY);
	BitBlt(mdc,0,0,1280-x,580,Background,x,y,SRCCOPY);
	if(gametime>500 && gametime<1500 && (int)x%80==0)													//速度修正
		vx=80;
	else if((int)x%40==0 && gametime>=1500 && gametime <2500)
		vx=40;
	else if((int)x%20==0 && gametime>=2500)
		vx=20;
	
}

void Printproc(HDC hdc,float power)										  //进度条
{
	SelectObject(Process1,process1);
	SelectObject(Process2,process2);
	BitBlt(hdc,1020,0,221,15,Process2,221,15,SRCAND);
	BitBlt(hdc,1020,0,221,15,Process2,221,0,SRCPAINT);
	proc=power/100;
	BitBlt(hdc,1020,0,200*proc,15,Process2,0,15,SRCAND);
	BitBlt(hdc,1020,0,200*proc,15,Process2,0,0,SRCPAINT);
}

void main_plane(HDC hdc)												   //飞机
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
	SelectObject(Eject,eject);											    //喷射
	vy+=ay;
	distance+=vx;
	y-=vy;
	BitBlt(hdc,150,180,250,250,Eject,num*250,250,SRCAND);
	BitBlt(hdc,150,180,250,250,Eject,num*250,0,SRCPAINT);
	flag1=0;/*数值为1可以喷射*/
	Sleep(50);
}

void main_fire(HDC hdc)
{
	SelectObject(Fire,fire);												//开火
	BitBlt(hdc,140,170,250,250,Fire,num*250,250,SRCAND);
	BitBlt(hdc,140,170,250,250,Fire,num*250,0,SRCPAINT);
	Sleep(100);
	flag7=0;
	y-=vy;
}

void main_drop(HDC hdc)
{
	SelectObject(Droping,drop);												//下落
	flag3=1;/*数值为1可以喷射、发射子弹*/
	BitBlt(hdc,150,180,250,250,Droping,num*250,250,SRCAND);
	BitBlt(hdc,150,180,250,250,Droping,num*250,0,SRCPAINT);
	Sleep(50);
	vy=-15;
	y-=vy;
}

void main_fly(HDC hdc)
{
	SelectObject(Fly,fly);													//飞行
	BitBlt(hdc,150,180,250,250,Fly,num*250,250,SRCAND);
	BitBlt(hdc,150,180,250,250,Fly,num*250,0,SRCPAINT);
	Sleep(50);
	vy+=gy;
	y-=vy;
}

void main_die(HDC hdc)
{
	SelectObject(Die,die);													//死亡
	BitBlt(hdc,150,330,250,250,Die,250,0,SRCAND);
	BitBlt(hdc,150,330,250,250,Die,0,0,SRCPAINT);
	Sleep(2000);
	if(flag6==1/*数值为1可以播放音效*/)
		PlaySound("over.wav", NULL, SND_FILENAME | SND_ASYNC);
	Sleep(3000);
	judge=-1;
}

void main_bullet(HDC hdc)
{
	SelectObject(Bullet,bullet);											//子弹
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

void main_gold(HDC hdc)														//金币
{
	if(gametime>200)
	{
		SelectObject(Gold,gold);
		if((1200-Plane_x)==800 || (1200-Plane_x)==900 || (1200-Plane_x)==1000)//控制金币输出
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

void main_bomb(HDC hdc)														//炸弹
{
	if(gametime>200)
	{
		SelectObject(Bomb,bomb);
		if((1200-Plane_x)==1200)											//控制炸弹输出
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

void main_bat(HDC hdc)														//蝙蝠
{
	if(gametime>300 && !CheckDie2(1200-Bat_x,Bat_y))
	{
		if(flag4==1/*数值为1可以生成Bat_y随机数*/)
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
																			//判断人物触蝙蝠死亡
	}
}

void main_boom(HDC hdc)														//爆炸
{
	SelectObject(Bat,boom);
	BitBlt(mdc,bex+20,bey-20,200,200,Bat,200*num_boom,200,SRCAND);
	BitBlt(mdc,bex+20,bey-20,200,200,Bat,200*num_boom,0,SRCPAINT);
	num_boom++;
	if(num_boom==8)
	{
		num_boom=0;
		flag5=1;/*数值为1表示蝙蝠活着*/
	}
	bat_life=3;
}

void main_glide(HDC hdc)													//滑翔
{
	SelectObject(Glide,glide);
	BitBlt(hdc,150,115,250,250,Glide,250,0,SRCAND);
	BitBlt(hdc,150,115,250,250,Glide,0,0,SRCPAINT);
	Sleep(50);
	vy=-5;
	y-=vy;
	flag8=0;
}

void Printdis(HDC hdc,int distance)											//距离
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

void Printheart(HDC hdc,int man_life)										//生命
{
	SelectObject(Heart,heart);
	for(h=0;h<man_life;h++)
	{
		BitBlt(mdc,h*30,0,28,24,Heart,0,24,SRCAND);
		BitBlt(mdc,h*30,0,28,24,Heart,0,0,SRCPAINT);
	}
}
void CheckDie1(float y)														//判断川普触地死亡
{
	if(y>9470)
	{
		flag9=0;
		isover=true;
		if(flag6==1/*数值为1可以播放音效*/)
		{
			PlaySound("die.wav", NULL, SND_FILENAME | SND_ASYNC);
		}
	}
}

bool CheckDie2(int x,int y)													//判断川普碰蝙蝠死亡
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

void CheckDie3(int x,int y,int i)											//判断蝙蝠死亡
{
	if(x>=1280-Bat_x&&x<=1320-Bat_x&&y>=Bat_y+30&&y<=Bat_y+90)
	{
		bat_life--;
		if(flag6==1/*数值为1可以播放音效*/ && bat_life!=0)
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
			flag5=0;/*数值为1表示蝙蝠活着*/
			bex=1200-Bat_x;
			bey=Bat_y;
			Bat_x=1500;
			if(flag6==1/*数值为1可以播放音效*/)
			{
				mciSendString("close batdie",buf,strlen(buf),NULL);
				mciSendString("open batdie.wav type avivideo alias batdie",buf,strlen(buf),NULL);
				mciSendString("play batdie",buf,strlen(buf),NULL);				
			}
		}
	}
}

bool CheckDie4(int x,int y,int j)											//判断金币被吃
{	
	if(x>=160&&x<=250&&y>=180&&y<=330)
	{
		if(flag6==1/*数值为1可以播放音效*/)
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

bool CheckDie5(int x,int y)													//判断川普碰炸弹死亡
{
	if(x>=160&&x<=250&&y>=180&&y<=330)
	{
		flag13=0;
		flag9=2;
		if(flag6==1/*数值为1可以播放音效*/)
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

void vCannon(HDC hdc)														//开炮界面
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

void Shot(int n)															//角度判断函数
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
	flag2=0;/*数值为1可以改变初始速度*/
}

void Over(HDC hdc)															//结束界面
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
				flag1=1;/*数值为1可以喷射*/
				if(flag3==1/*数值为1可以喷射、发射子弹*/)
					power-=5;
				if(power<0)
				{
					flag1=0;/*数值为1可以喷射*/
					power=0;
				}
				if(flag6==1/*数值为1可以播放音效*/)
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
				flag6=1;/*数值为1可以播放音效*/
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
		if(flag6==1/*数值为1可以播放音效*/)
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
		if(flag3==1/*数值为1可以喷射、发射子弹*/)
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

void Rank(int distance)						//最高分
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
