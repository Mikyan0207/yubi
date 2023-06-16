using Microsoft.Extensions.DependencyInjection;

namespace Yubi.Application;

/// <summary>
///     Extension methods for setting up application services in an <see cref="IServiceCollection" />.
/// </summary>
public static class DependencyInjection
{
    /// <summary>
    ///     Adds application services to the <see cref="IServiceCollection" />.
    /// </summary>
    /// <param name="services">The <see cref="IServiceCollection" /> to add the services to.</param>
    /// <returns>
    ///     The <see cref="IServiceCollection" /> so that additional calls can be chained.
    /// </returns>
    public static IServiceCollection AddApplication(this IServiceCollection services)
    {
        services.AddMediatR(c => { c.RegisterServicesFromAssemblies(); });

        return services;
    }
}