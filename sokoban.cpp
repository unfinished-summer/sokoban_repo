#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <thread>
#include <chrono>
using namespace std;

// 地图符号说明
// # 墙壁（不能走）
// @ 玩家
// $ 箱子
// . 终点（目标）
// * 箱子在终点上（胜利标记）
// + 玩家站在终点上
// 空格 空地
const int ROW=10;
const int COL=11;
const int MAX_LEVEL=3;
char levels[MAX_LEVEL][ROW][COL] = {
    // ========== 第0关（你原来的初始关卡） ==========
    {
        "##########",
        "#        #",
        "#  $ .   #",
        "#  #     #",
        "#    @   #",
        "#        #",
        "#   $ .  #",
        "#        #",
        "#        #",
        "##########"
    },
    // ========== 第1关（新增关卡示例） ==========
    {
        "##########",
        "#@   .   #",
        "# # $ #  #",
        "#        #",
        "#    $ . #",
        "#        #",
        "#  #     #",
        "#        #",
        "#        #",
        "##########"
    },
    // ========== 第2关（第二新增关卡） ==========
    {
        "##########",
        "#  @     #",
        "#  $ # . #",
        "#  $ #   #",
        "#        #",
        "#   #    #",
        "#    .   #",
        "#        #",
        "#        #",
        "##########"
    }
};

// 当前正在游玩的地图（运行时复制关卡数据到这里）
char map[ROW][COL];

// 记录当前是第几关
int currentLevel = 0;

// 加载对应序号的关卡
void loadLevel(int levelIdx)
{
    for(int i = 0; i < ROW; i++)
    {
        for(int j = 0; j < COL; j++)
        {
            map[i][j] = levels[levelIdx][i][j];
        }
    }
}

// 光标移动到指定坐标，替代清屏，消除闪烁
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawMap()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            cout << map[i][j];
        }
        cout << endl;
    }
}

bool checkWin()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (map[i][j] == '$')
                return false; 
        }
    }
    return true;
}

void move(int dx, int dy)
{
    int px, py;
    // 1. 先找到玩家 @ 的坐标
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (map[i][j] == '@' || map[i][j] == '+')
            {
                py = i;
                px = j;
                break;
            }
        }
    }

    // 玩家下一格坐标
    int nx = px + dx;
    int ny = py + dy;

    // 情况1：下一格是墙 #，不能移动
    if (map[ny][nx] == '#')
        return;

    // 情况2：下一格是空地 或 终点，直接走
    if (map[ny][nx] == ' ' || map[ny][nx] == '.')
    {
        // 玩家原来位置恢复地面/终点
        if (map[py][px] == '+')
            map[py][px] = '.';
        else
            map[py][px] = ' ';

        // 新位置生成玩家
        if (map[ny][nx] == '.')
            map[ny][nx] = '+'; // 站在终点
        else
            map[ny][nx] = '@';
        return;
    }

    // 情况3：下一格是箱子 $ 或 *（箱子在终点），需要推
    if (map[ny][nx] == '$' || map[ny][nx] == '*')
    {
        // 箱子的下一格
        int bx = nx + dx;
        int by = ny + dy;
        // 箱子前方是墙/箱子，推不动
        if (map[by][bx] == '#' || map[by][bx] == '$' || map[by][bx] == '*')
            return;

        // 箱子可以推动
        // 箱子原位置恢复地面/终点
        if (map[ny][nx] == '*')
            map[ny][nx] = '.';
        else
            map[ny][nx] = ' ';

        // 箱子新位置
        if (map[by][bx] == '.')
            map[by][bx] = '*'; // 箱子推到终点
        else
            map[by][bx] = '$';

        // 玩家移动到箱子原来的位置
        if (map[py][px] == '+')
            map[py][px] = '.';
        else
            map[py][px] = ' ';
        map[ny][nx] = '@';
    }
}

int main()
{
    // 隐藏控制台跳动光标
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    char op;
    // 程序启动加载第0关
    loadLevel(currentLevel);
    gotoxy(0, 0);
    drawMap();
    cout << "W-Up S-Down A-Left D-Right Q-Quit R-Restart Level " << currentLevel+1 << endl;

    while (true)
    {
        if (!_kbhit())
        {
            this_thread::sleep_for(chrono::milliseconds(20));
            continue;
        }

        op = _getch();
        bool needRedraw = true;
        switch (op)
        {
            case 'w': case 'W': move(0, -1); break;
            case 's': case 'S': move(0, 1); break;
            case 'a': case 'A': move(-1, 0); break;
            case 'd': case 'D': move(1, 0); break;
            // R键：重置当前关卡
            case 'r': case 'R': loadLevel(currentLevel); break;
            case 'q': case 'Q': return 0;
            default: needRedraw = false;
        }

        if (needRedraw)
        {
            gotoxy(0, 0);
            drawMap();
            cout << "W-Up S-Down A-Left D-Right Q-Quit R-Restart Level " << currentLevel+1 << endl;

            // 通关检测
            if (checkWin())
            {
                if(currentLevel < MAX_LEVEL - 1)
                {
                    // 还有下一关，自动切换
                    currentLevel++;
                    loadLevel(currentLevel);
                    gotoxy(0,0);
                    drawMap();
                    cout << "Level " << currentLevel+1 << " Start!" << endl;
                }
                else
                {
                    // 所有关卡全部通关
                    cout << "All Levels Complete! You Win!" << endl;
                    break;
                }
            }
        }
    }
    return 0;
}