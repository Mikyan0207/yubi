using System.Diagnostics;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using Yubi.Application;
using Yubi.Infrastructure;

namespace Yubi.Startup;

internal static class Program
{
    private const string YubiId = "A01E65B6-FB56-4465-A899-A96E9FF4CFA1";

    /// <summary>
    ///     The main entry point for the application.
    /// </summary>
    [STAThread]
    private static void Main(string[] args)
    {
        EnsureSingleInstance();

        CreateHost(args)
            .Run();
    }

    /// <summary>
    ///     Creates the host for the application.
    /// </summary>
    /// <param name="args">The arguments passed to the application.</param>
    /// <returns>
    ///     The <see cref="IHost" /> for the application.
    /// </returns>
    private static IHost CreateHost(string[] args)
    {
        return Host.CreateDefaultBuilder(args)
            .ConfigureAppConfiguration(config =>
            {
                config.AddCommandLine(args, new Dictionary<string, string>
                {
                    ["-c"] = "ConfigPath",
                    ["--config"] = "ConfigPath",
                    ["-b"] = "RunInBackground",
                    ["--background"] = "RunInBackground"
                });
            })
            .ConfigureServices((context, services) =>
            {
                services.AddApplication();
                services.AddInfrastructure();
            })
            .Build();
    }

    private static void EnsureSingleInstance()
    {
        using var mutex = new Mutex(false, $"Global\\{YubiId}");

        if (mutex.WaitOne(0, false))
        {
            return;
        }

        Debug.Write("Another instance is already running.");
        Environment.Exit(-1);
    }
}