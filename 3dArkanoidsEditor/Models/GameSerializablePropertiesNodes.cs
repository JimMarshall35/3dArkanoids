using System;
using System.Collections.Generic;
using System.Linq;

namespace _3dArkanoidsEditor.Models
{
    using System.Text;
    using System.Threading.Tasks;
    using System.Collections.Generic;
    using System.Numerics;
    using System.Collections.ObjectModel;

    class SerializablePropertiesNode
    {
        public SerializablePropertiesNode(string name, List<SerializableProperty> props, string nameOfParentProperty = null)
        {
            Name = name;
            Props = props;
            NameOfParentProperty = nameOfParentProperty;
        }

        public SerializablePropertiesNode()
        {
            Props = new List<SerializableProperty>();
        }
        public string Name { get; set; }
        public List<SerializableProperty> Props { get; set; }

        public string NameOfParentProperty { get; private set; }

        public IEnumerable<SerializablePropertiesNode> Children 
        { 
            get
            {
                return Props.Where(x => x.PropType == SerializablePropertyType.SerializableNodesArray)
                    .SelectMany(x => x.SerializableNodesArray);
            }
        }

    }

}
