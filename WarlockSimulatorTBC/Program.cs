using Microsoft.AspNetCore.Components.WebAssembly.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using Blazored.LocalStorage;
using BlazorWorker.Core;
using WarlockSimulatorTBC.ViewModels.Classes;
using WarlockSimulatorTBC.ViewModels.Interfaces;

namespace WarlockSimulatorTBC
{
	public class Program
	{
		public static async Task Main(string[] args)
		{
			var builder = WebAssemblyHostBuilder.CreateDefault(args);
			builder.RootComponents.Add<App>("#app");

			builder.Services.AddScoped(sp => new HttpClient { BaseAddress = new Uri(builder.HostEnvironment.BaseAddress) });
            builder.Services.AddScoped<ISidebarViewModel, SidebarViewModel>();
			builder.Services.AddBlazoredLocalStorage();
			builder.Services.AddWorkerFactory();

			await builder.Build().RunAsync();
		}
	}
}
