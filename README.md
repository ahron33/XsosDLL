# XsosDLL

<p align="center">
  <img src="screenshots/banner.png" width="750" alt="XsosDLL Banner">
</p>

<h3 align="center">Native C++ DLL • Lua Integration • Custom Interface</h3>

<p align="center">
  A Windows-based C++ project focused on Lua runtime integration, scripting, and custom graphical interfaces.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-Native-blue?style=for-the-badge&logo=cplusplus" />
  <img src="https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows" />
  <img src="https://img.shields.io/badge/IDE-Visual%20Studio-purple?style=for-the-badge&logo=visualstudio" />
</p>

---

## Overview

**XsosDLL** is a native Windows DLL project developed in C++, combining Lua scripting capabilities with low-level runtime interaction and a custom graphical interface.

The project explores the integration of two different Lua environments through a dedicated bridging system, alongside script execution, memory utilities, and an ImGui-powered interface.

Built with a focus on native development and runtime interaction, XsosDLL brings together several components into one Visual Studio solution.

## Features

| Feature | Description |
|---|---|
| Lua Integration | Embedded Lua environment and scripting support |
| Lua Bridge | Communication between Lua environments |
| Script Execution | Loading and executing Lua scripts |
| Custom Interface | Graphical interface powered by Dear ImGui |
| Code Editor | Integrated text editor component |
| Memory Utilities | Memory scanning and runtime utilities |
| HTTP Support | URL requests and string downloading |
| Native DLL | Compiled as a Windows Dynamic Link Library |

## Technology Stack

- **C++** — Core programming language
- **Lua** — Embedded scripting environment
- **Dear ImGui** — Graphical user interface
- **DirectX 11** — Rendering backend
- **Win32 API** — Windows integration
- **MSVC** — C++ compiler
- **Visual Studio** — Development environment

## Preview

<p align="center">
  <img src="screenshots/main.png" width="80%" alt="Main Interface">
</p>

<p align="center">
  <i>Main interface</i>
</p>

<p align="center">
  <img src="screenshots/editor.png" width="80%" alt="Code Editor">
</p>

<p align="center">
  <i>Integrated code editor</i>
</p>

## Project Structure

```text
XsosDLL/
│
├── V3rm/
│   ├── Lua/              # Lua source and headers
│   ├── Main.cpp          # Main implementation
│   ├── Bridge.h          # Lua bridge implementation
│   ├── MainImgui.h       # Interface components
│   ├── TextEditor.cpp    # Text editor implementation
│   ├── TextEditor.h      # Text editor header
│   ├── memory.cpp        # Memory utilities
│   ├── memory.h
│   ├── retcheck.h        # Runtime utilities
│   ├── imgui.cpp         # ImGui implementation
│   └── V3rm.vcxproj      # Visual Studio project
│
├── Debug/                # Debug build output
├── Release/              # Release build output
│
├── Axon.sln              # Visual Studio solution
└── README.md
```

## Getting Started

### Requirements

- Windows
- Visual Studio 2022
- Desktop development with C++
- Windows SDK
- MSVC v143 toolset

### Build Instructions

**1. Clone the repository**

```bash
git clone https://github.com/YOUR_USERNAME/XsosDLL.git
```

**2. Open the solution**

Open `Axon.sln` in Visual Studio.

**3. Configure the project**

Select the appropriate build configuration and platform supported by the project.

**4. Build**

Navigate to:

`Build → Build Solution`

The compiled DLL will be generated in the corresponding output directory.

> Note: This is a legacy project with version-dependent runtime addresses and compatibility requirements. A successful compilation does not guarantee runtime compatibility.

## Repository Notes

- The project uses legacy runtime offsets.
- Compatibility depends on the target runtime version and architecture.
- Debug and Release folders contain generated build artifacts.
- Some components may require updates to work with modern environments.

## Disclaimer

This repository is provided for educational and research purposes, demonstrating concepts related to native Windows development, Lua integration, and runtime interaction.

Use the code only in software and environments where you have explicit authorization.

The project is provided as-is, without guarantees of functionality, compatibility, or support.

## License

No license has currently been assigned to this repository.

---

<p align="center">
  Made with C++
</p>

<p align="center">
  <sub>XsosDLL — Exploring Native Development.</sub>
</p>
