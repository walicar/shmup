#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <glad.h>
#include <glfw3.h>
#include <bitset>
#include <string>


class WindowManager
{

    public:
        WindowManager() = delete;

        WindowManager(std::string const& title, unsigned int width, unsigned int height);

        void clean();

        void update();

        void process_events();


    private:
        GLFWwindow* window;

        std::bitset<8> buttons;
};



#endif