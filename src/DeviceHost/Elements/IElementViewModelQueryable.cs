using System;
using DeviceHost.Core;

namespace DeviceHost.Elements
{
    public interface IElementViewModelQueryable
    {
        Object QueryElementViewModel(IElementReference element);
    }
}
