#pragma once
#include <memory>

class Agent;
class Object;
class Config;
class AgentFactory
{
public:
    static std::unique_ptr<Agent> create(Object* object, const Config& config);
};
