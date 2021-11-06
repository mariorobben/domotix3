using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceHost.ViewModels
{
    public abstract class InfrastructureTreeViewItemViewModel : TreeViewItemViewModel, IElementConfigurationsViewModel
    {
        public abstract IEnumerable<Object> Elements { get; }
    }
}
