#include <3ds.h>
#include <CTRPluginFramework.hpp>

using CTRPluginFramework::Hook; using CTRPluginFramework::HookResult;

//todo: internationalize offsets
namespace Megamix::Hooks {
    Hook* testHook = new Hook();
    int testInt = 0;
    HookResult result;

    int* getTickflowOffset(int gameId) {
        testInt = gameId;
        return *(int**)(0x52b490 + gameId * 0x34 + 4); // original code
    }
    
    void Test() {
        testHook->Initialize(0x258df4, (u32)getTickflowOffset);
        testHook->SetReturnAddress(0x258e08);
        result = testHook->Enable();
    }
}