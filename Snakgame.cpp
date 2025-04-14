#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <fstream>
using namespace std;

bool gameover = false;
const int width = 40;
const int height = 30;
int x, y, fruitX, fruitY, score;
int tailX[200], tailY[200];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
COLORREF snakeColor = RGB(166, 16, 16);
int gamemode;
int walls, maxwalls = 50;
int wallX[500], wallY[500];
int snakespeed=150;
int level = 1;

void saveHighScore(int score) {
    ofstream file("highscores.txt", ios::app);
    if (file) {
        file << score << endl;
        file.close();
    }
    else {
        cout << "Error saving the score.\n";
    }
}
int findHighestScore() {
    ifstream file("highscores.txt");
    if (!file) {
        cout << "No high scores available.\n";
        return -1;
    }

    int score, highestScore = INT_MIN;
    while (file >> score) {
        if (score > highestScore) {
            highestScore = score;
        }
    }

    file.close();
    return (highestScore == INT_MIN) ? -1 : highestScore;
}

void displayHighScore() {
    int highestScore = findHighestScore();
    if (highestScore == -1) {
        cout << "No high scores found.\n";
    }
    else {
        cout << "Highest Score: " << highestScore << endl;
    }
}

void saveGameProgress() {
    ofstream Gamefile("gameprogress.txt", ios::out);
    if (!Gamefile) {
        cout << "Error: Could not save game progress.\n";
        return;
    }

    Gamefile << score << endl
        << x << endl
        << y << endl
        << gamemode << endl
        << snakespeed << endl
        << level << endl
        << nTail << endl;

    for (int i = 0; i < nTail; i++) {
        Gamefile << tailX[i] << " " << tailY[i] << endl; 
    }

    Gamefile << walls << endl;
    for (int i = 0; i < walls; i++) {
        Gamefile << wallX[i] << " " << wallY[i] << endl; 
    }

    Gamefile.close();
    cout << "Game progress saved successfully.\n";
}
void loadGameProgress() {
    ifstream Gamefile("gameprogress.txt");
    if (!Gamefile) {
        cout << "\nNo saved game found. Starting New Game.\n";
        return;
    }

    Gamefile >> score >> x >> y >> gamemode >> snakespeed >> level >> nTail;

    for (int i = 0; i < nTail; i++) {
        Gamefile >> tailX[i] >> tailY[i];
    }

    Gamefile >> walls;
    for (int i = 0; i < walls; i++) {
        Gamefile >> wallX[i] >> wallY[i];
    }

    Gamefile.close();
    cout << "Game progress loaded successfully.\n";
}
void GameContinue() {
    cout << "\nDo you want to continue your last game?\n1. Yes\n2. No\n";
    int gameChoice;
    cin >> gameChoice;

    if (gameChoice == 1) {
        cout << "\nLoading Last Game...\n";
        loadGameProgress();
    }
    else if (gameChoice == 2) {
        cout << "Starting New Game.\n";
    }
    else {
        cout << "Invalid choice. Starting New Game.\n";
    }
}

void chooseSnakeColor() {
    cout << "Choose a snake color: \n";
    cout << "1. Red\n2. Green\n3. Blue\n";
    int colorChoice;
    cin >> colorChoice;

    switch (colorChoice) {
    case 1:
        snakeColor = RGB(198, 12, 12);
        break;
    case 2:
        snakeColor = RGB(12, 111, 22);
        break;
    case 3:
        snakeColor = RGB(12, 22, 198);
        break;
    default:
        snakeColor = RGB(198, 12, 12);
    }
}

void setup() {
    gameover = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if (dir != RIGHT) dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT) dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN) dir = UP;
            break;
        case 's':
            if (dir != UP) dir = DOWN;
            break;
        case 'x':
            gameover = true;
            break;
        }
    }
}

void logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
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
    if (gamemode == 1)
    {
        if (x >= width)
            x = 0;
        else if (x < 0)
            x = width - 1;
        if (y >= height)
            y = 0;
        else if (y < 0)
            y = height - 1;
        for (int i = 0; i < nTail; i++) {
            if (tailX[i] == x && tailY[i] == y) {
                gameover = true;
            }
        }
    }
    else if (gamemode == 2 || gamemode == 3) {
        if (x >= width || x < 0 || y >= height || y < 0) {
            gameover = true;
        }
        for (int i = 0; i < walls; i++) {
            if (x == wallX[i] && y == wallY[i]) {
                gameover = true;
            }
        }
    }

    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameover = true;
        }
    }

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void myRect(int x1, int y1, int x2, int y2, int R, int G, int B) {
    HWND console_handle = GetConsoleWindow();
    HDC device_context = GetDC(console_handle);

    HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
    SelectObject(device_context, pen);
    HBRUSH brush = CreateSolidBrush(RGB(R, G, B));
    SelectObject(device_context, brush);

    Rectangle(device_context, x1, y1, x2, y2);
    ReleaseDC(console_handle, device_context);
    DeleteObject(pen);
    DeleteObject(brush);
}

void myEllipse(int x1, int y1, int x2, int y2, int R, int G, int B) {
    HWND console_handle = GetConsoleWindow();
    HDC device_context = GetDC(console_handle);

    HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
    SelectObject(device_context, pen);
    HBRUSH brush = CreateSolidBrush(RGB(R, G, B));
    SelectObject(device_context, brush);

    Ellipse(device_context, x1, y1, x2, y2);
    ReleaseDC(console_handle, device_context);
    DeleteObject(pen);
    DeleteObject(brush);
}

void draw() {
    int cellSize = 25;
    int offsetX = 400;
    int offsetY = 100;
    gotoxy(0, 0);

    myRect(offsetX - 5, offsetY - 5, offsetX + width * cellSize + 5, offsetY + height * cellSize + 5, 236, 249, 165);

    for (int i = 0; i < nTail; i++) {
        myRect(offsetX + tailX[i] * cellSize, offsetY + tailY[i] * cellSize,
            offsetX + (tailX[i] + 1) * cellSize, offsetY + (tailY[i] + 1) * cellSize, GetRValue(snakeColor), GetGValue(snakeColor), GetBValue(snakeColor));
    }
    for (int i = 0; i < walls; i++) {
        myRect(offsetX + wallX[i] * cellSize, offsetY + wallY[i] * cellSize,
            offsetX + (wallX[i] + 1) * cellSize, offsetY + (wallY[i] + 1) * cellSize, 204, 204, 0);
    }
    myRect(offsetX + x * cellSize, offsetY + y * cellSize,
        offsetX + (x + 1) * cellSize, offsetY + (y + 1) * cellSize, 0, 0, 0);

    myEllipse(offsetX + fruitX * cellSize, offsetY + fruitY * cellSize,
        offsetX + (fruitX + 1) * cellSize, offsetY + (fruitY + 1) * cellSize, 255, 0, 0);

    cout << "\n\n\t\t\t\t\tScore: " << score;
    if (gamemode == 2)
        cout << "\t\t\t\t\t\t\t\t Level: " << level;
}

void Gameover() {
    saveHighScore(score);
    system("cls");
    cout << "\nGame Over!\nFinal Score: " << score << endl;
    saveGameProgress();
}

void InfiniteMode() {
    setup();
    cout << "Infinite Mode\n";
    while (!gameover)
    {
        draw();
        input();
        logic();
        if (score >= 250)
            snakespeed = 50;
        else if (score >= 200)
            snakespeed = 70;
        else if (score >= 150)
            snakespeed = 90;
        else if (score >= 100)
            snakespeed =110;
        else if (score >= 50)
            snakespeed = 130;
        Sleep(snakespeed);
    }
    Gameover();
}


void CreateWalls() {
    for (int i = 0; i < walls; i++) {
        do {
            wallX[i] = rand() % width;
            wallY[i] = rand() % height;
        } while ((wallX[i] == x && wallY[i] == y) || (wallX[i] == fruitX && wallY[i] == fruitY));
    }
}

void LevelMode() {
    setup();
    cout << "\nLevel Mode\nStarting level: " << level << endl;
    walls = 0;
    while (!gameover) {
        if (score / 50 >= level) {
            level++;
            walls += 5;
            CreateWalls();
        }
        draw();
        input();
        logic();
        if (score >= 250)
            snakespeed = 50;
        else if (score >= 200)
            snakespeed = 70;
        else if (score >= 150)
            snakespeed = 90;
        else if (score >= 100)
            snakespeed = 110;
        else if (score >= 50)
            snakespeed = 130;
        Sleep(snakespeed);
    }
    Gameover();
}

void CustomMode() {
    setup();
    cout << "\nCustomise Game\nCreate Walls? \n1-Yes \n2-No\n";
    int choose_walls;
    do {
        cin >> choose_walls;
        if (choose_walls != 1 && choose_walls != 2)
            cout << "\nInvalid Option. Enter again\n";
    } while (choose_walls != 1 && choose_walls != 2);

    if (choose_walls == 1) {
        cout << "\nEnter number of walls between 0 and " << maxwalls << ": ";
        cin >> walls;
        while (walls < 0 || walls > maxwalls) {
            cout << "\nInvalid Option. Enter again: ";
            cin >> walls;
        }
        CreateWalls();
    }

    cout << "\nDecide the speed of the snake:  \n1-Slow  \n2-Medium  \n3-Fast\n";
    int speed;
    cin >> speed;
    while (speed < 1 || speed > 3) {
        cout << "\nInvalid Option. Enter again: ";
        cin >> speed;
    }
    if (speed == 1)
        snakespeed = 150;
    else if (speed == 2)
        snakespeed = 100;
    else if (speed == 3)
        snakespeed = 50;
    system("cls");
    while (!gameover) {
        draw();
        input();
        logic();
        Sleep(snakespeed);
    }
    Gameover();
}

void chooseGameMode() {
    GameContinue();
    system("cls");
    cout << "Select Game Mode:\n";
    cout << "1. Infinite Mode\n";
    cout << "2. Level Mode\n";
    cout << "3. Customise Mode\n";
    cin >> gamemode;
    switch (gamemode) {
    case 1:
        system("cls");
        InfiniteMode();
        break;
    case 2:
        system("cls");
        LevelMode();
        break;
    case 3:
        system("cls");
        CustomMode();
        break;
    default:
        cout << "\nInvalid choice. Please try again.\n";
        system("Pause");
        chooseGameMode();
    }
}

void menu() {
    int choice;
    bool validInput = false;
    do {
        system("cls");
        cout << "====================\n";
        cout << "    SNAKE GAME\n";
        cout << "====================\n";
        cout << "   1. Start Game\n";
        cout << "   2. Change Snake Colour\n";
        cout << "   3. Instructions\n";
        cout << "   4. High Scores\n";
        cout << "   5. Exit\n";
        cout << " Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            chooseGameMode();
            validInput = true;
            break;
        case 2:
            chooseSnakeColor();
            break;
        case 3:
            system("cls");
            cout << "Instructions:\n";
            cout << "- Use 'W', 'A', 'S', 'D' to move the snake.\n";
            cout << "- Collect the fruit to score points.\n";
            cout << "- Avoid hitting the walls or your own tail.\n";
            cout << "- Press 'X' during the game to exit.\n";
            cout << "Press any key to go back to the menu";
            _getch();
            break;
        case 4:
            system("cls");
            displayHighScore();
            cout << "Press any key to go back to the menu";
            _getch();
            break;
        case 5:
            cout << "\nExiting the game. Goodbye!\n";
            exit(0);
        default:
            cout << "\nInvalid choice. Please try again.\n";
            system("Pause");
        }
    } while (!validInput);
}

int main()
{
    menu();
    return 0;
}
