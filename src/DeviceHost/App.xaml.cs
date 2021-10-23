using System.IO;
using System.Threading.Tasks;
using System.Windows;
using DeviceHost.Configuration;
using DeviceHost.Devices.Wago;
using DeviceHost.DevicesConfiguration;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.DependencyInjection;
using Newtonsoft.Json;

namespace DeviceHost
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public App()
        {
            RegisterDevices();

            ServiceCollection services = new ServiceCollection();
            ConfigureServices(services);
            _serviceProvider = services.BuildServiceProvider();

            Task.Run(() =>
            {
                var host = new WebHostBuilder()
                    .ConfigureServices(services =>
                    {
                        services.AddSingleton<IDeviceRepository>(_deviceRepository);
                    })
                    .UseKestrel()
                    .UseUrls("http://*:5000")
                    .UseStartup<Startup>()
                    .Build();

                host.Run();
            });
        }

        private void ConfigureServices(ServiceCollection services)
        {
            services.AddSingleton<IDeviceRepository>(_deviceRepository);
            services.AddSingleton<MainWindow>();
        }

        private void OnStartup(object sender, StartupEventArgs e)
        {
            var mainWindow = _serviceProvider.GetService<MainWindow>();
            mainWindow.Show();
        }

        private void RegisterDevices()
        {
            var projectConfiguration = JsonConvert.DeserializeObject<ProjectConfiguration>(
                File.ReadAllText("robben.json"));
            projectConfiguration.Devices.ForEach(deviceConfiguration =>
            {
                switch (deviceConfiguration.Type)
                {
                    case "wago":
                    {
                        var wagoDeviceConfiguration = deviceConfiguration as WagoDeviceConfiguration;
                        _deviceRepository.RegisterDevice(new WagoDevice(wagoDeviceConfiguration.Name, wagoDeviceConfiguration.Modules));
                        break;
                    }
                }
            });
        }

        private DeviceRepository _deviceRepository = new DeviceRepository();
        private ServiceProvider _serviceProvider;
    }
}
