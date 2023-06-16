using System.Runtime.InteropServices;
using Yubi.Core.Structs;

namespace Yubi.Infrastructure.Interop;

/// <summary>
///     Interop methods for Windows API.
/// </summary>
/// <remarks>
///     We use <see cref="LibraryImportAttribute" /> to take advantage of the new source generator feature for DLL imports.
///     <br />
///     This will generate the P/Invoke signatures at compile time, instead of runtime.
/// </remarks>
public static partial class WindowsApi
{
    /// <summary>
    ///     An application-defined callback function used with the EnumDisplayMonitors function.
    /// </summary>
    /// <param name="hDesktop">A handle to the display monitor. This value will always be non-NULL.</param>
    /// <param name="hdc">
    ///     A handle to a device context for the display monitor. The device context has color attributes that
    ///     are appropriate.
    /// </param>
    /// <param name="pRect">A pointer to a RECT structure.</param>
    /// <param name="dwData">Application-defined data that EnumDisplayMonitors passes directly to the enumeration function.</param>
    /// <returns>
    ///     <c>true</c> to continue the enumeration; <c>false</c> to stop the enumeration.
    /// </returns>
    public delegate bool EnumMonitorsCallback(IntPtr hDesktop, IntPtr hdc, ref Rectangle pRect, IntPtr dwData);

    /// <summary>
    ///     An application-defined callback function used with the EnumWindows or EnumDesktopWindows function.
    /// </summary>
    /// <param name="hWnd">A handle to a top-level window.</param>
    /// <param name="lParam">The application-defined value given in EnumWindows or EnumDesktopWindows.</param>
    /// <returns>
    ///     To continue enumeration, the callback function must return <c>true</c>; to stop enumeration, it must return
    ///     <c>false</c>.
    /// </returns>
    public delegate bool EnumWindowsCallback(IntPtr hWnd, int lParam);

    /// <summary>
    ///     Enumerates all top-level windows on the screen by passing the handle to each window, in turn, to an
    ///     application-defined callback function.
    /// </summary>
    /// <param name="callback">A pointer to an application-defined callback function.</param>
    /// <param name="lParam">An application-defined value to be passed to the callback function.</param>
    /// <returns>
    ///     <c>true</c> if the function succeeds, <c>false</c> if not.
    /// </returns>
    [LibraryImport("user32.dll", EntryPoint = "EnumWindows", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    public static partial bool EnumWindows(EnumWindowsCallback callback, IntPtr lParam);

    /// <summary>
    ///     Indicates whether the specified window is visible or not.
    /// </summary>
    /// <param name="hWnd">The handle of the window.</param>
    /// <returns>
    ///     <c>true</c> if the window is visible, <c>false</c> if not.
    /// </returns>
    [LibraryImport("user32.dll")]
    [return: MarshalAs(UnmanagedType.Bool)]
    public static partial bool IsWindowVisible(IntPtr hWnd);

    /// <summary>
    ///     Retrieves the dimensions of the bounding rectangle of the specified window.
    /// </summary>
    /// <param name="hdc">A handle to the device context (DC) for the window.</param>
    /// <param name="lpRect">A pointer to a RECT structure that receives the screen coordinates.</param>
    /// <param name="callback">
    ///     A pointer to an application-defined callback function. For more information, see
    ///     <see cref="EnumMonitorsCallback" />.
    /// </param>
    /// <param name="dwData">Application-defined data that EnumDisplayMonitors passes directly to the enumeration function.</param>
    /// <returns>
    ///     If the function succeeds, the return value is <c>true</c>.
    /// </returns>
    [LibraryImport("user32.dll")]
    [return: MarshalAs(UnmanagedType.Bool)]
    public static partial bool EnumDisplayMonitors(
        IntPtr hdc,
        IntPtr lpRect,
        EnumMonitorsCallback callback,
        IntPtr dwData
    );

    /// <summary>
    ///     Retrieves information about the specified window.
    ///     The function also retrieves the value at a specified offset into the extra window memory.
    /// </summary>
    /// <param name="hWnd">A handle to the window and, indirectly, the class to which the window belongs.</param>
    /// <param name="index">The zero-based offset to the value to be retrieved.</param>
    /// <returns>
    ///     If the function succeeds, the return value is the requested value.
    /// </returns>
    public static IntPtr GetWindowLongPtr(IntPtr hWnd, int index)
    {
        return Environment.Is64BitProcess
            ? GetWindowLongPtr64(hWnd, index)
            : GetWindowLongPtr32(hWnd, index);
    }

    [LibraryImport("user32.dll", EntryPoint = "GetWindowLong")]
    private static partial IntPtr GetWindowLongPtr32(IntPtr hWnd, int index);

    [LibraryImport("user32.dll", EntryPoint = "GetWindowLongPtr")]
    private static partial IntPtr GetWindowLongPtr64(IntPtr hWnd, int index);
}