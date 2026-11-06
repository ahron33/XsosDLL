XsosDLL (Axon) ⚡

XsosDLL (Axon) is a high-performance, native C++ dynamic-link library designed for low-level memory manipulation, process interaction, and execution bridging between external user interfaces (such as C# WinForms applications) and target process environments.

🔑 Key Features

Native Performance: Built entirely in modern C++ with direct Win32 API interactions for minimal runtime overhead.

Non-Blocking Execution: Spawns asynchronous worker threads upon attachment (DLL_PROCESS_ATTACH) to preserve target application engine stability.

Inter-Process Communication (IPC): Native support for named pipes and shared memory mapping for smooth data transfer between UI launchers and the core library.

Memory Management & Hooking: Uses low-level virtual memory protections (VirtualProtect) for safe memory page modifications and execution flow hooking.

🛠️ Tech Stack & Requirements

Primary Language: C++ (Native)

API / Libraries: Windows SDK (Windows.h, Psapi.h), C++ Standard Template Library (STL)

Target OS: Windows 10 / 11 (x86 & x64 architectures)

IDE / Toolchain: Microsoft Visual Studio 2019+ (MSVC compiler target)

📁 Repository Structure

XsosDLL/
├── Axon.sln              # Visual Studio Solution Configuration
├── Axon.vcxproj          # C++ Build Configuration
├── Axon.vcxproj.filters  # Source File Organization
├── Axon.cpp / Main.cpp   # Core Logic & DllMain Entry Point
└── Axon.h                # Header Definitions & Directives


🚀 Building & Setup

Prerequisites

Ensure you have the following installed:

Visual Studio 2019 or later

Desktop development with C++ workload enabled

Latest Windows 10/11 SDK
