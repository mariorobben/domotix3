using System.IO;
using System.Threading.Tasks;
using System.Windows;
using DeviceHost.Configuration;
using DeviceHost.Devices;
using DeviceHost.Devices.Wago;
using DeviceHost.DevicesConfiguration;
using DeviceHost.ViewModels;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.DependencyInjection.Extensions;
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
            _projectConfiguration = JsonConvert.DeserializeObject<ProjectConfiguration>(File.ReadAllText("robben.json"));

            RegisterDevices();

            ServiceCollection services = new();
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
            var mainWindow = new MainWindow();
            mainWindow.Show();

            mainWindow.DataContext = new MainWindowViewModel(_deviceRepository, _projectConfiguration);
        }

        private void RegisterDevices()
        {
            _projectConfiguration.Devices.ForEach(deviceConfiguration =>
            {
                _deviceRepository.RegisterDevice(DeviceFactory.CreateDevice(deviceConfiguration));
            });
        }

        private readonly DeviceRepository _deviceRepository = new();
        private readonly ServiceProvider _serviceProvider;
        private ProjectConfiguration _projectConfiguration;
    }
}
