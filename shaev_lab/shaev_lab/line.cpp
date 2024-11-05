#include "line.h"
#include "pipeline.h"
#include "cs.h"


#include <unordered_map>
#include <iostream>

using std::cout;


void Line::check_connection(const std::unordered_map<int, Pipeline>& pipes, const std::unordered_map<int, CS>& css)
{
	for (const auto& p : pipes)
	{
		if (p.second.GetInput() != -1 && p.second.GetOutput() != -1)
		{
			cout << "Pipe id=" << p.second.GetID() << " have conaction input from CS id=" << p.second.GetInput() << "out CS id=" << p.second.GetOutput() << std::endl;
		}
		else if (p.second.GetInput() != -1 && p.second.GetOutput() == -1)
		{
			cout << "Pipe id=" << p.second.GetID() << " have conaction input from CS id=" << p.second.GetInput() << std::endl;
		}
		else if (p.second.GetInput() == -1 && p.second.GetOutput() != -1)
		{
			cout << "Pipe id=" << p.second.GetID() << " have conaction output to CS id=" << p.second.GetOutput() << std::endl;
		}
	}
};