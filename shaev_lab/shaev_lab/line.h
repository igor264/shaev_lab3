#pragma once
#ifndef LINE_H_
#define LINE_H_

#include "logs.h"
#include "pipeline.h"
#include "cs.h"

#include <unordered_map>

class Pipeline;
class CS;

class Line
{
public:
	void check_connection(const std::unordered_map<int, Pipeline>& pipes, const std::unordered_map<int, CS>& css);
	void create_connecton(std::unordered_map<int, Pipeline>& pipes, const std::unordered_map<int, CS>& css);
};

#endif