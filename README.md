Markdown
# XsosDLL Technical Documentation & Architecture Overview

## Overview

**XsosDLL** (Axon) is a native C++ dynamic-link library designed as a low-level interface for Lua script execution and game process memory manipulation. The project provides a bridge between an external user interface (such as a C# WinForms launcher) and the target game process by handling DLL injection, memory allocation, and Lua state interaction.

---

## Technical Specifications

* **Language:** C++ (Native) / C# (Wrapper UI)
* **Target Architecture:** x86 / x64 (Win32 API)
* **IDE / Toolchain:** Microsoft Visual Studio (MSVC)
* **Dependencies:**
  * Windows SDK (`Windows.h`, `Psapi.h`)
  * Target Lua C API / Internal Game Function Offsets
  * Standard Template Library (STL)

---

## File & Repository Structure

```text
XsosDLL/
├── Axon.sln              # Visual Studio Solution Configuration
├── Axon.vcxproj          # C++ Project Build Configuration
├── Axon.vcxproj.filters  # Visual Studio Filters for Source Organization
├── Axon.cpp / Main.cpp   # DLL Entry Point (DllMain) & Core Logic
├── Axon.h                # Header Exports, Type Definitions, Function Offsets
└── Debug / Release/      # Build Output Directories (.dll, .lib, .pdb)
