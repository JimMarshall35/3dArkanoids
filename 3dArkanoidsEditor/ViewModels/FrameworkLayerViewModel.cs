using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using _3dArkanoidsEditor.Models;

namespace _3dArkanoidsEditor.ViewModels
{
    class FrameworkLayerViewModel : ViewModelBase
    {
        public FrameworkLayerViewModel(GameFrameworkStackItem item)
        {
            m_stackLayerModel = item;
        }
        public string Name { get => m_stackLayerModel.Name; }
        public bool MasksPrevious { get => m_stackLayerModel.MasksPrevious; }

        private GameFrameworkStackItem m_stackLayerModel;

        private bool m_isMasked = true;
        public bool IsMasked
        {
            get => m_isMasked;
            set
            {
                m_isMasked = value;
                OnPropertyChange(nameof(IsMasked));
            }
        }
    }
}
