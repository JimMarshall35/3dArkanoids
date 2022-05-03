using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Google.Protobuf;

namespace _3dArkanoidsEditor.Services
{
    internal static class GRPCConversionHelper
    {
        public static Models.GameBoardDescription GRPCMessageToModel(EditorGRPC.BoardDescription grpc)
        {
            return new Models.GameBoardDescription(
                    grpc.Data.ToList<byte>(),
                    grpc.Width,
                    grpc.Height,
                    grpc.Depth);
        }

        public static EditorGRPC.BoardDescription ModelToGRPCMessage(Models.GameBoardDescription model)
        {
            var grpc = new EditorGRPC.BoardDescription();
            grpc.Data = ByteString.CopyFrom(model.PlayfieldArray.ToArray());
            grpc.Depth = model.Depth;
            grpc.Width = model.Width;
            grpc.Height = model.Height;
            return grpc;
        }
    }
}
