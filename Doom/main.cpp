
#include <iostream>
#include <Windows.h>
#include <string>
#include <chrono>
int ScreenWidth = 120;
int ScreenHeight = 40;

float PlayerX = 8.0f;
float PlayerY = 8.0f;
float PlayerA = 0.0f;

int MapHeight = 16;
int MapWidth = 16;

float Pov = 3.14 / 4.0;
float Depth = 16.0f;
int main()
{
    wchar_t* screen = new wchar_t[ScreenWidth * ScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    std::wstring map;
    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();
    while (1) {
        tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elTime = tp2 - tp1;
        tp1 = tp2;
        float felTime = elTime.count();




        if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
            PlayerA -= (0.7f) * felTime;
        }

        if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
            PlayerA += (0.7f) * felTime;
        }

        if (GetAsyncKeyState((unsigned short)'W') % 0x8000) {
            PlayerX += sinf(PlayerA) * 5.0f * felTime;
            PlayerY += cosf(PlayerA) * 5.0f * felTime;
        }

        if (GetAsyncKeyState((unsigned short)'S') % 0x8000) {
            PlayerX -= sinf(PlayerA) * 5.0f * felTime;
            PlayerY -= cosf(PlayerA) * 5.0f * felTime;
        }








        for (int x = 0; x < ScreenWidth; x++) {
            float Angle = (PlayerA - Pov / 2.0f) + ((float)x / (float)ScreenWidth) * Pov;

            float DistanceToWall = 0;
            bool HitWall = false;

            float EyeX = sinf(Angle);
            float EyeY = cosf(Angle);

            while (!HitWall && DistanceToWall < Depth) {
                DistanceToWall += 0.1f;
                int FakeX = (int)(PlayerX + EyeX * DistanceToWall);
                int FakeY = (int)(PlayerY + EyeY * DistanceToWall);
                
                if (FakeX < 0 || FakeX >= MapWidth || FakeY < 0 || FakeY >= MapWidth) {
                    HitWall = true;
                    DistanceToWall = Depth;
                }
                else {
                    if (map[FakeY * MapWidth + FakeX] == '#') {
                        HitWall = true;
                    }
                }
            }
            int Ceiling = (float)(ScreenHeight / 2.0) - ScreenHeight / ((float)DistanceToWall);
            int Floor = ScreenHeight - Ceiling;

            short Shade = ' ';
            if (DistanceToWall <= Depth / 4.0f) {
                Shade = 0x2588;
            }
            else if (DistanceToWall < Depth / 3.0f) {
                Shade = 0x2593;
            }
            else if (DistanceToWall < Depth / 2.0f) {
                Shade = 0X2592;
            }
            else if (DistanceToWall < Depth) {
                Shade = 0x2591;
            }
            else {
                Shade = ' ';
            }

            for (int y = 0; y < ScreenHeight; y++) {
                if (y < Ceiling) {
                    screen[y * ScreenWidth + x] = ' ';
                }
                else if(y > Ceiling && y <= Floor) {
                    screen[y * ScreenWidth + x] = Shade;
                }
                else {//Пол
                    float hui = 1.0f - (((float)y - ScreenHeight / 2.0f) / ((float)ScreenHeight / 2.0f));
                    if (hui < 0.25) {
                        Shade = '#';
                    }
                    else if (hui < 0.5) {
                        Shade = 'x';
                    }
                    else if (hui < 0.75) {
                        Shade = '.';
                    }
                    else if (hui < 1) {
                        Shade = '-';
                    }
                    else  {
                        Shade = ' ';
                    }
                    screen[y * ScreenWidth + x] = Shade;
                    //screen[y * ScreenWidth + x] = ' ';
                }
            }

        }


        screen[ScreenWidth * ScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
    }




}

