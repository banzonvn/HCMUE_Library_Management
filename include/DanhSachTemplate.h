#ifndef DANHSACHTEMPLATE_H
#define DANHSACHTEMPLATE_H

#include <vector>
#include <iostream>

template <typename T>
class CDanhSach {
private:
    std::vector<T*> m_vecData;
public:
    CDanhSach() {}
    ~CDanhSach() {
        for (auto item : m_vecData) {
            delete item;
        }
        m_vecData.clear();
    }

    void them(T* item) {
        m_vecData.push_back(item);
    }

    int getSoLuong() const {
        return m_vecData.size();
    }

    T* getAt(int index) {
        if (index >= 0 && index < (int)m_vecData.size()) return m_vecData[index];
        return nullptr;
    }

    std::vector<T*>& getVector() {
        return m_vecData;
    }
};

#endif
