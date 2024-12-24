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

    std::vector<std::vector<int>> weightMatrix; // Матрица весов
    std::vector<std::vector<int>> capacityMatrix; // Матрица пропускных способностей

    void update_weight_matrix(const std::unordered_map<int, Pipeline>& pipes);

public:
    Line() = default;

    void delete_pipe(int pipe_id, std::unordered_map<int, Pipeline>& pipes);
    void delete_cs(int cs_id, std::unordered_map<int, CS>& css, const std::unordered_map<int, Pipeline>& pipes);
    void save_connections(std::ofstream& out);
    void load_connections(std::ifstream& in, std::unordered_map<int, Pipeline>& pipes, std::unordered_map<int, CS>& css);
    void check_connection(const std::unordered_map<int, Pipeline>& pipes, const std::unordered_map<int, CS>& css);
    void create_connection(std::unordered_map<int, Pipeline>& pipes, std::unordered_map<int, CS>& css);
    void topologicalSort(const std::unordered_map<int, Pipeline>& pipes, const std::unordered_map<int, CS>& css);
    void refresh_weight_matrix(const std::unordered_map<int, Pipeline>& pipes);
    void mydikstra(const std::unordered_map<int, CS>& css);
    friend class CS;
    friend class Pipeline;
    void fordFulkersonMaxFlow();
    void update_capacity_matrix(const std::unordered_map<int, Pipeline>& pipes);
};

#endif
