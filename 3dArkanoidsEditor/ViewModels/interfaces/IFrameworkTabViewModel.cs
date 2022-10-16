using _3dArkanoidsEditor.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.ViewModels.interfaces
{
    interface IFrameworkTabViewModel
    {
        public List<FrameworkLayerViewModel> UpdateStack { get; }
        public List<FrameworkLayerViewModel> InputStack { get; }
        public List<FrameworkLayerViewModel> DrawableStack { get; }
        public void RecieveUpdate(GameFrameworkStacks newStacks);
    }
}
