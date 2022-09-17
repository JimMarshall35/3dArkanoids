#include "ISerializable.h"
#include <iostream>
#include <fstream>
#include <functional>


void DebugPrintSerializablePropertiesNode(const ISerializablePropertiesNode* node, int numTabs = 0) {
	for (int i = 0; i < numTabs; i++) {
		std::cout << "\t";
	}
	std::cout << "serializable thing " << node->GetSerializableNodeName() << "\n";
	for (const auto& prop : node->GetSerializableProperties()) {
		for (int i = 0; i < numTabs + 1; i++) {
			std::cout << "\t";
		}
		std::cout << prop.name << ": ";
		switch (prop.type) {
		case SerializablePropertyType::Double:
			std::cout << prop.data.dataUnion.Double << "\n";
			break;
		case SerializablePropertyType::Float:
			std::cout << prop.data.dataUnion.Float << "\n";
			break;
		case SerializablePropertyType::Int16:
			std::cout << prop.data.dataUnion.Int16 << "\n";
			break;
		case SerializablePropertyType::Int32:
			std::cout << prop.data.dataUnion.Int32 << "\n";
			break;
		case SerializablePropertyType::Int8:
			std::cout << prop.data.dataUnion.Int8 << "\n";
			break;
		case SerializablePropertyType::Uint16:
			std::cout << prop.data.dataUnion.Uint16 << "\n";
			break;
		case SerializablePropertyType::Uint32:
			std::cout << prop.data.dataUnion.Uint32 << "\n";
			break;
		case SerializablePropertyType::Uint8:
			std::cout << prop.data.dataUnion.Uint8 << "\n";
			break;
		case SerializablePropertyType::Vec2:
			std::cout << "{ x: " << prop.data.dataUnion.Vec2.x << " y: " << prop.data.dataUnion.Vec2.y << " }\n";
			break;
		case SerializablePropertyType::Vec3:
			std::cout << "{ x: " << prop.data.dataUnion.Vec3.x << " y: " << prop.data.dataUnion.Vec3.y << " z: " << prop.data.dataUnion.Vec3.z << " }\n";
			break;
		case SerializablePropertyType::Vec4:
			std::cout << "{ x: " << prop.data.dataUnion.Vec4.x << " y: " << prop.data.dataUnion.Vec4.y << " z: " << prop.data.dataUnion.Vec4.z << " a: " << prop.data.dataUnion.Vec4.a << " }\n";
			break;
		case SerializablePropertyType::Bytes:
			std::cout << "[bytes]\n";
			break;
		case SerializablePropertyType::SerializableNode:
			std::cout << "\n";
			DebugPrintSerializablePropertiesNode(prop.data.dataUnion.childNodes[0], numTabs + 1);
			break;
		case SerializablePropertyType::SerializableNodesArray:
			//std::cout << "[SerializableNodesArray]\n";
			std::cout << "\n";
			auto numProperties = prop.data.SizeIfApplicable;
			for (int i = 0; i < numProperties; i++) {
				const auto child = prop.data.dataUnion.childNodes[i];
				DebugPrintSerializablePropertiesNode(child,numTabs + 1);
			}
			break;
		}
	}
}

void DebugPrintAllSerializableThings()
{
	const auto& list = AutoList<ISerializable>::GetList();
	for (const auto item : list) {
		//std::cout << "serializable thing " << item->GetSerializableNodeName() << "\n";
		DebugPrintSerializablePropertiesNode(item);
	}
}

size_t getBigFileRequiredSize() {
	size_t size = 0;
	const auto& list = AutoList<ISerializable>::GetList();
	for (const auto item : list) {
		//std::cout << "serializable thing " << item->GetSerializableNodeName() << "\n";
		size += item->GetBinaryFileNumBytes();
	}
	return size;
}

void SaveSerializableToSingleBigBinary(std::string filePath)
{
	auto bufferSize = getBigFileRequiredSize();

	auto stagingBuffer = std::make_unique<char[]>(bufferSize);

	auto writePtr = stagingBuffer.get();

	const auto& list = AutoList<ISerializable>::GetList();
	for (const auto item : list) {
		writePtr = item->SaveToBuffer(writePtr);
	}

	std::ofstream file(filePath, std::ios::out | std::ios::binary);
	file.write(stagingBuffer.get(), bufferSize);


}


void LoadSerializableFromSingleBigBinary(std::string filePath)
{

	// open file
	std::ifstream is(filePath, std::ios::in | std::ios::binary);

	// get length
	is.seekg(0, is.end);
	int fileLength = is.tellg();
	is.seekg(0, is.beg);

	// allocate buffer for the entire files contents and write files contents into it
	auto inputBuf = std::make_unique<char[]>(fileLength);
	is.read(inputBuf.get(), fileLength);

	const char* readPtr = inputBuf.get();
	const auto& list = AutoList<ISerializable>::GetList();
	for (auto item : list) {
		readPtr = item->LoadFromBuffer(readPtr);
	}


}

enum ParserState {
	ReadingNameOfPropertiesNode,
	ReadingNameOfProperty,
	ReadingFirstCharacterOfProperty,
	ReadingFirstCharacterOfPropertiesNode,

	ReadingNumber,
	EndOfString,
	Error
};

ISerializablePropertiesNode* TryFindNodeInRoot(std::string name) {
	const auto& list = AutoList<ISerializable>::GetList();
	for (const auto item : list) {
		if (item->GetSerializableNodeName() == name) {
			return item;
		}
	}
	return nullptr;
}

ISerializablePropertiesNode* TryFindNodeInArrayOfChildren(ISerializablePropertiesNode** children, int numChildNodesBeingSearched, std::string name) {
	for (int i = 0; i < numChildNodesBeingSearched; i++) {
		const auto item = children[i];
		if (item->GetSerializableNodeName() == name) {
			return item;
		}
	}
	return nullptr;
}


ISerializablePropertiesNode** FindSerializableChildren(ISerializablePropertiesNode* node, std::string name, int& outNumNodes) {
	for (const auto& node : node->GetSerializableProperties()) {
		if (node.name == name) {
			outNumNodes = node.data.SizeIfApplicable;
			return node.data.dataUnion.childNodes;
		}
	}
	return nullptr;

}


ISerializablePropertiesNode* ParsePath(std::string path) {
	/*                                      s  s
	                                         s s
	                                          s s
	                                           s s
	                           __________________s
	                          (_____(____________@

	a hand rolled parser - not one of these ^
	
	*/
	auto state = ReadingFirstCharacterOfPropertiesNode;
	auto onChar = 0;
	char character = path[onChar];
	bool parseShouldContinue = true;
	const auto pathLength = path.length();

	const auto& list = AutoList<ISerializable>::GetList();
	std::string errorString = "";

	ISerializablePropertiesNode* returnVal = nullptr;

	ISerializablePropertiesNode** childNodesBeingSearched = nullptr;
	int numChildNodesBeingSearched;


	if (pathLength == 0) {
		std::cout << "passed a string with no length\n";
		return nullptr;
	}
	std::string pathPart = "";
	using CharReaderFunction = std::function<ParserState(void)>;

	CharReaderFunction ReadInCharOfPropertiesNode = 
		[&errorString, &numChildNodesBeingSearched, path,pathLength,&onChar,&pathPart,&character, &childNodesBeingSearched, &returnVal]() {
		pathPart += character;
		if (onChar + 1 >= pathLength) {
			// a property wasn't specified - the path ended on a node
			errorString = "No property was specified. Path was " + path+"\n";
			return Error;
		}
		character = path[++onChar];

		if (character == '.') {
			onChar++; // skip the full stop

			ISerializablePropertiesNode* node;
			if (childNodesBeingSearched == nullptr) {
				node = TryFindNodeInRoot(pathPart);
			}
			else {
				node = TryFindNodeInArrayOfChildren(childNodesBeingSearched, numChildNodesBeingSearched, pathPart);
			}
			if (node == nullptr) {
				errorString = "Node "+pathPart+" not found. Path was " + path + "\n";
				return Error;
			}
			returnVal = node;
			pathPart = "";
			return ReadingFirstCharacterOfProperty;
		}
		
		else {
			return ReadingNameOfPropertiesNode;
		}
			
		errorString = "Unknown error. Path was " + path + "\n";
		return Error;
	};

	CharReaderFunction ReadInCharOfProperty = 
		[&numChildNodesBeingSearched, &errorString, path, pathLength, &onChar, &pathPart, &character, &childNodesBeingSearched, &returnVal]() {
		pathPart += character;
		if (onChar + 1 >= pathLength) {
			// do something with the word, check it exists
			return EndOfString;
		}
		character = path[++onChar];

		if (character == '[') {
			onChar++; // skip the square bracket
			childNodesBeingSearched = FindSerializableChildren(returnVal, pathPart, numChildNodesBeingSearched);
			pathPart = "";
			return ReadingNumber;
		}
		else {
			return ReadingNameOfProperty;
		}
		errorString = "Unknown error. Path was " + path + "\n";

		return Error;
	};

	CharReaderFunction ReadInCharOfNumber = 
		[&errorString, &numChildNodesBeingSearched, path, pathLength, &onChar, &pathPart, &character, &childNodesBeingSearched, &returnVal]() {
		pathPart += character;
		if (onChar + 1 >= pathLength) {
			// do something with the word, check it exists
			errorString = "The path ended unexpectedly part way through a number. path given was: " + path + "\n";

			return Error;
		}
		character = path[++onChar];

		if (path[onChar] == ']') {
			onChar++;
			if (path[onChar] != '.') {
				errorString = "A closing square bracket must be followed by a full stop (.). path given was: " + path + "\n";
				return Error;
			}
			onChar++;
			
			int numberParsed = std::stoi(pathPart);
			if (numberParsed >= numChildNodesBeingSearched || numberParsed < 0) {
				errorString = "Specified index that was out of range: " + std::to_string(numberParsed) + " path given was: " + path + "\n";
				return Error;
			}
			returnVal = childNodesBeingSearched[numberParsed];
			pathPart = "";
			return ReadingNameOfProperty;
		}
		else {
			return ReadingNumber;
		}

		errorString = "Unknown error. Path was " + path + "\n";

		return Error;
	};


	do {
		character = path[onChar];
		switch (state) {
		case ReadingFirstCharacterOfProperty:
			if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z') || character == '_') { // first char can't be a number
				state = ReadInCharOfProperty();
			}
			else {
				errorString = "invalid character " + std::to_string(character) + " in part " + pathPart + " full path was " + path + "\n";
				state = Error;
			}
			break;
		case ReadingNameOfProperty:
			if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z') || (character >= '0' && character <= '9') || character == '_') {
				state = ReadInCharOfProperty();
			}
			else {
				errorString = "invalid character " + std::to_string(character) + " in part " + pathPart + " full path was " + path + "\n";

				state = Error;
			}
			break;

		case ReadingFirstCharacterOfPropertiesNode:
			if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z') || character == '_') { // first char can't be a number
				state = ReadInCharOfPropertiesNode();
			}
			else {
				errorString = "invalid character " + std::to_string(character) + " in part " + pathPart + " full path was " + path + "\n";

				state = Error;
			}
			break;
		case ReadingNameOfPropertiesNode:
			if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z') || (character >= '0' && character <= '9') || character == '_') {
				state = ReadInCharOfPropertiesNode();
			}
			else {
				errorString = "invalid character " + std::to_string(character) + " in part " + pathPart + " full path was " + path + "\n";

				state = Error;
			}
			break;
		case ReadingNumber:
			if (character >= '0' && character <= '9') {
				state = ReadInCharOfNumber();
			}
			else {
				errorString = "invalid character " + std::to_string(character) + " in part " + pathPart + " full path was " + path + "\n";

				state = Error;
			}
			break;
		case EndOfString:
			parseShouldContinue = false;
			break;
		case Error:
			std::cerr << "[Serializable Path Parser Error] " << errorString;
			parseShouldContinue = false;
			break;
		}
	} while (parseShouldContinue);

	return returnVal;
}

void SetSerializableProperty(std::string path, const SerializableProperty& value)
{
	using namespace std;
	auto node = ParsePath(path);
	node->SetSerializableProperty(value);

}
