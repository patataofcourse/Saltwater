#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "external/rt.h"

#include "Megamix.hpp"
#include "Config.hpp"

using CTRPluginFramework::OSD;

using Megamix::TempoTable;

std::vector<float> debugScoreArray;

// c++ is stupid
namespace GHooks = Megamix::Game::Hooks;

namespace Megamix::Hooks {
    RT_HOOK tickflowHook;
    RT_HOOK gateHook;
    RT_HOOK gatePracHook;

    RT_HOOK tempoStrmHook;
    RT_HOOK tempoSeqHook;
    RT_HOOK tempoAllHook;

    RT_HOOK regionFSHook;
    RT_HOOK regionOtherHook;

    RT_HOOK tickflowCommandsHook;

    RT_HOOK scoringFunctionHook;

    void* getTickflowOffset(int index) {
        if (config->tickflows.contains(index)) {
            int result = Megamix::btks.LoadFile(config->tickflows[index]);
            if (!result) {
                return (void*)(Megamix::btks.start);
            } else {
                OSD::Notify(CTRPluginFramework::Utils::Format("Error: %s", Megamix::ErrorMessage(result).c_str()));
            }
        }
        return Game::gGameTable()[index].tfStart;  // og code
    }

    void* getGateTickflowOffset(int index) {
        if (config->tickflows.contains(index + 0x100)) {
            int result = Megamix::btks.LoadFile(config->tickflows[index + 0x100]);
            if (!result) {
                return (void*)(Megamix::btks.start);
            } else {
                OSD::Notify(CTRPluginFramework::Utils::Format("Error: %s", Megamix::ErrorMessage(result).c_str()));    
            }
        }
        return Game::gGateTable()[index].tfStart; // og code
    }

    void* getGatePracticeTickflowOffset(int index) {
        if (config->tickflows.contains((index >> 2) + 0x110)) {
            int result = Megamix::btks.LoadFile(config->tickflows[(index >> 2) + 0x110]);
            if (!result) {
                return (void*)(Megamix::btks.start);
            } else {
                OSD::Notify(CTRPluginFramework::Utils::Format("Error: %s", Megamix::ErrorMessage(result).c_str()));    
            }
        }
        return Game::gGateTable()[index].tfGatePractice; // og code
    }

    TempoTable* getTempoStrm(Megamix::CSoundManager* this_, u32 id) {
        if (Megamix::btks.tempos.contains(id)) {
            return Megamix::btks.tempos[id];
        } else { // Original code
            for (s32 low = 0, high = this_->numberTempos; low <= high;) {
                s32 current_num = (low + high) / 2;
                Megamix::SM_TempoTable* current = &this_->tableStrm[current_num];
                if (current->id > id) high = current_num - 1;
                if (current->id < id) low = current_num + 1;
                if (current->id == id) return current->tempo;
            }
            return 0;
        }
    }

    TempoTable* getTempoSeq(Megamix::CSoundManager* this_, u32 id) {
        if (Megamix::btks.tempos.contains(id)) {
            return Megamix::btks.tempos[id];
        } else { // Original code
            for (s32 low = 0, high = this_->numberTempos; low <= high;) {
                s32 current_num = (low + high) / 2;
                Megamix::SM_TempoTable* current = &this_->tableSeq[current_num];
                if (current->id > id) high = current_num - 1;
                if (current->id < id) low = current_num + 1;
                if (current->id == id) return current->tempo;
            }
            return 0;
        }
    }

    TempoTable* getTempoAll(Megamix::CSoundManager* this_, u32 id) {
        if (Megamix::btks.tempos.contains(id)) {
            return Megamix::btks.tempos[id];
        } else { // Original code
            for (int i = 0; i < this_->numberTempos; i++) {
                Megamix::TempoTable* current = &this_->tempoTable[i];
                if (current->id1 == id || current->id2 == id)
                    return current;
            }
            return 0;
        }
    }

    Game::RegionSDK getRegionCTR() {
        using namespace Game;
        //TODO: handle JP region / JP langpack (?)

        if (isJP())
            return RegionSDK::JP;
        else if (isUS())
            return RegionSDK::US;
        else if (isEU())
            return RegionSDK::EU;
        else if (isKR())
            return RegionSDK::KR;
        else
            return RegionSDK::UNK;
    }

    Game::RegionMegamix getRegionMegamix() {
        using namespace Game;
        //TODO: handle JP region / JP langpack (?)

        if (isJP())
            return RegionMegamix::JP;
        else if (isUS())
            return RegionMegamix::US;
        else if (isEU())
            return RegionMegamix::EU;
        else if (isKR())
            return RegionMegamix::KR;
        else
            return RegionMegamix::UNK;
    }

    int decideFinalScore(Megamix::CResultManager *arg1) {
        u8 amtCategories = arg1->mAmtCategories;
        u32* amtHit = arg1->mAmtHit;
        u32* amtBarely = arg1->mAmtBarely;
        u32* amtMiss = arg1->mAmtMiss;
        float catScore10000 = 0.0f;
        float totalWeight = 0.0f;
        float scoreDivWeight = 0.0f;
        float amtInputs = 0.0f;
        float result = 0.0f;
        int finalResult = 0;
        float curScoreWeight = 0.0f;
        int validCategories = 0;
        float distributedWeight = 0.0f;
        float distributedAmtInputs = 0.0f;
        float distributedScore = 0.0f;

        debugScoreArray.clear();

        if (amtCategories != 0) {
            for (int g = 0; g < 7; g++) {
                amtInputs = (int)(amtHit[g] + amtBarely[g] + amtMiss[g]);
                if(amtInputs != 0){
                    validCategories += 1;
                }
            }

            distributedWeight = (float)(arg1->mScoreWeight[7]) / (float)(validCategories);
            distributedAmtInputs = (float)(amtHit[7] + amtBarely[7] + amtMiss[7]) / (float)(validCategories);
            if (arg1->points[7] < 1) {
                distributedScore = (float)(arg1->points[7] + 1) / (float)(validCategories);
            } else {
                distributedScore = (float)(arg1->points[7]) / (float)(validCategories);
            }

            for (int i = 0; amtCategories > i; i++) {
                if (i != 7) {
                    amtInputs = (float)(amtHit[i] + amtBarely[i] + amtMiss[i]);
                    if (amtInputs > 0) {
                        if ((float)(amtHit[i] + amtBarely[i] + amtMiss[i]) < 1) {
                            amtInputs = 0;
                        }
                        else {
                            amtInputs += distributedAmtInputs;
                            if (arg1->points[i] < 1) {
                                catScore10000 = ((arg1->points[i] + distributedScore + 1) * 10000) / (amtInputs * arg1->mMaxWeight[i]);
                            } else {
                                catScore10000 = ((arg1->points[i] + distributedScore) * 10000) / (amtInputs * arg1->mMaxWeight[i]);
                            }
                        }

                        debugScoreArray.push_back(catScore10000);

                        curScoreWeight = (float)(arg1->mScoreWeight[i]) + distributedWeight;
                        totalWeight += curScoreWeight;
                        scoreDivWeight += curScoreWeight/catScore10000;
                    }
                }
            }

            if (totalWeight > 0.0f) {
                result = totalWeight/scoreDivWeight;
                finalResult = (int)(result - (arg1->mAmtPenalties * arg1->mPenalty));
                if (finalResult > 10000) {
                    finalResult = 10000;
                }
                if (finalResult > 0) {
                    return finalResult;
                }
            }
        }
        return 0;
    }

    void TickflowHooks() {
        rtInitHook(&tickflowHook, GHooks::tickflow(), (u32)getTickflowOffset);
        rtEnableHook(&tickflowHook);
        rtInitHook(&gateHook, GHooks::gate(), (u32)getGateTickflowOffset);
        rtEnableHook(&gateHook);
        rtInitHook(&gatePracHook, GHooks::gatePractice(), (u32)getGatePracticeTickflowOffset);
        rtEnableHook(&gatePracHook);
    }

    void TempoHooks() {
        rtInitHook(&tempoStrmHook, GHooks::strmTempo(), (u32)getTempoStrm);
        rtEnableHook(&tempoStrmHook);
        rtInitHook(&tempoSeqHook, GHooks::seqTempo(), (u32)getTempoSeq);
        rtEnableHook(&tempoSeqHook);
        rtInitHook(&tempoAllHook, GHooks::allTempo(), (u32)getTempoAll);
        rtEnableHook(&tempoAllHook);
    }

    void RegionHooks() {
        if (!Megamix::isJP()){
            rtInitHook(&regionFSHook, GHooks::megamixRegionCode(), (u32)getRegionMegamix);
            rtEnableHook(&regionFSHook);
        }
        rtInitHook(&regionOtherHook, GHooks::sdkRegionCode(), (u32)getRegionCTR);
        rtEnableHook(&regionOtherHook);
    }

    void CommandHook() {
        rtInitHook(&tickflowCommandsHook, Game::hTickflowCmds::hook(), (u32)tickflowCommandsHookWrapper);
        rtEnableHook(&tickflowCommandsHook);
    }

    void ScoringHook(){
        rtInitHook(&scoringFunctionHook, GHooks::scoring(), (int)decideFinalScore);
        rtEnableHook(&scoringFunctionHook);
    }

    void DisableAllHooks() {
        rtDisableHook(&tickflowHook);
        rtDisableHook(&gateHook);
        rtDisableHook(&tempoStrmHook);
        rtDisableHook(&tempoSeqHook);
        rtDisableHook(&tempoAllHook);
        rtDisableHook(&regionFSHook);
        rtDisableHook(&regionOtherHook);
        rtDisableHook(&scoringFunctionHook);
        rtDisableHook(&tickflowCommandsHook);
    }



    template<typename T>
    T StubbedFunction() {
        return {};
    }

    template<>
    void StubbedFunction<void>() {
    }

    // redirects function at address to a stubbed function
    template<typename T>
    void StubFunction(u32 address) {
        rtGenerateJumpCode(
            (u32)   StubbedFunction<T>,
            (u32 *) address
        );
    }

    // template instantiations
    // if either StubbedFunction or StubFunction is used with a type, a template
    // instantiation must be added with that type
    template void StubFunction<void>(u32);

}
