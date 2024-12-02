#include "line.h"
#include "pipeline.h"
#include "cs.h"

#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>

using std::cout;

void Line::check_connection(const std::unordered_map<int, Pipeline>& pipes, const std::unordered_map<int, CS>& css) {
    for (const auto& pipe : pipes) {
        int input_cs = pipe_inputs.count(pipe.first) ? pipe_inputs.at(pipe.first) : -1;
        int output_cs = pipe_outputs.count(pipe.first) ? pipe_outputs.at(pipe.first) : -1;

        if (input_cs != -1 && output_cs != -1) {
            cout << "CS id=" << input_cs << " --> Pipe id=" << pipe.first
                << " --> CS id=" << output_cs << "\n";
        }
        else if (input_cs != -1) {
            cout << "CS id=" << input_cs << " --> Pipe id=" << pipe.first << "\n";
        }
        else if (output_cs != -1) {
            cout << "Pipe id=" << pipe.first << " --> CS id=" << output_cs << "\n";
        }
        else {
            cout << "Pipe id=" << pipe.first << " is not connected.\n";
        }
    }
}

void Line::create_connection(std::unordered_map<int, Pipeline>& pipes, std::unordered_map<int, CS>& css) {
    cout << "Choose connection type:\n1. Connect Pipe --> CS\n2. Connect CS --> Pipe\n0. Exit\n";
    int choice = read_input<int>("Enter choice: ", 0, 2);

    while (choice != 0) {
        int pipe_id = read_input<int>("Enter Pipe ID: ", 0, pipes.size() - 1);
        if (pipes.find(pipe_id) == pipes.end()) {
            cout << "Error: Pipe ID not found.\n";
            continue;
        }

        if (choice == 1) {
            int cs_id = read_input<int>("Enter CS ID (Output): ", 0, css.size() - 1);
            if (css.find(cs_id) == css.end()) {
                cout << "Error: CS ID not found.\n";
                continue;
            }
            pipe_outputs[pipe_id] = cs_id;  // Устанавливаем выход трубы
        }
        else if (choice == 2) {
            int cs_id = read_input<int>("Enter CS ID (Input): ", 0, css.size() - 1);
            if (css.find(cs_id) == css.end()) {
                cout << "Error: CS ID not found.\n";
                continue;
            }
            pipe_inputs[pipe_id] = cs_id;  // Устанавливаем вход трубы
        }

        choice = read_input<int>("Choose connection type (1/2/0): ", 0, 2);
    }
}

void Line::topologicalSort(const std::unordered_map<int, Pipeline>& pipes, const std::unordered_map<int, CS>& css) {
    int n = css.size();
    std::vector<std::vector<int>> weightMatrix(n, std::vector<int>(n, 0));

    // Заполнение матрицы весов
    for (const auto& input : pipe_inputs) {
        auto& pipe_id = input.first;
        auto& input_cs = input.second;
        if (pipe_outputs.count(pipe_id)) {
            int output_cs = pipe_outputs.at(pipe_id);
            if (input_cs != -1 && output_cs != -1) {
                weightMatrix[input_cs][output_cs] = pipes.at(pipe_id).LengthOfPipe;
            }
        }
    }

    // Вывод матрицы весов
    cout << "Weight Matrix (Length of Pipes):\n";
    cout << "   ";
    for (int i = 0; i < n; ++i) {
        cout << i << " ";
    }
    cout << "\n";

    for (int i = 0; i < n; ++i) {
        cout << i << ": ";
        for (int j = 0; j < n; ++j) {
            cout << weightMatrix[i][j] << " ";
        }
        cout << "\n";
    }

    // Рассчитываем входящие степени
    std::vector<int> inDegree(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (weightMatrix[i][j] > 0) {
                ++inDegree[j];
            }
        }
    }

    // Очередь вершин с входящей степенью 0
    std::queue<int> zeroDegreeQueue;
    for (int i = 0; i < n; ++i) {
        if (inDegree[i] == 0) {
            zeroDegreeQueue.push(i);
        }
    }

    // Топологическая сортировка
    std::vector<int> sortedOrder;
    while (!zeroDegreeQueue.empty()) {
        int current = zeroDegreeQueue.front();
        zeroDegreeQueue.pop();
        sortedOrder.push_back(current);

        for (int i = 0; i < n; ++i) {
            if (weightMatrix[current][i] > 0) {
                if (--inDegree[i] == 0) {
                    zeroDegreeQueue.push(i);
                }
            }
        }
    }

    // Проверка на цикл
    if (sortedOrder.size() != n) {
        std::cerr << "Error: Graph contains a cycle or disconnected components.\n";
        return;
    }

    // Вывод результата
    cout << "Topological Sort Order (CS IDs):\n";
    for (int id : sortedOrder) {
        cout << id << " ";
    }
    cout << "\n";
}

