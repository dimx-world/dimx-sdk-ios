#pragma once
#include <Input.h>
#include <imgui.h>

using CoreImGuiKeyMapArray = std::array<int, (int)InputKey::Max + 1>;
const CoreImGuiKeyMapArray& coreToImGuiKeyMap();
