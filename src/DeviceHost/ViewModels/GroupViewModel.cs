using System;
using System.Collections.Generic;
using System.Linq;
using DeviceHost.Core;
using DeviceHost.Elements;
using DeviceHost.Configuration;

namespace DeviceHost.ViewModels
{
    public class GroupViewModel : InfrastructureTreeViewItemViewModel
    {
        public GroupViewModel(InfrastructureViewModel infrastructureViewModel, IReadOnlyCollection<GroupConfiguration> groupHierarchy)
        {
            _infrastructureViewModel = infrastructureViewModel;
            _groupHierarchy = groupHierarchy;
            _groupsViewModel = new List<GroupViewModel>();
            if (groupHierarchy.Last().Groups is not null)
            {
                foreach (var child in groupHierarchy.Last().Groups)
                {
                    _groupsViewModel.Add(new GroupViewModel(infrastructureViewModel,
                        groupHierarchy.Concat(new[] { child }).ToList().AsReadOnly()));
                }
            }

            _elementReferences = new Dictionary<IElementConfiguration, IElementReference>();
            if (groupHierarchy.Last().Elements is not null)
            {
                foreach (var element in groupHierarchy.Last().Elements)
                {
                    _elementReferences.Add(element, ElementFactory.CreateElementReference(element));
                }
            }
        }

        public String Name
        {
            get { return _groupHierarchy.Last().Name; }
        }

        public IEnumerable<GroupViewModel> Groups
        {
            get
            {
                return _groupsViewModel;
            }
        }

        public override IEnumerable<Object> Elements
        {
            get
            {
                if (_groupHierarchy.Last().Elements is not null)
                {
                    foreach (var element in _groupHierarchy.Last().Elements)
                    {
                        yield return new
                        {
                            Location = string.Join(" / ", _groupHierarchy.Select(g => g.Name)),
                            Element = _infrastructureViewModel.Project.GetElementViewModel(_elementReferences[element])
                        };
                    }
                }


                foreach (var group in _groupsViewModel)
                {
                    foreach (var element in group.Elements)
                    {
                        yield return element;
                    }
                }
            }
        }

        private InfrastructureViewModel _infrastructureViewModel;
        private IReadOnlyCollection<GroupConfiguration> _groupHierarchy;

        private IList<GroupViewModel> _groupsViewModel;
        private IDictionary<IElementConfiguration, IElementReference> _elementReferences;
    }
}
