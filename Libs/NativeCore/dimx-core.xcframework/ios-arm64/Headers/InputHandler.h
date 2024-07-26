#pragma once

#ifndef DIMX_CORE_INPUT_HANDLER_H
#define DIMX_CORE_INPUT_HANDLER_H

class Input;
class InputHandler
{
public:
    virtual void onKeyPressed(const InputKey key, const KeyStatus status) {}

private:
    Input* mInput;
};

#endif // DIMX_CORE_INPUT_HANDLER_H