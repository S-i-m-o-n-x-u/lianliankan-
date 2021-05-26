//2020年12月27日18:47:34 徐思远2019010372
#include "function_declaration.h"
#include "math.h"
#define Len_Image 80   //连连看图片的边长
IMAGE sanguosha[13];//三国杀存储图库
IMAGE leagueoflegend[13];//lol的图案库
int readrank();    //读取存档并显示
int rank();    //写入存档
void PressKeyToRestart();       //按任意键回到菜单栏

typedef struct inf
{
    int guanshu;
	int fenshu;
}info;

int xt[2], yt[2];//存储点击位置
int rank_guanka[9],rank_fenshu[9];

int MapZhuangtai; //哪一种地图
char sgs[]="abcdefghijklm";// 地图中的三国杀的元素（字母）
char lol[]="nopqrstuvwxyz";//地图中lol元素（数字）
int score=0;    //分数
int combo_time;     //连击时间

int presscount = 0;//存储点击次数
int xipaicishu=1;//主动洗牌次数
int remaining = MapLie * MapHang;//剩余字母数
int kb_key;//键盘输入
int resist_time = 360;//游戏剩余时间
int helpcount = 2;//提示的次数
int guan_count = 1;//当前关卡数
int pause_flag = 0;//置为1说明游戏暂停 不计时

char Map[MapHang + 2][MapLie + 2] = { 0 };//字母前后要各有1个位置空白处用来走线（边上的可以直接消除）

LRESULT CALLBACK CBTHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND)wParam;
	if (nCode == HCBT_ACTIVATE)
	{
		if (nCode == HCBT_ACTIVATE)
		{
			SetDlgItemText((HWND)wParam, IDYES, "&三国杀");
			SetDlgItemText((HWND)wParam, IDNO, "&英雄联盟");
			SetDlgItemText((HWND)wParam, IDOK, "&OK");
			SetDlgItemText((HWND)wParam, IDCANCEL, "&Cancel");
			SetDlgItemText((HWND)wParam, IDABORT, "&开始新游戏");
			SetDlgItemText((HWND)wParam, IDRETRY, "&继续游戏");
			SetDlgItemText((HWND)wParam, IDIGNORE, "&查看排行榜");

		}
	}
	return 0;
}

int Msg(HWND hwnd, TCHAR *szText, TCHAR *szCaption, UINT uType)
{
	int ret;
	HHOOK hHook = SetWindowsHookEx(
		WH_CBT,
		CBTHookProc,
		NULL,
		GetCurrentThreadId()
		);
	ret = MessageBox(hwnd, szText, szCaption, uType);
	UnhookWindowsHookEx(hHook);
	return ret;
}

//判断三条线能否消除
int OneLine(int start[2], int end[2])//一条线消除
{
	int i;
	if (start[0] == end[0])
	{
		if (start[1] > end[1])
		{
			for (i = start[1] - 1; i > end[1]; i--)
				if (Map[start[0]][i] != 0)
					return 0;
			return 1;
		}
		else
		{
			for (i = start[1] + 1; i < end[1]; i++)
				if (Map[start[0]][i] != 0)
					return 0;
			return 1;
		}
	}
	else if (start[1] == end[1])
	{
		if (start[0] > end[0])
		{
			for (i = start[0] - 1; i > end[0]; i--)
				if (Map[i][start[1]] != 0)
					return 0;
			return 1;
		}
		else
		{
			for (i = start[0] + 1; i < end[0]; i++)
				if (Map[i][start[1]] != 0)
					return 0;
			return 1;
		}
	}
	return 0;
}//同行或同列能消除return1，否则return0
int TwoLines(int start[2], int end[2])
{
	if (OneLine(start, end) == 1)
		return 1;

	int p1[2], p2[2];
	p1[0] = start[0];
	p1[1] = end[1];
	p2[0] = end[0];
	p2[1] = start[1];

	if (Map[p1[0]][p1[1]] == 0 && OneLine(start, p1) && OneLine(end, p1))
		return 1;
	if (Map[p2[0]][p2[1]] == 0 && OneLine(start, p2) && OneLine(end, p2))
		return 1;

	return 0;
}//折线，两段OneLine均可走则可以消除
int ThreeLines(int start[2], int end[2])
{
	if (Map[start[0]][start[1]] == Map[end[0]][end[1]]
		&& (!(start[0] == end[0] && start[1] == end[1])))
	{//先后点击的start end字母相同且不是同一个位置
		if (TwoLines(start, end))
			return 1;

		int i, mid[2];
		for (i = start[0] - 1; i >= 0; i--)
		{
			if (Map[i][start[1]] != 0) break;
			mid[0] = i;
			mid[1] = start[1];
			if (TwoLines(mid, end)) return 1;
		}
		for (i = start[0] + 1; i <= MapHang + 1; i++)
		{
			if (Map[i][start[1]] != 0) break;
			mid[0] = i;
			mid[1] = start[1];
			if (TwoLines(mid, end)) return 1;
		}
		for (i = start[1] - 1; i >= 0; i--)
		{
			if (Map[start[0]][i] != 0) break;
			mid[0] = start[0];
			mid[1] = i;
			if (TwoLines(mid, end)) return 1;
		}
		for (i = start[1] + 1; i <= MapLie + 1; i++)
		{
			if (Map[start[0]][i] != 0) break;
			mid[0] = start[0];
			mid[1] = i;
			if (TwoLines(mid, end)) return 1;
		}
	}

	return 0;
}//化解成两条线和一条线的情况
//可以消除时画消除路径
int DrawLine(int start[2], int end[2])
{
	int i;
	if (start[0] == end[0])
	{
		if (start[1] > end[1])
		{
			for (i = start[1] - 1; i > end[1]; i--)
				if (Map[start[0]][i] != 0)
					return 0;
			line(start[0] * Len_Image + Len_Image/2, start[1] * Len_Image+ Len_Image/2, end[0] * Len_Image +Len_Image/2, end[1] * Len_Image + Len_Image/2);
			return 1;
		}
		else
		{
			for (i = start[1] + 1; i < end[1]; i++)
				if (Map[start[0]][i] != 0)
					return 0;
			line(start[0] * Len_Image + Len_Image/2, start[1] * Len_Image + Len_Image/2, end[0] * Len_Image + Len_Image/2, end[1] * Len_Image + Len_Image/2);
			return 1;
		}
	}
	else if (start[1] == end[1])
	{
		if (start[0] > end[0])
		{
			for (i = start[0] - 1; i > end[0]; i--)
				if (Map[i][start[1]] != 0)
					return 0;
			line(start[0] * Len_Image + Len_Image/2, start[1] * Len_Image + Len_Image/2, end[0] * Len_Image + Len_Image/2, end[1] * Len_Image + Len_Image/2);

			return 1;
		}
		else
		{
			for (i = start[0] + 1; i < end[0]; i++)
				if (Map[i][start[1]] != 0)
					return 0;
			line(start[0] * Len_Image + Len_Image/2, start[1] * Len_Image + Len_Image/2, end[0] * Len_Image + Len_Image/2, end[1] * Len_Image + Len_Image/2);

			return 1;
		}
	}
	return 0;
}
int DrawTwoLines(int start[2], int end[2])
{
	if (DrawLine(start, end) == 1)
		return 1;

	int p1[2], p2[2];
	p1[0] = start[0];
	p1[1] = end[1];
	p2[0] = end[0];
	p2[1] = start[1];

	if (Map[p1[0]][p1[1]] == 0 && OneLine(start, p1) && OneLine(end, p1))
	{
		DrawLine(start, p1);
		DrawLine(end, p1);
		return 1;
	}
	if (Map[p2[0]][p2[1]] == 0 && OneLine(start, p2) && OneLine(end, p2))
	{
		DrawLine(start, p2);
		DrawLine(end, p2);
		return 1;
	}
	return 0;
}
int DrawThreeLines(int start[2], int end[2])
{
	if (Map[start[0]][start[1]] == Map[end[0]][end[1]]
		&& (!(start[0] == end[0] && start[1] == end[1])))
	{//先后点击的start end字母相同且不是同一个位置
		if (DrawTwoLines(start, end))
			return 1;

		int i, mid[2];
		for (i = start[0] - 1; i >= 0; i--)
		{
			if (Map[i][start[1]] != 0) break;
			mid[0] = i;
			mid[1] = start[1];
			if (DrawTwoLines(mid, end))
			{
				DrawLine(mid, start);				
				return 1;
			}
		}
		for (i = start[0] + 1; i <= MapHang + 1; i++)
		{
			if (Map[i][start[1]] != 0) break;
			mid[0] = i;
			mid[1] = start[1];
			if (DrawTwoLines(mid, end))
			{
				DrawLine(mid, start);
				return 1;
			}
		}
		for (i = start[1] - 1; i >= 0; i--)
		{
			if (Map[start[0]][i] != 0) break;
			mid[0] = start[0];
			mid[1] = i;
			if (DrawTwoLines(mid, end))
			{
				DrawLine(mid, start);
				return 1;
			}
		}
		for (i = start[1] + 1; i <= MapLie + 1; i++)
		{
			if (Map[start[0]][i] != 0) break;
			mid[0] = start[0];
			mid[1] = i;
			if (DrawTwoLines(mid, end))
			{
				DrawLine(mid, start);
				return 1;
			}
		}
	}

	return 0;
}

//初始化 生成并显示地图
void Oninit_all()
{
	xt[0] = 0;//清空点击位置
	xt[1] = 0;//清空点击位置
	yt[0] = 0;//清空点击位置
	yt[1] = 0;//清空点击位置
	presscount = 0;//清空点击次数
	remaining = MapHang * MapLie;//加满剩余字母数

	for (int i = 0; i < MapHang + 1; i++)//清空地图
		for (int j = 0; j < MapLie + 1; j++)
			Map[i][j] = 0;

	resist_time = 360 - (guan_count - 1) * 10;//第一关360秒，每过一关时间减十秒，下限30s
	if (resist_time <= 30)
		resist_time = 30;	
}
void CreateMapsgs()//随机生成三国杀地图
{
	int cishu=0;
	srand((unsigned int)time(NULL));
	for (int p = 0, q = 0; p < 10;)
	{
		int tx = rand() % MapHang +1;
		int c = q % 13;
		int ty = rand() % (MapLie-1) +1;
		if ((Map[tx][ty] == 0)&&(Map[tx][ty+1] == 0))
		{
			Map[tx][ty] = sgs[c];
			Map[tx][ty+1] = sgs[c];
			p++;
			q++;
		}
	}
	//预置了十对相连的，竖向的

	for (int i = 0, j = 0; i < (MapHang * MapLie -20);)
	{
		
		int c = j % 13;
		int tx1 = rand() % MapHang +1;
		int ty1 = rand() % MapLie +1;


		if ((Map[tx1][ty1] == 0))
		{
			Map[tx1][ty1] = sgs[c];
			cishu++;
			if (cishu%2==0)
				j++;
			i++;

		}
		
	}//先填一个，再填一个，每填两次换一个图案，确保偶数。
}
void CreateMaplol()//随机生成lol地图
{
	int cishu = 0;
	srand((unsigned int)time(NULL));
	for (int p = 0, q = 0; p < 10;)
	{
		int tx = rand() % MapHang +1;
		int c = q % 13;
		int ty = rand() % (MapLie-1) +1;
		if ((Map[tx][ty] == 0)&&(Map[tx][ty+1] == 0))
		{
			Map[tx][ty] = lol[c];
			Map[tx][ty+1] = lol[c];
			p++;
			q++;
		}
	}
	//预置了十对相连的

	for (int i = 0, j = 0; i < (MapHang * MapLie -20);)
	{
		
		int c = j % 13;
		int tx1 = rand() % MapHang +1;
		int ty1 = rand() % MapLie +1;


		if ((Map[tx1][ty1] == 0))
		{
			Map[tx1][ty1] = lol[c];
			cishu++;
			if (cishu%2==0)
				j++;
			i++;

		}
		
	}
}
void ShowMapsgs()//显示三国杀地图
{
	cleardevice();//背景色清空屏幕
	for (int i = 0; i < 13; i++)
	{
		char s[20];
		sprintf(s, "图案\\%c.png", sgs[i]);
		loadimage(&sanguosha[i], s);
	}
	pause_flag = 0;//刷新地图时时间不暂停
	for (int i = 1; i < MapHang + 1; i++)
		for (int j = 1; j < MapLie + 1; j++)
			if (Map[i][j] != 0 && Map[i][j] != '*')
			{
				int num = strlen(sgs) - strlen(strchr(sgs, Map[i][j]));
				putimage(i * Len_Image,j * Len_Image, &sanguosha[num]);
			}
}
void ShowMaplol()//显示lol地图
{
	cleardevice();//背景色清空屏幕
	for (int i = 0; i < 13; i++)
	{
		char s[20];
		sprintf(s, "图案\\%c.png", lol[i]);
		loadimage(&leagueoflegend[i], s);
	}
	pause_flag = 0;//刷新地图时时间不暂停
	for (int i = 1; i < MapHang + 1; i++)
		for (int j = 1; j < MapLie + 1; j++)
			if (Map[i][j] != 0 && Map[i][j] != '*')
			{
				int num = strlen(lol) - strlen(strchr(lol, Map[i][j]));
				putimage(i * Len_Image,j * Len_Image, &leagueoflegend[num]);
			}
}
void xipai()
{
	char str2[50];
	sprintf(str2, "正在洗牌中···");
	outtextxy((MapHang + 2)*Len_Image / 2, 0, str2);
	int x1[2], x2[2];
	int flag = 0;
	srand((unsigned int)time(NULL));		
	while (1)
	{
		int tx = rand() % MapHang + 1;
		int ty = rand() % MapLie + 1;
		if (!flag)
		{
			if (Map[tx][ty] != 0)//不为空
			{
				x1[0] = tx;
				x1[1] = ty;
				flag = 1;
			}
		}
		else
		{
			if (Map[tx][ty] != 0 && Map[tx][ty] != Map[x1[0]][x1[1]])//不为空
			{
				x2[0] = tx;
				x2[1] = ty;
				break;
			}
		}
	}
	char chtem = Map[x1[0]][x1[1]];
	Map[x1[0]][x1[1]] = Map[x2[0]][x2[1]];
	Map[x2[0]][x2[1]] = chtem;

}//交换一下
void Wohin_gehst_du(int Dir)//你要去哪呀？输入数字，移动元素
{
	switch (Dir)
	{
	case 2://左
		for (int i = 2; i < MapHang + 1; i++)
			for (int j = 1; j < MapLie + 1; j++)
				if (Map[i-1][j] == 0)//左面的为空
				{
					Map[i-1][j] = Map[i][j];
					Map[i][j] = 0;
				}
		break;
	case 3://右
		for (int i = MapHang - 1; i > 0; i--)
			for (int j = 1; j < MapLie + 1; j++)
				if (Map[i+1][j] == 0) //右面的为空
				{
					Map[i+1][j] = Map[i][j];
					Map[i][j] = 0;
				}
		break;
	case 4://上
		for (int i = 1; i < MapHang + 1; i++)
			for (int j = 2; j < MapLie + 1; j++)
				if (Map[i][j-1] == 0)//上面的为空
				{
					Map[i][j-1] = Map[i][j];
					Map[i][j] = 0;
				}
		break;
	case 5://下
		for (int i = 1; i < MapHang + 1; i++)
			for (int j = MapLie - 1; j > 0; j--)
				if (Map[i][j+1] == 0)//下面的为空
				{
					Map[i][j+1] = Map[i][j];
					Map[i][j] = 0;
				}
		break;
	}
}
void Element_bewegen()//调用上述函数移动地图元素
{
	if (guan_count > 1)
	{
		for (int i = 1; i < MapHang - 1; i++)
			switch (guan_count)
			{
			case 2://上
				Wohin_gehst_du(4);
				break;
			case 3://下
				Wohin_gehst_du(5);
				break;
			case 4://左
				Wohin_gehst_du(2);
				break;
			case 5://右
				Wohin_gehst_du(3);
				break;
			case 6://左上
				Wohin_gehst_du(2);
				Wohin_gehst_du(4);
				break;
			case 7://右上
				Wohin_gehst_du(3);
				Wohin_gehst_du(4);
				break;
			case 8://左下
				Wohin_gehst_du(2);
				Wohin_gehst_du(5);
				break;
			case 9://右下
				Wohin_gehst_du(3);
				Wohin_gehst_du(5);
				break;
			default://其余情况左下
				Wohin_gehst_du(2);
				Wohin_gehst_du(5);
				break;
			}
	}
}
void Show_time(void *)
{
	while (1)
	{	
		outtextxy(0, 0, _T("                                                                                                                                                                                                              "));
		char str2[50];
		sprintf(str2, "按空格键提示（%d次） 按backspace主动洗牌（%d次）按Tab制表符保存游戏   按ESC退出游戏    剩余时间:%ds  关卡:%d  总得分:%d ", helpcount, xipaicishu, resist_time, guan_count,score);
		outtextxy(0, 0, str2);
		Sleep(1000);
		if (pause_flag == 0)
		{
			if (resist_time > 0)
				resist_time--;
		}
	}		
}
//游戏操作和判断结果
void Save_game()//存档
{
	char str[20];
	sprintf(str, "del memory.txt");
	system(str);//删除已有存档
	FILE  *mem;
	if ((mem = fopen("memory.txt", "w+")) == NULL)//创建失败则继续游戏
	{
		MessageBoxA(0, "存档失败", "连连看_天下英雄一起战斗吧", MB_OK || MB_SYSTEMMODAL);
		pause_flag = 1 - pause_flag;//游戏继续
	}
	else
	{
		//保存点击位置
		fwrite(&xt[0], sizeof(xt[0]), 1, mem);
		fwrite(&xt[1], sizeof(xt[1]), 1, mem);
		fwrite(&yt[0], sizeof(yt[0]), 1, mem);
		fwrite(&yt[1], sizeof(yt[1]), 1, mem);
		//保存点击次数
		fwrite(&presscount, sizeof(presscount), 1, mem);
		//保存剩余图片数
		fwrite(&remaining, sizeof(remaining), 1, mem);
		//保存剩余时间
		fwrite(&resist_time, sizeof(resist_time), 1, mem);
        //保存连击时间
		fwrite(&combo_time, sizeof(combo_time), 1, mem);
		//保存剩余求助次数
		fwrite(&helpcount, sizeof(helpcount), 1, mem);
        //保存剩余洗牌次数
		fwrite(&xipaicishu, sizeof(xipaicishu), 1, mem);
		//保存关卡数
		fwrite(&guan_count, sizeof(guan_count), 1, mem);
        //保存分数
		fwrite(&score, sizeof(score), 1, mem);
        //保存地图状态
        fwrite(&MapZhuangtai, sizeof(MapZhuangtai), 1, mem);
		//保存地图
		for (int i = 0; i < MapHang + 2; i++)
			for (int j = 0; j < MapLie + 2; j++)
				fwrite(&Map[i][j], sizeof(Map[i][j]), 1, mem);

		fclose(mem);
		int memres = MessageBoxA(0, "存档完成，是否退出", "连连看_天下英雄一起战斗吧", MB_OKCANCEL || MB_SYSTEMMODAL);
		if (memres == IDOK)//选择确定
		{
			closegraph();
			exit(0);
		}
		else if (memres == IDCANCEL)
			pause_flag = 1 - pause_flag;//游戏继续
	}
}
void Read_game()//读取游戏存档
{
	FILE  *mem;
	if ((mem = fopen("memory.txt", "r")) == NULL)
	{
		MessageBoxA(0, "没有存档，请重新选择", "连连看_天下英雄一起战斗吧", MB_OK || MB_SYSTEMMODAL);
		start_game();
	}
	else
	{
		//读取点击位置
		fread(&xt[0], sizeof(xt[0]), 1, mem);
		fread(&xt[1], sizeof(xt[1]), 1, mem);
		fread(&yt[0], sizeof(yt[0]), 1, mem);
		fread(&yt[1], sizeof(yt[1]), 1, mem);
		//读取点击次数
		fread(&presscount, sizeof(presscount), 1, mem);
		//读取剩余图片数
		fread(&remaining, sizeof(remaining), 1, mem);
		//读取剩余时间
		fread(&resist_time, sizeof(resist_time), 1, mem);
        //读取连击时间
		fread( &combo_time, sizeof(combo_time), 1, mem);
       //读取剩余洗牌次数
		fread(&xipaicishu, sizeof(xipaicishu), 1, mem);
		//读取剩余求助次数
		fread(&helpcount, sizeof(helpcount), 1, mem);
		//读取关卡数
		fread(&guan_count, sizeof(guan_count), 1, mem);
         //读取分数
		fread(&score, sizeof(score), 1, mem);
        //读取地图状态
        fread(&MapZhuangtai, sizeof(MapZhuangtai), 1, mem);
		//读取地图
		for (int i = 0; i < MapHang + 2; i++)
			for (int j = 0; j < MapLie + 2; j++)
				fread(&Map[i][j], sizeof(Map[i][j]), 1, mem);

		fclose(mem);
	}
}
int Modify_XY(double pos)//修正鼠标位置到图片中心
{
	return (int)(pos / Len_Image ) * Len_Image + Len_Image / 2;
}
void Get_Mouse()//获取鼠标事件
{
	if (MouseHit())
	{
		MOUSEMSG msg = GetMouseMsg();
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN://左键按下
			int loc_BUTTONDOWN[2] = {(Modify_XY(msg.x) - Len_Image / 2) / Len_Image, (Modify_XY(msg.y) - Len_Image / 2) / Len_Image };//重新保存下标

			if (Map[loc_BUTTONDOWN[0]][loc_BUTTONDOWN[1]] != 0)//所点击的位置不为空
			{
				fillrectangle(
					Modify_XY(msg.x) - Len_Image / 2, 
					Modify_XY(msg.y) - Len_Image / 2, 
					Modify_XY(msg.x) + Len_Image / 2,
					Modify_XY(msg.y) + Len_Image / 2);
				Sleep(30);//等待 屏幕显示
				presscount++;//计点击次数
				xt[presscount - 1] = Modify_XY(msg.x);//存储坐标
				yt[presscount - 1] = Modify_XY(msg.y);
				if (presscount == 2)//每点击两次 消除
				{		
					for (int i = 0; i < 2;i++)
						fillrectangle(
						xt[i] - Len_Image / 2,
						yt[i] - Len_Image / 2,
						xt[i] + Len_Image / 2,
						yt[i] + Len_Image / 2);
					presscount = 0;//重新计点击次数				
					delete_or_not(xt, yt);
					
				}
			}		
			break;
		}
	}
}
void Get_Keyboard()
{
    int saveres;
	if (_kbhit() != 0) //检查是否有键盘输入，若有则返回一个非0值，否则返回0  
	{
		while (_kbhit() != 0)  //可能存在多个按键,要全部取完,以最后一个为主  
			kb_key = _getch(); //将按键从控制台中取出并保存到key中

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)//按ESC存档	
		{
			int res = MessageBoxA(0, "是否要退出游戏？(若当前分数已超过排行榜最后一名将自动计入)", "连连看_天下英雄一起战斗吧", MB_YESNO | MB_SYSTEMMODAL);
		if (res == IDYES)
		{
			/*guan_count = 1;
			pause_flag = 0;
			restart_game();*/
			rank();
			closegraph();
			exit(0);
		}
		else
		{
		
		}
		}
		switch (kb_key)
		{
		case 32: //按下空格  给予提示
			if (helpcount)
			{
				Show_helpstep(kb_key);
				helpcount--;
			}
			break;
			case 8: //按下backspace  洗牌
			if (xipaicishu)
			{
				
					zhudongxipai();
				
				xipaicishu--;
			}
			break;
		case 9: //Tab  存档
			/*while (Find_step_solution())
			{
				Show_helpstep(kb_key);
			}*/ //只要有解就提示，好好玩游戏别作弊哦
            pause_flag = 1 - pause_flag;
		    saveres = MessageBoxA(0, "请选择是否存档", "连连看_天下英雄一起战斗吧", MB_OKCANCEL || MB_SYSTEMMODAL);
			if (saveres == IDOK)//选择确定
				Save_game();
			else if (saveres == IDCANCEL)
				pause_flag = 1 - pause_flag;
			break;
		default:
			break;
		}
	}
}
void delete_or_not(int xt[2], int yt[2])//执行判断消除操作
{
	int start[2] = { (xt[0] - Len_Image / 2) / Len_Image, 
		(yt[0] - Len_Image / 2) / Len_Image };//重新保存下标
	int end[2] = { (xt[1] - Len_Image / 2) / Len_Image, 
		(yt[1] - Len_Image / 2) / Len_Image };
	
	if (ThreeLines(start, end))//可以三条线以内消除	
	{
		DrawThreeLines(start, end);//画消除路径
		Sleep(100);//等待路径显示
		/*char c = Map[start[0]][start[1]];
		if(MapZhuangtai==IDYES)
		PlaySound(TEXT("音乐\\%c.wav"),NULL, SND_FILENAME | SND_ASYNC);*/ // 本来想做消除图案出三国杀武将语音包 如张角对应“雷公助我”，太复杂了后来就不想实现了……
		Map[start[0]][start[1]] = 0;//清除这两个位置
		Map[end[0]][end[1]] = 0;
		remaining -= 2;//剩余字母数减2	
		score+= 2;//无论如何分数都加二
		if (combo_time-resist_time<=2)
		{
			score += 2;      //如果有combo那就再加二

		}
		combo_time = resist_time;
		Element_bewegen();      //  消除后还要移动元素
		if(MapZhuangtai==IDYES)
			{
			ShowMapsgs();//显示地图	
			}
		if(MapZhuangtai==IDNO)
			{
			ShowMaplol();//显示地图	
			}
				
	}
}
int find_step(int x, int y)//0代表已无解
{
	int start[2] = { x, y };//重新保存下标
	for (int i = 1; i < MapHang + 1; i++)
	{
		for (int j = 1; j < MapLie + 1; j++)
		{
			int end[2] = { i, j };
			if (!(x == i&&y == j)
				&& Map[i][j] == Map[x][y]
				&& ThreeLines(start, end)
				&& Map[x][y] != 0)
				return 1;//有解
		}
	}
	return 0;
}
void Show_helpstep(int kb_key)//显示提示
{
	for (int x = 1; x < MapHang + 1; x++)
	{
		for (int y = 1; y < MapLie + 1; y++)
		{
			int start[2] = { x, y };//重新保存下标
			for (int i = 1; i < MapHang + 1; i++)
			{
				for (int j = 1; j < MapLie + 1; j++)
				{
					int end[2] = { i, j };
					if (!(x == i&&y == j)
						&& Map[i][j] == Map[x][y]
						&& ThreeLines(start, end)
						&& Map[x][y] != 0)
					{
						rectangle(
							i*Len_Image,
							j*Len_Image,
							(i + 1)*Len_Image,
							(j + 1)*Len_Image);
						rectangle(
							x*Len_Image,
							y*Len_Image,
							(x + 1)*Len_Image,
							(y + 1)*Len_Image);

						int start[2] = { i,j };//保存下标
						int end[2] = { x,y };
						DrawThreeLines(start, end);//把可以连的显示出来

						if (kb_key == 32)//如果是每关的空格键提示，那就提示0.3秒
						     Sleep(300);//等待路径显示//如果下方注释掉的话，可以延长提示时间。
						else  
							Sleep(5);//等待路径显示
    //***这里是开始位置
						Map[start[0]][start[1]] = 0;//既然提示了不如直接消除，省去手再去点的时间，节约时间得高分
						Map[end[0]][end[1]] = 0;
						remaining -= 2;//剩余字母数减2	
						score += 2;
		if (combo_time-resist_time<=2)
		{
			score += 2;

		}
		combo_time = resist_time;
						Element_bewegen();
    //***这里是结束位置。两个//***之间的部分如果注释掉，即可实现只提示不自动消除。
						if(MapZhuangtai == IDYES)
			{
			ShowMapsgs();//显示地图	
			}
		if(MapZhuangtai == IDNO)
			{
			ShowMaplol();//显示地图	
			}		
						return;
					}
				}
			}
		}
	}
}
int Find_step_solution()
{
	int flag = 0;
	for (int i = 1; i < MapHang + 1; i++)
		for (int j = 1; j < MapLie + 1; j++)
			if (find_step(i, j))//有解
				return 1;

	return 0;
}
void judge_game()
{
	if (!remaining)
	{
		pause_flag = 1;
		int res = MessageBoxA(0, "消除成功，是否开始下一关？\n是->进入下一关\n否->存排行榜，退出游戏", "连连看_天下英雄一起战斗吧", MB_YESNO | MB_SYSTEMMODAL);
		if (res == IDYES)
		{
			guan_count++;
            xipaicishu++;//此处洗牌次数是做成了每过一关加一，即每局只可主动洗牌一次，但没有洗的话可以保留带到下一局（反正洗牌也没啥大用，无解会自动洗）。
            if(guan_count<=10)
            {
                score = score+(guan_count-1)*20+(int)resist_time/2;   //每过一关奖励额外分数=剩余时间一半的分数+关卡*20
            }
            else
            {
                score = score+200+(int)resist_time/2;    //关卡大于10关后，全都向左下移动，故奖励分数均为200分+剩余时长的一半
            }
			Oninit_all();
			helpcount = 2;//提示次数则和洗牌次数不一样，每过一关恢复到两次，没用就浪费了，如果可累计的话怕太多了。
			if(MapZhuangtai == IDYES)
			{
			CreateMapsgs();//生成地图
			ShowMapsgs();//显示地图	
			}
			if(MapZhuangtai == IDNO)
			{
			CreateMaplol();//生成地图
			ShowMaplol();//显示地图	
			}
			pause_flag = 0;
			while (1)
			{
				Get_Mouse();
				Get_Keyboard();
				judge_game();//判断游戏是否结束
				Sleep(5);
			}
		}		
		else
		{
            rank();
			closegraph();
			exit(0);
		}
	}
	if (Find_step_solution() == 0)
	{
		pause_flag = 1;//时间暂停
		while (!Find_step_solution())	//洗牌出结果
		{
			for (int i = 0; i <= 2;i++)
				xipai();
		}
			
		if(MapZhuangtai==IDYES)
			{
			ShowMapsgs();//显示地图	
			}
		if(MapZhuangtai==IDNO)
			{
			ShowMaplol();//显示地图	
			}
	}
	if (resist_time <= 0)
	{
		int res = MessageBoxA(0, "时间到！游戏结束！分数已自动计入排行榜，是否继续游戏？\n（是->返回主菜单\n否->退出游戏)", "连连看_天下英雄一起战斗吧", MB_YESNO | MB_SYSTEMMODAL);
		if (res == IDYES)
		{
			
			rank();
            guan_count = 1;
			pause_flag = 0;
			restart_game();

		}
		else
		{
            rank();
			closegraph();
			exit(0);
		}
	}
}
void start_game()
{
    int res2;
	pause_flag = 1 - pause_flag;//游戏暂停
	int res = Msg(0, "请选择模式：\n开始新游戏->重新从第一关开始新游戏\n继续游戏->继续从上次存档位置开始游戏\n查看排行榜->排行榜记录了您最好的八次成绩\n游戏设置->本游戏没有需要设置的东西（无此按钮）\n退出游戏->请点击最右上角的叉叉", "连连看_游戏菜单", MB_ABORTRETRYIGNORE | MB_SYSTEMMODAL);
    
	if (res == IDIGNORE)//选择排行榜
	{
		readrank();		
		
	}
	else if(res == IDABORT)
	{
		res2 = Msg(0, "请选择新游戏模式：\n三国杀->滚滚长江东逝水，浪花淘尽英雄。将军您将回到东汉末年群雄并起的时代，招兵买马打出自己的天地。\n英雄联盟->这一切要从符文之地瓦洛兰大陆开始说起了……为了解决政治纷争与冲突，“英雄联盟”诞生了……化身召唤师战斗吧！\n", "连连看_开始新游戏", MB_YESNO | MB_SYSTEMMODAL);
			MapZhuangtai=res2;
        if(MapZhuangtai==IDYES)
        {
        for (int i = 0; i < 13; i++)
	{
		char s[20];
		sprintf(s, "图案\\%c.png", sgs[i]);
		loadimage(&sanguosha[i], s);
	}
		CreateMapsgs();//生成三国杀地图
    }
        else   if(MapZhuangtai==IDNO)
        {
            for (int i = 0; i < 13; i++)
	{
		char s[20];
		sprintf(s, "图案\\%c.png", lol[i]);
		loadimage(&leagueoflegend[i], s);
	}
		CreateMaplol();//生成lol地图
        }
	}
	else if(res == IDRETRY)
	{
		Read_game();//读档生成地图	
	}
	pause_flag = 1 - pause_flag;//游戏继续
	if(MapZhuangtai==IDYES)
			{
			ShowMapsgs();//显示三国杀地图	
			}
		if(MapZhuangtai==IDNO)
			{
			ShowMaplol();//显示lol地图	
			}	
	if(MapZhuangtai==IDYES)
	{
		PlaySound(TEXT("音乐\\三国杀主题曲.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // 三国杀bgm
	}
	if(MapZhuangtai==IDNO)
	{
		PlaySound(TEXT("音乐\\英雄联盟.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // legends never die
	}
	while (1)
	{		
		Get_Mouse();
		Get_Keyboard();
		judge_game();//判断游戏是否结束
		Sleep(5);
	}
}
void restart_game()
{
	Oninit_all();
	start_game();
}
void zhudongxipai()
{
			pause_flag = 1;
			char *arr;
			arr = (char *)malloc(remaining * sizeof(char));
			int k = 0;
			for(int i = 1; i <=14;i++)
			{
				for(int j=1;j <=10 ; j++)
				{
					if (Map[i][j]!= 0)
					{
						arr[k] = Map[i][j];
						k++;
					}
				}
			}
			k = 0;
			for(int j = 1;j<= 10 ; j++)
			{
				for(int i = 1; i <= 14 ;i++)
				{
					if (Map[i][j]!= 0)
					{
						Map[i][j] = arr[k];
						k++;
					}
				}
			}//先把所有图案都记录一下，再重新分配。

		pause_flag = 0;

		if(MapZhuangtai == IDYES)
			{
			ShowMapsgs();//显示地图	
			}
		if(MapZhuangtai == IDNO)
			{
			ShowMaplol();//显示地图	
			}
		free(arr);
		
}
int rank()
{
	cleardevice();
    int jigepaihangbang = 8;//要排行榜显示几个数，记得和readrank一起改，以及对话框中“排行榜记录了您最好的%d次成绩”
    int i, j, len = jigepaihangbang+1;
/*info tmp[10];
FILE *fp;
info cache;
info fenshuguanka;

int j,k;

fp=fopen("rank.txt","a+");
if(fp==NULL)                    
{  
	outtextxy((MapHang + 2)*Len_Image / 2, 100,"cannot open the files\n");  
	while(1)
		{
			PressKeyToRestart();
			Sleep(5);
		} 
	return -1;//如果文件出现错误返回-1  
}
fenshuguanka.fenshu=score;
fenshuguanka.guanshu=guan_count;
fwrite(&fenshuguanka, sizeof(fenshuguanka) , 1, fp );

while(i <=9&&fscanf(fp,"%d",tmp[i].fenshu)!=EOF&&fscanf(fp,"%d",tmp[i].guanshu)!=EOF)
{
    //fscanf(fp,"%d",tmp[i].fenshu);
    //fscanf(fp,"%d",tmp[i].guanshu);
	
	i++;
}
fread(&tmp,10,sizeof(tmp),fp);
n=i;
for (k = 0; k < i - 1; k++) {
        for ( j = 0; j < 10 - 1 - k; j++) {
            if (tmp[j].fenshu> tmp[j+1].fenshu) {        // 相邻元素两两对比
               cache = tmp[j+1];        // 元素交换
               tmp[j+1] = tmp[j];
                tmp[j] = cache;
            }
        }
}
if(n<10)
{}
else
{
if(score>=tmp[0].fenshu)
	{ 
        tmp[0]= fenshuguanka;
	}
}
*/
info tmp;
FILE *fp;

rank_guanka[jigepaihangbang] = guan_count;
rank_fenshu[jigepaihangbang] = score;//存一下当前信息

fp=fopen("rank.txt","a+");
if(fp==NULL)                    
      {  
         outtextxy((MapHang + 2)*Len_Image / 2, 100,"cannot open the files\n"); ;  
         while(1)
		{
			PressKeyToRestart();
			Sleep(5);
		} 
	    return -1;//如果文件出现错误返回-1    
       }
for(i = 0;i<jigepaihangbang;i++)
{
        fread(&rank_guanka[i], sizeof(rank_guanka[i]), 1, fp);
}
		for(i = 0;i<jigepaihangbang;i++)
{
        fread(&rank_fenshu[i], sizeof(rank_fenshu[i]), 1, fp);
}
        
	
        fclose(fp);
        
        int temp;
        for (i = 0; i < len - 1; i++) {
        for ( j = 0; j < len - 1 - i; j++) {
            if (rank_fenshu[j] < rank_fenshu[j+1]) {        // 相邻元素两两对比
                temp = rank_fenshu[j+1];        // 元素交换
                rank_fenshu[j+1] = rank_fenshu[j];
                rank_fenshu[j] = temp;
                 temp = rank_guanka[j+1];        // 元素交换
                rank_guanka[j+1] = rank_guanka[j];
                rank_guanka[j] = temp;
            }
        }
    }//与已有排行冒泡排序
        fp = fopen("rank.txt","w+");
        for( i = 0;i<jigepaihangbang;i++)
{
        fwrite(&rank_guanka[i], sizeof(rank_guanka[i]), 1, fp);
}
		for(i = 0;i<jigepaihangbang;i++)
{
        fwrite(&rank_fenshu[i], sizeof(rank_fenshu[i]), 1, fp);
}
       //分高者写入
fclose(fp);
return 0;

}
 
int readrank()
{
	cleardevice();
   /* info tmp[10];
	info temp;
	info cache;
	int i=0,j,k;
	FILE *fp;
	fp = fopen("rank.txt","a+");
 
	if(fp==NULL)              
	{  
		char str2[50];

		sprintf(str2, "无法打开文件");

		outtextxy((MapHang + 2)*Len_Image / 2, 80, str2);

        while(1)
		{
			PressKeyToRestart();
			Sleep(5);
		}

        return -1;  
    }
    while(i <=9&&fscanf(fp,"%d",tmp[i].fenshu)!=EOF&&fscanf(fp,"%d",tmp[i].guanshu)!=EOF)
{
    //fscanf(fp,"%d",tmp[i].fenshu);
    //fscanf(fp,"%d",tmp[i].guanshu);
	
	i++;
}
       fread(&tmp,10,sizeof(tmp),fp);
    k=i;
     for (i = 0; i < k - 1; i++) {
        for ( j = 0; j < k - 1 - i; j++) {
            if (tmp[j].fenshu< tmp[j+1].fenshu) {        // 相邻元素两两对比
               cache = tmp[j+1];        // 元素交换
               tmp[j+1] = tmp[j];
                tmp[j] = cache;
            }
        }
    }
	 j=0;
		for(i = 0;i <k; i++)
		{
			char str2[50];
            sprintf(str2, "第%d名： %d关%d分",i+1,tmp[j].guanshu,tmp[j].fenshu);
			outtextxy(80, 80+40*i, str2);
			j++;
		}
        	fclose(fp);
		while(1)
		{
			PressKeyToRestart();
			Sleep(5);
		}


        */
    FILE *fp;
    int jigepaihangbang = 8;//要排行榜显示几个数，记得和rank一起改，以及对话框中“排行榜记录了您最好的%d次成绩”
                    int i,j,len = jigepaihangbang;
        int temp;
	fp = fopen("rank.txt","r");
    if(fp==NULL)              
	{  
		char str2[50];

		sprintf(str2, "你的排行榜是空的呢！给你的源文件中有rank.txt，你是不是故意删掉了？");

		outtextxy((MapHang + 2)*Len_Image / 2, 80, str2);

        while(1)
		{
			PressKeyToRestart();
			Sleep(5);
		}

        return -1;  
    }
    for(i = 0;i<jigepaihangbang;i++)
{
        fread(&rank_guanka[i], sizeof(rank_guanka[i]), 1, fp);
}
		for(i = 0;i<jigepaihangbang;i++)
{
        fread(&rank_fenshu[i], sizeof(rank_fenshu[i]), 1, fp);
}
        

        for (i = 0; i < len - 1; i++) {
        for ( j = 0; j < len - 1 - i; j++) {
            if (rank_fenshu[j] < rank_fenshu[j+1]) {        // 相邻元素两两对比
                temp = rank_fenshu[j+1];        // 元素交换
                rank_fenshu[j+1] = rank_fenshu[j];
                rank_fenshu[j] = temp;
                 temp = rank_guanka[j+1];        // 元素交换
                rank_guanka[j+1] = rank_guanka[j];
                rank_guanka[j] = temp;
            }
        }
    }
       
        for(i = 0;i <jigepaihangbang; i++)
		{
			char str2[50];
            sprintf(str2, "第%d名： %d关%d分",i+1,rank_guanka[i],rank_fenshu[i]);
			outtextxy(80, 80+40*i, str2);
		
		}
        fclose(fp);
        while(1)
		{
			PressKeyToRestart();
			Sleep(5);
		} 

        
	return 0;

}
void PressKeyToRestart()
{
	    pause_flag = 1;
		char str3[50];
		sprintf(str3, "按任意键打开初始游戏菜单");

        outtextxy((MapHang + 2)*Len_Image / 2+Len_Image, 120, str3);
	int key;
	if (_kbhit() != 0) //检查是否有键盘输入，若有则返回一个非0值，否则返回0  
	{
		while (_kbhit() != 0)  //可能存在多个按键,要全部取完,以最后一个为主  
			key = _getch(); //将按键从控制台中取出并保存到key中
		if(key)
		{
			restart_game();
		}
	}
}