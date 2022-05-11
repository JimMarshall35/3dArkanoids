using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace _3dArkanoidsEditor.ViewModels.interfaces
{
    internal interface ITerminalViewModel
    {
        public IEnumerable<string> ConsoleItems { get; }
        public void WriteLine(string line);
        public ICommand ExecuteTerminalItemCommand { get; set; }
    }
}
