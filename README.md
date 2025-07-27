# WordCounter

A lightweight, real-time word and character counter application built with Dear ImGui. This application provides instant feedback on text length and validation for social media platforms like Twitter/X and Snapchat.

![WordCounter Demo](https://img.shields.io/badge/Platform-Windows-blue) ![Language](https://img.shields.io/badge/Language-C%2B%2B20-orange) ![License](https://img.shields.io/badge/License-Copyright%202023%20Avnish%20Kirnalli-green)

## Features

- **Real-time Counting**: Instantly displays word and character count of copied text
- **Social Media Validation**: 
  - Twitter/X character limit validation (280 characters)
  - Snapchat character limit validation (150 characters)
- **Auto Mode**: Automatically captures clipboard content at regular intervals
- **Borderless Window**: Clean, modern borderless interface
- **Compact Design**: Minimal footprint with expandable view
- **Cross-platform Ready**: Built with platform abstraction layer

## Built With

This project is built using the [ImGuiBorderlessWindow](https://github.com/AvnishGameDev/ImGuiBorderlessWindow) framework, which provides:

- **Borderless Window System**: Modern, frameless window design
- **Dear ImGui Integration**: Immediate mode GUI with custom styling
- **Platform Abstraction**: Support for Windows, macOS, and Linux
- **Custom Themes**: Pre-built themes including the "Comfy" theme used in this app
- **Font Management**: Easy font loading and Unicode support
- **Window Management**: Resizing, positioning, and behavior controls

## How It Works

1. **Copy Detection**: The application monitors clipboard changes when text is copied (Ctrl+C)
2. **Word Counting**: Uses string stream parsing to accurately count words
3. **Character Counting**: Provides precise character count including spaces and punctuation
4. **Platform Validation**: Shows real-time validation status for different social media platforms
5. **Auto Mode**: Optional automatic clipboard monitoring with configurable intervals

## Getting Started

### Prerequisites

- Windows 10/11
- Visual Studio 2019 or later with C++20 support
- Premake5 (for project generation)

### Building

1. Clone the repository:
   ```bash
   git clone [your-repo-url]
   cd WordCounter
   ```

2. Generate project files:
   ```bash
   premake5 vs2022
   ```

3. Open the solution:
   ```bash
   cd solution
   start WordCounter.sln
   ```

4. Build the project in Visual Studio (Debug or Release configuration)

### Running

After building, the executable will be located in:
- `bin/Debug/WordCounter.exe` (Debug build)
- `bin/Release/WordCounter.exe` (Release build)

The application includes all necessary assets and fonts in the Assets folder.

## Usage

1. **Basic Usage**: 
   - Launch the application
   - Copy any text (Ctrl+C)
   - The word and character count will update instantly

2. **Extended View**:
   - Click the arrow button (↓) to expand the window
   - View platform-specific validation status
   - Enable "Auto" mode for continuous monitoring

3. **Auto Mode**:
   - Toggle the "Auto?" checkbox in extended view
   - The app will automatically capture clipboard content every second
   - Useful for real-time monitoring while typing

## Project Structure

```
WordCounter/
├── WordCounter/              # Main application source
│   ├── MainApp.cpp          # Core application logic
│   ├── MainApp.h            # Application header
│   ├── Assets/              # Font files
│   └── Gui/                 # ImGuiBorderlessWindow framework
│       ├── App/             # Application base class
│       ├── Platform/        # Platform-specific implementations
│       └── ThirdParty/      # Dear ImGui library
├── bin/                     # Built executables and assets
├── solution/                # Generated Visual Studio project files
└── premake5.lua            # Build configuration
```

## Technical Details

- **Language**: C++20
- **GUI Framework**: Dear ImGui via ImGuiBorderlessWindow
- **Platform**: Windows (with cross-platform foundation)
- **Build System**: Premake5 → Visual Studio
- **Architecture**: x64

## Acknowledgments

- Built using [ImGuiBorderlessWindow](https://github.com/AvnishGameDev/ImGuiBorderlessWindow) framework
- [Dear ImGui](https://github.com/ocornut/imgui) for the immediate mode GUI
- Font assets included for consistent cross-system appearance

## License

Copyright (C) 2023 Avnish Kirnalli. All rights reserved.

---

*This application demonstrates the power and flexibility of the ImGuiBorderlessWindow framework for creating modern, lightweight desktop applications.*
