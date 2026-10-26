#include "../include/TienIch.h"
#include "../include/DanhSachTemplate.h"
#include <iostream>

int main() {
    SetConsoleOutputCP(65001); // Bat UTF-8 cho Console

    CTienIch::veLogoHCMUE();
    
    CTienIch::setMau(14);
    std::cout << "\n[!] KHET NOI THANH CONG KHUNG PROJECT HCMUE LIBRARY!\n\n";
    CTienIch::setMau(15);
    
    std::cout << "Nhan phim bat ky de thoat...";
    _getch();
    return 0;
}
