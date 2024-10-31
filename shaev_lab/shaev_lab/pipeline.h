#pragma once
#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <string>
#include <iostream>
#include <unordered_map>


class Pipeline
{
private:
    static int MaxIDPipe;
    int PipeID = 0;
    int LengthOfPipe = 0;
    short int Diameter = 0;
    std::string PipeName = "None";
    bool RepairIndicator = 0;
public:

    Pipeline();
    int GetID() const { return PipeID; };
    friend void ChangeRepair(Pipeline& pipe, bool repair);
    friend std::istream& operator >> (std::istream& in, std::unordered_map<int, Pipeline>& pipes);
    friend std::ostream& operator << (std::ostream& out, const Pipeline& pipe);
    friend std::ofstream& operator << (std::ofstream& out, const Pipeline& pipe);
    friend std::ifstream& operator >> (std::ifstream& in, std::unordered_map<int, Pipeline>& pipes);

    friend void filter_pipes_by_name(std::unordered_map<int, Pipeline>& pipes);
    friend void filter_pipes_by_repair(std::unordered_map<int, Pipeline>& pipes);
};

std::ostream& operator << (std::ostream& out, const std::unordered_map<int, Pipeline>& pipes);
std::ofstream& operator << (std::ofstream& out, const std::unordered_map<int, Pipeline>& pipes);

#endif