#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

#include "line.h"
class Line;
class CS
{
private:
    int CSID = 0;
    std::string CSName = "None";
    int NumberOfWorkshops = -1;
    int WorkshopsInWork = -1;
    short int Efficiency = -1;
public:

    CS();
    int GetID() const { return CSID; };

    void create_cs(std::unordered_map<int, CS>& css) { std::cin >> css; };
    friend void change_workshops_in_work(CS& cs, int& new_WorkshopsInWork);
    friend std::istream& operator >> (std::istream& in, std::unordered_map<int, CS>& css);
    friend std::ostream& operator << (std::ostream& out, const CS& cs);
    friend std::ofstream& operator << (std::ofstream& out, const CS& cs);
    friend std::ifstream& operator >> (std::ifstream& in, std::unordered_map<int, CS>& css);

    friend void filter_cs_by_name(std::unordered_map<int, CS>& cs);
    friend void filter_cs_by_work_percentage(std::unordered_map<int, CS>& cs);
    friend class Line;
};

std::ostream& operator << (std::ostream& out, const std::unordered_map<int, CS>& css);
std::ofstream& operator << (std::ofstream& out, const std::unordered_map<int, CS>& css);