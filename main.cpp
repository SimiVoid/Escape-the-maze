#include <fstream>
#include <string>
#include <cstdio>
#include <winuser.h>
#include <afxres.h>
#include <conio.h>
#include <chrono>
#include <vector>
#include <sstream>
#include <iostream>

#define WIN_WIDTH 120
#define WIN_HEIGHT 30

void showBorder();
void gotoXY(const int&, const int&);
void title();
int menu();
void game();
void scoreboards();

int main() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);

    system("color 30");
    title();
    Sleep(1500);

    bool isAlive = true;

    while(isAlive) {
        switch (menu()) {
            case '1':
                game();
                break;
            case '2':
                scoreboards();
                break;
            case 'x':
                isAlive = false;
                break;
        }
    }

    return 0;
}

void showBorder() {
    for(int i = 1; i < WIN_WIDTH - 1; ++i) {
        gotoXY(i, 0);
        printf("%c", '\xcd');
        gotoXY(i, WIN_HEIGHT - 1);
        printf("%c", '\xcd');
    }

    for(int i = 1; i < WIN_HEIGHT - 1; ++i) {
        gotoXY(0, i);
        printf("%c", '\xba');
        gotoXY(WIN_WIDTH - 1, i);
        printf("%c", '\xba');
    }

    gotoXY(WIN_WIDTH - 1, WIN_HEIGHT - 1);
    printf("%c", '\xbc');
    gotoXY(WIN_WIDTH - 1, 0);
    printf("%c", '\xbb');
    gotoXY(0, WIN_HEIGHT - 1);
    printf("%c", '\xc8');
    gotoXY(0, 0);
    printf("%c", '\xc9');
}

void gotoXY(const int& column, const int& line)
{
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(
            GetStdHandle(STD_OUTPUT_HANDLE),
            coord
    );
}

void title() {
    system("cls");

    showBorder();

    gotoXY(11, 6);
    printf(R"(___________                                      __  .__                                           )");
    gotoXY(11, 7);
    printf(R"(\_   _____/ ______ ____ _____  ______   ____   _/  |_|  |__   ____     _____ _____  ________ ____  )");
    gotoXY(11, 8);
    printf(R"( |    __)_ /  ___// ___\\__  \ \____ \_/ __ \  \   __\  |  \_/ __ \   /     \\__  \ \___   // __ \ )");
    gotoXY(11, 9);
    printf(R"( |        \\___ \\  \___ / __ \|  |_> >  ___/   |  | |   Y  \  ___/  |  Y Y  \/ __ \_/    /\  ___/ )");
    gotoXY(11, 10);
    printf(R"(/_______  /____  >\___  >____  /   __/ \___  >  |__| |___|  /\___  > |__|_|  (____  /_____ \\___  >)");
    gotoXY(11, 11);
    printf(R"(        \/     \/     \/     \/|__|        \/             \/     \/        \/     \/      \/    \/ )");
}

[[nodiscard]]
int menu() {
    system("cls");

    title();

    gotoXY(10, 15); printf("====== Menu =======");
    gotoXY(10, 17); printf("[1] - Play");
    gotoXY(10, 18); printf("[2] - Scoreboards");
    gotoXY(10, 19); printf("[x] - Exit");

    return getch();
}

void saveScore(const std::chrono::duration<double>& time) {
    std::string name;

    system("cls");
    title();
    gotoXY(10, 15); printf("===== You win =====");
    gotoXY(10, 16); printf("Your time: %2f", time.count());
    gotoXY(10, 17); printf("Enter your name: ");

    while((GetKeyState('W') & 0x8000) && (GetKeyState('S') & 0x8000)
            && (GetKeyState('A') & 0x8000) && (GetKeyState('D') & 0x8000)) { }

    std::cin >> name;
    std::vector<std::pair<std::string,double>> times;
    std::fstream file(std::string(getenv("APPDATA")) + R"(\SimiVoid Studio\Escape the maze\scoreboards.dat)", std::ios::in);
    std::string line;

    if(file.good() && file.is_open()) {
        while (std::getline(file, line)) {
            std::stringstream ss;
            ss << line;

            std::string temp;
            double timeTemp;
            ss >> temp >> temp >> timeTemp;
            times.emplace_back(temp, timeTemp);
        }

        file.close();
    }
    else {
        try {
            system(std::string("mkdir \"" + std::string(getenv("APPDATA")) + R"(\SimiVoid Studio\Escape the maze)" + "\"").c_str());
        }
        catch(const std::bad_alloc& e) {

        }
    }

    std::vector<std::pair<std::string,double>> nextTimes;

    bool isNot = true;

    if(times.empty())
        nextTimes.emplace_back(name, time.count());                                             

    for(int i = 0; i < times.size(); i++) {
        if (time.count() < times[0].second && isNot)
            nextTimes.emplace_back(name, time.count()), isNot = false, i--;
        if (i == 4)  break;

        nextTimes.emplace_back(times[i]);
    }

    file.open(std::string(getenv("APPDATA")) + R"(\SimiVoid Studio\Escape the maze\scoreboards.dat)", std::ios::out);

    for(int i = 0; i < nextTimes.size(); ++i)
        file << i + 1 << ". " << nextTimes[i].first << " " << nextTimes[i].second << "\n";
}

void game() {
    system("cls");
    showBorder();

    for(int i = 1; i < WIN_WIDTH - 1; ++i) {
        gotoXY(i, 2);
        printf("%c", '\xcd');
    }

    gotoXY(2, 1);
    printf("Time: ");

    bool maze[110][20];

    for(auto & i : maze)
        for(bool & j : i)
            j = true;

    for(int r = 0; r < rand() % 10; ++r)
        for(int i = 1; i < 109; ++i)
            for(int j = 1; j < 19; ++j)
                maze[i][j] = rand() % 10 < 3;

    for(int i = 0; i < 110; ++i)
        for(int j = 0; j < 20; ++j)
            if(maze[i][j]) {
                gotoXY(i + 5,j + 5);

                if(i > 0 && j > 0 && j < 19 && i < 109 && !maze[i - 1][j] && !maze[i][j - 1] && maze[i + 1][j] && maze[i][j + 1])
                    printf("%c", '\xc9');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && !maze[i + 1][j] && !maze[i][j - 1] && maze[i - 1][j] && maze[i][j + 1])
                    printf("%c", '\xbb');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && !maze[i + 1][j] && !maze[i][j + 1] && maze[i - 1][j] && maze[i][j - 1])
                    printf("%c", '\xbc');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && !maze[i - 1][j] && !maze[i][j + 1] && maze[i + 1][j] && maze[i][j - 1])
                    printf("%c", '\xc8');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && !maze[i - 1][j] && !maze[i + 1][j] && maze[i][j + 1] && maze[i][j - 1])
                    printf("%c", '\xba');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && !maze[i][j - 1] && !maze[i][j + 1] && maze[i + 1][j] && maze[i - 1][j])
                    printf("%c", '\xcd');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && maze[i][j - 1] && maze[i][j + 1] && maze[i + 1][j] && maze[i - 1][j])
                    printf("%c", '\xce');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && maze[i][j - 1] && maze[i][j + 1] && maze[i + 1][j] && !maze[i - 1][j])
                    printf("%c", '\xcc');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && maze[i][j - 1] && maze[i][j + 1] && !maze[i + 1][j] && maze[i - 1][j])
                    printf("%c", '\xb9');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && maze[i][j - 1] && !maze[i][j + 1] && maze[i + 1][j] && maze[i - 1][j])
                    printf("%c", '\xca');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && !maze[i][j - 1] && maze[i][j + 1] && maze[i + 1][j] && maze[i - 1][j])
                    printf("%c", '\xcb');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && !maze[i - 1][j] && !maze[i + 1][j] && maze[i][j + 1] && !maze[i][j - 1])
                    printf("%c", '\xba');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && !maze[i][j - 1] && !maze[i][j + 1] && maze[i + 1][j] && !maze[i - 1][j])
                    printf("%c", '\xcd');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && !maze[i - 1][j] && !maze[i + 1][j] && !maze[i][j + 1] && maze[i][j - 1])
                    printf("%c", '\xba');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && !maze[i][j - 1] && !maze[i][j + 1] && !maze[i + 1][j] && maze[i - 1][j])
                    printf("%c", '\xcd');
                else if(i > 0 && j > 0 && j < 19 && i < 109 && !maze[i][j - 1] && !maze[i][j + 1] && !maze[i + 1][j] && !maze[i - 1][j])
                    maze[i][j] = false;
                else if(i == 0 && j == 0)
                    printf("%c", '\xc9');
                else if(i == 0 && j == 19)
                    printf("%c", '\xc8');
                else if(i == 109 && j == 0)
                    printf("%c", '\xbb');
                else if(i == 109 && j == 19)
                    printf("%c", '\xbc');
                else if(i == 0 && j > 0 && j < 19 && !maze[i + 1][j] && maze[i][j + 1] && maze[i][j - 1])
                    printf("%c", '\xba');
                else if(i == 109 && j > 0 && j < 19 && !maze[i - 1][j] && maze[i][j + 1] && maze[i][j - 1])
                    printf("%c", '\xba');
                else if(i > 0 && j == 19 && i < 109 && !maze[i][j - 1] && maze[i + 1][j] && maze[i - 1][j])
                    printf("%c", '\xcd');
                else if(i > 0 && j == 0 && i < 109 && !maze[i][j + 1] && maze[i + 1][j] && maze[i - 1][j])
                    printf("%c", '\xcd');
                else if(i == 0 && j > 0 && j < 19 && maze[i][j - 1] && maze[i][j + 1] && maze[i + 1][j])
                    printf("%c", '\xcc');
                else if(i == 109 && j > 0 && j < 19 && maze[i][j - 1] && maze[i][j + 1] && maze[i - 1][j])
                    printf("%c", '\xb9');
                else if(i > 0 && j == 0 && i < 109 && maze[i][j + 1] && maze[i + 1][j] && maze[i - 1][j])
                    printf("%c", '\xcb');
                else if(i > 0 && j == 19 && i < 109 && maze[i][j - 1] && maze[i + 1][j] && maze[i - 1][j])
                    printf("%c", '\xca');
            }

    maze[0][2] = false;
    maze[109][17] = false;

    gotoXY(5, 7);
    printf(" ");
    gotoXY(114, 22);
    printf(" ");

    int playerX = 0;
    int playerY = 2;

    gotoXY(playerX + 5, playerY + 5);
    printf("%c", '\xcf');

    auto movePlayer = [&](int x, int y) {
        if(playerX + x >= 0 && playerX + x < 110 && playerY + y >= 0 && playerY + y < 20 && !maze[playerX + x][playerY + y]) {
            gotoXY(playerX + 5, playerY + 5);
            printf("%c", ' ');

            playerX += x;
            playerY += y;

            gotoXY(playerX + 5, playerY + 5);
            printf("%c", '\xcf');
        }
    };

    auto startTime = std::chrono::system_clock::now();

    auto showTime = [&]() {
        auto now = std::chrono::system_clock::now();
        std::chrono::duration<double> timeDifference = now - startTime;

        gotoXY(7, 1);
        printf("%2f", timeDifference.count());
    };

    while(true) {
        showTime();

        if(GetKeyState(VK_ESCAPE) & 0x8000)
            break;
        if(GetKeyState('W') & 0x8000)
            movePlayer(0, -1);
        if(GetKeyState('S') & 0x8000)
            movePlayer(0, 1);
        if(GetKeyState('A') & 0x8000)
            movePlayer(-1, 0);
        if(GetKeyState('D') & 0x8000)
            movePlayer(1, 0);

        if(playerX == 109 && playerY == 17) {
            saveScore(std::chrono::system_clock::now() - startTime);
            break;
        }

        Sleep(50);
    }
}

void scoreboards() {
    system("cls");

    title();

    std::ifstream file(std::string(getenv("APPDATA")) + R"(\SimiVoid Studio\Escape the maze\scoreboards.dat)");

    gotoXY(10, 15);
    printf("=== Scoreboards ===");

    std::string line;
    int i = 17;
    while(getline(file, line)) {
        gotoXY(10, i);
        printf("%s", line.c_str());
        ++i;
    }

    gotoXY(10, i + 1);
    printf("[r] - Return");

    while(getch() != 'r') { };
}