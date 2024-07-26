#pragma once

#ifndef DIMX_CORE_CONDITION_EVALUATOR_H
#define DIMX_CORE_CONDITION_EVALUATOR_H

class Config;

class ConditionEvaluator
{
public:
    virtual ~ConditionEvaluator() = default;

    virtual const char* type() const = 0;
    virtual bool evaluate(const Config& config) = 0;
};

#endif // DIMX_CORE_CONDITION_EVALUATOR_H