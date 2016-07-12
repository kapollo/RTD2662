

#define __OSDDRAW002__


#include "Core\Header\Include.h"

#if(_OSD_TYPE == _OSD002)

#if(_UART_SND_EN == _OFF)
BYTE code sNotUse[] = " ";
#endif

//-------------------------------------------------------------------

void CDrawNull(BYTE ucDrawID)
{
     ucDrawID = ucDrawID;
}       /*
//-------------------------------------------------------------------
bit CMIVedio8Enable(void)
{
	if(bSourceVideo())
    	return 1;
    else return 0;
}         */
//-------------------------------------------------------------------
bit CMIVedioEnable(void)
{
	if(bSourceVideo() || _GET_INPUT_SOURCE() == _SOURCE_HDMI || _GET_INPUT_SOURCE() == _SOURCE_YPBPR)
    	return 1;
    else return 0;
}
//------------------------------------------------------------------------
bit CMIVGAEnable(void)
{
	if(_GET_INPUT_SOURCE() == _SOURCE_VGA)
    	return 1;
    else return 0;
}
//---------------------------------------------------------------------------
bit CMITVEnable(void)
{
#if(_FM_DEVICE)
	if(_GET_INPUT_SOURCE() == _SOURCE_VIDEO_TV || bFM == 1)
#else
	if(_GET_INPUT_SOURCE() == _SOURCE_VIDEO_TV)
#endif
    	return 1;
    else return 0;
}
//---------------------------------------------------------------------------

bit CMITVFMEnable(void)
{
#if(_FM_DEVICE)
    if(bFM)
       return 0;
#endif

    return 1;
}
//---------------------------------------------------------------------------



void CClearSubMenu(void)
{ 
    OSDClear(4, 13, 5, 26, 0x00, BYTE_DISPLAY);
    OSDClear(5, 12, 5, 26, 0x8c, BYTE_ATTRIB);
    OSDClear(5, 12, 5, 26, 0x10, BYTE_COLOR);
}
//---------------------------------------------------------------------------
void CClearAdjustMenu(void)
{
    OSDClear(ROW(17), HEIGHT(3), COL(2), WIDTH(27) , 0x00, BYTE_DISPLAY);
    OSDClear(ROW(17), HEIGHT(3), COL(2), WIDTH(27) , 0x8c, BYTE_ATTRIB);
    OSDClear(ROW(17), HEIGHT(3), COL(2), WIDTH(27) , 0x10, BYTE_COLOR);
}
//---------------------------------------------------------------------------
void Draw2bit3x2Icon(BYTE *tIcon,BYTE x,BYTE y)
{

     Gotoxy(x,y,ALL_BYTE);

     pData[0] = tIcon[0]; 
     pData[1] = tIcon[1];
     pData[2] = tIcon[2];
     
     pData[3] = tIcon[0];
     pData[4] = tIcon[1] + 1;
     pData[5] = tIcon[2];

     pData[6] = tIcon[0];
     pData[7] = tIcon[1] + 2;
     pData[8] = tIcon[2];     
     
     CScalerWrite(_OSD_DATA_PORT_92, 9, pData, _NON_AUTOINC);
   

     Gotoxy(x,y + 1,ALL_BYTE);

     pData[0] = tIcon[0]; 
     pData[1] = tIcon[1] + 3;
     pData[2] = tIcon[2];
     
     pData[3] = tIcon[0];
     pData[4] = tIcon[1] + 4;
     pData[5] = tIcon[2];

     pData[6] = tIcon[0];
     pData[7] = tIcon[1] + 5;
     pData[8] = tIcon[2];
     
     CScalerWrite(_OSD_DATA_PORT_92, 9, pData, _NON_AUTOINC); 

}
//---------------------------------------------------------------------------
void SubMenuItemSelect(BYTE ucItem)
{
    ucItem += 5;
    Gotoxy(COL(2),ucItem,BYTE_DISPLAY);
    Textout(tSelectIcon);

    OSDLine(ucItem, COL(5),LENGTH(26), 0x60, BYTE_COLOR);
}
//---------------------------------------------------------------------------
void SubMenuItemClr(BYTE ucItem)
{
	ucItem += 5;
	OSDLine(ucItem, COL(2),LENGTH(2),  0x00, BYTE_DISPLAY);
	OSDLine(ucItem, COL(5),LENGTH(26), 0x10, BYTE_COLOR);
}
//---------------------------------------------------------------------------
void MMBeforEnterSubMenu(void)
{
	BYTE ucCol = g_ucMenuItemIndex * 4 + 1;
	switch(g_ucMenuItemIndex)
	{
		case _MI_COLOR:
			Draw2bit3x2Icon(tColorIco_Sel1,ucCol,ROW(1));
			break;
		case _MI_ADJUST:
			Draw2bit3x2Icon(tAdjustIco_Sel1,ucCol,ROW(1));
			break;
		case _MI_OSD:
			Draw2bit3x2Icon(tOsdIco_Sel1,ucCol,ROW(1));
			break;
		case _MI_TV:
			Draw2bit3x2Icon(tTVIco_Sel1,ucCol,ROW(1));
			break;
		case _MI_FUNCTION:
			Draw2bit3x2Icon(tFuncIco_Sel1,ucCol,ROW(1));
			break;
	}
}

//---------------------------------------------------------------------------
void CCloseOSD(BYTE ucDrawID)
{
    ucDrawID = ucDrawID;
    COsdDispOsdTimerEvent();
}

//---------------------------------------------------------------------------
void CPDrawMainMenu(BYTE ucDrawID)
{
    switch(ucDrawID)
    {
        // Menu Draw ID
        case _DRAW_ENTER_MENU:              //进入当前的菜单时调用       	 
             InitMainMenu();
             
			 CDrawCurrentMenu();

             if (GET_OSD_SIZE())
                 OSDPosition(_MAINMENU_WIDTH * 12+680, _MAINMENU_HEIGHT * 18+520, stOsdUserData.OsdHPos, stOsdUserData.OsdVPos, 0x03);
             else
                 OSDPosition(_MAINMENU_WIDTH * 12, _MAINMENU_HEIGHT * 18, stOsdUserData.OsdHPos, stOsdUserData.OsdVPos, 0x03);
             
             CURRENT_MENU_ITEM.Draw(_DRAW_SELECT);
             
             COsdFxEnableOsd();
             if (ucDrawID != _DRAW_ENTER_MENU)
             {
#if(_UART_SND_EN == _ON)
                 CUartSendString("Init Osd End.\n");
                 CUartPrintf("dd", ucDrawID);
#else
                 CTextOutEx(sNotUse, 0, 0);
#endif
             }
             break;
             
        case _DRAW_BEFORE_RETURN_PARENTMENU://返回到上一级菜单之前...
             COsdDispOsdTimerEvent();
        	 break;
        case _DRAW_BEFORE_ENTER_SUBMENU:    //进入子菜单之前...
			 MMBeforEnterSubMenu();
        	 break;
        case _DRAW_FROM_SUBMENU_RETURN:     //从子菜单返回
             CURRENT_MENU_ITEM.Draw(_DRAW_SELECT);
             CClearAdjustMenu();
             break;
    }
}
//---------------------------------------------------------------------

void CPDrawColorMenu(BYTE ucDrawID)
{
    switch(ucDrawID)
    {
        // Menu Draw ID
        case _DRAW_ENTER_MENU:              //进入当前的菜单时调用
			CURRENT_MENU_ITEM.Draw(_DRAW_SELECT);
            break;
             
        case _DRAW_BEFORE_RETURN_PARENTMENU://返回到上一级菜单之前...
			CURRENT_MENU_ITEM.Draw(_DRAW_NORMARL);
            break;
            
        case _DRAW_BEFORE_ENTER_SUBMENU:    //进入子菜单之前...
            break;
             
        case _DRAW_FROM_SUBMENU_RETURN:     //从子菜单返回
            break;
    }
}
//---------------------------------------------------------------------
void CPDrawAdjustMenu(BYTE ucDrawID)
{
    switch(ucDrawID)
    {
        // Menu Draw ID
        case _DRAW_ENTER_MENU:              //进入当前的菜单时调用
			CURRENT_MENU_ITEM.Draw(_DRAW_SELECT);
            break;
             
        case _DRAW_BEFORE_RETURN_PARENTMENU://返回到上一级菜单之前...
			CURRENT_MENU_ITEM.Draw(_DRAW_NORMARL);
            break;
        case _DRAW_BEFORE_ENTER_SUBMENU:    //进入子菜单之前...
            break;
             
        case _DRAW_FROM_SUBMENU_RETURN:     //从子菜单返回
            break;
    }
}
//---------------------------------------------------------------------
void CPDrawOsdMenu(BYTE ucDrawID)
{
    switch(ucDrawID)
    {
        // Menu Draw ID
        case _DRAW_ENTER_MENU:              //进入当前的菜单时调用
			CURRENT_MENU_ITEM.Draw(_DRAW_SELECT);
            break;
             
        case _DRAW_BEFORE_RETURN_PARENTMENU://返回到上一级菜单之前...
			CURRENT_MENU_ITEM.Draw(_DRAW_NORMARL);
            break;
        case _DRAW_BEFORE_ENTER_SUBMENU:    //进入子菜单之前...
             break;
             
        case _DRAW_FROM_SUBMENU_RETURN:     //从子菜单返回
             break;
    }
}
//---------------------------------------------------------------------
void CPDrawTVMenu(BYTE ucDrawID)
{
    switch(ucDrawID)
    {
        // Menu Draw ID
        case _DRAW_ENTER_MENU:              //进入当前的菜单时调用
			CURRENT_MENU_ITEM.Draw(_DRAW_SELECT);
            break;
             
        case _DRAW_BEFORE_RETURN_PARENTMENU://返回到上一级菜单之前...
			CURRENT_MENU_ITEM.Draw(_DRAW_NORMARL);
            break;
        case _DRAW_BEFORE_ENTER_SUBMENU:    //进入子菜单之前...
             break;
             
        case _DRAW_FROM_SUBMENU_RETURN:     //从子菜单返回
             break;
    }
}
//---------------------------------------------------------------------
void CPDrawFuncMenu(BYTE ucDrawID)
{
    switch(ucDrawID)
    {
        // Menu Draw ID
        case _DRAW_ENTER_MENU:              //进入当前的菜单时调用
			CURRENT_MENU_ITEM.Draw(_DRAW_SELECT);
            break;
             
        case _DRAW_BEFORE_RETURN_PARENTMENU://返回到上一级菜单之前...
			CURRENT_MENU_ITEM.Draw(_DRAW_NORMARL);
            break;
        case _DRAW_BEFORE_ENTER_SUBMENU:    //进入子菜单之前...
             break;
             
        case _DRAW_FROM_SUBMENU_RETURN:     //从子菜单返回
             break;
    }
}

//========================== Shortcut key Menu ===========================
void CPDrawShowMenu(BYTE ucDrawID)
{
    switch(ucDrawID)
    {
        // Menu Draw ID
        case _DRAW_ENTER_MENU:              //进入当前的菜单时调用
			InitOsdFrame();

			COsdFxDrawWindow(8,0,											//WORD usXStart,WORD usYStart,  
					_DIALOG_WIDTH * 12 + 8,_DIALOG_HEIGHT * 18,				//WORD usXEnd,WORD usYEnd,  
					tMainWindowStyle);										//BYTE *pStyle)
                 
            if (GET_OSD_SIZE())
                OSDPosition(_DIALOG_WIDTH * 12+680, _DIALOG_HEIGHT * 18+520, stOsdUserData.OsdHPos, 100, 0x03);
            else
                OSDPosition(_DIALOG_WIDTH * 12, _DIALOG_HEIGHT * 18, stOsdUserData.OsdHPos, 100, 0x03);
                   
			CURRENT_MENU_ITEM.Draw(_DRAW_MENU_ITEM);
			COsdFxEnableOsd();
            break;
             
        case _DRAW_BEFORE_RETURN_PARENTMENU://返回到上一级菜单之前...
        	COsdFxDisableOsd();			
            break;
        case _DRAW_BEFORE_ENTER_SUBMENU:    //进入子菜单之前...
        
             break;
             
        case _DRAW_FROM_SUBMENU_RETURN:     //从子菜单返回
        
             break;
    }
}

//---------------------------------------------------------------------
void CMIDrawSMBright(BYTE ucDrawID)
{
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
			break;
		case _DRAW_MENU_ITEM:        //画菜单项
			CCenterTextout(sBrightness[GET_LANGUAGE()],ROW(1),COL(0),_DIALOG_WIDTH);
			DRAW_DIALOG_SLIDER(stConBriData.Brightness);
			break;

		case _DRAW_NORMARL:          //标准状态

			break;
		case _DRAW_DISABLE:          //禁用状态

			break;
	}
}
//---------------------------------------------------------------------------
void CMISMBrightRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:
            SET_KEYREPEATENABLE();
		    stConBriData.Brightness = ValueInRangeChange(0, 100, stConBriData.Brightness, _NON_LOOP | ucMode);
			DRAW_DIALOG_SLIDER(stConBriData.Brightness);
            CAdjustBrightness();
            ucOsdEventMsg = _SAVE_EE_COLORPROC0_MSG;			
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------SMVolume            ----------------------------
void CMIDrawSMVolume(BYTE ucDrawID)
{
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CCenterTextout(sVolume[GET_LANGUAGE()],ROW(1),COL(0),_DIALOG_WIDTH);
			DRAW_DIALOG_SLIDER(GET_VOLUME());
			break;

		case _DRAW_NORMARL:          //标准状态
			break; 

		case _DRAW_DISABLE:          //禁用状态
			break;
	}
}
//---------------------------------------------------------------------------
void CMISMVolumeRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
            SET_KEYREPEATENABLE();
            stAudioData.Volume = ValueInRangeChange(0, 100, stAudioData.Volume, _NON_LOOP | ucMode);
            DRAW_DIALOG_SLIDER(GET_VOLUME());
            CSetVolume();
            ucOsdEventMsg = _SAVE_EE_AUDIO_DATA_MSG;
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------

//---------------------------SMMute            ----------------------------
void CMIDrawSMMute(BYTE ucDrawID)
{
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			break;

		case _DRAW_NORMARL:          //标准状态
			break; 

		case _DRAW_DISABLE:          //禁用状态
			break;
	}
}


//-----------------------------------------------------------------------
void CSetMuteState(void)
{
    if (GET_AUDIO_MUTE()) // Mute on 
        CLR_AUDIO_MUTE();
    else
       SET_AUDIO_MUTE();

    CDrawMuteState();

#if(_IF_PLL_DE_CHIP == _IF_PLL_DE_1338)
    if ((_GET_INPUT_SOURCE() == _SOURCE_VIDEO_TV && gmi_CModeLocked()) || _GET_INPUT_SOURCE() != _SOURCE_VIDEO_TV)
#endif
       CSetVolume();
    ucOsdEventMsg = _SAVE_EE_AUDIO_DATA_MSG;
}
//---------------------------------------------------------------------------
void CMISMMuteRun(BYTE ucMode)
{
    if (_SET_MUTE == ucMode)
       CSetMuteState();
}

//---------------------------SMInput ch            ----------------------------
#if(_VIDEO_TV_SUPPORT)
#define _INPUT_NO_NUMBER       0
#define _INPUT_ONE_NUMBER      1
#define _INPUT_TWO_NUMBER      2
#define _INPUT_THREE_NUMBER    3
void CDrawInputChMenu(void)
{
    BYTE ucInputState = GET_INPUTCH_STATE() + 1;
    
   // iInputCHCount = _INPUT_CH_TIMEOUT_COUNT;
    // Reset input number and state
    ucCurrentInputNumber = 0;
    // 0: no input number   1: input one number     2: input two number    3: input three number
    ucCurrentInputState  = _INPUT_NO_NUMBER;  
    
    if (ucInputState > 3)
    {   // Input state err reset input state to input one channel -
        ucInputState = 1;
        SET_INPUTCH_STATE(0);
        CEepromSaveTvData();
    }

    COsdFxDisableOsd();
    SetOSDDouble(0x03);
    SetOsdMap(tUserMenuOsdMap);
                         
    // Init OSD Ram
    OSDClear(0, GET_OSD_MAP_ROWCOUNT(), 0, 30, 0x8C, BYTE_ATTRIB);
    OSDClear(0, GET_OSD_MAP_ROWCOUNT(), 0, 30, 0x00, BYTE_DISPLAY);
    OSDLine(0, 0, 30, 0x40, BYTE_COLOR);
    OSDClear(1, GET_OSD_MAP_ROWCOUNT() - 1, 0, 30, 0x60, BYTE_COLOR);
    
    COsdFxCodeWrite(ucCloseAllWindow);
    OSDPosition(_MAINMENU_WIDTH * 12, _MAINMENU_HEIGHT * 18, 50, 1, 0x03);
                        
    Gotoxy(1, 0, BYTE_DISPLAY);         
    for(; ucInputState > 0; ucInputState--)
        OutputChar(0x2D);  // Show "-"

    
    COsdFxEnableOsd();
    CTimerReactiveTimerEvent(SEC(30),COsdDispOsdTimerEvent);
    bOSDTimeOut = 0;
}

//---------------------------------------------------------------------------
void CMIDrawSMInputCh(BYTE ucDrawID)
{
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
            CDrawInputChMenu();
 			break;

		case _DRAW_NORMARL:          //标准状态
			break; 

		case _DRAW_DISABLE:          //禁用状态
			break;
	}
}

//---------------------------------------------------------------------------
void CMISMInputChRun(BYTE ucMode)
{
    BYTE ucInputState = GET_INPUTCH_STATE() + 1;

    switch(ucMode)
    {
        case _OE_SC_INPUT_NUM0:
        case _OE_SC_INPUT_NUM1:
        case _OE_SC_INPUT_NUM2:
        case _OE_SC_INPUT_NUM3:
        case _OE_SC_INPUT_NUM4:
        case _OE_SC_INPUT_NUM5:
        case _OE_SC_INPUT_NUM6:
        case _OE_SC_INPUT_NUM7:
        case _OE_SC_INPUT_NUM8:
        case _OE_SC_INPUT_NUM9:
            SInceptNumber(ucMode - _OE_SC_INPUT_NUM0);
            break;

        case _OE_SC_INPUT_CHANGE_CH:
//            iInputCHCount = _INPUT_CH_TIMEOUT_COUNT;
            ucInputState = ValueInRangeChange(1, 3, ucInputState, _LOOP | 0x01);
            SET_INPUTCH_STATE(ucInputState-1);
            CEepromSaveTvData();
            
            // Reset input number and state
            ucCurrentInputNumber = 0;
            // 0: no input number   1: input one number     2: input two number    3: input three number
            ucCurrentInputState  = 0;  
            break;

        case _MENU_RUN:
            ucInputState = ucCurrentInputState;
            break;
    }

    SShowCurrentInputState(ucInputState);
    
    if (ucInputState == ucCurrentInputState)
    {
        BYTE ucMaxChannel = CloadMaxChannelNumber();
        
        CMuteOn();
        COsdDispOsdTimerEvent();
        CModeResetTVMode();
        CTimerDelayXms(200);
        
#if(_FM_DEVICE)
        if (1 == bFM)
        {
            ucPrevChannel =  stTvInfo.ucFMCurrCh;
            stTvInfo.ucFMCurrCh = (ucCurrentInputNumber > ucMaxChannel) ? ucMaxChannel : ucCurrentInputNumber;
            CSetTVChannel(stTvInfo.ucFMCurrCh);
            CShowTVNumber(stTvInfo.ucFMCurrCh, _SHOW_CH_TV_NUMBER);
        }
        else
#endif   
        {
            ucPrevChannel =  stTvInfo.CurChn;
            stTvInfo.CurChn = (ucCurrentInputNumber > ucMaxChannel) ? ucMaxChannel : ucCurrentInputNumber;
            CSetTVChannel(stTvInfo.CurChn);
//            CShowTVNumber(stTvInfo.CurChn,_SHOW_CH_TV_NUMBER | _SHOW_CH_TV_TYPE | _SHOW_CH_SOUND_TYPE);
            gmi_CI2CWriteIfPllDM(ucTVType, _TUNER_MUTE_OFF, _NORMAL_MODE);
        }

        ucOsdEventMsg = _SAVE_EE_TV_DATA_MSG;
    }

    CTimerReactiveTimerEvent(SEC(30), COsdDispOsdTimerEvent);
    bOSDTimeOut = 0;
}

//---------------------------------------------------------
void SInceptNumber(const WORD ucNumber)
{
    switch(ucCurrentInputState)
    { 
        case _INPUT_NO_NUMBER: // not input number
            ucCurrentInputNumber = ucNumber;
            ucCurrentInputState  = _INPUT_ONE_NUMBER;
            break;
            
        case _INPUT_ONE_NUMBER: // Input on number alrady
        case _INPUT_TWO_NUMBER: // Input two number alrady
            ucCurrentInputNumber = (ucCurrentInputNumber*10) + ucNumber;
            ucCurrentInputState  = (ucCurrentInputState == _INPUT_TWO_NUMBER) ? _INPUT_THREE_NUMBER : _INPUT_TWO_NUMBER;
            break;
            /*
            case _INPUT_TWO_NUMBER: // Input two number alrady
            ucCurrentInputNumber = (ucCurrentInputNumber*10) + ucNumber;
            ucCurrentInputState  = _INPUT_THREE_NUMBER;
            break;*/
    }
}

//---------------------------------------------------------
void SShowCurrentInputState(WORD ucInputState)
{
    Gotoxy(1, 0, BYTE_DISPLAY); 
    
    switch(ucCurrentInputState)
    {
        case _INPUT_NO_NUMBER: // not input number
            for(;ucInputState > 0;ucInputState--)
                OutputChar(0x2D);  // Show "-"
            OutputChar(0x00);  // Show " "
            OutputChar(0x00);  // Show " "
            break;
            
        case _INPUT_ONE_NUMBER: // Input on number alrady
            CShowNumber1(ucCurrentInputNumber,1);
            break;
            
        case _INPUT_TWO_NUMBER: // Input two number alrady
            CShowNumber1(ucCurrentInputNumber,2);
            break;
            
        case _INPUT_THREE_NUMBER: // Input three number alrady
            CShowNumber1(ucCurrentInputNumber,3);
            break;
    }
}

#endif // #if(_VIDEO_TV_SUPPORT)
//========================== Shortcut key Menu end ===========================

//---------------------------COLOR               ----------------------------
void CMIDrawCOLOR(BYTE ucDrawID)
{
    BYTE ucCol = _MI_COLOR * 4 + 1;
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
			Draw2bit3x2Icon(tColorIco_Select,ucCol,ROW(1));
			FMI_DRAW_PAGE_TITLE(sColor[GET_LANGUAGE()]);
			CDrawSubMenu();
			break;
		case _DRAW_MENU_ITEM:        //画菜单项
			Draw2bit3x2Icon(tColorIco_Normal,ucCol,ROW(1)); 
			break;
		case _DRAW_NORMARL:          //标准状态
			Draw2bit3x2Icon(tColorIco_Normal,ucCol,ROW(1));
			CClearSubMenu();
			break;
		case _DRAW_DISABLE:          //禁用状态
			Draw2bit3x2Icon(tColorIco_Disable,ucCol,ROW(1));
			break;
	}
}
//---------------------------------------------------------------------------
void CMICOLORRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:
        case _MENU_ADJ_INC:
            break;

        case _MENU_RUN:
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------ADJUST              ----------------------------
void CMIDrawADJUST(BYTE ucDrawID)
{
	BYTE ucCol = _MI_ADJUST * 4 + 1;
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
			Draw2bit3x2Icon(tAdjustIco_Select,ucCol,ROW(1));
			FMI_DRAW_PAGE_TITLE(sAdjust[GET_LANGUAGE()]);
			CDrawSubMenu();
			break;
		case _DRAW_MENU_ITEM:        //画菜单项
			Draw2bit3x2Icon(tAdjustIco_Normal,ucCol,ROW(1));
			break;
		case _DRAW_NORMARL:          //标准状态
			Draw2bit3x2Icon(tAdjustIco_Normal,ucCol,ROW(1));
			CClearSubMenu();
			break;
		case _DRAW_DISABLE:          //禁用状态
			Draw2bit3x2Icon(tAdjustIco_Disable,ucCol,ROW(1));
			break;
	}
}
//---------------------------------------------------------------------------
void CMIADJUSTRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:
            
        case _MENU_ADJ_INC:
            
            break;
        case _MENU_RUN:
            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------OSD                 ----------------------------
void CMIDrawOSD(BYTE ucDrawID)
{
	BYTE ucCol = _MI_OSD * 4 + 1;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
			Draw2bit3x2Icon(tOsdIco_Select,ucCol,ROW(1));
			FMI_DRAW_PAGE_TITLE(sOSD[GET_LANGUAGE()]);
			CDrawSubMenu();
			break;
		case _DRAW_MENU_ITEM:        //画菜单项
			Draw2bit3x2Icon(tOsdIco_Normal,ucCol,ROW(1));          
			break;
		case _DRAW_NORMARL:          //标准状态
			Draw2bit3x2Icon(tOsdIco_Normal,ucCol,ROW(1));
			CClearSubMenu();
			break;
		case _DRAW_DISABLE:          //禁用状态
			Draw2bit3x2Icon(tOsdIco_Disable,ucCol,ROW(1));
			break;
	}

}
//---------------------------------------------------------------------------
void CMIOSDRun(BYTE ucMode)
{  
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:
        case _MENU_ADJ_INC:           
            break;

        case _MENU_RUN:
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------TV                  ----------------------------
void CMIDrawTV(BYTE ucDrawID)
{
	BYTE ucCol = _MI_TV * 4 + 1;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
			Draw2bit3x2Icon(tTVIco_Select,ucCol,ROW(1));
			FMI_DRAW_PAGE_TITLE(sTV[GET_LANGUAGE()]);
			CDrawSubMenu();

			break;
		case _DRAW_MENU_ITEM:        //画菜单项
			Draw2bit3x2Icon(tTVIco_Normal,ucCol,ROW(1));          
			break;
		case _DRAW_NORMARL:          //标准状态
			Draw2bit3x2Icon(tTVIco_Normal,ucCol,ROW(1));
			CClearSubMenu();
			break;
		case _DRAW_DISABLE:          //禁用状态
			Draw2bit3x2Icon(tTVIco_Disable,ucCol,ROW(1));
			break;
	}
}
//---------------------------------------------------------------------------
void CMITVRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:
        case _MENU_ADJ_INC:
            break;

        case _MENU_RUN:
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------FUNCTION            ----------------------------
void CMIDrawFUNCTION(BYTE ucDrawID)
{
	BYTE ucCol = _MI_FUNCTION * 4 + 1;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
			Draw2bit3x2Icon(tFuncIco_Select,ucCol,ROW(1));
			FMI_DRAW_PAGE_TITLE(sFunction[GET_LANGUAGE()]);
			CDrawSubMenu();
			break;
		case _DRAW_MENU_ITEM:        //画菜单项
			Draw2bit3x2Icon(tFuncIco_Normal,ucCol,ROW(1));          
			break;
		case _DRAW_NORMARL:          //标准状态
			Draw2bit3x2Icon(tFuncIco_Normal,ucCol,ROW(1));
			CClearSubMenu();
			break;
		case _DRAW_DISABLE:          //禁用状态
			Draw2bit3x2Icon(tFuncIco_Disable,ucCol,ROW(1));
			break;
	}
}
//---------------------------------------------------------------------------
void CMIFUNCTIONRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:
            
        case _MENU_ADJ_INC:
            
            break;
        case _MENU_RUN:
            
            break;
    }
}
//---------------------------------------------------------------------------




//---------------------------EXIT                ----------------------------
void CMIDrawEXIT(BYTE ucDrawID)
{
	BYTE ucCol = _MI_EXIT * 4 + 1;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
			Draw2bit3x2Icon(tExitIco_Select,ucCol,ROW(1));
			FMI_DRAW_PAGE_TITLE(sExit[GET_LANGUAGE()]);
			CDrawSubMenu();
			break;
			
		case _DRAW_MENU_ITEM:        //画菜单项
			Draw2bit3x2Icon(tExitIco_Normal,ucCol,ROW(1));          
			break;

		case _DRAW_NORMARL:          //标准状态
			Draw2bit3x2Icon(tExitIco_Normal,ucCol,ROW(1));
			CClearSubMenu();
			break;

		case _DRAW_DISABLE:          //禁用状态
			Draw2bit3x2Icon(tExitIco_Disable,ucCol,ROW(1));
			break;
	}
}
//---------------------------------------------------------------------------
void CMIEXITRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC: 
        case _MENU_RUN:            
          //  COsdDispOsdTimerEvent();

            break;
    }
}
//---------------------------------------------------------------------------



//---------------------------Brightness          ----------------------------
void CMIDrawBrightness(BYTE ucDrawID)
{
	BYTE ucRow = _MI_BRIGHTNESS + 5;

	switch(ucDrawID)
	{
	  case _DRAW_SELECT:           //选中状态
	      SubMenuItemSelect(_MI_BRIGHTNESS);
	      DRAW_SLIDER(stConBriData.Brightness);
	      break;
	      
	  case _DRAW_MENU_ITEM:        //画菜单项
	      CTextOutEx(sBrightness[GET_LANGUAGE()],COL(5),ucRow);	      
	      break;
	      
	  case _DRAW_NORMARL:          //标准状态
	      SubMenuItemClr(_MI_BRIGHTNESS);
	      //CClearAdjustMenu();
	      break;

	  case _DRAW_DISABLE:          //禁用状态
	      FMI_DISABLE_MENIITEM_LINE(ucRow);
	      break;
	}
}
//---------------------------------------------------------------------------
void CMIBrightnessRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:
        case _MENU_ADJ_INC:
            SET_KEYREPEATENABLE();
		    stConBriData.Brightness = ValueInRangeChange(0, 100, stConBriData.Brightness, _NON_LOOP | ucMode);
			DRAW_SLIDER(stConBriData.Brightness);
            CAdjustBrightness();
            ucOsdEventMsg = _SAVE_EE_COLORPROC0_MSG;			
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------



//---------------------------Contrast            ----------------------------
void CMIDrawContrast(BYTE ucDrawID)
{
	BYTE ucRow = _MI_CONTRAST + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_CONTRAST);
	      	DRAW_SLIDER(stConBriData.Contrast);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sContrast[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_CONTRAST);
			//CClearAdjustMenu();
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------

void CMIContrastRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:         
        case _MENU_ADJ_INC:
            SET_KEYREPEATENABLE();
            stConBriData.Contrast = ValueInRangeChange(0, 100, stConBriData.Contrast, _NON_LOOP | ucMode);
            DRAW_SLIDER(stConBriData.Contrast);            
            CAdjustContrast();
            ucOsdEventMsg = _SAVE_EE_COLORPROC0_MSG;
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------Hue                 ----------------------------
void CMIDrawHue(BYTE ucDrawID)
{
	BYTE ucRow = _MI_HUE + 5;
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_HUE);
	      	DRAW_SLIDER(GET_HUE());
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sHue[GET_LANGUAGE()],COL(5),ucRow);

			break;
		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_HUE);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIHueRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:           
        case _MENU_ADJ_INC:
            SET_KEYREPEATENABLE();
            stHueSatData.Hue = ValueInRangeChange(0, 100, stHueSatData.Hue, _NON_LOOP | ucMode);
            DRAW_SLIDER(GET_HUE());            
	        if(bSourceVideo())
                gmi_CAdjustVDCHue(GET_HUE());
            else
                CAdjustYpbprhue(GET_HUE());
            ucOsdEventMsg = _SAVE_EE_HUE_SAT_DATA_MSG;
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------Saturation          ----------------------------
void CMIDrawSaturation(BYTE ucDrawID)
{
	BYTE ucRow = _MI_SATURATION + 5;
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_SATURATION);
			DRAW_SLIDER(GET_SATURATION());
            break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sSaturation[GET_LANGUAGE()],COL(5),ucRow);
	      	
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_SATURATION);
			//CClearAdjustMenu();
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMISaturationRun(BYTE ucMode)
{	
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:
            SET_KEYREPEATENABLE();
            stHueSatData.Saturation = ValueInRangeChange(0, 100, stHueSatData.Saturation, _NON_LOOP | ucMode);
            DRAW_SLIDER(GET_SATURATION());            
	        if(bSourceVideo())
                CVideoSetSaturation(GET_SATURATION());
            else
                CAdjustYpbprSaturation(GET_SATURATION()); 
            ucOsdEventMsg = _SAVE_EE_HUE_SAT_DATA_MSG;  
          
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------ColorTemp           ----------------------------
void CMIDrawColorTemp(BYTE ucDrawID)
{
	BYTE ucRow = _MI_COLORTEMP + 5;
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_COLORTEMP);
            if (GET_COLOR_TEMP_TYPE() > 4)
                SET_COLOR_TEMP_TYPE(0);

			CCenterTextout(sColorTempType[GET_COLOR_TEMP_TYPE()],ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sColorTemp[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_COLORTEMP);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIColorTempRun(BYTE ucMode)
{
    BYTE ucTemp = GET_COLOR_TEMP_TYPE();

    switch(ucMode)
    {
        case _MENU_ADJ_DEC:
        case _MENU_ADJ_INC: 
            ucTemp = ValueInRangeChange(0, 4, ucTemp, _LOOP | ucMode);
            SET_COLOR_TEMP_TYPE(ucTemp);
            CClearAdjustMenu();
			CCenterTextout(sColorTempType[GET_COLOR_TEMP_TYPE()],ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
        	CEepromLoadColorTempData();
        	CAdjustContrast();
            ucOsdEventMsg = _SAVE_EE_SYSTEMDATA_MSG;
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------





//---------------------------ColorReturn         ----------------------------
void CMIDrawColorReturn(BYTE ucDrawID)
{
	BYTE ucRow = _MI_COLORRETURN + 5;
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_COLORRETURN);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sReturn[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_COLORRETURN);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIColorReturnRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
        case _MENU_RUN:
            CReturnUpMenu();
            break;
    }
}
//---------------------------------------------------------------------------





//---------------------------AutoConfig          ----------------------------
void CMIDrawAutoConfig(BYTE ucDrawID)
{
	BYTE ucRow = _MI_AUTOCONFIG + 5;
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_AUTOCONFIG);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sAutoConfig[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_AUTOCONFIG);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIAutoConfigRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:
            CAutoDoAutoConfig();
            //CAutoDoWhiteBalance();
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------HPosition           ----------------------------
void CMIDrawHPosition(BYTE ucDrawID)
{
	BYTE ucRow = _MI_HPOSITION + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_HPOSITION);
	      	DRAW_SLIDER(COsdCtrlGetHPosition());
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sHPosition[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_HPOSITION);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIHPositionRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
            SET_KEYREPEATENABLE();
            stModeUserData.HPosition = ValueInRangeChange(stModeUserCenterData.CenterHPos - _HPOSITION_BIAS,
                                                           stModeUserCenterData.CenterHPos + _HPOSITION_BIAS,
                                                           stModeUserData.HPosition,_NON_LOOP | ucMode);
          	CAdjustHPosition();
	      	DRAW_SLIDER(COsdCtrlGetHPosition());
            ucOsdEventMsg = _SAVE_EE_MODEUSERDATA_MSG;
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------VPosition           ----------------------------
void CMIDrawVPosition(BYTE ucDrawID)
{
	BYTE ucRow = _MI_VPOSITION + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_VPOSITION);
	      	DRAW_SLIDER(COsdCtrlGetVPosition());
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sVPosition[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_VPOSITION);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIVPositionRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:
            SET_KEYREPEATENABLE();
            stModeUserData.VPosition = ValueInRangeChange(stModeUserCenterData.CenterVPos - _VPOSITION_BIAS,stModeUserCenterData.CenterVPos + _VPOSITION_BIAS,stModeUserData.VPosition,_NON_LOOP | ucMode);
        	CAdjustVPosition();
	      	DRAW_SLIDER(COsdCtrlGetVPosition());            
            ucOsdEventMsg = _SAVE_EE_MODEUSERDATA_MSG;
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------Phase               ----------------------------
void CMIDrawPhase(BYTE ucDrawID)
{
	BYTE ucRow = _MI_PHASE + 5;
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_PHASE);
	      	DRAW_SLIDER(COsdCtrlGetPhase());
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sPhase[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_PHASE);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIPhaseRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
            SET_KEYREPEATENABLE();
        	if((ucMode == _MENU_ADJ_DEC) && stModeUserData.Phase > 0)
        	  	stModeUserData.Phase -= 1;
        	else if((ucMode == _MENU_ADJ_INC) && stModeUserData.Phase < 63)
        	  	stModeUserData.Phase += 1;

        	CAdjustPhase(stModeUserData.Phase);
	      	DRAW_SLIDER(COsdCtrlGetPhase());            
            ucOsdEventMsg = _SAVE_EE_MODEUSERDATA_MSG;
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------Clock               ----------------------------
void CMIDrawClock(BYTE ucDrawID)
{
	BYTE ucRow = _MI_CLOCK + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_CLOCK);
	      	DRAW_SLIDER(COsdCtrlGetClock());
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sClock[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_CLOCK);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIClockRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:
            SET_KEYREPEATENABLE();
        	if ((ucMode == _MENU_ADJ_DEC) && (stModeUserData.Clock > COsdFxGetAdcClockRange(_GET_CLOCKRANGE_MIN)))
        	   stModeUserData.Clock -= 1;
        	else if((ucMode == _MENU_ADJ_INC) && (stModeUserData.Clock < COsdFxGetAdcClockRange(_GET_CLOCKRANGE_MAX)))
        	   stModeUserData.Clock += 1;

	      	DRAW_SLIDER(COsdCtrlGetClock());            
        	CAdjustAdcClock(stModeUserData.Clock);
            ucOsdEventMsg = _SAVE_EE_MODEUSERDATA_MSG;
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------AdjustReturn        ----------------------------
void CMIDrawAdjustReturn(BYTE ucDrawID)
{
	BYTE ucRow = _MI_ADJUSTRETURN + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_ADJUSTRETURN);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sReturn[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_ADJUSTRETURN);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIAdjustReturnRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
        case _MENU_RUN:
            CReturnUpMenu();            
            break;
    }
}
//---------------------------------------------------------------------------




//---------------------------Language            ----------------------------
void CMIDrawLanguage(BYTE ucDrawID)
{
	BYTE ucRow = _MI_LANGUAGE + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_LANGUAGE);
			CCenterTextout(sLanguageName[GET_LANGUAGE()],ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sLanguage[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_LANGUAGE);
			//CClearAdjustMenu();
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}

//-----------------------------------------------------------------------
void CAdjLanguage(BYTE ucAdj)
{
    BYTE ucLng = GET_LANGUAGE();
    
    if (ucAdj) 
        ucLng = GetNextLanguage(ucLng);
    else   
        ucLng = GetPrevLanguage(ucLng);
    
    SET_LANGUAGE(ucLng);
    
    // Load Font
    LoadLanguageFont();
}

//---------------------------------------------------------------------------
void CMILanguageRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:
            CClearAdjustMenu();
            CClearSubMenu();
            CAdjLanguage(ucMode);
            CDrawCurrentMenu();
			CCenterTextout(sLanguageName[GET_LANGUAGE()],ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
            ucOsdEventMsg = _SAVE_EE_OSDUSERDATA_MSG;            
            break;

        case _MENU_RUN:            
            break;
    }
}


//---------------------------HPosition           ----------------------------
void CMIDrawOSDHPosition(BYTE ucDrawID)
{
	BYTE ucRow = _MI_OSDHPOSITION + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_OSDHPOSITION);
	      	DRAW_SLIDER(stOsdUserData.OsdHPos);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sHPosition[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_OSDHPOSITION);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIOSDHPositionRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
            SET_KEYREPEATENABLE();
            stOsdUserData.OsdHPos = ValueInRangeChange(0, 100, stOsdUserData.OsdHPos, _NON_LOOP | ucMode);
            DRAW_SLIDER(stOsdUserData.OsdHPos);            
            if (GET_OSD_SIZE())
                OSDPosition(_MAINMENU_WIDTH * 12+680, _MAINMENU_HEIGHT * 18+520, stOsdUserData.OsdHPos, stOsdUserData.OsdVPos, 0x03);
            else
                OSDPosition(_MAINMENU_WIDTH * 12, _MAINMENU_HEIGHT * 18, stOsdUserData.OsdHPos, stOsdUserData.OsdVPos, 0x03);
            ucOsdEventMsg = _SAVE_EE_OSDUSERDATA_MSG;            
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------VPosition           ----------------------------
void CMIDrawOSDVPosition(BYTE ucDrawID)
{
	BYTE ucRow = _MI_OSDVPOSITION+ 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_OSDVPOSITION);
	      	DRAW_SLIDER(stOsdUserData.OsdVPos);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sVPosition[GET_LANGUAGE()],COL(5),ucRow);
			//CClearAdjustMenu();
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_OSDVPOSITION);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIOSDVPositionRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:
            SET_KEYREPEATENABLE();
            stOsdUserData.OsdVPos = ValueInRangeChange(0, 100, stOsdUserData.OsdVPos, _NON_LOOP | ucMode);
            DRAW_SLIDER(stOsdUserData.OsdVPos);            
            if (GET_OSD_SIZE())
                OSDPosition(_MAINMENU_WIDTH * 12+680, _MAINMENU_HEIGHT * 18+520, stOsdUserData.OsdHPos, stOsdUserData.OsdVPos, 0x03);
            else
                OSDPosition(_MAINMENU_WIDTH * 12, _MAINMENU_HEIGHT * 18, stOsdUserData.OsdHPos, stOsdUserData.OsdVPos, 0x03);
            ucOsdEventMsg = _SAVE_EE_OSDUSERDATA_MSG;                        
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------Timer               ----------------------------
void CMIDrawTimer(BYTE ucDrawID)
{
	BYTE ucRow = _MI_TIMER + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_TIMER);
            if (stOsdUserData.OsdTimeout < 5)
       			CCenterTextout(SOnOff[(bit)GET_OSD_SIZE()][GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
            else
                CShowNumber(_SHOW_NUM_POS_X, _SHOW_NUM_POS_Y, GET_OSDTIMEOUT());
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sTimer[GET_LANGUAGE()],COL(5),ucRow);
			//CClearAdjustMenu();
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_TIMER);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMITimerRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
            stOsdUserData.OsdTimeout = ValueInRangeChange(4, 60, stOsdUserData.OsdTimeout, _LOOP | ucMode);
			CClearAdjustMenu();
            if (stOsdUserData.OsdTimeout < 5)
       			CCenterTextout(SOnOff[(bit)GET_OSD_SIZE()][GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
            else
                CShowNumber(_SHOW_NUM_POS_X, _SHOW_NUM_POS_Y, GET_OSDTIMEOUT());
            ucOsdEventMsg = _SAVE_EE_OSDUSERDATA_MSG;                        
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------Double              ----------------------------
void CMIDrawDouble(BYTE ucDrawID)
{
	BYTE ucRow = _MI_DOUBLE + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_DOUBLE);
			CCenterTextout(SOnOff[(bit)GET_OSD_SIZE()][GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sDouble[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_DOUBLE);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIDoubleRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:
            SET_OSD_SIZE((GET_OSD_SIZE()) ? 0 : 1);
            CClearAdjustMenu();
			CCenterTextout(SOnOff[(bit)GET_OSD_SIZE()][GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
            ucOsdEventMsg = _SAVE_EE_SYSTEMDATA_MSG;

            COsdFxDisableOsd();
            if (GET_OSD_SIZE())
            {
                SetOSDDouble(0x03 | OSD_WINDOWCHAR_BLENDING);
                OSDPosition(_MAINMENU_WIDTH * 12+680, _MAINMENU_HEIGHT * 18+520, stOsdUserData.OsdHPos, stOsdUserData.OsdVPos, 0x03);
            }
            else
            {
                SetOSDDouble(OSD_WINDOWCHAR_BLENDING);
                OSDPosition(_MAINMENU_WIDTH * 12, _MAINMENU_HEIGHT * 18, stOsdUserData.OsdHPos, stOsdUserData.OsdVPos, 0x03);
            }
            COsdFxEnableOsd();
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------Transparent         ----------------------------
void CMIDrawTransparent(BYTE ucDrawID)
{
	BYTE ucRow = _MI_TRANSPARENT + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_TRANSPARENT);
            CShowNumber(_SHOW_NUM_POS_X, _SHOW_NUM_POS_Y, GET_OSDBLENDING());
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sTransparent[GET_LANGUAGE()],COL(5),ucRow);
			//CClearAdjustMenu();
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_TRANSPARENT);
			//CClearAdjustMenu();
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMITransparentRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
            stOsdUserData.OsdBlending = ValueInRangeChange(0, 7, stOsdUserData.OsdBlending, _LOOP | ucMode);
			CClearAdjustMenu();
            CShowNumber(_SHOW_NUM_POS_X, _SHOW_NUM_POS_Y, GET_OSDBLENDING());
        	CScalerSetBit(_OVERLAY_CTRL_6C, 0x23, ((stOsdUserData.OsdBlending & 0x07) << 2));
            ucOsdEventMsg = _SAVE_EE_OSDUSERDATA_MSG;                        
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------OsdReturn           ----------------------------
void CMIDrawOsdReturn(BYTE ucDrawID)
{
	BYTE ucRow = _MI_OSDRETURN + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_OSDRETURN);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sReturn[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_OSDRETURN);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIOsdReturnRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
        case _MENU_RUN:            
            CReturnUpMenu();
            break;
    }
}
//---------------------------------------------------------------------------



//---------------------------System              ----------------------------
void CMIDrawSystem(BYTE ucDrawID)
{
	BYTE ucRow = _MI_SYSTEM + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_SYSTEM);
			CCenterTextout(sTVType[ucTVType],ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sSystem[GET_LANGUAGE()],COL(5),ucRow);
			//CClearAdjustMenu();
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_SYSTEM);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}


//-----------------------------------------------------------------------
bit bSearchedTVType(BYTE ucTvType)
{
    switch(ucTvType) 
    {
        case _TV_NTSC_M    :    // 60Hz Sound 4.5M  NTSC M
#if(_TV_NTSC_M_SUPPORT)
            return 1;
#endif
            break;
        
        case _TV_NTSC_4_BG :    // 60Hz Sound 5.5M  NTSC 4/BG 
#if(_TV_NTSC_4_BG_SUPPORT)
            return 1;
#endif
            break;
        
        case _TV_NTSC_4_DK :    // 60Hz Sound 6.5M  NTSC 4/DK 
#if(_TV_NTSC_4_DK_SUPPORT)
            return 1;
#endif
            break;
        
        case _TV_NTSC_4_I  :    // 60Hz Sound 6.0M  NTSC 4/I
#if(_TV_NTSC_4_I_SUPPORT)
            return 1;
#endif
            break;
        
        case _TV_PAL_M     :    // 60Hz Sound 4.5M  PAL M  
#if(_TV_PAL_M_SUPPORT)
            return 1;
#endif
            break;
        
        case _TV_PAL_BG    :    // 50Hz Sound 5.5M  PAL B/G
#if(_TV_PAL_BG_SUPPORT)
            return 1;
#endif
            break;
        
        case _TV_PAL_I     :    // 50Hz Sound 6.0M  PAL I
#if(_TV_PAL_I_SUPPORT)
            return 1;
#endif
            break;
        
        case _TV_PAL_DK    :    // 50Hz Sound 6.5M  PAL D/K
#if(_TV_PAL_DK_SUPPORT)
            return 1;
#endif
            break;
        
        case _TV_PAL_N     :    // 50Hz Sound 4.5M  PAL N 
#if(_TV_PAL_N_SUPPORT)
            return 1;
#endif
            break;
        
        case _TV_SECAM_BG  :    // 50Hz Sound 5.5M  SECAM B/G
#if(_TV_SECAM_BG_SUPPORT)
            return 1;
#endif
            break;
        
        case _TV_SECAM_DK  :    // 50Hz Sound 6.5M  SECAM D/K
#if(_TV_SECAM_DK_SUPPORT)
            return 1;
#endif
            break;
        
        case _TV_SECAM_L   :    // 50Hz Sound 6.5M  SECAM L  
#if(_TV_SECAM_L_SUPPORT)
            return 1;
#endif
            break;
        
        case _TV_SECAM_LL  :    // 50Hz Sound 6.5M  Secam L' 
#if(_TV_SECAM_LL_SUPPORT)
            return 1;
#endif
            break;
    } // end switch 
    
    return 0;
}

//---------------------------------------------------------------------------
void CMISystemRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
            for(pData[15] = 0; pData[15] < (_MAX_TV_TYPE + 1); pData[15]++)
            {
                ucTVType = ValueInRangeChange(0, _MAX_TV_TYPE, ucTVType, _LOOP | ucMode);
                
                if (bSearchedTVType(ucTVType))
                    break;
            }
            
			CClearAdjustMenu();
			CCenterTextout(sTVType[ucTVType],ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
            CSaveChannelColorType(stTvInfo.CurChn, ucTVType);
            gmi_CI2CWriteIfPllDM(ucTVType, _TUNER_MUTE_OFF, _NORMAL_MODE);
            CSetTvColor(ucTVType);
            CLR_CLEAR_OSD_EN();
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------Channel             ----------------------------
void CMIDrawChannel(BYTE ucDrawID)
{
	BYTE ucRow = _MI_CHANNEL + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_CHANNEL);
#if(_FM_DEVICE)
            if (1 == bFM)
                CShowNumber(_SHOW_NUM_POS_X, _SHOW_NUM_POS_Y, stTvInfo.ucFMCurrCh);
            else
#endif
            CShowNumber(_SHOW_NUM_POS_X, _SHOW_NUM_POS_Y, stTvInfo.CurChn);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sChannel[GET_LANGUAGE()],COL(5),ucRow);
			//CClearAdjustMenu();
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_CHANNEL);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIChannelRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
            CMuteOn();

#if(_FM_DEVICE)
            if (1 == bFM)
            {
                stTvInfo.ucFMCurrCh = ValueInRangeChange(0, CloadMaxChannelNumber(), stTvInfo.ucFMCurrCh, _LOOP | ucMode);
                CSetTVChannel(stTvInfo.ucFMCurrCh);
    			CClearAdjustMenu();
                CShowNumber(_SHOW_NUM_POS_X, _SHOW_NUM_POS_Y, stTvInfo.ucFMCurrCh);
                ucPrevChannel =  stTvInfo.ucFMCurrCh;
                CSetVolume();
            }
            else
#endif
            {
                CModeResetTVMode();
                stTvInfo.CurChn = ValueInRangeChange(0, CloadMaxChannelNumber(), stTvInfo.CurChn, _LOOP | ucMode);
                CSetTVChannel(stTvInfo.CurChn);
    			CClearAdjustMenu();
                CShowNumber(_SHOW_NUM_POS_X, _SHOW_NUM_POS_Y, stTvInfo.CurChn);
                ucPrevChannel =  stTvInfo.CurChn;
            }
            bChangeChannel = 0;
            ucOsdEventMsg  = _SAVE_EE_TV_DATA_MSG;
            CLR_CLEAR_OSD_EN();
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------AutoSearch          ----------------------------
void CMIDrawAutoSearch(BYTE ucDrawID)
{
	BYTE ucRow = _MI_AUTOSEARCH + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_AUTOSEARCH);
            CShowCurrentChannelFreq();
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sAutoSearch[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_AUTOSEARCH);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIAutoSearchRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC: 
			CTvAutoSearch();
			CClearAdjustMenu();
            break;

        case _MENU_RUN:            
            break;
    }
}

//-----------------------------------------------------------------------
bit CKeyStopAutoSearch(void)
{
   if (_MENU_KEY_MASK == CKeyScan() || _MENU_KEY_MESSAGE == CIRKeyScan())
      return 1;

   return 0;
}

//-----------------------------------------------------------------------
void CShowAutoSerachTotal(BYTE ucSearchTotal)
{
    CShowNumber(2, _SHOW_NUM_POS_Y, ucSearchTotal);
}

//-----------------------------------------------------------------------
void CShowFreq(WORD iFreqN)
{
    DWORD lFreqTemp = 0;

    // Get PIF freq
#if(_IF_PLL_DE_CHIP == _IF_PLL_DE_1338)  
    if (bFM)
        lFreqTemp = (((float)iFreqN/_FM_TUNER_BP) - ((float)_PIF_FREQ/1000))*100;
    else
#endif
        lFreqTemp = (((float)iFreqN/_TUNER_BP) - ((float)_PIF_FREQ/1000))*100;

    iFreqN = lFreqTemp/100;  // MHz  
    Gotoxy(8, _SHOW_NUM_POS_Y, BYTE_DISPLAY);
    CShowNumber1(iFreqN, 0);
    OutputChar(0x2E); // "."

    iFreqN = lFreqTemp%100;  // KHz
    CShowNumber1(iFreqN, 2);

    OutputChar('M');
    OutputChar('h');
    OutputChar('z');
    OutputChar(' ');
}     

//-----------------------------------------------------------------------
void CShowCurrentChannelFreq(void)
{
    WORD iFreq = 0;

    iFreq = CLoadChannelFreq(stTvInfo.CurChn);
    CShowFreq(iFreq);
}
    
//-----------------------------------------------------------------------
void CShowAutoSearchSliderInOSD(WORD ucCurrentValue)
{
   // OSDClear(_SHOW_NUM_POS_Y, 1, 10, 8, 0x00, BYTE_DISPLAY);
   // OSDClear(_SHOW_NUM_POS_Y, 1, 10, 8, 0x8c, BYTE_ATTRIB);
    CShowFreq(ucCurrentValue);
}       
//---------------------------------------------------------------------------


//---------------------------ManualSearch        ----------------------------
void CMIDrawManualSearch(BYTE ucDrawID)
{
	BYTE ucRow = _MI_MANUALSEARCH + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_MANUALSEARCH);
            CShowCurrentChannelFreq();
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sMSearch[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_MANUALSEARCH);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIManualSearchRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
            CManualSearch((bit)ucMode, stTvInfo.CurChn);
            CTimerDelayXms(200);
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------Tuning              ----------------------------
void CMIDrawTuning(BYTE ucDrawID)
{
	BYTE ucRow = _MI_TUNING + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_TUNING);
            CShowCurrentChannelFreq();
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sTuning[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_TUNING);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMITuningRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
            SET_KEYREPEATENABLE();                                              
            CTuningCurrentChannel((bit)ucMode, stTvInfo.CurChn);
            CShowCurrentChannelFreq();
            break; 

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------Skip                ----------------------------
void CMIDrawSkip(BYTE ucDrawID)
{
	BYTE ucRow = _MI_SKIP + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_SKIP);
			CCenterTextout(SOnOff[(bit)CLoadChannelSkip(stTvInfo.CurChn)][GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sSkip[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_SKIP);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMISkipRun(BYTE ucMode)
{
    bit bSkip = CLoadChannelSkip(stTvInfo.CurChn);

    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC: 
            bSkip = ~bSkip;
            CSaveChannelSkip(bSkip, stTvInfo.CurChn);
            CClearAdjustMenu();
			CCenterTextout(SOnOff[bSkip][GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------

#if(_SLEEP_FUNC)
//---------------------------AutoPowerDown       ----------------------------
void CMIDrawAutoPowerDown(BYTE ucDrawID)
{
	BYTE ucRow = _MI_AUTOPOWERDOWN + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_AUTOPOWERDOWN);
            if (0 == _GET_POWER_DOWN_TIME())
			   CCenterTextout(SOnOff[0][GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
            else
               CShowNumber(_SHOW_NUM_POS_X, _SHOW_NUM_POS_Y, _GET_POWER_DOWN_TIME() * 15);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sAutoPowerDown[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_AUTOPOWERDOWN);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIAutoPowerDownRun(BYTE ucMode)
{
    BYTE ucTemp = _GET_POWER_DOWN_TIME();      

    switch(ucMode)
    {
        case _MENU_ADJ_DEC:           
        case _MENU_ADJ_INC:  
            ucTemp = ValueInRangeChange(0, 8, ucTemp, _LOOP | ucMode);
            _SET_POWER_DOWN_TIME(ucTemp);
            CClearAdjustMenu();
            if (ucTemp == 0)
			   CCenterTextout(SOnOff[0][GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
            else
               CShowNumber(_SHOW_NUM_POS_X, _SHOW_NUM_POS_Y, _GET_POWER_DOWN_TIME() * 15);

            ucAutoPowerDownTime = ucTemp * 15;
            if (0 == ucAutoPowerDownTime) // Auto power down time off
            {
                ucAutoPowerDownTime = 0xff;
            }
            ucMinuteCount = 0;  // Reset count

            ucOsdEventMsg = _SAVE_EE_TV_DATA_MSG;
            break;

        case _MENU_RUN:            
            break;
    }
}
#endif
//---------------------------------------------------------------------------


//---------------------------BlueScreen          ----------------------------
void CMIDrawBlueScreen(BYTE ucDrawID)
{
	BYTE ucRow = _MI_BLUESCREEN + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_BLUESCREEN);
			CCenterTextout(SOnOff[(bit)_GET_BLUE_BACKGROUND()][GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sBlueScreen[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_BLUESCREEN);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIBlueScreenRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:
            _SET_BLUE_BACKGROUND((_GET_BLUE_BACKGROUND()) ? 0 : 1);
            CClearAdjustMenu();
			CCenterTextout(SOnOff[_GET_BLUE_BACKGROUND()][GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
            ucOsdEventMsg = _SAVE_EE_SYSTEMDATA_MSG;
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------TVReturn            ----------------------------
void CMIDrawTVReturn(BYTE ucDrawID)
{
	BYTE ucRow = _MI_TVRETURN + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_TVRETURN);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sReturn[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_TVRETURN);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMITVReturnRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
        case _MENU_RUN:            
            CReturnUpMenu();
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------Reset               ----------------------------
void CMIDrawReset(BYTE ucDrawID)
{
	BYTE ucRow = _MI_RESET + 5;
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_RESET);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sReset[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_RESET);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIResetRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:  
            CDoReset();  
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------

#if(_DISPLAY_RATIO)
//---------------------------DisplayRatio        ----------------------------
void CDrawDisplayMode(void)
{
    if (_DISPMODE_43 == GET_DISPLAYMODE()) // 4:3
    	CCenterTextout(sDM4_3[GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
    else
    	CCenterTextout(sFull[GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
}

//---------------------------------------------------------------------------
void CMIDrawDisplayRatio(BYTE ucDrawID)
{
	BYTE ucRow = _MI_DISPLAY_RATIO + 5;
	
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_DISPLAY_RATIO);
            CDrawDisplayMode();
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sDisplayRatio[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_DISPLAY_RATIO);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIDisplayRatioRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:
            if (_DISPMODE_43 == GET_DISPLAYMODE())
                SET_DISPLAYMODE(_DISPMODE_FULL);
            else
                SET_DISPLAYMODE(_DISPMODE_43);
            CClearAdjustMenu();
            CDrawDisplayMode();
            CEepromSaveSystemData();
            ucTVSyncFailCount = 250;
            CModeResetMode();
            break;

        case _MENU_RUN:            
            break;
    }
}
#endif
//---------------------------------------------------------------------------

//---------------------------Volume              ----------------------------
void CMIDrawVolume(BYTE ucDrawID)
{
	BYTE ucRow = _MI_VOLUME + 5;
	
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_VOLUME);
	      	DRAW_SLIDER(GET_VOLUME());
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sVolume[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_VOLUME);
			//CClearAdjustMenu();
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIVolumeRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
            CLR_AUDIO_MUTE();
            SET_KEYREPEATENABLE();
            stAudioData.Volume = ValueInRangeChange(0, 100, stAudioData.Volume, _NON_LOOP | ucMode);
            DRAW_SLIDER(GET_VOLUME());
            CSetVolume();
            ucOsdEventMsg = _SAVE_EE_AUDIO_DATA_MSG;
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------Mute                ----------------------------
void CMIDrawMute(BYTE ucDrawID)
{
	BYTE ucRow = _MI_MUTE + 5;
	
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_MUTE);
			CCenterTextout(SOnOff[(bit)GET_AUDIO_MUTE()][GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
			break; 

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sMute[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_MUTE);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIMuteRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC: 
            if (GET_AUDIO_MUTE())
                CLR_AUDIO_MUTE();
            else
                SET_AUDIO_MUTE();
            CClearAdjustMenu();
			CCenterTextout(SOnOff[(bit)GET_AUDIO_MUTE()][GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
            CSetVolume();
            break;

        case _MENU_RUN:            
            break;
    }
}
//---------------------------------------------------------------------------


#if (_SOUND_PROCESSOR)
//---------------------------Balance             ----------------------------
void CMIDrawBalance(BYTE ucDrawID)
{
	BYTE ucRow = _MI_BALANCE + 5;
	
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_BALANCE);
	      	DRAW_SLIDER(stAudioData.Balance);
			break;
		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sBalance[GET_LANGUAGE()],COL(5),ucRow);
			break;
		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_BALANCE);
			break;
		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIBalanceRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:
        case _MENU_ADJ_INC:
            CLR_AUDIO_MUTE();
            SET_KEYREPEATENABLE();
            stAudioData.Balance = ValueInRangeChange(0, 100, stAudioData.Balance, _NON_LOOP | ucMode);
            DRAW_SLIDER(stAudioData.Balance);
            CSetAudioProcessor(stAudioData.Balance, stAudioData.Bass, stAudioData.Treble);
            ucOsdEventMsg = _SAVE_EE_AUDIO_DATA_MSG;
            break;

        case _MENU_RUN:
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------Bass                ----------------------------
void CMIDrawBass(BYTE ucDrawID)
{
	BYTE ucRow = _MI_BASS + 5;
	
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_BASS);
	      	DRAW_SLIDER(stAudioData.Bass);
			break;
		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sBass[GET_LANGUAGE()],COL(5),ucRow);
			break;
		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_BASS);
			break;
		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIBassRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:
        case _MENU_ADJ_INC:
            SET_KEYREPEATENABLE();
            stAudioData.Bass = ValueInRangeChange(0, 100, stAudioData.Bass, _NON_LOOP | ucMode);
            DRAW_SLIDER(stAudioData.Bass);
            CSetAudioProcessor(stAudioData.Balance, stAudioData.Bass, stAudioData.Treble);
            ucOsdEventMsg = _SAVE_EE_AUDIO_DATA_MSG;
            break; 

        case _MENU_RUN:
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------Treble              ----------------------------
void CMIDrawTreble(BYTE ucDrawID)
{
	BYTE ucRow = _MI_TREBLE + 5;
	
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_TREBLE);
	      	DRAW_SLIDER(stAudioData.Treble);
			break;
		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sTreble[GET_LANGUAGE()],COL(5),ucRow);
			break;
		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_TREBLE);
			break;
		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMITrebleRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:
        case _MENU_ADJ_INC:
            SET_KEYREPEATENABLE();
            stAudioData.Treble = ValueInRangeChange(0, 100, stAudioData.Treble, _NON_LOOP | ucMode);
            DRAW_SLIDER(stAudioData.Treble);
            CSetAudioProcessor(stAudioData.Balance, stAudioData.Bass, stAudioData.Treble);
            ucOsdEventMsg = _SAVE_EE_AUDIO_DATA_MSG;
            break;

        case _MENU_RUN:
            break;
    }
}
//---------------------------------------------------------------------------


#if(AUDIO_TYPE != _AUDIO_SC7313)
//---------------------------SRS                 ----------------------------
void CMIDrawSRS(BYTE ucDrawID)
{
	BYTE ucRow = _MI_SRS + 5;
	
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_SRS);
			break;
		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sSRS[GET_LANGUAGE()],COL(5),ucRow);
			break;
		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_SRS);
			break;
		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMISRSRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:
        case _MENU_ADJ_INC:
            break;

        case _MENU_RUN:
            break;
    }
}
//---------------------------------------------------------------------------


//---------------------------BBE                 ----------------------------
void CMIDrawBBE(BYTE ucDrawID)
{
	BYTE ucRow = _MI_BBE + 5;
	
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_BBE);
			break;
		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sBBE[GET_LANGUAGE()],COL(5),ucRow);
			break;
		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_BBE);
			break;
		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIBBERun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:
            
        case _MENU_ADJ_INC:
            
            break;
        case _MENU_RUN:
            
            break;
    }
}
#endif // #if(AUDIO_TYPE != _AUDIO_SC7313)
#endif // #if (_SOUND_PROCESSOR == ON)
//---------------------------------------------------------------------------

//---------------------------FM                ----------------------------
#if(_FM_DEVICE)
void CMIDrawFM(BYTE ucDrawID)
{
	BYTE ucRow = _MI_FM + 5;

	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_FM);
			CCenterTextout(SOnOff[bFM][GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sFM,COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_FM);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}

//---------------------------------------------------------------------------
void CMIFMRun(BYTE ucMode)
{
    bit bSkip = CLoadChannelSkip(stTvInfo.CurChn);

    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC: 
            bFM = ~bFM;
            CClearAdjustMenu();
			CCenterTextout(SOnOff[bFM][GET_LANGUAGE()], ROW(_SHOW_NUM_POS_Y), COL(0), _DIALOG_WIDTH);

            if (_GET_INPUT_SOURCE() != _SOURCE_VIDEO_TV)
            {
                if (bFM) 
                    Draw2bit3x2Icon(tTVIco_Normal, _MI_TV * 4 + 1, ROW(1));
                else
                    Draw2bit3x2Icon(tTVIco_Disable, _MI_TV * 4 + 1, ROW(1));
            }

            CChangeFM();
            break;

        case _MENU_RUN:            
            break;
    }
}
#endif

//---------------------------FuncReturn          ----------------------------
void CMIDrawFuncReturn(BYTE ucDrawID)
{
	BYTE ucRow = _MI_FUNC_RETURN+ 5;
	
	switch(ucDrawID)
	{
		case _DRAW_SELECT:           //选中状态
	      	SubMenuItemSelect(_MI_FUNC_RETURN);
			break;

		case _DRAW_MENU_ITEM:        //画菜单项
			CTextOutEx(sReturn[GET_LANGUAGE()],COL(5),ucRow);
			break;

		case _DRAW_NORMARL:          //标准状态
			SubMenuItemClr(_MI_FUNC_RETURN);
			break;

		case _DRAW_DISABLE:          //禁用状态
			FMI_DISABLE_MENIITEM_LINE(ucRow);
			break;
	}
}
//---------------------------------------------------------------------------
void CMIFuncReturnRun(BYTE ucMode)
{
    switch(ucMode)
    {
        case _MENU_ADJ_DEC:            
        case _MENU_ADJ_INC:            
        case _MENU_RUN:            
            CReturnUpMenu();
            break;
    }
}
//---------------------------------------------------------------------------

#endif		//#if(_OSD_TYPE == _OSD002)
