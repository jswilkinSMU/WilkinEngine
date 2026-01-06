# WilkinEngine
Personal DxD11 game engine created at SMU Guildhall

### About this engine:
  - Contains folder structures for AI, Animation, Audio, Core, Input, Math, Networking, Rendering, Skeleton, UI, and Window
  - Substantial math utils library and vertex utils library
  - Rendering with Directx11
  - Supports 2D and 3D rendering
  - Custom made engine subsystems:
    - Input System
    - Audio System
    - Networking System
    - Job System
    - UI System
    - Window
    - Renderer
    - EventSystem and DevConsole
  - Setup so that engine subsystems are optional.
  - Uses ThirdParty stbImage, FMOD, tinyXML, and SquirrelNoise
---
### AI
    - Folder contains files for BehaviorTree and BehaviorNode
    - Simple updateable behavior tree given to entities in game
    - Base class BehaviorNode with derived Sequence and Selector Nodes.
    - Leaf Nodes created through game code.
---
### Animation
    - Folder contains files for Animation and AnimStateMachine
    - Animation class used to store procedurally created animations from game code.
      - Requires a 3D skeleton specifically, 2D sprite animations are done through Renderer/SpriteAnimDefinition
    - Animation State Machine given to entities with a skeleton in game.
      - Transitions between different Animation States, updating in the background.
      - Also updates animation blending through the state machine.
      - Transitions and states created with strings through game code.
---
### Audio
    - Folder contains files for AudioSystem
    - Utilizes the FMOD library
    - Has functions for getting sound from file path, playing sound at 2D or 3D positions, and setting number of listeners.
---
### DevConsole
    - DevConsole works hand in hand with the EventSystem to Execute lines.
    - Full screen render with blinking insertion point and left/right arrow navigation.
    - Contains helpful devconsole specific static consts.
    - Events:
      - Event Echo Command handles echoing/printing back what was typed.
      - Event Key Pressed handles key input, typing, and insertion point.
      - Event Char Input handles char input by appending valid characters to current input line.
      - Event Command Clear clears all lines of text printed currently in devconsole.
      - Event Command Help prints out all currently registered commands from EventSystem.
    - DevConsole holds command history
      - Up and Down arrows navigate command hisotry.
    - Deletes lines automatically if they go off screen.
---
