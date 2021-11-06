using System;
using DeviceHost.Core;

namespace DeviceHost.ViewModels
{
    public interface IElementViewModelRepository
    {
        Object GetElementViewModel(IElementReference element);
    }
}
