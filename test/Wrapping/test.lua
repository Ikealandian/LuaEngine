function Square(Number)
    return Number * Number
end

function LuaPrint(Text)
    print("LUA", Text);
end

function HelloLua(fromC)
    if not fromC then
        LuaPrint("Hello, from Lua!")
    else
        LuaPrint("Hello, from C++!")
    end
end

local Success = FileW("Text.txt", "New File Contents")
if Success then
    LuaPrint("Successfully wrote to 'Text.txt'!");
    local RSuccess, Contents = FileR("Text.txt");
    if RSuccess then
        LuaPrint("Successfully read 'Text.txt'!");
        print(Contents);
    end
else
    LuaPrint("Failed to open 'Text.txt'!");
end

HelloLua(false)