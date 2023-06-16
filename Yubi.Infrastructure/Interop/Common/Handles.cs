using System.Runtime.InteropServices;

namespace Yubi.Infrastructure.Interop.Common;

public static class Handles
{
    /// <summary>
    ///     A <see cref="HandleRef" /> that represents a null handle.
    /// </summary>
    public static HandleRef NullHandleRef = new(null, IntPtr.Zero);
}