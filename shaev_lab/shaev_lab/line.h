#pragma once
#ifndef LINE_H_
#define LINE_H_

#include "logs.h"
#include "pipeline.h"
#include "cs.h"

#include <unordered_map>
#include <vector>

class Pipeline;
class CS;

class Line
{
private:
    static int MaxIDPipe;
    static int MaxIDCS;
    std::unordered_map<int, std::vector<int>> cs_inputs;  // Входящие трубы для CS
    std::unordered_map<int, std::vector<int>> cs_outputs; // Исходящие трубы для CS
    std::unordered_map<int, int> pipe_inputs;  // Входные CS для труб
    std::unordered_map<int, int> pipe_outputs; // Выходные CS для труб

public:
    void check_connection(const std::unordered_map<int, Pipeline>& pipes, const std::unordered_map<int, CS>& css);
    void create_connection(std::unordered_map<int, Pipeline>& pipes, std::unordered_map<int, CS>& css);
    void topologicalSort(const std::unordered_map<int, Pipeline>& pipes, const std::unordered_map<int, CS>& css);

    friend class CS;
    friend class Pipeline;
};
#endif
