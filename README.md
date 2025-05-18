# Comprehensive Project Specification for Implementation: Cub3D

## Project Overview

Cub3D is a first-person 3D maze game developed in C, inspired by the iconic game Wolfenstein 3D. The primary objective is to explore raycasting technology and implement it to create a dynamic 3D representation of a maze. The core value of this project lies in implementing raycasting, a classic 3D graphics technique, from scratch in C within a modern environment, allowing for learning game development fundamentals, algorithms, and efficient coding practices.

## Functional Requirements

### Game Configuration File Parsing

- Read game information from configuration files with .cub extension
- Configuration files must include:
    - Wall texture paths for each cardinal direction (NO, SO, WE, EA)
    - Floor (F) and ceiling (C) colors specified as RGB values (0-255)
    - Map data
- Elements before map data start with identifiers (NO, SO, WE, EA, F, C) followed by their information
- Elements may be separated by one or more empty lines
- Information in non-map elements may be separated by one or more spaces
- Map data must be placed at the end of the file

### 3D Visualization

- Implement raycasting to display a 3D view of the maze
- Walls must be rendered using the specified textures for each cardinal direction
- Floor and ceiling must be filled with the specified colors

### Player Controls

- Movement: Use WASD keys to navigate within the maze
- View rotation: Use left/right arrow keys to change view direction

### Program Termination

- Clean exit when the ESC key is pressed
- Clean exit when clicking the red X button on the window frame

### Map Structure

- Maps consist of characters: '0' (space), '1' (wall), and 'N'/'S'/'E'/'W' (player starting position and orientation)
- Maps must be completely surrounded by walls ('1')
- Spaces within the map may be separated by spaces
- Only one player starting position must exist within the map

### Error Handling

- When configuration file or map has errors, program must exit properly and output "Error\n" followed by a specific error message
- Invalid map conditions include: not surrounded by walls, invalid characters, multiple player starting positions, or spaces/tabs around spaces ('0') or player starting positions (considered as not enclosed)

## Technical Specifications

- Programming language: C
- Frameworks/Libraries:
    - MiniLibX: Required for rendering
    - math.h: Mathematical functions are allowed
    - Libft: Usage is permitted
- Allowed standard functions: open, close, read, write, printf, malloc, free, perror, strerror, exit, gettimeofday
- Architecture: Raycasting-based architecture that projects rays from the player's viewpoint into the scene and calculates intersections with objects (walls) to generate the image

## Implementation Details

### Core Data Structures

- Structure for player state (position, direction vector, camera plane vector)
- 2D char array for storing map data
- Structure for ray calculation information (direction, starting position, sideDist, deltaDist, hit flag, etc.)
- Structure for wall textures and floor/ceiling color information

### Raycasting Algorithm Implementation

1. **Ray Calculation**: Calculate the direction vector for each ray corresponding to each X-coordinate (pixel column) on the screen using the player's direction vector and camera plane vector
2. **DDA Algorithm**:
    - Initialize ray starting position (mapX, mapY), distance to the first grid boundary (sideDistX, sideDistY), and distance needed to move one square in X/Y direction (deltaDistX, deltaDistY)
    - deltaDistX and deltaDistY are proportional to the absolute value of the reciprocal of the ray's direction
    - Set a very large value when the ray's direction is 0
    - Loop until hitting a wall
    - In each loop, choose the smaller of sideDistX and sideDistY, add the corresponding deltaDist, and advance one step in the map coordinate (mapX or mapY)
    - Record the direction (side) that was advanced (0 for X-axis, 1 for Y-axis)
    - Check if the new map coordinate is a wall ('1') and exit the loop if true
3. **Wall Distance Calculation**: After the DDA loop ends, calculate the perpendicular distance (perpWallDist) from the player to the wall using the grid position and direction (side) where the ray hit the wall
    - Use perpendicular distance rather than Euclidean distance to prevent fisheye effect
    - Calculate perpendicular distance by subtracting one deltaDist from the corresponding sideDist
4. **Wall Height Calculation**: Calculate the height of the wall to be drawn on the screen using the perpendicular distance
    - Height is inversely proportional to the distance to the wall
5. **Wall Texture Determination**: Determine which texture to use based on which face of the wall the ray hit (north, south, east, or west)
    - Use side information and ray/player direction vectors to make this determination
6. **Wall Rendering**: Draw the wall on the screen at the corresponding X-coordinate column using the calculated height and determined texture
    - Calculate which position of the texture to use based on where the ray hit the wall
7. **Floor and Ceiling Rendering**: Fill the top half of the screen with the ceiling color and the bottom half with the floor color
8. **Player Movement Implementation**: Calculate candidate coordinates based on the player's current position and direction vector according to WASD key inputs
    - Check if the candidate coordinates (considering player width) are walls on the map
    - Update player position if they are not walls
9. **Player View Rotation**: Update the player's direction vector using a rotation matrix based on left/right arrow key inputs
    - Calculate X and Y components of the direction vector using trigonometric functions

### Optional Bonus Features

- Minimap display
- Mouse-based view rotation
- Door opening/closing mechanics
- Sprite rendering (barrels, objects)
- Wall collision detection

## Development Guidelines

- Code Quality: Follow appropriate naming conventions and coding standards
- Documentation: Include appropriate comments in the code
- Version Control: Use Git for team development and version management with clear commit messages

## Implementation Challenges

- Error handling implementation can be time-consuming
- Understanding and implementing mathematical calculations (especially for sprites) may be challenging
- In team development, it's important to decide on coding rules beforehand
- Implementing from scratch without a game engine deepens understanding of game engine mechanisms and value

## Implementation Sequence

1. Set up the basic project structure with MiniLibX integration
2. Implement configuration file parsing
3. Create map data structures and validation
4. Implement the basic raycasting algorithm for wall rendering
5. Add player movement and collision detection
6. Implement texture mapping for walls
7. Add floor and ceiling rendering
8. Implement user input handling for movement and program exit
9. Add error handling throughout the application
10. Implement bonus features if desired
