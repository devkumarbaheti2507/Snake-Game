using namespace std;
#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>
#include <windows.h>

int width = 40, height = 20;
bool gameOver;
int x, y, fruitX, fruitY, score;
int tailX[800], tailY[800];
int nTail;

enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
enum eDirection dir;

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void showCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void setup() {
    srand(time(NULL));
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    for (int i = 0; i < nTail; i++) {
        tailX[i] = 0;
        tailY[i] = 0;
    }
    nTail = 0;
    hideCursor();
    system("cls");
}

void gotoxy(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void draw() {
    gotoxy(0, 0);
    for (int i = 0; i < width + 2; i++)
        cout << "~";
    cout << endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "|";
            if (i == y && j == x)
                cout << "O";
            else if (i == fruitY && j == fruitX)
                cout << "F";
            else {
                int print = 0;
                for (int k = 0; k < nTail; k++) {
                    if (i == tailY[k] && j == tailX[k]) {
                        cout << "o";
                        print = 1;
                    }
                }
                if (!print)
                    cout << " ";
            }
            if (j == width - 1)
                cout << "|";
        }
        cout << endl;
    }
    for (int i = 0; i < width + 2; i++)
        cout << "~";
    cout << "\n\n";
    cout << "Score : " << score << endl;
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
}

void logic() {
    int prevX = tailX[0], prevY = tailY[0];
    int prevX2, prevY2;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prevX2 = tailX[i];
        prevY2 = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prevX2;
        prevY = prevY2;
    }
    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }
    if (x >= width || x < 0 || y >= height || y < 0)
        gameOver = true;
    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    showCursor();
    while (true) {
        system("cls");
        cout << "Press any key to play" << endl;
        cout << "Press ESC to exit" << endl;
        int ch = _getch();
        if (ch == 27)
            break;
        setup();
        while (!gameOver) {
            draw();
            input();
            logic();
            Sleep(100);
        }
        cout << "\nGame Over! Final Score : " << score << endl;
        showCursor();
        cout << "Press any key to continue..." << endl;
        _getch();
    }
    return 0;
}