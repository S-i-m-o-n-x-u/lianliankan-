//2020��12��27��18:47:50 ��˼Զ2019010372
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
#pragma comment(lib,"Winmm.lib")//PlaySound������������ļ���ʵ��

#define MapLie 10   //���εĸ�
#define MapHang 14 //����Ŀ�
#define Len_Image 80//������ͼƬ�߳�
extern IMAGE leagueoflegend[13]; //lol��ͼ����
extern IMAGE sanguosha[13];	//����ɱͼ����
extern char lol[];  // ��ͼ�е�lol��Ԫ�أ���ĸ��
extern char sgs[];  //��ͼ������ɱԪ�أ����֣�
extern int pause_flag;  // ��Ϊ1˵����Ϸ��ͣ ����ʱ

LRESULT CALLBACK CBTHookProc(int nCode, WPARAM wParam, LPARAM lParam);
int Msg(HWND hwnd, TCHAR *szText, TCHAR *szCaption, UINT uType);
//�ж��Ƿ��������
int OneLine(int start[2], int end[2]);//ֻ��1���ߣ�ͬ�л�ͬ�У�
int TwoLines(int start[2], int end[2]);//��2���ߣ����ߣ�
int ThreeLines(int start[2], int end[2]);//��3���ߣ�3=2+1��
//����������·��
int DrawLine(int start[2], int end[2]);//1����
int DrawTwoLines(int start[2], int end[2]);//2��
int DrawThreeLines(int start[2], int end[2]);//3��

//��ͼ��ʾ���
void Oninit_all();  //��ʼ����������
void CreateMapsgs();    //�����������ɱ��ͼ
void CreateMaplol();    //�������lol��ͼ
void ShowMapsgs();  //��ʾ����ɱ��ͼ
void ShowMaplol();  //��ʾlol��ͼ
void Wohin_gehst_du(int Dir);  //���������Directionִ�е�ͼԪ���ƶ�
void Element_bewegen(); //���ݹؿ�����Wohin_gehst_du(��Ҫȥ����ѽ)
void Show_time(void *); //��ʾʱ��

void Save_game();   //�浵
void Read_game();   //����

int Modify_XY(double pos);//�������λ��
void Get_Mouse();//��ȡ����¼�
void Get_Keyboard();//��ȡ�����¼�
void delete_or_not(int xt[2], int yt[2]);//ִ���ж���������
int find_step(int x, int y);//��Ѱ��һ��� ����0�������޽�
void Show_helpstep(int kb_key);//��ʾ��һ���
int Find_step_solution();//�ж���Ϸ�Ƿ��޽�
void judge_game();//�ж���Ϸ�Ƿ����
void start_game();//��ʼ��Ϸ
void restart_game();//�ؿ���Ϸ
void zhudongxipai();//����ϴ��