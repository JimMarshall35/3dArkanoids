grpc\Debug\bin\protoc editor_proto_files\PlayBoardEdit.proto --grpc_out=3dArkanoids --plugin=protoc-gen-grpc=grpc\Debug\bin\grpc_cpp_plugin.exe
grpc\Debug\bin\protoc editor_proto_files\PlayBoardEdit.proto --cpp_out=3dArkanoids

grpc\Debug\bin\protoc editor_proto_files\PlayBoardEdit.proto --grpc_out=3dArkanoidsEditor --plugin=protoc-gen-grpc=grpc\Debug\bin\grpc_csharp_plugin.exe
grpc\Debug\bin\protoc editor_proto_files\PlayBoardEdit.proto --csharp_out=3dArkanoidsEditor
pause
