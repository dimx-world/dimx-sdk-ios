#pragma once

#ifndef DIMX_CORE_CONDITION_PROCESSOR_H
#define DIMX_CORE_CONDITION_PROCESSOR_H

#include "ConditionEvaluator.h"
#include "CommonUtils.h"
#include <map>
#include <string>
#include <memory>

class Config;

class ConditionProcessor
{
    using ConditionEvaluatorPtr = std::unique_ptr<ConditionEvaluator>;

public:
    ConditionProcessor();
    ~ConditionProcessor() = default;

    template<class E>
    void addEvaluator()
    {
        auto e = std::make_unique<E>();
        auto result = mEvaluators.emplace(std::pair<std::string, ConditionEvaluatorPtr>(e->type(), std::move(e)));
        ASSERT(result.second, "This evaluator type has been already registered");
    }

    void process(Config& confug);

private:
    bool evaluateCases(const Config& config, Config& value);
    bool evaluateCondition(const Config& config);

    std::map<std::string, ConditionEvaluatorPtr> mEvaluators;
};

#endif // DIMX_CORE_CONDITION_PROCESSOR_H