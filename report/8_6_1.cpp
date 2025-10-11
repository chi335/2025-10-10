#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h> 

// *** 음악 재생을 위한 헤더 및 라이브러리 추가 ***
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") 

#define box_length 15 // 게임 영역 (좌우 길이)
#define box_height 15 // 바닥의 높이 (상하 길이)

// --- Color Constants ---
#define COLOR_DEFAULT 7  // White
#define COLOR_BLOCK 10   // Light Green (일반 블록)
#define COLOR_FLOOR_BLOCK 2 // Dark Green (바닥에 닿은 블록)
#define COLOR_SHADOW 8   // Gray (사용하지 않음)
#define COLOR_PERFECT 14 // Yellow
#define COLOR_TITLE 11   // Light Cyan
#define COLOR_END 12     // Light Red

// --- Global Variables ---
int block_stack[box_length * 2 + 1] = {0};
int score = 0; 
int perfect_count = 0; // PERFECT 횟수 기록

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
    
    // *** BGM 시작 (bgm.wav 파일을 반복 재생) ***
    PlaySound("bgm.wav", NULL, SND_ASYNC | SND_LOOP);
    
    char title[] = "BLOCK TOWER";
    set_color(COLOR_TITLE);
    gotoxy(15, 5); 
    for (int i = 0; title[i] != '\0'; i++) {
        printf("%c", title[i]);
        Sleep(80); 
    }
    
    set_color(COLOR_BLOCK);
    gotoxy(15, 7); printf("■■■■■"); Sleep(100);
    gotoxy(15, 8); printf(" ■■■"); Sleep(100);
    gotoxy(15, 9); printf("  ■"); Sleep(100);
    
    set_color(COLOR_DEFAULT);
    gotoxy(5, 12);
	printf("1. 스페이스 키로 블록을 떨어뜨려 최대한 높이, 안정적으로 쌓으세요!");
    gotoxy(5, 13);
	printf("2. 이웃 블록과 높이를 맞추면 PERFECT 보너스 점수 획득!");
    
    // PERFECT 조건 설명
    set_color(COLOR_PERFECT);
    gotoxy(5, 14);
	printf("   * PERFECT 조건: 양 옆 블록과 쌓인 높이가 정확히 같아야 합니다."); 

    set_color(COLOR_DEFAULT);
	gotoxy(15, 16); printf("아무키나 누르면 게임을 시작합니다.");
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
	printf("목표 블록 개수: %2d", box_height);
	gotoxy(box_length * 2 + 5, 6); set_color(COLOR_DEFAULT);
	printf("현재 점수: %4d", score);
    gotoxy(box_length * 2 + 5, 7); set_color(COLOR_PERFECT);
	printf("PERFECT 횟수: %2d", perfect_count);
    
	gotoxy(1, box_height + 3);
	printf("블록을 멈출 위치를 신중하게 선택하세요! \n");
    
	while(count < box_height)
	{
        // Update stats
		gotoxy(box_length * 2 + 5, 4); set_color(COLOR_DEFAULT);
		printf("시도 횟수: %2d / %2d", count + 1, box_height);
		gotoxy(box_length * 2 + 5, 5); set_color(COLOR_DEFAULT);
		printf("최대 높이: %2d", max_block());
		gotoxy(box_length * 2 + 5, 6); set_color(COLOR_DEFAULT);
		printf("현재 점수: %4d", score);
        gotoxy(box_length * 2 + 5, 7); set_color(COLOR_PERFECT);
		printf("PERFECT 횟수: %2d", perfect_count);
        
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
            // 이웃 블록과 높이가 정확히 일치할 때 PERFECT
            if (x > 1 && block_stack[x - 2] == current_height - 1) bonus += 5; 
            if (x < box_length * 2 + 1 && block_stack[x + 2] == current_height - 1) bonus += 5;
        }
        
        // Update score with bonus
        if (bonus > 0) { 
            score += bonus;
            perfect_count++; // PERFECT 횟수 증가
            
            // PERFECT 텍스트 출력 및 지우기 로직은 모두 제거되었습니다.
        }
        
		count++;
		getch(); // Wait for next key press
	}
}

void end(void)
{
    // *** BGM 정지 ***
    PlaySound(NULL, NULL, 0); 
    
    system("cls");
    int max_h = max_block();
    
    // Display END text
    set_color(COLOR_END);
    // 깔끔하게 --- GAME END --- 출력
    gotoxy(15, 6); 
    printf("--- GAME END ---");
    
    // Draw Tower Status
    set_color(COLOR_BLOCK);
    gotoxy(10, 10); printf("최대 높이: %d 블록", max_h);
    
    set_color(COLOR_PERFECT);
    gotoxy(10, 12); printf("최종 점수: %d 점", score);
    
    set_color(COLOR_TITLE);
    gotoxy(10, 13); printf("PERFECT 달성 횟수: %d 회", perfect_count);
    
    // Result Message 
    set_color(COLOR_DEFAULT);
    gotoxy(10, 16);
    if (max_h == box_height) {
        set_color(COLOR_TITLE);
        printf("완벽한 승리! 모든 블록을 성공적으로 쌓았습니다!");
    } else if (max_h >= box_height / 2) {
        printf("잘하셨습니다! 높은 탑을 쌓았군요.");
    } else {
        printf("아쉽지만, 다음 번에는 더 높은 탑을 쌓을 수 있을 거예요!");
    }
    
    // Final Message
    set_color(COLOR_DEFAULT);
    gotoxy(10, 19);
    printf("게임을 종료하려면 아무 키나 누르세요...");
    getch();
}

// -------------------------------------------------------------------
// Movement and Drawing Functions 
// -------------------------------------------------------------------

// left_right_move: 잔상 없이 좌우 이동
int left_right_move(void)
{
	int x=3, y=2, temp=2;
    int x_prev = 0; 
    
	do
	{
        x_prev = x; // 현재 위치를 이전 위치로 저장
        
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
        
        // 3. Erase the block at the *previous* position (잔상 방지)
        gotoxy(x_prev, y);
        printf("  ");
        
        // 4. Draw Moving Block at the new position
        set_color(COLOR_BLOCK);
		gotoxy(x, y);
		printf("■"); 
		Sleep(50);
        
	} while(!kbhit());
    
    // 5. 착지 직전, 움직이던 블록을 지워줍니다. 
    gotoxy(x, y);
    printf("  ");

	block_stack[x] += 1;
	return x;
}

// move_down: 즉시 착지 (테트리스 방식)
void move_down(int x)
{
    // 최종 착지 Y 좌표를 계산
    int landing_y = box_height + 2 - block_stack[x];
    
    // 바닥 착지 조건 확인 및 색상 변경
    if (block_stack[x] == 1) { // 해당 위치에 쌓인 블록이 1개일 경우 = 바닥에 처음 닿은 경우
        set_color(COLOR_FLOOR_BLOCK); // 바닥 블록 색상 적용 (Dark Green)
    } else {
        set_color(COLOR_BLOCK); // 다른 블록 위에 쌓이는 경우 (Light Green)
    }
    
    // 계산된 착지 위치에 블록을 즉시 표시
	gotoxy(x, landing_y);
	printf("■");
    
    // Reset color for UI/next block
    set_color(COLOR_DEFAULT);
}

// -------------------------------------------------------------------
// draw_rectangle: 게임 영역 테두리 그리기
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
