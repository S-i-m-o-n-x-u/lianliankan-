//2020年12月27日13:18:30 徐思远2019010372
#include "function_declaration.h"

int main()
{
	int kuan = Len_Image * (MapHang + 2) + 20;
	int gao = Len_Image * (MapLie + 2) + 20;
	initgraph(kuan,gao);

	HWND hwnd = GetHWnd(); 
	MoveWindow(hwnd, 0, 0, kuan, gao, 1);
	setwritemode(R2_XORPEN);	
	setfillcolor(WHITE);

	_beginthread(&Show_time, 0, 0);
	
start_game();
	

	system("pause");
	return 0;
}