# Engine Design

Overview of all the classes in SHMUP's game engine

## General

- **Shader**: allow engine to initialize and use openGL shaders
- **Texture**: allow engine to generate and bind images for texture use
- **Resource Manager**: create a singleton whose responsibility is to load resources
    - decouple file loading from resource classes: Shader, Texture
- **Sprite Cache**: store the sprites somewhere for later use
- **Window Manager**: setup GLFW, process key events
- **UI Manager**: render text on screen with game related information
- **Game**: responsible for main game loop, initliaze ECS system, and controls game state

## Core ECS

The core ECS system was created using this [tutorial](https://austinmorlan.com/posts/entity_component_system/).
This design pattern separates entities, components, and systems 
into managers which is controlled by a single `Coordinator` object.

- **Entity**: essentially an ID representing a living object in the game
- **Component**: a struct representing attributes an entity can have
- **System**: exposes an `update` function that directly interacts with the entities and their components

All entities must be created through the `Coordinator`; the object exposes methods to 
register components to an entity, which can then be sent to a system. The `ComponentManager` is
responsible for making sure entities with the same components are always packed together in
a contiguous array. The `Coordinator` also has an `EventManager` which is used to 
register callback functions and call those functions whenever it receives an event.

- **Coordinator**: singleton class used to register components, systems, events, and create entities
- **EntityManager**: the Entity Manager is in charge of distributing entity IDs and keeping record of which IDs 
are in use and which are not.
- **ComponentManager**: Component Manager is in charge of handling the component arrays
when a component needs to be added or removed 
- **SystemManager**: adds appropriate entities to each system's list of entities
- **EventManager**: EventManager is in charge of running callbacks when the appropriate event shows up

## Systems

- **AI System**: responsible for generating enemy projectiles and enemy movement
- **Animation System**: used to animate bomb sprite
- **Background System**: spawns star particles in the background
- **Collision System**: detect overlap of a projectile and actor (player or enemy), and deal damage
- **Movement System**: listen to key events to move player
- **Physics System**: applies velocity to projectiles
- **Projectile System**: listen to key events to shoot player projectiles: bullets, laser, bombs
- **Spawn System**: controls logic to spawn enemies
- **Sprite System**: render sprite objects, deactivate entities when they are OOB