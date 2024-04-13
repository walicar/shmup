#include <glad.h>
#include <glfw3.h>

const int HEIGHT = 800;
const int WIDTH = 400;

class Game
{
    public:
        Game();
        ~Game();
        void init();
        void process();
        void update();
        void render();
};