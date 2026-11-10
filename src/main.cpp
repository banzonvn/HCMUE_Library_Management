#include "../include/TienIch.h"
#include "../include/DanhSachTemplate.h"
#include "../include/QuanLyPhieuMuon.h"
#include <iostream>

int main() {
    SetConsoleOutputCP(65001); // Bat UTF-8 cho Console

    CTienIch::veLogoHCMUE();
    
    CTienIch::setMau(14);
    std::cout << "\nQUAN LY PHIEU MUON SACH\n";
    CTienIch::setMau(15);
    QuanLyPhieuMuon::chay();
    return 0;
}
