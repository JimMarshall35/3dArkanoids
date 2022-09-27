using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Google.Protobuf;
using static _3dArkanoidsEditor.Models.EditBlockResult;

namespace _3dArkanoidsEditor.Services
{
    internal static class GRPCConversionHelperExtensions
    {
        /// <summary>
        /// extension method for Models.GameBoardDescription to convert it
        /// to grpc generated class
        /// </summary>
        /// <param name="grpc"></param>
        /// <returns></returns>
        public static Models.GameBoardDescription ToModel(this EditorGRPC.BoardDescription grpc)
        {
            return new Models.GameBoardDescription(
                    grpc.Data.ToList<byte>(),
                    grpc.Width,
                    grpc.Height,
                    grpc.Depth);
        }

        /// <summary>
        /// extension method for the grpc generated BoardDescription
        /// class to convert it into the model class used internally
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        public static EditorGRPC.BoardDescription ToGRPCMessage(this Models.GameBoardDescription model)
        {
            var grpc = new EditorGRPC.BoardDescription();
            grpc.Data = ByteString.CopyFrom(model.PlayfieldArray.ToArray());
            grpc.Depth = model.Depth;
            grpc.Width = model.Width;
            grpc.Height = model.Height;
            return grpc;
        }

        public static EditorGRPC.BlockEdit ToGRPCMessage(this Models.SingleTileEdit singleTileEdit)
        {
            var grpc = new EditorGRPC.BlockEdit();
            grpc.X = singleTileEdit.XPos;
            grpc.Y = singleTileEdit.YPos;
            grpc.Z = singleTileEdit.ZPos;
            grpc.Newval = singleTileEdit.NewValue;
            return grpc;
        }

        public static Models.EditBlockResult ToModel(this EditorGRPC.EditBlockResult result)
        {
            return new Models.EditBlockResult((Result)result.Result, result.BlockCode, result.ErrorMessage);
        }

        public static Models.GameSettings ToModel(this EditorGRPC.GameSettings input)
        {
            var models = input.PossibleBlocks
                .Select(x => new Models.GameBlockType((byte)x.GameEngineCode, x.Red, x.Green, x.Blue, x.Alpha))
                .ToList();
            return new Models.GameSettings(models);
        }
        public static List<Models.SerializablePropertiesNode> ToModel(this EditorGRPC.SerializablePropertiesNodes nodes)
        {
            return
                nodes.Nodes
                    .Select(x => x.ToModel())
                    .ToList();
        }

        public static Models.SerializablePropertiesNode ToModel(this EditorGRPC.SerializablePropertiesNode node, string nameOfParentProperty=null)
        {
            return new Models.SerializablePropertiesNode(
                        node.Name,
                        node.Props
                            .Select(y => y.ToModel())
                            .ToList(),
                        nameOfParentProperty);
        }

        public static Models.SerializableProperty ToModel(this EditorGRPC.SerializableProperty prop)
        {
            Models.SerializableProperty returnVal = null;
            
            switch (prop.Type)
            {
                case EditorGRPC.SerializableProperty.Types.Type.Int8:
                    returnVal = new Models.SerializableProperty()
                    {
                        Int8 = (byte)prop.Data.I8,
                        PropType = Models.SerializablePropertyType.Int8
                    };
                    break;
                case EditorGRPC.SerializableProperty.Types.Type.Int16:
                    returnVal = new Models.SerializableProperty()
                    {
                        Int16 = (byte)prop.Data.I16,
                        PropType = Models.SerializablePropertyType.Int16
                    };
                    break;
                case EditorGRPC.SerializableProperty.Types.Type.Int32:
                    returnVal = new Models.SerializableProperty()
                    {
                        Int32 = (byte)prop.Data.I32,
                        PropType = Models.SerializablePropertyType.Int32
                    };
                    break;
                case EditorGRPC.SerializableProperty.Types.Type.Uint8:
                    returnVal = new Models.SerializableProperty()
                    {
                        Uint8 = (byte)prop.Data.U8,
                        PropType = Models.SerializablePropertyType.Uint8
                    };
                    break;
                case EditorGRPC.SerializableProperty.Types.Type.Uint16:
                    returnVal = new Models.SerializableProperty()
                    {
                        Uint16 = (byte)prop.Data.U16,
                        PropType = Models.SerializablePropertyType.Uint16
                    };
                    break;
                case EditorGRPC.SerializableProperty.Types.Type.Uint32:
                    returnVal = new Models.SerializableProperty()
                    {
                        Uint32 = (byte)prop.Data.U32,
                        PropType = Models.SerializablePropertyType.Uint32
                    };
                    break;
                case EditorGRPC.SerializableProperty.Types.Type.Float:
                    returnVal = new Models.SerializableProperty()
                    {
                        Float = (float)prop.Data.F,
                        PropType = Models.SerializablePropertyType.Float
                    };
                    break;
                case EditorGRPC.SerializableProperty.Types.Type.Double:
                    returnVal = new Models.SerializableProperty()
                    {
                        Double = (float)prop.Data.D,
                        PropType = Models.SerializablePropertyType.Double
                    };
                    break;
                case EditorGRPC.SerializableProperty.Types.Type.Bytes:
                    returnVal = new Models.SerializableProperty()
                    {
                        Bytes = prop.Data.B.ToString(),
                        PropType = Models.SerializablePropertyType.Bytes
                    };
                    break;
                case EditorGRPC.SerializableProperty.Types.Type.Vec2:
                    returnVal = new Models.SerializableProperty()
                    {
                        Vec2 = new Models.Vec2() { X=prop.Data.V2.X, Y=prop.Data.V2.Y},
                        PropType = Models.SerializablePropertyType.Vec2
                    };
                    break;
                case EditorGRPC.SerializableProperty.Types.Type.Vec3:
                    returnVal = new Models.SerializableProperty()
                    {
                        Vec3 = new Models.Vec3() { X = prop.Data.V3.X, Y = prop.Data.V3.Y, Z = prop.Data.V3.Z },
                        PropType = Models.SerializablePropertyType.Vec3
                    };
                    break;
                case EditorGRPC.SerializableProperty.Types.Type.Vec4:
                    returnVal = new Models.SerializableProperty()
                    {
                        Vec4 = new Models.Vec4() { R = prop.Data.V4.R, G = prop.Data.V4.G, B = prop.Data.V4.B, A = prop.Data.V4.A },
                        PropType = Models.SerializablePropertyType.Vec4
                    };
                    break;
                case EditorGRPC.SerializableProperty.Types.Type.SerializableNode:
                    returnVal = new Models.SerializableProperty()
                    {
                        SerializableNode = prop.Data.Children.Nodes.Select(x=>x.ToModel(prop.Name)).SingleOrDefault(),
                        PropType = Models.SerializablePropertyType.SerializableNode
                    };
                    break;
                case EditorGRPC.SerializableProperty.Types.Type.SerializableNodesArray:
                    returnVal = new Models.SerializableProperty()
                    {
                        SerializableNodesArray = prop.Data.Children.Nodes.Select(x => x.ToModel(prop.Name)).ToList(),
                        PropType = Models.SerializablePropertyType.SerializableNodesArray
                    };
                    break;

            }

            if(returnVal != null)
            {
                returnVal.Name = prop.Name;
            }
            return returnVal;
        }

        public static EditorGRPC.SetSerializablePropertyData ToGRPCSetMessage(this Models.SerializableProperty model, string path)
        {
            var rval = new EditorGRPC.SetSerializablePropertyData();
            rval.NewVal = new EditorGRPC.SerializableProperty();
            rval.NewVal.Data = new EditorGRPC.SerializablePropertyData();
            rval.Path = path;
            rval.NewVal.Name = model.Name;
            switch (model.PropType)
            {
                case Models.SerializablePropertyType.Uint32:
                    rval.NewVal.Data.U32 = model.Uint32;
                    rval.NewVal.Type = EditorGRPC.SerializableProperty.Types.Type.Uint32;
                    break;
                case Models.SerializablePropertyType.Uint16:
                    rval.NewVal.Data.U16 = model.Uint16;
                    rval.NewVal.Type = EditorGRPC.SerializableProperty.Types.Type.Uint16;
                    break;
                case Models.SerializablePropertyType.Uint8:
                    rval.NewVal.Data.U8 = model.Uint8;
                    rval.NewVal.Type = EditorGRPC.SerializableProperty.Types.Type.Uint8;
                    break;
                case Models.SerializablePropertyType.Int32:
                    rval.NewVal.Data.I32 = model.Int32;
                    rval.NewVal.Type = EditorGRPC.SerializableProperty.Types.Type.Int32;
                    break;
                case Models.SerializablePropertyType.Int16:
                    rval.NewVal.Data.I16 = model.Int16;
                    rval.NewVal.Type = EditorGRPC.SerializableProperty.Types.Type.Int16;
                    break;
                case Models.SerializablePropertyType.Int8:
                    rval.NewVal.Data.I8 = model.Int8;
                    rval.NewVal.Type = EditorGRPC.SerializableProperty.Types.Type.Int8;
                    break;
                case Models.SerializablePropertyType.Float:
                    rval.NewVal.Data.F = model.Float;
                    rval.NewVal.Type = EditorGRPC.SerializableProperty.Types.Type.Float;
                    break;
                case Models.SerializablePropertyType.Double:
                    rval.NewVal.Data.D = model.Double;
                    rval.NewVal.Type = EditorGRPC.SerializableProperty.Types.Type.Double;
                    break;
                case Models.SerializablePropertyType.Bytes:
                    rval.NewVal.Data.B = Google.Protobuf.ByteString.CopyFromUtf8(model.Bytes);
                    rval.NewVal.Type = EditorGRPC.SerializableProperty.Types.Type.Bytes;
                    break;
                case Models.SerializablePropertyType.Vec2:
                    rval.NewVal.Data.V2 = new EditorGRPC.Vec2();
                    rval.NewVal.Data.V2.X = model.Vec2.X;
                    rval.NewVal.Data.V2.Y = model.Vec2.Y;
                    rval.NewVal.Type = EditorGRPC.SerializableProperty.Types.Type.Vec2;
                    break;
                case Models.SerializablePropertyType.Vec3:
                    rval.NewVal.Data.V3 = new EditorGRPC.Vec3();
                    rval.NewVal.Data.V3.X = model.Vec3.X;
                    rval.NewVal.Data.V3.Y = model.Vec3.Y;
                    rval.NewVal.Data.V3.Z = model.Vec3.Z;
                    rval.NewVal.Type = EditorGRPC.SerializableProperty.Types.Type.Vec3;
                    break;
                case Models.SerializablePropertyType.Vec4:
                    rval.NewVal.Data.V4 = new EditorGRPC.Vec4();
                    rval.NewVal.Data.V4.R = model.Vec4.R;
                    rval.NewVal.Data.V4.G = model.Vec4.G;
                    rval.NewVal.Data.V4.B = model.Vec4.B;
                    rval.NewVal.Data.V4.A = model.Vec4.A;
                    rval.NewVal.Type = EditorGRPC.SerializableProperty.Types.Type.Vec4;
                    break;
                case Models.SerializablePropertyType.SerializableNode:
                case Models.SerializablePropertyType.SerializableNodesArray:
                case Models.SerializablePropertyType.ErrorValue:
                    throw new InvalidOperationException("we don't do that here");
                    break;
            }
            return rval;
        }

        public static EditorGRPC.BoardDescription ToGRPCSetMessage(this Models.GameBoardDescription description)
        {
            var rval = new EditorGRPC.BoardDescription();
            rval.Data = ByteString.CopyFrom(description.PlayfieldArray.ToArray());//new ByteString(description.PlayfieldArray);
            rval.Width = description.Width;
            rval.Height = description.Height;
            rval.Depth = description.Depth;
            return rval;
        }
    }
}
