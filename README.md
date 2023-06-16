<div align="center">
  <h1>Yubi</h1>
  <!-- <img src="assets/yubi_logo.png" alt="Yubi Logo" width="200"> -->
</div>

<div align="center">
    <a href="https://dotnet.microsoft.com/">
        <img src="https://img.shields.io/badge/.NET-7-512BD4?style=for-the-badge&logo=.net&labelColor=1f1f1f" alt=".NET">
    </a>
    <img src="https://img.shields.io/badge/license-GPL%20v3-blue.svg?style=for-the-badge&logo=none&labelColor=1f1f1f&color=0366d6" 
    <img src="https://img.shields.io/travis/Mikyan0207/yubi.svg?style=for-the-badge&logo=none&labelColor=1f1f1f&color=green" />
    <img src="https://img.shields.io/github/release/Mikyan0207/yubi.svg?style=for-the-badge&logo=none&labelColor=1f1f1f&color=green" />
    <img src="https://img.shields.io/github/downloads/Mikyan0207/yubi/total.svg?style=for-the-badge&logo=none&labelColor=1f1f1f&color=blueviolet" />
    <img src="https://img.shields.io/github/issues/Mikyan0207/yubi.svg?style=for-the-badge&logo=none&labelColor=1f1f1f&color=red" />
</div>

<br>

Yubi is a window tiling manager designed for Windows 10 and above.<br>
It provides a convenient way to organize and manage windows on your desktop by allowing you to easily arrange them in various layouts such as horizontal, vertical, and Binary Space Partitioning (BSP).

<br>

Yubi is inspired by [yabai](https://github.com/koekeishiya/yabai), a window management utility for macOS.<br>
The main goal of Yubi is to be a simple, lightweight, and easy-to-use window tiling manager for Windows.<br>

> If you want other features such as a status bar, you can check out [Yubi.StatusBar](https://github.com/Mikyan0207/yubi.statusbar), a status bar application for Yubi.<br>
> :construction: Yubi.StatusBar is currently under development.

<br>

## Table of Contents

- [Features](#features)
- [Installation](#installation)
  - [Manual Installation](#manual-installation)
  - [Installation via Scoop Package Manager](#installation-via-scoop-package-manager)
- [Usage](#usage)
- [Build](#build)
- [Architecture](#architecture)
- [Contributing](#contributing)
- [License](#license)

<br>


## Features

- 🪟 Window Tiling: Effortlessly organize and resize windows for efficient multitasking.
- :straight_ruler: Multiple Layouts: Choose from horizontal, vertical, and BSP (Binary Space Partitioning) layouts.
- :keyboard: Hotkeys: Assign hotkeys to quickly perform window management actions.
- :control_knobs: Fine-grained Control: Adjust window size and position with pixel-level precision.

<br>

## Installation

To begin using Yubi, you'll need to install it on your system. Follow the steps below to get started:
<br>

### Manual Installation

To manually install Yubi, please follow these steps:

1. Download the latest release from the [Releases](https://github.com/Mikyan0207/yubi/releases) page.
2. Extract the downloaded package to your preferred directory.
3. Run the `Yubi.exe` file to start Yubi.

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

## Usage

Yubi can be used in two ways: by running the `Yubi.exe` file or by using the `yubi` command in the command prompt if you installed Yubi using Scoop.


### Starting Yubi

After installing Yubi, you can start using it by running the `Yubi.exe` file or by using the `yubi` command in the command prompt if you installed Yubi using Scoop.
<br><br>

If you didn't install Yubi using Scoop, you can add the `yubi` command to your system path by following these steps:

1. Open the Start menu and search for "Edit the system environment variables".
2. Click on the "Environment Variables" button.
3. Under "System variables", select the "Path" variable and click on the "Edit" button.
4. Click on the "New" button and enter the path to the Yubi executable.
5. Click on the "OK" button to save the changes.
6. Open a new command prompt and run the `yubi` command to start Yubi.

<br>

### Running Yubi in the background

By default, Yubi will run in the foreground. If you want to run Yubi in the background, you can use the `-b` or `--background` flag when starting Yubi:

```shell
yubi -b
```

This will run Yubi as a background process, which means that it will continue running even if you close the command prompt window.
Yubi will still be visible in the system tray, so you can easily access it by right clicking on the Yubi icon.

<br>

### Powershell

If you want to use Yubi in Powershell, you can add the following code to your Powershell Profile:

```powershell
$Env:YUBI_CONFIG_PATH=<path-to-config-file> # Optional, default: $HOME\.config\Yubi\config.yml

function RunYubi
{
    if (!(Get-Process 'Yubi' -ErrorAction SilentlyContinue))
    {
        Start-Process 'Yubi' -WindowStyle hidden -ArgumentList "-b"
    }
}

function StopYubi
{
    if (Get-Process 'Yubi' -ErrorAction SilentlyContinue)
    {
        Stop-Process -Name 'Yubi'
    }
}

Set-Alias -Name 'yubi start' -Value RunYubi -Option AllScope -Force
Set-Alias -Name 'yubi stop' -Value StopYubi -Option AllScope -Force
```

<br>

## Configuration

Yubi supports configuration via a YAML file. The configuration file is located at `$HOME\.config\Yubi\config.yml` by default.
You can modify the configuration file to change the default settings for Yubi.<br>
You can also specify a custom configuration file using the `-c` or `--config` flag when starting Yubi:

```shell
yubi -c <path-to-config-file>
```

Alternatively, you can set the `YUBI_CONFIG_PATH` environment variable to the path of your configuration file:

```powershell
$Env:YUBI_CONFIG_PATH=<path-to-config-file>
```

<br>

## Build

By default, the project is configured with the following build settings:
```xml
<PublishTrimmed>true</PublishTrimmed>
<PublishSingleFile>true</PublishSingleFile>
<PublishReadyToRun>true</PublishReadyToRun>
<SelfContained>true</SelfContained>
<IncludeAllContentForSelfExtract>true</IncludeAllContentForSelfExtract>
```

<br>

To build the project from source, run the following command:

```shell
dotnet publish ./Yubi.Startup/Yubi.Startup.csproj --configuration=Release --runtime=win-x64 --output=./build
```

<br>

## Architecture

Yubi is built using C# and .NET 7 framework. The project is divided into several components:

- **Yubi.Core**: Contains shared classes and utilities used across other parts of the application.
- **Yubi.Infrastructure**: Contains the execution logic and operations.
- **Yubi.Application**: Defines interfaces and handles the interaction between the interface and infrastructure layers.


### Dependencies

Yubi uses the following dependencies:

- [MediatR](https://github.com/jbogard/MediatR): In-process messaging and notification
- [YamlDotNet](https://github.com/aaubry/YamlDotNet): YAML parser and emitter for .NET

Yubi tries to keep the number of dependencies to a minimum to reduce the size of the application.

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
