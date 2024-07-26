#pragma once

#include <config/Config.h>

class Model;
class ModelBuilder;
struct ModelBuilderFactory
{
    static std::unique_ptr<ModelBuilder> create(Model& model, const Config& config);
};

