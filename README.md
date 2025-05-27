# Mosaic Engine

## Overview

**Mosaic** is a C++ framework focused on making 2D and 3D game development harder to mess up. The framework aims to support most major platforms, including Windows, macOS, and most Linux distributions. Whether you're building a quick game jam prototype or a full-fledged indie title, Mosaic provides the tools you need — without the bloat.

At its core, Mosaic embraces a strict and explicit API design, inspired by modern graphics APIs like Vulkan. This approach prioritises clarity, predictability, and control, empowering developers to understand exactly what happens under the hood. Instead of hidden magic or implicit behaviour, Mosaic demands deliberate actions and explicit configuration, reducing bugs and unexpected side effects. This philosophy makes Mosaic especially suited for developers who want full control over their engine's inner workings, while still benefiting from robust foundational systems.

## Features

- **Input Handling**: Unified keyboard and mouse inputs
- **Event Management**: Flexible event and communication system
- **Component Design**: Relies on Components for functionality
- **Multiple Rendering APIs**: Support for both Vulkan and OpenGL

## Requirements

- Any C++23 compatible compiler
- [CMake](https://cmake.org/)
- A system with OpenGL or Vulkan support

## Roadmap

- **Rendering System**: Currently working to integrate a fully featured rendering system
- **Audio Playback**: Plans to integrate [OpenAL](https://www.openal.org/) for in-depth audio support
- **More Rendering Backends**: Potential future support for rendering backends like Metal or DirectX

## Third-Party Libraries

Mosaic Engine makes use of the following open-source libraries:

- **GLFW** — Cross-platform windowing and input
- **GLM** — Multipurpose mathematics library
- **GLEW** — OpenGL extension loader

Thanks to the authors and contributors of these projects!

## Licensing

Mosaic is released under the MIT licence. If you use it in a published game or project, a credit would be greatly appreciated (but it’s not required).