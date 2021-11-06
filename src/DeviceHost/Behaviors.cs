using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interactivity;

namespace DeviceHost
{
    // code taken from http://blog.magnusmontin.net/2014/01/30/wpf-using-behaviours-to-bind-to-readonly-properties-in-mvvm/

    public class TreeViewSelectedItemBlendBehavior : Behavior<TreeView>
    {
        //dependency property
        public static readonly DependencyProperty SelectedItemProperty =
            DependencyProperty.Register("SelectedItem", typeof(object),
                typeof(TreeViewSelectedItemBlendBehavior),
                new FrameworkPropertyMetadata(null) { BindsTwoWayByDefault = true });

        //property wrapper
        public object SelectedItem
        {
            get { return (object)GetValue(SelectedItemProperty); }
            set { SetValue(SelectedItemProperty, value); }
        }

        protected override void OnAttached()
        {
            base.OnAttached();
            this.AssociatedObject.SelectedItemChanged += OnTreeViewSelectedItemChanged;
        }

        protected override void OnDetaching()
        {
            base.OnDetaching();
            if (this.AssociatedObject != null)
                this.AssociatedObject.SelectedItemChanged -= OnTreeViewSelectedItemChanged;
        }

        private void OnTreeViewSelectedItemChanged(object sender,
            RoutedPropertyChangedEventArgs<object> e)
        {
            if (e.NewValue == null)
            {
                return;
            }
            this.SelectedItem = e.NewValue;
        }
    }
}
