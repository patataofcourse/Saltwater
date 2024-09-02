#ifndef RHMTYPES_H
#define RHMTYPES_H

#include <array>

//#include "types.h"

namespace Megamix {
    struct Tempo {
        float beats;
        u32 time;
        u16 flag8;
        u16 flagA;
    };

    struct TempoTable {
        u32 id1;
        u32 id2;
        u8 unk8;
        u16 unkA;
        Tempo* pos;
    };

    struct SM_TempoTable {
        u32 id;
        TempoTable* tempo;
    };

    struct CSoundManager {
        u8 pad0[0x8C];
        TempoTable* tempoTable;
        SM_TempoTable* tableStrm;
        SM_TempoTable* tableSeq;
        u16 numberTempos;
    };

    enum MuseumGameIndex : u16 {
        // Used in unused columns
        None = 0x101,

        // Tengoku - prequels
        AgbClapS = 0,
        AgbGhostS = 1,
        AgbHairS = 2,

        // DS - prequels
        NtrChorusS = 3,
        NtrPingPongS = 4,
        NtrRobotS = 5,
        NtrShootingS = 6,

        // Fever - prequels
        RvlBadmintonS = 7,
        RvlFleaS = 8,
        RvlGomaS = 9,
        RvlMuscleS = 0xa,

        // Megamix - prequels
        CtrFruitbasketS = 0xb,
        CtrInterpreterS = 0xc,
        CtrStepS = 0xd,
        CtrWoodCatS = 0xe,

        // Tengoku - standard games
        AgbBatterL = 0xf,
        AgbClapL = 0x10,
        AgbGhostL = 0x11,
        AgbHairL = 0x12,
        AgbHoppingL = 0x13,
        AgbMarcherL = 0x14,
        AgbNightWalkL = 0x15,
        AgbQuizL = 0x16,
        AgbRabbitL = 0x17,
        AgbRatL = 0x18,
        AgbShujiL = 0x19,
        AgbSpaceDanceL = 0x1a,
        AgbTapL = 0x1b,
        AgbTonoL = 0x1c,

        // DS - standard games
        NtrAirBoardL = 0x1d,
        NtrBackbeatL = 0x1e,
        NtrBlueBirdsL = 0x1f,
        NtrBoxShowL = 0x20,
        NtrCameraManL = 0x21,
        NtrChorusL = 0x22,
        NtrFrogL = 0x23,
        NtrIdolL = 0x24,
        NtrNinjaL = 0x25,
        NtrPingPongL = 0x26,
        NtrRobotL = 0x27,
        NtrShootingL = 0x28,
        NtrShortLiveL = 0x29,
        NtrShugyoL = 0x2a,

        // Fever - standard games
        RvlAssembleL = 0x2b,
        RvlBadmintonL = 0x2c,
        RvlBattingL = 0x2d,
        RvlBirdL = 0x2e,
        RvlBookL = 0x2f,
        RvlDateL = 0x30,
        RvlFishingL = 0x31,
        RvlFleaL = 0x32,
        RvlForkL = 0x33,
        RvlGolfL = 0x34,
        RvlGomaL = 0x35,
        RvlInterviewL = 0x36,
        RvlManjuL = 0x37,
        RvlMuscleL = 0x38,
        RvlRapL = 0x39,
        RvlRecieveL = 0x3a,
        RvlRobotL = 0x3b,
        RvlRocketL = 0x3c,
        RvlRotationL = 0x3d,
        RvlSamuraiL = 0x3e,
        RvlSeesawL = 0x3f,
        RvlSortL = 0x40,
        RvlWatchL = 0x41,

        // Megamix - standard games
        CtrBearL = 0x42,
        CtrBlancoL = 0x43,
        CtrChameleonL = 0x44,
        CtrDotSamuraiL = 0x45,
        CtrFruitbasketL = 0x46,
        CtrInterpreterL = 0x47,
        CtrPillowL = 0x48,
        CtrStepL = 0x49,
        CtrSumouL = 0x4a,
        CtrTangoL = 0x4b,
        CtrTeppanL = 0x4c,
        CtrWoodCatL = 0x4d,

        // Tengoku - sequels
        AgbClapA = 0x4e,
        AgbSpaceDanceA = 0x4f,
        AgbTapA = 0x50,

        // DS - sequels
        NtrFrogA = 0x51,
        NtrIdolA = 0x52,
        NtrPingPongA = 0x53,

        // Fever - sequels
        RvlGolfA = 0x54,
        RvlManjuA = 0x55,
        RvlMuscleA = 0x56,

        // Megamix - sequels
        CtrBlancoA = 0x57,
        CtrDotSamuraiA = 0x58,

        // Karate Man
        RvlKarate0 = 0x59,
        RvlKarate1 = 0x5a,
        RvlKarate2 = 0x5b,
        RvlKarate3 = 0x5c,
        RvlKarate4 = 0x5d,

        // Remixes
        RemixLED = 0x5e,
        RemixTED = 0x5f,
        Remix00 = 0x60,
        Remix01 = 0x61,
        Remix02 = 0x62,
        Remix03 = 0x63,
        Remix04 = 0x64,
        Remix05 = 0x65,
        Remix06 = 0x66,
        Remix07 = 0x67,

        // Endless games
        AgbVirus = 0x68,
        NtrCoinToss = 0x69,
        RvlSword = 0x6a,
        CtrChicken = 0x6b,
    };

    struct MuseumRow {
        u32 columnCount;
        std::array<u16, 5> gameIndices;
        u8 pad[2];
        const char* titleId;
        u32 highIndex;
        u32 lowIndex;

        MuseumRow(std::array<u16, 5> gameIndices, const char* titleId, u32 highIndex, u32 lowIndex) {
            if      (gameIndices[0] == 0x101) this->columnCount = 0;
            else if (gameIndices[1] == 0x101) this->columnCount = 1;
            else if (gameIndices[2] == 0x101) this->columnCount = 2;
            else if (gameIndices[3] == 0x101) this->columnCount = 3;
            else if (gameIndices[4] == 0x101) this->columnCount = 4;
            else                              this->columnCount = 5;

            this->gameIndices = gameIndices;
            this->titleId = titleId;
            this->highIndex = highIndex;
            this->lowIndex = lowIndex;
        }
    };

    struct Color8 {
        u8 r;
        u8 g;
        u8 b;
        u8 a;

        Color8() {
            this->r = 0;
            this->g = 0;
            this->b = 0;
            this->a = 0;
        }

        Color8(u8 r, u8 g, u8 b, u8 a) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        Color8(u32 color) {
            this->r = color >> 24;
            this->g = color >> 16;
            this->b = color >> 8;
            this->a = color;
        }
    };

    struct MuseumRowColor {
        Color8 unk1;
        Color8 background;
        Color8 edgeFade;
        Color8 unk3;

        MuseumRowColor(Color8 background, Color8 edgeFade) {
            this->unk1 = {0x00, 0x00, 0x00, 0x00};
            this->unk3 = {0xFF, 0xFF, 0xFF, 0x6E};

            this->background = background;
            this->edgeFade = edgeFade;
        }
    };

    struct CList {
        int* _vtable;
        struct CList* next;
        struct CList* prev;
    };

    struct TickFlowStack {
        u32* tick;
        u32 tickSize;
    };

    struct CTickflow {
        CList at0;
        u32* tickflow;
        u32 category;
        int tickFlowSize;
        float rest;
        u8 unk1;
        u32 condvar;
        u32 condvarStack[10];
        u8 pad1[0x17];
        u8 condvarStackCounter;
        struct TickFlowStack tickFlowStack[8];
        u8 pad2[0x3F];
        u8 tickFlowStackCounter;
        u8 buttonPromptIsReleased;
        u8 buttonPromptIsPressed;
        u32 buttonPromptButton;
        int buttonPromptPressSfx;
        int buttonPromptReleaseSfx;
        u8 pad3[7];
        u32 countdownValue;
        u32 countdownCounter;
    };

    //-----------------
    //Save File
    //-----------------

    enum class EGameRank : u8 {
        Locked = 0,
        Unplayed = 1,
        Unfinished = 2,
        Not_Good = 3,
        Ok = 4,
        High = 5,
        Perfect = 6
    };

    enum class EPlayStyle : u8 { /* simple tap / button */
        Button = 0,
        Tap = 1
    };

    struct CSaveFileData {
        u8 pad1[0x10];
        u32 playTime;
        u8 playerData;
        u8 pad2[0x5F];
        enum EGameRank gameRanks[104];
        u8 pad3[0xF18];
        u8 field3957_0xff4[1][16];
        u8 pad4[0xBF];
        u8 locale;
        u8 pad5[0x91];
        bool unlockedGoat;
        u8 pad6[0x5];
        u8 isFirstMuseumTalk;
        u8 pad7[0x2];
        bool hasBeenInCafe;
        u8 pad8[0x1D];
        u16 clPlay2;
        u16 cLPlay3;
        u8 pad9[0x4];
        u16 money;
        u16 coin;
        enum EPlayStyle playStyle;
        u8 timingEft;
        u8 isCec;
        u8 pad10[0x131];
        u8 field4646_0x12bc[104];
        u8 pad11[0xAA];
        u8 onion[3];
        u8 pad12[0x7];
        u32 goatLv;
        u8 pad13[0x26c];
    };

    struct CSaveData {
        int* _vtable;
        u32 unk1;
        u8 unk2;
        u8 unk3;
        u32 unk4;
        u32 unk5[4];
        u8 cSaveData_sub1[0x1c20];
        struct CSaveFileData fileData[4];
        u32 currentFile;
    };

    //-----------------
    //Input Manager
    //-----------------

    enum class EButtonFlag {
        None=0,
        A=1 << 0,
        B=1 << 1,
        Select=1 << 2,
        Start=1 << 3,
        DpadRight=1 << 4,
        DpadLeft=1 << 5,
        DpadUp=1 << 6,
        DpadDown=1 << 7,
        R=1 << 8,
        L=1 << 9,
        X=1 << 10,
        Y=1 << 11
    };

    enum class ClampMode {
        Circle=0,
        Cross=1,
        Minimum=2
    };

    struct AnalogStickClamper {
        s16 minOfStickClampCircle;
        s16 minOfStickClampCross;
        s16 minOfStickClampMinimum;
        s16 maxOfStickClampCircle;
        s16 maxOfStickClampCross;
        s16 maxOfStickClampMinimum;
        enum ClampMode stickClampMode;
        u8 padding;
        s16 threshold;
        float scale;
        float stroke;
        float strokeVelocity;
        float lastLength;
        float lastDiff;
    };

    struct PadReader {
        struct Pad* pad;
        s32 indexOfRead;
        u32 latestHold;
        struct AnalogStickClamper stickClamper;
        bool isReadLatestFirst;
        u32 padding3;
        u32 padding4;
        s64 tickOfRead;
    };

    struct CInputPadHandler {
        u8* _vtable;
        enum EButtonFlag holdButtons;
        enum EButtonFlag triggerButtons;
        enum EButtonFlag releaseButtons;
        enum EButtonFlag previousTriggerButtons;
        u32 field5_0x14;
        s8 field6_0x18[12];
        s8 field7_0x24[12];
        u8 CInputPadHandler_sub_x30_padding[4];
        s32 field9_0x34;
        float circlePadX;
        u32 circlePadY;
        float field12_0x40;
        float field13_0x44;
        float field14_0x48;
        float field15_0x4c;
        float field16_0x50;
        struct PadReader* padReader;
        u8 field18_0x58;
        u8 field19_0x59;
        u8 field20_0x5a;
        u8 field21_0x5b;
    };

    struct TouchPanelReader {
        struct TouchPanel* touchPanel;
        s32 indexOfRead;
        s64 tickOfRead;
    };

    struct TouchPanelStatus {
        u16 x;
        u16 y;
        u8 touch; /* 0 - pen up; 1 - touch */
        u32 padding;
    };

    struct CInputTouchPanelHandler {
        u8* _vtable;
        struct TouchPanelReader* touchPanelReader;
        struct TouchPanelStatus touchPanelStatus;
        struct TouchPanelStatus previousTouchPanelStatus;
        float field4_0x18;
        float field5_0x1c;
        float field6_0x20;
        float field7_0x24;
        float field8_0x28;
        float field9_0x2c;
        float field10_0x30;
        float field11_0x34;
        float field12_0x38;
        float field13_0x3c;
        float field14_0x40;
        float field15_0x44;
        float field16_0x48;
        float field17_0x4c;
        float field18_0x50;
        float field19_0x54;
        float field20_0x58;
        float field21_0x5c;
        float field22_0x60;
        s32 field23_0x64;
        bool currentTouched;
        bool previousTouched;
        u8 field26_0x6a;
        u8 field27_0x6b;
    };

    struct CInputManager {
        int* _vtable;
        struct CInputPadHandler* padHandler;
        struct CInputTouchPanelHandler* touchPanelHandler;
    };


    //-----------------
    //File Manager
    //-----------------

    struct CFileManager {
        int* _vtable;
        s32 field1_0x4;
        s32 field2_0x8;
        void* romWorkingMemory;
        u32 romWorkingMemorySize;
        u8* gzipWorkMemory;
        struct FileInfo* fileInfo;
        u32* fileIds;
        u32 fileIdCount;
        u32 field9_0x24;
        bool field10_0x28;
        u8 field11_0x29;
        u8 field12_0x2a;
        u8 field13_0x2b;
        u32 field14_0x2c;
        u16 locale[9];
        u16 sublocale[9];
    };

    struct FileInfo {
        u16 filePath[128];
        void* fileBuffer;
        u8* compFileBuffer;
        void* field3_0x108;
        size_t fileSize;
        size_t compFileSize;
        size_t field6_0x114;
        u8 mode;
        u8 field8_0x119;
        u8 field9_0x11a;
        u8 field10_0x11b;
        u32 alignment;
        u8 status;
        u8 field13_0x121;
        u8 field14_0x122;
        u8 field15_0x123;
        s32 id;
    };

} // namespace Megamix


    struct FileInfo {
        char16_t filePath[0x80];
        void* fileBuffer;
        u8* compressedFileBuffer;
        void* unk108;
        u32 fileSize;
        u32 compressedFileSize;
        u32 unk114;
        u8 mode;
        u32 alignment;
        u8 status;
        s32 fileId;
    };

    struct FileInputStream {
        void* vtable;
        struct FileBase {
            void* ptr;
            s64 position;
            s64 size;
        } base;
    };

    struct CFileManager {
        void* vtable;
        s32 unk4;
        s32 unk8;
        void* romWorkingMemory;
        u32 romWorkingMemorySize;
        u8* gzipWorkingMemory;
        FileInfo* fileInfo;
        u32 fileIds;
        u32 fileIdCount;
        u32 unk24;
        bool unk28;
        u32 unk2C;
        char16_t locale[9];
        char16_t sublocale[9];
        struct { u32 unk[2]; } thread; //TODO: s64?
        bool threadCreated;
    };

    struct CachedFileInfo {
        char16_t filePath[64];
        void* memory;
        u32 size;
        u8 mode;
        s32 alignment;
    };

    struct CCachedFile {
        void* vtable;
        void* heapStart;
        u8 expHeap[0x58]; // TODO
        u8 expHeapMutex[0xc]; //TODO
        u32 unk;
        CachedFileInfo* info;
    };

    struct CCachedFileManager {
        void* vtable;
        CCachedFile* files;
        s32 fileCount;
        bool enabled;
    };
}

#endif
