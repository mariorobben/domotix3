using System;
using System.ComponentModel;
using System.Linq.Expressions;

namespace DeviceHost.ViewModels
{
    public class ViewModelBase : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public void RaisePropertyChanged<T>(Expression<Func<T>> propertyExpression)
        {
            var propertyName = ExtractPropertyName(propertyExpression);
            RaisePropertyChanged(propertyName);
        }

        private static string ExtractPropertyName<T>(Expression<Func<T>> propertyExpression)
        {
            var memberExpression = propertyExpression.Body as MemberExpression;
            return memberExpression.Member.Name;
        }

        public void RaisePropertyChanged(string propertyName)
        {
            var propertyChanged = PropertyChanged;
            if (propertyChanged != null)
            {
                propertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }
    }
}
