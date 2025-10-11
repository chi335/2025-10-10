#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h> 

// *** ���� ����� ���� ��� �� ���̺귯�� �߰� ***
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") 

#define box_length 15 // ���� ���� (�¿� ����)
#define box_height 15 // �ٴ��� ���� (���� ����)

// --- Color Constants ---
#define COLOR_DEFAULT 7  // White
#define COLOR_BLOCK 10   // Light Green (�Ϲ� ���)
#define COLOR_FLOOR_BLOCK 2 // Dark Green (�ٴڿ� ���� ���)
#define COLOR_SHADOW 8   // Gray (������� ����)
#define COLOR_PERFECT 14 // Yellow
#define COLOR_TITLE 11   // Light Cyan
#define COLOR_END 12     // Light Red

// --- Global Variables ---
int block_stack[box_length * 2 + 1] = {0};
int score = 0; 
int perfect_count = 0; // PERFECT Ƚ�� ���

// --- Function Prototypes ---
void intro_game(void);
void game_control(void);
void gotoxy(int x, int y);
void set_color(int color);
int left_right_move(void);
void move_down(int x);
void draw_rectangle(int c, int r);
int max_block(void);
void end(void); 

// -------------------------------------------------------------------
// Main Function
// -------------------------------------------------------------------
int main(void)
{
	intro_game(); 
	game_control();
    end(); 
	
	return 0;
}

// -------------------------------------------------------------------
// Utility Functions
// -------------------------------------------------------------------
void gotoxy(int x, int y)
{
   COORD Pos = {(SHORT)(x - 1), (SHORT)(y - 1)};
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void set_color(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// -------------------------------------------------------------------
// Game Screen Functions
// -------------------------------------------------------------------

void intro_game(void)
{
	system("cls");
    
    // *** BGM ���� (bgm.wav ������ �ݺ� ���) ***
    PlaySound("bgm.wav", NULL, SND_ASYNC | SND_LOOP);
    
    char title[] = "BLOCK TOWER";
    set_color(COLOR_TITLE);
    gotoxy(15, 5); 
    for (int i = 0; title[i] != '\0'; i++) {
        printf("%c", title[i]);
        Sleep(80); 
    }
    
    set_color(COLOR_BLOCK);
    gotoxy(15, 7); printf("������"); Sleep(100);
    gotoxy(15, 8); printf(" ����"); Sleep(100);
    gotoxy(15, 9); printf("  ��"); Sleep(100);
    
    set_color(COLOR_DEFAULT);
    gotoxy(5, 12);
	printf("1. �����̽� Ű�� ����� ����߷� �ִ��� ����, ���������� ��������!");
    gotoxy(5, 13);
	printf("2. �̿� ��ϰ� ���̸� ���߸� PERFECT ���ʽ� ���� ȹ��!");
    
    // PERFECT ���� ����
    set_color(COLOR_PERFECT);
    gotoxy(5, 14);
	printf("   * PERFECT ����: �� �� ��ϰ� ���� ���̰� ��Ȯ�� ���ƾ� �մϴ�."); 

    set_color(COLOR_DEFAULT);
	gotoxy(15, 16); printf("�ƹ�Ű�� ������ ������ �����մϴ�.");
    set_color(COLOR_DEFAULT);
    
	getch();
}

void game_control(void)
{
	int x, count = 0;
	system("cls");
	draw_rectangle(box_length, box_height);
    
    // Initial display
	gotoxy(box_length * 2 + 5, 3); set_color(COLOR_DEFAULT);
	printf("��ǥ ��� ����: %2d", box_height);
	gotoxy(box_length * 2 + 5, 6); set_color(COLOR_DEFAULT);
	printf("���� ����: %4d", score);
    gotoxy(box_length * 2 + 5, 7); set_color(COLOR_PERFECT);
	printf("PERFECT Ƚ��: %2d", perfect_count);
    
	gotoxy(1, box_height + 3);
	printf("����� ���� ��ġ�� �����ϰ� �����ϼ���! \n");
    
	while(count < box_height)
	{
        // Update stats
		gotoxy(box_length * 2 + 5, 4); set_color(COLOR_DEFAULT);
		printf("�õ� Ƚ��: %2d / %2d", count + 1, box_height);
		gotoxy(box_length * 2 + 5, 5); set_color(COLOR_DEFAULT);
		printf("�ִ� ����: %2d", max_block());
		gotoxy(box_length * 2 + 5, 6); set_color(COLOR_DEFAULT);
		printf("���� ����: %4d", score);
        gotoxy(box_length * 2 + 5, 7); set_color(COLOR_PERFECT);
		printf("PERFECT Ƚ��: %2d", perfect_count);
        
        // 1. Horizontal Move & Drop
		x = left_right_move();
        
        // 2. Instant Drop (Tetris style)
		move_down(x);
        
        // 3. Score calculation
        int landing_y = box_height + 2 - block_stack[x];
        int bonus = 0;
        
        score += 10;
        
        if (block_stack[x] > 1) { 
            int current_height = block_stack[x];
            // �̿� ��ϰ� ���̰� ��Ȯ�� ��ġ�� �� PERFECT
            if (x > 1 && block_stack[x - 2] == current_height - 1) bonus += 5; 
            if (x < box_length * 2 + 1 && block_stack[x + 2] == current_height - 1) bonus += 5;
        }
        
        // Update score with bonus
        if (bonus > 0) { 
            score += bonus;
            perfect_count++; // PERFECT Ƚ�� ����
            
            // PERFECT �ؽ�Ʈ ��� �� ����� ������ ��� ���ŵǾ����ϴ�.
        }
        
		count++;
		getch(); // Wait for next key press
	}
}

void end(void)
{
    // *** BGM ���� ***
    PlaySound(NULL, NULL, 0); 
    
    system("cls");
    int max_h = max_block();
    
    // Display END text
    set_color(COLOR_END);
    // ����ϰ� --- GAME END --- ���
    gotoxy(15, 6); 
    printf("--- GAME END ---");
    
    // Draw Tower Status
    set_color(COLOR_BLOCK);
    gotoxy(10, 10); printf("�ִ� ����: %d ���", max_h);
    
    set_color(COLOR_PERFECT);
    gotoxy(10, 12); printf("���� ����: %d ��", score);
    
    set_color(COLOR_TITLE);
    gotoxy(10, 13); printf("PERFECT �޼� Ƚ��: %d ȸ", perfect_count);
    
    // Result Message 
    set_color(COLOR_DEFAULT);
    gotoxy(10, 16);
    if (max_h == box_height) {
        set_color(COLOR_TITLE);
        printf("�Ϻ��� �¸�! ��� ����� ���������� �׾ҽ��ϴ�!");
    } else if (max_h >= box_height / 2) {
        printf("���ϼ̽��ϴ�! ���� ž�� �׾ұ���.");
    } else {
        printf("�ƽ�����, ���� ������ �� ���� ž�� ���� �� ���� �ſ���!");
    }
    
    // Final Message
    set_color(COLOR_DEFAULT);
    gotoxy(10, 19);
    printf("������ �����Ϸ��� �ƹ� Ű�� ��������...");
    getch();
}

// -------------------------------------------------------------------
// Movement and Drawing Functions 
// -------------------------------------------------------------------

// left_right_move: �ܻ� ���� �¿� �̵�
int left_right_move(void)
{
	int x=3, y=2, temp=2;
    int x_prev = 0; 
    
	do
	{
        x_prev = x; // ���� ��ġ�� ���� ��ġ�� ����
        
        // 1. Update horizontal position
		x += temp; 
        
        // 2. Check and correct boundaries
		if (x > (box_length * 2)) 
		{
            x = box_length * 2; 
			temp = -2; 
		}
		if (x < 3) 
		{
			x = 3; 
			temp = 2; 
		}
        
        // 3. Erase the block at the *previous* position (�ܻ� ����)
        gotoxy(x_prev, y);
        printf("  ");
        
        // 4. Draw Moving Block at the new position
        set_color(COLOR_BLOCK);
		gotoxy(x, y);
		printf("��"); 
		Sleep(50);
        
	} while(!kbhit());
    
    // 5. ���� ����, �����̴� ����� �����ݴϴ�. 
    gotoxy(x, y);
    printf("  ");

	block_stack[x] += 1;
	return x;
}

// move_down: ��� ���� (��Ʈ���� ���)
void move_down(int x)
{
    // ���� ���� Y ��ǥ�� ���
    int landing_y = box_height + 2 - block_stack[x];
    
    // �ٴ� ���� ���� Ȯ�� �� ���� ����
    if (block_stack[x] == 1) { // �ش� ��ġ�� ���� ����� 1���� ��� = �ٴڿ� ó�� ���� ���
        set_color(COLOR_FLOOR_BLOCK); // �ٴ� ��� ���� ���� (Dark Green)
    } else {
        set_color(COLOR_BLOCK); // �ٸ� ��� ���� ���̴� ��� (Light Green)
    }
    
    // ���� ���� ��ġ�� ����� ��� ǥ��
	gotoxy(x, landing_y);
	printf("��");
    
    // Reset color for UI/next block
    set_color(COLOR_DEFAULT);
}

// -------------------------------------------------------------------
// draw_rectangle: ���� ���� �׵θ� �׸���
// -------------------------------------------------------------------
void draw_rectangle(int c, int r)
{
    int i, j;
    unsigned char a=0xa6;
    unsigned char b[7]; 
    for(i=1;i<7;i++)
   	b[i]=0xa0+i;

    set_color(COLOR_DEFAULT); // Set boundary color

    // Draw Top
    printf("%c%c",a, b[3]);
    for(i=0;i<c;i++)
  	printf("%c%c", a, b[1]);
    printf("%c%c", a, b[4]);
    printf("\n");
    
    // Draw Sides
    for(i=0;i<r;i++)
    {
	printf("%c%c", a, b[2]);
	for(j=0;j<c;j++)
		printf("  ");
	printf("%c%c",a, b[2]);
	printf("\n");
    }
    
    // Draw Bottom
    printf("%c%c", a, b[6]);
    for(i=0;i<c;i++)
	printf("%c%c", a, b[1]);
    printf("%c%c", a, b[5]);
    printf("\n");
}

int max_block(void)
{
	int i, max=0;
	for(i=1; i < box_length * 2 + 1; i++)
	{
		if (max <= block_stack[i])
			max=block_stack[i]; 
	}
	return max;
}
