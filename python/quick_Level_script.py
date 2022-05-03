
def CheckInputIsInt(inputPrompt):
    valid = False
    parsed = 0
    while not valid:
        try:
            rawInput = input(inputPrompt)
            parsed = int(rawInput)
            valid = True
        except ValueError:
            valid = False
    return parsed


name = input("level name:")
xrows = CheckInputIsInt("how many x?")
yrows = CheckInputIsInt("how many y?")
zrows = CheckInputIsInt("how many z?")
outputString = "const size_t "+name+"XRows = "+str(xrows)+";\n"
outputString += "const size_t "+name+"YRows = "+str(yrows)+";\n"
outputString += "const size_t "+name+"ZRows = "+str(zrows)+";\n"

outputString += "const unsigned char "+name+"["+str(yrows*zrows)+"]["+str(xrows)+"] = {\n"
for z in range(zrows):
    
    for y in range(yrows):
        outputString += "\t{"
        for x in range(xrows):
            outputString += "0x00,"

        outputString += "},\n"
    outputString += "\n\n"
outputString += "};"
print(outputString)
