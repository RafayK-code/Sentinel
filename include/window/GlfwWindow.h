#ifndef _GLFW_WINDOW_H
#define _GLFW_WINDOW_H

#include <string>
#include <GLFW/glfw3.h>

#include "Window.h"

namespace sntl
{
    class GlfwWindow : public Window
    {
    public:
        GlfwWindow(const std::string& name, uint32_t width, uint32_t height, bool fullscreen = false);
        ~GlfwWindow();

        void onTick() override;

        void getWidth() const override { return width_; }
        void getHeight() const override { return height_; }

        bool isVSync() const override { return vSync_};
        void setVSync(bool enabled) override;

        bool isFullScreen() const override { return fullscreen_; };

        void* getNativePtr() const override { return (void*)window_; }

    private:
        uint32_t width_;
        uint32_t height_;
        bool vSync_;
        bool fullscreen_;

        GLFWWindow* window_;
    }
}

#endif