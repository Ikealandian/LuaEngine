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

HelloLua(false)