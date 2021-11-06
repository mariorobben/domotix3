using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.Configuration;
using DeviceHost.Devices.Wago;
using DeviceHost.DevicesConfiguration;

namespace DeviceHost.ViewModels
{
    //public static class ViewModelFactory
    //{
    //    private static IDictionary<Type, Type> _viewModelMappings;

    //    static ViewModelFactory()
    //    {
    //        //var catalog = new AssemblyCatalog(Assembly.GetExecutingAssembly());

    //        //_viewModelMappings = new Dictionary<Type, Type>();

    //        //foreach (var vieswModelType in catalog.Parts
    //        //    .Select(part => MefHelper.ComposablePartExportType<ViewModelBase>(part))
    //        //    .Where(t => t != null))
    //        //{
    //        //    foreach (var constructor in vieswModelType.GetConstructors())
    //        //    {
    //        //        if (constructor.GetParameters().Length == 1 && !constructor.GetParameters()[0].ParameterType.IsSubclassOf(typeof(ViewModelBase)))
    //        //        {
    //        //            _viewModelMappings.Add(constructor.GetParameters()[0].ParameterType, vieswModelType.UnderlyingSystemType);
    //        //        }
    //        //    }
    //        //}
    //    }

    //    public static ViewModelBase CreateViewModel(Object model)
    //    {
    //        if (model is ProjectConfiguration projectConfiguration)
    //        {
    //            return new ProjectViewModel(projectConfiguration);
    //        }
    //        else if (model is WagoDevice wagoDevice)
    //        {
    //            return new WagoViewModel(wagoDevice);
    //        }

    //        throw new ArgumentException(String.Format("No viewmodel mapping for type {0}.", model.GetType().FullName), "model");
    //    }
    //}
}
