using System;
using System.Collections.Generic;

namespace DeviceHost.ViewModels
{
    public interface IElementConfigurationsViewModel
    {
        IEnumerable<Object> Elements { get; }
    }
}
