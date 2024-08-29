#include "src/Window.h"

#include <filesystem>
#include <iostream>

Window *window;

int main()
{
    auto path = std::filesystem::current_path();
    // check if path ends with Debug if not append Debug and set as current path
    if (path.string().find("Debug") == std::string::npos)
    {
        path = path / "Debug";
        std::filesystem::current_path(path);
    }

    window = new Window();
    auto initCode = window->init();
    if (initCode != 0)
    {
        std::cerr << "Failed to initialize window" << std::endl;
        return initCode;
    }

    while (!window->should_close())
    {
        window->update();
        window->render();
    }

    delete window;

    return 0;
}