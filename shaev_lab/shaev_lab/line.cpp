#include "line.h"
#include "pipeline.h"
#include "cs.h"

#include <vector>
#include <queue>
#include <unordered_map>
#include <iostream>

using std::cout;

class Pipeline;
class CS;


void Line::check_connection(const std::unordered_map<int, Pipeline>& pipes, const std::unordered_map<int, CS>& css)
{
	for (const auto& p : pipes)
	{
		if (p.second.GetInput() != -1 && p.second.GetOutput() != -1)
		{
			cout << "Pipe id=" << p.second.GetID() << " have connection input from CS id=" << p.second.GetInput() << "out CS id=" << p.second.GetOutput() << std::endl;
		}
		else if (p.second.GetInput() != -1 && p.second.GetOutput() == -1)
		{
			cout << "Pipe id=" << p.second.GetID() << " have connection input from CS id=" << p.second.GetInput() << std::endl;
		}
		else if (p.second.GetInput() == -1 && p.second.GetOutput() != -1)
		{
			cout << "Pipe id=" << p.second.GetID() << " have coneaction output to CS id=" << p.second.GetOutput() << std::endl;
		}
	}
};

void Line::create_connection(std::unordered_map<int, Pipeline>& pipes, std::unordered_map<int, CS>& css)
{
    int pipe_id = read_input<int>("Enter ID of pipe: ", 0, pipes.size() - 1);

    if (pipes.find(pipe_id) == pipes.end()) {
        std::cout << "Error: pipe ID = " << pipe_id << " not found." << std::endl;
        return;
    }

    std::cout << "Choice\nCreate input CS for pipe - 1\nCreate output CS for pipe - 2\nCreate input and output CS for pipe - 3\nExit - 0";
    int choice = read_input<int>("Enter choice: ", 0, 3);

    if (choice == 0) { return; }

    // ??????? ID ???????, ??????? ??? ?????? ??? ??????? ??? ????????
    std::unordered_map<int, bool> usedAsInput, usedAsOutput;
    for (const auto& pipe : pipes) {
        if (pipe.second.GetInput() != -1) {
            usedAsInput[pipe.second.GetInput()] = true;
        }
        if (pipe.second.GetOutput() != -1) {
            usedAsOutput[pipe.second.GetOutput()] = true;
        }
    }

    if (choice == 1 || choice == 3) // ???????? ??????? CS
    {
        std::cout << "Create CS - 1\nEnter ID of CS - 2\n";
        int input_cs_id = read_input<int>("", 1, 2);
        if (input_cs_id == 1) {
            std::cin >> css;
            input_cs_id = css.size() - 1;
        }
        else {
            // ???????? ?????? ????????? ??? ????? CS
            std::cout << "Available CS for input:\n";
            for (const auto& cs : css) {
                if (!usedAsOutput[cs.first]) { // ??????? ?? ?????? ???? ?????? ??? ?????
                    std::cout << "CS ID: " << cs.first << "\n";
                }
            }
            input_cs_id = read_input<int>("Enter ID of input CS: ", 0, css.size() - 1);
        }

        if (css.find(input_cs_id) == css.end() || usedAsOutput[input_cs_id]) {
            std::cout << "Error: CS with ID " << input_cs_id << " is not available as input." << std::endl;
            return;
        }
        pipes[pipe_id].SetInput(input_cs_id);
    }

    if (choice == 2 || choice == 3) // ???????? ???????? CS
    {
        std::cout << "Create CS - 1\nEnter ID of CS - 2\n";
        int output_cs_id = read_input<int>("", 1, 2);
        if (output_cs_id == 1) {
            std::cin >> css;
            output_cs_id = css.size() - 1;
        }
        else {
            // ???????? ?????? ????????? ??? ?????? CS
            std::cout << "Available CS for output:\n";
            for (const auto& cs : css) {
                if (!usedAsInput[cs.first]) { // ??????? ?? ?????? ???? ?????? ??? ????
                    std::cout << "CS ID: " << cs.first << "\n";
                }
            }
            output_cs_id = read_input<int>("Enter ID of output CS: ", 0, css.size() - 1);
        }

        if (css.find(output_cs_id) == css.end() || usedAsInput[output_cs_id]) {
            std::cout << "Error: CS with ID " << output_cs_id << " is not available as output." << std::endl;
            return;
        }
        pipes[pipe_id].SetOutput(output_cs_id);
    }
}

void Line::topologSort(const std::unordered_map<int, Pipeline>& pipes, const std::unordered_map<int, CS>& stations)
{
    std::vector<int> result;  // ?????? ??? ???????? ?????????? ??????????
    std::unordered_map<int, int> enterEdges;  // ??????? ???????? ?????? ??? ?????? ???????

    // ????????? ???????? ???????? ?????? ?????? ??? ???????, ??????? ??????????
    for (const auto& pipe : pipes) {
        if (pipe.second.GetOutput() != -1) {
            enterEdges[pipe.second.GetOutput()]++;
        }
    }

    std::queue<int> que;  // ??????? ??? ??????? ??? ???????? ??????, ??????????? ? ???????
    for (const auto& station : stations) {
        // ????????? ?????? ???????, ??????? ????????? ? ???????????
        if (enterEdges.find(station.first) == enterEdges.end()) {
            for (const auto& pipe : pipes) {
                if (pipe.second.GetInput() == station.first) {
                    que.push(station.first);
                    break;
                }
            }
        }
    }

    // ???????? ???? ?????????
    while (!que.empty()) {
        int curStation = que.front();
        que.pop();
        result.push_back(curStation);

        for (const auto& pipe : pipes) {
            if (pipe.second.GetInput() == curStation) {
                int outputStation = pipe.second.GetOutput();
                enterEdges[outputStation]--;

                if (enterEdges[outputStation] == 0) {
                    que.push(outputStation);
                }
            }
        }
    }

    // ????? ??????????
    std::cout << "Topological Sort Result: ";
    for (int stationID : result) {
        std::cout << "Station ID: " << stationID << " ";
    }
    std::cout << std::endl;
}
