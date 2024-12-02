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
    std::unordered_map<int, int> pipe_inputs;  // Входные CS для труб
    std::unordered_map<int, int> pipe_outputs; // Выходные CS для труб
public:
    void delete_pipe(int pipe_id, std::unordered_map<int, Pipeline>& pipes);
    void delete_cs(int cs_id, std::unordered_map<int, CS>& css);
    void save_connections(std::ofstream& out);
    void load_connections(std::ifstream& in, std::unordered_map<int, Pipeline>& pipes, std::unordered_map<int, CS>& css);
    void check_connection(const std::unordered_map<int, Pipeline>& pipes, const std::unordered_map<int, CS>& css);
    void create_connection(std::unordered_map<int, Pipeline>& pipes, std::unordered_map<int, CS>& css);
    void topologicalSort(const std::unordered_map<int, Pipeline>& pipes, const std::unordered_map<int, CS>& css);

    friend class CS;
    friend class Pipeline;
};
#endif
