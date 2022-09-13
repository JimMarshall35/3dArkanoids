using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Models
{
    enum SerializablePropertyType
    {
        Uint32 = 0,
        Uint16 = 1,
        Uint8 = 2,
        Int32 = 3,
        Int16 = 4,
        Int8 = 5,
        Float = 6,
        Double = 7,
        Bytes = 8,
        Vec2 = 9,
        Vec3 = 10,
        Vec4 = 11,
        SerializableNode = 12,
        SerializableNodesArray = 13,
        ErrorValue = 14
    }
}
