#ifndef QUANLYPHIEUMUON_H
#define QUANLYPHIEUMUON_H

#include <array>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

namespace QuanLyPhieuMuon {

using Sach = std::array<std::string, 9>;
using BanDoc = std::array<std::string, 3>;
using PhieuMuon = std::array<std::string, 6>;

inline std::string thuMucDuLieu() {
    std::string duongDan = __FILE__;
    const auto viTriSrc = duongDan.find_last_of("\\/");
    if (viTriSrc == std::string::npos) return "data";
    duongDan.resize(viTriSrc);
    const auto viTriProject = duongDan.find_last_of("\\/");
    if (viTriProject == std::string::npos) return duongDan + "\\data";
    return duongDan.substr(0, viTriProject) + "\\data";
}

inline std::string ngaySauSoNgay(int soNgay) {
    const auto thoiDiem = std::chrono::system_clock::now() + std::chrono::hours(24 * soNgay);
    const std::time_t thoiGian = std::chrono::system_clock::to_time_t(thoiDiem);
    std::tm ngay{};
    if (const std::tm* ngayHeThong = std::localtime(&thoiGian)) ngay = *ngayHeThong;
    std::ostringstream ketQua;
    ketQua << std::put_time(&ngay, "%Y-%m-%d");
    return ketQua.str();
}

inline std::vector<std::string> tachDong(const std::string& dong) {
    std::vector<std::string> truong;
    std::stringstream boDong(dong);
    std::string giaTri;
    while (std::getline(boDong, giaTri, '|')) {
        truong.push_back(giaTri);
    }
    if (!dong.empty() && dong.back() == '|') {
        truong.emplace_back();
    }
    return truong;
}

template <std::size_t SoTruong, typename BanGhi>
inline bool docFile(const std::string& duongDan, std::vector<BanGhi>& danhSach,
                    bool choPhepChuaTonTai = false) {
    std::ifstream tep(duongDan);
    if (!tep) {
        if (choPhepChuaTonTai && GetFileAttributesA(duongDan.c_str()) == INVALID_FILE_ATTRIBUTES) {
            danhSach.clear();
            return true;
        }
        std::cerr << "Khong the mo file: " << duongDan << '\n';
        return false;
    }

    danhSach.clear();
    std::string dong;
    std::size_t soDong = 0;
    while (std::getline(tep, dong)) {
        ++soDong;
        if (dong.empty()) continue;
        const auto truong = tachDong(dong);
        if (truong.size() != SoTruong) {
            std::cerr << "Du lieu khong hop le tai " << duongDan
                      << ", dong " << soDong << ".\n";
            return false;
        }
        BanGhi banGhi{};
        for (std::size_t i = 0; i < SoTruong; ++i) banGhi[i] = truong[i];
        danhSach.push_back(banGhi);
    }
    return true;
}

template <typename BanGhi>
inline std::string noiDanhSach(const std::vector<BanGhi>& danhSach) {
    std::ostringstream noiDung;
    for (const auto& banGhi : danhSach) {
        for (std::size_t i = 0; i < banGhi.size(); ++i) {
            if (i > 0) noiDung << '|';
            noiDung << banGhi[i];
        }
        noiDung << '\n';
    }
    return noiDung.str();
}

inline bool ghiTepTam(const std::string& duongDan, const std::string& noiDung) {
    std::ofstream tep(duongDan, std::ios::binary | std::ios::trunc);
    if (!tep) return false;
    tep << noiDung;
    return tep.good();
}

inline bool thayTheTep(const std::string& tepTam, const std::string& tepDich) {
    return MoveFileExA(tepTam.c_str(), tepDich.c_str(),
                       MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
}

inline bool luuSachVaPhieu(const std::vector<Sach>& sach,
                           const std::vector<PhieuMuon>& phieuMuon,
                           const std::vector<Sach>& sachCu) {
    const auto thuMuc = thuMucDuLieu();
    const auto tepSach = thuMuc + "\\Sach.txt";
    const auto tepPhieu = thuMuc + "\\PhieuMuon.txt";
    const auto tamSach = thuMuc + "\\Sach.txt.tmp";
    const auto tamPhieu = thuMuc + "\\PhieuMuon.txt.tmp";

    if (!ghiTepTam(tamSach, noiDanhSach(sach)) ||
        !ghiTepTam(tamPhieu, noiDanhSach(phieuMuon))) {
        std::cerr << "Khong the ghi file tam. Du lieu chua duoc cap nhat.\n";
        return false;
    }
    if (!thayTheTep(tamSach, tepSach)) {
        std::cerr << "Khong the cap nhat Sach.txt.\n";
        return false;
    }
    if (!thayTheTep(tamPhieu, tepPhieu)) {
        const auto tepPhucHoi = thuMuc + "\\Sach.txt.rollback";
        if (ghiTepTam(tepPhucHoi, noiDanhSach(sachCu))) {
            thayTheTep(tepPhucHoi, tepSach);
        }
        std::cerr << "Khong the cap nhat PhieuMuon.txt; da thu phuc hoi Sach.txt.\n";
        return false;
    }
    return true;
}

inline int timSach(const std::vector<Sach>& sach, const std::string& maSach) {
    for (std::size_t i = 0; i < sach.size(); ++i) {
        if (sach[i][0] == maSach) return static_cast<int>(i);
    }
    return -1;
}

inline int timBanDoc(const std::vector<BanDoc>& banDoc, const std::string& maBanDoc) {
    for (std::size_t i = 0; i < banDoc.size(); ++i) {
        if (banDoc[i][0] == maBanDoc) return static_cast<int>(i);
    }
    return -1;
}

inline bool doiSangSo(const std::string& chuoi, int& ketQua) {
    try {
        std::size_t daDoc = 0;
        ketQua = std::stoi(chuoi, &daDoc);
        return daDoc == chuoi.size();
    } catch (...) {
        return false;
    }
}

inline void hienThiPhieuMuon(const std::vector<PhieuMuon>& danhSach) {
    if (danhSach.empty()) {
        std::cout << "Chua co phieu muon nao.\n";
        return;
    }
    std::cout << "\n" << std::left << std::setw(12) << "So phieu"
              << std::setw(16) << "Ma ban doc" << std::setw(14) << "Ma sach"
              << std::setw(16) << "Ngay muon" << std::setw(16) << "Ngay phai tra"
              << "Trang thai\n";
    for (const auto& phieu : danhSach) {
        std::cout << std::left << std::setw(12) << phieu[0] << std::setw(16) << phieu[1]
                  << std::setw(14) << phieu[2] << std::setw(16) << phieu[3]
                  << std::setw(16) << phieu[4]
                  << (phieu[5] == "1" ? "Dang muon" : "Da tra") << '\n';
    }
}

inline void xemDanhSachPhieu() {
    std::vector<PhieuMuon> phieuMuon;
    if (docFile<6>(thuMucDuLieu() + "\\PhieuMuon.txt", phieuMuon, true)) {
        hienThiPhieuMuon(phieuMuon);
    }
}

inline void muonSach() {
    const auto thuMuc = thuMucDuLieu();
    std::vector<Sach> sach;
    std::vector<BanDoc> banDoc;
    std::vector<PhieuMuon> phieuMuon;
    if (!docFile<9>(thuMuc + "\\Sach.txt", sach) ||
        !docFile<3>(thuMuc + "\\DocGia.txt", banDoc) ||
        !docFile<6>(thuMuc + "\\PhieuMuon.txt", phieuMuon, true)) return;

    std::string maSach;
    std::string maBanDoc;
    std::cout << "Nhap ma sach: ";
    std::getline(std::cin, maSach);
    std::cout << "Nhap ma ban doc: ";
    std::getline(std::cin, maBanDoc);

    const int viTriSach = timSach(sach, maSach);
    if (viTriSach < 0) {
        std::cout << "Khong tim thay sach.\n";
        return;
    }
    if (sach[viTriSach][8] != "0") {
        std::cout << "Sach dang duoc muon, khong the cho muon.\n";
        return;
    }
    if (timBanDoc(banDoc, maBanDoc) < 0) {
        std::cout << "Ma ban doc khong ton tai.\n";
        return;
    }

    const std::vector<Sach> sachCu = sach;
    const int soPhieu = static_cast<int>(phieuMuon.size()) + 1;
    sach[viTriSach][8] = std::to_string(soPhieu);
    phieuMuon.push_back({std::to_string(soPhieu), maBanDoc, maSach,
                         ngaySauSoNgay(0), ngaySauSoNgay(7), "1"});

    if (luuSachVaPhieu(sach, phieuMuon, sachCu)) {
        std::cout << "Muon sach thanh cong. So phieu muon: " << soPhieu << '\n';
    }
}

inline void traSach() {
    const auto thuMuc = thuMucDuLieu();
    std::vector<Sach> sach;
    std::vector<PhieuMuon> phieuMuon;
    if (!docFile<9>(thuMuc + "\\Sach.txt", sach) ||
        !docFile<6>(thuMuc + "\\PhieuMuon.txt", phieuMuon)) return;

    std::string chuoiSoPhieu;
    std::cout << "Nhap so phieu muon can tra: ";
    std::getline(std::cin, chuoiSoPhieu);
    int soPhieu = 0;
    if (!doiSangSo(chuoiSoPhieu, soPhieu)) {
        std::cout << "So phieu muon khong hop le.\n";
        return;
    }

    int viTriPhieu = -1;
    for (std::size_t i = 0; i < phieuMuon.size(); ++i) {
        if (phieuMuon[i][0] == std::to_string(soPhieu)) {
            viTriPhieu = static_cast<int>(i);
            break;
        }
    }
    if (viTriPhieu < 0 || phieuMuon[viTriPhieu][5] == "0") {
        std::cout << "Khong tim thay phieu muon dang hoat dong.\n";
        return;
    }

    const int viTriSach = timSach(sach, phieuMuon[viTriPhieu][2]);
    int maPhieuTrenSach = 0;
    if (viTriSach < 0 || !doiSangSo(sach[viTriSach][8], maPhieuTrenSach) ||
        maPhieuTrenSach != soPhieu) {
        std::cout << "Du lieu sach va phieu muon khong khop; khong the tra sach.\n";
        return;
    }

    const std::vector<Sach> sachCu = sach;
    sach[viTriSach][8] = "0";
    phieuMuon[viTriPhieu][5] = "0";
    if (luuSachVaPhieu(sach, phieuMuon, sachCu)) {
        std::cout << "Tra sach thanh cong.\n";
    }
}

inline void chay() {
    while (true) {
        std::cout << "\n========== QUAN LY PHIEU MUON ==========\n"
                  << "1. Hien thi danh sach phieu muon\n"
                  << "2. Muon sach\n"
                  << "3. Tra sach\n"
                  << "0. Thoat\n"
                  << "Chon chuc nang: ";
        std::string luaChon;
        std::getline(std::cin, luaChon);
        if (luaChon == "1") xemDanhSachPhieu();
        else if (luaChon == "2") muonSach();
        else if (luaChon == "3") traSach();
        else if (luaChon == "0") return;
        else std::cout << "Lua chon khong hop le.\n";
    }
}

}

#endif