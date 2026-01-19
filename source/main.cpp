/**
 * @file main.cpp
 * @author Captain Kitty Cat (youtube.com/@captainkittyca2)
 * @brief Adds three masks, 3-day cycle, ISG, and more mechanics from Majora's Mask onto TP
 * @version 0.6.1
 * @date 2026-01-19
 *
 * @copyright Copyright (c) 2026
 *
 */
#include <display/console.h>     // Contains a very neat helper class to print to the console
#include <main.h>
#include <patch.h>     // Contains code for hooking into a function
#include <cstdio>
#include <tp/f_ap_game.h>
#include <tp/d_meter2.h>
#include <tp/d_meter_HIO.h>
#include <tp/d_com_inf_game.h>
#include <tp/m_do_audio.h>
#include <tp/d_menu_window.h>
#include <tp/JFWSystem.h>
#include <tp/processor.h>
#include <tp/JKRArchivePub.h>
#include <string.h>
#include <tp/rel/ids.h>
#include <tp/d_meter2_info.h>
#include <tp/d_bomb.h>
#include <gc_wii/OS.h>
#include <gc_wii/OSCache.h>
#include <gc_wii/OSTime.h>
#include <tp/d_item.h>
#include <JSystem/JMath/JMATrigonometric.h>

#ifdef PLATFORM_WII
#include <tp/m_re_controller_pad.h>
#else
#include <tp/m_do_controller_pad.h>
#endif

#define MSG_COLOR(id) "\x1A\x06\xFF\x00\x00" id
#define MSG_COLOR_UNK "\x00"
#define MSG_COLOR_RED "\x01"
#define MSG_COLOR_YELLOW "\x04"

namespace mod
{
    Mod* gMod = nullptr;

    void main()
    {
        Mod* mod = new Mod();
        mod->init();
    }

    void exit() {}

    Mod::Mod(): c( 0 )
    {
    }
    daMidna_checkMetamorphoseEnableBase_Def daMidna_c__checkMetamorphoseEnableBase = nullptr;
    //daE_fz_actionModeSet return_e_fz_actionModeSet = nullptr;
#ifndef PLATFORM_WII
    daA_arrow_execute return_a_arrow_execute = nullptr;
    daE_db_action return_e_db_action = nullptr;
    daE_dn_action return_e_dn_action = nullptr;
    daE_kk_actionModeSet return_e_kk_actionModeSet = nullptr;
    daE_kr_action return_e_kr_action = nullptr;
    daE_rd_action return_e_rd_action = nullptr;
    uint16_t bs_e_address[2] = {0x744, 0x768};
    uint16_t bu_e_address = 0x658;
    uint16_t db_e_address[2] = {0x6B8, 0x504C};
    uint16_t dd_e_address = 0x6E0;
#else
    uint16_t bs_e_address[2] = {0x698, 0x6BC};
    uint16_t bu_e_address = 0x8C0;
    uint16_t db_e_address[1] = {0x9B0};
    uint16_t dd_e_address = 0x930;
#endif
    daE_ww_actionModeSet return_e_ww_actionModeSet = nullptr;
    daNPC_impaa_execute return_npc_impaa_execute = nullptr;
    daE_oc_actionModeSet return_e_oc_actionModeSet = nullptr;
    daE_tt_actionModeSet return_e_tt_actionModeSet = nullptr;
    daE_ai_action return_e_ai_action = nullptr;
    daE_mm_action return_e_mm_action = nullptr;
    daE_sf_action return_e_sf_action = nullptr;
    daE_bu_attack return_e_bu_attack = nullptr;
    //daB_bh_execute return_b_bh_execute = nullptr;
    libtp::tp::jfw_system::SystemConsole* sysConsolePtr = libtp::tp::jfw_system::systemConsole;

    uint32_t readIconsTex = reinterpret_cast<uint32_t>(libtp::tp::d_meter2_info::readItemTexture);
    uint32_t bButtonDimm = reinterpret_cast<uint32_t>(libtp::tp::d_meter2_draw::setButtonIconBAlpha);
    uint32_t zButtonDimm = reinterpret_cast<uint32_t>(libtp::tp::d_meter2_draw::setButtonIconMidonaAlpha);
    uint32_t activeASMPatchingLOL = reinterpret_cast<uint32_t>(libtp::tp::d_menu_ring::setActiveCursor);
    uint32_t itemASMPatchLOOL = reinterpret_cast<uint32_t>(libtp::tp::d_menu_ring::setItemFromItemRing);
    uint32_t selectItemForZSelect = reinterpret_cast<uint32_t>(libtp::tp::d_com_inf_game::dComIfGp_getSelectItem);
    uint32_t zNewItemChangeExamination = reinterpret_cast<uint32_t>(libtp::tp::d_a_alink::checkNewItemChange);
    uint32_t gameOverCreatee = reinterpret_cast<uint32_t>(libtp::tp::d_gameover::createGameOver);
    uint32_t gameOverTeeeeeext = reinterpret_cast<uint32_t>(libtp::tp::d_gameover::ctGameOver);
    uint32_t gameHourssss = reinterpret_cast<uint32_t>(libtp::z2audiolib::z2scenemgr::setSceneName);
    //uint32_t retry__NOPELOLOLOL = reinterpret_cast<uint32_t>(libtp::tp::d_gameover::retryQueston0);

    // Saved mod info
    struct saveInfo
    {
        /* 0x00 */ uint8_t bobombStorePlace[2];
        /* 0x02 */ uint16_t rupeeeStoreePlace;
        /* 0x04 */ uint32_t timeRecordedReferr;
        /* 0x08 */ uint8_t dPadChoice;
        /* 0x09 */ uint8_t doubleDefencee;
        /* 0x0A */ uint8_t nightOfTheee;
        /* 0x0B */ uint8_t bowwStorePlace;
        uint8_t unk[62];
        /* 0x4A */ uint8_t zButtonSloot;
        /* 0x4B */ uint8_t zButtonnItemm;
        /* 0x4C */ uint8_t maskID;
        /* 0x4D */ uint8_t MMGlitches;
        /* 0x4E */ uint8_t cycle3Days;
        /* 0x4F */ uint8_t MMMaskPack1ID;
    } __attribute__((__packed__));

    saveInfo* saveInfoPtr = reinterpret_cast<saveInfo*>(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.reserve);

    uint8_t crouch = 0;
    bool shieldCrouch = false;
    bool maskWear = false;
    bool gameplayStatus = false;
    bool blastNope = true;
    bool dpaaadRight = false;
    bool dpaaadDown = false;
    //bool zDimOrNope = false;
    int64_t second = 0;
    int64_t counting = 0;
    uint8_t timeRemaining = 15;
    uint8_t timeUpdate = 15;
    int64_t second2 = 0;
    int64_t countinng2 = 0;
    uint8_t timeRemaininng2 = 30;
    uint8_t timeUpdaate2 = 30;

    __attribute__((used, visibility("default"))) void performStaticASMReplacement(uint32_t memoryOffset, uint32_t value)
    {
        *reinterpret_cast<uint32_t*>(memoryOffset) = value;

        // Clear the cache for the modified value
        // Assembly instructions need to clear the instruction cache as well
        libtp::memory::clear_DC_IC_Cache(reinterpret_cast<uint32_t*>(memoryOffset), sizeof(uint32_t));
    }

    // Check if no mask is worn
    /*bool areYouNoMask() {
        if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[0] != 0x2f || saveInfoPtr->maskID == 0)
            return true;
        return false;
    }*/

    // How visible the B button should be when wearing blast mask
    void iconBButtonDimFunc(uint8_t param0) {
#ifndef PLATFORM_WII
        switch (param0) {
            // default behavior
            case 0:
            {
                if (saveInfoPtr->maskID != 2 || blastNope)
                {
                    performStaticASMReplacement(bButtonDimm + 0x114, 0x40820028);
                    performStaticASMReplacement(bButtonDimm + 0x120, 0x4082001c);
                    performStaticASMReplacement(bButtonDimm + 0x130, 0x7c190378);
                    performStaticASMReplacement(bButtonDimm + 0x134, 0x7c1a0378);
                    performStaticASMReplacement(bButtonDimm + 0x138, 0x8b63042b);
                }
                break;
            }
            // dim
            case 1:
            {
                performStaticASMReplacement(bButtonDimm + 0x114, ASM_NOP);
                performStaticASMReplacement(bButtonDimm + 0x120, ASM_NOP);
                performStaticASMReplacement(bButtonDimm + 0x130, 0x3b200080);
                performStaticASMReplacement(bButtonDimm + 0x134, 0x3b400080);
                performStaticASMReplacement(bButtonDimm + 0x138, 0x3b600080);
                break;
            }
            // not dim
            case 2:
            {
                if (saveInfoPtr->maskID != 2 || blastNope) {
                    performStaticASMReplacement(bButtonDimm + 0x114, ASM_NOP);
                    performStaticASMReplacement(bButtonDimm + 0x120, ASM_NOP);
                    performStaticASMReplacement(bButtonDimm + 0x130, 0x3b2000ff);
                    performStaticASMReplacement(bButtonDimm + 0x134, 0x3b4000ff);
                    performStaticASMReplacement(bButtonDimm + 0x138, 0x3b6000ff);
                }
                break;
            }
        }
#else
        (void)param0;
#endif
    }

    bool isgStart = false;
    uint8_t isgCarryObjectsSwingCountDown = 0;

    void isgEnnd(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        libtp::tp::d_a_alink::decSwordBlur(linkActrPtr);
        libtp::tp::d_a_alink::resetAtCollision(linkActrPtr, 1);
        libtp::tp::J3DModel::ResetAtHit(&linkActrPtr->mAtCyl);
    }

    int32_t crouchStuff(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        if (!libtp::tp::d_a_alink::commonProcInitNotSameProc(linkActrPtr, libtp::tp::d_a_alink::PROC_CROUCH)) return 0;

        if (crouch == 1) libtp::tp::d_a_alink::setSingleAnimeBaseSpeed(linkActrPtr, libtp::tp::d_a_alink::ANM_CROUCH_DEF, libtp::tp::d_a_alink::crouchVars.mCrouchAnmSpeed,
                                                    libtp::tp::d_a_alink::crouchVars.mCrouchInterpolation);
        else {
            if (isgStart) isgEnnd(linkActrPtr);
            isgStart = false;
            isgCarryObjectsSwingCountDown = 0;
            if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[2] != 0xFF) {
                libtp::tp::d_a_alink::setSingleAnimeBaseSpeed(linkActrPtr, libtp::tp::d_a_alink::ANM_CROUCH_START, libtp::tp::d_a_alink::crouchVars.mCrouchAnmSpeed,
                                                                libtp::tp::d_a_alink::crouchVars.mCrouchInterpolation);
                linkActrPtr->mCurrent.mAngle.y = linkActrPtr->mCollisionRot.y;
                shieldCrouch = true;
                iconBButtonDimFunc(2);
                /*if (saveInfoPtr->MMGlitches == 0) {
                    sprintf(sysConsolePtr->consoleLine[4].line, "mDoStatus: 0x%x", libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDoStatus);
                }*/
                return 1;
            }
        }
        return 0;
    }

    int32_t downNope() {
        return false;
    }

    void stageFlagResetFunnc(uint8_t param0Stage, uint8_t param1Flag, uint8_t param2ValueForFlag) {
        libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[param0Stage].temp_flags.memoryFlags[param1Flag] = param2ValueForFlag;
    }

    void currentFlagReseetFunnc(uint8_t param1Flag, uint8_t param2FlagValue) {
        libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.memory.temp_flags.memoryFlags[param1Flag] = param2FlagValue;
    }

    void stageFlagResetSpecificFunnc(uint8_t param0Stage, uint8_t param1Flag, uint8_t param2FlagValue) {
        if (param2FlagValue != 1) libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[param0Stage].temp_flags.memoryFlags[param1Flag] &= ~1;
        if (param2FlagValue != 2) libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[param0Stage].temp_flags.memoryFlags[param1Flag] &= ~2;
        if (param2FlagValue != 4) libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[param0Stage].temp_flags.memoryFlags[param1Flag] &= ~4;
        if (param2FlagValue != 8) libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[param0Stage].temp_flags.memoryFlags[param1Flag] &= ~8;
        if (param2FlagValue != 16) libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[param0Stage].temp_flags.memoryFlags[param1Flag] &= ~16;
        if (param2FlagValue != 32) libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[param0Stage].temp_flags.memoryFlags[param1Flag] &= ~32;
        if (param2FlagValue != 64) libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[param0Stage].temp_flags.memoryFlags[param1Flag] &= ~64;
        if (param2FlagValue != 128) libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[param0Stage].temp_flags.memoryFlags[param1Flag] &= ~128;
    }

    void stageFlagReset2Funnc(uint8_t param0Stage, uint8_t param1Flag, uint8_t param2ValueForFlag) {
        libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[param0Stage].temp_flags.memoryFlags[param1Flag] |= param2ValueForFlag;
    }

    void eventFlagResetFunnc(uint16_t param0EventFlaag, bool param1OnBool) {
        if (param1OnBool) libtp::tp::d_save::onEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, param0EventFlaag);
        else libtp::tp::d_save::offEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, param0EventFlaag);
    }

    void isgInitiation(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        cXyz spC8;
        spC8 = linkActrPtr->mCurrent.mPosition;
        spC8.x += (50.0f * libtp::tp::sincosTable_.sinShort(linkActrPtr->mCollisionRot.y));
        spC8.y += 20.0f;
        spC8.z += (50.0f * libtp::tp::sincosTable_.cosShort(linkActrPtr->mCollisionRot.y));
        libtp::tp::d_a_alink::decSwordBlur(linkActrPtr);
        libtp::tp::d_a_alink::resetAtCollision(linkActrPtr, 1);
        libtp::tp::J3DModel::ResetAtHit(&linkActrPtr->mAtCyl);
        linkActrPtr->field_0x2fce = 5;
        linkActrPtr->mComboCutCount = 0;
        libtp::tp::d_a_player::offNoResetFlg0(linkActrPtr, libtp::tp::d_a_player::FLG0_UNK_8000);
        libtp::tp::d_a_player::offNoResetFlg2(linkActrPtr, libtp::tp::d_a_player::FLG2_UNK_2);
        libtp::tp::d_a_player::onResetFlg0(linkActrPtr, libtp::tp::d_a_player::RFLG0_UNK_2);
        libtp::tp::d_a_alink::setSwordAtParam(linkActrPtr, libtp::tp::J3DModel::dCcG_At_Spl_UNK_0, 7, 16, 2, libtp::tp::d_a_alink::cutVars.mSwordLength, libtp::tp::d_a_alink::cutVars.mSwordRadius);
        linkActrPtr->field_0x2fd0 = 2;
        linkActrPtr->mRunCutComboCount++;
        linkActrPtr->mCutType = 0x25;
        linkActrPtr->mRunCutComboCount = 1;
        linkActrPtr->field_0x2f96 = 2;
        linkActrPtr->field_0x307e = libtp::tp::d_a_alink::cutVars.mComboDuration;
        libtp::tp::d_a_alink::setCylAtParam(linkActrPtr, libtp::tp::d_a_alink::getSwordAtType(linkActrPtr), libtp::tp::J3DModel::dCcG_At_Spl_UNK_0, 7, 16, 2, 50.0f, 130.0f);
        libtp::tp::d_a_alink::setCutWaterDropEffect(linkActrPtr);
        linkActrPtr->field_0x2fb7 = 0;
        linkActrPtr->m_swordBlur.field_0x20 = 90;
        libtp::tp::d_a_alink::setSwordAtCollision(linkActrPtr, 1);
        libtp::tp::d_a_player::onNoResetFlg0(linkActrPtr, libtp::tp::d_a_player::FLG0_UNK_40);
        libtp::tp::J3DModel::StartCAtCyl(&linkActrPtr->mAtCyl, spC8);
        libtp::tp::J3DModel::cCcDSet(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDCcS, &linkActrPtr->mAtCyl);
        libtp::tp::d_a_alink::initBlur(&linkActrPtr->m_swordBlur, 0.0f, 0, &linkActrPtr->mSwordTopPos, &linkActrPtr->field_0x3498, &linkActrPtr->field_0x34a4);
    }

    bool crouchToISG(uint8_t whatDoStatusDude, libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        if (whatDoStatusDude == 0x1F || whatDoStatusDude == 6 || whatDoStatusDude == 0x1C || whatDoStatusDude == 0x80) {
            bool goodToGo = false;
            if (whatDoStatusDude == 6 || whatDoStatusDude == 0x1C) {
                if (linkActrPtr->mCutType > 0) {
                    isgInitiation(linkActrPtr);
                    goodToGo = true;
                }
            } else if (whatDoStatusDude == 0x1F || whatDoStatusDude == 0x80) {
                if (isgCarryObjectsSwingCountDown > 0) {
                    isgCarryObjectsSwingCountDown = 0;
                    libtp::tp::d_a_alink::deleteEquipItem(linkActrPtr, 0, 0);
                    goodToGo = true;
                }
            }
            if (goodToGo) {
                if (whatDoStatusDude != 0x1F) libtp::tp::d_a_alink::checkNextActionFromCrouch(linkActrPtr, 0);
                if (whatDoStatusDude == 0x1F) libtp::tp::d_a_alink::procGrabReadyInit(linkActrPtr);
                else if (whatDoStatusDude == 6) libtp::tp::d_a_alink::procDoorOpenInit(linkActrPtr);
                else libtp::tp::d_a_alink::procCoTalkInit(linkActrPtr);
                isgStart = true; crouch = 0; shieldCrouch = 0; return true;
            }
        }
        return false;
    }

    bool snowGameInOrOut() {
        return libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.mTmp, 0x0904) || libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.mTmp, 0x0902);
    }

    uint8_t timeResetUseed = 0;
    bool outOfTimeItsOver = false;
    bool smallTimeLeftHurryyyyy = false;

    void zButtonTextureMomentInit(uint8_t itemParam0) {
        char susTesttinng[] = ".bti";
        char susTesTinng2[42];
        switch (itemParam0)
        {
        case 0x40:
            strcpy(susTesTinng2, "tt_boomerang_05");
            break;
        case 0x41:
            strcpy(susTesTinng2, "im_sppiner_48");
            break;
        case 0x42:
            strcpy(susTesTinng2, "im_ironball_48");
            break;
        case 0x43:
            strcpy(susTesTinng2, "tt_bow_06");
            break;
        case 0x44:
            strcpy(susTesTinng2, "tt_hook_shot_01");
            break;
        case 0x45:
            strcpy(susTesTinng2, "im_magne_boots_06");
            break;
        case 0x46:
        case 0x4C:
            if (libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x2580)) strcpy(susTesTinng2, "im_copy_rod_48");
            else strcpy(susTesTinng2, "st_copy_rod_b");
            break;
        case 0x47:
            strcpy(susTesTinng2, "im_w_hookshot_48");
            break;
        case 0x48:
            strcpy(susTesTinng2, "tt_kantera_48");
            break;
        case 0x4A:
            strcpy(susTesTinng2, "im_fishing_48");
            break;
        case 0x4B:
            strcpy(susTesTinng2, "st_pachinko");
            break;
        case 0x4F:
        case 0x50:
            strcpy(susTesTinng2, "st_bompoach_lv1");
            break;
        case 0x60:
            strcpy(susTesTinng2, "tt_bottle_48");
            break;
        case 0x61:
            strcpy(susTesTinng2, "tt_bottle_recovery_48");
            break;
        case 0x62:
        case 0x66:
        case 0x68:
            strcpy(susTesTinng2, "tt_bottle_oil_48");
            break;
        case 0x64:
            strcpy(susTesTinng2, "tt_bottle_milk_48");
            break;
        case 0x65:
            strcpy(susTesTinng2, "tt_bottle_halfmilk_48");
            break;
        case 0x70:
            strcpy(susTesTinng2, "im_bom_normal_48");
            break;
        case 0x71:
            strcpy(susTesTinng2, "im_bom_suichu_48");
            break;
        case 0x72:
            strcpy(susTesTinng2, "im_bom_mushi_48");
            break;
        case 0xE1:
            strcpy(susTesTinng2, "zz_bunnyunny");
            break;
        case 0xE2:
            strcpy(susTesTinng2, "zz_maskask");
            break;
        case 0xE3:
            strcpy(susTesTinng2, "im_honny_48");
            break;
        case 0:
        case 0xFF:
            libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D69646F6E61)->hideTex();
            return;
        default:
            strcpy(susTesTinng2, "im_zelda_item_icon_rupy_13");
            break;
        }
        /*if (itemParam0 >= 0x60 && itemParam0 < 0x70) {
            strcpy(susTesTinng2, "im_bottle");
            if (itemParam0 > 0x60) {
                
            }
        }*/
        strcat(susTesTinng2, susTesttinng);
        //sprintf(sysConsolePtr->consoleLine[20].line, "%s", susTesTinng2);
        static_cast<libtp::tp::J2DPicture::J2DPicture*>(libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D69646F6E61))->smthgTesting2(susTesTinng2, 0);
        libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D69646F6E61)->showTex();
    }

    void timeReset3Day(bool SaveeNeww) {
        using namespace libtp::tp::d_com_inf_game;
        if (SaveeNeww) {
            dComIfG_gameInfo.play.mNextStage.mLayer = 0xFF;
            dComIfG_gameInfo.play.mNextStage.mPoint = 0;
            libtp::tp::d_item::execItemGet(0x48); libtp::tp::d_item::execItemGet(0x60);
            libtp::tp::d_item::execItemGet(0x4A); libtp::tp::d_item::execItemGet(0x4B);
            libtp::tp::d_save::onFirstBit(&dComIfG_gameInfo.save.save_file.player.player_get_item, 0x28);
            libtp::tp::d_save::onFirstBit(&dComIfG_gameInfo.save.save_file.player.player_get_item, 0x2A);
            libtp::tp::d_save::onFirstBit(&dComIfG_gameInfo.save.save_file.player.player_get_item, 0x2F);
            dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[0] = 0x2F;
            dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[1] = 0x28;
            dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[2] = 0x2A;
            /*libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_collect.equipment[2] = 0x2A;
            libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_collect.equipment[1] = 0x28;
            libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_collect.equipment[0] = 0x2F;*/
            libtp::tp::d_save::onEventBit(&dComIfG_gameInfo.save.save_file.mEvent, 0x580);
            libtp::tp::d_save::onEventBit(&dComIfG_gameInfo.save.save_file.mEvent, 0x501);
            libtp::tp::d_save::onEventBit(&dComIfG_gameInfo.save.save_file.mEvent, 0x604);
            libtp::tp::d_save::onEventBit(&dComIfG_gameInfo.save.save_file.mEvent, 0xC10);
            libtp::tp::d_save::onEventBit(&dComIfG_gameInfo.save.save_file.mEvent, 0x4308);
            dComIfG_gameInfo.save.save_file.player.player_last_stay_info.player_last_region |= (1 << 1);
            dComIfG_gameInfo.save.save_file.player.player_last_stay_info.player_last_region |= (1 << 2);
            dComIfG_gameInfo.save.save_file.player.player_last_stay_info.player_last_region |= (1 << 3);
            libtp::tp::d_save::onSwitch_dSv_memBit(&dComIfG_gameInfo.save.save_file.mSave[0].temp_flags, 52);
            libtp::tp::d_save::onSwitch_dSv_memBit(&dComIfG_gameInfo.save.save_file.mSave[2].temp_flags, 71);
            libtp::tp::d_save::onSwitch_dSv_memBit(&dComIfG_gameInfo.save.save_file.mSave[2].temp_flags, 2);
            libtp::tp::d_save::onSwitch_dSv_memBit(&dComIfG_gameInfo.save.save_file.mSave[3].temp_flags, 21);
            libtp::tp::d_save::onSwitch_dSv_memBit(&dComIfG_gameInfo.save.save_file.mSave[3].temp_flags, 31);
            libtp::tp::d_save::onSwitch_dSv_memBit(&dComIfG_gameInfo.save.save_file.mSave[6].temp_flags, 21);
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[3] = 0x10;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[8] = 0x7F;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[9] = 0xE2;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[0x0A] = 0x3F;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[0x0B] = 0xBA;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[0x0C] = 0x1B;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[0x0D] = 0x9B;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[0x0E] = 0xAE;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[0x0F] = 0x2F;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[0x12] = 2;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[0x13] = 0x19;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[0x14] = 0x81;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[0x15] = 0x10;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[0x16] = 1;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[0x17] = 0xA0;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[0x1A] = 0x90;
            dComIfG_gameInfo.save.save_file.mSave[0].temp_flags.memoryFlags[0x1B] = 8;

            dComIfG_gameInfo.save.save_file.mSave[1].temp_flags.memoryFlags[0x08] = 0xFD;
            dComIfG_gameInfo.save.save_file.mSave[1].temp_flags.memoryFlags[0x09] = 0x1F;
            dComIfG_gameInfo.save.save_file.mSave[1].temp_flags.memoryFlags[0x0A] = 0xFF;
            dComIfG_gameInfo.save.save_file.mSave[1].temp_flags.memoryFlags[0x0B] = 0x7F;
            dComIfG_gameInfo.save.save_file.mSave[1].temp_flags.memoryFlags[0x0F] = 0x19;

            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0] = 1;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[1] = 0xB6;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[2] = 0x7B;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[3] = 0x73;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[8] = 0x1B;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[9] = 0xD1;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0xA] = 0xF3;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0xB] = 0x6C;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0xC] = 0x41;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0xD] = 0xFD;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0xE] = 0xEB;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0xF] = 0xFE;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0x10] = 3;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0x11] = 0xFD;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0x12] = 0x9A;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0x13] = 0x81;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0x14] = 0x77;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0x15] = 0xFF;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0x16] = 0xFF;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0x17] = 0xF3;
            
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0] = 0x80;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[1] = 0x14;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[2] = 0x60;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[3] = 0x97;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[4] = 8;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[5] = 0xff;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[6] = 0xD0;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x0C] = 0x18;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x0D] = 0x81;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x0F] = 1;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x10] = 0x17;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x11] = 5;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x12] = 0xA;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x13] = 0x80;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x14] = 0xA;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x15] = 0x80;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x16] = 1;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x18] = 0x81;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x19] = 0x64;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x1A] = 0x10;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x1B] = 0x40;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x2C] = 2;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x39] = 2;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x43] = 8;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x45] = 0x10;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x46] = 0x11;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x47] = 0x30;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x49] = 2;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x4A] = 0x72;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x4B] = 0x48;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x4C] = 1;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x4D] = 8;
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x50] = 0x80;
            dComIfG_gameInfo.save.save_file.player.light_drop.faron_tear_count = 16;
            dComIfG_gameInfo.save.save_file.player.light_drop.eldin_tear_count = 16;
            dComIfG_gameInfo.save.save_file.player.light_drop.light_drop_flag = 3;
            //dComIfG_gameInfo.save.save_file.player.player_status_b.dark_clear_level_flag = 3;
        } else {
            // Forest Temple
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0] &= ~0x80;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0] &= ~0x40;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0] &= ~0x10;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0] &= ~8;
            dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0] &= ~1;
            stageFlagReset2Funnc(2, 0, 1);
            stageFlagResetFunnc(2, 4, 0);
            stageFlagResetFunnc(2, 9, 0xD1);
            stageFlagResetFunnc(2, 0xC, 0x41);
            stageFlagResetFunnc(2, 0xA, 0xF3);
            stageFlagResetFunnc(2, 0x10, 3);
            stageFlagResetFunnc(2, 0x13, 0xC9);
            stageFlagResetFunnc(2, 0x14, 0x7F);
            stageFlagResetFunnc(2, 0x17, 0xF0);
            dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x1B] = 0x40;
            eventFlagResetFunnc(0x0602, false);
            eventFlagResetFunnc(0x5901, false);
            eventFlagResetFunnc(0x5c04, false);
            eventFlagResetFunnc(0x5d08, false);
            eventFlagResetFunnc(0x5d04, false);
            eventFlagResetFunnc(0x5d02, false);
            eventFlagResetFunnc(0x6110, false);
            libtp::tp::d_item::execItemGet(0x48);

            stageFlagResetSpecificFunnc(0x10, 0, 128);
            stageFlagResetSpecificFunnc(0x10, 1, 8);
            for (uint8_t param0Dung = 2; param0Dung < 0x1E; param0Dung++) {
                if (param0Dung != 0x16 && param0Dung != 0x1D) stageFlagResetFunnc(0x10, param0Dung, 0);
                else if (param0Dung == 0x16) stageFlagResetSpecificFunnc(0x10, 0x16, 2);
                else if (param0Dung == 0x1D) stageFlagResetSpecificFunnc(0x10, 0x1D, 16);
            }

            // Goron Mines
            if (dComIfG_gameInfo.save.save_file.player.player_status_b.dark_clear_level_flag >= 3) {
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[1] &= ~0x80;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[1] &= ~0x10;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[1] &= ~2;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x1a] &= ~0x40;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x1a] &= ~0x20;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x1a] &= ~4;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x1a] &= ~2;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x1a] &= ~1;
                stageFlagResetFunnc(3, 0x1B, 0);
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x0C] &= ~0x20;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x0C] &= ~0x40;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x10] &= ~0x20;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x11] &= ~0x40;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x13] &= ~0x10;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x13] &= ~0x40;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x13] &= ~8;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x14] &= ~8;
                stageFlagResetFunnc(3, 0x15, 0xC);
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x16] &= ~0x80;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x16] &= ~0x20;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x16] &= ~0x10;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x16] &= ~1;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x17] &= ~0x80;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x17] &= ~2;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x17] &= ~1;
                eventFlagResetFunnc(0x0608, false);
                eventFlagResetFunnc(0x0780, false);
                eventFlagResetFunnc(0x0a08, false);
                eventFlagResetFunnc(0x0a04, false);
                eventFlagResetFunnc(0x0a02, false);
                eventFlagResetFunnc(0x0a01, false);
                eventFlagResetFunnc(0x1d20, false);
                eventFlagResetFunnc(0x1d08, false);
                eventFlagResetFunnc(0x1d04, false);
                eventFlagResetFunnc(0x1d02, false);
                dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x1F] = 0;
                eventFlagResetFunnc(0x0004, false);
                eventFlagResetFunnc(0x0740, false);
                eventFlagResetFunnc(0x0704, false);
                eventFlagResetFunnc(0x0702, false);
                eventFlagResetFunnc(0x1201, false);
                eventFlagResetFunnc(0x1340, false);
                eventFlagResetFunnc(0x1320, false);
                eventFlagResetFunnc(0x1302, false);
                eventFlagResetFunnc(0x2c01, false);
                eventFlagResetFunnc(0x2E80, false);
                eventFlagResetFunnc(0x3008, false);
                eventFlagResetFunnc(0x3702, false);
                eventFlagResetFunnc(0x3701, false);
                eventFlagResetFunnc(0x3c10, false);
                dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x52] = 0;
                eventFlagResetFunnc(0x0701, false);
            }
            stageFlagResetFunnc(0x11, 0, 0);
            stageFlagResetSpecificFunnc(0x11, 1, 0x10);
            stageFlagResetSpecificFunnc(0x11, 2, 2);
            stageFlagResetFunnc(0x11, 3, 0);
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[8] &= ~4;
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[8] &= ~2;
            stageFlagResetSpecificFunnc(0x11, 9, 0x20);
            stageFlagResetFunnc(0x11, 0xA, 0);
            stageFlagResetSpecificFunnc(0x11, 0xB, 4);
            stageFlagResetFunnc(0x11, 0xC, 0);
            stageFlagResetSpecificFunnc(0x11, 0xD, 0x80);
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0xE] &= ~8;
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0xE] &= ~4;
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0xE] &= ~2;
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0xE] &= ~1;
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0xF] &= ~0x80;
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0xF] &= ~0x40;
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0x10] &= ~0x20;
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0x10] &= ~2;
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0x11] &= ~4;
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0x11] &= ~2;
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0x11] &= ~1;
            stageFlagResetFunnc(0x11, 0x12, 0); stageFlagResetFunnc(0x11, 0x13, 0);
            stageFlagResetFunnc(0x11, 0x15, 0); stageFlagResetFunnc(0x11, 0x16, 0);
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0x17] &= ~0x80;
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0x17] &= ~0x20;
            dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0x17] &= ~0x10;
            stageFlagResetFunnc(0x11, 0x1C, 0);
            stageFlagResetSpecificFunnc(0x11, 0x1D, 16);

            // Lakebed Temple
            if (dComIfG_gameInfo.save.save_file.player.player_status_b.dark_clear_level_flag >= 7) {
                stageFlagResetSpecificFunnc(4, 0, 32);
                stageFlagResetFunnc(4, 1, 0);
                stageFlagResetFunnc(4, 2, 0);
                stageFlagResetFunnc(4, 3, 0);
                dComIfG_gameInfo.save.save_file.mSave[4].temp_flags.memoryFlags[8] &= ~0x20;
                dComIfG_gameInfo.save.save_file.mSave[4].temp_flags.memoryFlags[8] &= ~2;
                dComIfG_gameInfo.save.save_file.mSave[4].temp_flags.memoryFlags[9] &= ~8;
                dComIfG_gameInfo.save.save_file.mSave[4].temp_flags.memoryFlags[9] &= ~4;
                stageFlagResetSpecificFunnc(4, 0xC, 1);
                dComIfG_gameInfo.save.save_file.mSave[4].temp_flags.memoryFlags[0xE] &= ~0x80;
                dComIfG_gameInfo.save.save_file.mSave[4].temp_flags.memoryFlags[0xE] &= ~0x40;
                stageFlagResetFunnc(4, 0xF, 0x6F);
                stageFlagResetFunnc(4, 0x10, 0);
                stageFlagResetFunnc(4, 0x11, 0xE8);
                dComIfG_gameInfo.save.save_file.mSave[4].temp_flags.memoryFlags[0x13] &= ~0x20;
                dComIfG_gameInfo.save.save_file.mSave[4].temp_flags.memoryFlags[0x13] &= ~0x10;
                dComIfG_gameInfo.save.save_file.mSave[4].temp_flags.memoryFlags[0x13] &= ~1;
                dComIfG_gameInfo.save.save_file.mSave[4].temp_flags.memoryFlags[0x17] &= ~0x80;
                dComIfG_gameInfo.save.save_file.mSave[4].temp_flags.memoryFlags[0x17] &= ~0x20;
                dComIfG_gameInfo.save.save_file.mSave[4].temp_flags.memoryFlags[0x17] &= ~1;
                stageFlagResetFunnc(4, 0x18, 0);
                stageFlagResetFunnc(4, 0x19, 0);
                stageFlagResetFunnc(4, 0x1A, 0);
                stageFlagResetFunnc(4, 0x1B, 0);
                stageFlagResetFunnc(9, 3, 0);
                /*eventFlagResetFunnc(0x0804, false);
                eventFlagResetFunnc(0x0810, false);
                eventFlagResetFunnc(0x0820, false);
                eventFlagResetFunnc(0x0840, false);
                eventFlagResetFunnc(0x0904, false);*/
            }
            stageFlagResetFunnc(0x12, 0, 0);
            stageFlagResetFunnc(0x12, 1, 0);
            stageFlagResetSpecificFunnc(0x12, 2, 1);
            stageFlagResetSpecificFunnc(0x12, 3, 0x20);
            stageFlagResetSpecificFunnc(0x12, 8, 0x40);
            stageFlagResetFunnc(0x12, 9, 0);
            stageFlagResetFunnc(0x12, 0xA, 0);
            stageFlagResetFunnc(0x12, 0xB, 0);
            stageFlagResetSpecificFunnc(0x12, 0xD, 0x40);
            stageFlagResetFunnc(0x12, 0xE, 0);
            stageFlagResetSpecificFunnc(0x12, 0xF, 2);
            stageFlagResetFunnc(0x12, 0x10, 0);
            stageFlagResetFunnc(0x12, 0x11, 0);
            stageFlagResetFunnc(0x12, 0x12, 0);
            stageFlagResetFunnc(0x12, 0x13, 0);
            dComIfG_gameInfo.save.save_file.mSave[0x12].temp_flags.memoryFlags[0x14] &= ~0x10;
            dComIfG_gameInfo.save.save_file.mSave[0x12].temp_flags.memoryFlags[0x16] &= ~0x20;
            dComIfG_gameInfo.save.save_file.mSave[0x12].temp_flags.memoryFlags[0x16] &= ~0x10;
            dComIfG_gameInfo.save.save_file.mSave[0x12].temp_flags.memoryFlags[0x16] &= ~8;
            dComIfG_gameInfo.save.save_file.mSave[0x12].temp_flags.memoryFlags[0x16] &= ~1;
            stageFlagResetFunnc(0x12, 0x17, 0);
            stageFlagResetFunnc(0x12, 0x1C, 0);
            stageFlagResetSpecificFunnc(0x12, 0x1D, 16);

            // Arbiter Ground
            if (dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[0x13] & 0x40) {
                stageFlagResetFunnc(0xA, 2, 0);
                stageFlagResetFunnc(0xA, 3, 0);
                stageFlagResetFunnc(0xA, 4, 0);
                stageFlagResetFunnc(0xA, 8, 0);
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[9] &= ~8;
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[9] &= ~4;
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[9] &= ~2;
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[9] &= ~1;
                stageFlagResetSpecificFunnc(0xA, 0xA, 0x10);
                stageFlagResetFunnc(0xA, 0xB, 0);
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[0xD] &= ~2;
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[0xD] &= ~1;
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[0xE] &= ~0x80;
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[0xE] &= ~0x20;
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[0x10] &= ~0x40;
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[0x10] &= ~2;
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[0x10] &= ~1;
                stageFlagResetFunnc(0xA, 0x11, 0);
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[0x14] &= ~0x80;
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[0x14] &= ~0x40;
                stageFlagResetFunnc(0xA, 0x17, 0);
                stageFlagResetFunnc(0xA, 0x1C, 0);
                eventFlagResetFunnc(0x0b40, false);
            }
            stageFlagResetSpecificFunnc(0x13, 0, 4);
            stageFlagResetSpecificFunnc(0x13, 1, 8);
            stageFlagResetFunnc(0x13, 2, 0);
            stageFlagResetFunnc(0x13, 3, 0);
            uint8_t poeAmmoExterminator = 0;
            if (dComIfG_gameInfo.save.save_file.mSave[0x13].temp_flags.memoryFlags[8] & 0x40) poeAmmoExterminator++;
            if (dComIfG_gameInfo.save.save_file.mSave[0x13].temp_flags.memoryFlags[8] & 0x80) poeAmmoExterminator++;
            if (dComIfG_gameInfo.save.save_file.mSave[0x13].temp_flags.memoryFlags[0xC] & 0x80) poeAmmoExterminator++;
            if (dComIfG_gameInfo.save.save_file.mSave[0x13].temp_flags.memoryFlags[0xF] & 2) poeAmmoExterminator++;
            dComIfG_gameInfo.save.save_file.player.player_collect.poe_count -= poeAmmoExterminator;
            stageFlagResetFunnc(0x13, 8, 0);
            stageFlagResetFunnc(0x13, 9, 0);
            stageFlagResetFunnc(0x13, 0xA, 0);
            stageFlagResetFunnc(0x13, 0xB, 0);
            stageFlagResetFunnc(0x13, 0xC, 0);
            stageFlagResetFunnc(0x13, 0xD, 0);
            stageFlagResetFunnc(0x13, 0xF, 0);
            stageFlagResetFunnc(0x13, 0x10, 0);
            stageFlagResetFunnc(0x13, 0x11, 0);
            stageFlagResetFunnc(0x13, 0x12, 0);
            stageFlagResetFunnc(0x13, 0x13, 0);
            stageFlagResetFunnc(0x13, 0x14, 0);
            stageFlagResetSpecificFunnc(0x13, 0x15, 0x80);
            stageFlagResetFunnc(0x13, 0x16, 0);
            stageFlagResetFunnc(0x13, 0x17, 0);
            stageFlagResetFunnc(0x13, 0x1C, 0);
            stageFlagResetSpecificFunnc(0x13, 0x1D, 16);

            // Snowpeak
            if (libtp::tp::d_save::isSwitch_dSv_memBit(&dComIfG_gameInfo.save.save_file.mSave[8].temp_flags, 21)) {
                stageFlagResetFunnc(8, 3, 0);
                stageFlagResetFunnc(8, 0xD, 0);
                eventFlagResetFunnc(0x0002, false); eventFlagResetFunnc(0x0001, false);
                eventFlagResetFunnc(0x0120, false); eventFlagResetFunnc(0x0480, false);
                eventFlagResetFunnc(0x0020, false); eventFlagResetFunnc(0x0010, false);
                eventFlagResetFunnc(0x1110, false); eventFlagResetFunnc(0x1440, false);
                eventFlagResetFunnc(0x1420, false); eventFlagResetFunnc(0x0B20, false);
                eventFlagResetFunnc(0x0B10, false); eventFlagResetFunnc(0x0d10, false);
                eventFlagResetFunnc(0x0140, false); eventFlagResetFunnc(0x2208, false);
                eventFlagResetFunnc(0x2504, false); eventFlagResetFunnc(0x2502, false);
                eventFlagResetFunnc(0x3B40, false); eventFlagResetFunnc(0x3B20, false);
            }
            stageFlagResetSpecificFunnc(0x14, 1, 0x20);
            stageFlagResetFunnc(0x14, 2, 0);
            stageFlagResetFunnc(0x14, 3, 0);
            stageFlagResetFunnc(0x14, 8, 0);
            stageFlagResetSpecificFunnc(0x14, 9, 0x20);
            stageFlagResetFunnc(0x14, 0xA, 0);
            stageFlagResetFunnc(0x14, 0xB, 0);
            stageFlagResetFunnc(0x14, 0xC, 0);
            stageFlagResetFunnc(0x14, 0xD, 0);
            stageFlagResetFunnc(0x14, 0xE, 0);
            stageFlagResetFunnc(0x14, 0xF, 0);
            stageFlagResetSpecificFunnc(0x14, 0x10, 0x10);
            stageFlagResetFunnc(0x14, 0x11, 0);
            stageFlagResetFunnc(0x14, 0x12, 0);
            stageFlagResetSpecificFunnc(0x14, 0x13, 2);
            dComIfG_gameInfo.save.save_file.mSave[0x14].temp_flags.memoryFlags[0x14] &= ~0x40;
            dComIfG_gameInfo.save.save_file.mSave[0x14].temp_flags.memoryFlags[0x14] &= ~0x10;
            dComIfG_gameInfo.save.save_file.mSave[0x14].temp_flags.memoryFlags[0x14] &= ~8;
            dComIfG_gameInfo.save.save_file.mSave[0x14].temp_flags.memoryFlags[0x14] &= ~4;
            dComIfG_gameInfo.save.save_file.mSave[0x14].temp_flags.memoryFlags[0x14] &= ~2;
            dComIfG_gameInfo.save.save_file.mSave[0x14].temp_flags.memoryFlags[0x15] &= ~1;
            dComIfG_gameInfo.save.save_file.mSave[0x14].temp_flags.memoryFlags[0x15] &= ~0x80;
            dComIfG_gameInfo.save.save_file.mSave[0x14].temp_flags.memoryFlags[0x15] &= ~0x40;
            dComIfG_gameInfo.save.save_file.mSave[0x14].temp_flags.memoryFlags[0x15] &= ~0x20;
            stageFlagResetFunnc(0x14, 0x16, 0);
            stageFlagResetFunnc(0x14, 0x17, 0);
            stageFlagResetFunnc(0x14, 0x1C, 0);
            stageFlagResetSpecificFunnc(0x14, 0x1D, 16);
            // Have to reset heart container to prevent softlock when re-doing Snowpeak
            if (dComIfG_gameInfo.save.save_file.mSave[0x14].temp_flags.memoryFlags[0x1D] & 0x10) {
                dComIfG_gameInfo.save.save_file.mSave[0x14].temp_flags.memoryFlags[0x1D] &= ~0x10;
                dComIfG_gameInfo.save.save_file.player.player_status_a.maxHealth -= 5;
            }

            // Temple of Time
            if (dComIfG_gameInfo.save.save_file.player.player_collect.mirror >= 3) {
                eventFlagResetFunnc(0x4004, false);
                stageFlagResetSpecificFunnc(7, 3, 2);
                dComIfG_gameInfo.save.save_file.mSave[7].temp_flags.memoryFlags[8] &= ~0x80;
                dComIfG_gameInfo.save.save_file.mSave[7].temp_flags.memoryFlags[9] &= ~0x80;
                dComIfG_gameInfo.save.save_file.mSave[7].temp_flags.memoryFlags[9] &= ~0x40;
                dComIfG_gameInfo.save.save_file.mSave[7].temp_flags.memoryFlags[9] &= ~0x20;
                dComIfG_gameInfo.save.save_file.mSave[7].temp_flags.memoryFlags[9] &= ~0x10;
                dComIfG_gameInfo.save.save_file.mSave[7].temp_flags.memoryFlags[0x16] &= ~0x10;
                dComIfG_gameInfo.save.save_file.mSave[7].temp_flags.memoryFlags[0x16] |= 8;
                dComIfG_gameInfo.save.save_file.mSave[7].temp_flags.memoryFlags[0x16] &= ~2;
                dComIfG_gameInfo.save.save_file.mSave[7].temp_flags.memoryFlags[0x16] &= ~1;
                dComIfG_gameInfo.save.save_file.mSave[7].temp_flags.memoryFlags[0xB] &= ~0x10;
                stageFlagResetFunnc(7, 0xF, 0);
            }
            stageFlagResetFunnc(0x15, 0, 0); stageFlagResetSpecificFunnc(0x15, 1, 0x10);
            stageFlagResetSpecificFunnc(0x15, 2, 0x10); stageFlagResetSpecificFunnc(0x15, 3, 0x20);
            stageFlagResetFunnc(0x15, 4, 0); stageFlagResetFunnc(0x15, 5, 0);
            stageFlagResetFunnc(0x15, 6, 0); stageFlagResetFunnc(0x15, 7, 0);
            dComIfG_gameInfo.save.save_file.mSave[0x15].temp_flags.memoryFlags[8] &= ~0x20;
            dComIfG_gameInfo.save.save_file.mSave[0x15].temp_flags.memoryFlags[8] &= ~0x10;
            dComIfG_gameInfo.save.save_file.mSave[0x15].temp_flags.memoryFlags[8] &= ~8;
            dComIfG_gameInfo.save.save_file.mSave[0x15].temp_flags.memoryFlags[8] &= ~4;
            dComIfG_gameInfo.save.save_file.mSave[0x15].temp_flags.memoryFlags[9] &= ~8;
            dComIfG_gameInfo.save.save_file.mSave[0x15].temp_flags.memoryFlags[9] &= ~4;
            stageFlagResetSpecificFunnc(0x15, 0xA, 8); stageFlagResetFunnc(0x15, 0xB, 0);
            stageFlagResetFunnc(0x15, 0xC, 0); stageFlagResetFunnc(0x15, 0xD, 0);
            stageFlagResetFunnc(0x15, 0xE, 0); stageFlagResetFunnc(0x15, 0xF, 0);
            stageFlagResetFunnc(0x15, 0x10, 0); stageFlagResetFunnc(0x15, 0x11, 0);
            stageFlagResetFunnc(0x15, 0x12, 0); stageFlagResetFunnc(0x15, 0x13, 0);
            stageFlagResetFunnc(0x15, 0x14, 0); stageFlagResetFunnc(0x15, 0x15, 0);
            stageFlagResetFunnc(0x15, 0x16, 0); stageFlagResetFunnc(0x15, 0x17, 0);
            stageFlagResetFunnc(0x15, 0x1C, 0); stageFlagResetSpecificFunnc(0x15, 0x1D, 16);

            // City in the sky
            if (dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x21] & 2) {
                //libtp::tp::d_save::offSwitch_dSv_memBit(&dComIfG_gameInfo.save.save_file.mSave[3].temp_flags, 0x61);
                eventFlagResetFunnc(0x2204, false);
                eventFlagResetFunnc(0x2280, false);
                eventFlagResetFunnc(0x2320, false);
                eventFlagResetFunnc(0x2540, false);
                eventFlagResetFunnc(0x2580, false);
                eventFlagResetFunnc(0x2340, false);
                eventFlagResetFunnc(0x2b80, false);
                eventFlagResetFunnc(0x2b04, false);
                eventFlagResetFunnc(0x2e08, false);
                eventFlagResetFunnc(0x3120, false);
                eventFlagResetFunnc(0x3b08, false);
                eventFlagResetFunnc(0x5F08, false);
                eventFlagResetFunnc(0x5F10, false);
                eventFlagResetFunnc(0x5F20, false);
                eventFlagResetFunnc(0x5F80, false);
                eventFlagResetFunnc(0x6004, false);
                eventFlagResetFunnc(0x6008, false);
                eventFlagResetFunnc(0x6010, false);
                eventFlagResetFunnc(0x6020, false);
                eventFlagResetFunnc(0x6040, false);
                eventFlagResetFunnc(0x6080, false);
                
                dComIfG_gameInfo.save.save_file.mSave[6].temp_flags.memoryFlags[2] &= ~0x80;
                dComIfG_gameInfo.save.save_file.mSave[6].temp_flags.memoryFlags[2] &= ~8;
                dComIfG_gameInfo.save.save_file.mSave[6].temp_flags.memoryFlags[3] &= ~0x10;
                stageFlagResetFunnc(6, 0x11, 0);
                dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[8] &= ~4;
                dComIfG_gameInfo.save.save_file.mSave[2].temp_flags.memoryFlags[0xB] &= ~0x10;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[9] &= ~2;
                //dComIfG_gameInfo.save.save_file.mSave[4].temp_flags.memoryFlags[0x16] &= ~0x80;
                dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0xE] &= ~0x40;
                dComIfG_gameInfo.save.save_file.mSave[6].temp_flags.memoryFlags[8] &= ~0x10;
                dComIfG_gameInfo.save.save_file.mSave[6].temp_flags.memoryFlags[0xB] &= ~4;
                dComIfG_gameInfo.save.save_file.mSave[6].temp_flags.memoryFlags[0x13] &= ~1;
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[0xF] &= ~4;
                dComIfG_gameInfo.save.save_file.mSave[0xA].temp_flags.memoryFlags[0xF] &= ~2;
                /*libtp::tp::d_save::offFirstBit(&dComIfG_gameInfo.save.save_file.player.player_get_item, 0x82);
                libtp::tp::d_save::offFirstBit(&dComIfG_gameInfo.save.save_file.player.player_get_item, 0x83);
                libtp::tp::d_save::offFirstBit(&dComIfG_gameInfo.save.save_file.player.player_get_item, 0x84);
                libtp::tp::d_save::offFirstBit(&dComIfG_gameInfo.save.save_file.player.player_get_item, 0xe9);
                libtp::tp::d_save::offFirstBit(&dComIfG_gameInfo.save.save_file.player.player_get_item, 0xeb);*/
            }
            libtp::tp::d_save::setItem(&dComIfG_gameInfo.save.save_file.player.player_item, 21, 0xFF);
            libtp::tp::d_save::setItem(&dComIfG_gameInfo.save.save_file.player.player_item, 22, 0xFF);

            stageFlagResetFunnc(0x16, 0, 0);
            stageFlagResetFunnc(0x16, 1, 0);
            stageFlagResetSpecificFunnc(0x16, 2, 2);
            stageFlagResetSpecificFunnc(0x16, 3, 0x40);
            stageFlagResetFunnc(0x16, 8, 0);
            stageFlagResetFunnc(0x16, 9, 0);
            stageFlagResetFunnc(0x16, 0xA, 0); stageFlagResetFunnc(0x16, 0xB, 0);
            stageFlagResetSpecificFunnc(0x16, 0xC, 1);
            stageFlagResetFunnc(0x16, 0xD, 0); stageFlagResetFunnc(0x16, 0xE, 0);
            stageFlagResetFunnc(0x16, 0x10, 0); stageFlagResetFunnc(0x16, 0x12, 0);
            stageFlagResetFunnc(0x16, 0x13, 0);
            stageFlagResetFunnc(0x16, 0x15, 0); stageFlagResetFunnc(0x16, 0x17, 0);
            stageFlagResetFunnc(0x16, 0x1C, 0); stageFlagResetSpecificFunnc(0x16, 0x1D, 16);
            if (saveInfoPtr->zButtonSloot == 21 || saveInfoPtr->zButtonSloot == 22) {
                saveInfoPtr->zButtonnItemm = 0xFF;
                zButtonTextureMomentInit(0xFF);
            }

            // Palace of Twilight and Hyrule Castle
            if (dComIfG_gameInfo.save.save_file.player.player_collect.mirror >= 15) {
                eventFlagResetFunnc(0x4480, false); eventFlagResetFunnc(0x4440, false);
            }
            
            eventFlagResetFunnc(0x4420, false);
            eventFlagResetFunnc(0x4410, false); eventFlagResetFunnc(0x4408, false); eventFlagResetFunnc(0x4404, false);
            eventFlagResetFunnc(0x4402, false); eventFlagResetFunnc(0x4401, false); eventFlagResetFunnc(0x4580, false);
            eventFlagResetFunnc(0x4680, false); eventFlagResetFunnc(0x5410, false);

            eventFlagResetFunnc(0x4208, false);
            stageFlagResetFunnc(0x18, 0, 0); stageFlagResetFunnc(0x18, 1, 0); stageFlagResetFunnc(0x18, 2, 0);
            stageFlagResetFunnc(0x18, 3, 0); stageFlagResetFunnc(0x18, 7, 0);
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[8] &= ~8;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[8] &= ~0x10;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[9] &= ~0x20;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[9] &= ~0x10;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[9] &= ~4;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[9] &= ~2;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0xA] &= ~0x20;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0xA] &= ~0x10;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0xA] &= ~4;
            stageFlagResetFunnc(0x18, 0xB, 0); stageFlagResetFunnc(0x18, 0xC, 0);
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0xD] &= ~0x80;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0xD] &= ~0x40;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0xD] &= ~8;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0xD] &= ~4;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0xE] &= ~8;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0xE] &= ~0x10;
            stageFlagResetFunnc(0x18, 0xF, 0); dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x10] &= ~0x80;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x10] &= ~0x40;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x10] &= ~2;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x10] &= ~1;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x11] &= ~0x80;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x11] &= ~0x10;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x11] &= ~2;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x12] &= ~0x80;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x12] &= ~0x40;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x12] &= ~0x10;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x12] &= ~8;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x12] &= ~4;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x13] &= ~8;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x13] &= ~2;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x14] &= ~0x40;
            dComIfG_gameInfo.save.save_file.mSave[0x18].temp_flags.memoryFlags[0x14] &= ~0x10;
            stageFlagResetSpecificFunnc(0x18, 0x15, 4); stageFlagResetSpecificFunnc(0x18, 0x16, 1);
            stageFlagResetSpecificFunnc(0x18, 0x17, 0x80); stageFlagResetFunnc(0x18, 0x1C, 0);
            stageFlagResetSpecificFunnc(0x18, 0x1D, 16);
            libtp::tp::d_save::offEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.mTmp, 0x0C01);
            libtp::tp::d_save::offEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.mTmp, 0x0C02);
            libtp::tp::d_save::offEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.mTmp, 0x0C04);
            dComIfG_gameInfo.save.save_file.mSave[9].temp_flags.memoryFlags[0xA] &= ~0x80;
            stageFlagResetSpecificFunnc(0x17, 0, 0x40); stageFlagResetFunnc(0x17, 1, 0);
            stageFlagResetFunnc(0x17, 2, 0); stageFlagResetSpecificFunnc(0x17, 3, 1);
            stageFlagResetFunnc(0x17, 7, 0); stageFlagResetFunnc(0x17, 8, 0);
            stageFlagResetFunnc(0x17, 9, 0); stageFlagResetFunnc(0x17, 0xA, 0);
            stageFlagResetSpecificFunnc(0x17, 0xB, 2);
            dComIfG_gameInfo.save.save_file.mSave[0x17].temp_flags.memoryFlags[0xC] &= ~8;
            dComIfG_gameInfo.save.save_file.mSave[0x17].temp_flags.memoryFlags[0xC] &= ~0x20;
            dComIfG_gameInfo.save.save_file.mSave[0x17].temp_flags.memoryFlags[0xC] &= ~0x40;

            // Light Master Sword Cutscene flag
            //dComIfG_gameInfo.save.save_file.mSave[0x17].temp_flags.memoryFlags[0xC] &= ~0x80;
            
            stageFlagResetSpecificFunnc(0x17, 0xD, 1);
            dComIfG_gameInfo.save.save_file.mSave[0x17].temp_flags.memoryFlags[0xE] &= ~1;
            dComIfG_gameInfo.save.save_file.mSave[0x17].temp_flags.memoryFlags[0xE] &= ~2;
            dComIfG_gameInfo.save.save_file.mSave[0x17].temp_flags.memoryFlags[0xF] &= ~0x20;
            dComIfG_gameInfo.save.save_file.mSave[0x17].temp_flags.memoryFlags[0xF] &= ~0x10;
            dComIfG_gameInfo.save.save_file.mSave[0x17].temp_flags.memoryFlags[0xF] &= ~8;
            stageFlagResetFunnc(0x17, 0x10, 0); stageFlagResetFunnc(0x17, 0x11, 0);
            stageFlagResetSpecificFunnc(0x17, 0x12, 4); stageFlagResetSpecificFunnc(0x17, 0x13, 2);
            stageFlagResetFunnc(0x17, 0x15, 0); stageFlagResetSpecificFunnc(0x17, 0x16, 0x10);
            stageFlagResetFunnc(0x17, 0x17, 0); stageFlagResetFunnc(0x17, 0x1C, 0);
            stageFlagResetSpecificFunnc(0x17, 0x1D, 16);
            eventFlagResetFunnc(0x2640, false); eventFlagResetFunnc(0x2620, false);
            eventFlagResetFunnc(0x2610, false); eventFlagResetFunnc(0x2608, false);
            eventFlagResetFunnc(0x2604, false); eventFlagResetFunnc(0x2602, false);
            eventFlagResetFunnc(0x2601, false); eventFlagResetFunnc(0x2780, false);
            eventFlagResetFunnc(0x2740, false); eventFlagResetFunnc(0x2720, false); eventFlagResetFunnc(0x2880, false);
            eventFlagResetFunnc(0x4304, false); eventFlagResetFunnc(0x4302, false); eventFlagResetFunnc(0x4301, false);
        }

        // Ammo
        timeResetUseed = 2;
        dComIfG_gameInfo.save.save_file.player.player_item_record.bow_ammo = 0;
        dComIfG_gameInfo.save.save_file.player.player_item_record.slingshot_ammo = 0;
        dComIfG_gameInfo.save.save_file.player.player_status_a.currentRupees = 0;
        for (uint8_t bomVarBagTemp = 0; bomVarBagTemp < 3; bomVarBagTemp++) {
            if (libtp::tp::d_save::getItem(&dComIfG_gameInfo.save.save_file.player.player_item, 15 + bomVarBagTemp, false) != 0xFF) {
                libtp::tp::d_save::setItem(&dComIfG_gameInfo.save.save_file.player.player_item, 15 + bomVarBagTemp, 0x50);
            }
            dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[bomVarBagTemp] = 0;
        }
        saveInfoPtr->nightOfTheee = 0; saveInfoPtr->timeRecordedReferr = 4320000;
        outOfTimeItsOver = false;
        //smallTimeLeftHurryyyyy = false;
    }

    //uint8_t susAboutSomething = 0;
    uint8_t counterForTheCountDownCountCount = 25;
    uint8_t counterForTheCountDownCountCount2 = 25;
    char bufferForTheBuffBuff[24];
    char bufferForThePythonnn[23];
    bool notifyMeAboutTheTimeRIGHTNOW = false;

    void counterToSkyAngleConverter(uint32_t paraam0) {
        int16_t minuteVarr = static_cast<uint8_t>(paraam0/60000);
        uint8_t dayCheckVarr = 0;
        if (minuteVarr > 47) dayCheckVarr = 48;
        else if (minuteVarr > 23) dayCheckVarr = 24;
        minuteVarr -= dayCheckVarr;
        //minuteVarr += 6;
        float minuteVarr2 = (float)minuteVarr;
        float secondVarrrr = (static_cast<float>(paraam0)/1000.0f - (static_cast<uint8_t>(paraam0/60000)*60));
        secondVarrrr = secondVarrrr/6.0f;
        //secondVarrrr = 10.f - secondVarrrr;
        minuteVarr2 = (minuteVarr2 + (secondVarrrr/10.0f));
        //if (minuteVarr2 >= (float)minuteVarr + 1.0f) minuteVarr2 -= 1.0f;
        minuteVarr2 *= 15.f;
        if (minuteVarr2 >= 90.0f) minuteVarr2 = 450.f - minuteVarr2;
        else minuteVarr2 = 90.0f - minuteVarr2;
        libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_b.skyAngle = minuteVarr2;
    }

    void notificationNightOfTheee(uint32_t param0, uint8_t knightOfThee) {
        bool letsDoThisThiinng = false;
        switch (knightOfThee) {
            // Night of the 1st day
            case 0:
            {
                if (param0 < 3540000) {
                    letsDoThisThiinng = true;
                }
                break;
            }
            // Dawn of the 2nd day
            case 1:
            {
                if (param0 < 2880000) {
                    letsDoThisThiinng = true;
                }
                break;
            }
            // Night of the 2nd day
            case 2:
            {
                if (param0 < 2100000) {
                    letsDoThisThiinng = true;
                }
                break;
            }
            // Dawn of the Final day
            case 3:
            {
                if (param0 < 1440000) {
                    letsDoThisThiinng = true;
                }
                break;
            }
            // Night of the Final day
            case 4:
            {
                if (param0 < 660000) {
                    letsDoThisThiinng = true;
                }
                break;
            }
            default:
            {
                break;
            }
        }
        if (letsDoThisThiinng) {
            saveInfoPtr->nightOfTheee = knightOfThee + 1;
            char micTessssst[6];
            char mikeTesssst[] = " of the ";
            char microTessst[11];
            if (knightOfThee == 0 || knightOfThee == 2 || knightOfThee == 4) strcpy(micTessssst, "Night");
            else strcpy(micTessssst, "Dawn");
            if (knightOfThee == 0) strcpy(microTessst, "First Day");
            else if (knightOfThee < 3) strcpy(microTessst, "Second Day");
            else strcpy(microTessst, "Final Day");
            strcpy(bufferForTheBuffBuff, micTessssst);
            strcat(bufferForTheBuffBuff, mikeTesssst);
            strcat(bufferForTheBuffBuff, microTessst);
            notifyMeAboutTheTimeRIGHTNOW = true;
            libtp::tp::d_a_alink::dMeter2Info_setFloatingMessage(2047, 90, true);
        }
    }

    bool timeSurrenderToProc = false;
    bool timeSurrenderToMinigame = false;

    void timerPauseOrKeepMoving() {
        if (saveInfoPtr->cycle3Days == 0) {
            if (libtp::tp::d_timer::dTimer_isStart()) {
                if (!gameplayStatus || timeSurrenderToProc) {
                    libtp::tp::d_com_inf_game::dComIfG_TimerStop(-1);
                }
            } else {
                if (gameplayStatus && !timeSurrenderToProc) {
                    libtp::tp::d_com_inf_game::dComIfG_TimerReStart(-1);
                }
            }
        }
        timeSurrenderToProc = false;
    }
    
    void handleQuickTransform()
    {
        using namespace libtp::tp::d_com_inf_game;

        // Ensure that Link is loaded on the map.
        libtp::tp::d_a_alink::daAlink* linkMapPtr = dComIfG_gameInfo.play.mPlayer;
        if (!linkMapPtr)
        {
            return;
        }

        // Ensure that link is not in a cutscene.
        if (libtp::tp::d_a_alink::checkEventRun(linkMapPtr))
        {
            return;
        }

        // Check to see if Link has the ability to transform.
        if (!libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0xD04))
        {
            return;
        }

        // Make sure Link isn't riding anything (horse, boar, etc.)
        if (libtp::tp::d_camera::checkRide(linkMapPtr))
        {
            return;
        }

        // Make sure Link is not underwater or talking to someone.
        switch (linkMapPtr->mProcID)
        {
            case libtp::tp::d_a_alink::PROC_TALK:
            case libtp::tp::d_a_alink::PROC_SWIM_UP:
            case libtp::tp::d_a_alink::PROC_SWIM_DIVE:
            {
                return;
            }
            // If Link is targeting or pulling a chain, we don't want to remove the ability to use items in combat accidently.
            case libtp::tp::d_a_alink::PROC_ATN_ACTOR_MOVE:
            case libtp::tp::d_a_alink::PROC_ATN_ACTOR_WAIT:
            case libtp::tp::d_a_alink::PROC_WOLF_ATN_AC_MOVE:
            {
                break;
            }
            default:
            {
                // Disable the input that was just pressed, as sometimes it could cause items to be used or Wolf Link to dig.
#ifndef PLATFORM_WII
                libtp::tp::m_do_controller_pad::cpadInfo[libtp::tp::m_do_controller_pad::PAD_1].mPressedButtonFlags = 0;
#else
                libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags = 0;
#endif
                break;
            }
        }

        // Ensure there is a proper pointer to the mMeterClass and mpMeterDraw structs in g_meter2_info.
        const libtp::tp::d_meter2::dMeter2_c* meterClassPtr = libtp::tp::d_meter2_info::g_meter2_info.mMeterClass;
        if (!meterClassPtr)
        {
            return;
        }

        const libtp::tp::d_meter2_draw::dMeter2Draw_c* meterDrawPtr = meterClassPtr->mpMeterDraw;
        if (!meterDrawPtr)
        {
            return;
        }

        // The game will crash if trying to quick transform while holding the Ball and Chain
        if (linkMapPtr->mEquipItem == libtp::data::items::Ball_and_Chain)
        {
            return;
        }

        // Use the game's default checks for if the player can currently transform
        if (!daMidna_c__checkMetamorphoseEnableBase(libtp::tp::d_a_player::m_midnaActor))
        {
            return;
        }

        // Check if the player has scared someone in the current area in wolf form
        if ((libtp::tp::d_kankyo::env_light.mEvilPacketEnabled & 0x80) != 0)
        {
            return;
        }

        libtp::tp::d_a_alink::procCoMetamorphoseInit(linkMapPtr);
    }

    void MinigameMapIdentificationFunc(int8_t param0Room, int8_t param1Layer, int16_t param2Point) {
        (void)param1Layer;
        if (libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x0C01) && !libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x0d04)) {
            timeSurrenderToMinigame = true;
            return;
        }
        if (param0Room == 7 && (param2Point == 3 || param2Point == 1)) {
            if (strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "R_SP161") == 0) timeSurrenderToMinigame = true;
        } else if (strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "F_SP128") == 0 || !strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "R_SP110")) {
            timeSurrenderToMinigame = true;
            return;
        } if (param0Room == 1 && param2Point == 2 && param1Layer == -1) {
            if (strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "F_SP104") == 0) {
                /*libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mPoint = 3;
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mRoomNo = 1;
                strcpy(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mStage, "F_SP108");
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.enabled = 1;
                if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerPtr != nullptr) {
                    libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerPtr->m_is_ready = 0;
                }*/
                timeReset3Day(false);
                return;
            }
        } if (param0Room == 2 && param1Layer == 14 && param2Point == 10) {
            if (!strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "F_SP121")) {
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_collect.crystal = 1;
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mLayer = 0xFF;
                eventFlagResetFunnc(0x4220, true);
                eventFlagResetFunnc(0x0601, true);
                eventFlagResetFunnc(0x0604, true);
                eventFlagResetFunnc(0x0620, true);
                eventFlagResetFunnc(0x0080, true);
                eventFlagResetFunnc(0x1410, true);
                stageFlagResetFunnc(3, 1, 1);
                stageFlagResetFunnc(3, 2, 0xDF);
                stageFlagResetFunnc(3, 3, 0xFF);
                stageFlagResetFunnc(3, 8, 0x8D);
                stageFlagResetFunnc(3, 9, 0x31);
                stageFlagResetFunnc(3, 0xA, 0xF7);
                stageFlagResetFunnc(3, 0xB, 0xCF);
                stageFlagResetFunnc(3, 0xC, 0x99);
                stageFlagResetFunnc(3, 0xD, 0xE7);
                stageFlagResetFunnc(3, 0xE, 0x9E);
                stageFlagResetFunnc(3, 0xF, 0xBF);
                stageFlagResetFunnc(3, 0x10, 0x14);
                stageFlagResetFunnc(3, 0x11, 6);
                stageFlagResetFunnc(3, 0x12, 0x7F);
                stageFlagResetFunnc(3, 0x13, 0x22);
                stageFlagResetFunnc(3, 0x14, 0xA1);
                stageFlagResetFunnc(3, 0x15, 0xC);
                stageFlagResetFunnc(3, 0x16, 0x40);
                stageFlagResetFunnc(3, 0x17, 0x14);
                eventFlagResetFunnc(0x0a04, true);
                libtp::tp::d_save::onSwitch_dSv_memBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[3].temp_flags, 10);

                currentFlagReseetFunnc(8, 1);
                currentFlagReseetFunnc(9, 0xB1);
                currentFlagReseetFunnc(0xB, 0x20);
                currentFlagReseetFunnc(0xD, 0x80);
                currentFlagReseetFunnc(0xF, 2);
                currentFlagReseetFunnc(0x16, 7);
                currentFlagReseetFunnc(0x17, 0x50);
                eventFlagResetFunnc(0x1a01, true);
                eventFlagResetFunnc(0x1b02, true);
                eventFlagResetFunnc(0x1c80, true);
                //libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mPoint = 5;
            }
        } else if (param0Room == 9 && param2Point == 10 && param1Layer == 14) {
            if (!strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "F_SP121")) {
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_collect.crystal = 3;
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mLayer = 0xFF;
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.light_drop.lanayru_tear_count = 16;
                libtp::tp::d_save::onSwitch_dSv_memBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[4].temp_flags, 2);
                libtp::tp::d_save::onSwitch_dSv_memBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[4].temp_flags, 10);
                libtp::tp::d_save::onSwitch_dSv_memBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[6].temp_flags, 3);
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_last_stay_info.player_last_region |= (1 << 4);
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x16] |= 2;
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x17] |= 0x40;
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[3].temp_flags.memoryFlags[0x17] |= 0x20;
                stageFlagResetFunnc(4, 4, 0x7E);
                stageFlagResetFunnc(4, 5, 0xE7);
                stageFlagResetFunnc(4, 6, 0xE0);
                stageFlagResetFunnc(4, 8, 0xD8);
                stageFlagResetFunnc(4, 9, 3);
                stageFlagResetFunnc(4, 0xA, 0xBF);
                stageFlagResetFunnc(4, 0xB, 0x87);
                stageFlagResetFunnc(4, 0xC, 1);
                stageFlagResetFunnc(4, 0xD, 0x75);
                stageFlagResetFunnc(4, 0xE, 0x31);
                stageFlagResetFunnc(4, 0xF, 0x6F);
                stageFlagResetFunnc(4, 0x11, 0xE8);
                stageFlagResetFunnc(4, 0x12, 0xC0);
                stageFlagResetFunnc(4, 0x14, 0x7F);
                stageFlagResetFunnc(4, 0x15, 0xF2);
                stageFlagResetFunnc(4, 0x16, 0x73);
                stageFlagResetFunnc(4, 0x17, 0x42);
                stageFlagResetFunnc(4, 0x18, 3);
                stageFlagResetFunnc(4, 0x18, 3);
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.memory.temp_flags.memoryFlags[3] |= 1;
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.memory.temp_flags.memoryFlags[3] |= 2;
                eventFlagResetFunnc(0x0840, true);
                eventFlagResetFunnc(0x0820, true);
                eventFlagResetFunnc(0x0810, true);
                eventFlagResetFunnc(0x0804, true);
                stageFlagResetFunnc(9, 6, 0x10);
                eventFlagResetFunnc(0x0880, true);
                eventFlagResetFunnc(0x0901, true);
                eventFlagResetFunnc(0x0e04, true);
                eventFlagResetFunnc(0x0e02, true);
                eventFlagResetFunnc(0x2101, true);
                eventFlagResetFunnc(0x4520, true);
                eventFlagResetFunnc(0x4b04, true);
                eventFlagResetFunnc(0x5180, true);
                eventFlagResetFunnc(0x5604, true);
                eventFlagResetFunnc(0x5C20, true);
                eventFlagResetFunnc(0x5d10, true);
                eventFlagResetFunnc(0x5e08, true);
                eventFlagResetFunnc(0x5e80, true);
                eventFlagResetFunnc(0x6001, true);
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.memory.temp_flags.memoryFlags[9] |= 8;
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.memory.temp_flags.memoryFlags[9] |= 4;
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.memory.temp_flags.memoryFlags[9] |= 2;
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.memory.temp_flags.memoryFlags[0x17] |= 8;
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.memory.temp_flags.memoryFlags[0x17] &= ~0x20;
                eventFlagResetFunnc(0x0A20, true);
                eventFlagResetFunnc(0x0F08, true);
                libtp::tp::d_item::execItemGet(0x31);
            }
        } else if (param0Room == 0 && ((param1Layer == 4 && param2Point == 2) || (param1Layer == -1 && param2Point == 1))) {
            if (!strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "R_SP01")) timeSurrenderToMinigame = true;
        } else if (param2Point == 3 && param1Layer == 4 && param0Room == 0) {
            if (!strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "F_SP00")) timeSurrenderToMinigame = true;
        } else if (libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x0701)) {
            if (!strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "F_SP109")) timeSurrenderToMinigame = true;
        } else if (!strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "F_SP200")) timeSurrenderToMinigame = true;
    }

    int32_t timerTextOver(libtp::tp::J2DTextBox::J2DTextBox* textBoxPtrr, char const* theeTextYouShallSeeeeeeee) {
        (void)theeTextYouShallSeeeeeeee;
        return textBoxPtrr->setString("TIME OVER");
    }
    
    void remainingCountDownTimerr() {
        if (saveInfoPtr->cycle3Days == 0 && !snowGameInOrOut() && !timeSurrenderToMinigame) {
            if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerPtr == nullptr) {
                if (counterForTheCountDownCountCount > 0) counterForTheCountDownCountCount--;
                else {
                    counterForTheCountDownCountCount = 25;
                    counterForTheCountDownCountCount2 = 25;
                    libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerMode = -1;
                    libtp::tp::d_timer::dTimer_createTimer(-1,saveInfoPtr->timeRecordedReferr,1,0,210.0f,410.0f,32.0f,419.0f);
                    //sprintf(sysConsolePtr->consoleLine[4].line, "time: %d", susAboutSomething); susAboutSomething++;
                }
            }
            if (timeResetUseed == 0) {
                if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerPtr != nullptr) {
                    if (counterForTheCountDownCountCount2 > 0) counterForTheCountDownCountCount2--;
                    else if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerNowTimeMs < 37) {
                        if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerNowTimeMs == 0) {
                            libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerMode = -1;
                            counterForTheCountDownCountCount = 25;
                            counterForTheCountDownCountCount2 = 25;
                            libtp::tp::d_com_inf_game::dComIfG_TimerStart(-1, 0);
                            //libtp::z2audiolib::z2seqmgr::subBgmStart(&libtp::z2audiolib::z2audiomgr::g_mDoAud_zelAudio.mSeqMgr, 0x1000049);
                            if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mPoint == 2 && libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mRoomNo == 1 && strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "F_SP104") == 0)
                                {libtp::tp::d_timer::createStart(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerPtr->mp_tm_scrn, 0x3e3); saveInfoPtr->nightOfTheee = 0;}
                        } libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerPtr->mp_tm_scrn->mTimerTransY = 150.0f;
                    }
                    saveInfoPtr->timeRecordedReferr = libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerLimitTimeMs - libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerNowTimeMs;
                    counterToSkyAngleConverter(saveInfoPtr->timeRecordedReferr);
                    notificationNightOfTheee(saveInfoPtr->timeRecordedReferr, saveInfoPtr->nightOfTheee);
                    if (outOfTimeItsOver && !libtp::tp::d_a_player::checkNoResetFlg2(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer, libtp::tp::d_a_player::FLG2_FORCE_GAMEOVER)) {
#ifdef PLATFORM_WII
                        libtp::patch::writeBranchBL(gameOverTeeeeeext + 0x274, timerTextOver);
#else
                        performStaticASMReplacement(gameOverCreatee + 0x88, 0x800D8088);
                       // performStaticASMReplacement(retry__NOPELOLOLOL + 0x4C, 0x38000024);
                        libtp::patch::writeBranchBL(gameOverTeeeeeext + 0x270, timerTextOver);
#endif
                        libtp::tp::d_a_player::onNoResetFlg2(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer, libtp::tp::d_a_player::FLG2_FORCE_GAMEOVER);
                        libtp::tp::d_a_alink::procCoDeadInit(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer, 0);
                    } else if (saveInfoPtr->timeRecordedReferr == 0) outOfTimeItsOver = true;
                    //sprintf(sysConsolePtr->consoleLine[4].line, "time: %d", libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerLimitTimeMs);
                }
            }
        }
    }

    void* getInf1Ptr(const char* file)
    {
        uint32_t infPtrRaw = reinterpret_cast<uint32_t>(
            libtp::tp::JKRArchivePub::JKRArchivePub_getGlbResource(/* ROOT */ 0x524F4F54, file, nullptr));

        // getGlbResource gets a pointer to MESGbmg1, but we need a pointer to INF1, which is just past MESGbmg1, and MESGbmg1
        // has a size of 0x20
        return reinterpret_cast<void*>(infPtrRaw + 0x20);
    }

    int32_t gettRoooooo()
    {
        return static_cast<int32_t>(libtp::tp::d_stage::mStayNo);
    }

    bool playerIsInRoomSeeee(int32_t room, const char* stage)
    {
        // Only check room if it is valid
        // Room numbers are normally stored as int8_t, so the highest positive value is 127
        if ((room < 0) || (room > 127))
        {
            return false;
        }

        if (room != gettRoooooo())
        {
            return false;
        }

        // Only check stage if it is valid
        if (!stage)
        {
            return false;
        }

        return libtp::tp::d_a_alink::checkStageName(stage);
    }

    // Remember the bank info
    uint8_t rememberThisValuableInformation = 0;

    uint8_t tempBankinfo[3] = {0, 0, 0};
    uint16_t rupeeTempBankInfo = 0;
    int8_t nowLine = 7;

    void bankSystemControllererererer(uint8_t actionDetectorDeviceUltraXL) {
        char moreCharButSmallerFileSizeProbably[] = "-> ";
        // Are you depositing or withdrawing?
        if (actionDetectorDeviceUltraXL < 2) {
            char whereThePythonAtBro[] =  "Deposit";
            char WHERETHEPYTHONATBROOOOO[] = "Withdraw";
            if (!actionDetectorDeviceUltraXL) {
                sprintf(sysConsolePtr->consoleLine[7].line, "%s%s", moreCharButSmallerFileSizeProbably, whereThePythonAtBro);
                sprintf(sysConsolePtr->consoleLine[8].line, "%s", WHERETHEPYTHONATBROOOOO);
                rememberThisValuableInformation = 0;
            } else {
                sprintf(sysConsolePtr->consoleLine[7].line, "%s", whereThePythonAtBro);
                sprintf(sysConsolePtr->consoleLine[8].line, "%s%s",  moreCharButSmallerFileSizeProbably, WHERETHEPYTHONATBROOOOO);
                rememberThisValuableInformation = 1;
            }
        } else {
            if (actionDetectorDeviceUltraXL == 2) {
                sprintf(bufferForThePythonnn, "Rupees:       <%d>", rupeeTempBankInfo);
            } else if (actionDetectorDeviceUltraXL == 3) {
                sprintf(bufferForThePythonnn, "Arrows:       <%d>", tempBankinfo[0]);
            } else if (actionDetectorDeviceUltraXL == 4) {
                sprintf(bufferForThePythonnn, "Normal Bombs: <%d>", tempBankinfo[1]);
            } else if (actionDetectorDeviceUltraXL == 5) {
                sprintf(bufferForThePythonnn, "Water Bombs:  <%d>", tempBankinfo[2]);
            }
            sprintf(sysConsolePtr->consoleLine[nowLine].line, "%s%s", moreCharButSmallerFileSizeProbably, bufferForThePythonnn);
        }
    }

    uint8_t mainMapTimerrr = 0;

#ifndef PLATFORM_WII
    int32_t RButtonThingy(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        if (libtp::tp::m_do_controller_pad::cpadInfo[0].mHoldLockR && linkActrPtr->mEquipItem != 0x40 && saveInfoPtr->maskID != 2)
            return libtp::tp::d_a_alink::itemTriggerCheck(linkActrPtr, 8);
        else if (linkActrPtr->mEquipItem == 0x40)
            return libtp::tp::d_a_alink::itemTriggerCheck(linkActrPtr, 0x40);
        return false;
    }
    int32_t BButtonThingy(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        if (libtp::tp::m_do_controller_pad::cpadInfo[0].mHoldLockR == 0 && saveInfoPtr->maskID != 2)
            return libtp::tp::d_a_alink::itemTriggerCheck(linkActrPtr, 8);
        return false;
    }

    uint8_t temprtemErrorr[3][2] = {
        {1, 2},
        {0, 2},
        {0, 1}
    };

    void itemZButtonChangeeeed(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        if (linkActrPtr->mProcID != 0x92 && linkActrPtr->mEquipItem != 0xFF && !libtp::tp::d_a_alink::checkEquipAnime(linkActrPtr)) {
            //uint8_t temp_r00;
            for (uint8_t temprorarrry = 0; temprorarrry < 3; temprorarrry++) {
                if (linkActrPtr->mEquipItem == libtp::tp::d_com_inf_game::dComIfGp_getSelectItem(temprorarrry) && ((linkActrPtr->mEquipItem != libtp::tp::d_com_inf_game::dComIfGp_getSelectItem(temprtemErrorr[temprorarrry][0]) && linkActrPtr->mEquipItem != libtp::tp::d_com_inf_game::dComIfGp_getSelectItem(temprtemErrorr[temprorarrry][1])) || (linkActrPtr->mSelectItemId != temprtemErrorr[temprorarrry][0] && linkActrPtr->mSelectItemId != temprtemErrorr[temprorarrry][1]))) {
                    linkActrPtr->mSelectItemId = temprorarrry;
                }
            }
        }
    }

    void ammoItemZTime(uint8_t param0Itemm, uint8_t slotBaaag) {
        //sprintf(sysConsolePtr->consoleLine[3].line, "%d", slotBaaag);
        switch (param0Itemm)
        {
        case 0x43:
            sprintf(sysConsolePtr->consoleLine[2].line, "                                           %d", libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bow_ammo);
            break;
        case 0x4B:
            sprintf(sysConsolePtr->consoleLine[2].line, "                                           %d", libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.slingshot_ammo);
            break;
        case 0x4F:
        case 0x50:
        case 0x59:
        case 0x70:
        case 0x71:
        case 0x72:
            sprintf(sysConsolePtr->consoleLine[2].line, "                                           %d", libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[slotBaaag - 15]);
            break;
        default:
            strcpy(sysConsolePtr->consoleLine[2].line, "");
            break;
        }
    }

    /*void mapOpenClosedOrWhatIsItDudeTellMe() {
        if (libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x1D01)) {
            libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosY = -150.0f;
        } else {
            libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosY = 0.0f;
        }
    }*/

    int32_t miniMapTriggerCheckThing() {
        if ((libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 2) && !libtp::tp::d_menu_window::dMw_UP_TRIGGER()) {
            if (!libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x1D01)) {mainMapTimerrr = 0; /*mapOpenClosedOrWhatIsItDudeTellMe();*/ return 1;}
        }
        if (libtp::tp::m_do_controller_pad::cpadInfo[0].mButtonFlags & 2 && libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x1D01)) {
            //mapOpenClosedOrWhatIsItDudeTellMe();
            if (mainMapTimerrr < 15) mainMapTimerrr++;
            else {mainMapTimerrr = 0; return true;}
        }
        return false;
    }

    int32_t miniMapTriggerCheckThing2() {
        if ((libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 2) && !libtp::tp::d_menu_window::dMw_UP_TRIGGER()) {
            if (libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x1D01)) {mainMapTimerrr = 0; /*mapOpenClosedOrWhatIsItDudeTellMe();*/ return 1;}
        }
        if (libtp::tp::m_do_controller_pad::cpadInfo[0].mButtonFlags & 2 && !libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x1D01)) {
            //mapOpenClosedOrWhatIsItDudeTellMe();
            if (mainMapTimerrr < 15) mainMapTimerrr++;
            else {mainMapTimerrr = 0; /*mapOpenClosedOrWhatIsItDudeTellMe();*/ return true;}
        }
        return false;
    }

    int32_t midnaTriggerButtoooon(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        (void)linkActrPtr;
        return (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 1) != 0;
    }

    bool iconZButtonDimFunc(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        if ((libtp::tp::d_meter2_info::g_meter2_info.mMenuWindowClass->mpMenuRing != nullptr) || 
        ((saveInfoPtr->zButtonnItemm == 0x45 || saveInfoPtr->zButtonnItemm == 0x44 || saveInfoPtr->zButtonnItemm == 0x47) && (linkActrPtr->mProcID == 0xC4 || linkActrPtr->mProcID == 0xC5 || linkActrPtr->mProcID == 0xC7 || linkActrPtr->mProcID == 0xC8 || linkActrPtr->mProcID == 0xCA || linkActrPtr->mProcID == 0x15)) ||
        (saveInfoPtr->zButtonnItemm != 0xFF && linkActrPtr->mEquipItem != saveInfoPtr->zButtonnItemm && (linkActrPtr->mProcID == 0x15 || linkActrPtr->mProcID == 0x12 || linkActrPtr->mProcID == 0xA))) {
            return true;
        }
        return false;
    }
    bool patcheeeeeeeeeeeed = false;
    void patchDimOrNoPatchDimZZZZZ() {
        if (patcheeeeeeeeeeeed) {
            patcheeeeeeeeeeeed = false; performStaticASMReplacement(zButtonDimm + 0x3B4, 0x807D0314);
        } else {
            patcheeeeeeeeeeeed = true; libtp::patch::writeBranch(zButtonDimm + 0x3B4, zButtonDimm + 0x3E4);
        }
    }
    float itemZYPosMemory = 0.0f;
    void midnaIconRePositionYYYYYYYY() {
        if (libtp::tp::d_meter2_info::g_meter2_info.mMeterMap != nullptr) {
            float tempFloatMapSizeCheck = 0.0f;
            switch (static_cast<int16_t>(libtp::tp::d_meter2_info::g_meter2_info.mMeterMap->mSizeH))
            {
                case 144:
                    tempFloatMapSizeCheck = 152.0f;
                    itemZYPosMemory = 138.0f;
                    break;
                case 216:
                    tempFloatMapSizeCheck = 230.0f;
                    itemZYPosMemory = 208.0f;
                    break;
                default:
                    tempFloatMapSizeCheck = libtp::tp::d_meter2_info::g_meter2_info.mMeterMap->mSizeH + 6.0f;
                    itemZYPosMemory = libtp::tp::d_meter2_info::g_meter2_info.mMeterMap->mSizeH - 20.0f;
                    break;
            }
            libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosY = -tempFloatMapSizeCheck;
            libtp::tp::J2DPane::movePaneeClass(libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D69646F6E61), 515.0f, -350.0f + itemZYPosMemory);
            //itemZYPosMemory = tempFloatMapSizeCheck;
        }
    }
#else
    bool maskWaterUseBoolWii = false;
#endif

    void lineIsSetupSir(libtp::tp::d_save::dSv_player_item_c* itemPtrCMaybe) {
        (void)itemPtrCMaybe;

        using namespace libtp::tp::d_com_inf_game;

        static const uint8_t i_item_lst[24] = {0x0A, 0x08, 0x06, 0x02, 0x09, 0x04, 0x03, 0x00, 0x01, 0x17, 0x14, 0x05,
                                               0x0F, 0x10, 0x11, 0x0B, 0x0C, 0x0D, 0x0E, 0x13, 0x12, 0x16, 0x15, 0x7};
        int32_t i1 = 0;
        int32_t i2 = 0;
        libtp::tp::d_save::dSv_player_item_c* playerItemStructPtr = &dComIfG_gameInfo.save.save_file.player.player_item;
        uint8_t* playerItemSlots = &playerItemStructPtr->item_slots[0];

        for (; i1 < 24; i1++) // Clear all of the item slots.
        {
            playerItemSlots[i1] = 0xFF;
        }

        uint8_t* playerItem = &playerItemStructPtr->item[0];
        for (i1 = 0; i1 < 24; i1++) // Fill all of the item wheel slots with their respective items if gotten.
        {
            const uint32_t currentEntry = i_item_lst[i1];
            if (playerItem[currentEntry] != 0xFF)
            {
                playerItemSlots[i2] = static_cast<uint8_t>(currentEntry);
                i2++;
            }
        }
    }

    void masksHaveNames_HereAreTheNames(uint8_t itemParam1, libtp::tp::d_menu_ring::dMenu_Ring_c* ringMenuPtr) {
        char maskNameContainer[11];
        if (itemParam1 == 0xE1) strcpy(maskNameContainer, "Bunny Hood");
        else if (itemParam1 == 0xE2) strcpy(maskNameContainer, "Blast Mask");
        else if (itemParam1 == 0xE3) strcpy(maskNameContainer, "Stone Mask");
        else if (itemParam1 == 0xE4) strcpy(maskNameContainer, "Time Reset");
#if defined(TP_GJP) || defined(TP_WJP)
        ((libtp::tp::J2DTextBox::J2DTextBox*)(libtp::tp::J2DPane::searchhh(ringMenuPtr->mpCenterScreen, 0x6974656D5F6E3034)))->setString(maskNameContainer);
        ((libtp::tp::J2DTextBox::J2DTextBox*)(libtp::tp::J2DPane::searchhh(ringMenuPtr->mpCenterScreen, 0x6974656D5F6E3035)))->setString(maskNameContainer);
        ((libtp::tp::J2DTextBox::J2DTextBox*)(libtp::tp::J2DPane::searchhh(ringMenuPtr->mpCenterScreen, 0x6974656D5F6E3036)))->setString(maskNameContainer);
        ((libtp::tp::J2DTextBox::J2DTextBox*)(libtp::tp::J2DPane::searchhh(ringMenuPtr->mpCenterScreen, 0x6974656D5F6E3037)))->setString(maskNameContainer);
#else
        ((libtp::tp::J2DTextBox::J2DTextBox*)(libtp::tp::J2DPane::searchhh(ringMenuPtr->mpCenterScreen, 0x666974656D5F6E31)))->setString(maskNameContainer);
        ((libtp::tp::J2DTextBox::J2DTextBox*)(libtp::tp::J2DPane::searchhh(ringMenuPtr->mpCenterScreen, 0x666974656D5F6E32)))->setString(maskNameContainer);
        ((libtp::tp::J2DTextBox::J2DTextBox*)(libtp::tp::J2DPane::searchhh(ringMenuPtr->mpCenterScreen, 0x666974656D5F6E33)))->setString(maskNameContainer);
        ((libtp::tp::J2DTextBox::J2DTextBox*)(libtp::tp::J2DPane::searchhh(ringMenuPtr->mpCenterScreen, 0x666974656D5F6E34)))->setString(maskNameContainer);
#endif
    }

    /*void ironSpeeeeeeeeed() {
        if (saveInfoPtr->maskID == 1) {
            if (libtp::tp::d_a_alink::ironBootsVars.heavyStateSpeed == 1.0f) libtp::tp::d_a_alink::ironBootsVars.heavyStateSpeed = 0.4f;
            else libtp::tp::d_a_alink::ironBootsVars.heavyStateSpeed = 1.0f;
            libtp::gc_wii::os_cache::DCFlushRange(&libtp::tp::d_a_alink::ironBootsVars.heavyStateSpeed, sizeof(float));
        }
    }*/

    void bunnyHoodToggle(bool param0) {
        if (param0) {
            libtp::tp::d_a_alink::ironBootsVars.mWalkAnmSpeedMax = 4.0f;
            libtp::tp::d_a_alink::moveVars.mRunAnmSpeed = 3.0f;
            libtp::tp::d_a_alink::moveVars.mMaxSpeed = 34.5f;
            libtp::tp::d_a_alink::ironBallVarrs.mWalkAnmSpeedMax = 3.0f;
        } else {
            libtp::tp::d_a_alink::ironBootsVars.mWalkAnmSpeedMax = 1.8f;
            libtp::tp::d_a_alink::moveVars.mRunAnmSpeed = 1.5f;
            libtp::tp::d_a_alink::moveVars.mMaxSpeed = 23.0f;
            libtp::tp::d_a_alink::ironBallVarrs.mWalkAnmSpeedMax = 1.5f;
        }
        //libtp::gc_wii::os_cache::DCFlushRange(&libtp::tp::d_a_alink::ironBootsVars.heavyStateSpeed, sizeof(float));
    }

    bool noSwordLOL = false;
    bool colorUpDownAmog = false;
    int16_t colorRememberr = 0;

    void bubbleAttackContinue() {
        libtp::tp::d_a_alink::daAlink* linkActrPtr = libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer;
        for (countinng2 = timeRemaininng2; countinng2 >= 0; countinng2--) {
            if ((libtp::gc_wii::os_time::OSGetTime() - second2) <= static_cast<int64_t>(( countinng2 * (libtp::gc_wii::os::__OSBusClock/4)))) {
                //sprintf(sysConsolePtr->consoleLine[6].line, "Time Left: %lld", timeRemaininng2 - countinng2);
                timeUpdaate2 = timeRemaininng2 - countinng2;
            }
        }
        if (colorUpDownAmog && colorRememberr < 0) {
            colorRememberr++;
            //sprintf(sysConsolePtr->consoleLine[12].line, "WAAAAAAAAAAAAAAAAAAAA");
            linkActrPtr->field_0x32a0[0].r = colorRememberr; linkActrPtr->field_0x32a0[0].g = colorRememberr;
            linkActrPtr->field_0x32a0[1].r = colorRememberr; linkActrPtr->field_0x32a0[1].g = colorRememberr;
            if (colorRememberr >= 0) {
                colorRememberr = 0;
                linkActrPtr->field_0x32a0[0].r = colorRememberr; linkActrPtr->field_0x32a0[0].g = colorRememberr;
                linkActrPtr->field_0x32a0[1].r = colorRememberr; linkActrPtr->field_0x32a0[1].g = colorRememberr;
                colorUpDownAmog = false;
                if (linkActrPtr->mEquipItem == 0x103) libtp::tp::d_a_alink::deleteEquipItem(linkActrPtr, 0, 0);
            }
        } else if (!colorUpDownAmog && colorRememberr > -20) {
            //sprintf(sysConsolePtr->consoleLine[12].line, "YEEEEEEEEEE");
            colorRememberr--;
            linkActrPtr->field_0x32a0[0].r = colorRememberr; linkActrPtr->field_0x32a0[0].g = colorRememberr;
            linkActrPtr->field_0x32a0[1].r = colorRememberr; linkActrPtr->field_0x32a0[1].g = colorRememberr;
            if (colorRememberr <= -20) {
                colorRememberr = -20;
                linkActrPtr->field_0x32a0[0].r = colorRememberr; linkActrPtr->field_0x32a0[0].g = colorRememberr;
                linkActrPtr->field_0x32a0[1].r = colorRememberr; linkActrPtr->field_0x32a0[1].g = colorRememberr;
                colorUpDownAmog = true;
            }
        }
        //sprintf(sysConsolePtr->consoleLine[12].line, "colorR: %d", linkActrPtr->field_0x32a0[0].r);
        if ((libtp::gc_wii::os_time::OSGetTime() - second2) >= static_cast<int64_t>(( timeRemaininng2 * (libtp::gc_wii::os::__OSBusClock/4)))) {
            noSwordLOL = false;
            if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[0] == 0x2F) {
                
            }
            timeRemaininng2 = 30;
        strcpy(sysConsolePtr->consoleLine[6].line, "");
        }
    }

    void bubbleAttackInit() {
        libtp::tp::d_a_alink::daAlink* linkActrPtr = libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer;
        if (linkActrPtr->mEquipItem == 0x103) libtp::tp::d_a_alink::swordUnequip(linkActrPtr);
        noSwordLOL = true;
        second2 = libtp::gc_wii::os_time::OSGetTime();
        colorUpDownAmog = false;
        colorRememberr = 0;
    }

    /*bool PalaceOfDamageEnemiesOrSmthg() {
        return (strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "D_MNO8") == 0 || strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "D_MNO8B") == 0 || strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "D_MNO8C") == 0);
    }*/

    /*bool twilightMapTime(char const* param0, int32_t param1) {
        libtp::tp::d_com_inf_game::dComIfG_gameInfo.mWorldDark = 1;
        (void)param0;
        (void)param1;
        return true;
    }*/

    uint32_t dashAttackAdjustment = reinterpret_cast<uint32_t>(libtp::tp::d_a_alink::setCutDash);

    void Mod::init()
    {
        libtp::display::setConsole(true, 25);
        libtp::display::setConsoleColor(0, 0, 0, 0);

        uint32_t bottleFunc1 = reinterpret_cast<uint32_t>(libtp::tp::d_save::setBottleItemIn);
        uint32_t bottleFunc2 = reinterpret_cast<uint32_t>(libtp::tp::d_save::setEquipBottleItemIn);
        uint32_t bottleFunc3 = reinterpret_cast<uint32_t>(libtp::tp::d_save::checkBottle);
        uint32_t bottleFunc4 = reinterpret_cast<uint32_t>(libtp::tp::d_save::checkEmptyBottle);
        uint32_t maskDrawFlaags = reinterpret_cast<uint32_t>(libtp::tp::d_a_alink::drawLink);
        //uint32_t gameSceneExamm = reinterpret_cast<uint32_t>(libtp::tp::d_a_alink::changeGameScene);
        //uint32_t wideerScreeenn = reinterpret_cast<uint32_t>(libtp::tp::d_camera::init_phase2);
        //uint32_t cutNormalFuncASM = reinterpret_cast<uint32_t>(libtp::tp::d_a_alink::procCutNormalInit);

#ifndef PLATFORM_WII
        uint32_t newItemCheckk = reinterpret_cast<uint32_t>(libtp::tp::d_a_alink::checkItemChangeFromButton);
        uint32_t setItemZToTriggeeeer = reinterpret_cast<uint32_t>(libtp::tp::d_a_alink::checkSetItemTrigger);
        uint32_t setItemZToButtooooon = reinterpret_cast<uint32_t>(libtp::tp::d_a_alink::checkItemSetButton);
        uint32_t zButtonExecuteNowww = reinterpret_cast<uint32_t>(libtp::tp::d_a_alink::execute_);
        uint32_t spinnerWaitForZButton = reinterpret_cast<uint32_t>(libtp::tp::d_a_alink::procSpinnerWait);
        uint32_t dungeonWarpReadyAfterZItem = reinterpret_cast<uint32_t>(libtp::tp::d_a_alink::procDungeonWarpReadyInit);
        uint32_t unequipAllZEdition = reinterpret_cast<uint32_t>(libtp::tp::d_a_alink::allUnequip);
        performStaticASMReplacement(bottleFunc1 + 0x3C, 0x38000001);
        performStaticASMReplacement(bottleFunc2 + 0x60, 0x2800000b);
        performStaticASMReplacement(bottleFunc3 + 0x2C, 0x38000001);
        performStaticASMReplacement(bottleFunc4 + 0x08, 0x38000001);
        //performStaticASMReplacement(gameSceneExamm + 0x88, 0x38800000);
        //performStaticASMReplacement(gameSceneExamm + 0x90, 0x38C000FF);

        performStaticASMReplacement(newItemCheckk + 0x1C8, 0x28000003);
        performStaticASMReplacement(newItemCheckk + 0x2D4, 0x28000003);
        performStaticASMReplacement(newItemCheckk + 0x3B0, 0x2C030003);
        performStaticASMReplacement(setItemZToTriggeeeer + 0x80, 0x28000003);
        performStaticASMReplacement(setItemZToButtooooon + 0x58, 0x28000003);
        performStaticASMReplacement(setItemZToButtooooon + 0x60, 0x38600003);
        performStaticASMReplacement(zNewItemChangeExamination + 0x39C, 0x2C030003);
        performStaticASMReplacement(zNewItemChangeExamination + 0x5F0, 0x2C030003);
        performStaticASMReplacement(zNewItemChangeExamination + 0x6E0, 0x2C030003);
        performStaticASMReplacement(zButtonExecuteNowww + 0x160, 0x2C030003);
        performStaticASMReplacement(zButtonExecuteNowww + 0xCC4, 0x2C1B0003);
        performStaticASMReplacement(zButtonExecuteNowww + 0x21Ac, 0x2C1B0003);
        performStaticASMReplacement(spinnerWaitForZButton + 0x1C0, 0x2C1D0003);
        performStaticASMReplacement(dungeonWarpReadyAfterZItem + 0x54, 0x2C030003);
        performStaticASMReplacement(unequipAllZEdition + 0x78, 0x28000003);
        libtp::patch::writeBranch(maskDrawFlaags + 0x7B4, maskDrawFlaags + 0x7C4);
        libtp::patch::writeBranch(maskDrawFlaags + 0x7FC, maskDrawFlaags + 0x820);
        libtp::patch::writeBranch(maskDrawFlaags + 0x880, maskDrawFlaags + 0x8A4);
        libtp::patch::writeBranch(maskDrawFlaags + 0x8F0, maskDrawFlaags + 0x900);
#else
        performStaticASMReplacement(bottleFunc1 + 0x30, 0x38000001);
        performStaticASMReplacement(bottleFunc2 + 0x54, 0x2800000b);
        performStaticASMReplacement(bottleFunc3 + 0x24, 0x38000001);
        performStaticASMReplacement(bottleFunc4, 0x38000001);
        libtp::patch::writeBranch(maskDrawFlaags + 0x834, maskDrawFlaags + 0x844);
        libtp::patch::writeBranch(maskDrawFlaags + 0x884, maskDrawFlaags + 0x8A8);
        libtp::patch::writeBranch(maskDrawFlaags + 0x91C, maskDrawFlaags + 0x940);
        libtp::patch::writeBranch(maskDrawFlaags + 0x9A8, maskDrawFlaags + 0x9B8);
#endif
        //performStaticASMReplacement(wideerScreeenn + 0x178, 0xd01f00d0);
        //performStaticASMReplacement(cutNormalFuncASM + 0x114, ASM_NOP);

#ifdef TP_GEU
        uint32_t* patchMessageCalculation = reinterpret_cast<uint32_t*>(0x802395D8);
#elif defined TP_GJP
        uint32_t* patchMessageCalculation = reinterpret_cast<uint32_t*>(0x802398E0);
#elif defined TP_WUS2
        uint32_t* patchMessageCalculation = reinterpret_cast<uint32_t*>(0x80226318);
#elif defined TP_WJP
        uint32_t* patchMessageCalculation = reinterpret_cast<uint32_t*>(0x802268b4);
#else
        uint32_t* patchMessageCalculation = reinterpret_cast<uint32_t*>(0x80238F58);
#endif
        *patchMessageCalculation = ASM_NOP;
#ifdef PLATFORM_WII
        strcpy( sysConsolePtr->consoleLine[19].line, "-: Decrease by 10        +: Increase by 10");
#else
        strcpy( sysConsolePtr->consoleLine[19].line, "L: Decrease by 10        R: Increase by 10");
#endif
        strcpy( sysConsolePtr->consoleLine[20].line, "Navigation: D-Pad");
        strcpy( sysConsolePtr->consoleLine[21].line, "Accept: Press A          Cancel: Press B");
        strcpy( sysConsolePtr->consoleLine[23].line, "youtube.com/@captainkittyca2");
        sysConsolePtr->consoleLine[6].showLine = false;

        do_link_trampoline =
            libtp::patch::hookFunction(libtp::tp::dynamic_link::do_link ,[]( libtp::tp::dynamic_link::DynamicModuleControl* dmc ) {return gMod->procDoLink(dmc); } );
        
        return_do_unlink = libtp::patch::hookFunction(libtp::tp::dynamic_link::do_unlink, [](libtp::tp::dynamic_link::DynamicModuleControl* dmc) {return gMod->handle_do_unlink(dmc); } );

        shieldManually =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::getMainBckData, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, libtp::tp::d_a_alink::daAlink_ANM i_anmID) { return gMod->manualShield(linkActrPtr, i_anmID); } );
        unSheathShield =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::setShieldGuard, [](libtp::tp::d_a_alink::daAlink* linkActrPtr) { return gMod->unSheathingShield(linkActrPtr); } );
        //healthUIOn =
            //libtp::patch::hookFunction( libtp::tp::d_meter2_draw::setAlphaLifeAnimeMax, [](libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr) { return gMod->UICheck(dMeterDrawPtr); } );
        //offUI =
            //libtp::patch::hookFunction(libtp::tp::d_meter2_draw::setAlphaLifeAnimeMin ,[](libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr){ return gMod->UICheck2(dMeterDrawPtr);});
        return_executee =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::execute_, [](libtp::tp::d_a_alink::daAlink* linkActrPtr) { return gMod->executee(linkActrPtr);});
        return_drawMaskOrNot =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::drawLink, [](libtp::tp::d_a_alink::daAlink* linkActrPtr) { return gMod->maskDrawOrNoDraw(linkActrPtr); } );
        return_crouchhhh =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::procCrouch, [](libtp::tp::d_a_alink::daAlink* linkActrPtr) { return gMod->crouchhhhhTime(linkActrPtr); } );
        return_chargerCutInitiate =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::procCutTurnChargeInit, [](libtp::tp::d_a_alink::daAlink* linkActrPtr) { return gMod->cutTurnChargerInitiation(linkActrPtr); } );
        return_cutJumpInitialized =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::procCutJumpInit, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t airCutParaam) { return gMod->cutJumpManIniit(linkActrPtr, airCutParaam); } );
        return_grassWhistleGetItIt =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::procGrassWhistleGet, [](libtp::tp::d_a_alink::daAlink* linkActrPtr) { return gMod->grassWhistleGetItIt(linkActrPtr); } );
        return_grassWhistleWaitItIt =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::procGrassWhistleWait, [](libtp::tp::d_a_alink::daAlink* linkActrPtr) { return gMod->grassWhistleWaitItIt(linkActrPtr); } );
        layerNumCommonCommon_return =
            libtp::patch::hookFunction(libtp::tp::d_com_inf_game::getLayerNo_common_common, [](const char* param0, int32_t param1, int32_t param2) { return gMod->layerCommonCommonNumm(param0, param1, param2); } );
        //return_bootsMoveAnimatiooon =
            //libtp::patch::hookFunction( libtp::tp::d_a_alink::checkBootsMoveAnime, [](libtp::tp::d_a_alink::daAlink* param0, int32_t param1) { return gMod->bootsMovingAnimatioon(param0, param1); } );
        //return_wiiiiiiideScreeeeen =
            //libtp::patch::hookFunction( libtp::tp::d_camera::init_phase2, [](libtp::tp::d_camera::view_class* param0) { return gMod->isItAWideScreeeen(param0); } );
        //return_allowTheMasksPlease =
            //libtp::patch::hookFunction(libtp::tp::d_a_alink::checkSetItemTrigger, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) { return gMod->masksShallBeAllowed(linkActrPtr, param0); } );
        return_changedLink =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::changeLink, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) { return gMod->changedLinkLook(linkActrPtr, param0);});
        return_initThePlayer =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::playerInit, [](libtp::tp::d_a_alink::daAlink* linkActrPtr) { return gMod->playerInitialized(linkActrPtr); } );
        return_equipSword =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::swordEquip, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) { return gMod->swordEquipDecision(linkActrPtr, param0); } );
        return_wolfNow =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::setMetamorphoseModel, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) { return gMod->wolfNowComeOn(linkActrPtr, param0); } );
        return_swordMustBeSet =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::setSwordModel, [](libtp::tp::d_a_alink::daAlink* linkActrPtr) { return gMod->swordMustBeSet(linkActrPtr); } );
        return_zButtonRingWait =
            libtp::patch::hookFunction( libtp::tp::d_menu_ring::setActiveCursor, [](libtp::tp::d_menu_ring::dMenu_Ring_c* ringMenuPtr) { return gMod->zButtonItemRingWait(ringMenuPtr); } );
        //return_zItemSeleect2 =
            //libtp::patch::hookFunction( libtp::tp::d_menu_ring::setItemFromItemRing2, [](libtp::tp::d_menu_ring::dMenu_Ring_c* ringMenuPtr, int32_t param0, uint8_t param1) { return gMod->zItemSeleect2(ringMenuPtr, param0, param1); } );
        return_swordMustBeUnSet =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::offSwordModel, [](libtp::tp::d_a_alink::daAlink* linkActrPtr) { return gMod->swordMustBeUnSet(linkActrPtr); } );
        return_gameChangerr =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::changeGameScene, [](void* param0) { return gMod->gameChangerr(param0); } );
        //return_itemZChangeeeee =
            //libtp::patch::hookFunction( libtp::tp::d_a_alink::checkItemButtonChange, [](libtp::tp::d_a_alink::daAlink* linkActrPtr) { return gMod->itemZButtonChangeeeed(linkActrPtr); } );
        //talkCheck =
            //libtp::patch::hookFunction(libtp::tp::f_op_actor_mng::fopAcM_orderTalkEvent, [](void* linkActrPtr, void* a1, uint16_t a2, uint16_t a3){ return gMod->pauseTalk(linkActrPtr, a1, a2, a3);});
        initMap =
            libtp::patch::hookFunction( libtp::tp::d_stage::dStage_playerInit, [](void* stageDt, libtp::tp::d_stage::stage_dzr_header_entry* i_data, int32_t num, void* raw_data) { return gMod->mapInitialized(stageDt, i_data, num, raw_data); } );
        return_breakThatGuardNow =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::procGuardBreakInit, [](libtp::tp::d_a_alink::daAlink* linkActrPtr) { return gMod->guardHasBroke(linkActrPtr); } );
        return_regularDamageHurts =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::procDamageInit, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, void* objjjjj, int32_t unnnnnnk) { return gMod->dudeTheDamageHurts(linkActrPtr, objjjjj, unnnnnnk); } );
        return_damageSetIt =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::setDamagePoint, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t a1, int32_t a2, int32_t a3, int32_t a4) { return gMod->damagetheSettingPointMaybe(linkActrPtr, a1, a2, a3, a4); } );
        meterValuee =
            libtp::patch::hookFunction(libtp::tp::d_meter2::moveKantera, [](libtp::tp::d_meter2::dMeter2_c* dMeter2Ptr) { return gMod->valueOfTheMeter(dMeter2Ptr); } );
        return_messageSmthg =
            libtp::patch::hookFunction(libtp::tp::control::setMessageCode_inSequence, [](libtp::tp::control::TControl* control, const void* TProcessor, uint16_t unk3, uint16_t msgID) { return gMod->customMessageCode(control, TProcessor, unk3, msgID);});
            //libtp::patch::hookFunction(libtp::tp::d_kankyo::dKy_darkworld_stage_check, twilightMapTime);
        
        return_customItemSet =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::changeItemTriggerKeepProc, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, uint8_t param0, int32_t param1) { return gMod->customItemSetting(linkActrPtr, param0, param1); } );
        gotAnItem =
            libtp::patch::hookFunction(libtp::tp::d_item::execItemGet, [](uint8_t itemzz){ return gMod->gotIt(itemzz);});
        goTalkFunnc_return =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::procCoTalk, [](libtp::tp::d_a_alink::daAlink* linkActrPtr) { return gMod->goTalkFunnc(linkActrPtr); } );
        //return_singleAnimeIsSet =
            //libtp::patch::hookFunction( libtp::tp::d_a_alink::setUpperAnime, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, uint16_t anmID, libtp::tp::d_a_alink::daAlink_UPPER upperChoice, float param0, float param1, int16_t param2, float param3) { return gMod->animeSingleIsSet(linkActrPtr, anmID, upperChoice, param0, param1, param2, param3); } );
            libtp::patch::hookFunction( libtp::tp::d_menu_window::dMw_DOWN_TRIGGER, downNope);
            //libtp::patch::hookFunction( libtp::tp::d_menu_window::dMw_Z_TRIGGER, midnaTriggerButtoooon);
            libtp::patch::hookFunction( libtp::tp::d_a_alink::procCrouchInit, crouchStuff);
            libtp::patch::hookFunction( libtp::tp::d_save::setLineUpItem, lineIsSetupSir);
#ifndef PLATFORM_WII
            libtp::patch::hookFunction( libtp::tp::d_a_alink::spActionTrigger, RButtonThingy);
            libtp::patch::hookFunction( libtp::tp::d_a_alink::swordSwingTrigger, BButtonThingy);
            libtp::patch::hookFunction( libtp::tp::d_a_alink::checkItemButtonChange, itemZButtonChangeeeed);
        return_useOrNoUseBecauseItemZ =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::procNotUseItemInit, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) { return gMod->isThereEvenAUseForThis(linkActrPtr, param0); } );
        return_zButtonRingSelect =
            libtp::patch::hookFunction( libtp::tp::d_menu_ring::setItemFromItemRing, [](libtp::tp::d_menu_ring::dMenu_Ring_c* ringMenuPtr) { return gMod->zButtonItemRingSelect(ringMenuPtr); } );
        return_menuRingErasee =
            libtp::patch::hookFunction( libtp::tp::d_menu_ring::isCloseItemRing, [](libtp::tp::d_menu_ring::dMenu_Ring_c* param0) { return gMod->menuRingNowErasePlease(param0); } );
        return_getTheSelectedZItemGPGrandPrix =
            libtp::patch::hookFunction(libtp::tp::d_com_inf_game::dComIfGp_getSelectItem, [](int32_t param0) { return gMod->getTheSelectedGrandPrixForZ(param0); } );
        return_zButtonAlphaAdjustment =
            libtp::patch::hookFunction( libtp::tp::d_meter2_draw::setButtonIconMidonaAlpha, [](libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeter2DrawPtr, uint32_t param0) { return gMod->zButtonAlphaAdjustment(dMeter2DrawPtr, param0); } );
        return_bottleSettingsModel =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::setBottleModel, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, uint16_t param0) { return gMod->bottleModelSetting(linkActrPtr, param0); } );
        return_selectItemNumAddOnForZ =
            libtp::patch::hookFunction( libtp::tp::d_com_inf_game::dComIfGp_addSelectItemNum, [](int32_t param0, int16_t param1) { return gMod->zSelectItemAddOnn(param0, param1); } );
        return_alphaButtonMinExamination =
            libtp::patch::hookFunction( libtp::tp::d_meter2_draw::setAlphaButtonAnimeMin, [](libtp::tp::d_meter2_draw::dMeter2Draw_c* param0) { return gMod->alphaButtonMinExaminatioon(param0); } );
        return_alphaButtonMaxExamination =
            libtp::patch::hookFunction( libtp::tp::d_meter2_draw::setAlphaButtonAnimeMax, [](libtp::tp::d_meter2_draw::dMeter2Draw_c* param0) { return gMod->alphaButtonMaxExaminatioon(param0); } );
        return_drinkBottleZInitialized =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::procBottleDrinkInit, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, uint16_t param0) { return gMod->drinkBottleZInitializeed(linkActrPtr, param0); } );
        return_opeenBottleZInitialized =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::procBottleOpenInit, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, uint16_t param0) { return gMod->openBottleZInitialized(linkActrPtr, param0); } );
        mapDpadOn =
            libtp::patch::hookFunction( libtp::tp::d_meter2_draw::setAlphaButtonMapAnimeMax, [](libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr){ return gMod->dPadMapOn(dMeterDrawPtr);});
        mapDpadOff =
            libtp::patch::hookFunction( libtp::tp::d_meter2_draw::setAlphaButtonMapAnimeMin, [](libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterdrawPtr){ return gMod->dPadMapOfff(dMeterdrawPtr);});
        return_mapTheShow =
            libtp::patch::hookFunction( libtp::tp::d_com_inf_game::dComIfGp_maaapShow, []() { return gMod->mapReturnTheShow(); } );
        return_mapTheNotShow =
            libtp::patch::hookFunction( libtp::tp::d_com_inf_game::dComIfGp_maaapHide, []() { return gMod->mapReturnTheNotShow(); } );
        return_itmWaterUse =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::checkAcceptUseItemInWater, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, uint16_t param0) { return gMod->itemWaterTime(linkActrPtr, param0); } );
        
        return_vibCamStringSet =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::setVibString, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {return gMod->stringVibCamDude(optionsMenuPtr); } );

        return_soundPointerStringSet =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::setSoundString, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {return gMod->stringSoundPointerDude(optionsMenuPtr); } );

        return_vibCamMovee =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::vib_move, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) { return gMod->vibCamMoving(optionsMenuPtr); } );

        return_soundPointerMovee =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::sound_move, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) { return gMod->soundPointerMoving(optionsMenuPtr); } );
        
            libtp::patch::hookFunction( libtp::tp::d_a_alink::midnaTalkTrigger, midnaTriggerButtoooon);
            libtp::patch::hookFunction( libtp::tp::d_menu_window::dMw_LEFT_TRIGGER, miniMapTriggerCheckThing);
            libtp::patch::hookFunction( libtp::tp::d_menu_window::dMw_RIGHT_TRIGGER, miniMapTriggerCheckThing2);
#else
        return_hookShotItemWaterMasks =
            libtp::patch::hookFunction( libtp::tp::d_a_player::checkHookshotItem, [](int32_t param0) { return gMod->hookShotItemWaterMasks(param0); } );
        
        return_normalCutProcInit =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::procCutNormalInit, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) { return gMod->normalCutProcessOkDude(linkActrPtr, param0); } );
        
        return_initWiiCutTurnProc =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::procCutTurnInit, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0, int32_t param1) { return gMod->cutTurnWiiStuffInit(linkActrPtr, param0, param1); } );
        
        return_initWiiCutDaaashProc =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::setCutDash, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0, int32_t param1) { return gMod->cutDaaashWiiStuffInit(linkActrPtr, param0, param1); } );
        
        return_vibCamStringSet =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::setCameraString, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {return gMod->stringVibCamDude(optionsMenuPtr); } );

        return_soundPointerStringSet =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::setPointerString, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {return gMod->stringSoundPointerDude(optionsMenuPtr); } );

        return_vibCamMovee =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::camera_move, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) { return gMod->vibCamMoving(optionsMenuPtr); } );

        return_soundPointerMovee =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::pointer_move, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) { return gMod->soundPointerMoving(optionsMenuPtr); } );

        return_shortCutInitialization =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::shortcut_init, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) { return gMod->theShortCutInitializeed(optionsMenuPtr); } );
        
        return_calibrationInitialization =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::calibration_init, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) { return gMod->theCalibrationInitializeed(optionsMenuPtr); } );
#endif
        return_constFrameThing =
            libtp::patch::hookFunction( libtp::tp::f_ap_game::fapGm_Execute, []() { return gMod->procNewFrame(); } );
        return_newItemChangesEveryFrame =
            libtp::patch::hookFunction(libtp::tp::d_a_alink::checkNewItemChange, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, uint8_t param0) { return gMod->newItemChangesEveryFrame(linkActrPtr, param0); } );
        initLargeDamage =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::procCoLargeDamageInit, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0, int32_t param1, int16_t param2, int16_t param3, void* param4, int32_t param5) { return gMod->largeDamageInitial(linkActrPtr, param0, param1, param2, param3, param4, param5); } );
        maskWearPermissionReturn =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::checkCastleTownUseItem, [](uint16_t param0) { return gMod->permissionForMask(param0); } );
        return_jumpAutomaticInit =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::procAutoJumpInit, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) { return gMod->jumpAutomatcInit(linkActrPtr, param0); } );
        return_hangStartInitNow =
            libtp::patch::hookFunction( libtp::tp::d_a_alink::procHangFallStartInit, [](libtp::tp::d_a_alink::daAlink* linkActrPtr, void* param0) { return gMod->hangStartFallInit(linkActrPtr, param0); } );
        
        return_optionInitialize =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::optionInitialize, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {return gMod->initializeTheOption(optionsMenuPtr); } );

        return_attenStringSet =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::setAttenString, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {return gMod->stringAttentionDude(optionsMenuPtr); } );
        
        return_attenMovee =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::atten_move, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) { return gMod->attentionMoving(optionsMenuPtr); } );

        return_closedConfirmation =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::deleteOptionsMenu, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) { return gMod->closingConfirmation(optionsMenuPtr); } );

        return_moveOptionsMenu =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::optionsMenuMove, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) { return gMod->moveInTheOptionsMenu(optionsMenuPtr); } );

        return_openConfirmation =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::confirm_open_init, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) { return gMod->openingConfirmation(optionsMenuPtr); } );
        
        return_theClosedConfirmation =
            libtp::patch::hookFunction(libtp::tp::d_menu_option::confirm_close_init, [](libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) { return gMod->theClosingConfirmation(optionsMenuPtr); } );

        return_over0Retry =
            libtp::patch::hookFunction(libtp::tp::d_menu_save::retryQueston0, [](libtp::tp::d_menu_save::dMenu_save_c* menuSaveePtrr) { return gMod->retryOver0Hook(menuSaveePtrr); } );
        
        return_sceneNamee =
            libtp::patch::hookFunction(libtp::z2audiolib::z2scenemgr::setSceneName, [](libtp::z2audiolib::z2scenemgr::Z2SceneMgr* param0, char* param1, int32_t param2, int32_t param3) { return gMod->sceneNameGiveIt(param0, param1, param2, param3); } );
        
        return_itemTextureRead =
            libtp::patch::hookFunction( libtp::tp::d_meter2_info::readItemTexture, [](libtp::tp::d_meter2_info::G_Meter2_Info* g_meter2_info_ptr, uint8_t itemNo, void* param_1, 
                                libtp::tp::J2DPicture::J2DPicture* param_2, void* param_3, 
                                libtp::tp::J2DPicture::J2DPicture* param_4, void* param_5, 
                                libtp::tp::J2DPicture::J2DPicture* param_6, void* param_7, 
                                libtp::tp::J2DPicture::J2DPicture* param_8, int32_t param_9) { return gMod->reeadTheItemTexture(g_meter2_info_ptr, itemNo, param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9); } );
        gMod = this;
    }

    char titleMap[8] =  "F_SP102";
    char videoMap[8] =  "S_MV000";
    char option1[4] =   "ISG";
    char option2[12] =  "3-Day Cycle";
#ifdef PLATFORM_WII
    char option3[12] =  "D-pad Right";
#else
    char option3[11] =  "D-pad Down";
#endif
    char option6[14] =  "Armors Toggle";
    char option7[15] =  "Fast Transform";
    char option8[5] = "None";
    char option9[3] = "On";
    char option10[4] = "Off";
    char infoText1[23] = "Are these settings ok?";
    char infoText2[17] = "Cancel settings?";
    bool infoTextCheck = false;

    uint8_t firstTime = 0;
    bool firstTimeFirstPersoon = false;

    int32_t Mod::mapInitialized(void* stageDt, libtp::tp::d_stage::stage_dzr_header_entry* i_data, int32_t num, void* raw_data) {
        if ((blastNope == false || noSwordLOL) && libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer == nullptr) {
            blastNope = true;
            noSwordLOL = false;
            timeRemaining = 15; timeUpdate = 15; timeRemaininng2 = 30; timeUpdaate2 = 30;
                strcpy(sysConsolePtr->consoleLine[5].line, "");
                strcpy(sysConsolePtr->consoleLine[6].line, "");
        }
        crouch = 0;
        shieldCrouch = false;
        timeSurrenderToMinigame = false;
        //sprintf(sysConsolePtr->consoleLine[18].line, "Room: %d, spawn: %d, layer: %d, map: %s", libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mRoomNo, libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mPoint, libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mLayer, libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage);
        if (saveInfoPtr->cycle3Days == 0) MinigameMapIdentificationFunc(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mRoomNo, libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mLayer, libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mPoint);
        if (firstTime == 0 && strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, titleMap) != 0 && strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, videoMap) != 0 )
        {
            firstTime = 1;
            if (saveInfoPtr->MMMaskPack1ID != 27) {
                saveInfoPtr->rupeeeStoreePlace = 0;
                saveInfoPtr->bobombStorePlace[0] = 0;
                saveInfoPtr->bobombStorePlace[1] = 0;
                saveInfoPtr->bowwStorePlace = 0;
                saveInfoPtr->MMMaskPack1ID = 27;
                saveInfoPtr->dPadChoice = 0;
                saveInfoPtr->cycle3Days = 0;
                saveInfoPtr->maskID = 0;
                saveInfoPtr->MMGlitches = 0;
                saveInfoPtr->doubleDefencee = 0;
                saveInfoPtr->zButtonnItemm = 0xFF;
                saveInfoPtr->timeRecordedReferr = 4320000;
                libtp::tp::d_save::setItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 7, 0xE4);
                libtp::tp::d_save::setItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 12, 0xE3);
                libtp::tp::d_save::setItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 13, 0xE1);
                libtp::tp::d_save::setItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 14, 0xE2);
            }
            if (libtp::tp::d_com_inf_game::dComIfGs_isItemFirstBit(0x30)) {libtp::tp::d_save::offFirstBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_get_item, 0x30); saveInfoPtr->doubleDefencee = 1; libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[0] = 0x2f;}
        }
        else if (strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, titleMap) == 0) {firstTime = 0; firstTimeFirstPersoon = false; mainMapTimerrr = 0; /*zDimOrNope = false;*/}
        return initMap(stageDt, i_data, num, raw_data);
    }

    uint8_t maskMaterialss[3] = {8, 16, 10};

    void Mod::maskSelection(uint8_t theItemID, bool wear0, libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        uint8_t materialSel = theItemID - 0xE1;
#ifndef PLATFORM_WII
        libtp::tp::d_meter2_draw::dMeter2Draw_c* drawMeterrrrPtr = libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw;
#endif

        if (wear0) {
            //libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x725F69746D5F50)->showTex();
            //libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x725F69746D5F70)->hideTex();
            saveInfoPtr->maskID = materialSel + 1;
            libtp::tp::d_a_alink::seStartOnlyReverb(linkActrPtr, 0x2002D);
            if (!firstTimeFirstPersoon || saveInfoPtr->maskID == 1) libtp::tp::J3DShape::showMesh(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[maskMaterialss[materialSel]]->mShape);
            if (theItemID == 0xE2) {
#ifndef PLATFORM_WII
                libtp::tp::d_meter2_draw::changeTextureItemB(drawMeterrrrPtr, 0x70);
                if (!blastNope) iconBButtonDimFunc(1);
                else if (linkActrPtr->mEquipItem == 0x103) iconBButtonDimFunc(2);
#endif
            }
            else if (theItemID == 0xE1) {
                bunnyHoodToggle(true);
            }
            else if (theItemID == 0xE3) enemyNoticeSwitch(true);
        }
        else {
            saveInfoPtr->maskID = 0;
            libtp::tp::J3DShape::hideMesh(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[maskMaterialss[materialSel]]->mShape);
            libtp::tp::d_a_alink::seStartOnlyReverb(linkActrPtr, 0x2002D);
            if (theItemID == 0xE2) {
                iconBButtonDimFunc(0);
#ifndef PLATFORM_WII
                if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[1] == 0x28)
                    {libtp::tp::d_meter2_draw::changeTextureItemB(drawMeterrrrPtr, 0x28);}
                else if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[1] == 0x29)
                    {libtp::tp::d_meter2_draw::changeTextureItemB(drawMeterrrrPtr, 0x29);}
                else if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[1] == 0x3f)
                    {libtp::tp::d_meter2_draw::changeTextureItemB(drawMeterrrrPtr, 0x3f);}
                else if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[1] == 0x49)
                    {libtp::tp::d_meter2_draw::changeTextureItemB(drawMeterrrrPtr, 0x49);}
#endif
            }
            else if (theItemID == 0xE1) {
                bunnyHoodToggle(false);
            }
            else if (theItemID == 0xE3) enemyNoticeSwitch(false);
        }
    }

    void Mod::playerInitialized(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        return_initThePlayer(linkActrPtr);
        if (firstTime) {
            counterForTheCountDownCountCount = 25;
            counterForTheCountDownCountCount2 = 25;
            isgStart = false;
            timeResetUseed = 0;
            if (saveInfoPtr->cycle3Days == 0) counterToSkyAngleConverter(saveInfoPtr->timeRecordedReferr);
            /*if (saveInfoPtr->zButtonnItemm == 0xFF) zDimOrNope = true;
            else zDimOrNope = false;*/
#ifndef PLATFORM_WII
            //libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D69646F6E615F73)->hideTex();
            //libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x69746D52547374)->showTex();
            //libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6A5F6C6967687431)->hideTex();
            zButtonTextureMomentInit(saveInfoPtr->zButtonnItemm);
            if (!libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x1D01)) midnaIconRePositionYYYYYYYY();
#endif
            firstTimeFirstPersoon = false;
            mainMapTimerrr = 0;
            if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[0] == 0x2f && libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentForm == 0) {
                if (saveInfoPtr->maskID != 1) libtp::tp::J3DShape::hideMesh(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[8]->mShape);
                else if (saveInfoPtr->maskID == 1) {
                    bunnyHoodToggle(true);
                }
                if (saveInfoPtr->maskID == 2) {
                    libtp::tp::J3DShape::showMesh(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[16]->mShape);
#ifndef PLATFORM_WII
                    libtp::tp::d_meter2_draw::changeTextureItemB(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw, 0x70);
#endif
                    if (blastNope == false) {
                        blastNope = true;
                        if (linkActrPtr->mEquipItem == 0x103) iconBButtonDimFunc(2);
                        else iconBButtonDimFunc(0);
                        timeRemaining = 15; timeUpdate = 15;
                    strcpy(sysConsolePtr->consoleLine[5].line, "");
                    }
                }
                if (saveInfoPtr->maskID == 3) enemyNoticeSwitch(true);
                else if (saveInfoPtr->maskID != 3) libtp::tp::J3DShape::hideMesh(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[10]->mShape);
            }
            if (noSwordLOL) {
                noSwordLOL = false; timeRemaininng2 = 30; timeUpdaate2 = 30;
                strcpy(sysConsolePtr->consoleLine[6].line, "");
            }
        }
    }

    //uint8_t captureTheItemValueForAMomentThankYouVeryMuch = 0;

    int32_t Mod::customItemSetting(libtp::tp::d_a_alink::daAlink* linkActrPtr, uint8_t theItemID, int32_t param1) {
        /*if (theItemID == 2) {
            captureTheItemValueForAMomentThankYouVeryMuch = libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mSelectItem[theItemID];
            libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mSelectItem[theItemID] = saveInfoPtr->zButtonnItemm;
            performStaticASMReplacement(selectItemForZSelect + 0x30, ASM_NOP);
            performStaticASMReplacement(selectItemForZSelect + 0x44, ASM_NOP);
        }*/
        uint8_t itemTempVar = libtp::tp::d_com_inf_game::dComIfGp_getSelectItem(theItemID);
        // Will change 0xE3 to 0xE4 after adding stone mask
        if (itemTempVar > 0xE0 && itemTempVar < 0xE4) {
            if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[0] == 0x2f) {
                bool wearTheMaskBro = false;
                uint8_t toWearOrNotToWear = 0;
                for (uint8_t temporaryItemVar = 0xE1; temporaryItemVar < 0xE4; temporaryItemVar++) {
                    uint8_t materialSel = temporaryItemVar - 0xE1;
                    if (itemTempVar == temporaryItemVar) {
                        toWearOrNotToWear = temporaryItemVar;
                        if (linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[maskMaterialss[materialSel]]->mShape->mFlags) wearTheMaskBro = true;
                    } else {
                        if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer->field_0x064C->mMaterialTable.mMaterialNodePointer[maskMaterialss[materialSel]]->mShape->mFlags == 0) maskSelection(temporaryItemVar, false, linkActrPtr);
                    }
                }
                if (wearTheMaskBro) maskSelection(toWearOrNotToWear, true, linkActrPtr);
                else maskSelection(toWearOrNotToWear, false, linkActrPtr);
            }
            return 1;
        }
        if (itemTempVar == 0xE4) {
            param1 = 15;
            libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mPoint = 2;
            libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mLayer = -1;
            libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mRoomNo = 1;
            strcpy(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mStage, "F_SP104");
            libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.enabled = 1;
            if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerPtr != nullptr) {
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mTimerPtr->m_is_ready = 0;
            }
        }
        return return_customItemSet(linkActrPtr, theItemID, param1);
        /*const int32_t amongYouOrMeOrSomeone = return_customItemSet(linkActrPtr, theItemID, param1);
        if (amongYouOrMeOrSomeone && theItemID == 2) {
            performStaticASMReplacement(selectItemForZSelect + 0x30, 0x408200E0);
            performStaticASMReplacement(selectItemForZSelect + 0x44, 0x418200CC);
            libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mSelectItem[theItemID] = captureTheItemValueForAMomentThankYouVeryMuch;
        }
        return amongYouOrMeOrSomeone;*/
    }

    /*int32_t Mod::bootsMovingAnimatioon(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) {
        if (libtp::tp::d_a_alink::ironBootsVars.heavyStateSpeed == 1.0f) return false;
        return return_bootsMoveAnimatiooon(linkActrPtr, param0);
    }*/
    bool UiChecking = false;
    void Mod::gotIt(uint8_t itemzz) {
        //sprintf(sysConsolePtr->consoleLine[0].line, "item Get: 0x%x", itemzz);
        if (itemzz == 0x30) {saveInfoPtr->doubleDefencee = 1; return;}
        if (itemzz == 0x44) {
            if (libtp::tp::d_save::isFirstBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_get_item, 0x47)) return;
        }
#ifndef PLATFORM_WII
        if (UiChecking && itemzz > 9 && itemzz < 0x13) {
            bool visualizeAmmoOntoTheDisplay = false;
            if (itemzz < 0xE) {
                if (libtp::tp::d_a_player::checkBombItem(saveInfoPtr->zButtonnItemm) || saveInfoPtr->zButtonnItemm == 0x4f || saveInfoPtr->zButtonnItemm == 0x50) visualizeAmmoOntoTheDisplay = true;
            } else if (itemzz < 0x12) {
                if (saveInfoPtr->zButtonnItemm == 0x43) visualizeAmmoOntoTheDisplay = true;
            } else if (itemzz == 0x13) {
                if (saveInfoPtr->zButtonnItemm == 0x4B) visualizeAmmoOntoTheDisplay = true;
            }
            if (visualizeAmmoOntoTheDisplay) ammoItemZTime(saveInfoPtr->zButtonnItemm, saveInfoPtr->zButtonSloot);
        } else if (itemzz == 0x47) {
            if (saveInfoPtr->zButtonnItemm == 0x44) {
                saveInfoPtr->zButtonnItemm = 0xff;
                zButtonTextureMomentInit(0xFF);
            }
        }
#endif
        return gotAnItem(itemzz);
    }

    int32_t Mod::goTalkFunnc(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        timeSurrenderToProc = true;
        return goTalkFunnc_return(linkActrPtr);
    }
#ifndef PLATFORM_WII
    bool itemMenuZAnimate = false;
    uint8_t itemMenuZAnimate2 = 0;
    uint8_t reserveCurrentSlott = 0xE6;
    int16_t reserve_0x670 = 0;
    uint8_t declineButtoonnRequest = 4;
    bool checkFrameSkipVar = false;

    int32_t Mod::isThereEvenAUseForThis(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) {
        /*if (saveInfoPtr->zButtonnItemm == param0) {
            int16_t npcList = libtp::tp::d_a_alink::LockOnTarget(linkActrPtr->mAttention, 0)->mProcName;
            bool oneMoreChanceeeeeee = false;
            if (npcList == 0x278 && param0 == 0x90) oneMoreChanceeeeeee = true;
            if (oneMoreChanceeeeeee) {
                linkActrPtr->mDemo.mDemoMode = 0x25;
                linkActrPtr->mDemo.mParam1 = param0;
                linkActrPtr->mDemo.mParam0 = 2;
                libtp::tp::d_a_alink::checkDemoAction(linkActrPtr);
                libtp::tp::d_a_alink::procTradeItemOutInit(linkActrPtr);
                return 0;
            }
        }*/
        return return_useOrNoUseBecauseItemZ(linkActrPtr, param0);
    }
    void Mod::zButtonItemRingSelect(libtp::tp::d_menu_ring::dMenu_Ring_c* ringMenuPtr) {
        if ((libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x10)) return return_zButtonRingSelect(ringMenuPtr);
        if ((libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x400) || (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x800)) {
            uint8_t iteeemmmm = libtp::tp::d_save::getItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, ringMenuPtr->mItemSlots[ringMenuPtr->mCurrentSlot], false);
            if (iteeemmmm == saveInfoPtr->zButtonnItemm) {
                uint8_t xOrYItem = 0;
                if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x800) xOrYItem = 1;
                if (!xOrYItem) {
                    xOrYItem = ringMenuPtr->mItemSlots[ringMenuPtr->mXButtonSlot];
                } else xOrYItem = ringMenuPtr->mItemSlots[ringMenuPtr->mYButtonSlot];
                saveInfoPtr->zButtonnItemm = libtp::tp::d_save::getItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, xOrYItem, false);
                zButtonTextureMomentInit(saveInfoPtr->zButtonnItemm);
                saveInfoPtr->zButtonSloot = xOrYItem;
                ammoItemZTime(saveInfoPtr->zButtonnItemm, xOrYItem);
            }
        }
        return return_zButtonRingSelect(ringMenuPtr);
        //XYItemAnySet(0);
        //uint8_t iteeemmmm = libtp::tp::d_save::getItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, ringMenuPtr->mItemSlots[ringMenuPtr->mCurrentSlot], false);
        //if (iteeemmmm == saveInfoPtr->zButtonnItemm)
        //libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x725F69746D5F70)->yPositionTest(libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x725F69746D5F70)->yPositionCheck() - 100.0f);
    }
    bool Mod::menuRingNowErasePlease(libtp::tp::d_menu_ring::dMenu_Ring_c* param0) {
        if (itemMenuZAnimate) {
            itemMenuZAnimate = false; itemMenuZAnimate2 = 0;
            //libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconScale = 1.1f;
            //libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosX = 0.0f; libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosY = 0.0f;
            libtp::tp::J2DPane::movePaneeClass(libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D69646F6E61), 515.0f, -350.0f + itemZYPosMemory);
        }
        //if (param0->mOpenCloseFrames - 1 <= 0) zDimOrNope = false;
        return return_menuRingErasee(param0);
    }
    uint8_t Mod::getTheSelectedGrandPrixForZ(int32_t param0) {
        if (param0 != 2) return return_getTheSelectedZItemGPGrandPrix(param0);
        libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mSelectItem[param0] = saveInfoPtr->zButtonnItemm;
        performStaticASMReplacement(selectItemForZSelect + 0x30, ASM_NOP);
        performStaticASMReplacement(selectItemForZSelect + 0x44, ASM_NOP);

        const uint8_t amonganese = return_getTheSelectedZItemGPGrandPrix(param0);

        if (amonganese) {
            performStaticASMReplacement(selectItemForZSelect + 0x30, 0x408200E0);
            performStaticASMReplacement(selectItemForZSelect + 0x44, 0x418200CC);
        }
        return amonganese;
    }
    bool UiChecking2 = false;
    void Mod::zButtonAlphaAdjustment(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeter2DrawPtr, uint32_t param0) {
        if (UiChecking) {
            if (iconZButtonDimFunc(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer)) {
                //libtp::tp::d_pane_class::setAlphaPaneClasssss(dMeter2DrawPtr->mpButtonMidona, 255);
                libtp::tp::d_pane_class::setAlphaPaneClasssss(dMeter2DrawPtr->mpButtonXY[2], 255);
                if (itemMenuZAnimate) {dMeter2DrawPtr->field_0x724 = 1.5f; dMeter2DrawPtr->mZButtonAlpha = 1.5f; return return_zButtonAlphaAdjustment(dMeter2DrawPtr, param0);}
                //dMeter2DrawPtr->field_0x724 = 1.5f;
                //dMeter2DrawPtr->mZButtonAlpha = 1.5f;
                //libtp::tp::d_meter_hio::g_drawHIO.mZButtonAlpha = 1.5f;
                //libtp::tp::d_meter_hio::g_drawHIO.mZButtonItemBaseAlpha = 1.5f;
                if (!patcheeeeeeeeeeeed) patchDimOrNoPatchDimZZZZZ();
                //return;
            } else if (saveInfoPtr->zButtonnItemm == 0xFF) {
                if (!patcheeeeeeeeeeeed) patchDimOrNoPatchDimZZZZZ();
                
                //libtp::tp::d_pane_class::setAlphaPaneClasssss(dMeter2DrawPtr->mpButtonMidona, 85);
                libtp::tp::d_pane_class::setAlphaPaneClasssss(dMeter2DrawPtr->mpButtonXY[2], 85);
                //dMeter2DrawPtr->field_0x724 = 0.25f;
                //dMeter2DrawPtr->mZButtonAlpha = 0.25f;
                //libtp::tp::d_meter_hio::g_drawHIO.mZButtonAlpha = 0.25f;
                //libtp::tp::d_meter_hio::g_drawHIO.mZButtonItemBaseAlpha = 0.25f;
                //return;
            } else if (patcheeeeeeeeeeeed) patchDimOrNoPatchDimZZZZZ();
        } else if (patcheeeeeeeeeeeed) patchDimOrNoPatchDimZZZZZ();
        if (!UiChecking2) {
            if (libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D6E6E616132)->checkVisible()) {
                libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6A5F6C6967687431)->hideTex();
                libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D6E6E616132)->hideTex();
                libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D69646F6E615F73)->hideTex();
            }
            return;
        } else if (!libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D6E6E616132)->checkVisible()) {
            libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6A5F6C6967687431)->showTex();
            libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D6E6E616132)->showTex();
            libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D69646F6E615F73)->showTex();
        }
        return return_zButtonAlphaAdjustment(dMeter2DrawPtr, param0);
    }
    void Mod::bottleModelSetting(libtp::tp::d_a_alink::daAlink* linkActrPtr, uint16_t param0) {
        //sprintf(sysConsolePtr->consoleLine[6].line, "bottle: 0x%x", param0);
        if (saveInfoPtr->zButtonnItemm == param0) {
            saveInfoPtr->zButtonnItemm = 0x60;
            libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item.item[11] = saveInfoPtr->zButtonnItemm;
            zButtonTextureMomentInit(saveInfoPtr->zButtonnItemm);
            libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mSelectItem[2] = saveInfoPtr->zButtonnItemm;
        } else if (saveInfoPtr->zButtonnItemm == 0x60) {
            uint8_t tempU8Varrr = param0;
            if (param0 == 0x68) tempU8Varrr = 0x66;
            else if (param0 == 0x69) tempU8Varrr = 0x61;
            saveInfoPtr->zButtonnItemm = tempU8Varrr;
            libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item.item[11] = saveInfoPtr->zButtonnItemm;
            zButtonTextureMomentInit(saveInfoPtr->zButtonnItemm);
            libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mSelectItem[2] = saveInfoPtr->zButtonnItemm;
        }
        return return_bottleSettingsModel(linkActrPtr, param0);
    }
    void Mod::zSelectItemAddOnn(int32_t param0, int16_t param1) {
        if (param0 != 2 || (saveInfoPtr->zButtonnItemm != 0x59 && (saveInfoPtr->zButtonnItemm < 0x70 || saveInfoPtr->zButtonnItemm > 0x72)) ) return return_selectItemNumAddOnForZ(param0, param1);
        if (param0 == 2) {
            if (saveInfoPtr->zButtonnItemm > 0x6F && saveInfoPtr->zButtonnItemm < 0x73) {
                if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[saveInfoPtr->zButtonSloot - 15] > 0) {
                    libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mSelectItem[param0] = saveInfoPtr->zButtonnItemm;
                    libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[saveInfoPtr->zButtonSloot - 15] -= 1;
                    ammoItemZTime(saveInfoPtr->zButtonnItemm, saveInfoPtr->zButtonSloot);
                    if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[saveInfoPtr->zButtonSloot - 15] == 0) {
                        saveInfoPtr->zButtonnItemm = 0x50; zButtonTextureMomentInit(saveInfoPtr->zButtonnItemm);
                    }
                }
            }
        }
    }
    void Mod::alphaButtonMinExaminatioon(libtp::tp::d_meter2_draw::dMeter2Draw_c* param0) {
        if (UiChecking) {UiChecking = false; ammoItemZTime(0xFF, saveInfoPtr->zButtonSloot);}
        return return_alphaButtonMinExamination(param0);
    }

    void Mod::alphaButtonMaxExaminatioon(libtp::tp::d_meter2_draw::dMeter2Draw_c* param0) {
        if (!UiChecking) {UiChecking = true; ammoItemZTime(saveInfoPtr->zButtonnItemm, saveInfoPtr->zButtonSloot);}
        return return_alphaButtonMaxExamination(param0);
    }
    int32_t Mod::drinkBottleZInitializeed(libtp::tp::d_a_alink::daAlink* linkActrPtr, uint16_t param0) {
        const int32_t amogmoga = return_drinkBottleZInitialized(linkActrPtr, param0);
        if (amogmoga) {
            if (saveInfoPtr->zButtonnItemm == param0 || saveInfoPtr->zButtonnItemm == 0x60) {
                if (param0 == 0x64) saveInfoPtr->zButtonnItemm = 0x65;
                else saveInfoPtr->zButtonnItemm = 0x60;
                zButtonTextureMomentInit(saveInfoPtr->zButtonnItemm);
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mSelectItem[2] = saveInfoPtr->zButtonnItemm;
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item.item[11] = saveInfoPtr->zButtonnItemm;
                //sprintf(sysConsolePtr->consoleLine[6].line, "bottle: 0x%x", libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.mBottleNum[0]);
            }
        }
        return amogmoga;
    }
    int32_t Mod::openBottleZInitialized(libtp::tp::d_a_alink::daAlink* linkActrPtr, uint16_t param0) {
        const int32_t amogMania = return_opeenBottleZInitialized(linkActrPtr, param0);
        if (amogMania) {
            if (saveInfoPtr->zButtonnItemm == param0) {
                saveInfoPtr->zButtonnItemm = 0x60;
                zButtonTextureMomentInit(saveInfoPtr->zButtonnItemm);
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mSelectItem[2] = saveInfoPtr->zButtonnItemm;
                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item.item[11] = saveInfoPtr->zButtonnItemm;
            }
        }
        return amogMania;
    }
    int32_t Mod::itemWaterTime(libtp::tp::d_a_alink::daAlink* linkActrPtr, uint16_t param0) {

        if (param0 >= 0xE1 && param0 <= 0xE4) {
            if (gameplayStatus) return true;
        }
        return return_itmWaterUse(linkActrPtr, param0);
    }
    void Mod::dPadMapOn(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr) {
        if (!UiChecking2) UiChecking2 = true;
        //libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6974656D5F725F6E)->showTex();
        return mapDpadOn(dMeterDrawPtr);
    }
    void Mod::dPadMapOfff(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr) {
        if (UiChecking2) UiChecking2 = false;
        return mapDpadOff(dMeterDrawPtr);
    }
    void Mod::mapReturnTheShow() {
        //libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosY = -150.0f;
        midnaIconRePositionYYYYYYYY();
        return return_mapTheShow();
    }
    void Mod::mapReturnTheNotShow() {
        libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosY = 0.0f;
        itemZYPosMemory = 0.0f;
        libtp::tp::J2DPane::movePaneeClass(libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D69646F6E61), 515.0f, -350.0f);
        return return_mapTheNotShow();
    }
#else
    int32_t Mod::hookShotItemWaterMasks(int32_t param0) {
        if (maskWaterUseBoolWii) {
            maskWaterUseBoolWii = false;
            if (param0 >= 0xE1 && param0 <= 0xE4) {
                if (gameplayStatus) return 1;
            }
        }
        return return_hookShotItemWaterMasks(param0);
    }

    int32_t Mod::normalCutProcessOkDude(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) {
        if (isgStart) {
            isgStart = false;
            isgEnnd(linkActrPtr);
        }
        return return_normalCutProcInit(linkActrPtr, param0);
    }

    int32_t Mod::cutTurnWiiStuffInit(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0, int32_t param1) {
        if (isgStart) {
            isgStart = false;
            isgEnnd(linkActrPtr);
        }
        return return_initWiiCutTurnProc(linkActrPtr, param0, param1);
    }

    void Mod::cutDaaashWiiStuffInit(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0, int32_t param1) {
        if (isgStart) {
            isgStart = false;
            isgEnnd(linkActrPtr);
        }
        return return_initWiiCutDaaashProc(linkActrPtr, param0, param1);
    }
#endif

    bool rentingTime = false;
    uint8_t bobombInspectorrr = 0;
    uint8_t bobBobBOB[6] = {0, 0, 0, 0, 0, 0};

    void resetSomeStuffffffffffffff(bool tempBankOrNo) {
        for (uint8_t eightyKiloBytessss = 0; eightyKiloBytessss < 3; eightyKiloBytessss++) {
            bobBobBOB[eightyKiloBytessss] = 0; bobBobBOB[eightyKiloBytessss + 3] = 0;
            if (tempBankOrNo) tempBankinfo[eightyKiloBytessss] = 0;
        }
    }

    void bobReductionFunctionEfficiency(uint8_t referencePositionnnn, uint8_t waterBombssssss) {
        uint8_t existForOnlyThisYo = 0; if (waterBombssssss) existForOnlyThisYo = 3;
        libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[referencePositionnnn - existForOnlyThisYo] += bobBobBOB[referencePositionnnn];
        tempBankinfo[waterBombssssss+1] -= bobBobBOB[referencePositionnnn]; libtp::tp::d_save::setItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, (referencePositionnnn - existForOnlyThisYo) + 15, 0x70 + waterBombssssss);
    }

    uint8_t checkSomeBobRelatedStuffffff(uint8_t referencePoiiiiiiint, uint8_t waterBombssssss) {
        uint8_t multiplyTheeMaxAmmo = 1; if (libtp::tp::d_save::isFirstBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_get_item, 0x4F)) multiplyTheeMaxAmmo = 2;
        uint8_t existForThisOnlyHuh = 0; if (waterBombssssss) existForThisOnlyHuh = 3;
        return bobBobBOB[referencePoiiiiiiint] + libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[referencePoiiiiiiint - existForThisOnlyHuh] < (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_max.mItemMax[waterBombssssss + 1] * multiplyTheeMaxAmmo);
    }

    void depositItemsInBaaaaank() {
        if (tempBankinfo[rememberThisValuableInformation - 3] + saveInfoPtr->bobombStorePlace[rememberThisValuableInformation - 4] < (160/(rememberThisValuableInformation - 3))) { // if current normal bombs stored are less than max allowed stored
            for (uint8_t tempBagDefinitiveEdition = 0; tempBagDefinitiveEdition < 3; tempBagDefinitiveEdition++) {
                if (libtp::tp::d_save::getItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 15 + tempBagDefinitiveEdition, false) == (0x6C + rememberThisValuableInformation)) {
                    uint8_t totalBobCount = 0;
                    if (rememberThisValuableInformation == 4) { // normal bombs
                        if (!tempBagDefinitiveEdition) bobombInspectorrr |= 1; else if (tempBagDefinitiveEdition == 1) bobombInspectorrr |= 2; else bobombInspectorrr |= 4;
                        if (bobombInspectorrr & 1) bobBobBOB[0] = libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[0]; else bobBobBOB[0] = 0;
                        if (bobombInspectorrr & 2) bobBobBOB[1] = libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[1]; else bobBobBOB[1] = 0;
                        if (bobombInspectorrr & 4) bobBobBOB[2] = libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[2]; else bobBobBOB[2] = 0;
                        totalBobCount = (bobBobBOB[0] + bobBobBOB[1] + bobBobBOB[2]);
                    } else if (rememberThisValuableInformation == 5) {
                        if (!tempBagDefinitiveEdition) bobombInspectorrr |= 8; else if (tempBagDefinitiveEdition == 1) bobombInspectorrr |= 0x10; else bobombInspectorrr |= 0x20;
                        if (bobombInspectorrr & 8) bobBobBOB[3] = libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[0]; else bobBobBOB[3] = 0;
                        if (bobombInspectorrr & 0x10) bobBobBOB[4] = libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[1]; else bobBobBOB[4] = 0;
                        if (bobombInspectorrr & 0x20) bobBobBOB[5] = libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[2]; else bobBobBOB[5] = 0;
                        totalBobCount = (bobBobBOB[3] + bobBobBOB[4] + bobBobBOB[5]);
                    }
                    if (tempBankinfo[rememberThisValuableInformation - 3] < totalBobCount) {
                        tempBankinfo[rememberThisValuableInformation - 3]++; bankSystemControllererererer(rememberThisValuableInformation);
                        break;
                    }
                }
            }
        }
    }

    void undeposititemsInBaaaaank() {
        if (tempBankinfo[rememberThisValuableInformation - 3] > 0) {
            for (int8_t tempBagDefinitiveEdition = 2; tempBagDefinitiveEdition >= 0; tempBagDefinitiveEdition--) {
                if (libtp::tp::d_save::getItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 15 + tempBagDefinitiveEdition, false) == (0x6C + rememberThisValuableInformation)) {
                    uint8_t bobombDetectiveActive = tempBagDefinitiveEdition + 1; // = 3
                    if (rememberThisValuableInformation == 4) { // normal bombs
                        if (tempBagDefinitiveEdition == 2) bobombDetectiveActive = 4;
                        if (bobombInspectorrr & bobombDetectiveActive) {
                            tempBankinfo[1]--; bankSystemControllererererer(rememberThisValuableInformation);
                            if (tempBagDefinitiveEdition == 2) {
                                if (bobBobBOB[0] + bobBobBOB[1] == tempBankinfo[1]) bobombInspectorrr &= ~4;
                            } else if (tempBagDefinitiveEdition == 1) {
                                if (bobBobBOB[0] == tempBankinfo[1]) bobombInspectorrr &= ~2;
                            } else if (tempBankinfo[1] == 0) bobombInspectorrr &= ~1;
                            break;
                        }
                    } else if (rememberThisValuableInformation == 5) {
                        if (tempBagDefinitiveEdition == 2) bobombDetectiveActive = 0x20; else if (tempBagDefinitiveEdition == 1) bobombDetectiveActive = 0x10; else bobombDetectiveActive = 8;
                        if (bobombInspectorrr & bobombDetectiveActive) {
                            tempBankinfo[2]--; bankSystemControllererererer(5);
                            if (tempBagDefinitiveEdition == 2) {
                                if (bobBobBOB[3] + bobBobBOB[4] == tempBankinfo[2]) bobombInspectorrr &= ~0x20;
                            } else if (tempBagDefinitiveEdition == 1) {
                                if (bobBobBOB[3] == tempBankinfo[2]) bobombInspectorrr &= ~0x10;
                            } else if (tempBankinfo[2] == 0) bobombInspectorrr &= ~8;
                            break;
                        }
                    }
                }
            }
        }
    }

    void withdrawItemsInBaaaaank() {
        if (tempBankinfo[rememberThisValuableInformation - 9] < saveInfoPtr->bobombStorePlace[rememberThisValuableInformation - 10]) {
            uint8_t multiplyThouMaxAmmo = 1;
            if (libtp::tp::d_save::isFirstBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_get_item, 0x4F)) multiplyThouMaxAmmo = 2;
            for (uint8_t temporarBag = 0; temporarBag < 3; temporarBag++) {
                uint8_t bagIdentificationnnnn = libtp::tp::d_save::getItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 15 + temporarBag, false);
                if (bagIdentificationnnnn == 0x50 || (bagIdentificationnnnn == (0x66 + rememberThisValuableInformation) && libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[temporarBag] < (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_max.mItemMax[rememberThisValuableInformation - 9] * multiplyThouMaxAmmo))) { // if bag is empty or bag is same type of bob and not full
                    //uint8_t totalBobCount = 0;
                    if (rememberThisValuableInformation == 10) { // normal bob
                        if (!temporarBag && !(bobombInspectorrr & 8)) bobombInspectorrr |= 1; else if (temporarBag == 1 && !(bobombInspectorrr & 0x10)) bobombInspectorrr |= 2; else if (temporarBag == 2 && !(bobombInspectorrr & 0x20)) bobombInspectorrr |= 4;
                        if ((bobombInspectorrr & 1) && checkSomeBobRelatedStuffffff(0, 0)) {
                            bobBobBOB[0]++; tempBankinfo[1]++; bankSystemControllererererer(4); break;
                        } else if ((bobombInspectorrr & 2) && checkSomeBobRelatedStuffffff(1, 0)) {
                            bobBobBOB[1]++; tempBankinfo[1]++; bankSystemControllererererer(4); break;
                        } else if ((bobombInspectorrr & 4) && checkSomeBobRelatedStuffffff(2, 0)) {
                            bobBobBOB[2]++; tempBankinfo[1]++; bankSystemControllererererer(4); break;
                        }
                        //totalBobCount = (bobBobBOB[0] + bobBobBOB[1] + bobBobBOB[2]);
                    } else if (rememberThisValuableInformation == 11) {
                        if (!temporarBag && !(bobombInspectorrr & 1)) bobombInspectorrr |= 8; else if (temporarBag == 1 && !(bobombInspectorrr & 2)) bobombInspectorrr |= 0x10; else if (temporarBag == 2 && !(bobombInspectorrr & 4)) bobombInspectorrr |= 0x20;
                        if ((bobombInspectorrr & 8) && checkSomeBobRelatedStuffffff(3, 1)) {
                            bobBobBOB[3]++; tempBankinfo[2]++; bankSystemControllererererer(5); break;
                        } else if ((bobombInspectorrr & 0x10) && checkSomeBobRelatedStuffffff(4, 1)) {
                            bobBobBOB[4]++; tempBankinfo[2]++; bankSystemControllererererer(5); break;
                        } else if ((bobombInspectorrr & 0x20) && checkSomeBobRelatedStuffffff(5, 1)) {
                            bobBobBOB[5]++; tempBankinfo[2]++; bankSystemControllererererer(5); break;
                        }
                    }
                    //if (tempBankinfo[rememberThisValuableInformation - 9] + )
                }
            }
        }
    }

    void unwithdrawItemsInBaaaaank() {
        if (tempBankinfo[rememberThisValuableInformation - 9] > 0) {
            for (int8_t temporarBag = 2; temporarBag >= 0; temporarBag--) {
                if (rememberThisValuableInformation == 10) {
                    if (bobombInspectorrr & 4) {
                        bobBobBOB[2]--; tempBankinfo[1]--; bankSystemControllererererer(4);
                        if (bobBobBOB[2] == 0) bobombInspectorrr &= ~4;
                        break;
                    } else if (bobombInspectorrr & 2) {
                        bobBobBOB[1]--; tempBankinfo[1]--; bankSystemControllererererer(4);
                        if (bobBobBOB[1] == 0) bobombInspectorrr &= ~2;
                        break;
                    } else if (bobombInspectorrr & 1) {
                        bobBobBOB[0]--; tempBankinfo[1]--; bankSystemControllererererer(4);
                        if (bobBobBOB[0] == 0) bobombInspectorrr &= ~1;
                        break;
                    }
                } else if (rememberThisValuableInformation == 11) {
                    if (bobombInspectorrr & 0x20) {
                        bobBobBOB[5]--; tempBankinfo[2]--; bankSystemControllererererer(5);
                        if (bobBobBOB[5] == 0) bobombInspectorrr &= ~0x20;
                        break;
                    } else if (bobombInspectorrr & 0x10) {
                        bobBobBOB[4]--; tempBankinfo[2]--; bankSystemControllererererer(5);
                        if (bobBobBOB[4] == 0) bobombInspectorrr &= ~0x10;
                        break;
                    } else if (bobombInspectorrr & 8) {
                        bobBobBOB[3]--; tempBankinfo[2]--; bankSystemControllererererer(5);
                        if (bobBobBOB[3] == 0) bobombInspectorrr &= ~8;
                        break;
                    }
                }
            }
        }
    }

    void Mod::procNewFrame() {
        if (rentingTime) {
            uint8_t awaitingConfirmationNow = 0;
#ifdef PLATFORM_WII
            if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags & 4)
#else
            if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 4)
#endif
            {
                libtp::tp::m_Do_Audio::mDoAud_seStart(0xA1, 0, 0, 0);
                if (rememberThisValuableInformation < 2) bankSystemControllererererer((rememberThisValuableInformation + 1) % 2);
                else {
                    if (nowLine < 10) {
                        strcpy(sysConsolePtr->consoleLine[nowLine].line, bufferForThePythonnn);
                        strcpy(bufferForThePythonnn, sysConsolePtr->consoleLine[nowLine+1].line);
                        sprintf(sysConsolePtr->consoleLine[nowLine+1].line, "-> %s", bufferForThePythonnn);
                        nowLine++;
                    } else {
                        strcpy(sysConsolePtr->consoleLine[nowLine].line, bufferForThePythonnn);
                        strcpy(bufferForThePythonnn, sysConsolePtr->consoleLine[7].line);
                        sprintf(sysConsolePtr->consoleLine[7].line, "-> %s", bufferForThePythonnn);
                        nowLine = 7;
                    }
                    if (rememberThisValuableInformation < 7) rememberThisValuableInformation = nowLine - 5;
                    else rememberThisValuableInformation = nowLine + 1;
                }
            }
#ifdef PLATFORM_WII
            else if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags & 8)
#else
            else if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 8)
#endif
            {
                libtp::tp::m_Do_Audio::mDoAud_seStart(0xA1, 0, 0, 0);
                if (rememberThisValuableInformation < 2) bankSystemControllererererer((rememberThisValuableInformation + 1) % 2);
                else {
                    if (nowLine > 7) {
                        strcpy(sysConsolePtr->consoleLine[nowLine].line, bufferForThePythonnn);
                        strcpy(bufferForThePythonnn, sysConsolePtr->consoleLine[nowLine-1].line);
                        sprintf(sysConsolePtr->consoleLine[nowLine-1].line, "-> %s", bufferForThePythonnn);
                        nowLine--;
                    } else {
                        strcpy(sysConsolePtr->consoleLine[nowLine].line, bufferForThePythonnn);
                        strcpy(bufferForThePythonnn, sysConsolePtr->consoleLine[10].line);
                        sprintf(sysConsolePtr->consoleLine[10].line, "-> %s", bufferForThePythonnn);
                        nowLine = 10;
                    }
                    if (rememberThisValuableInformation < 7) rememberThisValuableInformation = nowLine - 5;
                    else rememberThisValuableInformation = nowLine + 1;
                }
            }
#ifdef PLATFORM_WII
            else if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags & 0x400)
#else
            else if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x200)
#endif
            {
                awaitingConfirmationNow = 1;
            }
#ifdef PLATFORM_WII
            else if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags & 0x800)
#else
            else if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x100)
#endif
            {
                libtp::tp::m_Do_Audio::mDoAud_seStart(0x18, 0, 0, 0);
                awaitingConfirmationNow = 2;
            }
#ifdef PLATFORM_WII
            else if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags & 1)
#else
            else if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 1)
#endif
            {
                if (rememberThisValuableInformation == 2 || rememberThisValuableInformation == 8) {
                    if (rupeeTempBankInfo > 0) {
                        rupeeTempBankInfo--;
                        bankSystemControllererererer(2);
                    }
                } else if (rememberThisValuableInformation == 3 || rememberThisValuableInformation == 9) {
                    if (tempBankinfo[0] > 0) {
                        tempBankinfo[0]--;
                        bankSystemControllererererer(3);
                    }
                } else if (rememberThisValuableInformation == 4 || rememberThisValuableInformation == 5) {
                    undeposititemsInBaaaaank();
                } else if (rememberThisValuableInformation == 10 || rememberThisValuableInformation == 11) {
                    unwithdrawItemsInBaaaaank();
                    /*if (tempBankinfo[2] > 0) {
                        tempBankinfo[2]--; bankSystemControllererererer(5);
                    }*/
                }
            }
#ifdef PLATFORM_WII
            else if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags & 2)
#else
            else if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 2)
#endif
            {
                if (rememberThisValuableInformation == 2 || rememberThisValuableInformation == 8) {
                    if (rupeeTempBankInfo + saveInfoPtr->rupeeeStoreePlace < 2000 && rememberThisValuableInformation == 2) {
                        if (rupeeTempBankInfo < libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentRupees) {
                            rupeeTempBankInfo++; bankSystemControllererererer(2);
                        }
                    } else if (rememberThisValuableInformation == 8) {
                        if (rupeeTempBankInfo < saveInfoPtr->rupeeeStoreePlace) {
                            if ((rupeeTempBankInfo + libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentRupees < 300) || (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentWallet == 1 && rupeeTempBankInfo + libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentRupees < 600) || (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentWallet == 2 && rupeeTempBankInfo + libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentRupees < 1000)) {
                                rupeeTempBankInfo++; bankSystemControllererererer(2);
                            }
                        }
                    }
                } else if (rememberThisValuableInformation == 3) {
                    if (tempBankinfo[0] + saveInfoPtr->bowwStorePlace < 200) {
                        if (tempBankinfo[0] < libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bow_ammo) {
                            tempBankinfo[0]++; bankSystemControllererererer(3);
                        }
                    }
                } else if (rememberThisValuableInformation == 9) {
                    if (tempBankinfo[0] < saveInfoPtr->bowwStorePlace) {
                        if (tempBankinfo[0] + libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bow_ammo < libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_max.mItemMax[0]) {
                            tempBankinfo[0]++; bankSystemControllererererer(3);
                        }
                    }
                } else if (rememberThisValuableInformation == 4 || rememberThisValuableInformation == 5) {
                    // if current normal bombs stored are less than max allowed stored
                    depositItemsInBaaaaank();
                } else if (rememberThisValuableInformation == 10 || rememberThisValuableInformation == 11) {
                    withdrawItemsInBaaaaank();
                }
            }
#ifdef PLATFORM_WII
            else if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags & 0x1000)
#else
            else if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x40)
#endif
            {
                if (rememberThisValuableInformation == 2 || rememberThisValuableInformation == 8) {
                    if (rupeeTempBankInfo - 10 > 0) rupeeTempBankInfo -= 10; else rupeeTempBankInfo = 0;
                    bankSystemControllererererer(2);
                } else if (rememberThisValuableInformation == 3 || rememberThisValuableInformation == 9) {
                    if (tempBankinfo[0] - 10 > 0) tempBankinfo[0] -= 10; else tempBankinfo[0] = 0;
                    bankSystemControllererererer(3);
                } else if (rememberThisValuableInformation == 4 || rememberThisValuableInformation == 5) {
                    for (uint8_t temporarryTempBagTemp = 0; temporarryTempBagTemp < 10; temporarryTempBagTemp++) {
                        undeposititemsInBaaaaank();
                    }
                } else if (rememberThisValuableInformation == 10 || rememberThisValuableInformation == 11) {
                    for (uint8_t bagIsTemporaryOk = 0; bagIsTemporaryOk < 10; bagIsTemporaryOk++) {
                        unwithdrawItemsInBaaaaank();
                    }
                }
            }
#ifdef PLATFORM_WII
            else if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags & 0x10)
#else
            else if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x20)
#endif
            {
                if (rememberThisValuableInformation == 2) {
                    if (rupeeTempBankInfo + 10 + saveInfoPtr->rupeeeStoreePlace <= 2000) {
                        if (rupeeTempBankInfo + 10 < libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentRupees) {
                            rupeeTempBankInfo += 10;
                        } else rupeeTempBankInfo = libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentRupees;
                        bankSystemControllererererer(2);
                    }
                } else if (rememberThisValuableInformation == 3) {
                    if (tempBankinfo[0] + 10 + saveInfoPtr->bowwStorePlace <= 200) {
                        if (tempBankinfo[0] + 10 < libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bow_ammo) {
                            tempBankinfo[0] += 10;
                        } else tempBankinfo[0] = libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bow_ammo;
                        bankSystemControllererererer(3);
                    }
                } else if (rememberThisValuableInformation == 4 || rememberThisValuableInformation == 5) {
                    for (uint8_t tempCountTo10 = 0; tempCountTo10 < 10; tempCountTo10++) {
                        depositItemsInBaaaaank();
                    }
                } else if (rememberThisValuableInformation == 8) {
                    if (rupeeTempBankInfo + 10 <= saveInfoPtr->rupeeeStoreePlace) {
                        if ((rupeeTempBankInfo + 10 + libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentRupees <= 300) || (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentWallet == 1 && rupeeTempBankInfo + 10 + libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentRupees < 600) || (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentWallet == 2 && rupeeTempBankInfo + 10 + libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentRupees < 1000)) {
                            rupeeTempBankInfo += 10;
                        }
                    } else rupeeTempBankInfo = saveInfoPtr->rupeeeStoreePlace;
                    bankSystemControllererererer(2);
                } else if (rememberThisValuableInformation == 9) {
                    if (tempBankinfo[0] + 10 <= saveInfoPtr->bowwStorePlace) {
                        if (tempBankinfo[0] + 10 + libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bow_ammo < libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_max.mItemMax[0]) {
                            tempBankinfo[0] += 10;
                        }
                    } else tempBankinfo[0] = saveInfoPtr->bowwStorePlace;
                    bankSystemControllererererer(3);
                } else if (rememberThisValuableInformation == 10 || rememberThisValuableInformation == 11) {
                    for (uint8_t temporarCountTo10 = 0; temporarCountTo10 < 10; temporarCountTo10++) {
                        withdrawItemsInBaaaaank();
                    }
                }
            }

            if (awaitingConfirmationNow == 1) {
                strcpy(sysConsolePtr->consoleLine[9].line, "");
                strcpy(sysConsolePtr->consoleLine[10].line, "");
                if (rememberThisValuableInformation < 2) {
                    libtp::display::setConsoleColor(0, 0, 0, 0);
                    sysConsolePtr->consoleLine[6].showLine = false;
                    rentingTime = false;
                    bobombInspectorrr = 0;
                } else {
#ifdef PLATFORM_WII
                    libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags = 0;
#else
                    libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags = 0;
#endif
                    bankSystemControllererererer(0);
                }
            } else if (awaitingConfirmationNow == 2) {
                if (rememberThisValuableInformation > 1) {
                    if (rememberThisValuableInformation < 8) {
                        saveInfoPtr->rupeeeStoreePlace += rupeeTempBankInfo;
                        saveInfoPtr->bowwStorePlace += tempBankinfo[0];
                        saveInfoPtr->bobombStorePlace[0] += tempBankinfo[1]; saveInfoPtr->bobombStorePlace[1] += tempBankinfo[2];
                        libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentRupees -= rupeeTempBankInfo;
                        libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bow_ammo -= tempBankinfo[0];
                        // continue here
                        while (tempBankinfo[1] > 0) {
                            tempBankinfo[1]--;
                            if (bobombInspectorrr & 1) {
                                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[0]--;
                                if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[0] == 0) {
                                    bobombInspectorrr &= ~1; libtp::tp::d_save::setItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 15, 0x50);
                                }
                            } else if (bobombInspectorrr & 2) {
                                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[1]--;
                                if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[1] == 0) {
                                    bobombInspectorrr &= ~2; libtp::tp::d_save::setItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 16, 0x50);
                                }
                            } else if (bobombInspectorrr & 4) {
                                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[2]--;
                                if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[2] == 0) {
                                    bobombInspectorrr &= ~4; libtp::tp::d_save::setItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 17, 0x50);
                                }
                            }
                        }
                        while (tempBankinfo[2] > 0) {
                            tempBankinfo[2]--;
                            if (bobombInspectorrr & 8) {
                                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[0]--;
                                if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[0] == 0) {
                                    bobombInspectorrr &= ~8; libtp::tp::d_save::setItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 15, 0x50);
                                }
                            } else if (bobombInspectorrr & 0x10) {
                                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[1]--;
                                if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[1] == 0) {
                                    bobombInspectorrr &= ~0x10; libtp::tp::d_save::setItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 16, 0x50);
                                }
                            } else if (bobombInspectorrr & 0x20) {
                                libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[2]--;
                                if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[2] == 0) {
                                    bobombInspectorrr &= ~0x20; libtp::tp::d_save::setItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 17, 0x50);
                                }
                            }
                        }
                    } else {
                        saveInfoPtr->rupeeeStoreePlace -= rupeeTempBankInfo;
                        saveInfoPtr->bowwStorePlace -= tempBankinfo[0];
                        saveInfoPtr->bobombStorePlace[0] -= tempBankinfo[1]; saveInfoPtr->bobombStorePlace[1] -= tempBankinfo[2];
                        libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentRupees += rupeeTempBankInfo;
                        libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bow_ammo += tempBankinfo[0];
                        while (tempBankinfo[1] > 0) {
                            if (bobombInspectorrr & 1) {
                                bobombInspectorrr &= ~1;
                                bobReductionFunctionEfficiency(0, 0);
                            } else if (bobombInspectorrr & 2) {
                                bobombInspectorrr &= ~2;
                                bobReductionFunctionEfficiency(1, 0);
                            } else if (bobombInspectorrr & 4) {
                                bobombInspectorrr &= ~4;
                                bobReductionFunctionEfficiency(2, 0);
                            }
                        }
                        while (tempBankinfo[2] > 0) {
                            if (bobombInspectorrr & 8) {
                                bobombInspectorrr &= ~8;
                                bobReductionFunctionEfficiency(3, 1);
                            } else if (bobombInspectorrr & 0x10) {
                                bobombInspectorrr &= ~0x10;
                                bobReductionFunctionEfficiency(4, 1);
                            } else if (bobombInspectorrr & 0x20) {
                                bobombInspectorrr &= ~0x20;
                                bobReductionFunctionEfficiency(5, 1);
                            }
                        }
                    }
                    strcpy(sysConsolePtr->consoleLine[9].line, "");
                    strcpy(sysConsolePtr->consoleLine[10].line, "");
                    libtp::display::setConsoleColor(0, 0, 0, 0);
                    sysConsolePtr->consoleLine[6].showLine = false;
                    rentingTime = false;
                    bobombInspectorrr = 0;
                } else {
                    resetSomeStuffffffffffffff(true);
                    rupeeTempBankInfo = 0;
#ifdef PLATFORM_WII
                    libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags = 0;
#else
                    libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags = 0;
#endif
                    if (rememberThisValuableInformation == 0) rememberThisValuableInformation = 2; else rememberThisValuableInformation = 8;
                    strcpy(bufferForThePythonnn, "Rupees:       <0>");
                    sprintf(sysConsolePtr->consoleLine[7].line, "-> %s", bufferForThePythonnn);
                    strcpy(sysConsolePtr->consoleLine[8].line, "Arrows:       <0>");
                    strcpy(sysConsolePtr->consoleLine[9].line, "Normal Bombs: <0>");
                    strcpy(sysConsolePtr->consoleLine[10].line, "Water Bombs:  <0>");
                    nowLine = 7;
                }
                resetSomeStuffffffffffffff(false);
            }
        }
#ifdef PLATFORM_WII
        if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags & 2) {
            if (saveInfoPtr->dPadChoice != 2) {
                libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags = 0;
                dpaaadRight = true;
            }
        } else if (dpaaadRight) dpaaadRight = false;
        
        if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mButtonFlags & 4) {
            libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mButtonFlags = 0;
            if (!dpaaadDown) {
                dpaaadDown = true; if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer != nullptr) {
                    if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentForm == 0) libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags = 0;
                }
            }
        } else if (dpaaadDown) dpaaadDown = false;
#endif
        return return_constFrameThing();
    }

    int32_t Mod::newItemChangesEveryFrame(libtp::tp::d_a_alink::daAlink* linkActrPtr, uint8_t param0) {
#ifndef PLATFORM_WII
        if (param0 != 2 || libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item_record.bomb_bag_ammo[saveInfoPtr->zButtonSloot - 15] == 0) return return_newItemChangesEveryFrame(linkActrPtr, param0);
        libtp::patch::writeBranch(zNewItemChangeExamination + 0x764, zNewItemChangeExamination + 0x770);
        const int32_t agomamoga = return_newItemChangesEveryFrame(linkActrPtr, param0);

        if (agomamoga) {
            performStaticASMReplacement(zNewItemChangeExamination + 0x764, 0x4BF6D3C1);
        }
        return agomamoga;
#else
        maskWaterUseBoolWii = true;
        const int32_t agomamoga = return_newItemChangesEveryFrame(linkActrPtr, param0);
        if (agomamoga) {
            maskWaterUseBoolWii = false;
        }
        return agomamoga;
#endif
        //uint8_t itemParam00 = saveInfoPtr->zButtonnItemm;
        //uint8_t itemParam00 = libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mSelectItem[param0];
        //libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mSelectItem[param0] = saveInfoPtr->zButtonnItemm;
        //performStaticASMReplacement(selectItemForZSelect + 0x30, ASM_NOP);
        //performStaticASMReplacement(selectItemForZSelect + 0x44, ASM_NOP);

        //const int32_t amogo = return_newItemChangesEveryFrame(linkActrPtr, param0);
        //if (amogo) {
            //performStaticASMReplacement(selectItemForZSelect + 0x30, 0x408200E0);
            //performStaticASMReplacement(selectItemForZSelect + 0x44, 0x418200CC);
            //libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mSelectItem[param0] = itemParam00;
        //}
        //return amogo;
        /*if (libtp::tp::d_a_alink::checkSpinnerRide(linkActrPtr) || itemParam00 == 0x50 ||
            ((itemParam00 == 0xF8 || libtp::tp::d_a_alink::checkOilBottleItem(itemParam00)) &&
            libtp::tp::d_a_alink::checkWaterInKandelaarOffset(linkActrPtr, linkActrPtr->mWaterY)) ||
            (libtp::tp::d_a_alink::checkCanoeRide(linkActrPtr) && libtp::tp::d_a_alink::checkStageName("F_SP127")) || libtp::tp::d_a_alink::checkCloudSea() ||
            (((linkActrPtr->mModeFlg & 0x40000) || libtp::tp::d_a_player::checkNoResetFlg0(linkActrPtr, libtp::tp::d_a_player::FLG0_UNDERWATER)) &&
            !libtp::tp::d_a_alink::checkAcceptUseItemInWater(linkActrPtr, itemParam00)) ||
            ((linkActrPtr->mModeFlg & 0x40000) && itemParam00 == 0x71) || !libtp::tp::d_a_alink::checkCastleTownUseItem(itemParam00) ||
            (libtp::tp::d_a_alink::checkBoardRide(linkActrPtr) && itemParam00 != 0x103) ||
            ((linkActrPtr->mModeFlg & 0x400) && 
            (itemParam00 == 0x60 || itemParam00 == 0x72 || itemParam00 == 0x42 ||
            itemParam00 == 0x46 || libtp::tp::d_a_alink::checkFishingRodItem(itemParam00))) ||
            ((linkActrPtr->mGndPolySpecialCode == 6 || linkActrPtr->field_0x2fbd == 1 || linkActrPtr->field_0x2fbd == 2 ||
            linkActrPtr->mWaterY - linkActrPtr->mCurrent.mPosition.y > 45.0f ||
            (linkActrPtr->field_0x2fbc == 6 && linkActrPtr->mWaterY - linkActrPtr->mCurrent.mPosition.y >= 0.0f) || linkActrPtr->field_0x2fbd == 3) &&
            itemParam00 == 0x41) ||
            (libtp::tp::d_a_alink::checkBossRoom() && libtp::tp::d_a_alink::checkDungeonWarpItem(itemParam00)) ||
            (itemParam00 == 0x25 &&
            (libtp::tp::d_a_alink::checklv)))*/
    }

    /*int32_t Mod::isItAWideScreeeen(libtp::tp::d_camera::view_class* param0) {
        sprintf(sysConsolePtr->consoleLine[0].line, "Wah");
        return return_wiiiiiiideScreeeeen(param0);
    }*/

    bool Mod::permissionForMask(uint16_t param0) {

        if (param0 >= 0xE1 && param0 <= 0xE4) {
            if (param0 < 0xE4 && libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[0] != 0x2F) return false;
            if (gameplayStatus) return true;
        } /*else if (param0 == 0x103 && saveInfoPtr->maskID == 2) {
            if (gameplayStatus) {
                if (blastNope) return true;
                else return false;
            }
        }*/
        return maskWearPermissionReturn(param0);
    }

    //int32_t Mod::masksShallBeAllowed(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) {
        //if (param0 > 0xE0 && param0 < 0xE4) {
        //for (uint8_t tempiiiii = 0; tempiiiii < 2; tempiiiii++) {
            //if (libtp::tp::d_a_alink::checkGroupItem(linkActrPtr, param0, libtp::tp::d_com_inf_game::dComIfGp_getSelectItem(tempiiiii)) && libtp::tp::d_a_alink::itemTriggerCheck(linkActrPtr, 1 << tempiiiii)) {
                /*if (param0 != 0x45)*/ //linkActrPtr->mSelectItemId = tempiiiii;
                //return 1;
            //}
        //}
        //return 0;
        //}
        //return return_allowTheMasksPlease(linkActrPtr, param0);
    //}

    void Mod::swordEquipDecision(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) {
        if (saveInfoPtr->maskID == 2 || noSwordLOL) {
            if (noSwordLOL) libtp::tp::d_a_alink::dMeter2Info_setFloatingMessage(2047, 90, true);
            return;
        }
        return return_equipSword(linkActrPtr, param0);
    }
    
    void Mod::swordMustBeSet(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        if (noSwordLOL) return;
        if (isgStart) isgInitiation(linkActrPtr);
        return return_swordMustBeSet(linkActrPtr);
    }

    void Mod::swordMustBeUnSet(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        if (saveInfoPtr->maskID == 2 && blastNope) iconBButtonDimFunc(0);
        return return_swordMustBeUnSet(linkActrPtr);
    }

    void Mod::gameChangerr(void* param0) {
        return_gameChangerr(param0);
        if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mLayer == 0xd && libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mPoint == 21 && libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mRoomNo == 1 && strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mStage, "F_SP108") == 0)
        {
            timeReset3Day(true);
        }
    }

    /*void Mod::animeSingleIsSet(libtp::tp::d_a_alink::daAlink* linkActrPtr, uint16_t anmID, libtp::tp::d_a_alink::daAlink_UPPER upperChoice, float param0, float param1, int16_t param2, float param3) {
        sprintf(sysConsolePtr->consoleLine[4].line, "animePlay: %x", anmID);
        return return_singleAnimeIsSet(linkActrPtr, anmID, upperChoice, param0, param1, param2, param3);
    }*/

    libtp::tp::d_a_alink::daAlink_BckData* Mod::manualShield(libtp::tp::d_a_alink::daAlink* linkActrPtr, libtp::tp::d_a_alink::daAlink_ANM i_anmID)
    {
#ifndef PLATFORM_WII
        if (libtp::tp::d_a_alink::checkUpperGuardAnime(linkActrPtr) && libtp::tp::m_do_controller_pad::cpadInfo[0].mHoldLockR == 0)
#else
        if (libtp::tp::d_a_alink::checkUpperGuardAnime(linkActrPtr) && (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mButtonFlags & 0x00004000) == 0)
#endif
        {
            return &libtp::tp::d_a_alink::m_anmDataTable[i_anmID].field_0x0;
        }
        return shieldManually(linkActrPtr, i_anmID);
    }

    void Mod::unSheathingShield(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
#ifndef PLATFORM_WII

        if (libtp::tp::m_do_controller_pad::cpadInfo[0].mHoldLockR == 0)
#else
        if ((libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mButtonFlags & 0x00004000) == 0 && !dpaaadDown)
#endif
        {
            return libtp::tp::d_a_player::offNoResetFlg2(linkActrPtr, libtp::tp::d_a_player::daPy_FLG2::FLG2_UNK_8000000);
        }
        else if (shieldCrouch) return libtp::tp::d_a_player::onNoResetFlg2(linkActrPtr, libtp::tp::d_a_player::daPy_FLG2::FLG2_UNK_8000000);
        return unSheathShield(linkActrPtr);
    }

    bool maskBoom = false;
    int32_t Mod::largeDamageInitial(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0, int32_t param1, int16_t param2, int16_t param3, void* param4, int32_t param5) {

        if (maskBoom) {
            if (linkActrPtr->mProcID == 0x1C) return 0;
            maskBoom = false;
        }
        if (isgStart) {
            isgStart = false;
            isgCarryObjectsSwingCountDown = 0;
            isgEnnd(linkActrPtr);
        }
#ifndef PLATFORM_WII
        if (libtp::tp::m_do_controller_pad::cpadInfo[0].mButtonFlags & 0x20) crouch = 0;
#else
        if (dpaaadDown) crouch = 0;
#endif
        return initLargeDamage(linkActrPtr, param0, param1, param2, param3, param4, param5);
    }

    int32_t Mod::damagetheSettingPointMaybe(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t a1, int32_t a2, int32_t a3, int32_t a4) {
        /*if (isgStart) {
            isgStart = false;
            isgCarryObjectsSwingCountDown = 0;
            isgEnnd(linkActrPtr);
        }*/
        if (linkActrPtr->mProcID == 0x1C && maskBoom) return 0;
        //sprintf(sysConsolePtr->consoleLine[3].line, "healthOuch: %d", a1);
        if (saveInfoPtr->doubleDefencee) {
            if (a1 > 1) a1 = a1/2;
            //sprintf(sysConsolePtr->consoleLine[5].line, "healthOuchHalf: %d", a1);
        }
        return return_damageSetIt(linkActrPtr, a1, a2, a3, a4);
    }

    int32_t Mod::jumpAutomatcInit(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) {
        crouch = 0; shieldCrouch = false;
        if (isgStart) return 0;
        return return_jumpAutomaticInit(linkActrPtr, param0);
    }

    int32_t Mod::hangStartFallInit(libtp::tp::d_a_alink::daAlink* linkActrPtr, void* param0) {
        crouch = 0; shieldCrouch = false;
        if (isgStart) return 0;
        return return_hangStartInitNow(linkActrPtr, param0);
    }

    uint8_t testtttttttTimerrrrrr = 0;

    void ninetySixKiloBytes(bool HowAboutIt) {
        if (HowAboutIt) {
#ifdef PLATFORM_WII
            libtp::patch::writeBranch(gameHourssss + 0x22B8, gameHourssss + 0x22E0);
#else
            libtp::patch::writeBranch(gameHourssss + 0x2BD4, gameHourssss + 0x2C00);
#endif
            smallTimeLeftHurryyyyy = true;
        } else {
            smallTimeLeftHurryyyyy = false;
#ifdef PLATFORM_WII
            performStaticASMReplacement(gameHourssss + 0x22B8, 0x418200AC);
#else
            performStaticASMReplacement(gameHourssss + 0x2BD4, 0x418200C0);
#endif
        }
    }

    int32_t Mod::executee(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        const int32_t amoga = return_executee(linkActrPtr);
        if (amoga) {
            if (noSwordLOL) bubbleAttackContinue();
            if (isgStart && linkActrPtr->mEquipItem == 0x103)
            {
                cXyz spC8;
                spC8 = linkActrPtr->mCurrent.mPosition;
                spC8.x += (50.0f * libtp::tp::sincosTable_.sinShort(linkActrPtr->mCollisionRot.y));
                spC8.y += 20.0f;
                spC8.z += (50.0f * libtp::tp::sincosTable_.cosShort(linkActrPtr->mCollisionRot.y));
                /*if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 4) {
                    libtp::tp::d_a_alink::decSwordBlur(linkActrPtr);
                    libtp::tp::d_a_alink::resetAtCollision(linkActrPtr, 1);
                    libtp::tp::J3DModel::ResetAtHit(&linkActrPtr->mAtCyl);
                    linkActrPtr->field_0x2fce = 5;
                    linkActrPtr->mComboCutCount = 0;
                    libtp::tp::d_a_player::offNoResetFlg0(linkActrPtr, libtp::tp::d_a_player::FLG0_UNK_8000);
                    libtp::tp::d_a_player::offNoResetFlg2(linkActrPtr, libtp::tp::d_a_player::FLG2_UNK_2);
                    libtp::tp::d_a_player::onResetFlg0(linkActrPtr, libtp::tp::d_a_player::RFLG0_UNK_2);
                    libtp::tp::d_a_alink::setSwordAtParam(linkActrPtr, libtp::tp::J3DModel::dCcG_At_Spl_UNK_0, 7, 16, 2, libtp::tp::d_a_alink::cutVars.mSwordLength, libtp::tp::d_a_alink::cutVars.mSwordRadius);
                    linkActrPtr->field_0x2fd0 = 2;
                    linkActrPtr->mRunCutComboCount++;
                    linkActrPtr->mCutType = 0x25;
                    linkActrPtr->mRunCutComboCount = 1;
                    linkActrPtr->field_0x2f96 = 2;
                    linkActrPtr->field_0x307e = libtp::tp::d_a_alink::cutVars.mComboDuration;
                    libtp::tp::d_a_alink::setCylAtParam(linkActrPtr, libtp::tp::d_a_alink::getSwordAtType(linkActrPtr), libtp::tp::J3DModel::dCcG_At_Spl_UNK_0, 7, 16, 2, 50.0f, 130.0f);
                    libtp::tp::d_a_alink::setCutWaterDropEffect(linkActrPtr);
                    linkActrPtr->field_0x2fb7 = 0;
                    linkActrPtr->m_swordBlur.field_0x20 = 70;
                    libtp::tp::d_a_alink::setSwordAtCollision(linkActrPtr, 1);
                    libtp::tp::d_a_player::onNoResetFlg0(linkActrPtr, libtp::tp::d_a_player::FLG0_UNK_40);
                    libtp::tp::J3DModel::StartCAtCyl(&linkActrPtr->mAtCyl, spC8);
                    libtp::tp::J3DModel::cCcDSet(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDCcS, &linkActrPtr->mAtCyl);
                    libtp::tp::d_a_alink::initBlur(&linkActrPtr->m_swordBlur, 0.0f, 0, &linkActrPtr->mSwordTopPos, &linkActrPtr->field_0x3498, &linkActrPtr->field_0x34a4);
                }*/
                /*if (testtttttttTimerrrrrr == 0) {
                    libtp::tp::d_a_alink::decSwordBlur(linkActrPtr);
                    libtp::tp::d_a_alink::resetAtCollision(linkActrPtr, 1);
                    libtp::tp::J3DModel::ResetAtHit(&linkActrPtr->mAtCyl);
                    linkActrPtr->field_0x2fce = 5;
                    linkActrPtr->mComboCutCount = 0;
                    libtp::tp::d_a_player::offNoResetFlg0(linkActrPtr, libtp::tp::d_a_player::FLG0_UNK_8000);
                    libtp::tp::d_a_player::offNoResetFlg2(linkActrPtr, libtp::tp::d_a_player::FLG2_UNK_2);
                    libtp::tp::d_a_player::onResetFlg0(linkActrPtr, libtp::tp::d_a_player::RFLG0_UNK_2);
                    libtp::tp::d_a_alink::setSwordAtParam(linkActrPtr, libtp::tp::J3DModel::dCcG_At_Spl_UNK_0, 7, 16, 2, libtp::tp::d_a_alink::cutVars.mSwordLength, libtp::tp::d_a_alink::cutVars.mSwordRadius);
                    linkActrPtr->field_0x2fd0 = 2;
                    linkActrPtr->mRunCutComboCount++;
                    linkActrPtr->mCutType = 0x25;
                    linkActrPtr->mRunCutComboCount = 1;
                    linkActrPtr->field_0x2f96 = 2;
                    linkActrPtr->field_0x307e = libtp::tp::d_a_alink::cutVars.mComboDuration;
                    libtp::tp::d_a_alink::setCylAtParam(linkActrPtr, libtp::tp::d_a_alink::getSwordAtType(linkActrPtr), libtp::tp::J3DModel::dCcG_At_Spl_UNK_0, 7, 16, 2, 50.0f, 130.0f);
                    libtp::tp::d_a_alink::setCutWaterDropEffect(linkActrPtr);
                    linkActrPtr->field_0x2fb7 = 0;
                    linkActrPtr->m_swordBlur.field_0x20 = libtp::tp::d_a_alink::cutVars.mBlurAlpha;
                    libtp::tp::d_a_alink::setSwordAtCollision(linkActrPtr, 1);
                    libtp::tp::d_a_player::onNoResetFlg0(linkActrPtr, libtp::tp::d_a_player::FLG0_UNK_40);
                    libtp::tp::J3DModel::StartCAtCyl(&linkActrPtr->mAtCyl, spC8);
                    libtp::tp::J3DModel::cCcDSet(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDCcS, &linkActrPtr->mAtCyl);
                    libtp::tp::d_a_alink::initBlur(&linkActrPtr->m_swordBlur, 0.0f, 0, &linkActrPtr->mSwordTopPos, &linkActrPtr->field_0x3498, &linkActrPtr->field_0x34a4);
                } else if {*/
                    libtp::tp::d_a_alink::setSwordAtCollision(linkActrPtr, 0);
                    libtp::tp::J3DModel::MoveCAtCyl(&linkActrPtr->mAtCyl, spC8);
                    libtp::tp::J3DModel::cCcDSet(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDCcS, &linkActrPtr->mAtCyl);
                    libtp::tp::d_a_alink::copyBlur(&linkActrPtr->m_swordBlur, &linkActrPtr->mSwordTopPos, &linkActrPtr->field_0x3498, &linkActrPtr->field_0x34a4);
                    libtp::tp::d_a_player::onNoResetFlg0(linkActrPtr, libtp::tp::d_a_player::FLG0_UNK_40);
#ifdef PLATFORM_WII
                    libtp::tp::d_bg_s_acch::setCommonLineSet(&linkActrPtr->mLinkLinChk, &spC8, &linkActrPtr->field_0x3720, linkActrPtr);
                    if (libtp::tp::d_bg_s_acch::LineCross(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDBgS, &linkActrPtr->mLinkLinChk) && !libtp::tp::d_bg_s_acch::GetPolyAttackThrough(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDBgS, linkActrPtr->mLinkLinChk) && libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[1] != 0x3F)
#else
                    if (libtp::tp::d_a_alink::commonLineCheck(linkActrPtr, &spC8, &linkActrPtr->field_0x3720) && !libtp::tp::d_bg_s_acch::GetPolyAttackThrough(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDBgS, linkActrPtr->mLinkLinChk) && libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[1] != 0x3F)
#endif
                    {
                        libtp::tp::J3DModel::cM3dGPla sp34;
                        libtp::tp::d_bg_s_acch::GetTriPlaBg_s(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDBgS, linkActrPtr->mLinkLinChk, &sp34);
                        libtp::tp::d_a_alink::dKy_SordFlush_set(linkActrPtr->mLinkLinChk.mLin.mEnd, 2);
                        libtp::tp::d_particle::setParticleNow(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mParticle, 0, 0x32E, &linkActrPtr->mLinkLinChk.mLin.mEnd, NULL, &linkActrPtr->mBodyAngle, NULL, 255, NULL, -1, NULL, NULL, NULL, 1.0f);
                    }
                    //libtp::tp::d_a_alink::traceBlur(&linkActrPtr->m_swordBlur, &linkActrPtr->mCurrent.mPosition, &linkActrPtr->mNext.mPosition, linkActrPtr->mCurrent.mAngle.y - linkActrPtr->field_0x2fe6);
                //}
                //testtttttttTimerrrrrr++;
                //if (testtttttttTimerrrrrr > 10) testtttttttTimerrrrrr = 0;
            }
            if ((saveInfoPtr->MMGlitches != 0 || linkActrPtr->mEquipItem != 0x103) && isgStart) isgEnnd(linkActrPtr);

            if (smallTimeLeftHurryyyyy && libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mEvent.mMapToolId != 0xFF) {
                ninetySixKiloBytes(false);
                libtp::z2audiolib::z2seqmgr::bgmStoop(&libtp::z2audiolib::z2audiomgr::g_mDoAud_zelAudio.mSeqMgr, 20, 0);
            }
        }
        return amoga;
    }

    int32_t Mod::maskDrawOrNoDraw(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mCameraInfo[0].mCameraAttentionStatus & 0x20) {
            if (!firstTimeFirstPersoon) {
                if (saveInfoPtr->maskID == 2) libtp::tp::J3DShape::hideMesh(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[16]->mShape);
                else if (saveInfoPtr->maskID == 3) libtp::tp::J3DShape::hideMesh(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[10]->mShape);
                firstTimeFirstPersoon = true;
            }
        } else if (firstTimeFirstPersoon) {
            if (saveInfoPtr->maskID == 2) libtp::tp::J3DShape::showMesh(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[16]->mShape);
            else if (saveInfoPtr->maskID == 3) libtp::tp::J3DShape::showMesh(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[10]->mShape);
            firstTimeFirstPersoon = false;
        }
        return return_drawMaskOrNot(linkActrPtr);
    }
    
    int32_t Mod::crouchhhhhTime(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        if (saveInfoPtr->MMGlitches == 0) {
            libtp::tp::d_a_alink::decideDoStatus(linkActrPtr);
            //sprintf(sysConsolePtr->consoleLine[4].line, "mDoStatus: 0x%x", libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDoStatus);
            if (isgCarryObjectsSwingCountDown > 0) isgCarryObjectsSwingCountDown--;
            //sprintf(sysConsolePtr->consoleLine[5].line, "chkAtHit: 0x%x", libtp::tp::J3DModel::ChkAtHitPlayerr(&linkActrPtr->mAtCps[0]) || libtp::tp::J3DModel::ChkAtHitPlayerr(&linkActrPtr->mAtCps[1]) || libtp::tp::J3DModel::ChkAtHitPlayerr(&linkActrPtr->mAtCps[2]));
#ifdef PLATFORM_WII
            if ((libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags & 0x800))
#else
            if ((libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x100))
#endif
            {
                if (crouchToISG(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDoStatus, linkActrPtr)) return 0;
                /*if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDoStatus == 0x1F && isgCarryObjectsSwingCountDown > 0) {
                    isgCarryObjectsSwingCountDown = 0;
                    libtp::tp::d_a_alink::deleteEquipItem(linkActrPtr, 0, 0);
                    libtp::tp::d_a_alink::procGrabReadyInit(linkActrPtr);
                    isgStart = true;
                    //isgInitiation(linkActrPtr);
                    crouch = 0; shieldCrouch = 0;
                } else if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDoStatus == 6 && linkActrPtr->mCutType > 0) {
                    isgStart = true;
                    isgInitiation(linkActrPtr);
                    libtp::tp::d_a_alink::checkNextActionFromCrouch(linkActrPtr, 0);
                    libtp::tp::d_a_alink::procDoorOpenInit(linkActrPtr);
                    crouch = 0; shieldCrouch = 0;
                    return 0;
                } else if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDoStatus == 0x80 || libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDoStatus == 0x1C) {
                    isgCarryObjectsSwingCountDown = 0;
                    libtp::tp::d_a_alink::deleteEquipItem(linkActrPtr, 0, 0);
                    isgStart = true;
                    libtp::tp::d_a_alink::checkNextActionFromCrouch(linkActrPtr, 0);
                    libtp::tp::d_a_alink::procCoTalkInit(linkActrPtr);
                    crouch = 0; shieldCrouch = 0;
                    return 0;
                }*/
            }
        }
        return return_crouchhhh(linkActrPtr);
    }

    int32_t Mod::cutTurnChargerInitiation(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        if (noSwordLOL) return 0;
        if (isgStart) {
            isgStart = false;
            isgEnnd(linkActrPtr);
        }
        return return_chargerCutInitiate(linkActrPtr);
    }

    int32_t Mod::cutJumpManIniit(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) {
        if (noSwordLOL) return 0;
        if (isgStart) {
            isgStart = false;
            isgEnnd(linkActrPtr);
        }
        return return_cutJumpInitialized(linkActrPtr, param0);
    }

    int32_t Mod::grassWhistleGetItIt(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        timeSurrenderToProc = true;
        return return_grassWhistleGetItIt(linkActrPtr);
    }

    int32_t Mod::grassWhistleWaitItIt(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
        timeSurrenderToProc = true;
        return return_grassWhistleWaitItIt(linkActrPtr);
    }

    int32_t Mod::layerCommonCommonNumm(const char* param0, int32_t param1, int32_t param2) {
        if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_collect.crystal > 0 && strcmp(param0, "F_SP108") == 0) {
            if (libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x1E08) && !libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x0D04)) return 3;
            if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mRoomNo == 6 && libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x2008)) return 5;
            if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mRoomNo == 0 || (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mRoomNo == 1 && libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mPoint != 0) || libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mRoomNo == 3 || libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mRoomNo == 4) return 2;
            if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mRoomNo == 5 && (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[0x10].temp_flags.memoryFlags[0x1D] & 8))
                return 1;
            if (!(libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[0x10].temp_flags.memoryFlags[0x1D] & 8) && (libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x1E08) || !libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x0C01))) return -1;
        }
        if (strcmp(param0, "F_SP121") == 0) eventFlagResetFunnc(0x0701, true); else {
            if (!(libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0x1D] & 8)) eventFlagResetFunnc(0x0701, false);
            else eventFlagResetFunnc(0x0701, true);
        }
        if (strcmp(param0, "F_SP118") == 0) {
            if (!libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x0B40)) return -1;
        } else if (strcmp(param0, "F_SP109") == 0) {
            if (!(libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[0x11].temp_flags.memoryFlags[0x1D] & 8)) {
                if (!libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x0A08)) {
                    libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[6].temp_flags.memoryFlags[0x17] &= ~0x20; // Epona can cross bridge even if broken
                    libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent.mEvent[0xA] &= ~0x20;
                } else if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent.mEvent[0xF] & 8) {
                    libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent.mEvent[0xA] |= 0x20;
                }
            }
        } else if (!strcmp(param0, "R_SP110")) {
            if (!libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x0704)) return -1;
        } else if (!strcmp(param0, "F_SP114") || !strcmp(param0, "D_MN11")) {
            if ((libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mSave[0x14].temp_flags.memoryFlags[0x1D] & 8)) return 3;
            if (libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x1420)) return 2;
            if (libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x1440)) return 1;
            return -1;
        }
        /*if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_collect.crystal >= 3 && !strcmp(param0, "F_SP109")) {
            if (libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x1320)) return 2;
            else if (libtp::tp::d_save::isEventBit(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent, 0x0701)) {
                sprintf(sysConsolePtr->consoleLine[0].line, "HELP");
                return 12;
            }
        }*/
        return layerNumCommonCommon_return(param0, param1, param2);
    }

    //uint8_t patience = 7;

    //bool bunnyOn = true;

    void Mod::changedLinkLook(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) {
        return_changedLink(linkActrPtr, param0);
        if (firstTime) {
            if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[0] == 0x2f) {
                if (blastNope == false) {
                    //blastNope = true;
#ifndef PLATFORM_WII
                    libtp::tp::j3dmodeldata::J3DModelData* modelMaskTest = linkActrPtr->field_0x064C;
                    linkActrPtr->mMagicArmorBodyBrk = (libtp::tp::J3DAnimation::J3DAnmTevRegKey*)libtp::tp::d_resource::getRes(linkActrPtr->mArcName, 0x18, libtp::tp::d_com_inf_game::dComIfG_gameInfo.mResControl.mObjectInfo, 0x62BE0);
                    libtp::tp::J3DAnimation::searchUpdateMaterialIDModelTev(linkActrPtr->mMagicArmorBodyBrk, modelMaskTest);
                    //libtp::tp::J3DAnimation::searchUpdateMaterialIDMateriall(linkActrPtr->mMagicArmorBodyBrk, &modelMaskTest->mMaterialTable);
                    //libtp::tp::j3dmodeldata::entryTevRegAnimatorDeluxe(modelMaskTest, linkActrPtr->mMagicArmorBodyBrk);
                    libtp::tp::entryTevRegAnimator(&modelMaskTest->mMaterialTable, linkActrPtr->mMagicArmorBodyBrk);
                    linkActrPtr->mMagicArmorBodyBrk->mFrame = linkActrPtr->mMagicArmorBodyBrk->mFrameMax;
#endif
                    //timeRemaining = 15; timeUpdate = 15;
                //strcpy(sysConsolePtr->consoleLine[5].line, "");
                }
                if (saveInfoPtr->maskID != 1) libtp::tp::J3DShape::hideMesh(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[8]->mShape);
                else if (saveInfoPtr->maskID == 1) {
                    bunnyHoodToggle(true);
                }
                if (saveInfoPtr->maskID == 2) {
                    libtp::tp::J3DShape::showMesh(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[16]->mShape);
#ifndef PLATFORM_WII
                    libtp::tp::d_meter2_draw::changeTextureItemB(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw, 0x70);
                    iconBButtonDimFunc(1);
#endif
                }
                if (saveInfoPtr->maskID == 3) enemyNoticeSwitch(true);
                else if (saveInfoPtr->maskID != 3) libtp::tp::J3DShape::hideMesh(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[10]->mShape);
            } else {
                if (saveInfoPtr->maskID != 0) {
                    enemyNoticeSwitch(false);
                    bunnyHoodToggle(false);
                    saveInfoPtr->maskID = 0;
#ifndef PLATFORM_WII
                    iconBButtonDimFunc(0);
                    if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[1] == 0x28)
                        {libtp::tp::d_meter2_draw::changeTextureItemB(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw, 0x28);}
                    else if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[1] == 0x29)
                        {libtp::tp::d_meter2_draw::changeTextureItemB(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw, 0x29);}
                    else if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[1] == 0x3f)
                        {libtp::tp::d_meter2_draw::changeTextureItemB(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw, 0x3f);}
                    else if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[1] == 0x49)
                        {libtp::tp::d_meter2_draw::changeTextureItemB(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw, 0x49);}
#endif
                }
            }
        } else {
            libtp::tp::J3DShape::hideMesh(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[8]->mShape);
            libtp::tp::J3DShape::hideMesh(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[10]->mShape);
        }
        /*if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[0] == 0x2f) {
            if (saveInfoPtr->maskID != 2) {
                libtp::tp::J3DShape::hideMesh(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer->field_0x064C->mMaterialTable.mMaterialNodePointer[16]->mShape);
                libtp::tp::d_a_alink::moveVars.mRunAnmSpeed = 1.5f;
                libtp::tp::d_a_alink::moveVars.mMaxSpeed = 23.0f;
                bunnyOn = false;
                if (saveInfoPtr->maskID == 1) {
                    libtp::tp::J3DShape::showMesh(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer->field_0x064C->mMaterialTable.mMaterialNodePointer[8]->mShape);
                    maskWear = true;
                }
            }
        }
        else saveInfoPtr->maskID = 0;*/
    }

    void Mod::wolfNowComeOn(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0) {
        crouch = 0;
        shieldCrouch = false;
        if (saveInfoPtr->maskID > 0) {
            maskSelection(saveInfoPtr->maskID + 0xE0, false, linkActrPtr);
        }
        return return_wolfNow(linkActrPtr, param0);
    }

    uint8_t storeBuffer[3];
    bool optionalRDetector = false;

    void Mod::initializeTheOption(libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {
#ifndef PLATFORM_WII
        if (libtp::tp::m_do_controller_pad::cpadInfo[0].mHoldLockR)
#else
        if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mButtonFlags & 0x2000)
#endif
        {
            optionalRDetector = true;
            storeBuffer[0] = saveInfoPtr->MMGlitches; storeBuffer[1] = saveInfoPtr->cycle3Days; storeBuffer[2] = saveInfoPtr->dPadChoice;
            optionsMenuPtr->field_0x21c[0][0]->setString(option1);
            optionsMenuPtr->field_0x21c[0][1]->setString(option1);
            optionsMenuPtr->field_0x21c[1][0]->setString(option2);
            optionsMenuPtr->field_0x21c[1][1]->setString(option2);
            optionsMenuPtr->field_0x21c[2][0]->setString(option3);
            optionsMenuPtr->field_0x21c[2][1]->setString(option3);
            if (libtp::tp::J2DPane::searchhh(optionsMenuPtr->mpScreen, 0x6C65745F30335F6E)->checkVisible()) {
                libtp::tp::J2DPane::searchhh(optionsMenuPtr->mpScreen, 0x6C65745F30335F6E)->hideTex();
                libtp::tp::J2DPane::searchhh(optionsMenuPtr->mpScreen, 0x6C65745F30355F6E)->hideTex();
            }
        }

        return return_optionInitialize(optionsMenuPtr);
    }

    void Mod::stringAttentionDude(libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {
        if (!optionalRDetector) return return_attenStringSet(optionsMenuPtr);

        if (saveInfoPtr->MMGlitches == 0) {
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[0][0]->mWindow)->setString(option9);
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[0][1]->mWindow)->setString(option9);
        }
        else {
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[0][0]->mWindow)->setString(option10);
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[0][1]->mWindow)->setString(option10);
        }
    }

    void Mod::stringVibCamDude(libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {
        if (!optionalRDetector) return return_vibCamStringSet(optionsMenuPtr);

        if (saveInfoPtr->cycle3Days == 0) {
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[1][0]->mWindow)->setString(option9);
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[1][1]->mWindow)->setString(option9);
        }
        else {
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[1][0]->mWindow)->setString(option10);
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[1][1]->mWindow)->setString(option10);
        }
    }

    void Mod::stringSoundPointerDude(libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {
        if (!optionalRDetector) return return_soundPointerStringSet(optionsMenuPtr);

        if (saveInfoPtr->dPadChoice == 0) {
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[2][0]->mWindow)->setString(option6);
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[2][1]->mWindow)->setString(option6);
        }
        else if (saveInfoPtr->dPadChoice == 1) {
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[2][0]->mWindow)->setString(option7);
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[2][1]->mWindow)->setString(option7);
        }
        else {
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[2][0]->mWindow)->setString(option8);
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[2][1]->mWindow)->setString(option8);
        }
    }

    void Mod::attentionMoving(libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {
        if (!optionalRDetector) return return_attenMovee(optionsMenuPtr);
#ifndef PLATFORM_WII
        if ((libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x2000000) || (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x1000000))
#else
        if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].stick.x > 0.2f || libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].stick.x < -0.2f)
#endif
        {
            if (saveInfoPtr->MMGlitches == 1) saveInfoPtr->MMGlitches = 0;
            else saveInfoPtr->MMGlitches = 1;
        }

        return return_attenMovee(optionsMenuPtr);
    }

    void Mod::vibCamMoving(libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {
        if (!optionalRDetector) return return_vibCamMovee(optionsMenuPtr);
#ifndef PLATFORM_WII
        if ((libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x2000000) || (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x1000000))
#else
        if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].stick.x > 0.2f || libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].stick.x < -0.2f)
#endif
        {
            if (saveInfoPtr->cycle3Days == 1) saveInfoPtr->cycle3Days = 0;
            else saveInfoPtr->cycle3Days = 1;
        }

        return return_vibCamMovee(optionsMenuPtr);
    }

    void Mod::soundPointerMoving(libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {
        if (!optionalRDetector) return return_soundPointerMovee(optionsMenuPtr);
#ifndef PLATFORM_WII
        if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x2000000)
#else
        if (libtp::tp::J2DPane::searchhh(optionsMenuPtr->mpScreen, 0x6C65745F30335F6E)->checkVisible()) {
            libtp::tp::J2DPane::searchhh(optionsMenuPtr->mpScreen, 0x6C65745F30335F6E)->hideTex();
            libtp::tp::J2DPane::searchhh(optionsMenuPtr->mpScreen, 0x6C65745F30355F6E)->hideTex();
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[3][0]->mWindow)->setString("");
            ((libtp::tp::J2DTextBox::J2DTextBox*)optionsMenuPtr->mpMenuText[3][1]->mWindow)->setString("");
        }
        libtp::tp::J2DPane::searchhh(optionsMenuPtr->mpScreen, 0x6C65745F30335F6E)->hideTex();
        libtp::tp::J2DPane::searchhh(optionsMenuPtr->mpScreen, 0x6C65745F30355F6E)->hideTex();
        if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].stick.x > 0.2f)
#endif
        {
            if (saveInfoPtr->dPadChoice == 2) saveInfoPtr->dPadChoice = 0;
            else saveInfoPtr->dPadChoice++;
        }
#ifndef PLATFORM_WII
        else if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x1000000)
#else
        else if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].stick.x < -0.2f)
#endif
        {
            if (saveInfoPtr->dPadChoice == 0) saveInfoPtr->dPadChoice = 2;
            else saveInfoPtr->dPadChoice--;
        }

        return return_soundPointerMovee(optionsMenuPtr);
    }

#ifdef PLATFORM_WII
    void Mod::theShortCutInitializeed(libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {
        if (optionalRDetector) {
            optionsMenuPtr->field_0x3ef = 2;
            libtp::tp::d_menu_option::pointer_init(optionsMenuPtr);
            return;
        }
        return return_shortCutInitialization(optionsMenuPtr);
    }

    void Mod::theCalibrationInitializeed(libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {
        if (optionalRDetector) {
            optionsMenuPtr->field_0x3ef = 2;
            libtp::tp::d_menu_option::pointer_init(optionsMenuPtr);
            return;
        }
        return return_calibrationInitialization(optionsMenuPtr);
    }
#endif

    void Mod::closingConfirmation(libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {
        if (optionalRDetector) {
            optionalRDetector = false;
            if (!infoTextCheck) {
                saveInfoPtr->MMGlitches = storeBuffer[0]; saveInfoPtr->cycle3Days = storeBuffer[1]; saveInfoPtr->dPadChoice = storeBuffer[2];
            } else {
                if (saveInfoPtr->cycle3Days != storeBuffer[1]) {
                    if (!saveInfoPtr->cycle3Days) {
                        timeSurrenderToMinigame = true;
                        libtp::tp::d_save::setItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 7, 0xE4);
                        libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mPoint = 2;
                        libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mLayer = -1;
                        libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mRoomNo = 1;
                        strcpy(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.mStage, "F_SP104");
                        libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mNextStage.enabled = 1;
                    }
                    else {
                        if (saveInfoPtr->zButtonnItemm == 0xE4) saveInfoPtr->zButtonnItemm = 0xFF;
                        libtp::tp::d_save::setItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, 7, 0xFF);
                    }
                }
                if (saveInfoPtr->MMGlitches != storeBuffer[0]) {
                    if (saveInfoPtr->MMGlitches) {
                        if (isgStart) {
                            isgStart = false;
                            isgCarryObjectsSwingCountDown = 0;
                            isgEnnd(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer);
                        }
                    }
                }
            }
        }

        return return_closedConfirmation(optionsMenuPtr);
    }

    bool confirmationCheck = false;

    void Mod::moveInTheOptionsMenu(libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {
        if (optionalRDetector) {
            if (!confirmationCheck) {
#ifndef PLATFORM_WII
                if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x100)
                {
                    libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags &= ~0x100;
                    libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags |= 0x200;
                    infoTextCheck = true;
                }
                else if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x200)
#else
                if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags & 0x800)
                {
                    libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags &= ~0x800;
                    libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags |= 0x400;
                    infoTextCheck = true;
                }
                else if (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mPressedButtonFlags & 0x400)
#endif
                {
                    infoTextCheck = false;
                }
            }
        }

        return return_moveOptionsMenu(optionsMenuPtr);
    }

    void Mod::openingConfirmation(libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {
        confirmationCheck = true;
        return_openConfirmation(optionsMenuPtr);
        if (optionalRDetector) {
            if (infoTextCheck) optionsMenuPtr->mpWarning->field_0x20->setString(infoText1);
            else optionsMenuPtr->mpWarning->field_0x20->setString(infoText2);
        }
    }

    void Mod::theClosingConfirmation(libtp::tp::d_menu_option::dMenu_Option_c* optionsMenuPtr) {
        confirmationCheck = false;
        return return_theClosedConfirmation(optionsMenuPtr);
    }

    void Mod::retryOver0Hook(libtp::tp::d_menu_save::dMenu_save_c* menuuuuSaveeeeePtrrrrr) {
        if (!outOfTimeItsOver) return return_over0Retry(menuuuuSaveeeeePtrrrrr);
        if (libtp::z2audiolib::z2seqmgr::checkBgmIDPlaying(&libtp::z2audiolib::z2audiomgr::g_mDoAud_zelAudio.mSeqMgr, 0x1000013) != true) {
            menuuuuSaveeeeePtrrrrr->mMenuProc = 0x24;
#ifdef PLATFORM_WII
            performStaticASMReplacement(gameOverTeeeeeext + 0x274, 0x4817c45d);
#else
            performStaticASMReplacement(gameOverCreatee + 0x88, 0x800D8084);
            performStaticASMReplacement(gameOverTeeeeeext + 0x270, 0x48164AB1);
#endif
            outOfTimeItsOver = false;
        }
    }

    bool AreYouCurrentlyInADungeonDude() {
        return ((libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStageData.mStagInfo->field_0x0c >> 16) & 7) == 1 || ((libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStageData.mStagInfo->field_0x0c >> 16) & 7) == 3 || strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, "F_SP104") == 0;
    }

    void Mod::sceneNameGiveIt(libtp::z2audiolib::z2scenemgr::Z2SceneMgr* param0, char* param1, int32_t param2, int32_t param3) {
        if (firstTime && strcmp(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mStage, titleMap) != 0 && saveInfoPtr->cycle3Days == 0) {
            if (!smallTimeLeftHurryyyyy) {
                if (saveInfoPtr->timeRecordedReferr < 360000 && !AreYouCurrentlyInADungeonDude()) {
                    ninetySixKiloBytes(true);
                }
            } else if (smallTimeLeftHurryyyyy && (saveInfoPtr->timeRecordedReferr > 360000 || AreYouCurrentlyInADungeonDude())) {
                ninetySixKiloBytes(false);
            }
        } else if (smallTimeLeftHurryyyyy) {
            ninetySixKiloBytes(false);
        }
        return return_sceneNamee(param0, param1, param2, param3);
    }

    //int32_t Mod::pauseTalk(void* actrThing1, void* actrThing2, uint16_t priority, uint16_t flag) {

        //UiChecking2 = false;
        //return talkCheck(actrThing1, actrThing2, priority, flag);
    //}

    bool timestartt = false;
    uint8_t padingTimer = 0;

    /*void Mod::UICheck2(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr) {
        if (UiChecking == true) {
            UiChecking = false;
        }
        return offUI(dMeterDrawPtr);
    }*/

    //void Mod::UICheck(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr) {
        //if (UiChecking == false) UiChecking = true;
        //libtp::tp::d_a_alink::daAlink* linkActrPtr = libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer;
        //sprintf(sysConsolePtr->consoleLine[6].line, "bunnyFlag: %d", libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer->field_0x06e8->mFlags);
        //sprintf(sysConsolePtr->consoleLine[7].line, "bItemAlpha: %f", libtp::tp::d_meter_hio::g_drawHIO.mHUDAlpha * libtp::tp::d_meter_hio::g_drawHIO.mMainHUDButtonsAlpha);
        //return healthUIOn(dMeterDrawPtr);
    //}
#ifndef PLATFORM_WII
    float yTempPositioo;
    float xTempPositioo;
#endif

    void Mod::zButtonItemRingWait(libtp::tp::d_menu_ring::dMenu_Ring_c* ringMenuPtr) {
#ifndef PLATFORM_WII
        if (reserveCurrentSlott != 0xE6 && !checkFrameSkipVar) {
            ringMenuPtr->mCurrentSlot = reserveCurrentSlott;
            ringMenuPtr->field_0x670 = reserve_0x670;
            reserve_0x670 = 0;
            reserveCurrentSlott = 0xE6;
        }
        if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x10) {
            uint8_t iteeemmmm = libtp::tp::d_save::getItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, ringMenuPtr->mItemSlots[ringMenuPtr->mCurrentSlot], false);
            if (ringMenuPtr->mStatus == 0 && ringMenuPtr->mOldStatus != 3 && ringMenuPtr->mOldStatus != 2) {
                if (iteeemmmm == saveInfoPtr->zButtonnItemm) libtp::tp::m_Do_Audio::mDoAud_seStart(0x4a, 0, 0, 0);
                else if (!ringMenuPtr->mPlayerIsWolf && iteeemmmm != 0xff) {
                    libtp::tp::J2DPane::J2DPane* zTexItemVisuaaal = libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D69646F6E61);
                    libtp::tp::m_Do_Audio::mDoAud_seStart(0x4f, 0, 0, 0);
                    //libtp::tp::d_menu_ring::setItemFromItemRing(ringMenuPtr);
                    xTempPositioo = (ringMenuPtr->mItemSlotPosX[ringMenuPtr->mCurrentSlot] + 300.0f) - 330.0f; yTempPositioo = (ringMenuPtr->mItemSlotPosY[ringMenuPtr->mCurrentSlot] - 400.0f) + 15.0f + itemZYPosMemory;
                    libtp::tp::J2DPane::movePaneeClass(zTexItemVisuaaal, xTempPositioo, yTempPositioo);
                    /*libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosX = (ringMenuPtr->mItemSlotPosX[ringMenuPtr->mCurrentSlot] - 244.0f) - 330.0f;
                    libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosY = (ringMenuPtr->mItemSlotPosY[ringMenuPtr->mCurrentSlot] - 59.0f) + 15.0f;*/
                    // scale: 1.1f
                    //libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconScale = 2.0f;
                    zButtonTextureMomentInit(iteeemmmm);
                    saveInfoPtr->zButtonSloot = ringMenuPtr->mItemSlots[ringMenuPtr->mCurrentSlot];
                    ammoItemZTime(iteeemmmm, ringMenuPtr->mItemSlots[ringMenuPtr->mCurrentSlot]);
                    itemMenuZAnimate = true;
                    itemMenuZAnimate2 = 3;
                    if (iteeemmmm == libtp::tp::d_save::getItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, ringMenuPtr->mItemSlots[ringMenuPtr->mXButtonSlot], false) || iteeemmmm == libtp::tp::d_save::getItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, ringMenuPtr->mItemSlots[ringMenuPtr->mYButtonSlot], false)) {
                        //libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags &= ~0x10;
                        uint8_t xOrYItem = 0;
                        //libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags |= 0x400;
                        if (iteeemmmm == libtp::tp::d_save::getItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, ringMenuPtr->mItemSlots[ringMenuPtr->mXButtonSlot], false)) xOrYItem = 0;
                        else xOrYItem = 1;
                        for (uint8_t slotParameterr = 0; slotParameterr < 0x18; slotParameterr++) {
                            if (saveInfoPtr->zButtonnItemm == libtp::tp::d_save::getItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, ringMenuPtr->mItemSlots[slotParameterr], false)) {
                                if (saveInfoPtr->zButtonnItemm != 0xFF) {
                                    reserveCurrentSlott = ringMenuPtr->mCurrentSlot;  ringMenuPtr->mCurrentSlot = slotParameterr;
                                    reserve_0x670 = ringMenuPtr->field_0x670;         ringMenuPtr->field_0x670 = ringMenuPtr->field_0x63E[ringMenuPtr->mCurrentSlot];
                                    if (!xOrYItem) libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags |= 0x400;
                                    else libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags |= 0x800;
                                    //XYItemAnySet(xOrYItem + 1);
                                    checkFrameSkipVar = true;
                                } else {
                                    //sprintf(sysConsolePtr->consoleLine[16].line, "Test: 0x%x", ringMenuPtr->mXButtonSlot);
                                    if (!xOrYItem) {
                                        ringMenuPtr->field_0x6b4[0] = 0xFF; ringMenuPtr->field_0x674[0] = 1; ringMenuPtr->mXButtonSlot = 0xFF;
                                    } else {
                                        ringMenuPtr->field_0x6b4[1] = 0xFF; ringMenuPtr->field_0x674[1] = 1; ringMenuPtr->mYButtonSlot = 0xFF;
                                    }
                                    ringMenuPtr->field_0x6b8[xOrYItem] = 0xFF;
                                    ringMenuPtr->field_0x6cb = 0xFF;
                                    ringMenuPtr->field_0x6cd = 0xFF;
                                    declineButtoonnRequest = xOrYItem;
                                    //ringMenuPtr->field_0x6b3 = 1;
                                    libtp::tp::d_menu_ring::setJumpItem(ringMenuPtr, true);
                                }
                                break;
                            }
                        }
                    }
                    saveInfoPtr->zButtonnItemm = iteeemmmm;
                }
            }
        }
        if (itemMenuZAnimate) {
            libtp::tp::J2DPane::J2DPane* zTexItemVisuaaal = libtp::tp::J2DPane::searchhh(libtp::tp::d_meter2_info::g_meter2_info.mMeterClass->mpMeterDraw->mpScreen, 0x6D69646F6E61);
            /*if (libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosX < 0.0f) {
                libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosX += 25.0f;
                if (libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosX > 0.0f) {libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosX = 0.0f; itemMenuZAnimate2--;}
            }
            if (libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosY > 0.0f) {
                libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosY -= 25.0f;
                if (libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosY < 0.0f) {libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosY = 0.0f; itemMenuZAnimate2--;}
            }
            if (libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconScale > 1.1f) {
                libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconScale -= 0.25f;
                if (libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconScale < 1.1f) {libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconScale = 1.1f; itemMenuZAnimate2--;}
            }*/
            //float xTempPositioo = zTexItemVisuaaal->xPositionCheck(); float yTempPositioo = zTexItemVisuaaal->yPositionCheck();
            if (xTempPositioo < 515.0f) {
                xTempPositioo += 25.0f;
                if (xTempPositioo >= 515.0f) {xTempPositioo = 515.0f; itemMenuZAnimate2--;}
            }
            if (yTempPositioo > -350.0f + itemZYPosMemory) {
                yTempPositioo -= 25.0f;
                if (yTempPositioo <= -350.0f + itemZYPosMemory) {yTempPositioo = -350.0f + itemZYPosMemory; itemMenuZAnimate2-=2;}
            }
            libtp::tp::J2DPane::movePaneeClass(zTexItemVisuaaal, xTempPositioo, yTempPositioo);
            if (!itemMenuZAnimate2) itemMenuZAnimate = false;
        }
        /*sprintf(sysConsolePtr->consoleLine[17].line, "posX: %f", libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosX);
        sprintf(sysConsolePtr->consoleLine[18].line, "posY: %f", libtp::tp::d_meter_hio::g_drawHIO.mMidnaIconPosY);
        sprintf(sysConsolePtr->consoleLine[19].line, "slotZItem: 0x%x", saveInfoPtr->zButtonnItemm);
        sprintf(sysConsolePtr->consoleLine[20].line, "oldStatus: %d", ringMenuPtr->mOldStatus);
        sprintf(sysConsolePtr->consoleLine[21].line, "currentStatus: %d", ringMenuPtr->mStatus);*/
        if (checkFrameSkipVar) checkFrameSkipVar = false;
#endif
        //sprintf(sysConsolePtr->consoleLine[1].line, "aa: %d", ringMenuPtr->mOldStatus);
        if (ringMenuPtr->mOldStatus == 1) {
            uint8_t itemmmm = libtp::tp::d_save::getItem(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_item, ringMenuPtr->mItemSlots[ringMenuPtr->mCurrentSlot], false);
            if (itemmmm > 0xE0 && itemmmm < 0xE5) masksHaveNames_HereAreTheNames(itemmmm, ringMenuPtr);
        }
        return return_zButtonRingWait(ringMenuPtr);
    }

    //void Mod::zItemSeleect2(libtp::tp::d_menu_ring::dMenu_Ring_c* ringMenuPtr, int32_t param0, uint8_t param1) {
        /*if (declineButtoonnRequest != param0)*/ //return return_zItemSeleect2(ringMenuPtr, param0, param1);
        //declineButtoonnRequest = 4;
    //}

    //libtp::tp::J3DGXColorS10 testColors = {0xaa, 0xff, 0x00, 0xff};
    //libtp::tp::J3DGXColorS10 testColors2 = {0x00, 0x00, 0xff, 0xff};
    //libtp::tp::J3DBlendInfo* testBlendThingy;
    //const char brkTestName[] = "testtBrk.brk";
    //const char greenArcNamee[] = "Kmdl";

    //libtp::tp::J3DGXColorS10 

    bool bomba = false;
    bool bamba = false;
    uint8_t trimer = 0;
    uint8_t bombytimer = 0;

    void Mod::valueOfTheMeter(libtp::tp::d_meter2::dMeter2_c* dMeter2Ptr) {
        if (((dMeter2Ptr->mStatus & 0x40) == 0 && !libtp::tp::d_meter2_info::g_meter2_info.mMenuWindowClass->mMenuStatus) && !gameplayStatus) {gameplayStatus = true; second = libtp::gc_wii::os_time::OSGetTime(); timeRemaining = timeUpdate+1; second2 = libtp::gc_wii::os_time::OSGetTime(); timeRemaininng2 = timeUpdaate2+1;}
        else if (((dMeter2Ptr->mStatus & 0x40) != 0 || libtp::tp::d_meter2_info::g_meter2_info.mMenuWindowClass->mMenuStatus > 0) && gameplayStatus) {gameplayStatus = false; if ((dMeter2Ptr->mStatus & 0x40) != 0) {crouch = 0; shieldCrouch = false;}}
        libtp::tp::d_a_alink::daAlink* linkActrPtr = libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer;
        //sprintf(sysConsolePtr->consoleLine[2].line, "mProcID: %x", libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer->mProcID);
        //sprintf(sysConsolePtr->consoleLine[3].line, "ReachTopSpeed: %d", libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mStartStage.mRoomNo);
        timerPauseOrKeepMoving();
        if (gameplayStatus) {
            remainingCountDownTimerr();
            if (saveInfoPtr->maskID > 0) {
                for (uint8_t tempItemObservation = 0xE1; tempItemObservation < 0xE4; tempItemObservation++) {
                    if (saveInfoPtr->maskID == tempItemObservation - 0xE0) {
#ifdef PLATFORM_WII
                        if (libtp::tp::d_a_alink::checkItemSetButton(linkActrPtr, tempItemObservation) == 4)
#else
                        if (libtp::tp::d_a_alink::checkItemSetButton(linkActrPtr, tempItemObservation) == 3)
#endif
                        {
                            maskSelection(tempItemObservation, false, linkActrPtr); break;
                        }
                    }
                }
            }
            if (blastNope == false) {
                for (counting = timeRemaining; counting >= 0; counting--) {
                    if ((libtp::gc_wii::os_time::OSGetTime() - second) <= static_cast<int64_t>(( counting * (libtp::gc_wii::os::__OSBusClock/4)))) {
#ifdef PLATFORM_WII
                        sprintf(sysConsolePtr->consoleLine[5].line, "Time Left: %lld", timeRemaining - counting);
#endif
                        timeUpdate = timeRemaining - counting;
                    }
                }
                if ((libtp::gc_wii::os_time::OSGetTime() - second) >= static_cast<int64_t>(( timeRemaining * (libtp::gc_wii::os::__OSBusClock/4)))) {
                    blastNope = true;
                    if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[0] == 0x2F) {
#ifndef PLATFORM_WII
                        libtp::tp::j3dmodeldata::J3DModelData* modelMaskTest = linkActrPtr->field_0x064C;
                        linkActrPtr->mMagicArmorBodyBrk = (libtp::tp::J3DAnimation::J3DAnmTevRegKey*)libtp::tp::d_resource::getRes(linkActrPtr->mArcName, 0x19, libtp::tp::d_com_inf_game::dComIfG_gameInfo.mResControl.mObjectInfo, 0x62BE0);
                        libtp::tp::J3DAnimation::searchUpdateMaterialIDModelTev(linkActrPtr->mMagicArmorBodyBrk, modelMaskTest);
                        libtp::tp::entryTevRegAnimator(&modelMaskTest->mMaterialTable, linkActrPtr->mMagicArmorBodyBrk);
                        linkActrPtr->mMagicArmorBodyBrk->mFrame = 0.0f;
#endif
                        if (linkActrPtr->mProcID == 0x1C || linkActrPtr->mEquipItem == 0x103) iconBButtonDimFunc(2);
                        else iconBButtonDimFunc(0);
                    }
                    timeRemaining = 15;
                strcpy(sysConsolePtr->consoleLine[5].line, "");
                }
            }
            if (maskBoom) {
                maskBoom = false;
            }
#ifdef PLATFORM_WII
            if (saveInfoPtr->maskID == 2 && libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].wiimote_shake > 0.2f)
#else
            if (saveInfoPtr->maskID == 2 && (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x200))
#endif
            {
                if (blastNope) {
                    maskBoom = true;
                    blastNope = false;
#ifndef PLATFORM_WII
                    libtp::tp::j3dmodeldata::J3DModelData* modelMaskTest = linkActrPtr->field_0x064C;
                    linkActrPtr->mMagicArmorBodyBrk = (libtp::tp::J3DAnimation::J3DAnmTevRegKey*)libtp::tp::d_resource::getRes(linkActrPtr->mArcName, 0x18, libtp::tp::d_com_inf_game::dComIfG_gameInfo.mResControl.mObjectInfo, 0x62BE0);
                    libtp::tp::J3DAnimation::searchUpdateMaterialIDModelTev(linkActrPtr->mMagicArmorBodyBrk, modelMaskTest);
                    //libtp::tp::J3DAnimation::searchUpdateMaterialIDMateriall(linkActrPtr->mMagicArmorBodyBrk, &modelMaskTest->mMaterialTable);
                    //libtp::tp::j3dmodeldata::entryTevRegAnimatorDeluxe(modelMaskTest, linkActrPtr->mMagicArmorBodyBrk);
                    libtp::tp::entryTevRegAnimator(&modelMaskTest->mMaterialTable, linkActrPtr->mMagicArmorBodyBrk);
                    linkActrPtr->mMagicArmorBodyBrk->mFrame = 0.0f;
#endif
                    //linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[16]->mPEBlock->blendAcquire()->mType = 3;
                    //testBlendThingy = linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[16]->mPEBlock->blendAcquire();
                    //testBlendThingy->mType = 3;
                    //linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[16]->mPEBlock->blendSett(testBlendThingy);
                    //libtp::tp::patchMaterialTest(linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[16]);
                    //linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[16]->mPEBlock->fullPeBlockLoadPlz();
                    iconBButtonDimFunc(1);
                    cXyz create_pos = cXyzMultiply(cXyzAdd(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer->mLeftHandPos, libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer->mRightHandPos), 0.5f);
                    if (libtp::tp::d_bg_s_acch::ChkWaterIn(&libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer->mLinkAcch)) libtp::tp::d_bomb::createWaterBombExplode(&create_pos);
                    else libtp::tp::d_bomb::createNormalBombExplode(&create_pos);
                    second = libtp::gc_wii::os_time::OSGetTime();
                }
            }
#ifndef PLATFORM_WII
            if (linkActrPtr->mProcID == 0x1C && libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x200)
#else
            if (linkActrPtr->mProcID == 0x1C && libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].wiimote_shake > 0.2f)
#endif
            {
                /*libtp::tp::d_a_alink::huLightVars.lanternAmbienceR = libtp::tp::d_a_alink::lanternVars.innerSphereR = libtp::tp::d_a_alink::lanternVars.outerSphereR = 0x43;
                libtp::tp::d_a_alink::huLightVars.lanternAmbienceG = libtp::tp::d_a_alink::lanternVars.innerSphereG = libtp::tp::d_a_alink::lanternVars.outerSphereG = 0xcb;
                libtp::tp::d_a_alink::huLightVars.lanternAmbienceB = libtp::tp::d_a_alink::lanternVars.innerSphereB = libtp::tp::d_a_alink::lanternVars.outerSphereB = 0x34;*/
                //libtp::tp::setTevColor(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer->field_0x064C->mMaterialTable.mMaterialNodePointer[0]->mTevBlock, 0, libtp::tp::J3DGXColorS10_ct4(, testColors));
                //libtp::tp::setTevColor(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer->field_0x064C->mMaterialTable.mMaterialNodePointer[0]->mTevBlock, 1, &testColors2);
                if (linkActrPtr->mCutType == 0 && saveInfoPtr->maskID != 2 && !noSwordLOL)
                {
                    if (linkActrPtr->mEquipItem != 0x103) {
                        if (linkActrPtr->mEquipItem != 0xff) libtp::tp::d_a_alink::deleteEquipItem(linkActrPtr, 0, 0);
                        libtp::tp::d_a_alink::setSwordModel(linkActrPtr);
                    }
                    if (saveInfoPtr->MMGlitches == 0 && (libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDoStatus == 0x1F || libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mDoStatus == 0x80)) {
                        isgCarryObjectsSwingCountDown = 10;
                        /*libtp::tp::d_a_alink::setUpperAnime(linkActrPtr, 0x80, libtp::tp::d_a_alink::UPPER_2, libtp::tp::d_a_alink::cutLeftVars.mCutAnm.mSpeed, 
                        libtp::tp::d_a_alink::cutLeftVars.mCutAnm.mStartFrame, libtp::tp::d_a_alink::cutLeftVars.mCutAnm.mEndFrame, libtp::tp::d_a_alink::cutLeftVars.mCutAnm.mInterpolation);
                        libtp::tp::d_a_alink::setFacePriTexture(linkActrPtr, libtp::tp::d_a_alink::FTANM_UNK_13);
                        libtp::tp::d_a_alink::setFaceBck(linkActrPtr, 0x104, 1, 0xFFFF);*/
#ifdef PLATFORM_WII
                        if (!libtp::tp::d_a_player::checkNoResetFlg0(linkActrPtr, libtp::tp::d_a_player::FLG0_UNDERWATER)) libtp::tp::d_a_alink::voiceStart(linkActrPtr, 0x1001C);
#else
                        libtp::tp::d_a_alink::setSwordVoiceSe(linkActrPtr, 0x1001C);
#endif
                        /*libtp::tp::d_a_alink::setCutWaterDropEffect(linkActrPtr);*/
                    } else libtp::tp::d_a_alink::setCutDash(linkActrPtr,0,0);
                }
            }
#ifdef PLATFORM_WII
            if (dpaaadDown && (libtp::tp::m_re_controller_pad::mReCPd::m_pad[0].mButtonFlags & 0x2000) == 0 && !libtp::tp::d_a_alink::checkFmChainGrabAnime(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer) && libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[2] != 0xFF && (linkActrPtr->mProcID < 5 || linkActrPtr->mProcID == 0x1C || (linkActrPtr->mProcID > 0x34 && linkActrPtr->mProcID < 0x39) || linkActrPtr->mProcID == 0x71))
#else
                if (libtp::tp::m_do_controller_pad::cpadInfo[0].mHoldLockR && libtp::tp::m_do_controller_pad::cpadInfo[0].mHoldLockL == 0 && !libtp::tp::d_a_alink::checkFmChainGrabAnime(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer) && libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[2] != 0xFF && (linkActrPtr->mProcID < 5 || linkActrPtr->mProcID == 0x1C || (linkActrPtr->mProcID > 0x34 && linkActrPtr->mProcID < 0x39) || linkActrPtr->mProcID == 0x71))
#endif
            {
                if (crouch == 0) {
                    libtp::tp::d_a_alink::procCrouchInit(linkActrPtr);
                    crouch = 1;
                }
            } else if (crouch != 0 && ((linkActrPtr->mProcID > 1 && linkActrPtr->mProcID < 5) || linkActrPtr->mProcID == 0x1C)) {
                libtp::tp::d_a_alink::checkNextActionFromCrouch(linkActrPtr, 0);
                crouch = 0;
                shieldCrouch = false;
                if (linkActrPtr->mEquipItem == 0x103 && saveInfoPtr->maskID == 2) iconBButtonDimFunc(2);
                else iconBButtonDimFunc(0);
            }

            if (linkActrPtr->mProcID == 0x1D && shieldCrouch) {
                libtp::tp::d_a_alink::procCrouchInit(linkActrPtr);
                timestartt = true;
            } else if (timestartt) {
                padingTimer++;
                if (padingTimer == 15) {
                    padingTimer = 0;
                    crouch = 0;
                    shieldCrouch = false;
                    iconBButtonDimFunc(0);
                    timestartt = false;
#ifdef PLATFORM_WII
                    if (dpaaadDown)
#else
                    if (libtp::tp::m_do_controller_pad::cpadInfo[0].mHoldLockR)
#endif
                    {
                        linkActrPtr->mProcID = 0x1D;
                        libtp::tp::d_a_alink::procCrouchInit(linkActrPtr);
                    }
                }
            }

            if (saveInfoPtr->dPadChoice == 0) {
#ifdef PLATFORM_WII
                if (libtp::tp::d_com_inf_game::dComIfGs_isItemFirstBit(0x31) && bomba == false && dpaaadRight && libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentForm == 0)
#else
                if (libtp::tp::d_com_inf_game::dComIfGs_isItemFirstBit(0x31) && bomba == false && (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 4) && libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.currentForm == 0)
#endif
                {
                    bamba = true;
                }
                if (bamba) {
                    trimer++;
                    if (trimer >= 15) {
                        bamba = false;
                        bomba = true;
                        trimer = 0;
                        bombytimer = 0;
                        libtp::tp::d_a_alink::setClothesChange(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer, 0);
                        libtp::tp::m_Do_Audio::mDoAud_seStart(0x4f, 0, 0, 0);
                        if (libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.equipment[0] != 0x31) libtp::tp::d_com_inf_game::setSelectEquipClothes(0x31);
                        else libtp::tp::d_com_inf_game::setSelectEquipClothes(0x2f);
                    }
                }
                if (bomba == true) {
                    bombytimer++;
                    if (bombytimer >= 20) {
                        bomba = false;
                        bombytimer = 0;
                    }
                }
            }
#ifndef PLATFORM_WII
        else if (saveInfoPtr->dPadChoice == 1 && (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 4)) handleQuickTransform();
#else
        else if (saveInfoPtr->dPadChoice == 1 && dpaaadRight) handleQuickTransform();
#endif
        }
        /*else {
            dMeter2Ptr->mpMeterDraw->field_0x724 = 1.5f;
            dMeter2Ptr->mpMeterDraw->mZButtonAlpha = 1.5f;
        }*/
        //dMeter2Ptr->mpMeterDraw->mButtonBAlpha = 0.9f;

        // used for dimming B Button item icon
        //libtp::tp::d_meter_hio::g_drawHIO.mXYButtonsItemDimAlpha = 255;
        //uint8_t thingyy = dMeter2Ptr->mpMeterDraw->mpItemBPane->mWhite.a;
        //libtp::tp::d_kankyo::env_light.daytime = 350.0f;
    //sprintf(sysConsolePtr->consoleLine[3].line, "gamePlayStatus: %d", gameplayStatus);
    //sprintf(sysConsolePtr->consoleLine[2].line, "cutsceneMaybe: %d", libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mEvent.mMapToolId);
    /*if (libtp::tp::m_do_controller_pad::cpadInfo[0].mPressedButtonFlags & 0x100) {
        libtp::tp::d_a_player::onNoResetFlg2(linkActrPtr, libtp::tp::d_a_player::FLG2_FORCE_GAMEOVER);
        libtp::tp::d_a_alink::procCoDeadInit(linkActrPtr, 0);
    }*/
        /*sprintf(sysConsolePtr->consoleLine[3].line, "dayNight: %d", libtp::tp::d_kankyo::dKy_daynight_check());
        sprintf(sysConsolePtr->consoleLine[5].line, "mirror: %d", libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_collect.mirror);
        sprintf(sysConsolePtr->consoleLine[6].line, "dayTime: %f", libtp::tp::d_kankyo::env_light.daytime);
        sprintf(sysConsolePtr->consoleLine[7].line, "gamePlayStatus: %d", gameplayStatus);
        sprintf(sysConsolePtr->consoleLine[8].line, "crouch: %d", crouch);
        sprintf(sysConsolePtr->consoleLine[9].line, "shieldCrouch: %d", shieldCrouch);
        sprintf(sysConsolePtr->consoleLine[10].line, "maskId: %d", saveInfoPtr->maskID);
        sprintf(sysConsolePtr->consoleLine[13].line, "mProcId: 0x%x", linkActrPtr->mProcID);
        sprintf(sysConsolePtr->consoleLine[15].line, "Snow warp: %d", libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.player.player_status_a.maxHealth);*/
        /*sprintf(sysConsolePtr->consoleLine[17].line, "startOrNoStart: %d", libtp::tp::d_timer::dTimer_isStart());
        sprintf(sysConsolePtr->consoleLine[19].line, "slotZItem: 0x%x", saveInfoPtr->zButtonnItemm);
        if (libtp::tp::d_meter2_info::g_meter2_info.mMeterMap != nullptr) sprintf(sysConsolePtr->consoleLine[21].line, "mapHeight: %f", libtp::tp::d_meter2_info::g_meter2_info.mMeterMap->mSizeH);*/
        //sprintf(sysConsolePtr->consoleLine[14].line, "FirstPersonMode: %d", libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mCameraInfo[0].mCameraAttentionStatus & 0x20);
        //sprintf(sysConsolePtr->consoleLine[15].line, "isUseButton: %d", 4 & libtp::tp::d_meter2_info::g_meter2_info.mUseButton);
        //sprintf(sysConsolePtr->consoleLine[10].line, "blendMode: %d", linkActrPtr->field_0x064C->mMaterialTable.mMaterialNodePointer[16]->mPEBlock->blendAcquire()->mType);
        return meterValuee(dMeter2Ptr);
    }

    int32_t Mod::guardHasBroke(libtp::tp::d_a_alink::daAlink* linkActrPtr) {
#ifndef PLATFORM_WII
        if (libtp::tp::m_do_controller_pad::cpadInfo[0].mButtonFlags & 0x20) crouch = 0;
#else
        if (dpaaadDown) crouch = 0;
#endif
        return return_breakThatGuardNow(linkActrPtr);
    }

    int32_t Mod::dudeTheDamageHurts(libtp::tp::d_a_alink::daAlink* linkActrPtr, void* objjjj, int32_t unnnnnk) {
        if (isgStart) {
            isgEnnd(linkActrPtr);
            isgStart = false;
            isgCarryObjectsSwingCountDown = 0;
        }
#ifndef PLATFORM_WII
        if (libtp::tp::m_do_controller_pad::cpadInfo[0].mButtonFlags & 0x20) crouch = 0;
#else
        if (dpaaadDown) crouch = 0;
#endif
        return return_regularDamageHurts(linkActrPtr, objjjj, unnnnnk);
    }

    bool customItemOrNo = false;
    uint8_t currentCustomItemNoChosen = 0;

    void itemTextureMoment(uint8_t valueButInHex) {
        uint32_t itemIconOffset;
        if (valueButInHex == 0xE3) itemIconOffset = 0x38c0001e;
        else if (valueButInHex == 0xE1) itemIconOffset = 0x38c00079;
        else itemIconOffset = 0x38c0007a;
#ifdef PLATFORM_WII
        *reinterpret_cast<uint32_t*>(readIconsTex + 0x8C) = itemIconOffset;
        libtp::memory::clear_DC_IC_Cache(reinterpret_cast<uint32_t*>(readIconsTex + 0x8C), sizeof(uint32_t));
#else
        *reinterpret_cast<uint32_t*>(readIconsTex + 0x94) = itemIconOffset;
        libtp::memory::clear_DC_IC_Cache(reinterpret_cast<uint32_t*>(readIconsTex + 0x94), sizeof(uint32_t));
#endif
    }

    int32_t Mod::reeadTheItemTexture(libtp::tp::d_meter2_info::G_Meter2_Info* g_meter2_info_ptr, uint8_t itemNo, void* param_1, 
                                libtp::tp::J2DPicture::J2DPicture* param_2, void* param_3, 
                                libtp::tp::J2DPicture::J2DPicture* param_4, void* param_5, 
                                libtp::tp::J2DPicture::J2DPicture* param_6, void* param_7, 
                                libtp::tp::J2DPicture::J2DPicture* param_8, int32_t param_9) {
        if (currentCustomItemNoChosen > 0xE0 && currentCustomItemNoChosen < 0xE4) {
            if (itemNo > 0xE0 && itemNo < 0xE4 && itemNo != currentCustomItemNoChosen) customItemOrNo = false;
        }
        if (itemNo > 0xE0 && itemNo < 0xE4)
#ifdef PLATFORM_WII
        {
            if (!customItemOrNo) {
                customItemOrNo = true;
                //sprintf(sysConsolePtr->consoleLine[7].line, "Yes!");
                libtp::patch::writeBranch(readIconsTex + 0x54, readIconsTex + 0x78);
                itemTextureMoment(itemNo);
                currentCustomItemNoChosen = itemNo;
            }
        } else if (customItemOrNo) {
            customItemOrNo = false;
            //sprintf(sysConsolePtr->consoleLine[7].line, "No!");
            performStaticASMReplacement(readIconsTex + 0x54, 0x28000048);
            performStaticASMReplacement(readIconsTex + 0x8C, 0x38c00023);
            //currentCustomItemNoChosen = 0;
        }
#else
        {
            if (!customItemOrNo) {
                customItemOrNo = true;
                //sprintf(sysConsolePtr->consoleLine[7].line, "Yes!");
                libtp::patch::writeBranch(readIconsTex + 0x58, readIconsTex + 0x80);
                itemTextureMoment(itemNo);
                currentCustomItemNoChosen = itemNo;
            }
        } else if (customItemOrNo) {
            customItemOrNo = false;
            //sprintf(sysConsolePtr->consoleLine[7].line, "No!");
            performStaticASMReplacement(readIconsTex + 0x58, 0x28000048);
            performStaticASMReplacement(readIconsTex + 0x94, 0x38c00023);
        }
#endif
        return return_itemTextureRead(g_meter2_info_ptr, itemNo, param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9);
    }

    uint16_t idEnemyStorageTemp[10] = {0, 0};

    uint32_t enemyRelPtrAddress[10] = {0, 0};

    // Only run when toggling mask on/off
    void Mod::enemyNoticeSwitch(bool noticedd) {
        if (idEnemyStorageTemp[0] == 0) return;
        using namespace libtp::tp::rel::relIDs;
        uint8_t addressOffsetSmthgSmthg = 0;
        if (noticedd) {
            while (idEnemyStorageTemp[addressOffsetSmthgSmthg] != 0) {
                switch (idEnemyStorageTemp[addressOffsetSmthgSmthg]) {
                    // Armos
                    case D_A_E_AI:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x5D0, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x6F8, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x584, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x6C4, 0x38600000);
#endif
                        break;
                    }
                    // Keese
                    case D_A_E_BA:
                    {
#ifndef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x564, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x814, 0x38600000);
#endif
                        break;
                    }
                    // Bombling
                    case D_A_E_BI:
                    {
#ifndef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x524, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x5A8, 0x38600000);
#endif
                        break;
                    }
                    // Stallkin
                    case D_A_E_BS:
                    {
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + bs_e_address[0], 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + bs_e_address[1], 0x38600000);
                        break;
                    }
                    // Bubble
                    case D_A_E_BU:
                    {
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + bu_e_address, 0x38600000);
                        break;
                    }
                    // Poison Mite
                    /*case D_A_E_BUG:
                    {
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xF00, 0x38600000);
                        break;
                    }*/
                    // Deku Baba
                    case D_A_E_DB:
                    {
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + db_e_address[0], 0x38600000);
                        break;
                    }
                    // Dodongo
                    case D_A_E_DD:
                    {
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + dd_e_address, 0x38600000);
                        break;
                    }
                    // Lizalfos
                    case D_A_E_DN:
                    {
#ifdef PLATFORM_WII
                        libtp::patch::writeBranch(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x2F3C, enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x3128);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x123C, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1278, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1270, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x12E4, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1320, 0x38600000);
#endif
                        break;
                    }
                    // Freezard
                    case D_A_E_FB:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xBA0, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xC38, 0x38600000);
#endif
                        break;
                    }
                    // (Mini) Freezard
                    case D_A_E_FZ:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1DA4, ASM_NOP);
                        libtp::patch::writeBranch(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x14DC, enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1370);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1BA4, ASM_NOP);
                        libtp::patch::writeBranch(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x12D8, enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x115C);
#endif
                        break;
                    }
                    case D_A_E_HB:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x844, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x550, 0x38600000);
#endif
                        break;
                    }
                    // Torch Slug
                    case D_A_E_HM:
                    {
#ifdef PLATFORM_WII
                        libtp::patch::writeBranch(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x2E30, enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x2E68);
#else
                        libtp::patch::writeBranch(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x3578, enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x35B0);
                        //performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x3284, ASM_NOP);
#endif
                        break;
                    }
                    // Tile Worms
                    case D_A_E_HZ:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x7D8, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x94C, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA74, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xBEC, 0x38600000);
#endif
                        break;
                    }
                    // Young Gohma
                    case D_A_E_KG:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x478, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x4DC, 0x38600000);
#endif
                        break;
                    }
                    // Chilfos
                    case D_A_E_KK:
                    {
#ifdef PLATFORM_WII
                        libtp::patch::writeBranch(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1120, enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x11E0);
#else
                        libtp::patch::writeBranch(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xECC, enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xF40);
#endif
                        break;
                    }
                    // Kargarok
                    case D_A_E_KR:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x8D0, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x938, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA8C, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xAFC, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x674, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x7A0, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x8F8, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x96C, 0x38600000);
                        //libtp::patch::writeBranch(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1650, enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xE4C);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x4918, ASM_NOP);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x493C, ASM_NOP);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x4948, ASM_NOP);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x4970, ASM_NOP);
#endif
                        break;
                    }
                    // Dynalfos
                    case D_A_E_MF:
                    {
                        //performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1218, 0x38600000);
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xEC8, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xF04, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x128C, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x12C8, 0x38600000);
#endif
                        break;
                    }
                    // Rat
                    case D_A_E_MS:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x400, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x2E4, 0x38600000);
#endif
                        break;
                    }
                    case D_A_E_ST:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xC84, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA34, 0x38600000);
#endif
                        break;
                    }
                    // Bokoblin
                    case D_A_E_OC:
                    {
#ifdef PLATFORM_WII
                        libtp::patch::writeBranch(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x904, enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x9B8);
                        libtp::patch::writeBranch(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA8C, enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xBB4);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xC88, 0x38600000);
#else
                        libtp::patch::writeBranch(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA24, enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xAD8);
                        libtp::patch::writeBranch(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xBA8, enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xC34);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xDAC, 0x38600000);
#endif
                        break;
                    }
                    // Bulblin
                    case D_A_E_RD:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1D10, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1D58, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1E28, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1E70, 0x38600000);
#endif
                        break;
                    }
                    // Stalfos
                    case D_A_E_SF:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x914, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x938, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x980, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x9A4, 0x38600000);
#endif
                        break;
                    }
                    // Moldorm
                    case D_A_E_SW:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x5670, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x6790, 0x38600000);
#endif
                        break;
                    }
                    // Water Toadpoli
                    case D_A_E_TK:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x428, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x494, 0x38600000);
#endif
                        break;
                    }
                    // Fire ToadPoli
                    case D_A_E_TK2:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x3F0, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x45C, 0x38600000);
#endif
                        break;
                    }
                    // Tektite
                    case D_A_E_TT:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x14D4, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1768, 0x38600000);
#endif
                        break;
                    }
                    case D_A_E_MM:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x15B4, 0x38000000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x3FC, 0x38600000);
#endif
                        break;
                    }
                    case D_A_E_S1:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x4E8, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x50C, 0x38600000);
#endif
                        break;
                    }
                    case D_A_E_SM2:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1340, 0x38000000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA4C, 0x38600000);
#endif
                        break;
                    }
                    // Wall Skulltula
                    case D_A_E_WS:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x7BC, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x9B4, 0x38600000);
#endif
                        break;
                    }
                    case D_A_E_YG:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x764, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x2E4, 0x38600000);
#endif
                        break;
                    }
                    // Twilight Deku Baba
                    case D_A_E_YH:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x9EC, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x6FC, 0x38600000);
#endif
                        break;
                    }
                    // Twilight Keese
                    case D_A_E_YK:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x98C, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA84, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x63C, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x6B0, 0x38600000);
#endif
                        break;
                    }
                    // Trill
                    case D_A_MYNA:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x3B90, 0x38600000);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x4400, 0x38600000);
#endif
                        break;
                    }
                    // White Wolfos
                    //case D_A_E_WW:
                    //{
                        //performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x62FC, ASM_NOP);
                        //performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x630C, ASM_NOP);
                        //performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x631C, ASM_NOP);
                        //performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x6338, ASM_NOP);
                        //libtp::patch::writeBranch(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x3344, enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x2200);
                        //break;
                    //}
                    // Goron Mines Beamos
                    case D_A_OBJ_BEMOS:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1904, 0x3860ffff);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1A50, 0x3860ffff);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1F34, 0x3860ffff);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x2090, 0x3860ffff);
#endif
                        break;
                    }
                    // Temple of Time Beamos
                    case D_A_OBJ_LV6BEMOS2:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x14CC, 0x3860ffff);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1604, 0x3860ffff);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x19C4, 0x3860ffff);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1B14, 0x3860ffff);
#endif
                        break;
                    }
                }
                addressOffsetSmthgSmthg++;
            }
        } else {
            while (idEnemyStorageTemp[addressOffsetSmthgSmthg] != 0) {
                switch (idEnemyStorageTemp[addressOffsetSmthgSmthg]) {
                    // Armos
                    case D_A_E_AI:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x5D0, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x6F8, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x584, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x6C4, 0x38600001);
#endif
                        break;
                    }
                    // Keese
                    case D_A_E_BA:
                    {
#ifndef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x564, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x814, 0x38600001);
#endif
                        break;
                    }
                    // Bombling
                    case D_A_E_BI:
                    {
#ifndef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x524, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x5A8, 0x38600001);
#endif
                        break;
                    }
                    // Stallkin
                    case D_A_E_BS:
                    {
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + bs_e_address[0], 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + bs_e_address[1], 0x38600002);
                        break;
                    }
                    // Bubble
                    case D_A_E_BU:
                    {
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + bu_e_address, 0x38600001);
                        break;
                    }
                    // Poison Mite
                    /*case D_A_E_BUG:
                    {
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xF00, 0x7FC3F378);
                        break;
                    }*/
                    // Deku Baba
                    case D_A_E_DB:
                    {
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + db_e_address[0], 0x38600001);
                        break;
                    }
                    // Dodongo
                    case D_A_E_DD:
                    {
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + dd_e_address, 0x38600001);
                        break;
                    }
                    // Lizalfos
                    case D_A_E_DN:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x2F3C, 0x418201ec);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x123C, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1278, 0x38600002);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1270, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x12E4, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1320, 0x38600002);
#endif
                        break;
                    }
                    // Freezard
                    case D_A_E_FB:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xBA0, 0x7fc3f378);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xC38, 0x7FE3FB78);
#endif
                        break;
                    }
                    // (Mini) Freezard
                    case D_A_E_FZ:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1DA4, 0x3b400001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x14DC, 0x9421FFF0);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1BA4, 0x3B000001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x12D8, 0x9421FFF0);
#endif
                        break;
                    }
                    case D_A_E_HB:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x844, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x550, 0x38600001);
#endif
                        break;
                    }
                    // Torch Slug
                    case D_A_E_HM:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x2E30, 0x41820030);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x3578, 0x41820030);
                        //performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x3284, 0x4BFFFD01);
#endif
                        break;
                    }
                    // Tile Worms
                    case D_A_E_HZ:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x7D8, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x94C, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA74, 0x38600000);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xBEC, 0x38600001);
#endif
                        break;
                    }
                    // Young Gohma
                    case D_A_E_KG:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x478, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x4DC, 0x38600001);
#endif
                        break;
                    }
                    // Chilfos
                    case D_A_E_KK:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1120, 0x3fc08048);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xECC, 0x881D4FAD);
#endif
                        break;
                    }
                    // Kargarok
                    case D_A_E_KR:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x8D0, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x938, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA8C, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xAFC, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x674, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x7A0, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x8F8, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x96C, 0x38600001);
                        //performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1650, 0x9421FF80);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x4918, 0x3BA00001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x493C, 0x3BA00001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x4948, 0x3BA00001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x4970, 0x3BA00001);
#endif
                        break;
                    }
                    // Dynalfos
                    case D_A_E_MF:
                    {
                        //performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1218, 0x38600001);
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xEC8, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xF04, 0x38600002);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x128C, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x12C8, 0x38600002);
#endif
                        break;
                    }
                    // Rat
                    case D_A_E_MS:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x400, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x2E4, 0x38600001);
#endif
                        break;
                    }
                    case D_A_E_ST:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xC84, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA34, 0x38600001);
#endif
                        break;
                    }
                    // Bokoblin
                    case D_A_E_OC:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA24, 0x408000b4);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA8C, 0x4082008c);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xC88, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA24, 0x408000B4);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xBA8, 0x40820094);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xDAC, 0x38600001);
#endif
                        break;
                    }
                    // Bulblin
                    case D_A_E_RD:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1D10, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1D58, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1E28, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1E70, 0x38600001);
#endif
                        break;
                    }
                    // Stalfos
                    case D_A_E_SF:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x914, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x938, 0x38600002);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x980, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x9A4, 0x38600002);
#endif
                        break;
                    }
                    // Moldorm
                    case D_A_E_SW:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x5670, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x6790, 0x38600001);
#endif
                        break;
                    }
                    // Water Toadpoli
                    case D_A_E_TK:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x428, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x494, 0x38600001);
#endif
                        break;
                    }
                    // Fire ToadPoli
                    case D_A_E_TK2:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x3F0, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x45C, 0x38600001);
#endif
                        break;
                    }
                    // Tektite
                    case D_A_E_TT:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x14D4, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1768, 0x38600001);
#endif
                        break;
                    }
                    case D_A_E_MM:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x15B4, 0x38000001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x3FC, 0x38600001);
#endif
                        break;
                    }
                    case D_A_E_S1:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x4E8, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x50C, 0x38600001);
#endif
                        break;
                    }
                    case D_A_E_SM2:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1340, 0x38000001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA4C, 0x38600001);
#endif
                        break;
                    }
                    // Wall Skulltula
                    case D_A_E_WS:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x7BC, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x9B4, 0x38600001);
#endif
                        break;
                    }
                    // White Wolfos
                    //case D_A_E_WW:
                    //{
                        //performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x62FC, 0x3BA00001);
                        //performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x630C, 0x3BA00001);
                        //performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x631C, 0x3BA00001);
                        //performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x6338, 0x3BA00001);
                        //performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x3344, 0x9421FF60);
                        //break;
                    //}
                    case D_A_E_YG:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x764, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x2E4, 0x38600001);
#endif
                        break;
                    }
                    // Twilight Deku Baba
                    case D_A_E_YH:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x9EC, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x6FC, 0x38600001);
#endif
                        break;
                    }
                    // Twilight Keese
                    case D_A_E_YK:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x98C, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0xA84, 0x38600001);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x63C, 0x38600001);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x6B0, 0x38600001);
#endif
                        break;
                    }
                    // Trill
                    case D_A_MYNA:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x3B90, 0x7fa3eb78);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x4400, 0x7FC3F378);
#endif
                        break;
                    }
                    // Goron Mines Beamos
                    case D_A_OBJ_BEMOS:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1904, 0x7f83e378);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1A50, 0x7f83e378);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1F34, 0x7F63DB78);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x2090, 0x7F63DB78);
#endif
                        break;
                    }
                    // Temple of Time Beamos
                    case D_A_OBJ_LV6BEMOS2:
                    {
#ifdef PLATFORM_WII
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x14CC, 0x7fa3eb78);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1604, 0x7fa3eb78);
#else
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x19C4, 0x7FE3FB78);
                        performStaticASMReplacement(enemyRelPtrAddress[addressOffsetSmthgSmthg] + 0x1B14, 0x7FE3FB78);
#endif
                        break;
                    }
                }
                addressOffsetSmthgSmthg++;
            }
        }
    }

    void enemyAppear(bool spawnOrNot, uint32_t enemyIdThing, uint32_t relPtrRaw) {
        if (spawnOrNot) {
            for (uint8_t countingparamater = 0; countingparamater < 10; countingparamater++) {
                if (idEnemyStorageTemp[countingparamater] == 0) {
                    idEnemyStorageTemp[countingparamater] = enemyIdThing;
                    enemyRelPtrAddress[countingparamater] = relPtrRaw;
                    break;
                }
            }
        }
        else {
            bool bringEnemyLower = false;
            for (uint8_t countingParameter = 0; countingParameter < 10; countingParameter++) {
                if (idEnemyStorageTemp[countingParameter] == enemyIdThing) {
                    idEnemyStorageTemp[countingParameter] = 0;
                    enemyRelPtrAddress[countingParameter] = 0;
                    if (countingParameter < 9) bringEnemyLower = true;
                    else break;
                }
                if (bringEnemyLower) {
                    if (countingParameter == 9) {
                        idEnemyStorageTemp[countingParameter] = 0; enemyRelPtrAddress[countingParameter] = 0;
                    } else {
                        idEnemyStorageTemp[countingParameter] = idEnemyStorageTemp[countingParameter+1];
                        enemyRelPtrAddress[countingParameter] = enemyRelPtrAddress[countingParameter+1];
                    }
                }
            }
        }
    }

    void Mod::onRELUnlink(libtp::tp::dynamic_link::DynamicModuleControl* dmc) {
        using namespace libtp::tp::rel::relIDs;
        switch (dmc->mModule->id) {
#ifndef PLATFORM_WII
            case D_A_ARROW:
            {
                return_a_arrow_execute = libtp::patch::unhookFunction(return_a_arrow_execute);
                break;
            }
#endif
            // Diababa 1
            /*case D_A_B_BH:
            {
                return_b_bh_execute = libtp::patch::unhookFunction(return_b_bh_execute);
                break;
            }*/
            // Armos
            case D_A_E_AI:
            {
                enemyAppear(false, D_A_E_AI, 0);
                return_e_ai_action = libtp::patch::unhookFunction(return_e_ai_action);
                break;
            }
            // Keese
            case D_A_E_BA:
            {
                enemyAppear(false, D_A_E_BA, 0);
                break;
            }
            // Bombling
            case D_A_E_BI:
            {
                enemyAppear(false, D_A_E_BI, 0);
                break;
            }
            // Stallkin
            case D_A_E_BS:
            {
                enemyAppear(false, D_A_E_BS, 0);
                break;
            }
            // Bubble
            case D_A_E_BU:
            {
                enemyAppear(false, D_A_E_BU, 0);
                return_e_bu_attack = libtp::patch::unhookFunction(return_e_bu_attack);
                break;
            }
            // Poison Mite
            /*case D_A_E_BUG:
            {
                enemyAppear(false, D_A_E_BUG, 0);
                break;
            }*/
            // Deku Baba
            case D_A_E_DB:
            {
                enemyAppear(false, D_A_E_DB, 0);
#ifndef PLATFORM_WII
                return_e_db_action = libtp::patch::unhookFunction(return_e_db_action);
#endif
                break;
            }
            // Dodongo
            case D_A_E_DD:
            {
                enemyAppear(false, D_A_E_DD, 0);
                break;
            }
            // Lizalfos
            case D_A_E_DN:
            {
                enemyAppear(false, D_A_E_DN, 0);
#ifndef PLATFORM_WII
                return_e_dn_action = libtp::patch::unhookFunction(return_e_dn_action);
#endif
                break;
            }
            // Freezard
            case D_A_E_FB:
            {
                enemyAppear(false, D_A_E_FB, 0);
                break;
            }
            // (Mini) Freezard
            case D_A_E_FZ:
            {
                enemyAppear(false, D_A_E_FZ, 0);
                //return_e_fz_actionModeSet = libtp::patch::unhookFunction(return_e_fz_actionModeSet);
                break;
            }
            case D_A_E_HB:
            {
                enemyAppear(false, D_A_E_HB, 0);
                break;
            }
            // Torch Slug
            case D_A_E_HM:
            {
                enemyAppear(false, D_A_E_HM, 0);
                break;
            }
            // Tile Worms
            case D_A_E_HZ:
            {
                enemyAppear(false, D_A_E_HZ, 0);
                break;
            }
            // Young Gohma
            case D_A_E_KG:
            {
                enemyAppear(false, D_A_E_KG, 0);
                break;
            }
            // Chilfos
            case D_A_E_KK:
            {
                enemyAppear(false, D_A_E_KK, 0);
#ifndef PLATFORM_WII
                return_e_kk_actionModeSet = libtp::patch::unhookFunction(return_e_kk_actionModeSet);
#endif
                break;
            }
            // Kargarok
            case D_A_E_KR:
            {
                enemyAppear(false, D_A_E_KR, 0);
#ifndef PLATFORM_WII
                return_e_kr_action = libtp::patch::unhookFunction(return_e_kr_action);
#endif
                break;
            }
            // Dynalfos
            case D_A_E_MF:
            {
                enemyAppear(false, D_A_E_MF, 0);
                break;
            }
            // Rat
            case D_A_E_MS:
            {
                enemyAppear(false, D_A_E_MS, 0);
                break;
            }
            // Skulltula
            case D_A_E_ST:
            {
                enemyAppear(false, D_A_E_ST, 0);
                break;
            }
            // Bokoblin
            case D_A_E_OC:
            {
                enemyAppear(false, D_A_E_OC, 0);
                return_e_oc_actionModeSet = libtp::patch::unhookFunction(return_e_oc_actionModeSet);
                break;
            }
            // Bulblin
            case D_A_E_RD:
            {
                enemyAppear(false, D_A_E_RD, 0);
#ifndef PLATFORM_WII
                return_e_rd_action = libtp::patch::unhookFunction(return_e_rd_action);
#endif
                break;
            }
            // Stalfos
            case D_A_E_SF:
            {
                enemyAppear(false, D_A_E_SF, 0);
                return_e_sf_action = libtp::patch::unhookFunction(return_e_sf_action);
                break;
            }
            // Moldorm
            case D_A_E_SW:
            {
                enemyAppear(false, D_A_E_SW, 0);
                break;
            }
            // Water Toadpoli
            case D_A_E_TK:
            {
                enemyAppear(false, D_A_E_TK, 0);
                break;
            }
            // Fire Toadpoli
            case D_A_E_TK2:
            {
                enemyAppear(false, D_A_E_TK2, 0);
                break;
            }
            // Tektite
            case D_A_E_TT:
            {
                enemyAppear(false, D_A_E_TT, 0);
                return_e_tt_actionModeSet = libtp::patch::unhookFunction(return_e_tt_actionModeSet);
                break;
            }
            case D_A_E_MM:
            {
                enemyAppear(false, D_A_E_MM, 0);
                return_e_mm_action = libtp::patch::unhookFunction(return_e_mm_action);
                break;
            }
            case D_A_E_S1:
            {
                enemyAppear(false, D_A_E_S1, 0);
                break;
            }
            case D_A_E_SM2:
            {
                enemyAppear(false, D_A_E_SM2, 0);
                break;
            }
            // Wall Skulltula
            case D_A_E_WS:
            {
                enemyAppear(false, D_A_E_WS, 0);
                break;
            }
            // White Wolfos
            case D_A_E_WW:
            {
                //enemyAppear(false, D_A_E_WW, 0);
                return_e_ww_actionModeSet = libtp::patch::unhookFunction(return_e_ww_actionModeSet);
                break;
            }
            case D_A_E_YG:
            {
                enemyAppear(false, D_A_E_YG, 0);
                break;
            }
            // Twilight Deku Baba
            case D_A_E_YH:
            {
                enemyAppear(false, D_A_E_YH, 0);
                break;
            }
            // Twilight Keese
            case D_A_E_YK:
            {
                enemyAppear(false, D_A_E_YK, 0);
                break;
            }
            // Trill
            case D_A_MYNA:
            {
                enemyAppear(false, D_A_MYNA, 0);
                break;
            }
            // Goron Mines Beamos
            case D_A_OBJ_BEMOS:
            {
                enemyAppear(false, D_A_OBJ_BEMOS, 0);
                break;
            }
            // Temple of Time Beamos
            case D_A_OBJ_LV6BEMOS2:
            {
                enemyAppear(false, D_A_OBJ_LV6BEMOS2, 0);
                break;
            }
            // Midna
            case D_A_MIDNA:
            {
                daMidna_c__checkMetamorphoseEnableBase = nullptr;
                break;
            }
            // Impaz
            case D_A_NPC_IMPAL:
            {
                return_npc_impaa_execute = libtp::patch::unhookFunction(return_npc_impaa_execute);
                break;
            }
        }
    }

    __attribute__((used, visibility("default"))) bool Mod::handle_do_unlink(libtp::tp::dynamic_link::DynamicModuleControl* dmc)
    {
        onRELUnlink(dmc);
        return return_do_unlink(dmc);
    }
    uint8_t tempCounteh = 30;


    bool Mod::procDoLink(libtp::tp::dynamic_link::DynamicModuleControl* dmc)
    {
        using namespace libtp::tp::rel::relIDs;
        // Call the original function immediately, as the REL file needs to be linked before applying patches
        const bool result = do_link_trampoline(dmc);

        // Get the pointer to the current REL file
        libtp::gc_wii::os_module::OSModuleInfo* modulePtr = dmc->mModule;

        // Make sure a REL file is actually loaded, as do_link will clear the pointer if something goes wrong
        if (!modulePtr)
        {
            return result;
        }

        // Get the REL pointer as a raw u32, to use for overwrites
        uint32_t relPtrRaw = reinterpret_cast<uint32_t>(modulePtr);

        //sprintf(sysConsolePtr->consoleLine[14].line, "actorr: 0x%x", modulePtr->id);

        // Modify the current REL file
        switch (modulePtr->id)     // May want to set up enums or defines for the module ids
        {
#ifndef PLATFORM_WII
            // Arrow (Because Z button)
            case D_A_ARROW:
            {
                return_a_arrow_execute =
                    libtp::patch::hookFunction(reinterpret_cast<daA_arrow_execute>(relPtrRaw + 0x33F4), [](libtp::tp::rel::d_a_arrow::daArrow_c* param0)
                    {
                        if (saveInfoPtr->zButtonnItemm == 0x43 || saveInfoPtr->zButtonnItemm == 0x4B) {
                            if (libtp::tp::f_op_actor_mng::fopAcM_GetParam(param0) == 1) ammoItemZTime(saveInfoPtr->zButtonnItemm, saveInfoPtr->zButtonSloot);
                        } 
                        return_a_arrow_execute(param0);
                    });
                break;
            }
#endif
            // Puppet Zelda
            case D_A_E_HZELDA:
            {
                //sprintf(sysConsolePtr->consoleLine[0].line, "relPtrRaw: %x", relPtrRaw+0x499C);
#ifdef PLATFORM_WII
                performStaticASMReplacement(0x3B54 + relPtrRaw, 0x00000000);
                performStaticASMReplacement(0x3B50 + relPtrRaw, 0x00000000);
#else
                performStaticASMReplacement(0x499c + relPtrRaw, 0x00000000);
                performStaticASMReplacement(0x4998 + relPtrRaw, 0x00000000);
                // nop out the greater than branch so that Zelda will always throw a Ball if she is able to
                //performStaticASMReplacement(relPtrRaw + 0xA94, ASM_NOP); // Previous: bge

                // nop out the addition of f1 (the random number of frames) to f0 (the base number of frames) so that there is
                // always only 100 frames between each of Zelda's attacks.
                //performStaticASMReplacement(relPtrRaw + 0x8EC, ASM_NOP); // Previous: bfadds f0,f0,f1
#endif
                break;
            }
            // Dark Nut
            case D_A_B_TN:
            {
#ifdef PLATFORM_WII
                performStaticASMReplacement(relPtrRaw + 0x2098, 0x38a00010);
                //performStaticASMReplacement(relPtrRaw + 0x20FC, 0x38000008);
#else
                performStaticASMReplacement(relPtrRaw + 0x2034, 0x38a00010);
                performStaticASMReplacement(relPtrRaw + 0x2038, 0x38800008);
#endif
                break;
            }
            // Dangoro
            case D_A_E_GOB:
            {
#ifdef PLATFORM_WII
                performStaticASMReplacement(relPtrRaw + 0x6330, 0x05000000);
#else
                performStaticASMReplacement(relPtrRaw + 0x72B8, 0x05000000);
#endif
                break;
            }
            // Sword Bad
            case D_A_E_VT:
            {
#ifdef PLATFORM_WII
                performStaticASMReplacement(relPtrRaw + 0xB95C, 0x06000000);
                performStaticASMReplacement(relPtrRaw + 0xB9DC, 0x03000000);
                performStaticASMReplacement(relPtrRaw + 0x6964, 0xc03e0120);
                performStaticASMReplacement(relPtrRaw + 0x6974, 0xc01e0120);
                performStaticASMReplacement(relPtrRaw + 0x697C, 0xc03e0120);
                performStaticASMReplacement(relPtrRaw + 0x6994, 0xc01e0120);
                performStaticASMReplacement(relPtrRaw + 0x6D38, 0xc03e0120);
                performStaticASMReplacement(relPtrRaw + 0x6D44, 0xc01e0120);
#else
                performStaticASMReplacement(relPtrRaw + 0xCBA4, 0x06000000);
                performStaticASMReplacement(relPtrRaw + 0xCC24, 0x03000000);
                performStaticASMReplacement(relPtrRaw + 0x7168, 0xC03D0138);
                performStaticASMReplacement(relPtrRaw + 0x7170, 0xC01D0138);
                performStaticASMReplacement(relPtrRaw + 0x7188, 0xC03D0138);
                performStaticASMReplacement(relPtrRaw + 0x7190, 0xC01D0138);
                performStaticASMReplacement(relPtrRaw + 0x75D4, 0xC03D0138);
                performStaticASMReplacement(relPtrRaw + 0x75DC, 0xC01D0138);
#endif
                break;
            }
            // Diababa 1
            /*case D_A_B_BH:
            {
                performStaticASMReplacement(relPtrRaw + 0x5384, 0x00000000);
                return_b_bh_execute =
                    libtp::patch::hookFunction(reinterpret_cast<daB_bh_execute>(relPtrRaw + 0x4064), [](libtp::tp::rel::d_a_b_bh::b_bh_class* bBhPtr)
                    {
                        if ((bBhPtr->mAction == 0 || bBhPtr->mAction == 20) && gameplayStatus) {
                            if (tempCounteh != 0) tempCounteh--;
                            else {
                                for (uint8_t bhTemp = 1; bhTemp < 1; bhTemp++) {
                                    bBhPtr->mTiiiimerrrs[bhTemp] = 0;
                                }
                                bBhPtr->mAction = 5;
                                bBhPtr->mMode = 0;
                                tempCounteh = 255;
                            }
                        }
                        return_b_bh_execute(bBhPtr);
                    });
                break;
            }*/
            // Diababa
            case D_A_B_BQ:
            {
#ifdef PLATFORM_WII
                performStaticASMReplacement(relPtrRaw + 0x6B54, 0x04000000);
                performStaticASMReplacement(relPtrRaw + 0x6CC4, 0x003201F4);
#else
                /*performStaticASMReplacement(relPtrRaw + 0x1E0, 0x38000032);
                performStaticASMReplacement(relPtrRaw + 0x1E8, 0x380003E8);*/
                //performStaticASMReplacement(relPtrRaw + 0x118C, 0xC03E0048);
                //performStaticASMReplacement(relPtrRaw + 0x1194, 0xC01E0048);
                performStaticASMReplacement(relPtrRaw + 0x78D0, 0x04000000);
                performStaticASMReplacement(relPtrRaw + 0x79E4, 0x003201F4);
#endif
                break;
            }
            // Fyrus
            case D_A_E_FM:
            {
#ifdef PLATFORM_WII
                performStaticASMReplacement(relPtrRaw + 0xAFE4, 0x41100000);
                performStaticASMReplacement(relPtrRaw + 0xAFE8, 0x41100000);
                performStaticASMReplacement(relPtrRaw + 0xAFEC, 0x40c00000);
                performStaticASMReplacement(relPtrRaw + 0xAFF0, 0x40a00000);
                performStaticASMReplacement(relPtrRaw + 0xAFF8, 0x453b8000);
                performStaticASMReplacement(relPtrRaw + 0xB014, 0x00000000);
#else
                //libtp::patch::writeBranch(relPtrRaw + 0x1148, relPtrRaw + 0x1188);
                //performStaticASMReplacement(relPtrRaw + 0x11A0, 0x38600001);
                performStaticASMReplacement(relPtrRaw + 0xBFC0, 0x41100000);
                performStaticASMReplacement(relPtrRaw + 0xBFC4, 0x41100000);
                performStaticASMReplacement(relPtrRaw + 0xBFC8, 0x40c00000);
                performStaticASMReplacement(relPtrRaw + 0xBFCC, 0x40a00000);
                performStaticASMReplacement(relPtrRaw + 0xBFD4, 0x453b8000);
                /*performStaticASMReplacement(relPtrRaw + 0xBFE4, 0x2000FF00);
                performStaticASMReplacement(relPtrRaw + 0xBFE8, 0x00000000);*/
                performStaticASMReplacement(relPtrRaw + 0xBFF0, 0x00000000);
                //performStaticASMReplacement(relPtrRaw + 0xC054, 0x43160000);
#endif
                break;
            }
            // Aeralfos
            case D_A_B_GG:
            {
                //sprintf(sysConsolePtr->consoleLine[12].line, "addresssss: 0x%x", relPtrRaw + 0xF4C4);
#ifdef PLATFORM_WII
                performStaticASMReplacement(relPtrRaw + 0xCC24, 0x00C8001E);
                performStaticASMReplacement(relPtrRaw + 0xCC2C, 0x00640384);
#else
                performStaticASMReplacement(relPtrRaw + 0xF4E0, 0x00C8001E);
                performStaticASMReplacement(relPtrRaw + 0xF4E8, 0x00640384);
#endif
                break;
            }
            // Armos
            case D_A_E_AI:
            {
#ifdef PLATFORM_WII
                performStaticASMReplacement(relPtrRaw + 0x2BB0, 0x04000000);
#else
                performStaticASMReplacement(relPtrRaw + 0x3494, 0x04000000);
#endif
                if (saveInfoPtr->maskID == 3) {
#ifdef PLATFORM_WII
                    performStaticASMReplacement(relPtrRaw+0x5D0, 0x38600000);
                    performStaticASMReplacement(relPtrRaw+0x6F8, 0x38600000);
#else
                    performStaticASMReplacement(relPtrRaw+0x584, 0x38600000);
                    performStaticASMReplacement(relPtrRaw+0x6C4, 0x38600000);
#endif
                }
                enemyAppear(true, D_A_E_AI, relPtrRaw);
#ifdef PLATFORM_WII
                return_e_ai_action =
                    libtp::patch::hookFunction(reinterpret_cast<daE_ai_action>(relPtrRaw + 0x1D64), [](libtp::tp::rel::d_a_e_ai::e_ai_class* eAiPtr)
                    {
                        //sprintf(sysConsolePtr->consoleLine[14].line, "mAction: %d, mMode1: %d, mMode2: %d", eAiPtr->mAction, eAiPtr->mMode1, eAiPtr->mMode2);
                        if (saveInfoPtr->maskID == 3) {
                            if (eAiPtr->mAction == 1 && eAiPtr->mMode1 == 1) {
                                eAiPtr->mAction = 4; eAiPtr->mMode1 = 0;
                            }
                        }
                        return_e_ai_action(eAiPtr);
                    });
#else
                return_e_ai_action =
                    libtp::patch::hookFunction(reinterpret_cast<daE_ai_action>(relPtrRaw + 0x2078), [](libtp::tp::rel::d_a_e_ai::e_ai_class* eAiPtr)
                    {
                        //sprintf(sysConsolePtr->consoleLine[14].line, "mAction: %d, mMode1: %d, mMode2: %d", eAiPtr->mAction, eAiPtr->mMode1, eAiPtr->mMode2);
                        if (saveInfoPtr->maskID == 3) {
                            if (eAiPtr->mAction == 1 && eAiPtr->mMode1 == 1) {
                                eAiPtr->mAction = 4; eAiPtr->mMode1 = 0;
                            }
                        }
                        return_e_ai_action(eAiPtr);
                    });
#endif
                break;
            }
            // Keese
            case D_A_E_BA:
            {
                //sprintf(sysConsolePtr->consoleLine[12].line, "%x",relPtrRaw + 0x564);
#ifndef PLATFORM_WII
                // damage towards Link
                performStaticASMReplacement(relPtrRaw + 0x3A0C, 0x02000000);
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw+0x564, 0x38600000);
#else
                performStaticASMReplacement(relPtrRaw + 0x3330, 0x02000000);
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw+0x814, 0x38600000);
#endif
                enemyAppear(true, D_A_E_BA, relPtrRaw);
                //if (PalaceOfDamageEnemiesOrSmthg())
                break;
            }
            // Bombling
            case D_A_E_BI:
            {
#ifndef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw+0x524, 0x38600000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw+0x5A8, 0x38600000);
#endif
                enemyAppear(true, D_A_E_BI, relPtrRaw);
                break;
            }
            // Stallkin
            case D_A_E_BS:
            {
                if (saveInfoPtr->maskID == 3) {
                    performStaticASMReplacement(relPtrRaw+bs_e_address[0], 0x38600000);
                    performStaticASMReplacement(relPtrRaw+bs_e_address[1], 0x38600000);
                }
                enemyAppear(true, D_A_E_BS, relPtrRaw);
                break;
            }
            // Bubble
            case D_A_E_BU:
            {
#ifdef PLATFORM_WII
                performStaticASMReplacement(relPtrRaw + 0x3200, 0x02000000);
                return_e_bu_attack =
                    libtp::patch::hookFunction(reinterpret_cast<daE_bu_attack>(relPtrRaw + 0x145C), [](libtp::tp::rel::d_a_e_bu::e_bu_class* eBuPtr)
                    {
                        if (libtp::tp::J3DModel::ChkAtHitPlayerr(&eBuPtr->ccSph) && (eBuPtr->ccSph.mGobjAt.mRPrm & 1) == 0) bubbleAttackInit();
                        return_e_bu_attack(eBuPtr);
                    });
#else
                performStaticASMReplacement(relPtrRaw + 0x3874, 0x02000000);
                return_e_bu_attack =
                    libtp::patch::hookFunction(reinterpret_cast<daE_bu_attack>(relPtrRaw + 0x1240), [](libtp::tp::rel::d_a_e_bu::e_bu_class* eBuPtr)
                    {
                        if (libtp::tp::J3DModel::ChkAtHitPlayerr(&eBuPtr->ccSph) && (eBuPtr->ccSph.mGobjAt.mRPrm & 1) == 0) bubbleAttackInit();
                        return_e_bu_attack(eBuPtr);
                    });
#endif
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw+bu_e_address, 0x38600000);
                enemyAppear(true, D_A_E_BU, relPtrRaw);
                break;
            }
            // Poison Mite
            /*case D_A_E_BUG:
            {
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0xF00, 0x38600000);
                enemyAppear(true, D_A_E_BUG, relPtrRaw);
                break;
            }*/
            // Deku Baba
            case D_A_E_DB:
            {
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw+db_e_address[0], 0x38600000);
                enemyAppear(true, D_A_E_DB, relPtrRaw);
#ifdef PLATFORM_WII
                performStaticASMReplacement(relPtrRaw + 0x6310, 0x04000000);
#else
                performStaticASMReplacement(relPtrRaw + 0x731C, 0x04000000);
                return_e_db_action =
                    libtp::patch::hookFunction(reinterpret_cast<daE_db_action>(relPtrRaw + db_e_address[1]), [](libtp::tp::rel::d_a_e_db::e_db_class* eDbPtr)
                    {
                        //sprintf(sysConsolePtr->consoleLine[14].line, "mAction: %d, mMode1: %d, mMode2: %d", eDbPtr->mAction, eDbPtr->mMode1, eDbPtr->mMode2);
                        if (saveInfoPtr->maskID == 3) {
                            if (eDbPtr->mAction == 0) {
                                if (eDbPtr->mMode1 == 3 || eDbPtr->mMode1 == 4 || eDbPtr->mMode1 == 20) {
                                    if (eDbPtr->mMode1 == 3 && eDbPtr->mMode2 == 1) {
                                        eDbPtr->mMode1 = 0; eDbPtr->mMode2 = 5; eDbPtr->mAction = 14;
                                    }
                                    else if (eDbPtr->mMode1 == 4) {
                                        eDbPtr->mMode1 = 3; eDbPtr->mMode2 = -1;
                                    }
                                    else if (eDbPtr->mMode1 == 20) {
                                        if (eDbPtr->mMode2 == 1) eDbPtr->mMode2 = 2;
                                        else if (eDbPtr->mMode2 == 20) eDbPtr->mMode2 = 21;
                                    }
                                }
                            } else if (eDbPtr->mAction == 14) {
                                if (eDbPtr->mMode1 == 1) {eDbPtr->mMode1 = 0; eDbPtr->mMode2 = 5;}
                            }
                        }
                        return_e_db_action(eDbPtr);
                    });
#endif
                break;
            }
            // Dodongo
            case D_A_E_DD:
            {
#ifdef PLATFORM_WII
                performStaticASMReplacement(relPtrRaw + 0x4624, 0x04000000);
#else
                performStaticASMReplacement(relPtrRaw + 0x52C4, 0x04000000);
#endif
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw+dd_e_address, 0x38600000);
                enemyAppear(true, D_A_E_DD, relPtrRaw);
                break;
            }
            // Lizalfos
            case D_A_E_DN:
            {
                //sprintf(sysConsolePtr->consoleLine[12].line, "addressEDb: %x", relPtrRaw + 0x2968);
                if (saveInfoPtr->maskID == 3) {
#ifdef PLATFORM_WII
                    libtp::patch::writeBranch(relPtrRaw + 0x2F3C, relPtrRaw + 0x3128);
                    performStaticASMReplacement(relPtrRaw + 0x123C, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0x1278, 0x38600000);
#else
                    performStaticASMReplacement(relPtrRaw + 0x1270, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0x12E4, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0x1320, 0x38600000);
#endif
                }
                enemyAppear(true, D_A_E_DN, relPtrRaw);

#ifdef PLATFORM_WII
                performStaticASMReplacement(relPtrRaw + 0x2B94, 0xc03f00A4);
                performStaticASMReplacement(relPtrRaw + 0x2BA0, 0xc01f00A4);
                performStaticASMReplacement(relPtrRaw + 0x3058, 0xc03f00A4);
                performStaticASMReplacement(relPtrRaw + 0x3060, 0xc01f00A4);
                performStaticASMReplacement(relPtrRaw + 0x8820, 0x03000000);
#else
                performStaticASMReplacement(relPtrRaw + 0x2F54, 0xC03F00C0);
                performStaticASMReplacement(relPtrRaw + 0x2F5C, 0xC01F00C0);
                performStaticASMReplacement(relPtrRaw + 0x33F4, 0xC03F00C0);
                performStaticASMReplacement(relPtrRaw + 0x33FC, 0xC01F00C0);
                performStaticASMReplacement(relPtrRaw + 0x9D98, 0x03000000);
                return_e_dn_action =
                    libtp::patch::hookFunction(reinterpret_cast<daE_dn_action>(relPtrRaw + 0x6358), [](libtp::tp::rel::d_a_e_dn::e_dn_class* eDnPtr)
                    {
                        if (saveInfoPtr->maskID == 3) {
                            if (eDnPtr->mAction > 2 && eDnPtr->mAction < 9) {
                                eDnPtr->mAction = 0;
                                eDnPtr->mMode = 0;
                            }
                        }
                        return_e_dn_action(eDnPtr);
                    });
#endif
                break;
            }
            // Freezard
            case D_A_E_FB:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0xBA0, 0x38600000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0xC38, 0x38600000);
#endif
                enemyAppear(true, D_A_E_FB, relPtrRaw);
                break;
            }
            // (Mini) Freezard
            case D_A_E_FZ:
            {
                if (saveInfoPtr->maskID == 3) {
#ifdef PLATFORM_WII
                    performStaticASMReplacement(relPtrRaw + 0x1DA4, ASM_NOP);
                    libtp::patch::writeBranch(relPtrRaw + 0x14DC, relPtrRaw + 0x1370);
#else
                    performStaticASMReplacement(relPtrRaw + 0x1BA4, ASM_NOP);
                    libtp::patch::writeBranch(relPtrRaw + 0x12D8, relPtrRaw + 0x115C);
#endif
                }
                enemyAppear(true, D_A_E_FZ, relPtrRaw);
                /*return_e_fz_actionModeSet = 
                    libtp::patch::hookFunction(reinterpret_cast<daE_fz_actionModeSet>(relPtrRaw + 0x36C), [](libtp::tp::f_op_actor::fopEn_enemy_c* eFzPtr, int32_t param0, int32_t param1)
                    {
                        sprintf(sysConsolePtr->consoleLine[14].line, "param0: %d, param1: %d", param0, param1);
                        return_e_fz_actionModeSet(eFzPtr, param0, param1);
                    });*/
                break;
            }
            case D_A_E_HB:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw+0x844, 0x38600000);
                performStaticASMReplacement(relPtrRaw + 0x43A4, 0x02000000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw+0x550, 0x38600000);
                performStaticASMReplacement(relPtrRaw + 0x5164, 0x02000000);
#endif
                enemyAppear(true, D_A_E_HB, relPtrRaw);
                break;
            }
            // Torch Slug
            case D_A_E_HM:
            {
                if (saveInfoPtr->maskID == 3) {
#ifdef PLATFORM_WII
                    libtp::patch::writeBranch(relPtrRaw + 0x2E30, relPtrRaw + 0x2E68);
#else
                    libtp::patch::writeBranch(relPtrRaw + 0x3578, relPtrRaw + 0x35B0);
                    //performStaticASMReplacement(relPtrRaw + 0x3284, ASM_NOP);
#endif
                }
                enemyAppear(true, D_A_E_HM, relPtrRaw);
                break;
            }
            // Tile Worms
            case D_A_E_HZ:
            {
                if (saveInfoPtr->maskID == 3) {
#ifdef PLATFORM_WII
                    performStaticASMReplacement(relPtrRaw+0x7D8, 0x38600001);
                    performStaticASMReplacement(relPtrRaw+0x94C, 0x38600000);
#else
                    performStaticASMReplacement(relPtrRaw+0xA74, 0x38600001);
                    performStaticASMReplacement(relPtrRaw+0xBEC, 0x38600000);
#endif
                }
                enemyAppear(true, D_A_E_HZ, relPtrRaw);
                break;
            }
            // Young Gohma
            case D_A_E_KG:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x478, 0x38600000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x4DC, 0x38600000);
#endif
                enemyAppear(true, D_A_E_KG, relPtrRaw);
                break;
            }
            // Chilfos
            case D_A_E_KK:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) libtp::patch::writeBranch(relPtrRaw + 0x1120, relPtrRaw + 0x11E0);
#else
                if (saveInfoPtr->maskID == 3) libtp::patch::writeBranch(relPtrRaw + 0xECC, relPtrRaw + 0xF40);
                return_e_kk_actionModeSet =
                    libtp::patch::hookFunction(reinterpret_cast<daE_kk_actionModeSet>(relPtrRaw + 0x734), [](libtp::tp::f_op_actor::fopEn_enemy_c* eKkPtr, int32_t param0, int32_t param1)
                    {
                        //sprintf(sysConsolePtr->consoleLine[14].line, "param0: %d, param1: %d", param0, param1);
                        if (saveInfoPtr->maskID == 3) {
                            if (param0 == 3) {
                                param0 = 0; param1 = 0;
                            }
                        }
                        return_e_kk_actionModeSet(eKkPtr, param0, param1);
                    });
#endif
                enemyAppear(true, D_A_E_KK, relPtrRaw);
                break;
            }
            // Kargarok
            case D_A_E_KR:
            {
                if (saveInfoPtr->maskID == 3) {
#ifdef PLATFORM_WII
                    performStaticASMReplacement(relPtrRaw + 0x8D0, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0x938, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0xA8C, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0xAFC, 0x38600000);
#else
                    performStaticASMReplacement(relPtrRaw + 0x674, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0x7A0, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0x8F8, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0x96C, 0x38600000);
                    //libtp::patch::writeBranch(relPtrRaw + 0x1650, relPtrRaw + 0xE4C);
                    performStaticASMReplacement(relPtrRaw + 0x4918, ASM_NOP);
                    performStaticASMReplacement(relPtrRaw + 0x493C, ASM_NOP);
                    performStaticASMReplacement(relPtrRaw + 0x4948, ASM_NOP);
                    performStaticASMReplacement(relPtrRaw + 0x4970, ASM_NOP);
#endif
                }
                enemyAppear(true, D_A_E_KR, relPtrRaw);
#ifndef PLATFORM_WII
                return_e_kr_action =
                    libtp::patch::hookFunction(reinterpret_cast<daE_kr_action>(relPtrRaw + 0x48C4), [](libtp::tp::rel::d_a_e_kr::e_kr_class* eKrPtr)
                    {
                        //sprintf(sysConsolePtr->consoleLine[15].line, "Action: %d, Mode: %d, 0x664: %d", eKrPtr->mAction, eKrPtr->mMode, eKrPtr->field_0x664);
                        /*if (saveInfoPtr->maskID == 3) {
                            if (eKrPtr->mAction == 3) {
                                if (eKrPtr->mMode == 4 || eKrPtr->mMode == 5 || eKrPtr->mMode == 7) {eKrPtr->mAction = 0;}
                            }
                        }*/
                        return_e_kr_action(eKrPtr);
                    });
#endif
                break;
            }
            // Dynalfos
            case D_A_E_MF:
            {
                if (saveInfoPtr->maskID == 3) {
                    //performStaticASMReplacement(relPtrRaw + 0x1218, 0x38600000);
#ifdef PLATFORM_WII
                    performStaticASMReplacement(relPtrRaw + 0xEC8, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0xF04, 0x38600000);
#else
                    performStaticASMReplacement(relPtrRaw + 0x128C, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0x12C8, 0x38600000);
#endif
                }
                enemyAppear(true, D_A_E_MF, relPtrRaw);
                break;
            }
            // Rat
            case D_A_E_MS:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x400, 0x38600000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x2E4, 0x38600000);
#endif
                enemyAppear(true, D_A_E_MS, relPtrRaw);
                break;
            }
            // Skulltula
            case D_A_E_ST:
            {
                //sprintf(sysConsolePtr->consoleLine[0].line, "relPtrRaw: %x", 0x8FA0 + relPtrRaw);
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0xC84, 0x38600000);
                performStaticASMReplacement(relPtrRaw+0x7924, 0x00000000);
                // damage towards Link
                performStaticASMReplacement(relPtrRaw + 0x7BDC, 0x03000000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0xA34, 0x38600000);
                performStaticASMReplacement(relPtrRaw+0x8CA8, 0x00000000);
                // damage towards Link
                performStaticASMReplacement(relPtrRaw + 0x8FA0, 0x03000000);
#endif
                enemyAppear(true, D_A_E_ST, relPtrRaw);
                break;
            }
            // Bulblin
            case D_A_E_RD:
            {
                //sprintf(sysConsolePtr->consoleLine[6].line, "damage: %x", relPtrRaw + 0x11e8);
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) {
                    performStaticASMReplacement(relPtrRaw + 0x1D10, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0x1D58, 0x38600000);
                }
                performStaticASMReplacement(relPtrRaw + 0x1216C, 0x03000000);
#else
                if (saveInfoPtr->maskID == 3) {
                    performStaticASMReplacement(relPtrRaw + 0x1E28, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0x1E70, 0x38600000);
                }
                performStaticASMReplacement(relPtrRaw + 0x14768, 0x03000000);
                //sprintf(sysConsolePtr->consoleLine[12].line, "addressERd: %x", relPtrRaw + 0x1449C);
                return_e_rd_action =
                    libtp::patch::hookFunction(reinterpret_cast<daE_rd_action>(relPtrRaw + 0xc540), [](libtp::tp::rel::d_a_e_rd::e_rd_class* eRdPtr)
                    {
                        //sprintf(sysConsolePtr->consoleLine[11].line, "actionERd: %d", eRdPtr->mAction);
                        if (saveInfoPtr->maskID == 3) {
                            if (eRdPtr->mAction > 0x2 && eRdPtr->mAction < 0xb) {
                                eRdPtr->mAction = 0;
                                eRdPtr->mMode = 0;
                            }
                        }
                        return_e_rd_action(eRdPtr);
                    });
#endif
                enemyAppear(true, D_A_E_RD, relPtrRaw);
                //libtp::patch::writeBranch(relPtrRaw + 0x2F0C, relPtrRaw + 0x2F24);
                break;
            }
            // Stalfos
            case D_A_E_SF:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) {
                    performStaticASMReplacement(relPtrRaw + 0x914, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0x938, 0x38600000);
                }
                performStaticASMReplacement(relPtrRaw + 0x4530, 0x04000000);
                return_e_sf_action =
                    libtp::patch::hookFunction(reinterpret_cast<daE_sf_action>(relPtrRaw + 0x9CC), [](libtp::tp::rel::d_a_e_sf::e_sf_class* eSfPtr)
                    {
                        //sprintf(sysConsolePtr->consoleLine[12].line, "e_sfAction: %d", eSfPtr->mAction);
                        if (saveInfoPtr->maskID == 3) {
                            if ((eSfPtr->mAction > 2 && eSfPtr->mAction < 7) || eSfPtr->mAction == 31) {

                                // YouTube Poll 1 option 1
                                /*if (eSfPtr->mAction != 31) eSfPtr->mAction = 30;
                                else eSfPtr->mAction = 32;*/

                                // Youtube Poll 1 option 2
                                if (eSfPtr->mAction == 31) eSfPtr->mAction = 32;
                                else eSfPtr->mAction = 0;

                                eSfPtr->mMode = 0;
                            }
                        }
                        return_e_sf_action(eSfPtr);
                    });
#else
                if (saveInfoPtr->maskID == 3) {
                    performStaticASMReplacement(relPtrRaw + 0x980, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0x9A4, 0x38600000);
                }
                performStaticASMReplacement(relPtrRaw + 0x4F8C, 0x04000000);
                return_e_sf_action =
                    libtp::patch::hookFunction(reinterpret_cast<daE_sf_action>(relPtrRaw + 0x2454), [](libtp::tp::rel::d_a_e_sf::e_sf_class* eSfPtr)
                    {
                        //sprintf(sysConsolePtr->consoleLine[12].line, "e_sfAction: %d", eSfPtr->mAction);
                        if (saveInfoPtr->maskID == 3) {
                            if ((eSfPtr->mAction > 2 && eSfPtr->mAction < 7) || eSfPtr->mAction == 31) {

                                // YouTube Poll 1 option 1
                                /*if (eSfPtr->mAction != 31) eSfPtr->mAction = 30;
                                else eSfPtr->mAction = 32;*/

                                // Youtube Poll 1 option 2
                                if (eSfPtr->mAction == 31) eSfPtr->mAction = 32;
                                else eSfPtr->mAction = 0;

                                eSfPtr->mMode = 0;
                            }
                        }
                        return_e_sf_action(eSfPtr);
                    });
#endif
                enemyAppear(true, D_A_E_SF, relPtrRaw);
                break;
            }
            // Moldorm
            case D_A_E_SW:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) {
                    performStaticASMReplacement(relPtrRaw + 0x5670, 0x38600000);
                }
                performStaticASMReplacement(relPtrRaw + 0x7160, 0x03000000);
#else
                if (saveInfoPtr->maskID == 3) {
                    performStaticASMReplacement(relPtrRaw + 0x6790, 0x38600000);
                }
                performStaticASMReplacement(relPtrRaw + 0x8C70, 0x03000000);
#endif
                enemyAppear(true, D_A_E_SW, relPtrRaw);
                break;
            }
            // Water Toadpoli
            case D_A_E_TK:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x428, 0x38600000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x494, 0x38600000);
#endif
                enemyAppear(true, D_A_E_TK, relPtrRaw);
                break;
            }
            // Fire Toadpoli
            case D_A_E_TK2:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x3F0, 0x38600000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x45C, 0x38600000);
#endif
                enemyAppear(true, D_A_E_TK2, relPtrRaw);
                break;
            }
            // Tektite
            case D_A_E_TT:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x14D4, 0x38600000);
                return_e_tt_actionModeSet =
                    libtp::patch::hookFunction(reinterpret_cast<daE_tt_actionModeSet>(relPtrRaw + 0x560), [](libtp::tp::f_op_actor::fopEn_enemy_c* eTtPtr, int32_t param0, int32_t param1)
                    {
                        if (saveInfoPtr->maskID == 3) {
                            if (param0 == 1 || param0 == 2) {param0 = 0; param1 = 0;}
                        }
                        return_e_tt_actionModeSet(eTtPtr, param0, param1);
                    });
                    performStaticASMReplacement(relPtrRaw + 0x3FC0, 0x02000000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x1768, 0x38600000);
                return_e_tt_actionModeSet =
                    libtp::patch::hookFunction(reinterpret_cast<daE_tt_actionModeSet>(relPtrRaw + 0x5F8), [](libtp::tp::f_op_actor::fopEn_enemy_c* eTtPtr, int32_t param0, int32_t param1)
                    {
                        if (saveInfoPtr->maskID == 3) {
                            if (param0 == 1 || param0 == 2) {param0 = 0; param1 = 0;}
                        }
                        return_e_tt_actionModeSet(eTtPtr, param0, param1);
                    });
                    performStaticASMReplacement(relPtrRaw + 0x4B20, 0x02000000);
#endif
                enemyAppear(true, D_A_E_TT, relPtrRaw);
                break;
            }
            case D_A_E_MM:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x15B4, 0x38000000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x3FC, 0x38600000);
#endif
                enemyAppear(true, D_A_E_MM, relPtrRaw);
                //sprintf(sysConsolePtr->consoleLine[12].line, "addressEMm: 0x%x", relPtrRaw+0xBD4);
                return_e_mm_action =
#ifdef PLATFORM_WII
                    libtp::patch::hookFunction(reinterpret_cast<daE_mm_action>(relPtrRaw + 0xC38), [](libtp::tp::rel::d_a_e_mm::e_mm_class* eMmPtr)

                    {
                        if (saveInfoPtr->maskID == 3) {
                            if (eMmPtr->mAction == 1 && eMmPtr->mMode != 4) {
                                eMmPtr->mMode = 4;
                            }
                        }
                        return_e_mm_action(eMmPtr);
                    });
#else
                    libtp::patch::hookFunction(reinterpret_cast<daE_mm_action>(relPtrRaw + 0xBD4), [](libtp::tp::rel::d_a_e_mm::e_mm_class* eMmPtr)

                    {
                        if (saveInfoPtr->maskID == 3) {
                            if (eMmPtr->mAction == 1 && eMmPtr->mMode != 4) {
                                eMmPtr->mMode = 4;
                            }
                        }
                        return_e_mm_action(eMmPtr);
                    });
#endif
                break;
            }
            // Bokoblin
            case D_A_E_OC:
            {
#ifdef PLATFORM_WII

                if (saveInfoPtr->maskID == 3) {
                    libtp::patch::writeBranch(relPtrRaw + 0x904, relPtrRaw + 0x9B8);
                    libtp::patch::writeBranch(relPtrRaw + 0xA8C, relPtrRaw + 0xBB4);
                    performStaticASMReplacement(relPtrRaw + 0xC88, 0x38600000);
                }
                return_e_oc_actionModeSet =
                    libtp::patch::hookFunction(reinterpret_cast<daE_oc_actionModeSet>(relPtrRaw + 0x15A8), [](libtp::tp::f_op_actor::fopEn_enemy_c* eOcPtr, int32_t param0, int32_t param1)
                    {
                        if (saveInfoPtr->maskID == 3) {
                            if (param0 == 3 || param0 == 4 || param0 == 15) {
                                param0 = 0; param1 = 0;
                            }
                        }
                        return_e_oc_actionModeSet(eOcPtr, param0, param1);
                    });
#else
                if (saveInfoPtr->maskID == 3) {
                    libtp::patch::writeBranch(relPtrRaw + 0xA24, relPtrRaw + 0xAD8);
                    libtp::patch::writeBranch(relPtrRaw + 0xBA8, relPtrRaw + 0xC34);
                    performStaticASMReplacement(relPtrRaw + 0xDAC, 0x38600000);
                }
                /*performStaticASMReplacement(relPtrRaw + 0x369C, 0xc01d00f4);
                performStaticASMReplacement(relPtrRaw + 0x3694, 0xC03D00f4);
                performStaticASMReplacement(relPtrRaw + 0x3708, 0xC03D00f4);
                performStaticASMReplacement(relPtrRaw + 0x3710, 0xC05D00f4);*/
                return_e_oc_actionModeSet =
                    libtp::patch::hookFunction(reinterpret_cast<daE_oc_actionModeSet>(relPtrRaw + 0x18B0), [](libtp::tp::f_op_actor::fopEn_enemy_c* eOcPtr, int32_t param0, int32_t param1)
                    {
                        if (saveInfoPtr->maskID == 3) {
                            if (param0 == 3 || param0 == 4 || param0 == 15) {
                                param0 = 0; param1 = 0;
                            }
                        }
                        return_e_oc_actionModeSet(eOcPtr, param0, param1);
                    });
#endif
                enemyAppear(true, D_A_E_OC, relPtrRaw);
                break;
            }
            case D_A_E_S1:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x4E8, 0x38600000);
                performStaticASMReplacement(relPtrRaw + 0x5BB0, 0x03000000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x50C, 0x38600000);
                performStaticASMReplacement(relPtrRaw + 0x6864, 0x03000000);
#endif
                enemyAppear(true, D_A_E_S1, relPtrRaw);
                break;
            }
            // Chu
            case D_A_E_SM2:
            {
                //sprintf(sysConsolePtr->consoleLine[8].line, "Amonganese: %x", relPtrRaw + 0xA4C);
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x1340, 0x38000000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0xA4C, 0x38600000);
                //performStaticASMReplacement(relPtrRaw + 0x50B0, 0x02000000);
#endif
                enemyAppear(true, D_A_E_SM2, relPtrRaw);
                break;
            }
            // Wall Skulltula
            case D_A_E_WS:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x7BC, 0x38600000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x9B4, 0x38600000);
#endif
                enemyAppear(true, D_A_E_WS, relPtrRaw);
                break;
            }
            // White Wolfos
            case D_A_E_WW:
            {
                //if (saveInfoPtr->maskID == 3) {
                    //performStaticASMReplacement(relPtrRaw + 0x62FC, ASM_NOP);
                    //performStaticASMReplacement(relPtrRaw + 0x630C, ASM_NOP);
                    //performStaticASMReplacement(relPtrRaw + 0x631C, ASM_NOP);
                    //performStaticASMReplacement(relPtrRaw + 0x6338, ASM_NOP);
                    //libtp::patch::writeBranch(relPtrRaw + 0x3344, relPtrRaw + 0x2200);
                //}
                //enemyAppear(true, D_A_E_WW, relPtrRaw);
#ifdef PLATFORM_WII
                return_e_ww_actionModeSet =
                    libtp::patch::hookFunction(reinterpret_cast<daE_ww_actionModeSet>(relPtrRaw + 0x604), [](libtp::tp::f_op_actor::fopEn_enemy_c* eWwPtr, int32_t param0, int32_t param1)
                    {
                        //sprintf(sysConsolePtr->consoleLine[14].line, "param0: %d, param1: %d", param0, param1);
                        if (saveInfoPtr->maskID == 3) {
                            if (param0 == 2) param0 = 3;
                        }
                        return_e_ww_actionModeSet(eWwPtr, param0, param1);
                    });
#else
                return_e_ww_actionModeSet =
                    libtp::patch::hookFunction(reinterpret_cast<daE_ww_actionModeSet>(relPtrRaw + 0x6B0), [](libtp::tp::f_op_actor::fopEn_enemy_c* eWwPtr, int32_t param0, int32_t param1)
                    {
                        //sprintf(sysConsolePtr->consoleLine[14].line, "param0: %d, param1: %d", param0, param1);
                        if (saveInfoPtr->maskID == 3) {
                            if (param0 == 2) param0 = 3;
                        }
                        return_e_ww_actionModeSet(eWwPtr, param0, param1);
                    });
#endif
                break;
            }
            case D_A_E_YG:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x764, 0x38600000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x2E4, 0x38600000);
#endif
                enemyAppear(true, D_A_E_YG, relPtrRaw);
                break;
            }
            // Twilight Deku Baba
            case D_A_E_YH:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x9EC, 0x38600000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x6FC, 0x38600000);
#endif
                enemyAppear(true, D_A_E_YH, relPtrRaw);
                break;
            }
            // Twilight Keese
            case D_A_E_YK:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) {
                    performStaticASMReplacement(relPtrRaw + 0x98C, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0xA84, 0x38600000);
                }
#else
                if (saveInfoPtr->maskID == 3) {
                    performStaticASMReplacement(relPtrRaw + 0x63C, 0x38600000);
                    performStaticASMReplacement(relPtrRaw + 0x6B0, 0x38600000);
                }
#endif
                enemyAppear(true, D_A_E_YK, relPtrRaw);
                break;
            }
            // Trill
            case D_A_MYNA:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x3B90, 0x38600000);
#else
                if (saveInfoPtr->maskID == 3) performStaticASMReplacement(relPtrRaw + 0x4400, 0x38600000);
#endif
                enemyAppear(true, D_A_MYNA, relPtrRaw);
                break;
            }
            // Goron Mines Beamos
            case D_A_OBJ_BEMOS:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) {
                    performStaticASMReplacement(relPtrRaw + 0x1904, 0x3860ffff);
                    performStaticASMReplacement(relPtrRaw + 0x1A50, 0x3860ffff);
                }
                performStaticASMReplacement(relPtrRaw + 0x48EC, 0x03000000);
#else
                if (saveInfoPtr->maskID == 3) {
                    performStaticASMReplacement(relPtrRaw + 0x1F34, 0x3860ffff);
                    performStaticASMReplacement(relPtrRaw + 0x2090, 0x3860ffff);
                }
                performStaticASMReplacement(relPtrRaw + 0x5980, 0x03000000);
#endif
                enemyAppear(true, D_A_OBJ_BEMOS, relPtrRaw);
                break;
            }
            // Temple of Time Beamos
            case D_A_OBJ_LV6BEMOS2:
            {
#ifdef PLATFORM_WII
                if (saveInfoPtr->maskID == 3) {
                    performStaticASMReplacement(relPtrRaw + 0x14CC, 0x3860ffff);
                    performStaticASMReplacement(relPtrRaw + 0x1604, 0x3860ffff);
                }
                performStaticASMReplacement(relPtrRaw + 0x32BC, 0x04000000);
#else
                if (saveInfoPtr->maskID == 3) {
                    performStaticASMReplacement(relPtrRaw + 0x19C4, 0x3860ffff);
                    performStaticASMReplacement(relPtrRaw + 0x1B14, 0x3860ffff);
                }
                performStaticASMReplacement(relPtrRaw + 0x3D40, 0x04000000);
#endif
                enemyAppear(true, D_A_OBJ_LV6BEMOS2, relPtrRaw);
                break;
            }
            // Midna
            case D_A_MIDNA:
            {
#ifdef PLATFORM_WII
                daMidna_c__checkMetamorphoseEnableBase =
                    reinterpret_cast<daMidna_checkMetamorphoseEnableBase_Def>(relPtrRaw + 0x7A7C);
#else
                daMidna_c__checkMetamorphoseEnableBase =
                    reinterpret_cast<daMidna_checkMetamorphoseEnableBase_Def>(relPtrRaw + 0x8A0C);
#endif
                break;
            }
            // Impaz
            case D_A_NPC_IMPAL:
            {
#ifdef PLATFORM_WII
                return_npc_impaa_execute =
                    libtp::patch::hookFunction(reinterpret_cast<daNPC_impaa_execute>(relPtrRaw + 0x2950), [](libtp::tp::f_op_actor::fopAc_ac_c* npcIMPAAPtr)
                    {
                        if ((libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.memory.temp_flags.memoryFlags[9] & 2) && !(libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x22] & 0x80)) {
                            libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x22] |= 0x80; libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x5F] |= 0x80;
                            libtp::tp::d_item::execItemGet(0x83); libtp::tp::d_item::execItemGet(0xE9);
                        }
                        return_npc_impaa_execute(npcIMPAAPtr);
                    });
#else
                return_npc_impaa_execute =
                    libtp::patch::hookFunction(reinterpret_cast<daNPC_impaa_execute>(relPtrRaw + 0xD2C), [](libtp::tp::f_op_actor::fopAc_ac_c* npcIMPAAPtr)
                    {
                        if ((libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.memory.temp_flags.memoryFlags[9] & 2) && !(libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x22] & 0x80)) {
                            libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x22] |= 0x80; libtp::tp::d_com_inf_game::dComIfG_gameInfo.save.save_file.mEvent.mEvent[0x5F] |= 0x80;
                            libtp::tp::d_item::execItemGet(0x83); libtp::tp::d_item::execItemGet(0xE9);
                        }
                        return_npc_impaa_execute(npcIMPAAPtr);
                    });
#endif
                    //sprintf(sysConsolePtr->consoleLine[3].line, "addressImpaz: 0x%x", relPtrRaw + 0x2950);
                break;
            }
            default:
            {
                //sprintf(sysConsolePtr->consoleLine[tempCounteh+12].line, "addressEDb: %x", modulePtr->id);
                //tempCounteh++;
                break;
            }
        }

        return result;
    }

    void* getZel00BmgInf()
    {
        uint32_t infPtrRaw = reinterpret_cast<uint32_t>(libtp::tp::JKRArchivePub::JKRArchivePub_getGlbResource(
            0x524F4F54, // ROOT
            "zel_00.bmg",
            libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mMsgDtArchive[0]));

        // getGlbResource gets a pointer to MESGbmg1, but we need a pointer to INF1, which is just past MESGbmg1, and MESGbmg1
        // has a size of 0x20
        return reinterpret_cast<void*>(infPtrRaw + 0x20);
    }

    void insertCustomMessage(libtp::tp::control::TControl* control, const void* TProcessor, uint16_t unk3, uint16_t msgID)
    {
        auto setMessageText = [=](const char* text)
        {
            if (text)
            {
                control->msg = text;
                control->wMsgRender = text;
            }
        };
        const void* unk = libtp::tp::processor::getResource_groupID(TProcessor, unk3);
        if (!unk) return;
        const void* currentInf1 = *reinterpret_cast<void**>(reinterpret_cast<uint32_t>(unk) + 0xC);

        if (currentInf1 == getZel00BmgInf())
        {

            uint16_t bowDescription = 0xbc0;

            if (msgID == bowDescription) {
                if (noSwordLOL) {
                    const char* newMessage;
                    newMessage = "You've been " MSG_COLOR(MSG_COLOR_RED) "jinxed" MSG_COLOR(MSG_COLOR_UNK) "! You can't\ndraw your sword!";
                    setMessageText(newMessage);
                    return;
                }
                else if (notifyMeAboutTheTimeRIGHTNOW) {
                    notifyMeAboutTheTimeRIGHTNOW = false;
                    char* bufferForTheCurrennnnntTime = bufferForTheBuffBuff;
                    setMessageText(bufferForTheCurrennnnntTime);
                    return;
                }
            }
        } else {
            
            auto checkForSpecificMsg =
                [=](uint32_t desiredMsgId, int32_t room, const char* stage, const void* currentInf1, const char* desiredFile)
            {
                // Check if the message ids are the same
                if (msgID != desiredMsgId)
                {
                    return false;
                }

                // Check if the stage and room are correct
                if (!playerIsInRoomSeeee(room, stage))
                {
                    return false;
                }

                // Check if the desired file is being used
                return currentInf1 == getInf1Ptr(desiredFile);
            };

            if (checkForSpecificMsg(0x658, 1, "F_SP103", currentInf1, "zel_01.bmg")) {
                libtp::display::setConsoleColor(0, 0, 0, 180);
                bankSystemControllererererer(0);
                sprintf(sysConsolePtr->consoleLine[14].line, "Rupees:       %d", saveInfoPtr->rupeeeStoreePlace);
                sprintf(sysConsolePtr->consoleLine[15].line, "Arrows:       %d", saveInfoPtr->bowwStorePlace);
                sprintf(sysConsolePtr->consoleLine[16].line, "Normal Bombs: %d", saveInfoPtr->bobombStorePlace[0]);
                sprintf(sysConsolePtr->consoleLine[17].line, "Water Bombs:  %d", saveInfoPtr->bobombStorePlace[1]);
                sysConsolePtr->consoleLine[6].showLine = true;
                rentingTime = true;
            }
        }
    }

    __attribute__((used, visibility("default"))) bool Mod::customMessageCode(libtp::tp::control::TControl* control, const void* TProcessor, uint16_t unk3, uint16_t msgID)
    {
        const bool ret = return_messageSmthg(control, TProcessor, unk3, msgID);

        if (ret) {
            insertCustomMessage(control, TProcessor, unk3, msgID);
        }
        return ret;
    }

    /*void drawThatAMAZINGXYZ__COMEON(libtp::tp::d_meter2_draw::dMeter2Draw_c* dmeterDrawPtr, int32_t i_no, uint8_t i_itemNo, uint8_t i_action, bool param3, bool param4) {
        uint64_t tagggg[] = {0x6974656D5F785F6E, 0x6974656D5F795F6E, 0x6974656D5F725F6E};

        if (!param3) {
            libtp::tp::J2DPane::searchhh(dmeterDrawPtr->mpScreen, tagggg[i_no])->hideTex();

            int32_t var_r26;
            if (i_no == 0) {
                var_r26 = ((libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mXSetFlag & 2) ? true : false) | ((libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mXSetFlag & 4) ? true : false);
            } else if (i_no == 1) {
                var_r26 = ((libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mYSetFlag & 2) ? true : false) | ((libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mYSetFlag & 4) ? true : false);
            } else if (i_no == 2) {
                var_r26 = ((libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mZSetFlag & 2) ? true : false) | ((libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mZSetFlag & 4) ? true : false);
            }

            char* mp_string = libtp::tp::d_meter2_draw::getActionString(dmeterDrawPtr, i_action, 1, &dmeterDrawPtr->field_0x768[i_no]);
            if (var_r26) {
                dmeterDrawPtr->field_0x768[i_no] = 7;
            }

            if (param4) {
                if (i_action == 0x10) {
                    if (dmeterDrawPtr->field_0x620[i_no] == 0.0f) dmeterDrawPtr->field_0x620[i_no] = 18.0f - libtp::tp::d_meter_hio::g_drawHIO.mXYButtonHighlightAnimSpeed;
                    dmeterDrawPtr->field_0x75c[i_no] = 1;
                } else {
                    if (i_action != 0) {
                        dmeterDrawPtr->field_0x620[i_no] = 18.0f - libtp::tp::d_meter_hio::g_drawHIO.mXYButtonHighlightAnimSpeed;
                    }
                    dmeterDrawPtr->field_0x75c[i_no] = 0;
                }
            }

            if (*mp_string != 0) dmeterDrawPtr->mpTextXY[i_no]->mWindow->showTex();
            else dmeterDrawPtr->mpTextXY[i_no]->mWindow->hideTex();

            // JUT_ASSERT(std::strlen(mp_string) < (64));

            for (uint8_t iii = 0; iii < 5; iii++) {
                strcpy(static_cast<libtp::tp::J2DTextBox::J2DTextBox*>(dmeterDrawPtr->mpTextXY[iii][i_no].mWindow)->mStringPtr, mp_string);
            }

            dmeterDrawPtr->mpTextXY[i_no]->mWindow->mScaleX = libtp::tp::d_meter_hio::g_drawHIO.mXYButtonFontScale;
            dmeterDrawPtr->mpTextXY[i_no]->mWindow->mScaleY = libtp::tp::d_meter_hio::g_drawHIO.mXYButtonFontScale;
            libtp::tp::d_pane_class::paneTrans(dmeterDrawPtr->mpTextXY[i_no], libtp::tp::d_meter_hio::g_drawHIO.mXYButtonFontPosX, libtp::tp::d_meter_hio::g_drawHIO.mXYButtonFontPosY);
        } else {
            libtp::tp::J2DPane::searchhh(dmeterDrawPtr->mpScreen, tagggg[i_no])->showTex();
            dmeterDrawPtr->mpTextXY[i_no]->mWindow->hideTex();

            int32_t var_r29;
            if (i_itemNo == 0xFF || i_itemNo == 0) {
                libtp::tp::J2DPane::searchhh(dmeterDrawPtr->mpScreen, tagggg[i_no])->hideTex();
                var_r29 = 1;
            } else {
                libtp::tp::J2DPane::searchhh(dmeterDrawPtr->mpScreen, tagggg[i_no])->showTex();
                libtp::tp::d_meter2_draw::changeTextureItemXY(dmeterDrawPtr, i_no, i_itemNo);
                var_r29 = 0;
            }

            switch (i_no) {
                case 0:
                    libtp::tp::d_meter2_draw::setItemParamX(dmeterDrawPtr, i_itemNo);
                    break;
                case 1:
                    libtp::tp::d_meter2_draw::setItemParamY(dmeterDrawPtr, i_itemNo);
                    break;
                case 2:
                    libtp::tp::d_meter2_draw::setITemParamZ(dmeterDrawPtr, i_itemNo);
                    break;
            }
            libtp::tp::J2DPane::rotateDPaneClass(dmeterDrawPtr->mpItemXY )*/
            //dmeterDrawPtr->mpTextXY[i_no]->mWindow->mRotateOffsetX = dmeterDrawPtr->mpTextXY[i_no]->mWindow.
        //}
    //}

}     // namespace mod