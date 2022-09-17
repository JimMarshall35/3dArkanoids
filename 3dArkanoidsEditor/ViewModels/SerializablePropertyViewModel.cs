using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using _3dArkanoidsEditor.Models;

namespace _3dArkanoidsEditor.ViewModels
{
    class SerializablePropertyViewModel
    {
        public SerializablePropertyViewModel(SerializableProperty serializableProperty)
        {
            m_serializableProperty = serializableProperty;
        }

        public uint Uint32 { get => m_serializableProperty.Uint32; set => m_serializableProperty.Uint32 = value; }
        public ushort Uint16 { get => m_serializableProperty.Uint16; set => m_serializableProperty.Uint16 = value; }
        public byte Uint8 { get => m_serializableProperty.Uint8; set => m_serializableProperty.Uint8 = value; }
        public int Int32 { get => m_serializableProperty.Int32; set => m_serializableProperty.Int32 = value; }
        public short Int16 { get => m_serializableProperty.Int16; set => m_serializableProperty.Int16 = value; }
        public byte Int8 { get => m_serializableProperty.Int8; set => m_serializableProperty.Int8 = value; }
        public float Float { get => m_serializableProperty.Float; set => m_serializableProperty.Float = value; }
        public double Double { get => m_serializableProperty.Double; set => m_serializableProperty.Double = value; }
        public string Bytes { get => m_serializableProperty.Bytes; set => m_serializableProperty.Bytes = value; }
        public Vec2 Vec2 { get => m_serializableProperty.Vec2; set => m_serializableProperty.Vec2 = value; }
        public Vec3 Vec3 { get => m_serializableProperty.Vec3; set => m_serializableProperty.Vec3 = value; }
        public Vec4 Vec4 { get => m_serializableProperty.Vec4; set => m_serializableProperty.Vec4 = value; }
        //public SerializablePropertiesNode SerializableNode { get; set; }
        //public List<SerializablePropertiesNode> SerializableNodesArray { get; set; }
        public SerializablePropertyType PropType { get => m_serializableProperty.PropType; set => m_serializableProperty.PropType = value; }
        public string Name { get => m_serializableProperty.Name; set => m_serializableProperty.Name = value; }

        private SerializableProperty m_serializableProperty;
    }
}
