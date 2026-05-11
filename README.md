# LearningOpenGL

Learning OpenGL with C++ as part of my Game Engine Foundations studies.

---

# About

This repository contains my exercises, experiments, and projects while learning modern OpenGL and real-time rendering concepts using C++.

The goal is to build a strong foundation for graphics programming and future game engine development.

---

# Topics Covered

- OpenGL basics
- Graphics pipeline
- Rendering primitives
- Vertex data
- VAO / VBO
- Shaders
- Transformations
- Camera systems
- Lighting
- Textures

---

# Technologies

- C++
- OpenGL
- GLFW
- GLEW
- Visual Studio

---

# Progress

## Week 1

- Setup GLFW and GLEW
- Create OpenGL window
- Implement render loop
- Clear screen and swap buffers
- Render:
  - Triangle
  - Square
  - Circle
- Learn legacy OpenGL rendering

---

## Week 2

- Setup VAO and VBO
- Add vertex data for rendering
- Learn modern OpenGL graphics pipeline
- Create shader program
- Render first triangle using Modern OpenGL

---

# Project Structure

```text
Src/
|-- Dependencies/
|   |-- GLFW/
|   |   |-- Include/
|   |   `-- Lib/
|   |
|   `-- GLEW/
|       |-- Include/
|       `-- Lib/
|
`-- main.cpp
```

# Download Dependencies

## GLFW

Official website:

https://www.glfw.org/

Download the precompiled binaries for Windows.

---

## GLEW

Official website:

https://glew.sourceforge.net/

Download the Windows binaries.

---

# Visual Studio Configuration

Open your project in Visual Studio.

---

# 1. Add Include Directories (.h files)

These folders contain the header files used by:

```cpp
#include <glew.h>
#include <glfw3.h>
```

## Steps

Go to:

```text
Project -> Properties
```

Then:

```text
C/C++ -> General -> Additional Include Directories
```

Add:

```text
$(ProjectDir)Dependencies\GLFW\Include
$(ProjectDir)Dependencies\GLEW\Include
```

---

# 2. Add Library Directories (.lib files)

These folders contain the `.lib` files used during linking.

## Steps

Go to:

```text
Linker -> General -> Additional Library Directories
```

Add:

```text
$(ProjectDir)Dependencies\GLFW\Lib
$(ProjectDir)Dependencies\GLEW\Lib
```

---

# 3. Add Required Libraries

Now tell Visual Studio which libraries to link.

## Steps

Go to:

```text
Linker -> Input -> Additional Dependencies
```

Add:

```text
glfw3.lib
glew32s.lib
opengl32.lib
```

---

# 4. Add GLEW_STATIC Definition

If using the static version of GLEW, add:

```text
GLEW_STATIC
```

## Steps

Go to:

```text
C/C++ -> Preprocessor -> Preprocessor Definitions
```

Add:

```text
GLEW_STATIC
```

---

# 5. Include Headers in Your Code

```cpp
#include <glew.h>
#include <glfw3.h>
```

Important:

`glew.h` must be included before `glfw3.h`

---

# 6. Build the Project

Build:

```text
CTRL + SHIFT + B
```

Run:

```text
CTRL + F5
```

If everything is configured correctly, an OpenGL window should appear.

---

# Author

Leo

