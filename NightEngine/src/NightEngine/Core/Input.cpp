#include "hzpch.h"

#include "Input.h"
#include "Platform/Windows/Window.h"

#include <GLFW/glfw3.h>

namespace Night
{
    std::unique_ptr<Input> mInstance;
    std::unique_ptr<Input>& Input::GetInstance()
    {
        if (!mInstance)
        {
            mInstance = std::make_unique<Input>();
        }
        return mInstance;
    }

    /*!*************************************************************************
    Store key pressed input feedback in vector mReset
    ****************************************************************************/
    bool Input::KeyPressed(const int& key)
    {
        return mKeyStatus[key] == GLFW_PRESS;
    }

    /*!*************************************************************************
    Store key hold input feedback status in mKeyStatus
    ****************************************************************************/
    bool Input::KeyHold(const int& key)
    {
        return mKeyStatus[key] == GLFW_REPEAT || mKeyStatus[key] == GLFW_PRESS;
    }

    /*!*************************************************************************
    Store key released input feedback in both vector mReset and mKeyStatus
    ****************************************************************************/
    bool Input::KeyReleased(const int& key)
    {
        return mKeyStatus[key] == GLFW_RELEASE;
    }

    /*!*************************************************************************
    Set keystatus based on keypressed
    ****************************************************************************/
    void Input::SetKeyStatus(keycode key, keystatus status)
    {
        mReset.emplace_back(key);
        mKeyStatus[key] = status;
    }

    /*!*************************************************************************
    Resets mReset vector and mKeyStatus map to empty
    ****************************************************************************/
    void Input::ResetPressedKey()
    {
        for (auto& key : mReset)
        {
            if (mKeyStatus[key] == GLFW_PRESS)
                mKeyStatus[key] = GLFW_REPEAT;
        }

        mReset.clear();
    }

    /*!*************************************************************************
    Store mouse input feedback if mouse is pressed
    ****************************************************************************/
    bool Input::MousePressed(const int& key)
    {
        return mMouseStatus[key] == GLFW_PRESS;
    }

    /*!*************************************************************************
    Store mouse input feedback if mouse is held
    ****************************************************************************/
    bool Input::MouseHold(const int& key)
    {
        return  mMouseStatus[key] == GLFW_REPEAT;
    }

    /*!*************************************************************************
    Store mouse input feedback if mouse is released
    ****************************************************************************/
    bool Input::MouseIsReleased(const int& key)
    {
        return mMouseStatus[key] == GLFW_RELEASE;
    }

    /*!*************************************************************************
    Set mouse status
    ****************************************************************************/
    void Input::SetMouseStatus(mousecode key, mousestatus status)
    {
        mouseReset.emplace_back(key);
        mMouseStatus[key] = status;
    }

    /*!*************************************************************************
    Reset mouse input status in mMouseStatus to 0
    ****************************************************************************/
    void Input::ResetPressedMouse()
    {
        for (auto& key : mouseReset)
        {
            if (mMouseStatus[key] == GLFW_PRESS)
                mMouseStatus[key] = GLFW_REPEAT;
        }

        mouseReset.clear();
    }

    /*!*************************************************************************
    Checks if key is pressed by finding mKeyStatus
    ****************************************************************************/
    bool Input::isKeyPressed(keycode key)
    {
        auto it = mKeyStatus.find(key);
        if (it != mKeyStatus.end())
            return it->second == GLFW_PRESS; //find the key in the map and execute the order

        return false;
    }
}