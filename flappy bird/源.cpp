#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<time.h>

IMAGE bk;
IMAGE bk1;
IMAGE BK[2];
IMAGE BIRD[3][3];
IMAGE RBIRD;
IMAGE PIPE[2][2];
IMAGE NUM[10];
IMAGE TITLE;
IMAGE BUTTON_START;
IMAGE LAND;
IMAGE GAME_OVER;
IMAGE TEMP;
IMAGE SCORE_PANLE;
IMAGE SNUM[10];
IMAGE TUTOR;
IMAGE AU;
IMAGE AG;
IMAGE CU;
IMAGE MEDAL;
IMAGE NEW;

int birdx = 0;
int isday = 0;
int birdtype = 0;
int birdpose = 0;
int prepose = birdpose;
int landx = -48;
int vx = 11;
int pipex[2] = { 0 };
int pipey[2] = { 0 };
float y = 100;
float g = 9.2;
float vy = 0;
int top = 14;
int bottom = 35;
int left = 20;
int right = 35;
int pipeup = -280;
int pipedown = 140;
clock_t s_t;

void Init();
int getcontral();
int die();
void flush();
void draw();
void move(long);
void drawScore(int y, int score);
int score = 0;
int best = 0;

void load()
{
	loadimage(&bk, _T("res\\bg_day.png"));
	loadimage(&bk1, _T("res\\bg_day.png"));

	loadimage(&BK[0], _T("res\\bg_night.png"));
	loadimage(&BK[1], _T("res\\bg_day.png"));

	loadimage(&RBIRD, _T("res\\BIRD0_0.png"));
	loadimage(&BIRD[0][0], _T("res\\BIRD0_0.png"));
	loadimage(&BIRD[0][1], _T("res\\BIRD0_1.png"));
	loadimage(&BIRD[0][2], _T("res\\BIRD0_2.png"));
	loadimage(&BIRD[1][0], _T("res\\BIRD1_0.png"));
	loadimage(&BIRD[1][1], _T("res\\BIRD1_1.png"));
	loadimage(&BIRD[1][2], _T("res\\BIRD1_2.png"));
	loadimage(&BIRD[2][0], _T("res\\BIRD2_0.png"));
	loadimage(&BIRD[2][1], _T("res\\BIRD2_1.png"));
	loadimage(&BIRD[2][2], _T("res\\BIRD2_2.png"));

	loadimage(&PIPE[0][0], _T("res\\PIPE_down.png"));
	loadimage(&PIPE[0][1], _T("res\\PIPE_up.png"));
	loadimage(&PIPE[1][0], _T("res\\PIPE2_down.png"));
	loadimage(&PIPE[1][1], _T("res\\PIPE2_up.png"));

	loadimage(&NUM[0], _T("res\\font_048.png"));
	loadimage(&NUM[1], _T("res\\font_049.png"));
	loadimage(&NUM[2], _T("res\\font_050.png"));
	loadimage(&NUM[3], _T("res\\font_051.png"));
	loadimage(&NUM[4], _T("res\\font_052.png"));
	loadimage(&NUM[5], _T("res\\font_053.png"));
	loadimage(&NUM[6], _T("res\\font_054.png"));
	loadimage(&NUM[7], _T("res\\font_055.png"));
	loadimage(&NUM[8], _T("res\\font_056.png"));
	loadimage(&NUM[9], _T("res\\font_057.png"));

	loadimage(&SNUM[0], _T("res\\number_score_00.png"));
	loadimage(&SNUM[1], _T("res\\number_score_01.png"));
	loadimage(&SNUM[2], _T("res\\number_score_02.png"));
	loadimage(&SNUM[3], _T("res\\number_score_03.png"));
	loadimage(&SNUM[4], _T("res\\number_score_04.png"));
	loadimage(&SNUM[5], _T("res\\number_score_05.png"));
	loadimage(&SNUM[6], _T("res\\number_score_06.png"));
	loadimage(&SNUM[7], _T("res\\number_score_07.png"));
	loadimage(&SNUM[8], _T("res\\number_score_08.png"));
	loadimage(&SNUM[9], _T("res\\number_score_09.png"));
	loadimage(&TITLE, _T("res\\title.png"));
	loadimage(&BUTTON_START, _T("res\\button_play.png"));
	loadimage(&LAND, _T("res\\land.png"));

	loadimage(&GAME_OVER, _T("res\\text_game_over.png"));
	loadimage(&SCORE_PANLE, _T("res\\score_panel.png"));
	loadimage(&TUTOR, _T("res\\tutorial.png"));

	loadimage(&AU, _T("res\\medals_3.png"));
	loadimage(&AG, _T("res\\medals_2.png"));
	loadimage(&CU, _T("res\\medals_1.png"));
	loadimage(&MEDAL, _T("res\\medals_0.png"));

	loadimage(&NEW, _T("res\\new.png"));

}

void drawAlpha(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
	if (dstimg == NULL)
	{
		return;
	}
	// 变量初始化
	DWORD* dst = GetImageBuffer(dstimg);
	DWORD* src = GetImageBuffer(srcimg);
	int src_width = srcimg->getwidth();
	int src_height = srcimg->getheight();
	int dst_width = dstimg->getwidth();
	int dst_height = dstimg->getheight();

	// 实现透明贴图  可优化
	for (int iy = 0; iy < src_height; iy++)
	{
		for (int ix = 0; ix < src_width; ix++)
		{
			int srcX = ix + iy * src_width;
			int sa = ((src[srcX] & 0xff000000) >> 24);
			int sr = ((src[srcX] & 0xff0000) >> 16);
			int sg = ((src[srcX] & 0xff00) >> 8);
			int sb = src[srcX] & 0xff;
			if (x + ix >= 0 && x + ix < dst_width
				&& y + iy >= 0 && y + iy < dst_height)
			{
				int dstX = (x + ix) + (y + iy) * dst_width;
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				dst[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)
					| (sb * sa / 255 + db * (255 - sa) / 255);
			}
		}
	}
}

void begin()
{
	Init();
	clock_t t = clock();
	while (1)
	{

		BeginBatchDraw();
		drawAlpha(&bk, 0, 0, &BK[isday]);
		drawAlpha(&bk, BK[0].getwidth() / 2 - BIRD[0][0].getwidth() / 2, 100, &BIRD[birdtype][birdpose]);
		drawAlpha(&bk, 288 / 2 - 178 / 2, 180, &TITLE);
		drawAlpha(&bk, 288 / 2 - 116 / 2, 250, &BUTTON_START);
		do {
			birdpose = ((clock() - s_t) / 110) % 3;
		} while (birdpose == prepose);
		prepose = birdpose;
		FlushBatchDraw();
		putimage(0, 0, &bk);
		if (getcontral())
			break;
		Sleep(10);
	}
}

void Init()
{
	y = 100;
	vy = 0;
	score = 0;
	birdx = 27;
	srand((unsigned int)time(NULL));
	initgraph(288, 512);
	load();
	isday = rand() % 2;
	birdtype = rand() % 3;
	pipex[0] = 200;
	pipex[1] = 200 + 52 + 120;
	pipey[0] = rand() % 250;
	pipey[1] = rand() % 250;
}

void start()
{
	Init();
	drawAlpha(&bk, 0, 0, &BK[isday]);              
	s_t = clock();
	while (1)
	{
		BeginBatchDraw();
		drawAlpha(&bk, 0, 0, &BK[isday]);
		drawAlpha(&bk, 27, (int)y, &BIRD[birdtype][birdpose]);
		drawAlpha(&bk, 0, 170, &TUTOR);
		drawAlpha(&bk, pipex[0], pipey[0] + pipeup, &PIPE[!isday][0]);
		drawAlpha(&bk, pipex[0], pipey[0] + pipedown, &PIPE[!isday][1]);
		drawAlpha(&bk, landx, BK[0].getheight() - LAND.getheight(), &LAND);
		do {
			birdpose = ((clock() - s_t) / 150) % 3;
		} while (birdpose == prepose);
		prepose = birdpose;
		putimage(0, 0, &bk);
		FlushBatchDraw();
		if (getcontral())
			break;
		Sleep(20);
	}
}

void game()
{
	clock_t t = clock();
	flush();
	while (!die())
	{
		move(clock() - t);
		t = clock();

		if (getcontral())
			vy = -26;
		draw(); 
		Sleep(10);
	}
	clock_t e_t = clock();
	vy = -10;
	while (clock() - e_t < 1000);
	vy = -30;
	e_t = clock();
	while (y < 360)
	{
		y += (clock() - e_t) * vy / 100;
		vy += g * (clock() - e_t) / 100;
		e_t = clock();
		draw();
		Sleep(10);
	}
}

int die()
{
	if (y + bottom > 512 - 112)
		return 1;

	if (birdx + right > pipex[0] && birdx + left < pipex[0] + 52)
	{
		if (y + top < pipey[0] + 40 || y + bottom > pipey[0] + 140)
			return 1;
	}

	if (birdx + right > pipex[1] && birdx + left < pipex[1] + 52)
	{
		if (y + top < pipey[1] + 40 || y + bottom > pipey[1] + 140)
			return 1;
	}

	return 0;
}

void flush()
{
	birdx = 288 / 2 - 120;
	drawAlpha(&bk, 0, 0, &BK[isday]);
}

void draw()
{
	BeginBatchDraw();
	drawAlpha(&bk, 0, 0, &BK[isday]);


	drawAlpha(&bk, pipex[0], pipey[0] + pipeup, &PIPE[!isday][0]);
	drawAlpha(&bk, pipex[0], pipey[0] + pipedown, &PIPE[!isday][1]);

	drawAlpha(&bk, pipex[1], pipey[1] + pipeup, &PIPE[!isday][0]);
	drawAlpha(&bk, pipex[1], pipey[1] + pipedown, &PIPE[!isday][1]);

	drawAlpha(&bk, landx, BK[0].getheight() - LAND.getheight(), &LAND);
	drawAlpha(&bk, birdx, (int)y, &BIRD[birdtype][birdpose]);
	drawScore(60, score);
	putimage(0, 0, &bk);
	FlushBatchDraw();

}

void move(long t)
{
	y += t * vy / 100;
	vy += g * t / 100;
	pipex[0] -= t * vx / 100;
	pipex[1] -= t * vx / 100;
	if (pipex[0] < -52)
	{
		pipex[0] = 278;
		pipey[0] = rand() % 250;
	}
	if (pipex[1] < -52)
	{
		pipex[1] = 278;
		pipey[1] = rand() % 250;
	}
	landx -= t * vx / 100;
	birdpose = ((clock() - s_t) / 100) % 3;


	if (landx < -44)
		landx = -20;
	if (birdx == pipex[0] || birdx == pipex[1])
		score++;
}

void drawScore(int y, int score)
{
	BeginBatchDraw();
	if (score == 0)
	{
		drawAlpha(&bk, BK[0].getwidth() / 2 - 24 / 2, 60, &NUM[0]);
	}
	int num[9], digit = 0;
	while (score)
	{
		num[digit++] = score % 10;
		score /= 10;
	}
	for (int i = digit - 1; i >= 0; i--)
	{
		drawAlpha(&bk, BK[0].getwidth() / 2 - i * 24, 60, &NUM[num[i]]);
	}
	FlushBatchDraw();
}

void drawMedals(int lever)
{
	if (lever == 0)
		drawAlpha(&bk, 58, 245, &MEDAL);
	if (lever == 1)
		drawAlpha(&bk, 58, 245, &CU);
	if (lever == 2)
		drawAlpha(&bk, 58, 245, &AG);
	if (lever >= 3)
		drawAlpha(&bk, 58, 245, &AU);

}

void end()
{
	BeginBatchDraw();
	int lever = (score - 20) / 20;
	drawAlpha(&bk, BK[0].getwidth() / 2 - GAME_OVER.getwidth() / 2, 120, &GAME_OVER);
	drawAlpha(&bk, BK[0].getwidth() / 2 - SCORE_PANLE.getwidth() / 2, 200, &SCORE_PANLE);
	drawMedals(lever);
	if (score > best)
	{
		drawAlpha(&bk, 150, 275, &NEW);
		best = score;
	}
	if (score == 0)
		drawAlpha(&bk, 200, 235, &SNUM[0]);
	if (best == 0)
		drawAlpha(&bk, 200, 275, &SNUM[0]);
	int digit = 0, num[9], temp = score;

	while (temp)
	{
		num[digit++] = temp % 10;
		temp /= 10;
	}
	for (int i = digit - 1; i >= 0; i--)
		drawAlpha(&bk, 200 - i * 16, 235, &SNUM[num[i]]);
	int digit1 = 0, num1[9], temp1 = best;
	while (temp1)
	{
		num1[digit1++] = temp1 % 10;
		temp1 /= 10;
	}
	for (int i = digit1 - 1; i >= 0; i--)
		drawAlpha(&bk, 200 - i * 16, 275, &SNUM[num1[i]]);
	putimage(0, 0, &bk);
	FlushBatchDraw();
	system("pause");
}
int getcontral()
{
	bool res = false;

	if (_kbhit())
	{
		char ch = _getch();
		if (ch == ' ')
		{
			res = true;
		}
	}

	MOUSEMSG msg;
	while (MouseHit())
	{
		msg = GetMouseMsg();
		if (msg.mkLButton)
		{
			res = true;
		}
	}

	return res;
}

int main()
{
	begin();
	while (1)
	{
		start();
		game();
		end();
	}
	return 0;
}
