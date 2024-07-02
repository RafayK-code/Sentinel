#ifndef _GLFW_WINDOW_H
#define _GLFW_WINDOW_H

#include <string>
#include <GLFW/glfw3.h>

#include "core/Core.h"
#include "Window.h"

namespace sntl
{
    class ENGINE_API GlfwWindow : public Window
    {
    public:
        GlfwWindow(const std::string& title, int xpos, int ypos, int width, int height, WindowType type = WindowType::WINDOWED);
        ~GlfwWindow();

        void onTick() override;

        int getWidth() const override { return data_.width; }
        int getHeight() const override { return data_.height; }

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
            std::string title;

            int xpos;
            int ypos;

            int width;
            int height;

            GlfwWindow* curr;
        };

        WindowData data_;
        WindowData formerData_;

        bool vSync_;

        WindowType type_;

        GLFWwindow* window_;
        GLFWmonitor* monitor_;
    };
}

#endif