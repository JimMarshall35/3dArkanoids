// <auto-generated>
//     Generated by the protocol buffer compiler.  DO NOT EDIT!
//     source: editor_proto_files/PlayBoardEdit.proto
// </auto-generated>
#pragma warning disable 0414, 1591
#region Designer generated code

using grpc = global::Grpc.Core;

namespace EditorGRPC {
  public static partial class PlayBoardEdit
  {
    static readonly string __ServiceName = "EditorGRPC.PlayBoardEdit";

    [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
    static void __Helper_SerializeMessage(global::Google.Protobuf.IMessage message, grpc::SerializationContext context)
    {
      #if !GRPC_DISABLE_PROTOBUF_BUFFER_SERIALIZATION
      if (message is global::Google.Protobuf.IBufferMessage)
      {
        context.SetPayloadLength(message.CalculateSize());
        global::Google.Protobuf.MessageExtensions.WriteTo(message, context.GetBufferWriter());
        context.Complete();
        return;
      }
      #endif
      context.Complete(global::Google.Protobuf.MessageExtensions.ToByteArray(message));
    }

    [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
    static class __Helper_MessageCache<T>
    {
      public static readonly bool IsBufferMessage = global::System.Reflection.IntrospectionExtensions.GetTypeInfo(typeof(global::Google.Protobuf.IBufferMessage)).IsAssignableFrom(typeof(T));
    }

    [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
    static T __Helper_DeserializeMessage<T>(grpc::DeserializationContext context, global::Google.Protobuf.MessageParser<T> parser) where T : global::Google.Protobuf.IMessage<T>
    {
      #if !GRPC_DISABLE_PROTOBUF_BUFFER_SERIALIZATION
      if (__Helper_MessageCache<T>.IsBufferMessage)
      {
        return parser.ParseFrom(context.PayloadAsReadOnlySequence());
      }
      #endif
      return parser.ParseFrom(context.PayloadAsNewBuffer());
    }

    [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
    static readonly grpc::Marshaller<global::EditorGRPC.Point> __Marshaller_EditorGRPC_Point = grpc::Marshallers.Create(__Helper_SerializeMessage, context => __Helper_DeserializeMessage(context, global::EditorGRPC.Point.Parser));
    [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
    static readonly grpc::Marshaller<global::EditorGRPC.EditBlockResult> __Marshaller_EditorGRPC_EditBlockResult = grpc::Marshallers.Create(__Helper_SerializeMessage, context => __Helper_DeserializeMessage(context, global::EditorGRPC.EditBlockResult.Parser));
    [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
    static readonly grpc::Marshaller<global::EditorGRPC.Void> __Marshaller_EditorGRPC_Void = grpc::Marshallers.Create(__Helper_SerializeMessage, context => __Helper_DeserializeMessage(context, global::EditorGRPC.Void.Parser));
    [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
    static readonly grpc::Marshaller<global::EditorGRPC.BoardDescription> __Marshaller_EditorGRPC_BoardDescription = grpc::Marshallers.Create(__Helper_SerializeMessage, context => __Helper_DeserializeMessage(context, global::EditorGRPC.BoardDescription.Parser));
    [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
    static readonly grpc::Marshaller<global::EditorGRPC.SetBoardDescriptionResult> __Marshaller_EditorGRPC_SetBoardDescriptionResult = grpc::Marshallers.Create(__Helper_SerializeMessage, context => __Helper_DeserializeMessage(context, global::EditorGRPC.SetBoardDescriptionResult.Parser));

    [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
    static readonly grpc::Method<global::EditorGRPC.Point, global::EditorGRPC.EditBlockResult> __Method_AddBlock = new grpc::Method<global::EditorGRPC.Point, global::EditorGRPC.EditBlockResult>(
        grpc::MethodType.Unary,
        __ServiceName,
        "AddBlock",
        __Marshaller_EditorGRPC_Point,
        __Marshaller_EditorGRPC_EditBlockResult);

    [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
    static readonly grpc::Method<global::EditorGRPC.Point, global::EditorGRPC.EditBlockResult> __Method_RemoveBlock = new grpc::Method<global::EditorGRPC.Point, global::EditorGRPC.EditBlockResult>(
        grpc::MethodType.Unary,
        __ServiceName,
        "RemoveBlock",
        __Marshaller_EditorGRPC_Point,
        __Marshaller_EditorGRPC_EditBlockResult);

    [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
    static readonly grpc::Method<global::EditorGRPC.Void, global::EditorGRPC.BoardDescription> __Method_GetBoardState = new grpc::Method<global::EditorGRPC.Void, global::EditorGRPC.BoardDescription>(
        grpc::MethodType.Unary,
        __ServiceName,
        "GetBoardState",
        __Marshaller_EditorGRPC_Void,
        __Marshaller_EditorGRPC_BoardDescription);

    [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
    static readonly grpc::Method<global::EditorGRPC.BoardDescription, global::EditorGRPC.SetBoardDescriptionResult> __Method_SetBoardState = new grpc::Method<global::EditorGRPC.BoardDescription, global::EditorGRPC.SetBoardDescriptionResult>(
        grpc::MethodType.Unary,
        __ServiceName,
        "SetBoardState",
        __Marshaller_EditorGRPC_BoardDescription,
        __Marshaller_EditorGRPC_SetBoardDescriptionResult);

    /// <summary>Service descriptor</summary>
    public static global::Google.Protobuf.Reflection.ServiceDescriptor Descriptor
    {
      get { return global::EditorGRPC.PlayBoardEditReflection.Descriptor.Services[0]; }
    }

    /// <summary>Base class for server-side implementations of PlayBoardEdit</summary>
    [grpc::BindServiceMethod(typeof(PlayBoardEdit), "BindService")]
    public abstract partial class PlayBoardEditBase
    {
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual global::System.Threading.Tasks.Task<global::EditorGRPC.EditBlockResult> AddBlock(global::EditorGRPC.Point request, grpc::ServerCallContext context)
      {
        throw new grpc::RpcException(new grpc::Status(grpc::StatusCode.Unimplemented, ""));
      }

      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual global::System.Threading.Tasks.Task<global::EditorGRPC.EditBlockResult> RemoveBlock(global::EditorGRPC.Point request, grpc::ServerCallContext context)
      {
        throw new grpc::RpcException(new grpc::Status(grpc::StatusCode.Unimplemented, ""));
      }

      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual global::System.Threading.Tasks.Task<global::EditorGRPC.BoardDescription> GetBoardState(global::EditorGRPC.Void request, grpc::ServerCallContext context)
      {
        throw new grpc::RpcException(new grpc::Status(grpc::StatusCode.Unimplemented, ""));
      }

      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual global::System.Threading.Tasks.Task<global::EditorGRPC.SetBoardDescriptionResult> SetBoardState(global::EditorGRPC.BoardDescription request, grpc::ServerCallContext context)
      {
        throw new grpc::RpcException(new grpc::Status(grpc::StatusCode.Unimplemented, ""));
      }

    }

    /// <summary>Client for PlayBoardEdit</summary>
    public partial class PlayBoardEditClient : grpc::ClientBase<PlayBoardEditClient>
    {
      /// <summary>Creates a new client for PlayBoardEdit</summary>
      /// <param name="channel">The channel to use to make remote calls.</param>
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public PlayBoardEditClient(grpc::ChannelBase channel) : base(channel)
      {
      }
      /// <summary>Creates a new client for PlayBoardEdit that uses a custom <c>CallInvoker</c>.</summary>
      /// <param name="callInvoker">The callInvoker to use to make remote calls.</param>
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public PlayBoardEditClient(grpc::CallInvoker callInvoker) : base(callInvoker)
      {
      }
      /// <summary>Protected parameterless constructor to allow creation of test doubles.</summary>
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      protected PlayBoardEditClient() : base()
      {
      }
      /// <summary>Protected constructor to allow creation of configured clients.</summary>
      /// <param name="configuration">The client configuration.</param>
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      protected PlayBoardEditClient(ClientBaseConfiguration configuration) : base(configuration)
      {
      }

      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual global::EditorGRPC.EditBlockResult AddBlock(global::EditorGRPC.Point request, grpc::Metadata headers = null, global::System.DateTime? deadline = null, global::System.Threading.CancellationToken cancellationToken = default(global::System.Threading.CancellationToken))
      {
        return AddBlock(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual global::EditorGRPC.EditBlockResult AddBlock(global::EditorGRPC.Point request, grpc::CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_AddBlock, null, options, request);
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual grpc::AsyncUnaryCall<global::EditorGRPC.EditBlockResult> AddBlockAsync(global::EditorGRPC.Point request, grpc::Metadata headers = null, global::System.DateTime? deadline = null, global::System.Threading.CancellationToken cancellationToken = default(global::System.Threading.CancellationToken))
      {
        return AddBlockAsync(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual grpc::AsyncUnaryCall<global::EditorGRPC.EditBlockResult> AddBlockAsync(global::EditorGRPC.Point request, grpc::CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_AddBlock, null, options, request);
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual global::EditorGRPC.EditBlockResult RemoveBlock(global::EditorGRPC.Point request, grpc::Metadata headers = null, global::System.DateTime? deadline = null, global::System.Threading.CancellationToken cancellationToken = default(global::System.Threading.CancellationToken))
      {
        return RemoveBlock(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual global::EditorGRPC.EditBlockResult RemoveBlock(global::EditorGRPC.Point request, grpc::CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_RemoveBlock, null, options, request);
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual grpc::AsyncUnaryCall<global::EditorGRPC.EditBlockResult> RemoveBlockAsync(global::EditorGRPC.Point request, grpc::Metadata headers = null, global::System.DateTime? deadline = null, global::System.Threading.CancellationToken cancellationToken = default(global::System.Threading.CancellationToken))
      {
        return RemoveBlockAsync(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual grpc::AsyncUnaryCall<global::EditorGRPC.EditBlockResult> RemoveBlockAsync(global::EditorGRPC.Point request, grpc::CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_RemoveBlock, null, options, request);
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual global::EditorGRPC.BoardDescription GetBoardState(global::EditorGRPC.Void request, grpc::Metadata headers = null, global::System.DateTime? deadline = null, global::System.Threading.CancellationToken cancellationToken = default(global::System.Threading.CancellationToken))
      {
        return GetBoardState(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual global::EditorGRPC.BoardDescription GetBoardState(global::EditorGRPC.Void request, grpc::CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_GetBoardState, null, options, request);
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual grpc::AsyncUnaryCall<global::EditorGRPC.BoardDescription> GetBoardStateAsync(global::EditorGRPC.Void request, grpc::Metadata headers = null, global::System.DateTime? deadline = null, global::System.Threading.CancellationToken cancellationToken = default(global::System.Threading.CancellationToken))
      {
        return GetBoardStateAsync(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual grpc::AsyncUnaryCall<global::EditorGRPC.BoardDescription> GetBoardStateAsync(global::EditorGRPC.Void request, grpc::CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_GetBoardState, null, options, request);
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual global::EditorGRPC.SetBoardDescriptionResult SetBoardState(global::EditorGRPC.BoardDescription request, grpc::Metadata headers = null, global::System.DateTime? deadline = null, global::System.Threading.CancellationToken cancellationToken = default(global::System.Threading.CancellationToken))
      {
        return SetBoardState(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual global::EditorGRPC.SetBoardDescriptionResult SetBoardState(global::EditorGRPC.BoardDescription request, grpc::CallOptions options)
      {
        return CallInvoker.BlockingUnaryCall(__Method_SetBoardState, null, options, request);
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual grpc::AsyncUnaryCall<global::EditorGRPC.SetBoardDescriptionResult> SetBoardStateAsync(global::EditorGRPC.BoardDescription request, grpc::Metadata headers = null, global::System.DateTime? deadline = null, global::System.Threading.CancellationToken cancellationToken = default(global::System.Threading.CancellationToken))
      {
        return SetBoardStateAsync(request, new grpc::CallOptions(headers, deadline, cancellationToken));
      }
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      public virtual grpc::AsyncUnaryCall<global::EditorGRPC.SetBoardDescriptionResult> SetBoardStateAsync(global::EditorGRPC.BoardDescription request, grpc::CallOptions options)
      {
        return CallInvoker.AsyncUnaryCall(__Method_SetBoardState, null, options, request);
      }
      /// <summary>Creates a new instance of client from given <c>ClientBaseConfiguration</c>.</summary>
      [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
      protected override PlayBoardEditClient NewInstance(ClientBaseConfiguration configuration)
      {
        return new PlayBoardEditClient(configuration);
      }
    }

    /// <summary>Creates service definition that can be registered with a server</summary>
    /// <param name="serviceImpl">An object implementing the server-side handling logic.</param>
    [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
    public static grpc::ServerServiceDefinition BindService(PlayBoardEditBase serviceImpl)
    {
      return grpc::ServerServiceDefinition.CreateBuilder()
          .AddMethod(__Method_AddBlock, serviceImpl.AddBlock)
          .AddMethod(__Method_RemoveBlock, serviceImpl.RemoveBlock)
          .AddMethod(__Method_GetBoardState, serviceImpl.GetBoardState)
          .AddMethod(__Method_SetBoardState, serviceImpl.SetBoardState).Build();
    }

    /// <summary>Register service method with a service binder with or without implementation. Useful when customizing the  service binding logic.
    /// Note: this method is part of an experimental API that can change or be removed without any prior notice.</summary>
    /// <param name="serviceBinder">Service methods will be bound by calling <c>AddMethod</c> on this object.</param>
    /// <param name="serviceImpl">An object implementing the server-side handling logic.</param>
    [global::System.CodeDom.Compiler.GeneratedCode("grpc_csharp_plugin", null)]
    public static void BindService(grpc::ServiceBinderBase serviceBinder, PlayBoardEditBase serviceImpl)
    {
      serviceBinder.AddMethod(__Method_AddBlock, serviceImpl == null ? null : new grpc::UnaryServerMethod<global::EditorGRPC.Point, global::EditorGRPC.EditBlockResult>(serviceImpl.AddBlock));
      serviceBinder.AddMethod(__Method_RemoveBlock, serviceImpl == null ? null : new grpc::UnaryServerMethod<global::EditorGRPC.Point, global::EditorGRPC.EditBlockResult>(serviceImpl.RemoveBlock));
      serviceBinder.AddMethod(__Method_GetBoardState, serviceImpl == null ? null : new grpc::UnaryServerMethod<global::EditorGRPC.Void, global::EditorGRPC.BoardDescription>(serviceImpl.GetBoardState));
      serviceBinder.AddMethod(__Method_SetBoardState, serviceImpl == null ? null : new grpc::UnaryServerMethod<global::EditorGRPC.BoardDescription, global::EditorGRPC.SetBoardDescriptionResult>(serviceImpl.SetBoardState));
    }

  }
}
#endregion
