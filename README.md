# Xsos V2

**A lightweight C# Windows Forms application for Lua script editing and execution.**

Xsos V2 provides a simple interface for working with Lua scripts in supported Roblox-related environments. It combines a syntax-highlighted code editor with script execution, process interaction, and named-pipe communication in a compact desktop application.

<img width="321" height="315" alt="Screenshot_212" src="https://github.com/user-attachments/assets/82ac13db-e980-4a15-82a0-6a7a67e2c365" />

## ✨ Features

- **Lua Code Editor** — Syntax highlighting, code folding, and keyword support powered by ScintillaNET.
- **Script Execution** — Send Lua scripts through named-pipe communication.
- **Process Injection** — Includes Windows API functionality for loading a DLL into a target process.
- **File Support** — Open `.lua` and `.txt` script files, including drag-and-drop support.
- **Update System** — Checks for newer versions and supports downloading application updates.
- **Options Interface** — Provides configurable application options.
- **Dark UI** — A minimal Windows Forms interface designed for straightforward use.

## 🛠️ Built With

| Technology | Purpose |
|---|---|
| C# | Core application |
| Windows Forms | Desktop user interface |
| ScintillaNET | Lua code editor |
| Win32 API | Windows process interaction |
| Named Pipes | Inter-process communication |
| .NET | Application framework |

## 📁 Project Structure

```text
Xsos-V2/
├── Injector.cs
├── MainForm.cs
├── MainForm.Designer.cs
├── MainForm.resx
├── NamedPipes.cs
└── OptionsForm.cs
```

## 🚀 Getting Started

1. Clone or download this repository.
2. Open the project in Visual Studio.
3. Restore the required NuGet dependencies, including ScintillaNET.
4. Build the project.
5. Run the application in an appropriate test environment.

Additional project configuration or dependencies may be required depending on the original solution setup.

## ⚙️ How It Works

Xsos V2 uses several Windows technologies to connect its editor interface with a target process:

1. Scripts are written or loaded into the ScintillaNET editor.
2. The application uses its process interaction functionality to load the required DLL.
3. Named pipes provide a communication channel for sending script text.
4. The application includes an update mechanism to retrieve newer releases.
