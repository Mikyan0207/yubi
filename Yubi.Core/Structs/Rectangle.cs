using System.Runtime.InteropServices;

namespace Yubi.Core.Structs;

[StructLayout(LayoutKind.Sequential)]
public class Rectangle
{
    public int Left;

    public int Top;

    public int Right;

    public int Bottom;
}