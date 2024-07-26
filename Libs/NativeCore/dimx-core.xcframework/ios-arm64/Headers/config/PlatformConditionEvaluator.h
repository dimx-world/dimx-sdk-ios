#pragma once

#ifndef DIMX_CORE_PLATFORM_CONDITION_EVALUATOR_H
#define DIMX_CORE_PLATFORM_CONDITION_EVALUATOR_H

#include "ConditionEvaluator.h"
#include "Platform.h"
#include <string>


/*
  {
    "type" : "patform",
    "value" : "Windows" | "Web" | "Linux" | "Android" | "iOS" | "MagicLeap" | "Desktop" | "Mobile" | "XR"
*/

class PlatformConditionEvaluator : public ConditionEvaluator
{
public:
    ~PlatformConditionEvaluator() override = default;

    const char* type() const override;
    bool evaluate(const Config& config) override;
};

#endif // DIMX_CORE_PLATFORM_CONDITION_EVALUATOR_H