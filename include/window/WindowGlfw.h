#ifndef _GLFW_WINDOW_H
#define _GLFW_WINDOW_H

#include <string>
#include <GLFW/glfw3.h>

#include "core/Core.h"
#include "Window.h"

namespace sntl
{
    class ENGINE_API WindowGlfw : public IWindow
    {
    public:
        WindowGlfw(const std::string& title, int xpos, int ypos, int width, int height, WindowType type = WindowType::WINDOWED, bool maximized = false);
        ~WindowGlfw();

        void onTick() override;

        int getWidth() const override { return data_.width; }
        int getHeight() const override { return data_.height; }

        int getXPos() const override { return data_.xpos; }
        int getYPos() const override { return data_.ypos; }

        bool isOpen() const override { return open_; }

        bool isVSync() const override { return vSync_; }
        void setVSync(bool enabled) override;

        WindowType getWindowType() const override { return type_; }
        void setWindowType(WindowType windowType) override;

        void* getNativePtr() const override { return (void*)window_; }

    private:
        void initWindow();
        void killWindow();

    private:
        struct WindowData
        {
            int xpos;
            int ypos;

            int width;
            int height;
        };

        WindowData data_;
        WindowData formerData_;

        std::string title_;

        bool vSync_;
        bool maximized_;
        bool open_;

        WindowType type_;

        GLFWwindow* window_;
        GLFWmonitor* monitor_;
    };
}

#endif