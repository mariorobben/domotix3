using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.Configuration;
using DeviceHost.Core;
using DeviceHost.Elements;

namespace DeviceHost.ViewModels
{
    public class InfrastructureViewModel : ViewModelBase
    {
        public InfrastructureViewModel(ProjectViewModel project, InfrastructureConfiguration infrastructureConfiguration)
        {
            Project = project;

            foreach (var group in infrastructureConfiguration.Groups)
            {
                _groups.Add(new GroupViewModel(this, new [] { group }));
            }
        }

        public IEnumerable<GroupViewModel> Groups => _groups;

        public InfrastructureTreeViewItemViewModel SelectedItem
        {
            get => _selectedItem;

            set
            {
                _selectedItem = value;

                RaisePropertyChanged(() => this.SelectedItem);
            }
        }

        public ProjectViewModel Project { get; }

        private IList<GroupViewModel> _groups = new List<GroupViewModel>();
        private InfrastructureTreeViewItemViewModel _selectedItem;
    }
}
