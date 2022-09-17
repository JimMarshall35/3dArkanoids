using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using _3dArkanoidsEditor.Models;
using _3dArkanoidsEditor.Services;

namespace _3dArkanoidsEditor.ViewModels
{
    class SerializablePropertiesNodeViewModel
    {
        public SerializablePropertiesNodeViewModel(SerializablePropertiesNode node, IGameEditorClient editorClient, SerializablePropertiesNodeViewModel parent = null, int indexInArrayOfChildren = 0)
        {
            Parent = parent;
            m_node = node;
            Index = indexInArrayOfChildren;
            m_editorClient = editorClient;
        }

        public string Name
        {
            get => m_node.Name;
            set => m_node.Name = value;
        }
        public IEnumerable<SerializablePropertyViewModel> Props
        {
            get => m_node.Props
                .Select(x => new SerializablePropertyViewModel(x, this, m_editorClient));
        }

        public IEnumerable<SerializablePropertiesNodeViewModel> Children
        {
            get => m_node.Children
                    .Select((x, i) => new SerializablePropertiesNodeViewModel(x,m_editorClient, this, i));
        }

        public SerializablePropertiesNodeViewModel Parent{ get; private set; }

        public string NameOfParentProperty { get => m_node.NameOfParentProperty; }

        private readonly SerializablePropertiesNode m_node;

        private IGameEditorClient m_editorClient;

        public int Index { get; private set; }
    }
}
