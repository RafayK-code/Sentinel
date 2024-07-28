#include "window/glfw/WindowGlfw.h"

#include "core/Dbg.h"
#include "events/WindowEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

namespace sntl
{
    static uint8_t windowCount = 0;

    WindowGlfw::WindowGlfw(const std::string& title, int xpos, int ypos, int width, int height, WindowType windowType, bool maximized)
        : data_({xpos, ypos, width, height}), title_(title), type_(windowType), monitor_(nullptr), maximized_(maximized)
    {
        initWindow();
    }

    WindowGlfw::~WindowGlfw()
    {
        killWindow();
    }

    void WindowGlfw::initWindow()
    {
        if (windowCount == 0)
            DBG_ASSERT(glfwInit(), "Could not initialize glfw");

        monitor_ = glfwGetPrimaryMonitor();
        DBG_ASSERT(monitor_, "Could not get primary monitor");

        if (type_ == WindowType::FULLSCREEN || type_ == WindowType::BORDERLESS)
        {
            const GLFWvidmode* mode = glfwGetVideoMode(monitor_);

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

            if (type_ == WindowType::BORDERLESS)
            {
                glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
                data_.height++;     //stupid hack
            }
        }

        else 
        {
            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            glfwWindowHint(GLFW_MAXIMIZED, maximized_);
        }

        window_ = glfwCreateWindow(data_.width, data_.height, title_.c_str(), type_ == WindowType::FULLSCREEN ? monitor_ : nullptr, nullptr);
        DBG_ASSERT(window_, "Could not create GLFW window");

        windowCount++;
        open_ = true;

        glfwMakeContextCurrent(window_);

        glfwSetWindowUserPointer(window_, this);
        setVSync(true);

        // setup callbacks

        glfwSetWindowCloseCallback(window_, [](GLFWwindow* window)
        {
            WindowGlfw* curr = static_cast<WindowGlfw*>(glfwGetWindowUserPointer(window));
            curr->open_ = false;

            WindowCloseEvent event;
            curr->dispatchEvent(event);
        });

        glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height)
        {
            WindowGlfw* curr = static_cast<WindowGlfw*>(glfwGetWindowUserPointer(window));
            curr->data_.width = width;
            curr->data_.height = height;

            WindowResizeEvent event(width, height);
            curr->dispatchEvent(event);
        });

        glfwSetWindowFocusCallback(window_, [](GLFWwindow* window, int focused)
        {
            WindowGlfw* curr = static_cast<WindowGlfw*>(glfwGetWindowUserPointer(window));

            if (focused)
            {
                WindowFocusEvent event;
                curr->dispatchEvent(event);
            }

            else
            {
                WindowLostFocusEvent event;
                curr->dispatchEvent(event);
            }
        });

        glfwSetWindowPosCallback(window_, [](GLFWwindow* window, int xpos, int ypos)
        {
            WindowGlfw* curr = static_cast<WindowGlfw*>(glfwGetWindowUserPointer(window));
            curr->data_.xpos = xpos;
            curr->data_.ypos = ypos;

            WindowMovedEvent event(xpos, ypos);
            curr->dispatchEvent(event);
        });

        glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowGlfw* curr = static_cast<WindowGlfw*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyDownEvent event(key);
                curr->dispatchEvent(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyUpEvent event(key);
                curr->dispatchEvent(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyDownEvent event(key, true);
                curr->dispatchEvent(event);
                break;
            }
            }
        });

        glfwSetCharCallback(window_, [](GLFWwindow* window, unsigned int keycode)
        {
            WindowGlfw* curr = static_cast<WindowGlfw*>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(keycode);
            curr->dispatchEvent(event);
        });

        glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowGlfw* curr = static_cast<WindowGlfw*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonDownEvent event(button);
                curr->dispatchEvent(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonUpEvent event(button);
                curr->dispatchEvent(event);
                break;
            }
            }
        });

        glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xpos, double ypos)
        {
            WindowGlfw* curr = static_cast<WindowGlfw*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event((float)xpos, (float)ypos);
            curr->dispatchEvent(event);
        });

        glfwSetScrollCallback(window_, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowGlfw* curr = static_cast<WindowGlfw*>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            curr->dispatchEvent(event);
        });

        glfwSetWindowMaximizeCallback(window_, [](GLFWwindow* window, int maximized) 
        {
            WindowGlfw* curr = static_cast<WindowGlfw*>(glfwGetWindowUserPointer(window));
            curr->maximized_ = maximized;
        });
    }

    void WindowGlfw::killWindow()
    {
        glfwDestroyWindow(window_);
        windowCount--;

        if (windowCount == 0)
            glfwTerminate();
    }

    void WindowGlfw::onTick()
    {
        glfwPollEvents();

        // when we introduce renderer, this will probably go away (at least it wont be handled here)
        glfwSwapBuffers(window_);
    }

    void WindowGlfw::setVSync(bool enabled)
    {
        glfwSwapInterval(enabled);
    }

    void WindowGlfw::setWindowType(WindowType windowType)
    {
        type_ = windowType;

        formerData_.xpos = data_.xpos;
        formerData_.ypos = data_.ypos;
        formerData_.width = data_.width;
        formerData_.height = data_.height;

        if (!monitor_)
        {
            monitor_ = glfwGetPrimaryMonitor();
            DBG_ASSERT(monitor_, "Could not get primary monitor");
        }

        const GLFWvidmode* mode = glfwGetVideoMode(monitor_);

        switch (type_)
        {
        case WindowType::FULLSCREEN:
            glfwSetWindowMonitor(window_, monitor_, 0, 0, mode->width, mode->height, mode->refreshRate);
            break;
        case WindowType::BORDERLESS:
            glfwSetWindowAttrib(window_, GLFW_DECORATED, GLFW_FALSE);
            glfwSetWindowAttrib(window_, GLFW_MAXIMIZED, GLFW_FALSE);
            glfwSetWindowMonitor(window_, nullptr, 0, 0, mode->width, mode->height + 1, 0);
            break;
        case WindowType::WINDOWED:
            glfwSetWindowAttrib(window_, GLFW_DECORATED, GLFW_TRUE);
            glfwSetWindowAttrib(window_, GLFW_MAXIMIZED, maximized_);
            glfwSetWindowMonitor(window_, nullptr, formerData_.xpos, formerData_.ypos, formerData_.width, formerData_.height, 0);
        }
    }
}