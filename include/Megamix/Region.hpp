#ifndef RHMREGION_H
#define RHMREGION_H

#include <CTRPluginFramework/Menu/MessageBox.hpp>
#include <cstdlib>
#include <expected>
#include <vector>
#include <string>

#include "types.h"

#include "Megamix/Error.hpp"
#include "Megamix/Types.hpp"

extern u8 region;

namespace Megamix {
    struct GameInterface {
        u32 gameCode;
        u32 revision; // for potential future use?
        const char* regionName;

        // general code regions

        u32 textEnd;
        u32 rodataEnd;
        u32 dataEnd;
        u32 bssEnd;

        // game definitions

        GameDef* gameTable;
        GateGameDef* gateTable;

        // tickflow loading

        u32 tickflowHookPos;
        u32 gateHookPos;
        u32 gatePracHookPos;

        // music tempo

        TempoTable* tempoTable;
    };

    // Rhythm Tengoku: The Best + (Japan) (0004000000155a00) (rev0)
    extern const GameInterface jpCode;
    // Rhythm Heaven Megamix (Americas) (000400000018a400) (rev0)
    extern const GameInterface usCode;
    // Rhythm Paradise Megamix (Europe) (000400000018a500) (rev0)
    extern const GameInterface euCode;
    // Rhythm Sesang: The Best + (Korea) (000400000018a600) (rev0)
    extern const GameInterface krCode;

    extern const GameInterface* pointers;

    std::expected<Void, u32> initGameInterface(u32 gameCode);

    namespace Game {
        inline u32 _textEnd () { return pointers->textEnd; }
        inline u32 _rodataEnd() { return pointers->rodataEnd; }
        inline u32 _dataEnd() { return pointers->dataEnd; }
        inline u32 _bssEnd() { return pointers->dataEnd; }

        inline const char* _regionName() { return pointers->regionName; }

        inline GameDef* gGameTable() { return pointers->gameTable; }
        inline GateGameDef* gGateTable() { return pointers->gateTable; }
        inline TempoTable* gTempoTable() { return pointers->tempoTable; }

        namespace Hooks {
            inline u32 tickflow() { return pointers->tickflowHookPos; }
            inline u32 gate() { return pointers->gateHookPos; }
            inline u32 gatePractice() { return pointers->gatePracHookPos; }
        }
    }
}


namespace Region {

    enum {
        JP = 0,
        US = 1,
        EU = 2,
        KR = 3,
        KR_CTR = 5, // in the SDK, KR is 5 instead of 3
        UNK
    };

    u8 FromCode(u32 code);
    std::string Name();

    std::vector<u32> MuseumRowsInfoAddresses();
    std::vector<u32> MuseumRowsColorsAddresses();
    u32              MuseumRowsColorsInitFunc();
    // TODO: maybe make this into MuseumRowsCmps that returns a
    // map of int -> vector<u8> other regions might use different registers
    std::vector<u32> MuseumRowsR1Cmps();
    std::vector<u32> MuseumRowsR8Cmps();

    u32 StrmTempoHookFunc();
    u32 SeqTempoHookFunc();
    u32 AllTempoHookFunc();

    u32 TickflowCommandsSwitch();
    u32 TickflowCommandsEnd();
    u32 TickflowAsyncSubLocation();

    u32 GlobalSaveDataPointer();
    u32 GlobalInputManagerPointer();
    u32 GlobalFileManagerPointer();

    std::vector<u32> RetryRemixLocs();

    u32 RegionFSHookFunc();
    u32 RegionOtherHookFunc();

    Megamix::CBlackBarManager** BlackbarLayout();

    typedef int (*SWPrintfSignature) (char16_t* buffer, size_t size, const char16_t* format, ...);
    typedef u32 (*SetTextBoxStringSignature) (Megamix::TextBox *, const char16_t *, u32);

    SWPrintfSignature SWPrintfFunc();
    SetTextBoxStringSignature SetTextBoxStringFunc();
}

#endif