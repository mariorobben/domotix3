using System;

namespace DeviceHost.ViewModels
{
    public class TreeViewItemViewModel : ViewModelBase
    {
        public Boolean IsSelected
        {
            get { return _isSelected; }
            set
            {
                if (_isSelected != value)
                {
                    _isSelected = value;
                    RaisePropertyChanged(() => this.IsSelected);
                }
            }
        }

        public Boolean IsExpanded
        {
            get { return _isExpanded; }
            set
            {
                if (_isExpanded != value)
                {
                    _isExpanded = value;
                    RaisePropertyChanged(() => this.IsExpanded);
                }
            }
        }

        private Boolean _isSelected;
        private Boolean _isExpanded;
    }
}
