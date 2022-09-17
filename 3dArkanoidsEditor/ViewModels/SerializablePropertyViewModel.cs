using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using _3dArkanoidsEditor.Models;
using _3dArkanoidsEditor.Services;

namespace _3dArkanoidsEditor.ViewModels
{
    class SerializablePropertyViewModel
    {
        public SerializablePropertyViewModel(SerializableProperty serializableProperty, SerializablePropertiesNodeViewModel parent, IGameEditorClient editorClient)
        {
            m_serializableProperty = serializableProperty;
            m_parent = parent;
            m_gameEditorClient = editorClient;
        }
        
        private string GetPath()
        {
            var pathString = Name;
            var thisPropertiesParent = m_parent;

            while (thisPropertiesParent != null)
            {
                if (thisPropertiesParent.Parent != null)
                {
                    pathString = thisPropertiesParent.NameOfParentProperty + "[" + thisPropertiesParent.Index + "]." + pathString;
                }
                else
                {
                    pathString = thisPropertiesParent.Name + "." + pathString;
                }
                thisPropertiesParent = thisPropertiesParent.Parent;
            }
            return pathString;
        }

        private void HandlePropertySet<T>(T val,Action<T, string> func)
        {
            var path = GetPath();
            func(val, path);
            m_gameEditorClient.SetSerializablePropertyAsync(m_serializableProperty, path);
        }

        public uint Uint32 
        { 
            get => m_serializableProperty.Uint32; 
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Uint32 = val;

            });
        }

        public ushort Uint16
        {
            get => m_serializableProperty.Uint16; 
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Uint16 = val;
            });
        }

        public byte Uint8 
        { 
            get => m_serializableProperty.Uint8; 
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Uint8 = val;
            });
        }

        public int Int32 
        { 
            get => m_serializableProperty.Int32; 
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Int32 = val;
            });
        }

        public short Int16 
        { 
            get => m_serializableProperty.Int16;
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Int16 = val;
            });
        }

        public byte Int8
        {
            get => m_serializableProperty.Int8;
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Int8 = val;
            });
        }

        public float Float 
        { 
            get => m_serializableProperty.Float; 
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Float = val;
            });
        }

        public double Double
        {
            get => m_serializableProperty.Double;
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Double = val;
            });
        }
        public string Bytes
        {
            get => m_serializableProperty.Bytes;
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Bytes = val;
            });
        }
        public float Vec2X 
        {
            get => m_serializableProperty.Vec2.X;
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Vec2.X = val;
            });
        }
        public float Vec2Y 
        {
            get => m_serializableProperty.Vec2.Y;
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Vec2.Y = val;
            });
        }

        public float Vec3X
        {
            get => m_serializableProperty.Vec3.X;
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Vec3.X = val;
            });
        }
        public float Vec3Y
        {
            get => m_serializableProperty.Vec3.Y;
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Vec3.Y = val;
            });
        }
        public float Vec3Z
        {
            get => m_serializableProperty.Vec3.Z;
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Vec3.Z = val;
            });
        }

        public float Vec4R
        {
            get => m_serializableProperty.Vec4.R;
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Vec4.R = val;
            });
        }
        public float Vec4G
        {
            get => m_serializableProperty.Vec4.G;
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Vec4.G = val;
            });
        }
        public float Vec4B
        {
            get => m_serializableProperty.Vec4.B;
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Vec4.B = val;
            });
        }
        public float Vec4A
        {
            get => m_serializableProperty.Vec4.A;
            set => HandlePropertySet(value, (val, path) =>
            {
                m_serializableProperty.Vec4.A = val;
            });
        }

        //public SerializablePropertiesNode SerializableNode { get; set; }
        //public List<SerializablePropertiesNode> SerializableNodesArray { get; set; }
        public SerializablePropertyType PropType { get => m_serializableProperty.PropType; set => m_serializableProperty.PropType = value; }
        public string Name { get => m_serializableProperty.Name; set => m_serializableProperty.Name = value; }

        private SerializableProperty m_serializableProperty;

        private SerializablePropertiesNodeViewModel m_parent;

        private IGameEditorClient m_gameEditorClient;
    }
}
