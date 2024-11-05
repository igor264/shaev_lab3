#include "pipeline.h"
#include "logs.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::cout;
using std::cin;


int Pipeline::MaxIDPipe = 0;

Pipeline::Pipeline()
{
    PipeID = MaxIDPipe++;
}


std::istream& operator >> (std::istream& in, std::unordered_map<int, Pipeline>& pipes) {
    Pipeline pipe;

    pipe.PipeName = read_input<std::string>("Enter name pipe: ");
    pipe.LengthOfPipe = read_input<int>("Enter length: ", 0);
    pipe.Diameter = read_input<int>("Enter diameter: ", 0);
    pipe.RepairIndicator = read_input<int>("Enter repair (0 - working, 1 - on repairing): ", 0, 1);

    pipes.insert({ pipe.GetID(), pipe });
    return in;
}


std::ostream& operator << (std::ostream& out, const std::unordered_map<int, Pipeline>& pipes)
{
    if (pipes.size() == 0) { cout << "you have not pipes\n"; return out; }
    for (const auto& pipe : pipes) {
        out << pipe.second;
    }
    return out;
};

std::ostream& operator << (std::ostream& out, const Pipeline& pipe)
{
    out << "\nID: " << pipe.PipeID
        << "\nPipe name: " << pipe.PipeName
        << "\nPipe length: " << pipe.LengthOfPipe
        << "\nPipe diameter: " << pipe.Diameter
        << "\nThe pipe attribute: " << pipe.RepairIndicator << std::endl;
    return out;
};

std::ofstream& operator << (std::ofstream& out, const std::unordered_map<int, Pipeline>& pipes)
{
    for (const auto& pipe : pipes) {
        out << pipe.second;
    }
    return out;
}

std::ofstream& operator << (std::ofstream& out, const Pipeline& pipe)
{
    out << "@pipeline@" << '\n'
        << pipe.PipeName << '\n'
        << pipe.LengthOfPipe << '\n'
        << pipe.Diameter << '\n'
        << pipe.RepairIndicator << '\n';
    return out;
};


std::ifstream& operator >> (std::ifstream& in, std::unordered_map<int, Pipeline>& pipes) {
    std::string line;
    while (std::getline(in, line)) {
        if (line == "@pipeline@") {
            Pipeline pipe;

            if (!std::getline(in, pipe.PipeName) || pipe.PipeName.empty()) {
                cout << "Error pipe in file\n";
                continue; 
            }

            if (!(in >> pipe.LengthOfPipe) || pipe.LengthOfPipe < 0) {
                in.clear(); in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "Error pipe in file\n";
                continue;
            }

            if (!(in >> pipe.Diameter) || pipe.Diameter <= 0) {
                in.clear(); in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "Error pipe in file\n";
                continue;
            }

            if (!(in >> pipe.RepairIndicator) || (pipe.RepairIndicator != 0 && pipe.RepairIndicator != 1)) {
                in.clear(); in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "Error pipe in file\n";
                continue;
            }

            pipes.insert({ pipe.GetID(), pipe });
            //std::cerr << "1\n" << pipe.PipeName << std::endl << pipe.LengthOfPipe << std::endl << pipe.Diameter << std::endl << pipe.RepairIndicator << std::endl;
        }
    }
    return in;
}

void ChangeRepair(Pipeline& pipe, bool repair)
{
    pipe.RepairIndicator = repair;
}

void filter_pipes_by_name(std::unordered_map<int, Pipeline>& pipes)
{
    std::string name = read_input<std::string>("Enter the name or part of the name of the pipe to filter: ");
    std::vector<int> ids;
    bool flag = false;

    for (const auto& p : pipes) {
        if (p.second.PipeName.find(name) != std::string::npos) {
            cout << p.second;
            ids.push_back(p.second.PipeID);
            flag = true;
        }
    }

    if (!flag) { cout << "You don't have pipe with name: " << name << std::endl; return; }

    int ch = read_input<int>("0 - menu:\n1 - chenge pipes\n", 0, 1);

    if (ch == 0) { return; }
    ch = read_input<int>("Enter repair", 0, 1);

    for (const auto& id : ids)
    {
        ChangeRepair(pipes[id], ch);
    }
}

void filter_pipes_by_repair(std::unordered_map<int, Pipeline>& pipes)
{
    bool repair_status = read_input<bool>("Enter repair status (0 - working, 1 - on repairing): ");
    std::vector<int> ids;
    bool flag = false;

    for (const auto& p : pipes) {
        if (p.second.RepairIndicator == repair_status) {
            cout << p.second;
            ids.push_back(p.second.PipeID);
            flag = true;
        }
    }

    if (!flag) { cout << "You don't have pipe with repair: " << repair_status << std::endl; return; }

    int ch = read_input<int>("0 - menu:\n1 - chenge pipes\n", 0, 1);

    if (ch == 0) { return; }
    ch = read_input<int>("Enter repair", 0, 1);

    for (const auto& id : ids)
    {
        ChangeRepair(pipes[id], ch);
    }
}

