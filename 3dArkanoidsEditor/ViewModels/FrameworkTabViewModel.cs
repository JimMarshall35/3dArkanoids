using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using _3dArkanoidsEditor.Models;

namespace _3dArkanoidsEditor.ViewModels
{
    class FrameworkTabViewModel : ViewModelBase
    {
        public FrameworkTabViewModel()
        {
            DrawableStack = new List<FrameworkLayerViewModel>();
            InputStack = new List<FrameworkLayerViewModel>();
            UpdateStack = new List<FrameworkLayerViewModel>();
        }

        public void RecieveUpdate(GameFrameworkStacks newStacks)
        {
            DrawableStack = newStacks.DrawableStack
                .Select(model => new FrameworkLayerViewModel(model))
                .ToList();

            InputStack = newStacks.InputStack
                .Select(model => new FrameworkLayerViewModel(model))
                .ToList();

            UpdateStack = newStacks.UpdatableStack
                .Select(model => new FrameworkLayerViewModel(model))
                .ToList();

            SetIsMasked(DrawableStack);
            SetIsMasked(UpdateStack);
            SetIsMasked(InputStack);
        }

        private void SetIsMasked(List<FrameworkLayerViewModel> stack)
        {
            for(int i=stack.Count -1; i >=0; i--)
            {
                FrameworkLayerViewModel thisOne = stack[i];
                thisOne.IsMasked = false;
                if (thisOne.MasksPrevious)
                {
                    break;
                }
            }
        }

        private List<FrameworkLayerViewModel> m_drawableStack;
        public List<FrameworkLayerViewModel> DrawableStack
        { 
            get => m_drawableStack;
            private set
            {
                m_drawableStack = value;
                OnPropertyChange(nameof(DrawableStack));
            }
        }

        private List<FrameworkLayerViewModel>  m_inputStack;
        public List<FrameworkLayerViewModel> InputStack 
        { 
            get => m_inputStack;
            private set
            {
                m_inputStack = value;
                OnPropertyChange(nameof(InputStack));
            }
        }

        private List<FrameworkLayerViewModel> m_updateStack;
        public List<FrameworkLayerViewModel> UpdateStack
        {
            get => m_updateStack;
            private set
            {
                m_updateStack = value;
                OnPropertyChange(nameof(UpdateStack));
            }
        }

    }
}
