using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using _3dArkanoidsEditor.Models;

namespace _3dArkanoidsEditor.ViewModels
{
    class SerializablePropertiesNodeViewModel
    {
        public SerializablePropertiesNodeViewModel(SerializablePropertiesNode node)
        {
            m_node = node;
        }

        public string Name 
        {
            get => m_node.Name;
            set => m_node.Name = value;
        }
        public IEnumerable<SerializablePropertyViewModel> Props 
        {
            get => m_node.Props
                .Select(x => new SerializablePropertyViewModel(x));
        }

        public IEnumerable<SerializablePropertiesNodeViewModel> Children
        {
            get => m_node.Children
                    .Select(x => new SerializablePropertiesNodeViewModel(x));
        }

        private readonly SerializablePropertiesNode m_node;
    }
}
