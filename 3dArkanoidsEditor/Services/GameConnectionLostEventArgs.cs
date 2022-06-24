using _3dArkanoidsEditor.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Services
{
    internal class GameConnectionLostEventArgs
    {
        internal GameConnectionLostEventArgs(string msg)
        {
            Message = msg;
        }

        internal string Message { get; private set; }
    }
}
