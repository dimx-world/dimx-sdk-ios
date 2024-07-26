#pragma once
#include <Common.h>
#include <world/UIController2D.h>

class EditorDimension;
class EditorUIManager;
class Dimension;
class Location;

class EditorUIController: public UIController2D
{
public:
    EditorUIController(UIManager* mgr, ui::WindowPtr window);
    EditorUIManager& editorManager();
    EditorDimension& editor();
    Dimension* selectedDimension();
    Location* selectedLocation();
};
