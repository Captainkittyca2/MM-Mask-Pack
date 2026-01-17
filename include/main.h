/**
 * @file main.h
 * @author Captain Kitty Cat (youtube.com/@captainkittyca2)
 * @brief Adds three masks, 3-day cycle, ISG, and more mechanics from Majora's Mask onto TP
 * @version 0.6
 * @date 2026-01-12
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once
#include <display/console.h>
#include <tp/f_ap_game.h>
#include <tp/d_meter2_info.h>
#include <tp/control.h>
#include <tp/d_stage.h>
#include <tp/dynamic_link.h>
#include <tp/rel/d_a_arrow.h>
#include <tp/rel/d_a_e_ai.h>
#include <tp/rel/d_a_e_mm.h>
#include <tp/rel/d_a_e_sf.h>
#include <tp/rel/d_a_e_bu.h>
#include <tp/rel/d_a_b_bh.h>
#include <tp/d_kankyo.h>
#include <tp/d_camera.h>
#include <tp/d_gameover.h>
#include <Z2AudioLib/Z2AudioMgr.h>
#ifndef PLATFORM_WII
#include <tp/rel/d_a_e_db.h>
#include <tp/rel/d_a_e_dn.h>
#include <tp/rel/d_a_e_kr.h>
#include <tp/rel/d_a_e_rd.h>
#endif

#include <cinttypes>

#define ASM_NOP (0x60000000)

namespace mod
{
    //typedef void (*daE_fz_actionModeSet)(libtp::tp::f_op_actor::fopEn_enemy_c*, int32_t, int32_t);
#ifndef PLATFORM_WII
    typedef void (*daA_arrow_execute)(libtp::tp::rel::d_a_arrow::daArrow_c*);
    typedef void (*daE_db_action)(libtp::tp::rel::d_a_e_db::e_db_class*);
    typedef void (*daE_dn_action)(libtp::tp::rel::d_a_e_dn::e_dn_class*);
    typedef void (*daE_kr_action)(libtp::tp::rel::d_a_e_kr::e_kr_class*);
    typedef void (*daE_rd_action)(libtp::tp::rel::d_a_e_rd::e_rd_class*);
    typedef void (*daE_kk_actionModeSet)(libtp::tp::f_op_actor::fopEn_enemy_c*, int32_t, int32_t);
#endif
    typedef void (*daE_ww_actionModeSet)(libtp::tp::f_op_actor::fopEn_enemy_c*, int32_t, int32_t);
    typedef void (*daE_oc_actionModeSet)(libtp::tp::f_op_actor::fopEn_enemy_c*, int32_t, int32_t);
    typedef void (*daE_tt_actionModeSet)(libtp::tp::f_op_actor::fopEn_enemy_c*, int32_t, int32_t);
    typedef void (*daE_ai_action)(libtp::tp::rel::d_a_e_ai::e_ai_class*);
    typedef void (*daE_mm_action)(libtp::tp::rel::d_a_e_mm::e_mm_class*);
    typedef void (*daE_sf_action)(libtp::tp::rel::d_a_e_sf::e_sf_class*);
    typedef void (*daE_bu_attack)(libtp::tp::rel::d_a_e_bu::e_bu_class*);
    typedef bool (*daMidna_checkMetamorphoseEnableBase_Def)(void*);
    typedef void (*daNPC_impaa_execute)(libtp::tp::f_op_actor::fopAc_ac_c*);
    //typedef void (*daB_bh_execute)(libtp::tp::rel::d_a_b_bh::b_bh_class*);
    void main();
    class Mod
    {
       public:
        Mod();
        void init();

       private:
        // Console
        libtp::display::Console c;
        libtp::tp::d_a_alink::daAlink_BckData* ( *shieldManually )(libtp::tp::d_a_alink::daAlink* linkActrPtr, libtp::tp::d_a_alink::daAlink_ANM i_anmID) = nullptr;
        //void ( *healthUIOn)(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr) = nullptr;
        //void ( *offUI)(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr) = nullptr;
        void ( *meterValuee)(libtp::tp::d_meter2::dMeter2_c* dMeter2Ptr) = nullptr;
        void ( *unSheathShield)(libtp::tp::d_a_alink::daAlink* linkActrPtr) = nullptr;
        void ( *clothesChanging)(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param1) = nullptr;
        void ( *return_changedLink)(libtp::tp::d_a_alink::daAlink*, int32_t param0) = nullptr;
        void ( *return_initThePlayer)(libtp::tp::d_a_alink::daAlink*) = nullptr;
        void ( *return_equipSword)(libtp::tp::d_a_alink::daAlink*, int32_t) = nullptr;
        void ( *return_optionInitialize)(libtp::tp::d_menu_option::dMenu_Option_c*) = nullptr;
        void ( *return_attenStringSet)(libtp::tp::d_menu_option::dMenu_Option_c*) = nullptr;
        void ( *return_vibCamStringSet)(libtp::tp::d_menu_option::dMenu_Option_c*) = nullptr;
        void ( *return_soundPointerStringSet)(libtp::tp::d_menu_option::dMenu_Option_c*) = nullptr;
        void ( *return_attenMovee)(libtp::tp::d_menu_option::dMenu_Option_c*) = nullptr;
        void ( *return_vibCamMovee)(libtp::tp::d_menu_option::dMenu_Option_c*) = nullptr;
        void ( *return_soundPointerMovee)(libtp::tp::d_menu_option::dMenu_Option_c*) = nullptr;
        void ( *return_closedConfirmation)(libtp::tp::d_menu_option::dMenu_Option_c*) = nullptr;
        void ( *return_moveOptionsMenu)(libtp::tp::d_menu_option::dMenu_Option_c*) = nullptr;
        void ( *return_theClosedConfirmation)(libtp::tp::d_menu_option::dMenu_Option_c*) = nullptr;
        void ( *return_openConfirmation)(libtp::tp::d_menu_option::dMenu_Option_c*) = nullptr;
        void ( *return_wolfNow)(libtp::tp::d_a_alink::daAlink*, int32_t) = nullptr;
        void ( *return_zButtonRingWait)(libtp::tp::d_menu_ring::dMenu_Ring_c*) = nullptr;
        //void ( *return_zItemSeleect2)(libtp::tp::d_menu_ring::dMenu_Ring_c*, int32_t, uint8_t) = nullptr;
        void ( *return_swordMustBeSet)(libtp::tp::d_a_alink::daAlink*) = nullptr;
        void ( *return_swordMustBeUnSet)(libtp::tp::d_a_alink::daAlink*) = nullptr;
        void ( *return_gameChangerr)(void*) = nullptr;
        //void ( *return_itemZChangeeeee)(libtp::tp::d_a_alink::daAlink*) = nullptr;
        bool ( *return_do_unlink)(libtp::tp::dynamic_link::DynamicModuleControl* dmc) = nullptr;
        bool ( *do_link_trampoline)(libtp::tp::dynamic_link::DynamicModuleControl* dmc) = nullptr;
        bool ( *maskWearPermissionReturn)(uint16_t) = nullptr;
        bool ( *return_messageSmthg)(libtp::tp::control::TControl* control, const void* TProcessor, uint16_t unk3, uint16_t msgID) = nullptr;
        int32_t ( *initMap)(void* stageDt, libtp::tp::d_stage::stage_dzr_header_entry* i_data, int32_t num, void* raw_data) = nullptr;
        int32_t ( *return_regularDamageHurts)(libtp::tp::d_a_alink::daAlink* linkActrPtr, void*, int32_t) = nullptr;
        int32_t ( *initLargeDamage)(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0, int32_t param1, int16_t param2, int16_t param3, void* param4, int32_t param5) = nullptr;
        int32_t ( *return_damageSetIt)(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t a1, int32_t a2, int32_t a3, int32_t a4) = nullptr;
        int32_t ( *return_breakThatGuardNow)(libtp::tp::d_a_alink::daAlink*) = nullptr;
        //int32_t ( *talkCheck)(void* actorPtr, void* actorPtr2, uint16_t priority, uint16_t flag) = nullptr;
        int32_t ( *return_customItemSet)(libtp::tp::d_a_alink::daAlink*, uint8_t, int32_t) = nullptr;
        int32_t ( *return_jumpAutomaticInit)(libtp::tp::d_a_alink::daAlink*, int32_t) = nullptr;
        int32_t ( *return_hangStartInitNow)(libtp::tp::d_a_alink::daAlink*, void*) = nullptr;
        int32_t ( *return_executee)(libtp::tp::d_a_alink::daAlink*) = nullptr;
        int32_t ( *return_drawMaskOrNot)(libtp::tp::d_a_alink::daAlink*) = nullptr;
        int32_t ( *return_crouchhhh)(libtp::tp::d_a_alink::daAlink*) = nullptr;
        int32_t ( *return_chargerCutInitiate)(libtp::tp::d_a_alink::daAlink*) = nullptr;
        int32_t ( *return_cutJumpInitialized)(libtp::tp::d_a_alink::daAlink*, int32_t) = nullptr;
        int32_t ( *return_grassWhistleGetItIt)(libtp::tp::d_a_alink::daAlink*) = nullptr;
        int32_t grassWhistleGetItIt(libtp::tp::d_a_alink::daAlink*);
        int32_t ( *return_grassWhistleWaitItIt)(libtp::tp::d_a_alink::daAlink*) = nullptr;
        int32_t grassWhistleWaitItIt(libtp::tp::d_a_alink::daAlink*);
        int32_t ( *layerNumCommonCommon_return)(const char*, int32_t, int32_t) = nullptr;
        int32_t ( *goTalkFunnc_return)(libtp::tp::d_a_alink::daAlink*) = nullptr;
        int32_t goTalkFunnc(libtp::tp::d_a_alink::daAlink*);
        void ( *gotAnItem)(uint8_t item) = nullptr;
        void gotIt(uint8_t items);
        void ( *return_over0Retry)(libtp::tp::d_menu_save::dMenu_save_c*) = nullptr;
        void retryOver0Hook(libtp::tp::d_menu_save::dMenu_save_c*);
        void ( *return_sceneNamee)(libtp::z2audiolib::z2scenemgr::Z2SceneMgr*, char*, int32_t, int32_t) = nullptr;
        void sceneNameGiveIt(libtp::z2audiolib::z2scenemgr::Z2SceneMgr*, char*, int32_t, int32_t);
        //int32_t ( *return_bootsMoveAnimatiooon)(libtp::tp::d_a_alink::daAlink*, int32_t) = nullptr;
#ifndef PLATFORM_WII
        int32_t ( *return_useOrNoUseBecauseItemZ)(libtp::tp::d_a_alink::daAlink*, int32_t) = nullptr;
        int32_t isThereEvenAUseForThis(libtp::tp::d_a_alink::daAlink*, int32_t);
        void ( *return_zButtonRingSelect)(libtp::tp::d_menu_ring::dMenu_Ring_c*) = nullptr;
        void zButtonItemRingSelect(libtp::tp::d_menu_ring::dMenu_Ring_c*);
        bool ( *return_menuRingErasee)(libtp::tp::d_menu_ring::dMenu_Ring_c*) = nullptr;
        bool menuRingNowErasePlease(libtp::tp::d_menu_ring::dMenu_Ring_c*);
        uint8_t ( *return_getTheSelectedZItemGPGrandPrix)(int32_t) = nullptr;
        uint8_t getTheSelectedGrandPrixForZ(int32_t);
        void ( *return_zButtonAlphaAdjustment)(libtp::tp::d_meter2_draw::dMeter2Draw_c*, uint32_t) = nullptr;
        void zButtonAlphaAdjustment(libtp::tp::d_meter2_draw::dMeter2Draw_c*, uint32_t);
        void ( *return_bottleSettingsModel)(libtp::tp::d_a_alink::daAlink*, uint16_t) = nullptr;
        void bottleModelSetting(libtp::tp::d_a_alink::daAlink*, uint16_t);
        void ( *return_selectItemNumAddOnForZ)(int32_t, int16_t) = nullptr;
        void zSelectItemAddOnn(int32_t, int16_t);
        void ( *return_alphaButtonMinExamination)(libtp::tp::d_meter2_draw::dMeter2Draw_c*) = nullptr;
        void ( *return_alphaButtonMaxExamination)(libtp::tp::d_meter2_draw::dMeter2Draw_c*) = nullptr;
        void alphaButtonMinExaminatioon(libtp::tp::d_meter2_draw::dMeter2Draw_c*);
        void alphaButtonMaxExaminatioon(libtp::tp::d_meter2_draw::dMeter2Draw_c*);
        int32_t ( *return_drinkBottleZInitialized)(libtp::tp::d_a_alink::daAlink*, uint16_t) = nullptr;
        int32_t drinkBottleZInitializeed(libtp::tp::d_a_alink::daAlink*, uint16_t);
        int32_t ( *return_opeenBottleZInitialized)(libtp::tp::d_a_alink::daAlink*, uint16_t) = nullptr;
        int32_t openBottleZInitialized(libtp::tp::d_a_alink::daAlink*, uint16_t);
        int32_t ( *return_itmWaterUse)(libtp::tp::d_a_alink::daAlink*, uint16_t) = nullptr;
        int32_t itemWaterTime(libtp::tp::d_a_alink::daAlink*, uint16_t);
        void ( *mapDpadOn)(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr) = nullptr;
        void dPadMapOn(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr);
        void ( *mapDpadOff)(libtp::tp::d_meter2_draw::dMeter2Draw_c*) = nullptr;
        void dPadMapOfff(libtp::tp::d_meter2_draw::dMeter2Draw_c*);
        void ( *return_mapTheShow)() = nullptr;
        void (*return_mapTheNotShow)() = nullptr;
        void mapReturnTheShow();
        void mapReturnTheNotShow();
#else
        int32_t ( *return_hookShotItemWaterMasks)(int32_t) = nullptr;
        int32_t hookShotItemWaterMasks(int32_t);
        int32_t ( *return_normalCutProcInit)(libtp::tp::d_a_alink::daAlink*, int32_t) = nullptr;
        int32_t normalCutProcessOkDude(libtp::tp::d_a_alink::daAlink*, int32_t);
        int32_t ( *return_initWiiCutTurnProc)(libtp::tp::d_a_alink::daAlink*, int32_t, int32_t) = nullptr;
        int32_t cutTurnWiiStuffInit(libtp::tp::d_a_alink::daAlink*, int32_t, int32_t);
        void ( *return_initWiiCutDaaashProc)(libtp::tp::d_a_alink::daAlink*, int32_t, int32_t) = nullptr;
        void cutDaaashWiiStuffInit(libtp::tp::d_a_alink::daAlink*, int32_t, int32_t);
        void ( *return_shortCutInitialization)(libtp::tp::d_menu_option::dMenu_Option_c*) = nullptr;
        void ( *return_calibrationInitialization)(libtp::tp::d_menu_option::dMenu_Option_c*) = nullptr;
        void theShortCutInitializeed(libtp::tp::d_menu_option::dMenu_Option_c*);
        void theCalibrationInitializeed(libtp::tp::d_menu_option::dMenu_Option_c*);
#endif
        void ( *return_constFrameThing)() = nullptr;
        void procNewFrame();
        int32_t ( *return_newItemChangesEveryFrame)(libtp::tp::d_a_alink::daAlink*, uint8_t) = nullptr;
        int32_t newItemChangesEveryFrame(libtp::tp::d_a_alink::daAlink*, uint8_t);
        //int32_t ( *return_wiiiiiiideScreeeeen)(libtp::tp::d_camera::view_class*) = nullptr;
        //int32_t ( *return_allowTheMasksPlease)(libtp::tp::d_a_alink::daAlink*, int32_t) = nullptr;
        void ( *return_singleAnimeIsSet)(libtp::tp::d_a_alink::daAlink* linkActrPtr, uint16_t, libtp::tp::d_a_alink::daAlink_UPPER, float param0, float param1, int16_t param2, float param3) = nullptr;
        int32_t ( *return_itemTextureRead)(libtp::tp::d_meter2_info::G_Meter2_Info*, uint8_t itemNo, void* param_1, 
                                libtp::tp::J2DPicture::J2DPicture* param_2, void* param_3, 
                                libtp::tp::J2DPicture::J2DPicture* param_4, void* param_5, 
                                libtp::tp::J2DPicture::J2DPicture* param_6, void* param_7, 
                                libtp::tp::J2DPicture::J2DPicture* param_8, int32_t param_9) = nullptr;

        libtp::tp::d_a_alink::daAlink_BckData* manualShield(libtp::tp::d_a_alink::daAlink* linkActrPtr, libtp::tp::d_a_alink::daAlink_ANM i_anmID);
        //void UICheck(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr);
        //void UICheck2(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr);
        void closingConfirmation(libtp::tp::d_menu_option::dMenu_Option_c*);
        void moveInTheOptionsMenu(libtp::tp::d_menu_option::dMenu_Option_c*);
        void theClosingConfirmation(libtp::tp::d_menu_option::dMenu_Option_c*);
        void openingConfirmation(libtp::tp::d_menu_option::dMenu_Option_c*);
        void valueOfTheMeter(libtp::tp::d_meter2::dMeter2_c* dMeter2Ptr);
        void unSheathingShield(libtp::tp::d_a_alink::daAlink* linkActrPtr);
        void changingTheClothes(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param1);
        void changedLinkLook(libtp::tp::d_a_alink::daAlink*, int32_t);
        void playerInitialized(libtp::tp::d_a_alink::daAlink*);
        void swordEquipDecision(libtp::tp::d_a_alink::daAlink*, int32_t);
        void wolfNowComeOn(libtp::tp::d_a_alink::daAlink*, int32_t);
        //void performStaticASMReplacement(uint32_t memoryOffset, uint32_t value);
        //void iconBButtonDimFunc(uint8_t param0);
        void enemyNoticeSwitch(bool);
        void maskSelection(uint8_t, bool, libtp::tp::d_a_alink::daAlink*);
        void zButtonItemRingWait(libtp::tp::d_menu_ring::dMenu_Ring_c*);
        //void zItemSeleect2(libtp::tp::d_menu_ring::dMenu_Ring_c*, int32_t, uint8_t);
        void swordMustBeSet(libtp::tp::d_a_alink::daAlink*);
        void swordMustBeUnSet(libtp::tp::d_a_alink::daAlink*);
        void gameChangerr(void* param0);
        void initializeTheOption(libtp::tp::d_menu_option::dMenu_Option_c*);
        void stringAttentionDude(libtp::tp::d_menu_option::dMenu_Option_c*);
        void stringVibCamDude(libtp::tp::d_menu_option::dMenu_Option_c*);
        void stringSoundPointerDude(libtp::tp::d_menu_option::dMenu_Option_c*);
        void attentionMoving(libtp::tp::d_menu_option::dMenu_Option_c*);
        void vibCamMoving(libtp::tp::d_menu_option::dMenu_Option_c*);
        void soundPointerMoving(libtp::tp::d_menu_option::dMenu_Option_c*);
        //void itemZButtonChangeeeed(libtp::tp::d_a_alink::daAlink*);
        bool handle_do_unlink(libtp::tp::dynamic_link::DynamicModuleControl* dmc);
        void onRELUnlink(libtp::tp::dynamic_link::DynamicModuleControl* dmc);
        bool procDoLink(libtp::tp::dynamic_link::DynamicModuleControl* dmc );
        bool permissionForMask(uint16_t);
        bool customMessageCode(libtp::tp::control::TControl* control, const void* TProcessor, uint16_t unk3, uint16_t msgID);
        int32_t mapInitialized(void* stageDt, libtp::tp::d_stage::stage_dzr_header_entry* i_data, int32_t num, void* raw_data);
        int32_t largeDamageInitial(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t param0, int32_t param1, int16_t param2, int16_t param3, void* param4, int32_t param5);
        int32_t damagetheSettingPointMaybe(libtp::tp::d_a_alink::daAlink* linkActrPtr, int32_t a1, int32_t a2, int32_t a3, int32_t a4);
        int32_t dudeTheDamageHurts(libtp::tp::d_a_alink::daAlink* linkActrPtr, void*, int32_t);
        int32_t guardHasBroke(libtp::tp::d_a_alink::daAlink*);
        //int32_t pauseTalk(void* actorPtr, void* actorPtr2, uint16_t priority, uint16_t flag);
        int32_t customItemSetting(libtp::tp::d_a_alink::daAlink*, uint8_t, int32_t);
        int32_t jumpAutomatcInit(libtp::tp::d_a_alink::daAlink*, int32_t);
        int32_t hangStartFallInit(libtp::tp::d_a_alink::daAlink*, void*);
        int32_t executee(libtp::tp::d_a_alink::daAlink*);
        int32_t maskDrawOrNoDraw(libtp::tp::d_a_alink::daAlink*);
        int32_t crouchhhhhTime(libtp::tp::d_a_alink::daAlink*);
        int32_t cutTurnChargerInitiation(libtp::tp::d_a_alink::daAlink*);
        int32_t cutJumpManIniit(libtp::tp::d_a_alink::daAlink*, int32_t);
        int32_t layerCommonCommonNumm(const char*, int32_t, int32_t);
        //int32_t bootsMovingAnimatioon(libtp::tp::d_a_alink::daAlink*, int32_t);
        //int32_t isItAWideScreeeen(libtp::tp::d_camera::view_class*);
        //int32_t masksShallBeAllowed(libtp::tp::d_a_alink::daAlink*, int32_t);
        void animeSingleIsSet(libtp::tp::d_a_alink::daAlink* linkActrPtr, uint16_t, libtp::tp::d_a_alink::daAlink_UPPER, float param0, float param1, int16_t param2, float param3);
        int32_t reeadTheItemTexture(libtp::tp::d_meter2_info::G_Meter2_Info*, uint8_t itemNo, void* param_1, 
                                libtp::tp::J2DPicture::J2DPicture* param_2, void* param_3, 
                                libtp::tp::J2DPicture::J2DPicture* param_4, void* param_5, 
                                libtp::tp::J2DPicture::J2DPicture* param_6, void* param_7, 
                                libtp::tp::J2DPicture::J2DPicture* param_8, int32_t param_9);
    };
}     // namespace mod