using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace _3dArkanoidsEditor.ViewModels
{
    internal class CanvasZLayerChangerViewModel : ViewModelBase
    {
        public CanvasZLayerChangerViewModel(Action<int> OnLayerChanged, int TotalLayers)
        {
            m_onLayerChanged = OnLayerChanged;
            MaxLayerIndex = TotalLayers - 1;
        }
        private bool m_enabled;
        public bool Enabled
        {
            get
            {
                return m_enabled;
            }
            set
            {
                m_enabled = value;
                OnPropertyChange(nameof(Enabled));
            }
        }
        private int m_currentLayer = 0;
        public int CurrentLayer
        {
            get
            {
                return m_currentLayer;
            }
            set
            {
                m_currentLayer = value;
                OnPropertyChange(nameof(CurrentLayer));
                m_onLayerChanged(value);
            }
        }

        private int m_maxLayerIndex = 0;
        public int MaxLayerIndex
        {
            get
            {
                return m_maxLayerIndex;
            }
            private set
            {
                m_maxLayerIndex = value;
                OnPropertyChange(nameof(MaxLayerIndex));
            }
        }

        private Visibility m_visibility = Visibility.Hidden; // should use a converter class and a bool for this for maximum MVVM... but can't be bothered
        public Visibility Visibility
        {
            get
            {
                return m_visibility;
            }
            set
            {
                m_visibility = value;
                OnPropertyChange(nameof(Visibility));
            }
        }

        public void SetMaxLayerIndex(int totalNumZLayers)
        {
            MaxLayerIndex = totalNumZLayers - 1;
        }

        private readonly Action<int> m_onLayerChanged;
        
    }
}
