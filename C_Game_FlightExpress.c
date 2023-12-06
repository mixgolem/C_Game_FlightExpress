#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <process.h>

// ���� ������ �̺�Ʈ �ڵ� ����
HANDLE g_hExitEvent;

// �� ũ��� ���� 120ĭ, ���� 300��
#define MAP_SIZE_ROW 300
#define MAP_SIZE_COL 120
#define LIFE 3 // �⺻ ���

// ���� ���� ����
int row_pos = 55; // �ʱ� ��ġ
int life = LIFE; // ���
int score = 0; // ����
int SPEED = 300; // ���� �ӵ�(�������� �����)
int char_color = 15; // ĳ���� ����� ����

// ���ؽ� ����
HANDLE mutex;

// �ܼ�â���� ��� ��ġ �����ϴ� �Լ�
void gotoxy(int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// �ܼ�â ��� ���� ���� �Լ�
void textcolor(int colorNum)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

// ���� ������
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

// ���ڿ��� �� �ϴ� �� ó�� ����ϴ� �Լ�
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

// ���� �ΰ� ���
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

// ���ӿ��� ȭ�� ���
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

// ���� �޴� �Լ�
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

// ĳ���� ��� �� ���� �Լ�
void game_char(char** map)
{
    while (1)
    {
        char input = _getch();
        Sleep(1);
        if ((input == 'a') && (row_pos > 2))
        {
            WaitForSingleObject(mutex, INFINITE); // ���ؽ� ��
            row_pos -= 2;
            ReleaseMutex(mutex); // ���ؽ� ���
        }
        else if (input == 'd' && (row_pos < 114))
        {
            WaitForSingleObject(mutex, INFINITE); // ���ؽ� ��
            row_pos += 2;
            ReleaseMutex(mutex); // ���ؽ� ���
        }
        else if (input == 'w' && (char_color < 15))
        {
            WaitForSingleObject(mutex, INFINITE); // ���ؽ� ��
            char_color++;
            ReleaseMutex(mutex); // ���ؽ� ���
        }
        else if (input == 's' && (char_color > 1))
        {
            WaitForSingleObject(mutex, INFINITE); // ���ؽ� ��
            char_color--;
            ReleaseMutex(mutex); // ���ؽ� ���
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
        // �̺�Ʈ ��ü�� �̿��� �ٸ� �����尡 ����Ǿ����� Ȯ��
        if (WaitForSingleObject(g_hExitEvent, 0) == WAIT_OBJECT_0)
        {
            // �ٸ� �����尡 ����Ǿ����� ���� �����嵵 ����
            _endthreadex(0);
        }
    }
    // ������ ���� �� �̺�Ʈ �ñ׳�
    SetEvent(g_hExitEvent);

    _endthreadex(0);
}

// �� �������� �����ϴ� �Լ�
void game_map_create(char** map)
{
    srand(time(NULL));

    for (int i = 0; i < MAP_SIZE_ROW; i++)
    {
        for (int j = 0; j < MAP_SIZE_COL; j++)
        {
            int random_seed = rand() % 60; // �� ���� �������� ���̵� �����
            if (random_seed < 1)
            {

                map[i][j] = '*'; // � ����
            }
            else
            {
                map[i][j] = ' '; // ����� ����
            }
        }
    }
}

// �� ��� �Լ�
void game_map(char** map)
{
    for (int i = 0; i < MAP_SIZE_ROW - 27; i++) // �� ó�� ��µ� ���� ������ ��
    {
        WaitForSingleObject(mutex, INFINITE); // ���ؽ� ��
        gotoxy(0, 0);
        for (int j = 27; j > 0; j--)
        {
            textcolor(8);
            printf("%s\n", map[i + j]); // ���������� �Ʒ��� ���� �� �پ� �����
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
            printf("��");
            textcolor(7);
        }

        // �浹 ����
        if ((map[i][row_pos] == '*') || (map[i][row_pos + 1] == '*') || (map[i][row_pos + 2] == '*'))
        {
            life -= 1;
        }

        // ����� 0�� �� ���
        if (life <= 0) 
        {
            gotoxy(6, 29);
            printf("     ");
            ReleaseMutex(mutex); // ���ؽ� ���
            SetEvent(g_hExitEvent);
            _endthreadex(0);
        }

        ReleaseMutex(mutex); // ���ؽ� ���

        Sleep(SPEED);

        // �̺�Ʈ ��ü�� �̿��� �ٸ� �����尡 ����Ǿ����� Ȯ��
        if (WaitForSingleObject(g_hExitEvent, 0) == WAIT_OBJECT_0)
        {
            // �ٸ� �����尡 ����Ǿ����� ���� �����嵵 ����
            _endthreadex(0);
        }
    }
    // ������ ���� �� �̺�Ʈ �ñ׳�
    SetEvent(g_hExitEvent);
    _endthreadex(0);
}

// ��Ƽ������ �����÷���(ĳ������ �����Ӱ� ��� �������� ���� ���������� �����ϱ� ����)
HANDLE game_play(char** map)
{
    // �� ����
    game_map_create(map);

    // �̺�Ʈ ����
    g_hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // _beginthreadex�� ������ ���� ��, ������ �ڵ��� �����ϴ� ���� ����.
    HANDLE charThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)game_char, (void*)map, 0, NULL);
    HANDLE mapThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)game_map, (void*)map, 0, NULL);

    // �� �����尡 ��� ����� ������ ���
    WaitForMultipleObjects(2, (const HANDLE[]) { charThread, mapThread }, TRUE, INFINITE);

    // ������ �ڵ� �ݱ�
    CloseHandle(charThread);
    CloseHandle(mapThread);

    // �̺�Ʈ �ݱ�
    CloseHandle(g_hExitEvent);

    return charThread;
}

int main(void)
{
    system("title Flight Express");

    // ���� ����
    int select_menu = 0; // �޴� ����
    // map 2���� �迭 �����Ҵ�
    char** map = calloc(MAP_SIZE_ROW, sizeof(char*));
    for (int i = 0; i < MAP_SIZE_ROW; i++)
    {
        map[i] = calloc(MAP_SIZE_COL, 2 * sizeof(char));
    }

    // ���ؽ� �ʱ�ȭ
    mutex = CreateMutex(NULL, FALSE, NULL);

    // ���α׷� ����

    while (1)
    {
        system("cls");
        select_menu = game_menu();

        if (select_menu == '1') // ���� ���� �޴�
        {
            select_menu = '0';
            system("cls");
            game_logo();
            gotoxy(20, 10);
            sys_print("Game Start!!!\n");

            // ���� �ʱ� ����
            life = LIFE;
            score = 0;
            row_pos = 55;
            system("cls");

            // ���� �÷���
            HANDLE charThread = game_play(map);

            // ������ ���Ḧ ��ٸ�
            WaitForSingleObject(charThread, INFINITE);

            // ������ �ڵ��� �ݾ���
            CloseHandle(charThread);

            // ������ ���� �� ���� 0�� ���� ���� ���� ȭ�� ���
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
        else if (select_menu == '2') // ���� ���� �޴�
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
            printf("    [��]::When the Life becomes 0, it's GAME OVER!!");
            textcolor(7);
            _getch();
        }
        else if (select_menu == '3') // ����� ����, ���̵� ����
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
        else if (select_menu == '4') // ũ����
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