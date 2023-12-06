#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <process.h>

// 전역 변수로 이벤트 핸들 선언
HANDLE g_hExitEvent;

// 맵 크기는 가로 120칸, 세로 300줄
#define MAP_SIZE_ROW 300
#define MAP_SIZE_COL 120
#define LIFE 3 // 기본 목숨

// 전역 변수 설정
int row_pos = 55; // 초기 위치
int life = LIFE; // 목숨
int score = 0; // 점수
int SPEED = 300; // 게임 속도(낮을수록 어려움)
int char_color = 15; // 캐릭터 비행기 색상

// 뮤텍스 선언
HANDLE mutex;

// 콘솔창에서 출력 위치 변경하는 함수
void gotoxy(int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 콘솔창 출력 색상 변경 함수
void textcolor(int colorNum)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

// 색상 열거형
enum ColorType
{
    BLACK,       // 0
    darkBLUE,    // 1
    DarkGreen,   // 2
    darkSkyBlue, // 3
    DarkRed,     // 4
    DarkPurple,  // 5
    DarkYellow,  // 6
    GRAY,        // 7
    DarkGray,    // 8
    BLUE,        // 9
    GREEN,       // 10
    SkyBlue,     // 11
    RED,         // 12
    PURPLE,      // 13
    YELLOW,      // 14
    WHITE        // 15
} COLOR;

// 문자열을 말 하는 것 처럼 출력하는 함수
void sys_print(char* print)
{
    Sleep(200);
    int num;
    num = strlen(print);
    for (int i = 0; i < num; i++)
    {
        printf("%c", print[i]);
        Sleep(50);
    }
    Sleep(100);
}

// 게임 로고 출력
void game_logo() {
    textcolor(12);
    puts("=============================================================================================");
    textcolor(13);
    puts("______  _      _____  _____  _   _  _____   _____ __   ________ ______  _____  _____  _____");
    textcolor(14);
    puts("|  ___|| |    |_   _||  __ \\| | | ||_   _| |  ___|\\ \\ / /| ___ \\| ___ \\|  ___|/  ___|/  ___|");
    textcolor(15);
    puts("| |_   | |      | |  | |  \\/| |_| |  | |   | |__   \\ V / | |_/ /| |_/ /| |__  \\ `--. \\ `--.");
    textcolor(11);
    puts("|  _|  | |      | |  | | __ |  _  |  | |   |  __|  /   \\ |  __/ |    / |  __|  `--. \\ `--. \\");
    textcolor(10);
    puts("| |    | |____ _| |_ | |_) || | | |  | |   | |___ / /^\\ \\| |    | |\\ \\ | |___ /\\__/ / \\__/ /");
    textcolor(9);
    puts("\\_|    \\_____/ \\___/ \\__ __/\\_| |_/  \\_/   \\____/ \\/   \\/\\_|    \\_| \\_|\\____/ \\____/ \\____/");
    textcolor(4);
    puts("=============================================================================================");
    textcolor(7);
}

// 게임오버 화면 출력
void game_over()
{
    system("cls");
    for (int i = 0; i < 15; i++) {
        textcolor(i);
        Sleep(150);
        gotoxy(0, 0);
        puts("\t\t _____   ___  ___  ___ _____   _____  _   _  _____ ______   _   _ ");
        puts("\t\t|  __ \\ / _ \\ |  \\/  ||  ___| |  _  || | | ||  ___|| ___ \\ | | | |");
        puts("\t\t| |  \\// /_\\ \\| .  . || |__   | | | || | | || |__  | |_/ / | | | |");
        puts("\t\t| | __ |  _  || |\\/| ||  __|  | | | || | | ||  __| |    /  | | | |");
        puts("\t\t| |_\\ \\| | | || |  | || |___  \\ \\_/ /\\ \\_/ /| |___ | |\\ \\  |_| |_|");
        puts("\t\t \\____/\\_| |_/\\_|  |_/\\____/   \\___/  \\___/ \\____/ \\_| \\_| (_) (_)");
    }
    gotoxy(20, 10);
    sys_print("Press any key");
    textcolor(7);
    _getch();
    _getch();
}

// 게임 메뉴 함수
int game_menu()
{
    game_logo();
    gotoxy(40, 10);
    puts("===========");
    gotoxy(40, 11);
    puts("[1]START");
    gotoxy(40, 12);
    puts("[2]Guide");
    gotoxy(40, 13);
    puts("[3]Settings");
    gotoxy(40, 14);
    puts("[4]Credit");
    gotoxy(40, 15);
    puts("===========");

    int select_menu = _getche();

    return select_menu;
}

// 캐릭터 출력 및 조종 함수
void game_char(char** map)
{
    while (1)
    {
        char input = _getch();
        Sleep(1);
        if ((input == 'a') && (row_pos > 2))
        {
            WaitForSingleObject(mutex, INFINITE); // 뮤텍스 락
            row_pos -= 2;
            ReleaseMutex(mutex); // 뮤텍스 언락
        }
        else if (input == 'd' && (row_pos < 114))
        {
            WaitForSingleObject(mutex, INFINITE); // 뮤텍스 락
            row_pos += 2;
            ReleaseMutex(mutex); // 뮤텍스 언락
        }
        else if (input == 'w' && (char_color < 15))
        {
            WaitForSingleObject(mutex, INFINITE); // 뮤텍스 락
            char_color++;
            ReleaseMutex(mutex); // 뮤텍스 언락
        }
        else if (input == 's' && (char_color > 1))
        {
            WaitForSingleObject(mutex, INFINITE); // 뮤텍스 락
            char_color--;
            ReleaseMutex(mutex); // 뮤텍스 언락
        }

        gotoxy(0, 26);
        printf("                                                                                                                        ");
        gotoxy(row_pos, 26);
        textcolor(char_color);
        printf("=^=");
        textcolor(7);
        gotoxy(0, 27);

        if (input == 'q')
        {
            break;
        }
        // 이벤트 객체를 이용해 다른 스레드가 종료되었는지 확인
        if (WaitForSingleObject(g_hExitEvent, 0) == WAIT_OBJECT_0)
        {
            // 다른 스레드가 종료되었으면 현재 스레드도 종료
            _endthreadex(0);
        }
    }
    // 스레드 종료 시 이벤트 시그널
    SetEvent(g_hExitEvent);

    _endthreadex(0);
}

// 맵 무작위로 생성하는 함수
void game_map_create(char** map)
{
    srand(time(NULL));

    for (int i = 0; i < MAP_SIZE_ROW; i++)
    {
        for (int j = 0; j < MAP_SIZE_COL; j++)
        {
            int random_seed = rand() % 60; // 이 값이 작을수록 난이도 어려움
            if (random_seed < 1)
            {

                map[i][j] = '*'; // 운석 생성
            }
            else
            {
                map[i][j] = ' '; // 빈공간 생성
            }
        }
    }
}

// 맵 출력 함수
void game_map(char** map)
{
    for (int i = 0; i < MAP_SIZE_ROW - 27; i++) // 맨 처음 출력될 맵의 기준이 됨
    {
        WaitForSingleObject(mutex, INFINITE); // 뮤텍스 락
        gotoxy(0, 0);
        for (int j = 27; j > 0; j--)
        {
            textcolor(8);
            printf("%s\n", map[i + j]); // 위에서부터 아래로 맵을 한 줄씩 출력함
            textcolor(7);
            gotoxy(0, 27 - j);
        }
        textcolor(14);
        gotoxy(0, 27);
        printf("-------------------------------------------------------------------------------------------------------------------------");
        gotoxy(0, 28);
        printf("Score::%3d\t[Q]Shutdown\t[W] [S]::Change Color", score++);
        textcolor(7);
        gotoxy(0, 29);
        printf("                 ");
        gotoxy(0, 29);
        printf("Life::");
        for (int i = 0; i < life; i++)
        {
            gotoxy(6 + i, 29);
            textcolor(12);
            printf("♥");
            textcolor(7);
        }

        // 충돌 판정
        if ((map[i][row_pos] == '*') || (map[i][row_pos + 1] == '*') || (map[i][row_pos + 2] == '*'))
        {
            life -= 1;
        }

        // 목숨이 0이 될 경우
        if (life <= 0) 
        {
            gotoxy(6, 29);
            printf("     ");
            ReleaseMutex(mutex); // 뮤텍스 언락
            SetEvent(g_hExitEvent);
            _endthreadex(0);
        }

        ReleaseMutex(mutex); // 뮤텍스 언락

        Sleep(SPEED);

        // 이벤트 객체를 이용해 다른 스레드가 종료되었는지 확인
        if (WaitForSingleObject(g_hExitEvent, 0) == WAIT_OBJECT_0)
        {
            // 다른 스레드가 종료되었으면 현재 스레드도 종료
            _endthreadex(0);
        }
    }
    // 스레드 종료 시 이벤트 시그널
    SetEvent(g_hExitEvent);
    _endthreadex(0);
}

// 멀티스레드 게임플레이(캐릭터의 움직임과 운석이 떨어지는 것이 독립적으로 실행하기 위해)
HANDLE game_play(char** map)
{
    // 맵 생성
    game_map_create(map);

    // 이벤트 생성
    g_hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // _beginthreadex로 스레드 시작 시, 스레드 핸들을 저장하는 것이 좋다.
    HANDLE charThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)game_char, (void*)map, 0, NULL);
    HANDLE mapThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)game_map, (void*)map, 0, NULL);

    // 두 스레드가 모두 종료될 때까지 대기
    WaitForMultipleObjects(2, (const HANDLE[]) { charThread, mapThread }, TRUE, INFINITE);

    // 스레드 핸들 닫기
    CloseHandle(charThread);
    CloseHandle(mapThread);

    // 이벤트 닫기
    CloseHandle(g_hExitEvent);

    return charThread;
}

int main(void)
{
    system("title Flight Express");

    // 변수 선언
    int select_menu = 0; // 메뉴 선택
    // map 2차원 배열 동적할당
    char** map = calloc(MAP_SIZE_ROW, sizeof(char*));
    for (int i = 0; i < MAP_SIZE_ROW; i++)
    {
        map[i] = calloc(MAP_SIZE_COL, 2 * sizeof(char));
    }

    // 뮤텍스 초기화
    mutex = CreateMutex(NULL, FALSE, NULL);

    // 프로그램 시작

    while (1)
    {
        system("cls");
        select_menu = game_menu();

        if (select_menu == '1') // 게임 시작 메뉴
        {
            select_menu = '0';
            system("cls");
            game_logo();
            gotoxy(20, 10);
            sys_print("Game Start!!!\n");

            // 게임 초기 설정
            life = LIFE;
            score = 0;
            row_pos = 55;
            system("cls");

            // 게임 플레이
            HANDLE charThread = game_play(map);

            // 스레드 종료를 기다림
            WaitForSingleObject(charThread, INFINITE);

            // 스레드 핸들을 닫아줌
            CloseHandle(charThread);

            // 스레드 종료 후 생명 0일 경우는 게임 오버 화면 출력
            if (life <= 0) {
                game_over();
            }
            
            system("cls");
            game_logo();
            textcolor(14);
            gotoxy(20, 10);
            sys_print("Your Score is ");
            printf("%d", score);
            textcolor(7);
            _getch();
            
        }
        else if (select_menu == '2') // 게임 설명 메뉴
        {
            select_menu = '0';
            system("cls");
            game_logo();
            textcolor(14);
            gotoxy(20, 10);
            sys_print("[Guide]\n");
            gotoxy(20, 11);
            printf("    [A]::Move Left\t\t[D]::Move Right");
            gotoxy(20, 12);
            printf("[W] [S]::Change Color\t[Q]::Shutdown\n");
            gotoxy(20, 13);
            printf("    [♥]::When the Life becomes 0, it's GAME OVER!!");
            textcolor(7);
            _getch();
        }
        else if (select_menu == '3') // 비행기 색상, 난이도 조절
        {
            select_menu = '0';
            system("cls");
            game_logo();
            textcolor(14);
            gotoxy(20, 10);
            sys_print("[Settings]\n");
            gotoxy(20, 11);
            printf("[W] [S]::Change Color");
            gotoxy(20, 12);
            printf("[Z] [C]::Change Speed(Difficulty)");
            gotoxy(20, 13);
            printf("    [Q]::Shutdown");
            textcolor(7);
            while (1)
            {
                gotoxy(20, 15);
                textcolor(char_color);
                printf("=^=");
                textcolor(7);
                gotoxy(30, 15);
                printf("SPEED::%3d", SPEED);
                char select_3 = _getch();
                if (select_3 == 'w' && char_color < 15)
                {
                    char_color++;
                }
                else if (select_3 == 's' && char_color > 1)
                {
                    char_color--;
                }
                else if (select_3 == 'z' && SPEED > 50)
                {
                    SPEED-=50;
                }
                else if (select_3 == 'c' && SPEED < 300)
                {
                    SPEED += 50;
                }
                else if (select_3 == 'q')
                {
                    break;
                }
            }
            
        }
        else if (select_menu == '4') // 크레딧
        {
            select_menu = '0';
            system("cls");
            game_logo();
            textcolor(14);
            gotoxy(20, 10);
            sys_print("[Credit]\n");
            gotoxy(20, 11);
            sys_print("  SungKyul Univ. ICE ChanYoung Kim");
            gotoxy(20, 12);
            sys_print("  github.com/mixgolem");
            gotoxy(20, 13);
            sys_print("  lavieentulipa116@gmail.com");
            gotoxy(20, 14);
            sys_print("  Coding using VisualStudio 2022");
            textcolor(7);
            _getch();
        }
    }

    for (int i = 0; i < MAP_SIZE_COL; i++)
    {
        free(map[i]);
    }
    free(map);
    CloseHandle(mutex);

    return 0;
}