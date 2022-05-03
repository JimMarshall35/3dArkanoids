using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Google.Protobuf;

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
    }
}
