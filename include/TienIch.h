#ifndef TIENICH_H
#define TIENICH_H

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>

class CTienIch {
public:
    static void setMau(int code) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, code);
    }

    static std::string nhapMatKhauAn(bool &isESC) {
        std::string password = "";
        char ch;
        isESC = false;
        while (true) {
            ch = _getch();
            if (ch == 27) { // Phím ESC
                isESC = true;
                return "";
            }
            if (ch == 13) { // Phím Enter
                std::cout << std::endl;
                break;
            }
            if (ch == 8) { // Phím Backspace
                if (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b";
                }
            } else {
                password.push_back(ch);
                std::cout << "*";
            }
        }
        return password;
    }

    static void veLogoHCMUE() {
        setMau(14); // Màu Vàng
        std::cout << "===================================================================================\n";
        setMau(11); // Màu Cyan
        std::cout << "  _  _   ____  __  __  _  _  ____     _     _  ____  ____   __   ____  _  _ \n";
        std::cout << " ( || ) / ___)(  \\/  )( || )(  __)   ( )   ( )(  _ \\(  _ \\ / _\\ (  _ \\( \\/ )\n";
        std::cout << "  ) __ \\( (__  )    (  ) __ ( ) _)    / (_/\\ ) )(  _ (  _ \\/    \\ )   / \\  /\n";
        std::cout << " (_)(_) \\____)(_/\\/\\_)(_)(_)(____)   \\____/(__)(____/(____/_/\\_\\(__\\_)  \\/  \n";
        setMau(14);
        std::cout << "===================================================================================\n";
        setMau(10); // Màu Xanh lá
        std::cout << "     TRUONG DAI HOC SU PHAM TP.HCM - HE THONG QUAN LY THU VIEN THONG MINH         \n";
        setMau(15);
    }
};

#endif
