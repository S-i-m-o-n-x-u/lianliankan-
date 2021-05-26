//2020��12��27��18:47:34 ��˼Զ2019010372
#include "function_declaration.h"
#include "math.h"
#define Len_Image 80   //������ͼƬ�ı߳�
IMAGE sanguosha[13];//����ɱ�洢ͼ��
IMAGE leagueoflegend[13];//lol��ͼ����
int readrank();    //��ȡ�浵����ʾ
int rank();    //д��浵
void PressKeyToRestart();       //��������ص��˵���

typedef struct inf
{
    int guanshu;
	int fenshu;
}info;

int xt[2], yt[2];//�洢���λ��
int rank_guanka[9],rank_fenshu[9];

int MapZhuangtai; //��һ�ֵ�ͼ
char sgs[]="abcdefghijklm";// ��ͼ�е�����ɱ��Ԫ�أ���ĸ��
char lol[]="nopqrstuvwxyz";//��ͼ��lolԪ�أ����֣�
int score=0;    //����
int combo_time;     //����ʱ��

int presscount = 0;//�洢�������
int xipaicishu=1;//����ϴ�ƴ���
int remaining = MapLie * MapHang;//ʣ����ĸ��
int kb_key;//��������
int resist_time = 360;//��Ϸʣ��ʱ��
int helpcount = 2;//��ʾ�Ĵ���
int guan_count = 1;//��ǰ�ؿ���
int pause_flag = 0;//��Ϊ1˵����Ϸ��ͣ ����ʱ

char Map[MapHang + 2][MapLie + 2] = { 0 };//��ĸǰ��Ҫ����1��λ�ÿհ״��������ߣ����ϵĿ���ֱ��������

LRESULT CALLBACK CBTHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND)wParam;
	if (nCode == HCBT_ACTIVATE)
	{
		if (nCode == HCBT_ACTIVATE)
		{
			SetDlgItemText((HWND)wParam, IDYES, "&����ɱ");
			SetDlgItemText((HWND)wParam, IDNO, "&Ӣ������");
			SetDlgItemText((HWND)wParam, IDOK, "&OK");
			SetDlgItemText((HWND)wParam, IDCANCEL, "&Cancel");
			SetDlgItemText((HWND)wParam, IDABORT, "&��ʼ����Ϸ");
			SetDlgItemText((HWND)wParam, IDRETRY, "&������Ϸ");
			SetDlgItemText((HWND)wParam, IDIGNORE, "&�鿴���а�");

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

//�ж��������ܷ�����
int OneLine(int start[2], int end[2])//һ��������
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
}//ͬ�л�ͬ��������return1������return0
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
}//���ߣ�����OneLine���������������
int ThreeLines(int start[2], int end[2])
{
	if (Map[start[0]][start[1]] == Map[end[0]][end[1]]
		&& (!(start[0] == end[0] && start[1] == end[1])))
	{//�Ⱥ�����start end��ĸ��ͬ�Ҳ���ͬһ��λ��
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
}//����������ߺ�һ���ߵ����
//��������ʱ������·��
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
	{//�Ⱥ�����start end��ĸ��ͬ�Ҳ���ͬһ��λ��
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

//��ʼ�� ���ɲ���ʾ��ͼ
void Oninit_all()
{
	xt[0] = 0;//��յ��λ��
	xt[1] = 0;//��յ��λ��
	yt[0] = 0;//��յ��λ��
	yt[1] = 0;//��յ��λ��
	presscount = 0;//��յ������
	remaining = MapHang * MapLie;//����ʣ����ĸ��

	for (int i = 0; i < MapHang + 1; i++)//��յ�ͼ
		for (int j = 0; j < MapLie + 1; j++)
			Map[i][j] = 0;

	resist_time = 360 - (guan_count - 1) * 10;//��һ��360�룬ÿ��һ��ʱ���ʮ�룬����30s
	if (resist_time <= 30)
		resist_time = 30;	
}
void CreateMapsgs()//�����������ɱ��ͼ
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
	//Ԥ����ʮ�������ģ������

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
		
	}//����һ��������һ����ÿ�����λ�һ��ͼ����ȷ��ż����
}
void CreateMaplol()//�������lol��ͼ
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
	//Ԥ����ʮ��������

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
void ShowMapsgs()//��ʾ����ɱ��ͼ
{
	cleardevice();//����ɫ�����Ļ
	for (int i = 0; i < 13; i++)
	{
		char s[20];
		sprintf(s, "ͼ��\\%c.png", sgs[i]);
		loadimage(&sanguosha[i], s);
	}
	pause_flag = 0;//ˢ�µ�ͼʱʱ�䲻��ͣ
	for (int i = 1; i < MapHang + 1; i++)
		for (int j = 1; j < MapLie + 1; j++)
			if (Map[i][j] != 0 && Map[i][j] != '*')
			{
				int num = strlen(sgs) - strlen(strchr(sgs, Map[i][j]));
				putimage(i * Len_Image,j * Len_Image, &sanguosha[num]);
			}
}
void ShowMaplol()//��ʾlol��ͼ
{
	cleardevice();//����ɫ�����Ļ
	for (int i = 0; i < 13; i++)
	{
		char s[20];
		sprintf(s, "ͼ��\\%c.png", lol[i]);
		loadimage(&leagueoflegend[i], s);
	}
	pause_flag = 0;//ˢ�µ�ͼʱʱ�䲻��ͣ
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
	sprintf(str2, "����ϴ���С�����");
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
			if (Map[tx][ty] != 0)//��Ϊ��
			{
				x1[0] = tx;
				x1[1] = ty;
				flag = 1;
			}
		}
		else
		{
			if (Map[tx][ty] != 0 && Map[tx][ty] != Map[x1[0]][x1[1]])//��Ϊ��
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

}//����һ��
void Wohin_gehst_du(int Dir)//��Ҫȥ��ѽ���������֣��ƶ�Ԫ��
{
	switch (Dir)
	{
	case 2://��
		for (int i = 2; i < MapHang + 1; i++)
			for (int j = 1; j < MapLie + 1; j++)
				if (Map[i-1][j] == 0)//�����Ϊ��
				{
					Map[i-1][j] = Map[i][j];
					Map[i][j] = 0;
				}
		break;
	case 3://��
		for (int i = MapHang - 1; i > 0; i--)
			for (int j = 1; j < MapLie + 1; j++)
				if (Map[i+1][j] == 0) //�����Ϊ��
				{
					Map[i+1][j] = Map[i][j];
					Map[i][j] = 0;
				}
		break;
	case 4://��
		for (int i = 1; i < MapHang + 1; i++)
			for (int j = 2; j < MapLie + 1; j++)
				if (Map[i][j-1] == 0)//�����Ϊ��
				{
					Map[i][j-1] = Map[i][j];
					Map[i][j] = 0;
				}
		break;
	case 5://��
		for (int i = 1; i < MapHang + 1; i++)
			for (int j = MapLie - 1; j > 0; j--)
				if (Map[i][j+1] == 0)//�����Ϊ��
				{
					Map[i][j+1] = Map[i][j];
					Map[i][j] = 0;
				}
		break;
	}
}
void Element_bewegen()//�������������ƶ���ͼԪ��
{
	if (guan_count > 1)
	{
		for (int i = 1; i < MapHang - 1; i++)
			switch (guan_count)
			{
			case 2://��
				Wohin_gehst_du(4);
				break;
			case 3://��
				Wohin_gehst_du(5);
				break;
			case 4://��
				Wohin_gehst_du(2);
				break;
			case 5://��
				Wohin_gehst_du(3);
				break;
			case 6://����
				Wohin_gehst_du(2);
				Wohin_gehst_du(4);
				break;
			case 7://����
				Wohin_gehst_du(3);
				Wohin_gehst_du(4);
				break;
			case 8://����
				Wohin_gehst_du(2);
				Wohin_gehst_du(5);
				break;
			case 9://����
				Wohin_gehst_du(3);
				Wohin_gehst_du(5);
				break;
			default://�����������
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
		sprintf(str2, "���ո����ʾ��%d�Σ� ��backspace����ϴ�ƣ�%d�Σ���Tab�Ʊ��������Ϸ   ��ESC�˳���Ϸ    ʣ��ʱ��:%ds  �ؿ�:%d  �ܵ÷�:%d ", helpcount, xipaicishu, resist_time, guan_count,score);
		outtextxy(0, 0, str2);
		Sleep(1000);
		if (pause_flag == 0)
		{
			if (resist_time > 0)
				resist_time--;
		}
	}		
}
//��Ϸ�������жϽ��
void Save_game()//�浵
{
	char str[20];
	sprintf(str, "del memory.txt");
	system(str);//ɾ�����д浵
	FILE  *mem;
	if ((mem = fopen("memory.txt", "w+")) == NULL)//����ʧ���������Ϸ
	{
		MessageBoxA(0, "�浵ʧ��", "������_����Ӣ��һ��ս����", MB_OK || MB_SYSTEMMODAL);
		pause_flag = 1 - pause_flag;//��Ϸ����
	}
	else
	{
		//������λ��
		fwrite(&xt[0], sizeof(xt[0]), 1, mem);
		fwrite(&xt[1], sizeof(xt[1]), 1, mem);
		fwrite(&yt[0], sizeof(yt[0]), 1, mem);
		fwrite(&yt[1], sizeof(yt[1]), 1, mem);
		//����������
		fwrite(&presscount, sizeof(presscount), 1, mem);
		//����ʣ��ͼƬ��
		fwrite(&remaining, sizeof(remaining), 1, mem);
		//����ʣ��ʱ��
		fwrite(&resist_time, sizeof(resist_time), 1, mem);
        //��������ʱ��
		fwrite(&combo_time, sizeof(combo_time), 1, mem);
		//����ʣ����������
		fwrite(&helpcount, sizeof(helpcount), 1, mem);
        //����ʣ��ϴ�ƴ���
		fwrite(&xipaicishu, sizeof(xipaicishu), 1, mem);
		//����ؿ���
		fwrite(&guan_count, sizeof(guan_count), 1, mem);
        //�������
		fwrite(&score, sizeof(score), 1, mem);
        //�����ͼ״̬
        fwrite(&MapZhuangtai, sizeof(MapZhuangtai), 1, mem);
		//�����ͼ
		for (int i = 0; i < MapHang + 2; i++)
			for (int j = 0; j < MapLie + 2; j++)
				fwrite(&Map[i][j], sizeof(Map[i][j]), 1, mem);

		fclose(mem);
		int memres = MessageBoxA(0, "�浵��ɣ��Ƿ��˳�", "������_����Ӣ��һ��ս����", MB_OKCANCEL || MB_SYSTEMMODAL);
		if (memres == IDOK)//ѡ��ȷ��
		{
			closegraph();
			exit(0);
		}
		else if (memres == IDCANCEL)
			pause_flag = 1 - pause_flag;//��Ϸ����
	}
}
void Read_game()//��ȡ��Ϸ�浵
{
	FILE  *mem;
	if ((mem = fopen("memory.txt", "r")) == NULL)
	{
		MessageBoxA(0, "û�д浵��������ѡ��", "������_����Ӣ��һ��ս����", MB_OK || MB_SYSTEMMODAL);
		start_game();
	}
	else
	{
		//��ȡ���λ��
		fread(&xt[0], sizeof(xt[0]), 1, mem);
		fread(&xt[1], sizeof(xt[1]), 1, mem);
		fread(&yt[0], sizeof(yt[0]), 1, mem);
		fread(&yt[1], sizeof(yt[1]), 1, mem);
		//��ȡ�������
		fread(&presscount, sizeof(presscount), 1, mem);
		//��ȡʣ��ͼƬ��
		fread(&remaining, sizeof(remaining), 1, mem);
		//��ȡʣ��ʱ��
		fread(&resist_time, sizeof(resist_time), 1, mem);
        //��ȡ����ʱ��
		fread( &combo_time, sizeof(combo_time), 1, mem);
       //��ȡʣ��ϴ�ƴ���
		fread(&xipaicishu, sizeof(xipaicishu), 1, mem);
		//��ȡʣ����������
		fread(&helpcount, sizeof(helpcount), 1, mem);
		//��ȡ�ؿ���
		fread(&guan_count, sizeof(guan_count), 1, mem);
         //��ȡ����
		fread(&score, sizeof(score), 1, mem);
        //��ȡ��ͼ״̬
        fread(&MapZhuangtai, sizeof(MapZhuangtai), 1, mem);
		//��ȡ��ͼ
		for (int i = 0; i < MapHang + 2; i++)
			for (int j = 0; j < MapLie + 2; j++)
				fread(&Map[i][j], sizeof(Map[i][j]), 1, mem);

		fclose(mem);
	}
}
int Modify_XY(double pos)//�������λ�õ�ͼƬ����
{
	return (int)(pos / Len_Image ) * Len_Image + Len_Image / 2;
}
void Get_Mouse()//��ȡ����¼�
{
	if (MouseHit())
	{
		MOUSEMSG msg = GetMouseMsg();
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN://�������
			int loc_BUTTONDOWN[2] = {(Modify_XY(msg.x) - Len_Image / 2) / Len_Image, (Modify_XY(msg.y) - Len_Image / 2) / Len_Image };//���±����±�

			if (Map[loc_BUTTONDOWN[0]][loc_BUTTONDOWN[1]] != 0)//�������λ�ò�Ϊ��
			{
				fillrectangle(
					Modify_XY(msg.x) - Len_Image / 2, 
					Modify_XY(msg.y) - Len_Image / 2, 
					Modify_XY(msg.x) + Len_Image / 2,
					Modify_XY(msg.y) + Len_Image / 2);
				Sleep(30);//�ȴ� ��Ļ��ʾ
				presscount++;//�Ƶ������
				xt[presscount - 1] = Modify_XY(msg.x);//�洢����
				yt[presscount - 1] = Modify_XY(msg.y);
				if (presscount == 2)//ÿ������� ����
				{		
					for (int i = 0; i < 2;i++)
						fillrectangle(
						xt[i] - Len_Image / 2,
						yt[i] - Len_Image / 2,
						xt[i] + Len_Image / 2,
						yt[i] + Len_Image / 2);
					presscount = 0;//���¼Ƶ������				
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
	if (_kbhit() != 0) //����Ƿ��м������룬�����򷵻�һ����0ֵ�����򷵻�0  
	{
		while (_kbhit() != 0)  //���ܴ��ڶ������,Ҫȫ��ȡ��,�����һ��Ϊ��  
			kb_key = _getch(); //�������ӿ���̨��ȡ�������浽key��

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)//��ESC�浵	
		{
			int res = MessageBoxA(0, "�Ƿ�Ҫ�˳���Ϸ��(����ǰ�����ѳ������а����һ�����Զ�����)", "������_����Ӣ��һ��ս����", MB_YESNO | MB_SYSTEMMODAL);
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
		case 32: //���¿ո�  ������ʾ
			if (helpcount)
			{
				Show_helpstep(kb_key);
				helpcount--;
			}
			break;
			case 8: //����backspace  ϴ��
			if (xipaicishu)
			{
				
					zhudongxipai();
				
				xipaicishu--;
			}
			break;
		case 9: //Tab  �浵
			/*while (Find_step_solution())
			{
				Show_helpstep(kb_key);
			}*/ //ֻҪ�н����ʾ���ú�����Ϸ������Ŷ
            pause_flag = 1 - pause_flag;
		    saveres = MessageBoxA(0, "��ѡ���Ƿ�浵", "������_����Ӣ��һ��ս����", MB_OKCANCEL || MB_SYSTEMMODAL);
			if (saveres == IDOK)//ѡ��ȷ��
				Save_game();
			else if (saveres == IDCANCEL)
				pause_flag = 1 - pause_flag;
			break;
		default:
			break;
		}
	}
}
void delete_or_not(int xt[2], int yt[2])//ִ���ж���������
{
	int start[2] = { (xt[0] - Len_Image / 2) / Len_Image, 
		(yt[0] - Len_Image / 2) / Len_Image };//���±����±�
	int end[2] = { (xt[1] - Len_Image / 2) / Len_Image, 
		(yt[1] - Len_Image / 2) / Len_Image };
	
	if (ThreeLines(start, end))//������������������	
	{
		DrawThreeLines(start, end);//������·��
		Sleep(100);//�ȴ�·����ʾ
		/*char c = Map[start[0]][start[1]];
		if(MapZhuangtai==IDYES)
		PlaySound(TEXT("����\\%c.wav"),NULL, SND_FILENAME | SND_ASYNC);*/ // ������������ͼ��������ɱ�佫������ ���ŽǶ�Ӧ���׹����ҡ���̫�����˺����Ͳ���ʵ���ˡ���
		Map[start[0]][start[1]] = 0;//���������λ��
		Map[end[0]][end[1]] = 0;
		remaining -= 2;//ʣ����ĸ����2	
		score+= 2;//������η������Ӷ�
		if (combo_time-resist_time<=2)
		{
			score += 2;      //�����combo�Ǿ��ټӶ�

		}
		combo_time = resist_time;
		Element_bewegen();      //  ������Ҫ�ƶ�Ԫ��
		if(MapZhuangtai==IDYES)
			{
			ShowMapsgs();//��ʾ��ͼ	
			}
		if(MapZhuangtai==IDNO)
			{
			ShowMaplol();//��ʾ��ͼ	
			}
				
	}
}
int find_step(int x, int y)//0�������޽�
{
	int start[2] = { x, y };//���±����±�
	for (int i = 1; i < MapHang + 1; i++)
	{
		for (int j = 1; j < MapLie + 1; j++)
		{
			int end[2] = { i, j };
			if (!(x == i&&y == j)
				&& Map[i][j] == Map[x][y]
				&& ThreeLines(start, end)
				&& Map[x][y] != 0)
				return 1;//�н�
		}
	}
	return 0;
}
void Show_helpstep(int kb_key)//��ʾ��ʾ
{
	for (int x = 1; x < MapHang + 1; x++)
	{
		for (int y = 1; y < MapLie + 1; y++)
		{
			int start[2] = { x, y };//���±����±�
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

						int start[2] = { i,j };//�����±�
						int end[2] = { x,y };
						DrawThreeLines(start, end);//�ѿ���������ʾ����

						if (kb_key == 32)//�����ÿ�صĿո����ʾ���Ǿ���ʾ0.3��
						     Sleep(300);//�ȴ�·����ʾ//����·�ע�͵��Ļ��������ӳ���ʾʱ�䡣
						else  
							Sleep(5);//�ȴ�·����ʾ
    //***�����ǿ�ʼλ��
						Map[start[0]][start[1]] = 0;//��Ȼ��ʾ�˲���ֱ��������ʡȥ����ȥ���ʱ�䣬��Լʱ��ø߷�
						Map[end[0]][end[1]] = 0;
						remaining -= 2;//ʣ����ĸ����2	
						score += 2;
		if (combo_time-resist_time<=2)
		{
			score += 2;

		}
		combo_time = resist_time;
						Element_bewegen();
    //***�����ǽ���λ�á�����//***֮��Ĳ������ע�͵�������ʵ��ֻ��ʾ���Զ�������
						if(MapZhuangtai == IDYES)
			{
			ShowMapsgs();//��ʾ��ͼ	
			}
		if(MapZhuangtai == IDNO)
			{
			ShowMaplol();//��ʾ��ͼ	
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
			if (find_step(i, j))//�н�
				return 1;

	return 0;
}
void judge_game()
{
	if (!remaining)
	{
		pause_flag = 1;
		int res = MessageBoxA(0, "�����ɹ����Ƿ�ʼ��һ�أ�\n��->������һ��\n��->�����а��˳���Ϸ", "������_����Ӣ��һ��ս����", MB_YESNO | MB_SYSTEMMODAL);
		if (res == IDYES)
		{
			guan_count++;
            xipaicishu++;//�˴�ϴ�ƴ�����������ÿ��һ�ؼ�һ����ÿ��ֻ������ϴ��һ�Σ���û��ϴ�Ļ����Ա���������һ�֣�����ϴ��Ҳûɶ���ã��޽���Զ�ϴ����
            if(guan_count<=10)
            {
                score = score+(guan_count-1)*20+(int)resist_time/2;   //ÿ��һ�ؽ����������=ʣ��ʱ��һ��ķ���+�ؿ�*20
            }
            else
            {
                score = score+200+(int)resist_time/2;    //�ؿ�����10�غ�ȫ���������ƶ����ʽ���������Ϊ200��+ʣ��ʱ����һ��
            }
			Oninit_all();
			helpcount = 2;//��ʾ�������ϴ�ƴ�����һ����ÿ��һ�ػָ������Σ�û�þ��˷��ˣ�������ۼƵĻ���̫���ˡ�
			if(MapZhuangtai == IDYES)
			{
			CreateMapsgs();//���ɵ�ͼ
			ShowMapsgs();//��ʾ��ͼ	
			}
			if(MapZhuangtai == IDNO)
			{
			CreateMaplol();//���ɵ�ͼ
			ShowMaplol();//��ʾ��ͼ	
			}
			pause_flag = 0;
			while (1)
			{
				Get_Mouse();
				Get_Keyboard();
				judge_game();//�ж���Ϸ�Ƿ����
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
		pause_flag = 1;//ʱ����ͣ
		while (!Find_step_solution())	//ϴ�Ƴ����
		{
			for (int i = 0; i <= 2;i++)
				xipai();
		}
			
		if(MapZhuangtai==IDYES)
			{
			ShowMapsgs();//��ʾ��ͼ	
			}
		if(MapZhuangtai==IDNO)
			{
			ShowMaplol();//��ʾ��ͼ	
			}
	}
	if (resist_time <= 0)
	{
		int res = MessageBoxA(0, "ʱ�䵽����Ϸ�������������Զ��������а��Ƿ������Ϸ��\n����->�������˵�\n��->�˳���Ϸ)", "������_����Ӣ��һ��ս����", MB_YESNO | MB_SYSTEMMODAL);
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
	pause_flag = 1 - pause_flag;//��Ϸ��ͣ
	int res = Msg(0, "��ѡ��ģʽ��\n��ʼ����Ϸ->���´ӵ�һ�ؿ�ʼ����Ϸ\n������Ϸ->�������ϴδ浵λ�ÿ�ʼ��Ϸ\n�鿴���а�->���а��¼������õİ˴γɼ�\n��Ϸ����->����Ϸû����Ҫ���õĶ������޴˰�ť��\n�˳���Ϸ->���������ϽǵĲ��", "������_��Ϸ�˵�", MB_ABORTRETRYIGNORE | MB_SYSTEMMODAL);
    
	if (res == IDIGNORE)//ѡ�����а�
	{
		readrank();		
		
	}
	else if(res == IDABORT)
	{
		res2 = Msg(0, "��ѡ������Ϸģʽ��\n����ɱ->������������ˮ���˻��Ծ�Ӣ�ۡ����������ص�����ĩ��Ⱥ�۲����ʱ�����б��������Լ�����ء�\nӢ������->��һ��Ҫ�ӷ���֮����������½��ʼ˵���ˡ���Ϊ�˽�����η������ͻ����Ӣ�����ˡ������ˡ��������ٻ�ʦս���ɣ�\n", "������_��ʼ����Ϸ", MB_YESNO | MB_SYSTEMMODAL);
			MapZhuangtai=res2;
        if(MapZhuangtai==IDYES)
        {
        for (int i = 0; i < 13; i++)
	{
		char s[20];
		sprintf(s, "ͼ��\\%c.png", sgs[i]);
		loadimage(&sanguosha[i], s);
	}
		CreateMapsgs();//��������ɱ��ͼ
    }
        else   if(MapZhuangtai==IDNO)
        {
            for (int i = 0; i < 13; i++)
	{
		char s[20];
		sprintf(s, "ͼ��\\%c.png", lol[i]);
		loadimage(&leagueoflegend[i], s);
	}
		CreateMaplol();//����lol��ͼ
        }
	}
	else if(res == IDRETRY)
	{
		Read_game();//�������ɵ�ͼ	
	}
	pause_flag = 1 - pause_flag;//��Ϸ����
	if(MapZhuangtai==IDYES)
			{
			ShowMapsgs();//��ʾ����ɱ��ͼ	
			}
		if(MapZhuangtai==IDNO)
			{
			ShowMaplol();//��ʾlol��ͼ	
			}	
	if(MapZhuangtai==IDYES)
	{
		PlaySound(TEXT("����\\����ɱ������.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // ����ɱbgm
	}
	if(MapZhuangtai==IDNO)
	{
		PlaySound(TEXT("����\\Ӣ������.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // legends never die
	}
	while (1)
	{		
		Get_Mouse();
		Get_Keyboard();
		judge_game();//�ж���Ϸ�Ƿ����
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
			}//�Ȱ�����ͼ������¼һ�£������·��䡣

		pause_flag = 0;

		if(MapZhuangtai == IDYES)
			{
			ShowMapsgs();//��ʾ��ͼ	
			}
		if(MapZhuangtai == IDNO)
			{
			ShowMaplol();//��ʾ��ͼ	
			}
		free(arr);
		
}
int rank()
{
	cleardevice();
    int jigepaihangbang = 8;//Ҫ���а���ʾ���������ǵú�readrankһ��ģ��Լ��Ի����С����а��¼������õ�%d�γɼ���
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
	return -1;//����ļ����ִ��󷵻�-1  
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
            if (tmp[j].fenshu> tmp[j+1].fenshu) {        // ����Ԫ�������Ա�
               cache = tmp[j+1];        // Ԫ�ؽ���
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
rank_fenshu[jigepaihangbang] = score;//��һ�µ�ǰ��Ϣ

fp=fopen("rank.txt","a+");
if(fp==NULL)                    
      {  
         outtextxy((MapHang + 2)*Len_Image / 2, 100,"cannot open the files\n"); ;  
         while(1)
		{
			PressKeyToRestart();
			Sleep(5);
		} 
	    return -1;//����ļ����ִ��󷵻�-1    
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
            if (rank_fenshu[j] < rank_fenshu[j+1]) {        // ����Ԫ�������Ա�
                temp = rank_fenshu[j+1];        // Ԫ�ؽ���
                rank_fenshu[j+1] = rank_fenshu[j];
                rank_fenshu[j] = temp;
                 temp = rank_guanka[j+1];        // Ԫ�ؽ���
                rank_guanka[j+1] = rank_guanka[j];
                rank_guanka[j] = temp;
            }
        }
    }//����������ð������
        fp = fopen("rank.txt","w+");
        for( i = 0;i<jigepaihangbang;i++)
{
        fwrite(&rank_guanka[i], sizeof(rank_guanka[i]), 1, fp);
}
		for(i = 0;i<jigepaihangbang;i++)
{
        fwrite(&rank_fenshu[i], sizeof(rank_fenshu[i]), 1, fp);
}
       //�ָ���д��
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

		sprintf(str2, "�޷����ļ�");

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
            if (tmp[j].fenshu< tmp[j+1].fenshu) {        // ����Ԫ�������Ա�
               cache = tmp[j+1];        // Ԫ�ؽ���
               tmp[j+1] = tmp[j];
                tmp[j] = cache;
            }
        }
    }
	 j=0;
		for(i = 0;i <k; i++)
		{
			char str2[50];
            sprintf(str2, "��%d���� %d��%d��",i+1,tmp[j].guanshu,tmp[j].fenshu);
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
    int jigepaihangbang = 8;//Ҫ���а���ʾ���������ǵú�rankһ��ģ��Լ��Ի����С����а��¼������õ�%d�γɼ���
                    int i,j,len = jigepaihangbang;
        int temp;
	fp = fopen("rank.txt","r");
    if(fp==NULL)              
	{  
		char str2[50];

		sprintf(str2, "������а��ǿյ��أ������Դ�ļ�����rank.txt�����ǲ��ǹ���ɾ���ˣ�");

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
            if (rank_fenshu[j] < rank_fenshu[j+1]) {        // ����Ԫ�������Ա�
                temp = rank_fenshu[j+1];        // Ԫ�ؽ���
                rank_fenshu[j+1] = rank_fenshu[j];
                rank_fenshu[j] = temp;
                 temp = rank_guanka[j+1];        // Ԫ�ؽ���
                rank_guanka[j+1] = rank_guanka[j];
                rank_guanka[j] = temp;
            }
        }
    }
       
        for(i = 0;i <jigepaihangbang; i++)
		{
			char str2[50];
            sprintf(str2, "��%d���� %d��%d��",i+1,rank_guanka[i],rank_fenshu[i]);
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
		sprintf(str3, "��������򿪳�ʼ��Ϸ�˵�");

        outtextxy((MapHang + 2)*Len_Image / 2+Len_Image, 120, str3);
	int key;
	if (_kbhit() != 0) //����Ƿ��м������룬�����򷵻�һ����0ֵ�����򷵻�0  
	{
		while (_kbhit() != 0)  //���ܴ��ڶ������,Ҫȫ��ȡ��,�����һ��Ϊ��  
			key = _getch(); //�������ӿ���̨��ȡ�������浽key��
		if(key)
		{
			restart_game();
		}
	}
}