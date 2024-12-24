#include "line.h"
#include "pipeline.h"
#include "cs.h"

#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <fstream>

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
    refresh_weight_matrix(pipes);
}

void Line::update_weight_matrix(const std::unordered_map<int, Pipeline>& pipes) {
    int n = MaxIDCS + 1;
    weightMatrix.assign(n, std::vector<int>(n, std::numeric_limits<int>::max())); // Инициализируем матрицу весов

    for (const auto& input : pipe_inputs) {
        int pipe_id = input.first;
        int input_cs = input.second;

        if (pipe_outputs.count(pipe_id)) {
            int output_cs = pipe_outputs.at(pipe_id);

            if (input_cs != -1 && output_cs != -1) {
                const auto& pipe = pipes.at(pipe_id);
                if (!pipe.RepairIndicator) { // Проверяем, не находится ли труба в ремонте
                    // Выбираем минимальный вес, если уже есть связь
                    if (weightMatrix[input_cs][output_cs] > pipe.LengthOfPipe) {
                        weightMatrix[input_cs][output_cs] = pipe.LengthOfPipe;
                    }
                }
            }
        }
    }

    // Удаляем значения "бесконечности" (неактуальные связи)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (weightMatrix[i][j] == std::numeric_limits<int>::max()) {
                weightMatrix[i][j] = 0; // Сбрасываем "пустые" связи
            }
        }
    }
    update_capacity_matrix(pipes);
}


void Line::refresh_weight_matrix(const std::unordered_map<int, Pipeline>& pipes) {
    update_weight_matrix(pipes);
    cout << "Weight matrix has been updated.\n";
}


void Line::topologicalSort(const std::unordered_map<int, Pipeline>& pipes, const std::unordered_map<int, CS>& css) {
    if (weightMatrix.empty()) {
        cout << "Weight matrix is empty. Updating...\n";
        update_weight_matrix(pipes);
    }

    int n = weightMatrix.size();
    std::vector<int> inDegree(n, 0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (weightMatrix[i][j] > 0) {
                ++inDegree[j];
            }
        }
    }

    std::queue<int> zeroDegreeQueue;
    for (int i = 0; i < n; ++i) {
        if (inDegree[i] == 0) {
            zeroDegreeQueue.push(i);
        }
    }

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

    if (sortedOrder.size() != n) {
        std::cerr << "Error: Graph contains a cycle\n";
        return;
    }

    cout << "Topological Sort Order (CS IDs):\n";
    for (int id : sortedOrder) {
        cout << id << " ";
    }
    cout << "\n";
}

void Line::delete_pipe(int pipe_id, std::unordered_map<int, Pipeline>& pipes) {
    if (pipes.find(pipe_id) == pipes.end()) {
        cout << "Error: Pipe ID not found.\n";
        return;
    }

    // Удаляем соединения, связанные с трубой
    pipe_inputs.erase(pipe_id);
    pipe_outputs.erase(pipe_id);

    // Удаляем саму трубу
    pipes.erase(pipe_id);
    cout << "Pipe with ID " << pipe_id << " has been deleted.\n";
    refresh_weight_matrix(pipes);
}

void Line::delete_cs(int cs_id, std::unordered_map<int, CS>& css, const std::unordered_map<int, Pipeline>& pipes) {
    if (css.find(cs_id) == css.end()) {
        cout << "Error: CS ID not found.\n";
        return;
    }

    // Удаляем все соединения, связанные с данным КС
    for (auto it = pipe_inputs.begin(); it != pipe_inputs.end(); ) {
        if (it->second == cs_id) {
            it = pipe_inputs.erase(it);
        }
        else {
            ++it;
        }
    }

    for (auto it = pipe_outputs.begin(); it != pipe_outputs.end(); ) {
        if (it->second == cs_id) {
            it = pipe_outputs.erase(it);
        }
        else {
            ++it;
        }
    }

    // Удаляем сам КС
    css.erase(cs_id);
    cout << "CS with ID " << cs_id << " has been deleted.\n";
    refresh_weight_matrix(pipes);
}

void Line::save_connections(std::ofstream& out)  {
    for (const auto& con : pipe_outputs) {
        out << "@connection@\n";
        out << "OUTPUT " << con.first << " " << con.second << "\n";
    }

    for (const auto& con : pipe_inputs) {
        out << "@connection@\n";
        out << "INPUT " << con.first << " " << con.second << "\n";
    }

    std::cout << "Connections successfully saved.\n";
}



void Line::load_connections(std::ifstream& in, std::unordered_map<int, Pipeline>& pipes, std::unordered_map<int, CS>& css)
{
    std::string line;
    while (std::getline(in, line)) {
        if (line == "@connection@") {
            int pipe_id, cs_id;
            std::string connection_type;

            if (!(in >> connection_type)) {
                std::cout << "Error: Missing connection type in file. Skipping entry.\n";
                in.clear();
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (!(in >> pipe_id) || pipes.find(pipe_id) == pipes.end()) {
                std::cout << "Error: Invalid or non-existent pipe ID in file. Skipping entry.\n";
                in.clear();
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (!(in >> cs_id) || css.find(cs_id) == css.end()) {
                std::cout << "Error: Invalid or non-existent CS ID in file. Skipping entry.\n";
                in.clear();
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (connection_type == "OUTPUT") {
                pipe_outputs[pipe_id] = cs_id;  // Устанавливаем выход трубы
            }
            else if (connection_type == "INPUT") {
                pipe_inputs[pipe_id] = cs_id;  // Устанавливаем вход трубы
            }
            else {
                std::cout << "Error: Unknown connection type '" << connection_type << "'. Skipping entry.\n";
                in.clear();
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    std::cout << "Connections successfully loaded from " << ".\n";
    in.close();
    refresh_weight_matrix(pipes);
}

void Line::mydikstra(const std::unordered_map<int, CS>& css) {
    int n = weightMatrix.size();

    // Запрос начальной и конечной КС
    int start, end;
    std::cout << "Enter the ID of the starting compressor station (0 to " << n - 1 << "): ";
    std::cin >> start;
    std::cout << "Enter the ID of the ending compressor station (0 to " << n - 1 << "): ";
    std::cin >> end;

    if (start < 0 || start >= n || end < 0 || end >= n) {
        std::cout << "Error: Invalid CS IDs. Please try again.\n";
        return;
    }

    std::vector<int> distances(n, std::numeric_limits<int>::max()); // Дистанции от начальной вершины
    std::vector<int> predecessors(n, -1); // Для восстановления пути
    distances[start] = 0;

    // Очередь минимального приоритета (расстояние, вершина)
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.push({ 0, start });

    while (!pq.empty()) {
        auto top = pq.top();
        int current_distance = top.first;
        int current_node = top.second;
        pq.pop();

        if (current_distance > distances[current_node]) {
            continue;
        }

        for (int neighbor = 0; neighbor < n; ++neighbor) {
            if (weightMatrix[current_node][neighbor] > 0) { // Есть соединение
                int new_distance = current_distance + weightMatrix[current_node][neighbor];
                if (new_distance < distances[neighbor]) {
                    distances[neighbor] = new_distance;
                    predecessors[neighbor] = current_node;
                    pq.push({ new_distance, neighbor });
                }
            }
        }
    }

    // Вывод результата
    if (distances[end] == std::numeric_limits<int>::max()) {
        std::cout << "No path exists from CS " << start << " to CS " << end << ".\n";
        return;
    }

    std::cout << "Shortest distance from CS " << start << " to CS " << end << ": " << distances[end] << "\n";

    // Восстановление пути
    std::vector<int> path;
    for (int at = end; at != -1; at = predecessors[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    std::cout << "Path:\n";
    for (size_t i = 0; i < path.size(); ++i) {
        int cs_id = path[i];
        std::cout << "CS ID: " << cs_id;

        if (i < path.size() - 1) {
            std::cout << " --> ";
        }
        else {
            std::cout << "\n";
        }
    }
}


void Line::update_capacity_matrix(const std::unordered_map<int, Pipeline>& pipes) {
    int n = MaxIDCS + 1;
    capacityMatrix.assign(n, std::vector<int>(n, 0)); // Матрица пропускных способностей

    for (const auto& input : pipe_inputs) {
        int pipe_id = input.first;
        int input_cs = input.second;

        if (pipe_outputs.count(pipe_id)) {
            int output_cs = pipe_outputs.at(pipe_id);

            if (input_cs != -1 && output_cs != -1) {
                const auto& pipe = pipes.at(pipe_id);
                if (!pipe.RepairIndicator) { // Проверяем, не находится ли труба в ремонте
                    capacityMatrix[input_cs][output_cs] += (pipe.Diameter)^5 / (pipe.LengthOfPipe * 10) ; // Используем диаметр как пропускную способность
                }
            }
        }
    }

    std::cout << "Capacity matrix has been updated.\n";
}

bool bfs(const std::vector<std::vector<int>>& residualGraph, int source, int sink, std::vector<int>& parent) {
    int n = residualGraph.size();
    std::vector<bool> visited(n, false);
    std::queue<int> q;

    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int next = 0; next < n; ++next) {
            if (!visited[next] && residualGraph[current][next] > 0) {
                q.push(next);
                parent[next] = current;
                visited[next] = true;

                if (next == sink) {
                    return true; // Достигли стока
                }
            }
        }
    }

    return false;
}

void Line::fordFulkersonMaxFlow() {
    int start, end;
    std::cout << "Enter the ID of the starting compressor station (0 to " << capacityMatrix.size() - 1 << "): ";
    std::cin >> start;
    std::cout << "Enter the ID of the ending compressor station (0 to " << capacityMatrix.size() - 1 << "): ";
    std::cin >> end;
    int n = capacityMatrix.size();
    std::vector<std::vector<int>> residualGraph = capacityMatrix;
    std::vector<int> parent(n, -1);
    int maxFlow = 0;

    while (bfs(residualGraph, start, end, parent)) {
        int pathFlow = std::numeric_limits<int>::max();
        std::vector<int> path; // Для сохранения текущего пути

        // Определяем минимальную пропускную способность на пути
        for (int v = end; v != start; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, residualGraph[u][v]);
        }

        // Обновляем остаточный граф и сохраняем путь
        for (int v = end; v != start; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
            path.push_back(v); // Добавляем вершину пути
        }
        path.push_back(start); // Добавляем начальную вершину
        std::reverse(path.begin(), path.end()); // Путь идет от start к end

        // Вывод пути
        std::cout << "Augmenting path found: ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i];
            if (i != path.size() - 1) std::cout << " -> ";
        }
        std::cout << " with flow: " << pathFlow << "\n";

        maxFlow += pathFlow; // Увеличиваем общий поток
    }

    std::cout << "The maximum possible flow from CS " << start << " to CS " << end << " is: " << maxFlow << "\n";
}
