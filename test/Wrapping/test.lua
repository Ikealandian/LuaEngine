function HelloLua(fromC)
    if not fromC then
        print("Hello, from Lua!")
    else 
        print("Hello, from C++!")
    end
end