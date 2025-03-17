#pragma once
#include <Enum.h>

DECL_ENUM(UIType,  Element,   Root,   Window,   Text,   Button,   Image,   ListBox,   InputText,   PropertyView,   RadioGroup,   CheckBox,   ComboBox,   Menu,   Container,   Gizmo,   CollapseFrame,   StyleEditor,   TabBar,   TabBarItem,   TouchPad,   LoadingIndicator,   ProgressBar,   ProgressBarCircular,   Plot);
DECL_ESTR(UIType, "Element", "Root", "Window", "Text", "Button", "Image", "ListBox", "InputText", "PropertyView", "RadioGroup", "CheckBox", "ComboBox", "Menu", "Container", "Gizmo", "CollapseFrame", "StyleEditor", "TabBar", "TabBarItem", "TouchPad", "LoadingIndicator", "ProgressBar", "ProgressBarCircular", "Plot");

DECL_ENUM(HorizontalAlign,  Left,   Center,   Right);
DECL_ESTR(HorizontalAlign, "Left", "Center", "Right");

DECL_ENUM(VerticalAlign,  Top,   Center,   Bottom);
DECL_ESTR(VerticalAlign, "Top", "Center", "Bottom");
