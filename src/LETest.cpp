// 
// Lua Engine Testing     
//

#include <LEngine.hpp>
#include <string>

int main(int, char**)
{
    // Automatically Creates and Destroys
    LState L;

    std::string Script = R"(
        print("LUA", "Hello!")
    )";

    L_RunScript(L, Script.c_str());

    return 0;
}