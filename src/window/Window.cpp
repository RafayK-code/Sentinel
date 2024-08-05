#include "window/Window.h"
#include "window/glfw/WindowGlfw.h"

namespace sntl
{
    RefPtr<IWindow> IWindow::create(const std::string& title, int xpos, int ypos, int width, int height, WindowType type, bool maximized)
    {
        return makeRef<WindowGlfw>(title, xpos, ypos, width, height, type, maximized);
    }
}