namespace _3dArkanoidsEditor.Models
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Numerics;
    using System.Text;
    using System.Threading.Tasks;


    class SerializableProperty
    {
        public uint Uint32 { get; set; }
        public ushort Uint16 { get; set; }
        public byte Uint8 { get; set; }
        public int Int32 { get; set; }
        public short Int16 { get; set; }
        public byte Int8 { get; set; }
        public float Float { get; set; }
        public double Double { get; set; }
        public string Bytes { get; set; }
        public Vec2 Vec2 { get; set; }
        public Vec3 Vec3 { get; set; }
        public Vec4 Vec4 { get; set; }
        public SerializablePropertiesNode SerializableNode { get; set; }
        public List<SerializablePropertiesNode> SerializableNodesArray { get; set; }
        public SerializablePropertyType PropType { get; set; }
        public string Name { get; set; }
    }
}
