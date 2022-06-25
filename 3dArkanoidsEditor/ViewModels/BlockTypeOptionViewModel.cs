using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using _3dArkanoidsEditor.Models;

namespace _3dArkanoidsEditor.ViewModels
{
    internal class BlockTypeOptionViewModel : ViewModelBase
    {
        private GameBlockType m_gameBlockType;
        public GameBlockType GameBlockType
        {
            get
            {
                return m_gameBlockType;
            }
            private set
            {
                m_gameBlockType = value;
                OnPropertyChange(nameof(GameBlockType));
            }
        }
        public BlockTypeOptionViewModel(GameBlockType blockType)
        {
            GameBlockType = blockType;
        }

    }
}
