using Microsoft.Extensions.DependencyInjection;

namespace Yubi.Infrastructure;

/// <summary>
///     Extension methods for setting up infrastructure services in an <see cref="IServiceCollection" />.
/// </summary>
public static class DependencyInjection
{
    /// <summary>
    ///     Adds infrastructure services to the <see cref="IServiceCollection" />.
    /// </summary>
    /// <param name="services">The <see cref="IServiceCollection" /> to add the services to.</param>
    /// <returns>
    ///     The <see cref="IServiceCollection" /> so that additional calls can be chained.
    /// </returns>
    public static IServiceCollection AddInfrastructure(this IServiceCollection services)
    {
        return services;
    }
}