<div align="center">
  <h1>Yubi - Window Tiling Manager for Windows 10+</h1>
  <!-- <img src="assets/yubi_logo.png" alt="Yubi Logo" width="200"> -->
</div>

<div align="center">
    <img src="https://img.shields.io/badge/license-GPL%20v3-blue.svg?style=for-the-badge&logo=none&labelColor=1f1f1f&color=0366d6" />
    <img src="https://img.shields.io/travis/Mikyan0207/yubi.svg?style=for-the-badge&logo=none&labelColor=1f1f1f&color=green" />
    <img src="https://img.shields.io/github/release/Mikyan0207/yubi.svg?style=for-the-badge&logo=none&labelColor=1f1f1f&color=green" />
    <img src="https://img.shields.io/github/downloads/Mikyan0207/yubi/total.svg?style=for-the-badge&logo=none&labelColor=1f1f1f&color=blueviolet" />
    <img src="https://img.shields.io/github/issues/Mikyan0207/yubi.svg?style=for-the-badge&logo=none&labelColor=1f1f1f&color=red" />
</div>

<br>

Yubi is a window tiling manager designed for Windows 10 and above.<br>
It provides a convenient way to organize and manage windows on your desktop by allowing you to easily arrange them in various layouts such as horizontal, vertical, and Binary Space Partitioning (BSP).

<br>

## Features

- 🪟 Window Tiling: Effortlessly organize and resize windows for efficient multitasking.
- 📐 Multiple Layouts: Choose from horizontal, vertical, and BSP (Binary Space Partitioning) layouts.
- 🎨 Customizable Themes: Personalize the appearance of Yubi to suit your preferences.
- 🖱️ Hotkeys: Assign hotkeys to quickly perform window management actions.
- 🌐 Status Bar: Display customizable status bars for enhanced information and productivity.
- 🎛️ Fine-grained Control: Adjust window size and position with pixel-level precision.
- 🔍 Window Searching: Quickly locate and bring specific windows to the forefront.

<br>

## Installation

To begin using Yubi, you'll need to install it on your system. Follow the steps below to get started:
<br>

### Manual Installation

To manually install Yubi, please follow these steps:

1. Download the latest release from the [Releases](https://github.com/Mikyan0207/yubi/releases) page.
2. Extract the downloaded package to your preferred directory.
3. Run the `yubi.exe` file to start Yubi.

<br>

### Installation via Scoop Package Manager

[Yubi](https://scoop.sh/) can also be installed using the Scoop package manager. If you haven't installed Scoop yet, you can do so by following the instructions on the [Scoop website](https://scoop.sh/).

Once Scoop is set up, open a command prompt and run the following command to install Yubi:

```shell
scoop bucket add extras
scoop install yubi
```

This will automatically download and install Yubi on your system.

<br>

## Build

To build the project from source, run the following command:

```shell
dotnet publish ./Yubi.Interface/Yubi.Interface.csproj --configuration=Release --runtime=win-x64 --output=. --self-contained -p:PublishSingleFile=true -p:IncludeAllContentForSelfExtract=true
```

<br>

## Architecture

Yubi is built using C# and .NET 7 framework. The project is divided into several components:

- **Yubi.Core**: Contains shared classes and utilities used across other parts of the application.
- **Yubi.Interface**: Handles the logic for displaying the status bar.
- **Yubi.Infrastructure**: Contains the execution logic and operations.
- **Yubi.Application**: Defines interfaces and handles the interaction between the interface and infrastructure layers.

<br>

## Contributing

We welcome contributions from the community. To contribute to Yubi, please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Make the necessary changes and commit them.
4. Push your changes to your forked repository.
5. Submit a pull request detailing your changes.

Please ensure that you adhere to the coding conventions and maintain a clean commit history.

## License

Yubi is released under the GPL v3 License. See [LICENSE](LICENSE) for more information.

---

Thank you for your interest in Yubi! If you have any questions or suggestions, please feel free to open an issue on the [Issues](https://github.com/Mikyan0207/yubi/issues) page.