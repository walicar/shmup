# Design
## Classes
- **Shader**: allow engine to initialize and use openGL shaders
- **Game**: initialize entities, and control game state
    - implement expected methods for game loop: `process()` -> `update()` -> `render()`
- **Resource Manager**: create a singleton whose responsibility is to load resources
    - decouple file loading from resource classes: Shader