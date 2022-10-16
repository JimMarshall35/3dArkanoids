using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Models
{
    public class GameFrameworkStacks
    {
        public GameFrameworkStacks(
            List<GameFrameworkStackItem> updatableStack,
            List<GameFrameworkStackItem> drawableStack,
            List<GameFrameworkStackItem> inputStack)
        {
            UpdatableStack = updatableStack;
            DrawableStack = drawableStack;
            InputStack = inputStack;
        }
        public List<GameFrameworkStackItem> UpdatableStack { get; private set; }
        public List<GameFrameworkStackItem> DrawableStack { get; private set; }
        public List<GameFrameworkStackItem> InputStack { get; private set; }


    }
}
