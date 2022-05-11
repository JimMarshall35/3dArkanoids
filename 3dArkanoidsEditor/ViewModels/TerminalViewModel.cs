using _3dArkanoidsEditor.utils;
using _3dArkanoidsEditor.ViewModels.interfaces;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace _3dArkanoidsEditor.ViewModels
{
    internal class TerminalViewModel : ViewModelBase, ITerminalViewModel
    {
        public TerminalViewModel()
        {
            ExecuteTerminalItemCommand = new RelayCommand(ExcecuteTerminalString);
            m_consoleItems.Add("3D Arkanoids");
        }
        private object m_lock = new object();
        private ObservableCollection<string> m_consoleItems = new ObservableCollection<string>();
        public IEnumerable<string> ConsoleItems => m_consoleItems;

        public ICommand ExecuteTerminalItemCommand { get; set; }

        public void WriteLine(string line)
        {
            lock (m_lock)
            {
                m_consoleItems.Add(line);
            }
        }
        private void ExcecuteTerminalString(object e)
        {
            string command = (string)e;
            if (command == "cls")
            {
                m_consoleItems.Clear();
                return;
            }
            WriteLine(command);
        }
    }
}
