#include "window/GlfwWindow.h"

#include "core/Dbg.h"
#include "events/WindowEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

namespace sntl
{
    static uint8_t windowCount = 0;

    GlfwWindow::GlfwWindow(const std::string& title, int xpos, int ypos, int width, int height, WindowType windowType)
        : data_({title, xpos, ypos, width, height, this}), type_(windowType), monitor_(nullptr)
    {
        initWindow();
    }

    GlfwWindow::~GlfwWindow()
    {
        killWindow();
    }

    void GlfwWindow::initWindow()
    {
        if (windowCount == 0)
            DBG_ASSERT(glfwInit(), "Could not initialize glfw");

        monitor_ = glfwGetPrimaryMonitor();
        DBG_ASSERT(monitor_, "Could not get primary monitor");

        if (type_ == WindowType::FULLSCREEN)
        {
            const GLFWvidmode* mode = glfwGetVideoMode(monitor_);
            window_ = glfwCreateWindow(mode->width, mode->height, data_.title.c_str(), monitor_, nullptr);
            DBG_ASSERT(window_, "Could not create GLFW window");

            int monitorX, monitorY;
            glfwGetMonitorPos(monitor_, &monitorX, &monitorY);
            
            formerData_.width = data_.width;
            formerData_.height = data_.height;
            formerData_.xpos = monitorX + (mode->width - data_.width) / 2;
            formerData_.ypos = monitorY + (mode->height - data_.height) / 2;

            data_.width = mode->width;
            data_.height = mode->height;
            data_.xpos = monitorX;
            data_.ypos = monitorY;
        }

        else
        {
            if (type_ == WindowType::WINDOWED_MAXIMIZED || type_ == WindowType::BORDERLESS_MAXIMIZED)
            {
                const GLFWvidmode* mode = glfwGetVideoMode(monitor_);
                int monitorX, monitorY;
                glfwGetMonitorPos(monitor_, &monitorX, &monitorY);

                data_.xpos = monitorX;
                data_.ypos = monitorY;
                data_.width = mode->width;
                data_.height = mode->height;
            }

            if (type_ == WindowType::BORDERLESS || type_ == WindowType::BORDERLESS_MAXIMIZED)
                glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            else 
                glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

            window_ = glfwCreateWindow(data_.width, data_.height, data_.title.c_str(), nullptr, nullptr);
            glfwSetWindowPos(window_, data_.xpos, data_.ypos);
            DBG_ASSERT(window_, "Could not create GLFW window");
        }

        windowCount++;

        glfwSetWindowUserPointer(window_, &data_);
        setVSync(true);

        // setup callbacks

        glfwSetWindowCloseCallback(window_, [](GLFWwindow* window)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            data.curr->dispatchEvent(event);
        });

        glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.width = width;
            data.height = height;

            WindowResizeEvent event(width, height);
            data.curr->dispatchEvent(event);
        });

        glfwSetWindowFocusCallback(window_, [](GLFWwindow* window, int focused)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            if (focused)
            {
                WindowFocusEvent event;
                data.curr->dispatchEvent(event);
            }

            else
            {
                WindowLostFocusEvent event;
                data.curr->dispatchEvent(event);
            }
        });

        glfwSetWindowPosCallback(window_, [](GLFWwindow* window, int xpos, int ypos)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.xpos = xpos;
            data.ypos = ypos;

            WindowMovedEvent event(xpos, ypos);
            data.curr->dispatchEvent(event);
        });

        glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyDownEvent event(key);
                data.curr->dispatchEvent(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyUpEvent event(key);
                data.curr->dispatchEvent(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyDownEvent event(key, true);
                data.curr->dispatchEvent(event);
                break;
            }
            }
        });

        glfwSetCharCallback(window_, [](GLFWwindow* window, unsigned int keycode)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(keycode);
            data.curr->dispatchEvent(event);
        });

        glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonDownEvent event(button);
                data.curr->dispatchEvent(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonUpEvent event(button);
                data.curr->dispatchEvent(event);
                break;
            }
            }
        });

        glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xpos, double ypos)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event((float)xpos, (float)ypos);
            data.curr->dispatchEvent(event);
        });

        glfwSetScrollCallback(window_, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.curr->dispatchEvent(event);
        });
    }

    void GlfwWindow::killWindow()
    {
        glfwDestroyWindow(window_);
        windowCount--;

        if (windowCount == 0)
            glfwTerminate();
    }

    void GlfwWindow::onTick()
    {
        glfwPollEvents();

        // when we introduce renderer, this will probably go away
        glfwSwapBuffers(window_);
    }

    void GlfwWindow::setVSync(bool enabled)
    {
        glfwSwapInterval(enabled);
    }

    void GlfwWindow::setWindowType(WindowType windowType)
    {
        type_ = windowType;

        formerData_.xpos = data_.xpos;
        formerData_.ypos = data_.ypos;
        formerData_.width = data_.width;
        formerData_.height = data_.height;

        if (type_ == WindowType::FULLSCREEN)
        {
            if (!monitor_)
            {
                monitor_ = glfwGetPrimaryMonitor();
                DBG_ASSERT(monitor_, "Could not get primary monitor");
            }

            const GLFWvidmode* mode = glfwGetVideoMode(monitor_);
            glfwSetWindowMonitor(window_, monitor_, 0, 0, mode->width, mode->height, mode->refreshRate);
        }

        else
        {
            bool enableDecorated = type_ == WindowType::WINDOWED;
            glfwSetWindowAttrib(window_, GLFW_DECORATED, enableDecorated);
            glfwSetWindowMonitor(window_, nullptr, formerData_.xpos, formerData_.ypos, formerData_.width, formerData_.height, 0);
        }
    }
}