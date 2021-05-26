//2020年12月27日18:47:50 徐思远2019010372
#pragma once

#define  _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <graphics.h>
#include <process.h>
#include <easyx.h>
#pragma comment(lib,"Winmm.lib")//PlaySound函数在这个库文件里实现

#define MapLie 10   //矩形的高
#define MapHang 14 //矩阵的宽
#define Len_Image 80//正方形图片边长
extern IMAGE leagueoflegend[13]; //lol的图案库
extern IMAGE sanguosha[13];	//三国杀图案库
extern char lol[];  // 地图中的lol的元素（字母）
extern char sgs[];  //地图中三国杀元素（数字）
extern int pause_flag;  // 置为1说明游戏暂停 不计时

LRESULT CALLBACK CBTHookProc(int nCode, WPARAM wParam, LPARAM lParam);
int Msg(HWND hwnd, TCHAR *szText, TCHAR *szCaption, UINT uType);
//判断是否可以消除
int OneLine(int start[2], int end[2]);//只用1条线（同列或同行）
int TwoLines(int start[2], int end[2]);//用2条线（折线）
int ThreeLines(int start[2], int end[2]);//用3条线（3=2+1）
//可以消除画路径
int DrawLine(int start[2], int end[2]);//1条线
int DrawTwoLines(int start[2], int end[2]);//2条
int DrawThreeLines(int start[2], int end[2]);//3条

//地图显示相关
void Oninit_all();  //初始化所有数据
void CreateMapsgs();    //随机生成三国杀地图
void CreateMaplol();    //随机生成lol地图
void ShowMapsgs();  //显示三国杀地图
void ShowMaplol();  //显示lol地图
void Wohin_gehst_du(int Dir);  //根据输入的Direction执行地图元素移动
void Element_bewegen(); //根据关卡调用Wohin_gehst_du(你要去哪里呀)
void Show_time(void *); //显示时间

void Save_game();   //存档
void Read_game();   //读档

int Modify_XY(double pos);//修正鼠标位置
void Get_Mouse();//获取鼠标事件
void Get_Keyboard();//获取键盘事件
void delete_or_not(int xt[2], int yt[2]);//执行判断消除操作
int find_step(int x, int y);//找寻下一组解 返回0代表已无解
void Show_helpstep(int kb_key);//提示下一组解
int Find_step_solution();//判断游戏是否无解
void judge_game();//判断游戏是否结束
void start_game();//开始游戏
void restart_game();//重开游戏
void zhudongxipai();//主动洗牌