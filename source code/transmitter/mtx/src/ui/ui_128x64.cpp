
#include "Arduino.h"

#include "../mtx.h"
#include "../templates.h"
#include "../tonePlayer.h"
#include "../sd/modelStrings.h"
#include "bitmaps.h"
#include "ui.h"
#include "uiCommon.h"

#if defined (UI_128X64)

#if defined (DISPLAY_KS0108)
#include "../lcd/GFX.h"
#include "../lcd/LCDKS0108.h"
LCDKS0108 display = LCDKS0108(PIN_KS_RS, PIN_KS_EN, PIN_KS_CS1, PIN_KS_CS2);
#endif

//------ Menu strings --------
// Max 19 characters per string

char const mainMenu[][20] PROGMEM = { 
  "Model", "Inputs", "Mixer", "Outputs", "Extras", "Telemetry", "System", "Receiver", "About"
};
enum {
  MAIN_MENU_MODEL, MAIN_MENU_INPUTS, MAIN_MENU_MIXER, MAIN_MENU_OUTPUTS, MAIN_MENU_EXTRAS,
  MAIN_MENU_TELEMETRY, MAIN_MENU_SYSTEM, MAIN_MENU_RECEIVER, MAIN_MENU_ABOUT
};

char const extrasMenu[][20] PROGMEM = { 
  "Custom curves", "Functn generators", "Logical switches", "Counters", "Timers", "Safety checks",
  "Notifications", "Trim setup", "Flight modes"
};
enum {
  EXTRAS_MENU_CUSTOM_CURVES, EXTRAS_MENU_FUNCTION_GENERATORS, EXTRAS_MENU_LOGICAL_SWITCHES,
  EXTRAS_MENU_COUNTERS, EXTRAS_MENU_TIMERS, EXTRAS_MENU_SAFETY_CHECKS, EXTRAS_MENU_NOTIFICATION_SETUP, 
  EXTRAS_MENU_TRIM_SETUP, EXTRAS_MENU_FLIGHT_MODES
};

char const systemMenu[][20] PROGMEM = { 
  "RF setup", "Sound", "Backlight", "Appearance", "Advanced"
};
enum {
  SYSTEM_MENU_RF, SYSTEM_MENU_SOUND, SYSTEM_MENU_BACKLIGHT, SYSTEM_MENU_APPEARANCE, SYSTEM_MENU_ADVANCED
};

char const advancedMenu[][20] PROGMEM = { 
  "Sticks", "Switches", "Battery", "Security", "Miscellaneous", "Debug"
};
enum {
  ADVANCED_MENU_STICKS, ADVANCED_MENU_SWITCHES, ADVANCED_MENU_BATTERY, ADVANCED_MENU_SECURITY,
  ADVANCED_MENU_MISC, ADVANCED_MENU_DEBUG
};

//----------------------------

//RF power level strings. Max 9 chars per string
char const rfPowerStr[][10] PROGMEM = {  
  "Low", "Medium", "Max"
};

//Backlight timeout strings. Max 9 chars per string
char const backlightTimeoutStr[][10] PROGMEM = { 
  "5s", "15s", "30s", "1min", "5min", "15min", "Never"
};

//Backlight timeout strings. Max 9 chars per string
char const backlightWakeupStr[][10] PROGMEM = { 
  "Keys", "Actvty"
};

//---------------------------- Main UI states ------------------------------------------------------

enum {
  //--- Home screen and related
  SCREEN_HOME,
  SCREEN_CHANNEL_MONITOR,
  CONTEXT_MENU_HOME_SCREEN,
  SCREEN_WIDGET_SETUP,
  CONTEXT_MENU_WIDGETS,
  DIALOG_COPY_WIDGET,
  DIALOG_MOVE_WIDGET,
  
  //---- Main menu ----
  SCREEN_UNLOCK_MAIN_MENU,
  SCREEN_MAIN_MENU,
  
  //---- Model ----
  SCREEN_UNLOCK_MODEL_MANAGER,
  SCREEN_MODEL,
  CONTEXT_MENU_ACTIVE_MODEL,
  CONTEXT_MENU_INACTIVE_MODEL,
  CONTEXT_MENU_FREE_MODEL,
  DIALOG_MODEL_TYPE,
  DIALOG_RENAME_MODEL,
  DIALOG_COPYFROM_MODEL,
  CONFIRMATION_MODEL_BACKUP,
  DIALOG_RESTORE_MODEL,
  CONFIRMATION_MODEL_RESTORE,
  CONFIRMATION_MODEL_COPY,
  CONFIRMATION_MODEL_DELETE,
  CONFIRMATION_MODEL_RESET,
  
  //---- Inputs ----
  SCREEN_INPUTS,
  
  //---- Mixer ----
  SCREEN_MIXER,
  DIALOG_MIX_FLIGHT_MODE,
  CONTEXT_MENU_MIXER,
  SCREEN_MIXER_OUTPUT,
  DIALOG_COPY_MIX,
  DIALOG_MOVE_MIX,
  DIALOG_RENAME_MIX,
  CONFIRMATION_MIXES_RESET,
  CONTEXT_MENU_MIXER_TEMPLATES,
  CONFIRMATION_LOAD_MIXER_TEMPLATE,
  SCREEN_MIXER_OVERVIEW,
  
  //---- Outputs ----
  SCREEN_OUTPUTS,
  CONTEXT_MENU_OUTPUTS,
  DIALOG_RENAME_CHANNEL,
  
  //---- Extras ---
  SCREEN_EXTRAS_MENU,
  //custom curves
  SCREEN_CUSTOM_CURVES,
  CONTEXT_MENU_CUSTOM_CURVE,
  DIALOG_RENAME_CUSTOM_CURVE,
  DIALOG_COPY_CUSTOM_CURVE,
  DIALOG_INSERT_CURVE_POINT,
  DIALOG_DELETE_CURVE_POINT,
  //logical switches
  SCREEN_LOGICAL_SWITCHES,
  CONTEXT_MENU_LOGICAL_SWITCHES,
  DIALOG_COPY_LOGICAL_SWITCH,
  SCREEN_LOGICAL_SWITCH_OUTPUTS,
  //counters
  SCREEN_COUNTERS,
  CONTEXT_MENU_COUNTERS,
  DIALOG_RENAME_COUNTER,
  DIALOG_COPY_COUNTER,
  CONFIRMATION_CLEAR_ALL_COUNTERS,
  //flight modes
  SCREEN_FLIGHT_MODES,
  //function generators
  SCREEN_FUNCTION_GENERATORS,
  CONTEXT_MENU_FUNCGEN,
  DIALOG_COPY_FUNCGEN,
  SCREEN_FUNCGEN_OUTPUTS,
  //trim setup
  SCREEN_TRIM_SETUP,
  //safety checks
  SCREEN_SAFETY_CHECKS,
  //timers
  SCREEN_TIMERS,
  CONTEXT_MENU_TIMERS,
  //notifications
  SCREEN_NOTIFICATION_SETUP,
  CONTEXT_MENU_NOTIFICATIONS,
  DIALOG_COPY_NOTIFICATION,

  //---- Telemetry ----
  SCREEN_TELEMETRY,
  CONTEXT_MENU_ACTIVE_SENSOR,
  CONTEXT_MENU_FREE_SENSOR,
  SCREEN_CREATE_SENSOR,
  CONTEXT_MENU_SENSOR_TEMPLATES,
  SCREEN_SENSOR_STATS,
  SCREEN_EDIT_SENSOR,
  CONFIRMATION_DELETE_SENSOR,
  
  //---- System settings ----
  SCREEN_SYSTEM_MENU,
  SCREEN_RF,
  SCREEN_SOUND,
  SCREEN_BACKLIGHT,
  SCREEN_APPEARANCE,
  //advanced system settings
  SCREEN_UNLOCK_ADVANCED_MENU,
  SCREEN_ADVANCED_MENU,
  SCREEN_STICKS,
  SCREEN_SWITCHES,
  SCREEN_BATTERY,
  SCREEN_SECURITY,
  SCREEN_MISC,
  SCREEN_DEBUG,
  SCREEN_INTERNAL_EEPROM_DUMP,
  SCREEN_EXTERNAL_EEPROM_DUMP,
  SCREEN_CHARACTER_SET,
  CONFIRMATION_FACTORY_RESET,
  
  //---- Receiver ----
  SCREEN_RECEIVER,
  SCREEN_RECEIVER_CONFIG,
  SCREEN_RECEIVER_BINDING,
  
  //---- About ----
  SCREEN_ABOUT,
  SCREEN_EASTER_EGG,
  
  //---- Generic text viewer ----
  SCREEN_TEXT_VIEWER,
};

//---------------------------- Misc ----------------------------------------------------------------

char txtBuff[32]; //generic buffer for working with strings. Leave at 32 as it's also reused to dump eeprom

uint8_t theScreen = SCREEN_HOME;
uint8_t lastScreen = SCREEN_HOME;

uint8_t focusedItem = 1; 

bool isEditTextDialog = false;
bool isDisplayingBattWarn = false;

//context menu
uint8_t contextMenuTopItem = 1;
uint8_t contextMenuFocusedItem = 1;
uint8_t contextMenuSelectedItemID = 0xff;

//Model
uint8_t thisModelIdx = 0;

//mixer
uint8_t thisMixIdx = 0;
uint8_t destMixIdx = 0; 

//channel outputs
uint8_t thisChIdx = 0;

//counters
uint8_t thisCounterIdx = 0;
uint8_t destCounterIdx = 0;

//function generators
uint8_t thisFgenIdx = 0;
uint8_t destFgenIdx = 0;

//telemetry 
uint8_t thisTelemIdx = 0;

//logical switch
uint8_t thisLsIdx = 0;
uint8_t destLsIdx = 0;

//custom curves
uint8_t thisCrvIdx = 0;
uint8_t destCrvIdx = 0;
uint8_t thisCrvPt = 0;

//widgets
uint8_t thisWidgetIdx = 0;
uint8_t destWidgetIdx = 0;

//timers
uint8_t thisTimerIdx = 0;

//notifications
uint8_t thisNotificationIdx = 0;
uint8_t destNotificationIdx = 0;

//others 
bool isRequestingStickCalibration = false;
bool batteryGaugeCalibrated = true;
bool isRequestingSwitchesSetup = false;
bool mainMenuLocked = true;
const char* textViewerText;

//---------------------------- Function declarations -----------------------------------------------

void handleBatteryWarnUI();

void changeToScreen(uint8_t scrn);
void changeFocusOnUpDown(uint8_t numItems);
void toggleEditModeOnSelectClicked();
void drawCursor(uint8_t xpos, uint8_t ypos);
void drawHeader(const char* str);
void drawMenu(char const list[][20], uint8_t numItems, const uint8_t *const icons[], uint8_t *topItem, uint8_t *highlightedItem);
void drawDottedVLine(uint8_t x, uint8_t y, uint8_t len, uint8_t fgColor, uint8_t bgColor);
void drawDottedHLine(uint8_t x, uint8_t y, uint8_t len, uint8_t fgColor, uint8_t bgColor);
void drawBoundingBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void drawScrollBar(uint8_t xpos, uint8_t ypos, uint16_t numItems, uint16_t topItem, uint16_t numVisible, uint16_t viewportHeight);
void drawCheckbox(uint8_t xpos, uint8_t ypos, bool val);
void drawLoaderSpinner(uint8_t xpos, uint8_t ypos, uint8_t size);
void drawAnimatedSprite(uint8_t x, uint8_t y, const uint8_t* const bitmapTable[], uint8_t w, uint8_t h, uint8_t color, 
                        uint8_t frameCount, uint16_t frameTime, uint32_t timeOffset);
void drawHorizontalBarChart(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color, int16_t val, int16_t valMin, int16_t valMax);
void drawHorizontalBarChartZeroCentered(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color, int16_t val, int16_t range);
void drawTrimSlider(uint8_t x, uint8_t y, int8_t val, uint8_t range, bool horizontal);
void drawDialogCopyMove(const char* str, uint8_t srcIdx, uint8_t destIdx, bool isCopy);
void drawCustomCurve(custom_curve_t *crv, uint8_t selectPt, uint8_t src);
void drawToast();
void drawTelemetryValue(uint8_t xpos, uint8_t ypos, uint8_t idx, int16_t rawVal, bool blink);
void printTelemParam(uint8_t xpos, uint8_t ypos, uint8_t idx, int32_t val);
void printFullScreenMsg(const char* str);
void printHHMMSS(uint32_t millisecs);
void printVoltage(int16_t millivolts);
void printSeconds(uint16_t decisecs);
void printModelName(char* buff, uint8_t modelIdx);
void printTimerValue(uint8_t idx);
void editTextDialog(const char* title, char* buff, uint8_t lenBuff, bool allowEmpty, bool trimStr, bool isSecureMode);
void validatePassword(uint8_t nextScreen, uint8_t prevScreen);
void contextMenuInitialise();
void contextMenuAddItem(const char* str, uint8_t itemID);
void contextMenuDraw();
uint8_t contextMenuGetItemCount();

//==================================================================================================

void initialiseDisplay()
{
  display.begin();
  display.setTextWrap(false);
  display.clearDisplay();
  display.display();
}

//==================================================================================================

void startInitialSetup()
{
  isRequestingStickCalibration = true;
  batteryGaugeCalibrated = false;
  isRequestingSwitchesSetup = true;
}

//============================ Generic messages ====================================================

void showMsg(const char* str)
{
  display.clearDisplay();
  printFullScreenMsg(str);
  display.setInterlace(false);
  display.display();
}

void showWaitMsg()
{
  display.clearDisplay();
  printFullScreenMsg(PSTR("Please wait"));
  display.setInterlace(false);
  display.display();
}

//============================ Battery warning =====================================================

void handleBatteryWarnUI()
{
  if(!batteryGaugeCalibrated)
    return;
  
  static uint32_t battWarnMillis = millis();
  static bool battWarnDismissed = false;
  
  if(battState == BATTLOW)
  {
    if(!battWarnDismissed)
    {
      //show warning
      display.clearDisplay();
      drawAnimatedSprite(50, 14, animation_low_batt, 27, 11, BLACK, 7, 300, battWarnMillis);
      printFullScreenMsg(PSTR("\nBattery low"));
      display.display();
      
      isDisplayingBattWarn = true;
      
      //play warning tone
      audioToPlay = AUDIO_BATTERY_WARN;
      playTones();
      
      //self dismiss warning or on a button press
      if((pressedButton > 0 || millis() - battWarnMillis > 2000))
      {
        battWarnDismissed = true;
        battWarnMillis = millis();
        isDisplayingBattWarn = false;
        //only kill button events if a button was pressed
        if(pressedButton > 0)
          killButtonEvents();
      }
    }
    //remind low battery every 10 minutes
    if(battWarnDismissed && (millis() - battWarnMillis > 600000UL)) 
    {
      battWarnDismissed = false;
      battWarnMillis = millis();
    }
  }
  else
  {
    isDisplayingBattWarn = false;
    battWarnMillis = millis();
  }
}

//============================ Safety warning ======================================================

void handleSafetyWarnUI()  //blocking function
{
  if(isRequestingSwitchesSetup || isRequestingStickCalibration)
    return;
  
  while(1)
  {
    delay(10);
    
    readSwitchesAndButtons();
    determineButtonEvent();
    readSticks();
    
    handlePowerOff();
    
    checkBattery();
    handleBatteryWarnUI();
    if(isDisplayingBattWarn)
      continue;
    
    inactivityAlarmHandler();
    
    display.clearDisplay();
    
    static bool audioTriggered = false;
    
    //Check throttle position
    bool isThrottleWarn = false;
    if(Model.checkThrottle && (Model.type == MODEL_TYPE_AIRPLANE || Model.type == MODEL_TYPE_MULTICOPTER))
    {
      uint8_t axisType[] = {
        Sys.x1AxisType, Sys.y1AxisType, Sys.x2AxisType, Sys.y2AxisType,
        Sys.x3AxisType, Sys.y3AxisType, Sys.x4AxisType, Sys.y4AxisType,
        STICK_AXIS_NON_CENTERING, STICK_AXIS_NON_CENTERING
      };
      
      int16_t valIn[] = {
        x1AxisIn, y1AxisIn, x2AxisIn, y2AxisIn,
        x3AxisIn, y3AxisIn, x4AxisIn, y4AxisIn,
        knobAIn, knobBIn
      };
      
      uint8_t idx = Model.thrSrcRaw - SRC_RAW_ANALOG_FIRST;
      if(axisType[idx] == STICK_AXIS_SELF_CENTERING && (valIn[idx] > 0 || valIn[idx] < 0 ))
        isThrottleWarn = true;
      if(axisType[idx] == STICK_AXIS_NON_CENTERING && valIn[idx] > - 450)
        isThrottleWarn = true;
    }

    //Check switch positions
    uint8_t numSwitchWarnings = 0;
    for(uint8_t i = 0; i < MAX_NUM_PHYSICAL_SWITCHES; i++)
    {
      if(Model.switchWarn[i] == -1 || Sys.swType[i] == SW_ABSENT) //dont check
        continue;
      int8_t desiredState = Model.switchWarn[i];
      if(swState[i] != desiredState)
      {
        numSwitchWarnings++;
        uint8_t xpos = 6 + ((numSwitchWarnings - 1) % 5) * 24;
        uint8_t ypos = 38 + ((numSwitchWarnings - 1) / 5) * 8;
        if(!isThrottleWarn)
          ypos -= 17;
        display.setCursor(xpos, ypos);
        getSrcName(txtBuff, SRC_SW_PHYSICAL_FIRST + i, sizeof(txtBuff));
        display.print(txtBuff);
      }
    }
    
    //Draw on screen
    if(numSwitchWarnings == 0 && !isThrottleWarn)
    {
      display.clearDisplay();
      audioTriggered = false;
      return;
    }
    else
    {
      display.fillRect(0, 0, 128, 11, BLACK);
      display.drawBitmap(27, 0, icon_warning, 11, 10, WHITE);
      display.setTextColor(WHITE);
      display.setCursor(43, 2);
      display.print(F("WARNING"));
      display.setTextColor(BLACK);

      if(isThrottleWarn)
      {
        display.setCursor(0, 13);
        display.print(F("\x07"));
        display.print(F("Check throttle:"));
        display.setCursor(6, 21);
        getSrcName(txtBuff, Model.thrSrcRaw, sizeof(txtBuff));
        display.print(txtBuff);
      }
      
      if(numSwitchWarnings)
      {
        if(isThrottleWarn)
          display.setCursor(0, 30);
        else
          display.setCursor(0, 13);
        display.print(F("\x07"));
        display.print(F("Check switches:"));
      }
      
      display.setCursor(28, 56);
      display.print(F("[OK] to skip"));
      
      display.display();
      
      if(!audioTriggered)
      {
        audioTriggered = true;
        audioToPlay = AUDIO_SAFETY_WARN;
      }
    }
    
    //skip warning when the Select key is clicked/held
    if(clickedButton == KEY_SELECT || heldButton == KEY_SELECT)
    {
      killButtonEvents();
      display.clearDisplay();
      audioTriggered = false;
      return; 
    }
    
    //call tone player function
    playTones();
  }
}

//============================ Main user interface =================================================

void handleMainUI()
{
  //-------------- Keys filtering for backlight ----------
  if(Sys.backlightSuppressFirstKey && Sys.backlightEnabled)
  {
    static bool suppressDownClicked = false; //flag to kill the clickedButton event on home screen
    if(!backlightIsOn)
    {
      if(buttonCode == KEY_SELECT || buttonCode == KEY_UP)
        killButtonEvents();
      if(buttonCode == KEY_DOWN)
      {
        if(theScreen == SCREEN_HOME) 
        {
          //Only let through the held event so we can mute/unmute telemetry alarms without
          //requiring extra key action.
          pressedButton = 0;
          suppressDownClicked = true; 
        }
        else
          killButtonEvents();
      }
    }
    if(suppressDownClicked && clickedButton == KEY_DOWN)
    {
      suppressDownClicked = false;
      clickedButton = 0;
    }
  }

  //-------------- Timers -------------------------------
  timerHandler();

  //-------------- Inactivity alarm ---------------------
  inactivityAlarmHandler();
  
  //-------------- Telemetry alarms ---------------------
  telemetryAlarmHandler();
  
  //-------------- Battery warning ----------------------
  handleBatteryWarnUI();
  if(isDisplayingBattWarn)
    return;
  
  //-------------- Enable interlace mode by default -----
  //This can be overriden where necessary specially in some screens
  //to prevent interlace artifacts
  display.setInterlace(true); 

  //-------------- Main ui state machine ----------------
  switch (theScreen)
  {
    ////////////////////////////////// HOME ////////////////////////////////////////////////////////
    
    case SCREEN_HOME:
      {
        if(isRequestingStickCalibration)
        {
          changeToScreen(SCREEN_STICKS);
          break; 
        }

        if(isRequestingSwitchesSetup)
        {
          changeToScreen(SCREEN_SWITCHES);
          break;
        }
        
        if(!batteryGaugeCalibrated)
        {
          changeToScreen(SCREEN_BATTERY);
          break;
        }
        
        if(!Sys.lockStartup || isEmptyStr(Sys.password, sizeof(Sys.password)))
          mainMenuLocked = false;
        
        //--------------- Icons -----------------
        
        uint8_t icon_xpos = 127;
        
        //Battery level indicator
        //Gauges don't indicate state of charge; only battery voltage
        if(Sys.useNumericalBatteryIndicator)
        {
          //print right aligned. 
          //Using a hack to measure the width of text first before actually printing.
          display.setCursor(0, 64);
          printVoltage(battVoltsNow);
          uint8_t len = display.getCursorX() / 6;
          icon_xpos = 127 - len * 6;
          display.setCursor(icon_xpos, 0);
          printVoltage(battVoltsNow);
        }
        else
        {
          icon_xpos = 113;
          display.drawRect(icon_xpos, 0, 14, 7, BLACK);
          display.drawVLine(icon_xpos + 14, 2, 3, BLACK);
          if(battState == BATTHEALTY)
          {
            static int8_t lastNumOfBars = 20;
            int8_t numOfBars = 2 + ((int32_t)(battVoltsNow - Sys.battVoltsMin) * 20) / (Sys.battVoltsMax - Sys.battVoltsMin);
            if(numOfBars > 20) 
              numOfBars = 20;
            if(numOfBars > lastNumOfBars && numOfBars - lastNumOfBars < 2) //prevent jitter at boundaries
              numOfBars = lastNumOfBars;
            for(uint8_t i = 0; i < numOfBars/2; i++)
              display.drawVLine(icon_xpos + 2 + i, 2, 3, BLACK);
            lastNumOfBars = numOfBars;
          }
        }
        
        //Draw other icons. Offset the x position by (4 + iconWidth)
        //Rf icon and tx power level as signal strength bars
        if(Sys.rfEnabled)
        {
          icon_xpos -= 16;
          display.drawBitmap(icon_xpos, 0, icon_rf, 7, 7, BLACK);
          uint8_t bars = 2 + (4 * Sys.rfPower) / (RF_POWER_COUNT - 1);
          for(uint8_t i = 0; i < bars; i++)
            display.drawVLine(icon_xpos + 6 + i, 6 - i, i + 1, BLACK);
        }
        
        //Mute icon
        if(!Sys.soundEnabled)
        {
          icon_xpos -= 12;
          display.drawBitmap(icon_xpos, 0, icon_mute, 8, 7, BLACK);
        }
        
        //Lock icon
        if(mainMenuLocked)
        {
          icon_xpos -= 9;
          display.drawBitmap(icon_xpos, 0, icon_padlock, 5, 7, BLACK);
        }          

        //------------ Trims --------------------
        static uint32_t endTime; 
        if(buttonCode >= KEY_TRIM_FIRST && buttonCode <= KEY_TRIM_LAST)
          endTime = millis() + 3000UL;
        if(!Sys.autohideTrims ||(Sys.autohideTrims && millis() < endTime))
        {
          if(Model.X1Trim.trimState != TRIM_DISABLED) 
          {
            int8_t val = Model.X1Trim.trimState == TRIM_FLIGHT_MODE ? Model.FlightMode[activeFmdIdx].x1Trim : Model.X1Trim.commonTrim;
            drawTrimSlider(14, 60, val, 40, true);
          }
          if(Model.Y1Trim.trimState != TRIM_DISABLED)
          {
            int8_t val = Model.Y1Trim.trimState == TRIM_FLIGHT_MODE ? Model.FlightMode[activeFmdIdx].y1Trim : Model.Y1Trim.commonTrim;
            drawTrimSlider(3, 15, val, 40, false);
          }
          if(Model.X2Trim.trimState != TRIM_DISABLED)
          {
            int8_t val = Model.X2Trim.trimState == TRIM_FLIGHT_MODE ? Model.FlightMode[activeFmdIdx].x2Trim : Model.X2Trim.commonTrim;
            drawTrimSlider(73, 60, val, 40, true);
          }
          if(Model.Y2Trim.trimState != TRIM_DISABLED)
          {
            int8_t val = Model.Y2Trim.trimState == TRIM_FLIGHT_MODE ? Model.FlightMode[activeFmdIdx].y2Trim : Model.Y2Trim.commonTrim;
            drawTrimSlider(124, 15, val, 40, false);
          }
        }
        
        //------------ Model name, flight modes -------------
        //Model name
        display.setCursor(14, 9);
        printModelName(Model.name, Sys.activeModelIdx);

        //Flightmode
        //Determine if flight modes are defined other than the default. 
        //If any exist, show the active flight mode. Otherwise hide it, as user does not care.
        bool hasDefinedFlightModes = false;
        for(uint8_t i = 1; i < NUM_FLIGHT_MODES; i++)
        {
          if(Model.FlightMode[i].swtch != CTRL_SW_NONE)
          {
            hasDefinedFlightModes = true;
            break;
          }
        }
        if(hasDefinedFlightModes)
        {
          if(isEmptyStr(Model.FlightMode[activeFmdIdx].name, sizeof(Model.FlightMode[0].name)))
          {
            strlcpy_P(txtBuff, PSTR("FMD"), sizeof(txtBuff));
            char suffix[5];
            memset(suffix, 0, sizeof(suffix));
            itoa(activeFmdIdx + 1, suffix, 10);
            strlcat(txtBuff, suffix, sizeof(txtBuff));
          }
          else
            strlcpy(txtBuff, Model.FlightMode[activeFmdIdx].name, sizeof(txtBuff));
          //print right aligned
          uint8_t len = strlen(txtBuff) + 2; //add 2 for brackets 
          display.setCursor(115 - len * 6, 9);
          display.print(F("("));
          display.print(txtBuff);
          display.print(F(")"));
        }
        
        //separator line
        display.drawHLine(14, 18, 100, BLACK);
        
        //------------ Widgets ------------------
        
        //get available telemetry sensors into a list
        uint8_t tlmQQ[NUM_WIDGETS];
        uint8_t tlmCount = 0;
        for(uint8_t i = 0; i < NUM_CUSTOM_TELEMETRY; i++)
        {
          if(!isEmptyStr(Model.Telemetry[i].name, sizeof(Model.Telemetry[0].name))) 
          {
            //skip sensors that are not set to show on home
            if(!Model.Telemetry[i].showOnHome)
              continue;
            //skip "no data" sensors
            if(telemetryReceivedValue[i] == TELEMETRY_NO_DATA) 
              continue;
            //check if already assigned to a widget
            bool skip = false;
            for(uint8_t j = 0; j < NUM_WIDGETS; j++)
            {
              if(Model.Widget[j].type == WIDGET_TYPE_TELEMETRY && Model.Widget[j].src == i)
                skip = true;
            }
            if(skip)
              continue;
            
            tlmQQ[tlmCount] = i;
            tlmCount++;
            if(tlmCount == sizeof(tlmQQ))
              break;
          }
        }
        
        //draw the widgets
        uint8_t ypos = 21;
        uint8_t printCount = 0;
        uint8_t tlmCntr = 0;
        for(uint8_t i = 0; i < NUM_WIDGETS; i++)
        {
          widget_params_t *widget = &Model.Widget[i];
          bool hasPrinted = false;
          if(widget->type == WIDGET_TYPE_TELEMETRY)
          {
            uint8_t idx = widget->src;
            if(widget->src == WIDGET_SRC_AUTO)
            { 
              if(tlmCount == 0 || tlmCntr >= tlmCount)
                continue;
              idx = tlmQQ[tlmCntr];
              tlmCntr++;
            }
            uint8_t xpos = 14;
            if(telemetryMuteAlarms && Model.Telemetry[idx].alarmCondition != TELEMETRY_ALARM_CONDITION_NONE)
            {
              display.drawBitmap(12, ypos + 1, icon_mute_tiny, 6, 5, BLACK);
              xpos = 20;
            }
            display.setCursor(xpos, ypos);
            display.print(Model.Telemetry[idx].name);
            display.print(F(":"));
            if(widget->disp == WIDGET_DISP_NUMERICAL || telemetryReceivedValue[idx] == TELEMETRY_NO_DATA)
              drawTelemetryValue(73, ypos, idx, telemetryReceivedValue[idx], true);
            else if(widget->disp == WIDGET_DISP_GAUGE)
            {
              bool show = true;
              if(telemetryAlarmState[idx] && (millis() % 1000 > 700))
                show = false;
              if(show)
              {
                int32_t tVal = ((int32_t) telemetryReceivedValue[idx] * Model.Telemetry[idx].multiplier) / 100;
                tVal += Model.Telemetry[idx].offset;
                drawHorizontalBarChart(73, ypos+1, 41, 4, BLACK, tVal, widget->gaugeMin, widget->gaugeMax);
              }
            }
            hasPrinted = true;
          }
          else if(widget->type == WIDGET_TYPE_MIXSOURCES)
          {
            if(widget->src == SRC_NONE)
              continue;
            display.setCursor(14, ypos);
            getSrcName(txtBuff, widget->src, sizeof(txtBuff));
            display.print(txtBuff);
            display.print(F(":"));
            display.setCursor(73, ypos);
            if(widget->disp == WIDGET_DISP_NUMERICAL)
              display.print(mixSources[widget->src]/5);
            else if(widget->disp == WIDGET_DISP_GAUGE)
              drawHorizontalBarChart(73, ypos+1, 41, 4, BLACK, mixSources[widget->src]/5, widget->gaugeMin, widget->gaugeMax);
            else if(widget->disp == WIDGET_DISP_GAUGE_ZERO_CENTERED)
              drawHorizontalBarChartZeroCentered(74, ypos+2, 39, 3, BLACK, mixSources[widget->src]/5, widget->gaugeMax - widget->gaugeMin);
            hasPrinted = true;
          }
          else if(widget->type == WIDGET_TYPE_OUTPUTS)
          {
            display.setCursor(14, ypos);
            if(!isEmptyStr(Model.Channel[widget->src].name, sizeof(Model.Channel[0].name)))
              display.print(Model.Channel[widget->src].name);
            else
            {
              display.print(F("Ch"));
              display.print(widget->src + 1);
              display.print(F(" out"));
            }
            display.print(F(":"));
            display.setCursor(73, ypos);
            if(widget->disp == WIDGET_DISP_NUMERICAL)
              display.print(channelOut[widget->src]/5);
            else if(widget->disp == WIDGET_DISP_GAUGE)
              drawHorizontalBarChart(73, ypos+1, 41, 4, BLACK, channelOut[widget->src]/5, widget->gaugeMin, widget->gaugeMax);
            else if(widget->disp == WIDGET_DISP_GAUGE_ZERO_CENTERED)
              drawHorizontalBarChartZeroCentered(74, ypos+2, 39, 3, BLACK, channelOut[widget->src]/5, widget->gaugeMax - widget->gaugeMin);
            hasPrinted = true;
          }
          else if(widget->type == WIDGET_TYPE_TIMERS)
          {
            display.setCursor(14, ypos);
            printTimerValue(widget->src);
            //print timer name
            display.setCursor(display.getCursorX() + 6, ypos);
            display.print(Model.Timer[widget->src].name);
            hasPrinted = true;
          }
          else if(widget->type == WIDGET_TYPE_COUNTERS)
          {
            display.setCursor(14, ypos);
            if(!isEmptyStr(Model.Counter[widget->src].name, sizeof(Model.Counter[0].name)))
              display.print(Model.Counter[widget->src].name);
            else
            {
              display.print(F("Counter"));
              display.print(widget->src + 1);
            }
            display.print(F(":"));
            display.setCursor(73, ypos);
            display.print(counterOut[widget->src]);
            hasPrinted = true;
          }

          if(hasPrinted)
          {
            printCount++;
            ypos += 9;
          }
          //check if no more space on the screen
          if(printCount == 4)
            break;
        }
        
        //-------------- handle keys ------------
        
        if(clickedButton == KEY_SELECT)
        {
          if(mainMenuLocked) changeToScreen(SCREEN_UNLOCK_MAIN_MENU);
          else changeToScreen(SCREEN_MAIN_MENU);
        }
        else if(clickedButton == KEY_UP)
          changeToScreen(SCREEN_CHANNEL_MONITOR);
        else if(clickedButton == KEY_DOWN)
          changeToScreen(CONTEXT_MENU_HOME_SCREEN);
        
        //mute audible telemetry alarms
        if(heldButton == KEY_DOWN && millis() - buttonStartTime > 1000UL)
        {
          killButtonEvents();
          telemetryMuteAlarms = !telemetryMuteAlarms;
          audioToPlay = AUDIO_TELEM_MUTE_CHANGED;
          makeToast(telemetryMuteAlarms ? PSTR("Telemetry muted") : PSTR("Telemetry unmuted") , 2000, 0);
        }
      }
      break;
      
    case SCREEN_CHANNEL_MONITOR:
      {
        // display.setInterlace(false); 
        
        drawHeader(PSTR("Outputs"));
        
        //scrollable 
        uint8_t numPages = (NUM_RC_CHANNELS + 9) / 10;
        static uint8_t thisPage = 1;
        
        static bool viewInitialised = false;
        if(!viewInitialised) 
        {
          if(lastScreen == CONTEXT_MENU_OUTPUTS) //start in the page that has the channel we want to view
            thisPage = (thisChIdx + 10) / 10;
          viewInitialised = true;
        }

        isEditMode = true;
        thisPage = incDec(thisPage, numPages, 1, INCDEC_WRAP, INCDEC_SLOW);
        
        uint8_t startIdx = (thisPage - 1) * 10;
        for(uint8_t i = startIdx; i < startIdx + 10 && i < NUM_RC_CHANNELS; i++)
        {
          if((i - startIdx) < 5)
          {
            if(lastScreen == CONTEXT_MENU_OUTPUTS && i == thisChIdx)
            {
              display.setCursor(0, 10 + (i - startIdx) * 11);
              display.write(0xB1);
            }
            display.setCursor(6, 10 + (i - startIdx) * 11);
          }
          else
          { 
            if(lastScreen == CONTEXT_MENU_OUTPUTS && i == thisChIdx)
            {
              display.setCursor(65, 10 + (i - (startIdx + 5)) * 11);
              display.write(0xB1);
            }        
            display.setCursor(71, 10 + (i - (startIdx + 5)) * 11);
          }
          display.print(F("Ch"));          
          display.print(1 + i);  
          display.print(F(":"));
          if(i < 9)
            display.print(F(" "));
          display.print(channelOut[i] / 5);
        }
        //show scrollbar
        drawScrollBar(127, 9, numPages, thisPage, 1, 1 * 54);

        if(heldButton == KEY_SELECT)
        {
          viewInitialised = false;
          if(lastScreen == CONTEXT_MENU_OUTPUTS)
            changeToScreen(SCREEN_OUTPUTS);
          else
            changeToScreen(SCREEN_HOME);
        }
      }
      break;
      
    case CONTEXT_MENU_HOME_SCREEN:
      {
        enum {
          ITEM_SETUP_WIDGETS,
          ITEM_START_STOP_TIMER_FIRST,
          ITEM_START_STOP_TIMER_LAST = ITEM_START_STOP_TIMER_FIRST + NUM_TIMERS - 1,
          ITEM_RESET_TIMER_FIRST,
          ITEM_RESET_TIMER_LAST = ITEM_RESET_TIMER_FIRST + NUM_TIMERS - 1,
        };
        
        static char const startTimerStr[][20] PROGMEM = {"Start timer 1", "Start timer 2", "Start timer 3"};
        static char const stopTimerStr[][20] PROGMEM = {"Stop timer 1", "Stop timer 2", "Stop timer 3"};
        static char const resetTimerStr[][20] PROGMEM = {"Reset timer 1", "Reset timer 2", "Reset timer 3"};

        //dynamically add items
        contextMenuInitialise();
        for(uint8_t idx = 0; idx < NUM_TIMERS; idx++)
        {
          //only add a timer if the timer has a widget associated
          for(uint8_t i = 0; i < NUM_WIDGETS; i++)
          {
            if(Model.Widget[i].type == WIDGET_TYPE_TIMERS && Model.Widget[i].src == idx)
            {
              if(Model.Timer[idx].swtch == CTRL_SW_NONE)
              {
                if(timerIsRunning[idx]) 
                  contextMenuAddItem(stopTimerStr[idx], ITEM_START_STOP_TIMER_FIRST + idx);
                else 
                  contextMenuAddItem(startTimerStr[idx], ITEM_START_STOP_TIMER_FIRST + idx);
              }
              contextMenuAddItem(resetTimerStr[idx], ITEM_RESET_TIMER_FIRST + idx);
              break; 
            }
          }
        }
        if(!mainMenuLocked)        
          contextMenuAddItem(PSTR("Setup widgets"), ITEM_SETUP_WIDGETS);
        contextMenuDraw();
        
        if(contextMenuSelectedItemID >= ITEM_START_STOP_TIMER_FIRST && contextMenuSelectedItemID <= ITEM_START_STOP_TIMER_LAST)
        {
          uint8_t idx = contextMenuSelectedItemID - ITEM_START_STOP_TIMER_FIRST;
          timerForceRun[idx] = timerIsRunning[idx] ? false : true;
          changeToScreen(SCREEN_HOME);
        }
        if(contextMenuSelectedItemID >= ITEM_RESET_TIMER_FIRST && contextMenuSelectedItemID <= ITEM_RESET_TIMER_LAST)
        {
          uint8_t idx = contextMenuSelectedItemID - ITEM_RESET_TIMER_FIRST;
          resetTimerRegister(idx);
          changeToScreen(SCREEN_HOME);
        }
        if(contextMenuSelectedItemID == ITEM_SETUP_WIDGETS)
          changeToScreen(SCREEN_WIDGET_SETUP);
          
        if(heldButton == KEY_SELECT || contextMenuGetItemCount() == 0) //exit
          changeToScreen(SCREEN_HOME);
      }
      break;

    case SCREEN_WIDGET_SETUP:
      {
        drawHeader(PSTR("Widgets"));
        
        widget_params_t *widget = &Model.Widget[thisWidgetIdx];
        
        uint8_t numItems = 1; //initial val
        display.setCursor(8, 9);
        display.print(F("Widget"));
        display.print(thisWidgetIdx + 1);
        display.drawHLine(8, 17, display.getCursorX() - 9, BLACK);
        
        //type 
        numItems++;
        display.setCursor(0, 20);
        display.print(F("Type:"));
        display.setCursor(48, 20);
        display.print(findStringInIdStr(enum_WidgetType, widget->type));
        
        //src
        numItems++;
        display.setCursor(0, 29);
        display.print(F("Src:"));
        display.setCursor(48, 29);
        if(widget->type == WIDGET_TYPE_TELEMETRY)
        {
          if(widget->src == WIDGET_SRC_AUTO) 
            display.print(findStringInIdStr(enum_WidgetSource, WIDGET_SRC_AUTO));
          else 
            display.print(Model.Telemetry[widget->src].name);
        }
        else if(widget->type == WIDGET_TYPE_OUTPUTS)
        {
          display.print(F("Ch"));
          display.print(widget->src + 1);
          if(!isEmptyStr(Model.Channel[widget->src].name, sizeof(Model.Channel[0].name)))
          {
            display.print(F(" "));
            display.print(Model.Channel[widget->src].name);
          }
        }
        else if(widget->type == WIDGET_TYPE_MIXSOURCES)
        {
          getSrcName(txtBuff, widget->src, sizeof(txtBuff));
          display.print(txtBuff);
        }
        else if(widget->type == WIDGET_TYPE_COUNTERS)
        {
          if(!isEmptyStr(Model.Counter[widget->src].name, sizeof(Model.Counter[0].name)))
            display.print(Model.Counter[widget->src].name);
          else
          {
            display.print(F("Counter"));
            display.print(widget->src + 1);
          }
        }
        else if(widget->type == WIDGET_TYPE_TIMERS)
        {
          display.print(F("Timer"));
          display.print(widget->src + 1);
          if(!isEmptyStr(Model.Timer[widget->src].name, sizeof(Model.Timer[0].name)))
          {
            display.print(F(" "));
            display.print(Model.Timer[widget->src].name);
          }
        }

        //dynamic printables
        if((widget->type == WIDGET_TYPE_TELEMETRY && widget->src != WIDGET_SRC_AUTO) 
          || widget->type == WIDGET_TYPE_MIXSOURCES || widget->type == WIDGET_TYPE_OUTPUTS)
        {
          //disp
          numItems++;
          display.setCursor(0, 38);
          display.print(F("Disp:"));
          display.setCursor(48, 38);
          display.print(findStringInIdStr(enum_WidgetDisplay, widget->disp));

          //gaugeMin
          if(widget->disp == WIDGET_DISP_GAUGE || widget->disp == WIDGET_DISP_GAUGE_ZERO_CENTERED)
          {
            numItems++;
            display.setCursor(0, 47);
            display.print((widget->disp == WIDGET_DISP_GAUGE_ZERO_CENTERED) ? F("Left:") : F("Min:"));
            display.setCursor(48, 47);
            if(widget->type == WIDGET_TYPE_TELEMETRY)
            {
              printTelemParam(display.getCursorX(), display.getCursorY(), widget->src, widget->gaugeMin);
              display.print(Model.Telemetry[widget->src].unitsName);
            }
            else if(widget->type == WIDGET_TYPE_OUTPUTS || widget->type == WIDGET_TYPE_MIXSOURCES)
              display.print(widget->gaugeMin);
            
            numItems++;
            display.setCursor(0, 56);
            display.print((widget->disp == WIDGET_DISP_GAUGE_ZERO_CENTERED) ? F("Right:") : F("Max:"));
            display.setCursor(48, 56);
            if(widget->type == WIDGET_TYPE_TELEMETRY)
            {
              printTelemParam(display.getCursorX(), display.getCursorY(), widget->src, widget->gaugeMax);
              display.print(Model.Telemetry[widget->src].unitsName);
            }
            else if(widget->type == WIDGET_TYPE_OUTPUTS || widget->type == WIDGET_TYPE_MIXSOURCES)
              display.print(widget->gaugeMax);
          }
        }
        
        //context menu
        uint8_t contextMenu = numItems + 1;
        display.fillRect(120, 0, 8, 7, WHITE);
        display.drawBitmap(120, 0, focusedItem == contextMenu ? icon_context_menu_focused : icon_context_menu, 8, 7, BLACK);
        
        //cursor
        if(focusedItem == 1)
          drawCursor(0, 9);
        else if(focusedItem != contextMenu)
          drawCursor(40, (focusedItem * 9) + 2);
        
        changeFocusOnUpDown(numItems + 1); //1 added for context menu
        toggleEditModeOnSelectClicked();

        //edit items
        if(focusedItem == 1) //change to next widget
          thisWidgetIdx = incDec(thisWidgetIdx, 0, NUM_WIDGETS - 1, INCDEC_WRAP, INCDEC_SLOW);
        else if(focusedItem == 2) //type
        {
          uint8_t prevType = widget->type;
          widget->type = incDec(widget->type, 0, WIDGET_TYPE_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);
          if(widget->type != prevType)
          {
            widget->disp = WIDGET_DISP_NUMERICAL;
            widget->gaugeMin = -100;
            widget->gaugeMax = 100;
            if(widget->type == WIDGET_TYPE_TELEMETRY)
            {
              widget->src = WIDGET_SRC_AUTO;
              widget->gaugeMin = 0;
            }
            if(widget->type == WIDGET_TYPE_MIXSOURCES)
              widget->src = SRC_NONE;
            if(widget->type == WIDGET_TYPE_OUTPUTS 
               || widget->type == WIDGET_TYPE_COUNTERS 
               || widget->type == WIDGET_TYPE_TIMERS)
            {
              widget->src = 0;
            }
          }
        }
        else if(focusedItem == 3) //src
        {
          if(widget->type == WIDGET_TYPE_TELEMETRY)
          {
            //skip empty telemetry (non-extant sensors)
            //use an array to hold extant sensors
            uint8_t srcQQ[NUM_CUSTOM_TELEMETRY + 1]; //1 added for auto
            uint8_t srcCnt = 0;
            for(uint8_t i = 0; i < sizeof(srcQQ) - 1; i++)
            {
              if(isEmptyStr(Model.Telemetry[i].name, sizeof(Model.Telemetry[0].name)))
                continue;
              srcQQ[srcCnt] = i;
              srcCnt++;
            }
            srcQQ[srcCnt] = WIDGET_SRC_AUTO;
            srcCnt++;

            uint8_t idxQQ = 0;
            for(uint8_t i = 0; i < srcCnt; i++) //search for a match
            {
              if(srcQQ[i] == widget->src)
              {
                idxQQ = i;
                break;
              }
            }
            idxQQ = incDec(idxQQ, 0, srcCnt - 1, INCDEC_WRAP, INCDEC_SLOW);
            uint8_t prevSrc = widget->src;
            widget->src = srcQQ[idxQQ];
            if(widget->src != prevSrc)
            {
              widget->gaugeMin = 0;
              widget->gaugeMax = 100;
            }
          }
          else if(widget->type == WIDGET_TYPE_MIXSOURCES)
          {
            //detect moved source
            uint8_t movedSrc = procMovedSource(getMovedSource());
            if(movedSrc != SRC_NONE)
              widget->src = movedSrc;
            //inc dec
            widget->src = incDecSource(widget->src, INCDEC_FLAG_MIX_SRC);
          }
          else if(widget->type == WIDGET_TYPE_OUTPUTS)
            widget->src = incDec(widget->src, 0, NUM_RC_CHANNELS - 1, INCDEC_NOWRAP, INCDEC_SLOW);
          else if(widget->type == WIDGET_TYPE_COUNTERS)
            widget->src = incDec(widget->src, 0, NUM_COUNTERS - 1, INCDEC_NOWRAP, INCDEC_SLOW);
          else if(widget->type == WIDGET_TYPE_TIMERS)
            widget->src = incDec(widget->src, 0, NUM_TIMERS - 1, INCDEC_NOWRAP, INCDEC_SLOW);
        }
        else if(focusedItem == 4) //disp
        {
          uint8_t prevDisp =  widget->disp;
          do {
            widget->disp = incDec(widget->disp, 0, WIDGET_DISP_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);
          } while(widget->type == WIDGET_TYPE_TELEMETRY && widget->disp == WIDGET_DISP_GAUGE_ZERO_CENTERED);
          
          if(widget->disp != prevDisp)
          {
            if(widget->disp == WIDGET_DISP_GAUGE_ZERO_CENTERED)
            {
              if(widget->gaugeMin > 0)
                widget->gaugeMin = 0 - widget->gaugeMin;
              widget->gaugeMax = 0 - widget->gaugeMin;
            }
          }
        }
        else if(focusedItem == 5) //gaugeMin
        {
          if(widget->type == WIDGET_TYPE_TELEMETRY)
            widget->gaugeMin = incDec(widget->gaugeMin, -30000, widget->gaugeMax, INCDEC_NOWRAP, INCDEC_FAST);
          else if(widget->type == WIDGET_TYPE_MIXSOURCES || widget->type == WIDGET_TYPE_OUTPUTS)
            widget->gaugeMin = incDec(widget->gaugeMin, -100, widget->gaugeMax, INCDEC_NOWRAP, INCDEC_NORMAL);
          if(widget->disp == WIDGET_DISP_GAUGE_ZERO_CENTERED)
            widget->gaugeMax = 0 - widget->gaugeMin;
        }
        else if(focusedItem == 6) //gaugeMax
        {
          if(widget->type == WIDGET_TYPE_TELEMETRY)
            widget->gaugeMax = incDec(widget->gaugeMax, widget->gaugeMin, 30000, INCDEC_NOWRAP, INCDEC_FAST);
          else if(widget->type == WIDGET_TYPE_MIXSOURCES || widget->type == WIDGET_TYPE_OUTPUTS)
            widget->gaugeMax = incDec(widget->gaugeMax, widget->gaugeMin, 100, INCDEC_NOWRAP, INCDEC_NORMAL);
          if(widget->disp == WIDGET_DISP_GAUGE_ZERO_CENTERED)
            widget->gaugeMin = 0 - widget->gaugeMax;
        }
        
        //open context menu
        if(focusedItem == contextMenu && clickedButton == KEY_SELECT)
          changeToScreen(CONTEXT_MENU_WIDGETS);
        
        //exit
        if(heldButton == KEY_SELECT)
          changeToScreen(SCREEN_HOME);
      }
      break;
      
    case CONTEXT_MENU_WIDGETS:
      {
        enum {
          ITEM_COPY_WIDGET,
          ITEM_MOVE_WIDGET,
          ITEM_RESET_WIDGET,
        };
        
        contextMenuInitialise();
        contextMenuAddItem(PSTR("Copy to"), ITEM_COPY_WIDGET);
        contextMenuAddItem(PSTR("Move to"), ITEM_MOVE_WIDGET);
        contextMenuAddItem(PSTR("Reset widget"), ITEM_RESET_WIDGET);
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_RESET_WIDGET)
        {
          resetWidgetParams(thisWidgetIdx);
          changeToScreen(SCREEN_WIDGET_SETUP);
        }
        if(contextMenuSelectedItemID == ITEM_COPY_WIDGET)
        {
          destWidgetIdx = thisWidgetIdx;
          changeToScreen(DIALOG_COPY_WIDGET);
        }
        if(contextMenuSelectedItemID == ITEM_MOVE_WIDGET)
        {
          destWidgetIdx = thisWidgetIdx;
          changeToScreen(DIALOG_MOVE_WIDGET);
        }
        
        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_WIDGET_SETUP);
      }
      break;
      
    case DIALOG_COPY_WIDGET:
    case DIALOG_MOVE_WIDGET:
      {
        isEditMode = true;
        destWidgetIdx = incDec(destWidgetIdx, 0, NUM_WIDGETS - 1, INCDEC_WRAP, INCDEC_SLOW);
        drawDialogCopyMove(PSTR("Widget"), thisWidgetIdx, destWidgetIdx, theScreen == DIALOG_COPY_WIDGET);
        if(clickedButton == KEY_SELECT)
        {
          if(theScreen == DIALOG_COPY_WIDGET)
            Model.Widget[destWidgetIdx] = Model.Widget[thisWidgetIdx];
          else
          {
            uint8_t newPos = destWidgetIdx;
            uint8_t oldPos = thisWidgetIdx;
            //copy from old position to temp
            widget_params_t Temp_Widget = Model.Widget[oldPos];
            //shift elements
            uint8_t thisPos = oldPos;
            if(newPos < oldPos)
            {
              while(thisPos > newPos)
              {
                Model.Widget[thisPos] = Model.Widget[thisPos - 1];
                thisPos--;
              }
            }
            else if(newPos > oldPos)
            {
              while(thisPos < newPos)
              {
                Model.Widget[thisPos] = Model.Widget[thisPos + 1];
                thisPos++;
              }
            }
            //copy from temp into new position
            Model.Widget[newPos] = Temp_Widget;
          }
          thisWidgetIdx = destWidgetIdx;
          changeToScreen(SCREEN_WIDGET_SETUP);
        }
        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_WIDGET_SETUP);
      }
      break;
      
    ////////////////////////////////// MAIN MENU ///////////////////////////////////////////////////
    
    case SCREEN_UNLOCK_MAIN_MENU:
      {
        validatePassword(SCREEN_MAIN_MENU, SCREEN_HOME);
      }
      break;
    
    case SCREEN_MAIN_MENU:
      {
        drawHeader(PSTR("Main menu"));
        
        static uint8_t topItem = 1, highlightedItem = 1;
        drawMenu(mainMenu, sizeof(mainMenu)/sizeof(mainMenu[0]), mainMenuIcons, &topItem, &highlightedItem);
        
        //handle selection
        if(clickedButton == KEY_SELECT)
        {
          uint8_t selectIdx = highlightedItem - 1;
          if(selectIdx == MAIN_MENU_MODEL) 
          {
            if(Sys.lockModels && !isEmptyStr(Sys.password, sizeof(Sys.password))) 
              changeToScreen(SCREEN_UNLOCK_MODEL_MANAGER);
            else 
              changeToScreen(SCREEN_MODEL);
          }
          if(selectIdx == MAIN_MENU_INPUTS) changeToScreen(SCREEN_INPUTS); 
          if(selectIdx == MAIN_MENU_MIXER)  changeToScreen(SCREEN_MIXER);
          if(selectIdx == MAIN_MENU_OUTPUTS) changeToScreen(SCREEN_OUTPUTS);
          if(selectIdx == MAIN_MENU_EXTRAS) changeToScreen(SCREEN_EXTRAS_MENU);
          if(selectIdx == MAIN_MENU_TELEMETRY) changeToScreen(SCREEN_TELEMETRY);
          if(selectIdx == MAIN_MENU_SYSTEM) changeToScreen(SCREEN_SYSTEM_MENU);
          if(selectIdx == MAIN_MENU_RECEIVER) changeToScreen(SCREEN_RECEIVER);
          if(selectIdx == MAIN_MENU_ABOUT) changeToScreen(SCREEN_ABOUT);
        }
        else if(heldButton == KEY_SELECT)
        {
          if(!Sys.rememberMenuPosition)
          {
            topItem = 1;
            highlightedItem = 1;
          }
          changeToScreen(SCREEN_HOME);
        }
      }
      break;
      
    ////////////////////////////////// MODEL ///////////////////////////////////////////////////////
      
    case SCREEN_UNLOCK_MODEL_MANAGER:
      {
        validatePassword(SCREEN_MODEL, SCREEN_MAIN_MENU);
      }
      break;
      
    case SCREEN_MODEL:
      {
        drawHeader(mainMenu[MAIN_MENU_MODEL]);
        
        //------ scrollable list of models ----------
        const uint8_t numVisible = 6;
        
        static uint8_t topItem = 1;
        static bool viewInitialised = false;
        static bool modelListNeedsUpdate = true;
        if(!viewInitialised)
        {
          viewInitialised = true;
          modelListNeedsUpdate = true;
          thisModelIdx = Sys.activeModelIdx;
        }
        
        // handle navigation
        focusedItem = thisModelIdx + 1;
        changeFocusOnUpDown(maxNumOfModels);
        uint8_t prevTopItem = topItem;
        if(focusedItem < topItem)
          topItem = focusedItem;
        while(focusedItem >= topItem + numVisible)
          topItem++;
        if(topItem != prevTopItem)
          modelListNeedsUpdate = true;

        // fill list
        //Check at once and only update on scroll or just entered this screen so we 
        //don't have to constantly read all names from eeprom (a somewhat slow process)
        static char mdlStr[numVisible][sizeof(Model.name)]; //string table
        if(modelListNeedsUpdate)
        {
          modelListNeedsUpdate = false;
          for(uint8_t i = 0; i < numVisible && i < maxNumOfModels; i++)
          {
            uint8_t modelIdx = topItem - 1 + i;
            if(eeModelIsFree(modelIdx)) // write 0xFF to indicate its free
              mdlStr[i][0] = 0xFF;
            else //get the name
            {
              eeGetModelName(txtBuff, modelIdx, sizeof(txtBuff));
              strlcpy(&mdlStr[i][0], txtBuff, sizeof(Model.name));
            }
          }
        }
        
        for(uint8_t line = 0; line < numVisible && line < maxNumOfModels; line++)
        {
          uint8_t ypos = 10 + line * 9;
          uint8_t item = topItem + line;
          if(focusedItem == topItem + line) //highlight
          {
            display.fillRect(0, ypos - 1, item < 10 ? 7 : 13, 9, BLACK);
            display.setTextColor(WHITE);
          }
          display.setCursor(1, ypos);
          display.print(item);
          display.setTextColor(BLACK);
          
          uint8_t modelIdx = item - 1;
          display.setCursor(16, ypos);
          if((uint8_t)mdlStr[line][0] != 0xFF)
            printModelName(mdlStr[line], modelIdx);
          if(modelIdx == Sys.activeModelIdx) //indicate it is active
            display.drawBitmap(display.getCursorX() + 4, ypos + 1, icon_checkmark_regular, 7, 5, BLACK);
        }
        
        //scroll bar
        drawScrollBar(127, 9, maxNumOfModels, topItem, numVisible, numVisible * 9);

        //----- end of list ----------------------
        
        if(focusedItem <= maxNumOfModels)
          thisModelIdx = focusedItem - 1;
        
        if(clickedButton == KEY_SELECT)
        {
          if(thisModelIdx == Sys.activeModelIdx)
            changeToScreen(CONTEXT_MENU_ACTIVE_MODEL);
          else if(!eeModelIsFree(thisModelIdx))
            changeToScreen(CONTEXT_MENU_INACTIVE_MODEL);
          else if(eeModelIsFree(thisModelIdx))
            changeToScreen(CONTEXT_MENU_FREE_MODEL);
          //force the list to be updated
          modelListNeedsUpdate = true; 
        }

        if(heldButton == KEY_SELECT)
        {
          viewInitialised = false;//reset view
          eeSaveSysConfig();
          changeToScreen(SCREEN_MAIN_MENU);
        }
      }
      break;
      
    case CONTEXT_MENU_ACTIVE_MODEL:
      {
        enum {
          ITEM_RENAME_MODEL,
          ITEM_DUPLICATE_MODEL,
          ITEM_COPY_FROM,
          ITEM_RESET_MODEL,
          ITEM_BACKUP_MODEL,
          ITEM_RESTORE_MODEL,
        };
        
        contextMenuInitialise();
        contextMenuAddItem(PSTR("Rename model"), ITEM_RENAME_MODEL);
        if(maxNumOfModels > 1)
        {
          contextMenuAddItem(PSTR("Duplicate"), ITEM_DUPLICATE_MODEL);
          contextMenuAddItem(PSTR("Copy from"), ITEM_COPY_FROM);
        }
        contextMenuAddItem(PSTR("Reset model"), ITEM_RESET_MODEL);
        if(sdHasCard())
        {
          contextMenuAddItem(PSTR("Backup to SD"), ITEM_BACKUP_MODEL);
          contextMenuAddItem(PSTR("Restore from SD"), ITEM_RESTORE_MODEL);
        }
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_RENAME_MODEL) 
          changeToScreen(DIALOG_RENAME_MODEL);
        if(contextMenuSelectedItemID == ITEM_DUPLICATE_MODEL)
        {          
          //Find the nearest free slot and copy to it
          uint8_t nearestLeft = 0xff;
          uint8_t nearestRight = 0xff;
          for(uint8_t i = thisModelIdx + 1; i < maxNumOfModels; i++) //search in right direction
          {
            if(eeModelIsFree(i)) 
            {
              nearestRight = i; 
              break;
            }
          }
          for(int8_t i = thisModelIdx - 1; i >= 0; i--) //search in left direction
          {
            if(eeModelIsFree(i))
            {
              nearestLeft = i; 
              break;
            }
          }
          if(nearestLeft == 0xff && nearestRight == 0xff) //no free slots
          {
            makeToast(PSTR("No free slots"), 2000, 0);
            changeToScreen(SCREEN_MODEL);
          }
          else
          {
            uint8_t destination;
            uint8_t distRight = (nearestRight == 0xff) ? 0xff : nearestRight - thisModelIdx;
            uint8_t distLeft = (nearestLeft == 0xff) ? 0xff : thisModelIdx - nearestLeft;
            if(distRight == 0xff)
              destination = nearestLeft;
            else if(distLeft == 0xff)
              destination = nearestRight;
            else
            {
              destination = (distRight <= distLeft) ? nearestRight : nearestLeft;
            }
            //--- copy
            showWaitMsg();
            stopTones();
            //save the active model
            eeSaveModelData(Sys.activeModelIdx);
            //save to this slot and set it active
            eeSaveModelData(destination);
            Sys.activeModelIdx = destination;
            thisModelIdx = destination;
            //exit
            changeToScreen(SCREEN_MODEL);
          }
        }
        if(contextMenuSelectedItemID == ITEM_COPY_FROM)    
          changeToScreen(DIALOG_COPYFROM_MODEL);
        if(contextMenuSelectedItemID == ITEM_RESET_MODEL)  
          changeToScreen(CONFIRMATION_MODEL_RESET);
        if(contextMenuSelectedItemID == ITEM_BACKUP_MODEL)
          changeToScreen(CONFIRMATION_MODEL_BACKUP);
        if(contextMenuSelectedItemID == ITEM_RESTORE_MODEL)
          changeToScreen(DIALOG_RESTORE_MODEL);
        
        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_MODEL);
      }
      break;
      
    case CONTEXT_MENU_INACTIVE_MODEL:
      {
        enum {
          ITEM_LOAD_MODEL,
          ITEM_DELETE_MODEL,
          ITEM_BACKUP_MODEL,
        };
        
        contextMenuInitialise();
        contextMenuAddItem(PSTR("Load model"), ITEM_LOAD_MODEL);
        contextMenuAddItem(PSTR("Delete model"), ITEM_DELETE_MODEL);
        if(sdHasCard())
          contextMenuAddItem(PSTR("Backup to SD"), ITEM_BACKUP_MODEL);
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_LOAD_MODEL) 
        {
          showWaitMsg();
          stopTones();
          //Save the active model before changing to another model
          eeSaveModelData(Sys.activeModelIdx);
          //load model and set it active
          eeReadModelData(thisModelIdx);
          Sys.activeModelIdx = thisModelIdx; 
          //reset other stuff
          resetTimerRegisters();
          resetCounterRegisters();
          Sys.rfEnabled = false;
          //reinitialise the mixer
          reinitialiseMixerCalculations();
          //Safety checks
          handleSafetyWarnUI();
          //restore timers
          restoreTimerRegisters();
          //restore counters
          restoreCounterRegisters();
          //exit
          changeToScreen(SCREEN_MODEL);
        }
        if(contextMenuSelectedItemID == ITEM_DELETE_MODEL)
          changeToScreen(CONFIRMATION_MODEL_DELETE);
        if(contextMenuSelectedItemID == ITEM_BACKUP_MODEL)
          changeToScreen(CONFIRMATION_MODEL_BACKUP);
        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_MODEL);
      }
      break;
      
    case CONTEXT_MENU_FREE_MODEL:
      {
        enum {
          ITEM_NEW_MODEL,
          ITEM_RESTORE_MODEL,
        };
        
        contextMenuInitialise();
        contextMenuAddItem(PSTR("New model"), ITEM_NEW_MODEL);
        if(sdHasCard())
          contextMenuAddItem(PSTR("Restore from SD"), ITEM_RESTORE_MODEL);
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_NEW_MODEL)
          changeToScreen(DIALOG_MODEL_TYPE);
        if(contextMenuSelectedItemID == ITEM_RESTORE_MODEL)
          changeToScreen(DIALOG_RESTORE_MODEL);

        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_MODEL);
      }
      break;
      
    case DIALOG_MODEL_TYPE:
      {
        isEditMode = true;
        
        static uint8_t mdlType;
        static bool initialised = false;
        if(!initialised)
        {
          initialised = true;
          mdlType = Model.type;
        }
        
        mdlType = incDec(mdlType, 0, MODEL_TYPE_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);

        drawBoundingBox(11, 14, 105, 35,BLACK);
        drawCursor(21, 28);
        display.setCursor(17, 18);
        display.print(F("Model type"));
        display.setCursor(29, 28);
        display.print(findStringInIdStr(enum_ModelType, mdlType));
        
        if(clickedButton == KEY_SELECT) //create the model
        {
          showWaitMsg();
          stopTones();
          //Save the current active model first
          eeSaveModelData(Sys.activeModelIdx);
          //reset stuff
          resetTimerRegisters();
          resetCounterRegisters();
          Sys.rfEnabled = false;
          //create model and set it active
          eeCreateModel(thisModelIdx);
          Sys.activeModelIdx = thisModelIdx;
          //write model type
          Model.type = mdlType;
          //load default mixer template
          if(mdlType == MODEL_TYPE_AIRPLANE || mdlType == MODEL_TYPE_MULTICOPTER)
          {
            if(Sys.mixerTemplatesEnabled)
              loadMixerTemplateBasic(0);
          }
          //save 
          eeSaveModelData(Sys.activeModelIdx);
          // changeToScreen(SCREEN_MODEL);
          changeToScreen(DIALOG_RENAME_MODEL);
        }
        
        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_MODEL);
      }
      break;
      
    case DIALOG_RENAME_MODEL:
      {
        isEditTextDialog = true;
        editTextDialog(PSTR("Model name"), Model.name, sizeof(Model.name), true, true, false);
        if(!isEditTextDialog) //exited
        {
          showWaitMsg();
          stopTones();
          eeSaveModelData(Sys.activeModelIdx);
          changeToScreen(SCREEN_MODEL); 
        }
      }
      break;
      
    case DIALOG_COPYFROM_MODEL:
      {
        //change source model
        isEditMode = true;
        do {
          thisModelIdx = incDec(thisModelIdx, 0, maxNumOfModels - 1 , INCDEC_WRAP, INCDEC_SLOW);
        } while(eeModelIsFree(thisModelIdx) || Model.type != eeGetModelType(thisModelIdx));
        
        drawBoundingBox(11, 14, 105, 35,BLACK);
        display.setCursor(17, 18);
        display.print(F("Copy from"));
        display.setCursor(29, 28);
        eeGetModelName(txtBuff, thisModelIdx, sizeof(txtBuff)); 
        printModelName(txtBuff, thisModelIdx);
        drawCursor(21, 28);

        if(clickedButton == KEY_SELECT)
        {
          if(thisModelIdx == Sys.activeModelIdx)
            changeToScreen(SCREEN_MODEL);
          else
            changeToScreen(CONFIRMATION_MODEL_COPY);
        }
        else if(heldButton == KEY_SELECT) //exit
        {
          thisModelIdx = Sys.activeModelIdx;
          changeToScreen(SCREEN_MODEL);
        }
      }
      break;
      
    case DIALOG_RESTORE_MODEL:
      {
        static bool initialised = false;
        static uint16_t mdlCount = 0;
        static uint16_t thisPos = 0; //Better name
        static char nameStr[13]; //buffer to hold name. Only 8.3 names supported
        
        if(!initialised)
        {
          initialised = true;
          //show wait message as getting the count may take some time when there are many models
          showWaitMsg();
          stopTones();
          //find how many models we have backed up on the sd card
          mdlCount = sdGetModelCount();
          thisPos = 0;
          sdGetModelName(nameStr, thisPos, sizeof(nameStr));
        }
        
        if(mdlCount == 0) //no backups exist on the sd card
        {
          makeToast(PSTR("No models on SD"), 2000, 0);
          initialised = false;
          changeToScreen(SCREEN_MODEL);
          break;
        }
          
        drawBoundingBox(11, 14, 105, 35,BLACK);
        display.setCursor(17, 18);
        display.print(F("Restore from SD"));
        display.setCursor(29, 28);
        display.print(nameStr);
        drawCursor(21, 28);
        
        //get the name
        isEditMode = true;
        uint8_t lastPos = thisPos;
        thisPos = incDec(thisPos, 0, mdlCount - 1, INCDEC_NOWRAP, INCDEC_SLOW); //strictly No Wrap here
        if(thisPos != lastPos) //changed, refresh
          sdGetModelName(nameStr, thisPos, sizeof(nameStr));
        
        if(clickedButton == KEY_SELECT)
        {
          initialised = false;
          //Copy name into txtBuff. 
          //Assumption is txtBuff won't be modified by something else. This isn't much of an issue 
          //since we can always retry if the restore fails.         
          strlcpy(txtBuff, nameStr, sizeof(txtBuff));
          changeToScreen(CONFIRMATION_MODEL_RESTORE);
        }

        if(heldButton == KEY_SELECT) //exit
        {
          initialised = false;
          changeToScreen(SCREEN_MODEL);
        }
      }
      break;
      
    case CONFIRMATION_MODEL_RESTORE:
      {
        if(thisModelIdx == Sys.activeModelIdx)
          printFullScreenMsg(PSTR("Model data will be\noverwritten.\nContinue?\n\nYes [Up] \nNo [Down]"));
        if(clickedButton == KEY_UP || thisModelIdx != Sys.activeModelIdx)
        {
          showWaitMsg();
          stopTones();
          //save active model first but only if we aren't restoring to active slot
          //otherwise there is no point in saving first
          if(thisModelIdx != Sys.activeModelIdx)
            eeSaveModelData(Sys.activeModelIdx);
          //reset stuff
          resetTimerRegisters();
          resetCounterRegisters();
          Sys.rfEnabled = false;
          //restore the model
          if(sdRestoreModel(txtBuff))
          {
            //set the model as active
            Sys.activeModelIdx = thisModelIdx;
            //also save it to eeprom
            eeSaveModelData(Sys.activeModelIdx);
            //reinitialise the mixer
            reinitialiseMixerCalculations();
            //restore timers
            restoreTimerRegisters();
            //restore counters
            restoreCounterRegisters();
            //exit
            changeToScreen(SCREEN_MODEL);
          }
          else
          {
            //exit
            makeToast(PSTR("Restore failed"), 2000, 0);
            changeToScreen(SCREEN_MODEL);
          }
        }
        else if(clickedButton == KEY_DOWN || heldButton == KEY_SELECT)
          changeToScreen(SCREEN_MODEL);
      }
      break;
      
    case CONFIRMATION_MODEL_BACKUP:
      {
        static bool initialised = false;
        static bool modelBackupExists = false;
        static char nameStr[13]; //buffer to hold model name. Only 8.3 names supported
        
        if(!initialised)
        {
          initialised = true;
          modelBackupExists = false;

          //get the model name into the nameStr buffer
          eeGetModelName(nameStr, thisModelIdx, sizeof(nameStr));
          bool nameEmpty = false;
          if(isEmptyStr(nameStr, sizeof(Model.name))) 
            nameEmpty = true;
          else
          {
            //Force conforming to 8.3 naming rules. ### TODO add some more checks
            
            // Strip leading periods
            uint8_t k = 0;
            while(nameStr[k] == '.') 
            {
              if(!isalnum(nameStr[k]) && nameStr[k] != '_')                  
                memmove(&nameStr[k], &nameStr[k + 1], strlen(nameStr) - k);
              else
                k++;
            }
            
            // Convert name to uppercase and replace invalid characters with '_'
            // Also limit the base name to 8 characters max
            uint8_t len = strlen(nameStr);
            uint8_t i = 0;
            char basename[9];
            for(i = 0; i < len; i++) 
            {
              if(i == 8) 
                break;
              if(nameStr[i] == ' ' || nameStr[i] == '/') 
                basename[i] = '_';
              else 
                basename[i] = toupper(nameStr[i]);
            }
            basename[i] = '\0';
            
            //check again if empty
            if(i == 0) 
              nameEmpty = true;
            else if(basename[i - 1] == '.') 
            {
              // If the basename ends with a period, remove it
              i--;
              basename[i] = '\0';
            }
            
            //copy back to nameStr
            strlcpy(nameStr, basename, sizeof(nameStr));
          }
          
          if(nameEmpty) //make a generic name
          {
            strlcpy_P(nameStr, PSTR("MODEL"), sizeof(nameStr));
            char suffix[5];
            memset(suffix, 0, sizeof(suffix));
            itoa(thisModelIdx + 1, suffix, 10);
            strlcat(nameStr, suffix, sizeof(nameStr));
          }
          
          //append file extension
          // strlcat(nameStr, ".MDL", sizeof(nameStr)); //is this really necessary??
          
          //show wait message as checking may take some time when there are many models
          showWaitMsg();
          stopTones();
          
          //check if the name already exists
          if(sdSimilarModelExists(nameStr))
            modelBackupExists = true;
        }
        
        if(modelBackupExists)
          printFullScreenMsg(PSTR("A backup with\na similar name\nexists.\nOverwrite it?\n\nYes [Up] \nNo [Down]"));
        if(clickedButton == KEY_UP || !modelBackupExists)
        {
          showWaitMsg();
          stopTones();
          //save the active model first
          eeSaveModelData(Sys.activeModelIdx);
          //read into ram the model we want to back up to sd card
          eeReadModelData(thisModelIdx);
          //back up to the sd card
          if(!sdBackupModel(nameStr))
            makeToast(PSTR("Backup failed"), 2000, 0);
          //now read back the active model from eeprom
          eeReadModelData(Sys.activeModelIdx);
          //exit
          initialised = false;
          changeToScreen(SCREEN_MODEL);
        }
        else if(clickedButton == KEY_DOWN || heldButton == KEY_SELECT)
        {
          initialised = false;
          changeToScreen(SCREEN_MODEL);
        }
      }
      break;
      
    case CONFIRMATION_MODEL_COPY:
      {
        printFullScreenMsg(PSTR("Model data will be\noverwritten.\nContinue?\n\nYes [Up] \nNo [Down]"));
        if(clickedButton == KEY_UP)
        {
          showWaitMsg();
          stopTones();
          //temporarily store model name as we shall maintain it 
          strlcpy(txtBuff, Model.name, sizeof(txtBuff));
          //load source model into ram
          eeReadModelData(thisModelIdx);
          //restore the model name
          strlcpy(Model.name, txtBuff, sizeof(Model.name));
          //save
          eeSaveModelData(Sys.activeModelIdx);
          //reinitialise mixer calculations
          reinitialiseMixerCalculations();
          //reset other stuff
          resetTimerRegisters();
          resetCounterRegisters();
          Sys.rfEnabled = false;
          //reinit
          thisModelIdx = Sys.activeModelIdx; 
          //exit
          changeToScreen(SCREEN_MODEL);
        }
        else if(clickedButton == KEY_DOWN || heldButton == KEY_SELECT)
        {
          thisModelIdx = Sys.activeModelIdx;
          changeToScreen(SCREEN_MODEL);
        }
      }
      break;
      
    case CONFIRMATION_MODEL_RESET:
      {
        printFullScreenMsg(PSTR("Model data will\nbe reset.\nContinue?\n\nYes [Up] \nNo [Down]"));
        if(clickedButton == KEY_UP)
        {
          showWaitMsg();
          stopTones();
          //temporarily store the model type for later restoring
          uint8_t mdlType = Model.type;
          //reset
          resetModelParams();
          //load the default mixer template
          if(mdlType == MODEL_TYPE_AIRPLANE || mdlType == MODEL_TYPE_MULTICOPTER)
          {
            if(Sys.mixerTemplatesEnabled)
              loadMixerTemplateBasic(0);
          }
          //restore the model type
          Model.type = mdlType;
          //save to eeprom
          eeSaveModelData(Sys.activeModelIdx);
          //reset other stuff
          resetTimerRegisters();
          resetCounterRegisters();
          Sys.rfEnabled = false;
          //reinitialise the mixer
          reinitialiseMixerCalculations();
          //exit
          changeToScreen(SCREEN_MODEL);
        }
        else if(clickedButton == KEY_DOWN || heldButton == KEY_SELECT)
          changeToScreen(SCREEN_MODEL);
      }
      break;
      
    case CONFIRMATION_MODEL_DELETE:
      {
        printFullScreenMsg(PSTR("Delete model?\n\nYes [Up] \nNo [Down]"));
        if(clickedButton == KEY_UP)
        {
          showWaitMsg();
          stopTones();
          eeDeleteModel(thisModelIdx);
          changeToScreen(SCREEN_MODEL);
        }
        else if(clickedButton == KEY_DOWN || heldButton == KEY_SELECT)
          changeToScreen(SCREEN_MODEL);
      }
      break;
    
    ////////////////////////////////// INPUTS //////////////////////////////////////////////////////
    
    case SCREEN_INPUTS:
      {
        drawHeader(mainMenu[MAIN_MENU_INPUTS]);

        enum { 
          PAGE_AIL_CURVE, 
          PAGE_ELE_CURVE, 
          PAGE_RUD_CURVE, 
          PAGE_THR_CURVE,
          PAGE_STICKS,
          PAGE_KNOBS,
          PAGE_SWITCHES,
          
          NUM_PAGES
        };
        
        static uint8_t page;
        
        //check if the page is actually available to prevent getting stuck in an infinite loop
        if(Model.type == MODEL_TYPE_OTHER 
          && (page == PAGE_RUD_CURVE || page == PAGE_AIL_CURVE || page == PAGE_ELE_CURVE || page == PAGE_THR_CURVE))
        {
          page = PAGE_STICKS;
        }
        
        toggleEditModeOnSelectClicked();
        if(focusedItem == 1)
        {
          if(Model.type == MODEL_TYPE_AIRPLANE || Model.type == MODEL_TYPE_MULTICOPTER)
            page = incDec(page, 0, NUM_PAGES - 1, INCDEC_WRAP, INCDEC_SLOW);
          else if(Model.type == MODEL_TYPE_OTHER) 
          {
            //hide rud, ail, ele, thr curves
            do {
              page = incDec(page, 0, NUM_PAGES - 1, INCDEC_WRAP, INCDEC_SLOW);
            } while(page == PAGE_RUD_CURVE || page == PAGE_AIL_CURVE || page == PAGE_ELE_CURVE || page == PAGE_THR_CURVE);
          }
          //Show cursor
          drawCursor(0, 9);
        }
          
        //------ RUD, AIL, ELE CURVES 
        if(page == PAGE_RUD_CURVE || page == PAGE_AIL_CURVE || page == PAGE_ELE_CURVE)  
        {  
          changeFocusOnUpDown(5);
          
          rate_expo_t* qqRateExpo[3] = {&Model.RudDualRate, &Model.AilDualRate, &Model.EleDualRate};
          uint8_t qqPage[3] = {PAGE_RUD_CURVE, PAGE_AIL_CURVE, PAGE_ELE_CURVE};
          uint8_t qqSrc[3] = {SRC_RUD, SRC_AIL, SRC_ELE};
          uint8_t* qqSrcRaw[3] = {&Model.rudSrcRaw, &Model.ailSrcRaw, &Model.eleSrcRaw};
          int16_t qqValIn[3] = {mixSources[Model.rudSrcRaw], mixSources[Model.ailSrcRaw], mixSources[Model.eleSrcRaw]};
          
          uint8_t idx = 0;
          //find the idx
          for(uint8_t i = 0; i < 3; i++)
          {
            if(qqPage[i] == page)
            {
              idx = i;
              break;
            }
          }
          
          int8_t  *rate = &(qqRateExpo[idx]->rate1);
          int8_t  *expo = &(qqRateExpo[idx]->expo1);
          uint8_t *swtch = &(qqRateExpo[idx]->swtch);
          if(*swtch != CTRL_SW_NONE && checkSwitchCondition(*swtch))
          {
            rate = &(qqRateExpo[idx]->rate2);
            expo = &(qqRateExpo[idx]->expo2);
          }
          
          //--- Edit values
          
          if(focusedItem == 2)
            *rate = incDec(*rate, 0, 100, INCDEC_NOWRAP, INCDEC_NORMAL); 
          else if(focusedItem == 3)
            *expo = incDec(*expo, -100, 100, INCDEC_NOWRAP, INCDEC_NORMAL);
          else if(focusedItem == 4 && isEditMode)
            *swtch = incDecControlSwitch(*swtch, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW);
          else if(focusedItem == 5 && isEditMode)
          {
            //auto detect moved
            uint8_t movedSrc = getMovedSource();
            if(movedSrc != SRC_NONE)
            {
              if(movedSrc >= SRC_RAW_ANALOG_FIRST && movedSrc <= SRC_RAW_ANALOG_LAST)
                *qqSrcRaw[idx] = movedSrc;
            }
            //inc dec
            *qqSrcRaw[idx] = incDec(*qqSrcRaw[idx], SRC_RAW_ANALOG_FIRST, 
                                             SRC_RAW_ANALOG_LAST, INCDEC_NOWRAP, INCDEC_SLOW);
          }

          //--- Draw text
          
          display.setCursor(8, 9);
          getSrcName(txtBuff, qqSrc[idx], sizeof(txtBuff));
          display.print(txtBuff);
          display.drawHLine(8, 17, display.getCursorX() - 9, BLACK);

          display.setCursor(0, 20);
          display.print(F("Rate:"));
          display.setCursor(42, 20);
          display.print(*rate);
          display.print(F("%"));
          
          display.setCursor(0, 29);
          display.print(F("Expo:"));
          display.setCursor(42, 29);
          display.print(*expo);
          display.print(F("%"));
          
          display.setCursor(0, 38);
          display.print(F("D/R:"));
          display.setCursor(42, 38);
          if(*swtch == CTRL_SW_NONE)
            display.print(F("--"));
          else
          {
            getControlSwitchName(txtBuff, *swtch, sizeof(txtBuff));
            display.print(txtBuff);
          }
          
          display.setCursor(0, 47);
          display.print(F("Src:"));
          display.setCursor(42, 47);
          getSrcName(txtBuff, *qqSrcRaw[idx], sizeof(txtBuff));
          display.print(txtBuff);
          
          //--- Show the current D/R in use. Only show if we have dual rates enabled
          if(*swtch != CTRL_SW_NONE)
          {
            display.setCursor(0, 56);
            if(checkSwitchCondition(*swtch))
              display.print(F("(Rate2)"));
            else
              display.print(F("(Rate1)"));
          }
          
          if(focusedItem > 1)
            drawCursor(34, (focusedItem * 9) + 2);

          //--- Draw graph
          
          //draw x y axis lines
          display.drawVLine(100, 11, 51, BLACK);
          display.drawHLine(75, 36, 51, BLACK);
          
          //draw stick input marker
          static int8_t lastVal = 0;
          static uint32_t lastMovedTime = 0;
          int16_t difference = qqValIn[idx]/5 - lastVal;
          if(difference >= 5 || difference <= -5)
          {
            lastVal = qqValIn[idx]/5;
            lastMovedTime = millis();
          }
          if(millis() - lastMovedTime < 3000)
          {
            display.setInterlace(false);
            drawDottedVLine(100 + qqValIn[idx]/20, 11, 51, BLACK, WHITE);
            int8_t y = calcRateExpo(qqValIn[idx], *rate, *expo) / 20;
            drawDottedHLine(75, 36 - y, 51, BLACK, WHITE);
          }
          
          //cache the y values so we don't have to recalculate them every time
          static int8_t lastRate, lastExpo;
          static int8_t graphYVals[26]; 
          if(lastRate != *rate || lastExpo != *expo)
          {
            lastRate = *rate;
            lastExpo = *expo;
            for(int16_t i = 0; i <= 25; i++)
              graphYVals[i] = calcRateExpo(i * 20, *rate, *expo) / 20;
          }
          
          //plot the points
          for(int16_t i = 0; i <= 25; i++)
          {
            display.drawPixel(100 + i, 36 - graphYVals[i], BLACK);
            display.drawPixel(100 - i, 36 + graphYVals[i], BLACK);
            if(i > 0)
            {
              int8_t y0 = graphYVals[i - 1];
              int8_t y1 = graphYVals[i];
              if(abs(y1 - y0) > 1)
              {
                display.drawLine(100 + i - 1, 36 - y0, 100 + i, 36 - y1, BLACK);
                display.drawLine(100 - i + 1, 36 + y0, 100 - i, 36 + y1, BLACK);
              }
            }
          }
        }
    
        //------ THROTTLE CURVE
        if(page == PAGE_THR_CURVE)
        {
          display.setCursor(8, 9);
          getSrcName(txtBuff, SRC_THR, sizeof(txtBuff));
          display.print(txtBuff);
          display.drawHLine(8, 17, display.getCursorX() - 9, BLACK);
          
          custom_curve_t *crv = &Model.ThrottleCurve;
          
          static uint8_t thisPt = 0;
          if(thisPt >= crv->numPoints)
            thisPt = 0;
          
          //scrollable list
          
          enum {
            ITEM_CURVE_NUM_POINTS,
            ITEM_CURVE_POINT,
            ITEM_CURVE_XVAL,
            ITEM_CURVE_YVAL,
            ITEM_CURVE_SMOOTH,
            ITEM_THR_SRC_RAW,
            
            ITEM_COUNT
          };
          
          uint8_t listItemIDs[ITEM_COUNT]; 
          uint8_t listItemCount = 0;
          
          //add item Ids to the list of Ids
          for(uint8_t i = 0; i < sizeof(listItemIDs); i++)
            listItemIDs[listItemCount++] = i;
          
          //handle navigation
          changeFocusOnUpDown(listItemCount + 1); //+1 for title focus
          static uint8_t topItem = 1;
          if(focusedItem == 1)
            topItem = 1;
          else if(focusedItem > 1)
          {
            if(focusedItem - 1 < topItem)
              topItem = focusedItem - 1;
            while(focusedItem - 1 >= topItem + 5)
              topItem++;
          }
          
          //fill list and edit items
          for(uint8_t line = 0; line < 5 && line < listItemCount; line++)
          {
            uint8_t ypos = 20 + line*9;
            if(focusedItem - 1 == topItem + line)
              drawCursor(32, ypos);
            
            uint8_t itemID = listItemIDs[topItem - 1 + line];
            bool edit = (itemID == listItemIDs[focusedItem - 2] && isEditMode) ? true : false;
            
            display.setCursor(0, ypos);
            switch(itemID)
            {
              case ITEM_THR_SRC_RAW:
                {
                  display.print(F("Src:"));
                  display.setCursor(40, ypos);
                  getSrcName(txtBuff, Model.thrSrcRaw, sizeof(txtBuff));
                  display.print(txtBuff);
                  if(edit)
                  {
                    //auto detect moved
                    uint8_t movedSrc = getMovedSource();
                    if(movedSrc != SRC_NONE)
                    {
                      if(movedSrc >= SRC_RAW_ANALOG_FIRST && movedSrc <= SRC_RAW_ANALOG_LAST)
                        Model.thrSrcRaw = movedSrc;
                    }
                    //inc dec
                    Model.thrSrcRaw = incDec(Model.thrSrcRaw, SRC_RAW_ANALOG_FIRST, 
                                                     SRC_RAW_ANALOG_LAST, INCDEC_NOWRAP, INCDEC_SLOW);
                  }
                }
                break;
              
              case ITEM_CURVE_NUM_POINTS:
                {
                  display.print(F("Curv:"));
                  display.setCursor(40, ypos);
                  display.print(crv->numPoints);
                  display.print(F("pts"));
                  if(edit)
                  {
                    uint8_t prevNumPoints = crv->numPoints;
                    crv->numPoints = incDec(crv->numPoints, MIN_NUM_POINTS_CUSTOM_CURVE, 
                                                    MAX_NUM_POINTS_CUSTOM_CURVE, INCDEC_NOWRAP, INCDEC_SLOW);
                    if(crv->numPoints != prevNumPoints) //recalculate points if changed
                    {
                      calcNewCurvePts(crv, prevNumPoints);
                      thisPt = 0;
                    }
                  }
                }
                break;
              
              case ITEM_CURVE_POINT:
                {
                  display.print(F("Pt:"));
                  display.setCursor(40, ypos);
                  display.write(97 + thisPt);
                  if(edit)
                    thisPt = incDec(thisPt, 0, crv->numPoints - 1, INCDEC_WRAP, INCDEC_SLOW);
                }
                break;
                
              case ITEM_CURVE_XVAL:
                {
                  display.print(F("Xval:"));
                  display.setCursor(40, ypos);
                  display.print(crv->xVal[thisPt]);
                  if(edit && thisPt > 0 && thisPt < crv->numPoints - 1)
                  {
                    int8_t _minVal = crv->xVal[thisPt - 1];
                    int8_t _maxVal = crv->xVal[thisPt + 1];
                    crv->xVal[thisPt] = incDec(crv->xVal[thisPt], _minVal, _maxVal, INCDEC_NOWRAP, INCDEC_NORMAL);
                  }
                }
                break;
                
              case ITEM_CURVE_YVAL:
                {
                  display.print(F("Yval:"));
                  display.setCursor(40, ypos);
                  display.print(crv->yVal[thisPt]);
                  if(edit)
                    crv->yVal[thisPt] = incDec(crv->yVal[thisPt], -100, 100, INCDEC_NOWRAP, INCDEC_NORMAL);
                }
                break;
                
              case ITEM_CURVE_SMOOTH:
                {
                  display.print(F("Smth:"));
                  drawCheckbox(40, ypos, crv->smooth);
                  if(edit)
                    crv->smooth = incDec(crv->smooth, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
                }
                break;
            }
          }
          
          //scrollbar
          drawScrollBar(69, 19, listItemCount, topItem, 5, 5 * 9);
        
          //--- draw graph
          //draw stick input marker
          static int8_t lastVal = mixSources[Model.thrSrcRaw]/5;
          static uint32_t lastMovedTime = 0;
          bool moved = false;
          int16_t difference = mixSources[Model.thrSrcRaw]/5 - lastVal;
          if(difference >= 5 || difference <= -5)
          {
            lastVal = mixSources[Model.thrSrcRaw]/5;
            lastMovedTime = millis();
          }
          if(millis() - lastMovedTime < 3000)
          {
            moved = true;
            display.setInterlace(false);
          }
          //draw graph
          drawCustomCurve(crv, (focusedItem >= 3 && focusedItem <= 5) ? thisPt : 0xff, moved ? Model.thrSrcRaw : (uint8_t)SRC_NONE);
        }

        //------ STICKS
        if(page == PAGE_STICKS)
        {
          display.setCursor(8, 9);
          strlcpy_P(txtBuff, PSTR("Sticks"), sizeof(txtBuff));
          display.print(txtBuff);
          display.drawHLine(8, 17, display.getCursorX() - 9, BLACK);
          
          uint8_t qqSrc[] = {
            SRC_X1_AXIS, SRC_Y1_AXIS, SRC_X2_AXIS, SRC_Y2_AXIS,
            SRC_X3_AXIS, SRC_Y3_AXIS, SRC_X4_AXIS, SRC_Y4_AXIS
          };
          int16_t qqVal[]  = {
            x1AxisIn, y1AxisIn, x2AxisIn, y2AxisIn,
            x3AxisIn, y3AxisIn, x4AxisIn, y4AxisIn
          };
          
          for(uint8_t i = 0; i < sizeof(qqSrc); i++)
          {
            uint8_t xpos = 0;
            uint8_t ypos = 20 + i*9;
            if(i >= 4)
            {
              xpos = 69;
              ypos = 20 + (i-4)*9;
            }
            display.setCursor(xpos, ypos);
            getSrcName(txtBuff, qqSrc[i], sizeof(txtBuff));
            display.print(txtBuff);
            display.print(F(":"));
            display.setCursor(display.getCursorX() + 6, ypos);
            display.print(qqVal[i]/5);
          }
        }
        
        //------ KNOBS
        if(page == PAGE_KNOBS)
        {
          display.setCursor(8, 9);
          strlcpy_P(txtBuff, PSTR("Knobs"), sizeof(txtBuff));
          display.print(txtBuff);
          display.drawHLine(8, 17, display.getCursorX() - 9, BLACK);
          
          uint8_t qqSrc[] = {SRC_KNOB_A, SRC_KNOB_B};
          int16_t qqVal[]  = {knobAIn, knobBIn};
          for(uint8_t i = 0; i < sizeof(qqSrc); i++)
          {
            uint8_t xpos = 0;
            uint8_t ypos = 20 + i*9;
            
            display.setCursor(xpos, ypos);
            getSrcName(txtBuff, qqSrc[i], sizeof(txtBuff));
            display.print(txtBuff);
            display.print(F(":"));
            display.setCursor(display.getCursorX() + 6, ypos);
            display.print(qqVal[i]/5);
          }
        }
        
        //------ SWITCHES
        if(page == PAGE_SWITCHES)
        {
          display.setCursor(8, 9);
          strlcpy_P(txtBuff, PSTR("Switches"), sizeof(txtBuff));
          display.print(txtBuff);
          display.drawHLine(8, 17, display.getCursorX() - 9, BLACK);
          
          //show switches
          uint8_t cntr = 0;
          for(uint8_t i = 0; i < MAX_NUM_PHYSICAL_SWITCHES; i++)
          {
            if(Sys.swType[i] == SW_ABSENT)
              continue;
            
            uint8_t xpos = 0;
            uint8_t ypos = 20 + cntr * 9;
            if(cntr >= 5)
            {
              xpos = 69;
              ypos = 20 + (cntr - 5)*9;
            }
            display.setCursor(xpos, ypos);
            getSrcName(txtBuff, SRC_SW_PHYSICAL_FIRST + i, sizeof(txtBuff));
            display.print(txtBuff);
            display.print(F(": "));
            if(swState[i] == SWUPPERPOS) display.print(F("-100"));
            else if(swState[i] == SWLOWERPOS) display.print(F("100"));
            else display.print(F("0"));
            
            cntr++;
          }
        }
        
        //------ Exit
        if(heldButton == KEY_SELECT)
          changeToScreen(SCREEN_MAIN_MENU);
      }
      break;
      
    ////////////////////////////////// MIXER ///////////////////////////////////////////////////////
      
    case SCREEN_MIXER:
      {
        drawHeader(mainMenu[MAIN_MENU_MIXER]);
        
        mixer_params_t *mxr = &Model.Mixer[thisMixIdx];
        
        //For safety and to prevent unintended effects, we do not directly edit the mixer output variable 
        //but instead use some kind of delayed action, where we edit a temporary variable 
        //and later assign it to the output in the right conditions.
        static uint8_t tempMixerOutput;
        static bool tempInitialised = false;
        if(!tempInitialised)
        {
          tempMixerOutput = mxr->output;
          tempInitialised = true;
        }

        //--- Dynamic list ---
        
        enum {
          ITEM_MIX_NO,
          ITEM_MIX_OUTPUT,
          ITEM_MIX_SWITCH,
          ITEM_MIX_OPERATION,
          ITEM_MIX_INPUT,
          ITEM_MIX_WEIGHT,
          ITEM_MIX_OFFSET,
          ITEM_MIX_CURVE_TYPE,
          ITEM_MIX_CURVE_VAL,
          ITEM_MIX_TRIM_ENABLED,
          ITEM_MIX_FLIGHT_MODE,
          ITEM_MIX_DELAY_UP,
          ITEM_MIX_DELAY_DOWN,
          ITEM_MIX_SLOW_UP,
          ITEM_MIX_SLOW_DOWN,
          
          ITEM_COUNT
        };
        
        uint8_t listItemIDs[ITEM_COUNT]; //stores the added item IDs
        uint8_t listItemCount = 0;
        
        //add item Ids to the list of Ids
        for(uint8_t i = 0; i < sizeof(listItemIDs); i++)
        {
          if(mxr->operation == MIX_HOLD)
          {
            if(i != ITEM_MIX_NO && i != ITEM_MIX_OUTPUT && i != ITEM_MIX_SWITCH && i!= ITEM_MIX_OPERATION)
              continue;
          }
          if(i == ITEM_MIX_FLIGHT_MODE)
          {
            if(Model.type == MODEL_TYPE_AIRPLANE || Model.type == MODEL_TYPE_MULTICOPTER)
              listItemIDs[listItemCount++] = i;
            continue;
          }
          listItemIDs[listItemCount++] = i;
        }
        
        //add context menu
        uint8_t contextMenu = listItemCount + 1;
        
        //initialise the view
        static uint8_t topItem;
        static uint8_t lastFocusedItem;
        static bool viewInitialised = false;
        if(!viewInitialised)
        {
          focusedItem = 1;
          lastFocusedItem = 1;
          topItem = 1;
          viewInitialised = true;
        }
        
        //handle navigatation
        
        focusedItem = lastFocusedItem;
        changeFocusOnUpDown(listItemCount + 1); //add 1 for the context menu access
        toggleEditModeOnSelectClicked();
        
        if(focusedItem < topItem)
          topItem = focusedItem;
        while(focusedItem >= topItem + 7)
          topItem++;
        if(focusedItem == contextMenu)
          topItem = 1;
        
        lastFocusedItem = focusedItem;
        
        //fill list and edit items
        for(uint8_t line = 0; line < 7 && line < listItemCount; line++)
        {
          uint8_t ypos = 8 + line*8;
          if(focusedItem == topItem + line)
            drawCursor(52, ypos);
          
          uint8_t itemID = listItemIDs[topItem - 1 + line];
          bool edit = (itemID == listItemIDs[focusedItem - 1] && isEditMode) ? true : false;
          
          display.setCursor(0, ypos);
          switch(itemID)
          {
            case ITEM_MIX_NO:
              {
                display.print(F("Mix no:"));
                display.setCursor(60, ypos);
                display.print(F("#"));
                display.print(thisMixIdx + 1);
                display.setCursor(90, ypos);
                display.print(mxr->name);
                if(edit) 
                {
                  //assign from temp
                  mxr->output = tempMixerOutput;
                  tempInitialised = false;
                  //change to another mix
                  thisMixIdx = incDec(thisMixIdx, 0, NUM_MIXSLOTS - 1, INCDEC_WRAP, INCDEC_SLOW);
                }
              }
              break;
              
            case ITEM_MIX_OUTPUT:
              {
                display.print(F("Output:"));
                display.setCursor(60, ypos);
                getSrcName(txtBuff, tempMixerOutput, sizeof(txtBuff));
                display.print(txtBuff);
                if(tempMixerOutput >= SRC_CH1 && tempMixerOutput < SRC_CH1 + NUM_RC_CHANNELS)
                {
                  uint8_t chIdx = tempMixerOutput - SRC_CH1;
                  display.setCursor(90, ypos);
                  display.print(Model.Channel[chIdx].name);
                }
                if(edit)
                {
                  do {
                  tempMixerOutput = incDecSource(tempMixerOutput, INCDEC_FLAG_MIX_SRC);
                  } while(tempMixerOutput > SRC_NONE && tempMixerOutput < SRC_CH1); //skip over
                }                
              }
              break;

            case ITEM_MIX_SWITCH:
              {
                display.print(F("Switch:"));
                display.setCursor(60, ypos);
                getControlSwitchName(txtBuff, mxr->swtch, sizeof(txtBuff));
                display.print(txtBuff);
                if(edit)
                  mxr->swtch = incDecControlSwitch(mxr->swtch, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW);
              }
              break;
            
            case ITEM_MIX_OPERATION:
              {
                display.print(F("Opertn:"));
                display.setCursor(60, ypos);
                display.print(findStringInIdStr(enum_MixerOperation, mxr->operation));
                if(edit)
                  mxr->operation = incDec(mxr->operation, 0, MIX_OPERATOR_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);
              }
              break;
              
            case ITEM_MIX_INPUT:
              {
                display.print(F("Input :"));
                display.setCursor(60, ypos);
                getSrcName(txtBuff, mxr->input, sizeof(txtBuff));
                display.print(txtBuff);
                if(edit)
                {
                  //auto detect moved source
                  uint8_t movedSrc = procMovedSource(getMovedSource());
                  if(movedSrc != SRC_NONE)
                    mxr->input = movedSrc;
                  //inc dec
                  mxr->input = incDecSource(mxr->input, INCDEC_FLAG_MIX_SRC);
                }
              }
              break;
              
            case ITEM_MIX_WEIGHT:
              {
                display.print(F("Weight:"));
                display.setCursor(60, ypos);
                display.print(mxr->weight);
                if(edit)
                  mxr->weight = incDec(mxr->weight, -100, 100, INCDEC_NOWRAP, INCDEC_NORMAL);
              }
              break;
              
            case ITEM_MIX_OFFSET:
              {
                display.print(F("Offset:"));
                display.setCursor(60, ypos);
                display.print(mxr->offset);
                if(edit)
                  mxr->offset = incDec(mxr->offset, -100, 100, INCDEC_NOWRAP, INCDEC_NORMAL);
              }
              break;
              
            case ITEM_MIX_CURVE_TYPE:
              {
                display.print(F("Curve :"));
                display.setCursor(60, ypos);
                display.print(findStringInIdStr(enum_MixerCurveType, mxr->curveType));
                if(edit)
                {
                  uint8_t prevCurveType = mxr->curveType;
                  mxr->curveType = incDec(mxr->curveType, 0, MIX_CURVE_TYPE_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);
                  if(mxr->curveType != prevCurveType) 
                    mxr->curveVal = 0; //reset value if changed
                }
              }
              break;
              
            case ITEM_MIX_CURVE_VAL:
              {
                display.print(F("CrvVal:"));
                display.setCursor(60, ypos);
                if(mxr->curveType == MIX_CURVE_TYPE_DIFF || mxr->curveType == MIX_CURVE_TYPE_EXPO)
                  display.print(mxr->curveVal);
                else if(mxr->curveType == MIX_CURVE_TYPE_FUNCTION)
                  display.print(findStringInIdStr(enum_MixerCurveType_Func, mxr->curveVal));
                else if(mxr->curveType == MIX_CURVE_TYPE_CUSTOM)
                {
                  if(isEmptyStr(Model.CustomCurve[mxr->curveVal].name, sizeof(Model.CustomCurve[0].name)))
                  {
                    display.print(F("Crv"));
                    display.print(mxr->curveVal + 1);
                  }
                  else
                    display.print(Model.CustomCurve[mxr->curveVal].name);
                }
                if(edit)
                {
                  if(mxr->curveType == MIX_CURVE_TYPE_DIFF || mxr->curveType == MIX_CURVE_TYPE_EXPO)
                    mxr->curveVal = incDec(mxr->curveVal, -100, 100, INCDEC_NOWRAP, INCDEC_NORMAL);
                  else if(mxr->curveType == MIX_CURVE_TYPE_FUNCTION)
                    mxr->curveVal = incDec(mxr->curveVal, 0, MIX_CURVE_FUNC_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);
                  else if(mxr->curveType == MIX_CURVE_TYPE_CUSTOM)
                    mxr->curveVal = incDec(mxr->curveVal, 0, NUM_CUSTOM_CURVES - 1, INCDEC_WRAP, INCDEC_SLOW);
                }
              }
              break;
              
            case ITEM_MIX_TRIM_ENABLED:
              {
                display.print(F("Trim  :"));
                display.setCursor(60, ypos);
                
                uint8_t input = mxr->input;
                if(input == SRC_RUD) input = Model.rudSrcRaw;
                if(input == SRC_THR) input = Model.thrSrcRaw;
                if(input == SRC_AIL) input = Model.ailSrcRaw;
                if(input == SRC_ELE) input = Model.eleSrcRaw;
                
                if(input == SRC_X1_AXIS || input == SRC_Y1_AXIS || input == SRC_X2_AXIS || input == SRC_Y2_AXIS)
                {
                  drawCheckbox(60, ypos, mxr->trimEnabled);
                  if(edit)
                    mxr->trimEnabled = incDec(mxr->trimEnabled, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
                }
                else
                  display.print(F("N/A"));
              }
              break;
              
            case ITEM_MIX_FLIGHT_MODE:
              {
                display.print(F("F-Mode:"));
                display.setCursor(60, ypos);
                if(mxr->flightMode == 0xff)
                  display.print(F("All"));
                else
                {
                  bool delimit = false;
                  for(uint8_t i = 0; i < NUM_FLIGHT_MODES; i++)
                  {
                    if((mxr->flightMode >> i) & 0x01)
                    {
                      if(delimit)
                        display.print(F(","));
                      display.print(i+1);
                      delimit = true;
                    }
                  }
                }
                if(edit)
                  changeToScreen(DIALOG_MIX_FLIGHT_MODE);
              }
              break;
              
            case ITEM_MIX_DELAY_UP:
              {
                display.print(F("Dly Up:"));
                display.setCursor(60, ypos);
                printSeconds(mxr->delayUp);
                if(edit)
                  mxr->delayUp = incDec(mxr->delayUp, 0, 600, INCDEC_NOWRAP, INCDEC_SLOW_START);
              }
              break;
              
            case ITEM_MIX_DELAY_DOWN:
              {
                display.print(F("Dly Dn:"));
                display.setCursor(60, ypos);
                printSeconds(mxr->delayDown);
                if(edit)
                  mxr->delayDown = incDec(mxr->delayDown, 0, 600, INCDEC_NOWRAP, INCDEC_SLOW_START);
              }
              break;
              
            case ITEM_MIX_SLOW_UP:
              {
                display.print(F("SlowUp:"));
                display.setCursor(60, ypos);
                printSeconds(mxr->slowUp);
                if(edit)
                  mxr->slowUp = incDec(mxr->slowUp, 0, 600, INCDEC_NOWRAP, INCDEC_SLOW_START);
              }
              break;
              
            case ITEM_MIX_SLOW_DOWN:
              {
                display.print(F("SlowDn:"));
                display.setCursor(60, ypos);
                printSeconds(mxr->slowDown);
                if(edit)
                  mxr->slowDown = incDec(mxr->slowDown, 0, 600, INCDEC_NOWRAP, INCDEC_SLOW_START);
              }
              break;
          }
        }
        
        //Draw scroll bar
        drawScrollBar(127, 8, listItemCount, topItem, 7, 7 * 8);
        
        //Show context menu icon
        display.fillRect(120, 0, 8, 7, WHITE);
        display.drawBitmap(120, 0, focusedItem == contextMenu ? icon_context_menu_focused : icon_context_menu, 8, 7, BLACK);
        
        //Assign from temp
        if(!isEditMode || (buttonCode == 0 && millis() - buttonReleaseTime > 1000))
        {
          mxr->output = tempMixerOutput;
          tempInitialised = false;
        }

        //Open context menu
        if(focusedItem == contextMenu && clickedButton == KEY_SELECT)
        {
          changeToScreen(CONTEXT_MENU_MIXER);
          viewInitialised = false;
          //assign from temp
          mxr->output = tempMixerOutput;
          tempInitialised = false;
        }

        //Exit
        if(heldButton == KEY_SELECT)
        {
          viewInitialised = false;
          changeToScreen(SCREEN_MAIN_MENU);
          //assign from temp
          mxr->output = tempMixerOutput; 
          tempInitialised = false;
        } 
      }
      break;
      
    case DIALOG_MIX_FLIGHT_MODE:
      {
        //--- scrollable list

        //handle navigation
        changeFocusOnUpDown(NUM_FLIGHT_MODES);
        static uint8_t topItem = 1;
        if(focusedItem < topItem)
          topItem = focusedItem;
        while(focusedItem >= topItem + 4)
          topItem++;
        
        //Calculate y coord for item 0. Items are center aligned
        uint8_t numVisible = NUM_FLIGHT_MODES <= 4 ? NUM_FLIGHT_MODES : 4;
        uint8_t y0 = ((display.height() - (numVisible * 9)) / 2) + 1;  //9 is line height
        
        //draw bounding box
        drawBoundingBox(11, y0 - 3, 105, numVisible * 9 + 4, BLACK);
        
        //fill list
        for(uint8_t line = 0; line < numVisible; line++)
        {
          uint8_t ypos = y0 + line * 9;
          uint8_t item = topItem + line;
          if(focusedItem == item)
            drawCursor(17, ypos);
          uint8_t idx = item - 1;
          drawCheckbox(25, ypos, Model.Mixer[thisMixIdx].flightMode & (1 << idx));
          display.setCursor(35, ypos);
          display.print(F("FMD"));
          display.print(idx + 1);
          display.print(F(" "));
          display.print(Model.FlightMode[idx].name);
        }
        
        //scroll bar
        uint8_t  y = Sys.useRoundRect ? y0 : y0 - 1;
        uint16_t h = Sys.useRoundRect ? numVisible * 9 - 2 : numVisible * 9;
        drawScrollBar(112, y, NUM_FLIGHT_MODES, topItem, numVisible, h);
        
        //--- edit items
        toggleEditModeOnSelectClicked();
        if(isEditMode && (pressedButton == KEY_UP || pressedButton == KEY_DOWN))
        {
          uint8_t idx = focusedItem - 1;
          Model.Mixer[thisMixIdx].flightMode ^= 1 << idx; //toggle bit
        }
        
        //--- exit 
        if(heldButton == KEY_SELECT)
        {
          if((Model.Mixer[thisMixIdx].flightMode & (uint8_t)((1 << NUM_FLIGHT_MODES) - 1)) == 0) //nothing selected
          {
            killButtonEvents();
            makeToast(PSTR("Select at least one"), 2000, 0);
          }
          else 
            changeToScreen(SCREEN_MIXER);
        }
      }
      break;

    case CONTEXT_MENU_MIXER:
      {
        enum {
          ITEM_MIXER_OUTPUTS,
          ITEM_COPY_MIX,
          ITEM_MOVE_MIX,
          ITEM_INSERT_FREE,
          ITEM_RENAME_MIX,
          ITEM_RESET_MIX,
          ITEM_RESET_ALL_MIXES,
          ITEM_COMPACT_MIXES,
          ITEM_MIXER_TEMPLATES,
          ITEM_MIXER_OVERVIEW,
        };
        
        contextMenuInitialise();
        contextMenuAddItem(PSTR("View outputs"), ITEM_MIXER_OUTPUTS);
        contextMenuAddItem(PSTR("Copy mix to"), ITEM_COPY_MIX);
        contextMenuAddItem(PSTR("Move mix to"), ITEM_MOVE_MIX);
        contextMenuAddItem(PSTR("Insert free"), ITEM_INSERT_FREE);
        contextMenuAddItem(PSTR("Rename mix" ), ITEM_RENAME_MIX);
        contextMenuAddItem(PSTR("Reset mix" ), ITEM_RESET_MIX);
        contextMenuAddItem(PSTR("Reset all mixes"), ITEM_RESET_ALL_MIXES);
        contextMenuAddItem(PSTR("Compact mixes"), ITEM_COMPACT_MIXES);
        contextMenuAddItem(PSTR("Show overview"), ITEM_MIXER_OVERVIEW);
        if(Model.type == MODEL_TYPE_AIRPLANE || Model.type == MODEL_TYPE_MULTICOPTER)
        {
          if(Sys.mixerTemplatesEnabled)
            contextMenuAddItem(PSTR("Templates"), ITEM_MIXER_TEMPLATES);
        }
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_MIXER_OUTPUTS) 
          changeToScreen(SCREEN_MIXER_OUTPUT);
        if(contextMenuSelectedItemID == ITEM_COPY_MIX)
        {
          destMixIdx = thisMixIdx;
          changeToScreen(DIALOG_COPY_MIX);
        }
        if(contextMenuSelectedItemID == ITEM_MOVE_MIX)
        {
          destMixIdx = thisMixIdx;
          changeToScreen(DIALOG_MOVE_MIX);
        }
        if(contextMenuSelectedItemID == ITEM_INSERT_FREE)
        {
          //Find a slot that is empty.
          //If the slot exists after thisMixIdx, move it here, otherwise we dont.
          uint8_t freeSlot = 0xFF;
          uint8_t mixIdx = NUM_MIXSLOTS - 1;
          uint8_t occupiedCount = 0;
          while(mixIdx > thisMixIdx)
          {
            if(freeSlot == 0xFF && Model.Mixer[mixIdx].output == SRC_NONE && Model.Mixer[mixIdx].input == SRC_NONE)
              freeSlot = mixIdx;
            //count how many occupied slots exist between the free slot and the destination
            if(freeSlot != 0xFF && (Model.Mixer[mixIdx].output != SRC_NONE || Model.Mixer[mixIdx].input != SRC_NONE))
              occupiedCount++;
            mixIdx--;
          }
          if(freeSlot == 0xFF || (occupiedCount == 0 && Model.Mixer[thisMixIdx].output == SRC_NONE 
                                  && Model.Mixer[thisMixIdx].input == SRC_NONE))
          {
            makeToast(PSTR("Insert failed"), 2000, 0);
          }
          else
          {
            resetMixerParams(freeSlot);
            moveMix(thisMixIdx, freeSlot);
          }
          changeToScreen(SCREEN_MIXER);
        }
        if(contextMenuSelectedItemID == ITEM_RENAME_MIX)
          changeToScreen(DIALOG_RENAME_MIX);
        if(contextMenuSelectedItemID == ITEM_RESET_MIX)
        {
          resetMixerParams(thisMixIdx);
          changeToScreen(SCREEN_MIXER);
        }
        if(contextMenuSelectedItemID == ITEM_RESET_ALL_MIXES)
          changeToScreen(CONFIRMATION_MIXES_RESET);
        if(contextMenuSelectedItemID == ITEM_COMPACT_MIXES)
        {
          //Find all empty slots and move them to the end.
          //This is similar to the classic 'move all zeros to end of array' problem.
          
          //find the rearmost occupied slot
          uint8_t rearmostIdx = 0;
          for(uint8_t mixIdx = NUM_MIXSLOTS - 1; mixIdx > 0; mixIdx--)
          {
            if(Model.Mixer[mixIdx].output != SRC_NONE || Model.Mixer[mixIdx].input != SRC_NONE)
            {
              rearmostIdx = mixIdx;
              break;
            }
          }
          //find and move all free slots to end
          uint8_t mixIdx = 0;
          while(mixIdx < rearmostIdx)
          {
            if(Model.Mixer[mixIdx].output == SRC_NONE && Model.Mixer[mixIdx].input == SRC_NONE)
            {
              moveMix(NUM_MIXSLOTS - 1, mixIdx);
              rearmostIdx--;
              //we don't immediately increment the mixIdx to also move any adjacent free slots
            }
            else
              mixIdx++;
          }
          thisMixIdx = 0;
          changeToScreen(SCREEN_MIXER);
        }
        if(contextMenuSelectedItemID == ITEM_MIXER_TEMPLATES)
          changeToScreen(CONTEXT_MENU_MIXER_TEMPLATES);
        if(contextMenuSelectedItemID == ITEM_MIXER_OVERVIEW)
          changeToScreen(SCREEN_MIXER_OVERVIEW);
        
        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_MIXER);
      }
      break;
      
    case CONTEXT_MENU_MIXER_TEMPLATES:
    case CONFIRMATION_LOAD_MIXER_TEMPLATE:
      {
        enum {
          ITEM_BASIC,
          ITEM_ELEVON,
          ITEM_VTAIL,
          ITEM_DIFF_THRUST
        };
        
        if(theScreen == CONTEXT_MENU_MIXER_TEMPLATES)
        {
          contextMenuInitialise();
          contextMenuAddItem(PSTR("Basic"), ITEM_BASIC);
          if(Model.type == MODEL_TYPE_AIRPLANE)
          {
            contextMenuAddItem(PSTR("Elevon"), ITEM_ELEVON);
            contextMenuAddItem(PSTR("Vtail"), ITEM_VTAIL);
            contextMenuAddItem(PSTR("Diff thrust"), ITEM_DIFF_THRUST);
          }
          contextMenuDraw();
          
          if(contextMenuSelectedItemID != 0xff) //selection
            theScreen = CONFIRMATION_LOAD_MIXER_TEMPLATE;

          if(heldButton == KEY_SELECT) //exit
            changeToScreen(SCREEN_MIXER);
        }
        else if(theScreen == CONFIRMATION_LOAD_MIXER_TEMPLATE)
        {
          bool hasWarning = false;
          //basic
          if(contextMenuSelectedItemID == ITEM_BASIC) 
          {
            if(hasEnoughMixSlots(thisMixIdx, 4))
            {
              if(hasOccupiedMixSlots(thisMixIdx, 4))
                hasWarning = true;
              if(clickedButton == KEY_UP || !hasWarning)
              {
                loadMixerTemplateBasic(thisMixIdx);
                changeToScreen(SCREEN_MIXER);
              }
            }
            else
              changeToScreen(SCREEN_MIXER);
          }
          //elevon
          if(contextMenuSelectedItemID == ITEM_ELEVON) 
          {
            if(hasEnoughMixSlots(thisMixIdx, 4))
            {
              if(hasOccupiedMixSlots(thisMixIdx, 4))
                hasWarning = true;
              if(clickedButton == KEY_UP || !hasWarning)
              {
                loadMixerTemplateElevon(thisMixIdx);
                changeToScreen(SCREEN_MIXER);
              }
            }
            else
              changeToScreen(SCREEN_MIXER);
          }
          //vtail
          if(contextMenuSelectedItemID == ITEM_VTAIL) 
          {
            if(hasEnoughMixSlots(thisMixIdx, 4))
            {
              if(hasOccupiedMixSlots(thisMixIdx, 4))
                hasWarning = true;
              if(clickedButton == KEY_UP || !hasWarning)
              {
                loadMixerTemplateVtail(thisMixIdx);
                changeToScreen(SCREEN_MIXER);
              }
            }
            else
              changeToScreen(SCREEN_MIXER);
          }
          //differential thrust
          if(contextMenuSelectedItemID == ITEM_DIFF_THRUST) 
          {
            if(hasEnoughMixSlots(thisMixIdx, 4))
            {
              if(hasOccupiedMixSlots(thisMixIdx, 4))
                hasWarning = true;
              if(clickedButton == KEY_UP || !hasWarning)
              {
                loadMixerTemplateDiffThrust(thisMixIdx);
                changeToScreen(SCREEN_MIXER);
              }
            }
            else
              changeToScreen(SCREEN_MIXER);
          }
          
          //show warning
          if(hasWarning)
            printFullScreenMsg(PSTR("One or more\ndestination slots\nalready occupied.\nContinue?\n\nYes [Up] \nNo [Down]"));
          
          //exit
          if(clickedButton == KEY_DOWN || heldButton == KEY_SELECT)
            changeToScreen(SCREEN_MIXER);
        }
      }
      break;

    case DIALOG_RENAME_MIX:
      {
        isEditTextDialog = true;
        editTextDialog(PSTR("Mix name"), Model.Mixer[thisMixIdx].name, sizeof(Model.Mixer[0].name), true, true, false);
        if(!isEditTextDialog) //exited
          changeToScreen(SCREEN_MIXER);
      }
      break;
      
    case CONFIRMATION_MIXES_RESET:
      {
        printFullScreenMsg(PSTR("Reset all mixes?\n\nYes [Up] \nNo [Down]"));
        if(clickedButton == KEY_UP)
        {
          resetMixerParams();
          thisMixIdx = 0;
          destMixIdx = 0;
          changeToScreen(SCREEN_MIXER);
        }
        else if(clickedButton == KEY_DOWN || heldButton == KEY_SELECT)
          changeToScreen(SCREEN_MIXER);
      }
      break;
      
    case SCREEN_MIXER_OUTPUT:
      {
        display.setInterlace(false); 
        
        drawHeader(PSTR("Mixer output"));
        
        uint8_t numPagesPrpCh = (NUM_RC_CHANNELS + 7) / 8;
        uint8_t numPagesVrtCh = (NUM_VIRTUAL_CHANNELS + 7) / 8;
        uint8_t numPages = numPagesPrpCh + numPagesVrtCh;
        static uint8_t thisPage = 1;
        static bool viewInitialised = false;
        if(!viewInitialised) //start in the page that has the channel we want to view
        {
          uint8_t _outputIdx = Model.Mixer[thisMixIdx].output;
          if(_outputIdx >= SRC_CH1 && _outputIdx < (SRC_CH1 + NUM_RC_CHANNELS))
            thisPage = ((_outputIdx - SRC_CH1) + 8) / 8;
          else if(_outputIdx >= SRC_VIRTUAL_FIRST && _outputIdx <= SRC_VIRTUAL_LAST)
            thisPage = numPagesPrpCh + ((_outputIdx - SRC_VIRTUAL_FIRST) + 8) / 8;
          viewInitialised = true;
        }
        
        //change page
        isEditMode = true;
        thisPage = incDec(thisPage, numPages, 1, INCDEC_WRAP, INCDEC_SLOW);
        
        //--- draw graphs---
        uint8_t startIdx = (thisPage - 1) * 8;
        uint8_t endIdx = NUM_RC_CHANNELS - 1;
        uint8_t offsetIdx = 0;
        display.setCursor(0, 7);
        if(thisPage <= numPagesPrpCh) //show real channels 
          display.print(F("Ch"));
        else //show virtual channels
        {  
          display.print(F("Virt"));
          startIdx = NUM_RC_CHANNELS + (8 * (thisPage - (numPagesPrpCh + 1)));
          endIdx = NUM_RC_CHANNELS + NUM_VIRTUAL_CHANNELS - 1;
          offsetIdx = NUM_RC_CHANNELS;
        }
        uint8_t count = 0;
        for(uint8_t i = startIdx; i < startIdx + 8 && i <= endIdx; i++)
        {
          int8_t outVal = mixSources[SRC_CH1 + i] / 25;
          uint8_t xOffset = (i - startIdx) * 16;
          if(outVal > 0)
            display.fillRect(6 + xOffset, 36 - outVal, 3, outVal , BLACK);
          else if(outVal < 0)
          {
            outVal = -outVal;
            display.fillRect(6 + xOffset, 35, 3, outVal, BLACK);
          }
          drawDottedVLine(7 + xOffset, 16, 39, BLACK, WHITE);
          //show the channel number
          uint8_t num = (i + 1) - offsetIdx;
          if(num < 10)
            display.setCursor(5 + xOffset, 57);
          else
            display.setCursor(2 + xOffset, 57);
          display.print(num);
          count++;
        }
        //draw midpoint
        display.drawHLine(3, 35, 9 + 16*(count - 1) , BLACK); 
        //show scrollbar
        drawScrollBar(127, 8, numPages, thisPage, 1, 1 * 56);

        if(heldButton == KEY_SELECT)
        {
          viewInitialised = false;
          changeToScreen(SCREEN_MIXER);
        }
      }
      break;
    
    case DIALOG_COPY_MIX:
    case DIALOG_MOVE_MIX:
      {
        isEditMode = true;
        destMixIdx = incDec(destMixIdx, 0, NUM_MIXSLOTS - 1, INCDEC_WRAP, INCDEC_SLOW);
        drawDialogCopyMove(PSTR("Mix#"), thisMixIdx, destMixIdx, theScreen == DIALOG_COPY_MIX);
        if(clickedButton == KEY_SELECT)
        {
          if(theScreen == DIALOG_COPY_MIX)
            Model.Mixer[destMixIdx] =  Model.Mixer[thisMixIdx];
          else
            moveMix(destMixIdx, thisMixIdx);
          thisMixIdx = destMixIdx; 
          changeToScreen(SCREEN_MIXER); 
        }
        if(heldButton == KEY_SELECT)
          changeToScreen(SCREEN_MIXER);
      }
      break;
      
    case SCREEN_MIXER_OVERVIEW:
      {
        drawHeader(PSTR("Overview"));
        
        //--- scrollable list
        
        uint8_t listItemIDs[NUM_MIXSLOTS];
        uint8_t listItemCount = 0;
        //add to list
        for(uint8_t i = 0; i < NUM_MIXSLOTS; i++)
        {
          if(Model.Mixer[i].output == SRC_NONE)
            continue;
          if(Model.Mixer[i].input == SRC_NONE && Model.Mixer[i].operation != MIX_HOLD)
            continue;
          listItemIDs[listItemCount++] = i;
        }
        
        static uint8_t thisPage = 1;
        
        if(listItemCount == 0)
          printFullScreenMsg(PSTR("No mixes defined yet"));
        else
        {
          uint8_t numPages = (listItemCount + 5) / 6;
          isEditMode = true;
          thisPage = incDec(thisPage, numPages, 1, INCDEC_WRAP, INCDEC_SLOW);
          
          uint8_t startIdx = (thisPage - 1) * 6;
          for(uint8_t i = startIdx; i < startIdx + 6 && i < listItemCount; i++)
          {
            uint8_t ypos = 9 * (i - startIdx + 1);
            uint8_t mixIdx = listItemIDs[i];
            
            //output
            display.setCursor(0, ypos);
            getSrcName(txtBuff, Model.Mixer[mixIdx].output, sizeof(txtBuff));
            display.print(txtBuff);
            
            //operation
            display.setCursor(30, ypos);
            if(Model.Mixer[mixIdx].operation == MIX_ADD) display.print(F("+="));
            if(Model.Mixer[mixIdx].operation == MIX_MULTIPLY) display.print(F("*="));
            if(Model.Mixer[mixIdx].operation == MIX_REPLACE) display.print(F("="));
            if(Model.Mixer[mixIdx].operation == MIX_HOLD) display.print(F("Hold"));
            
            if(Model.Mixer[mixIdx].operation != MIX_HOLD)
            {
              //weight. Right align
              uint8_t digits = 1;
              uint8_t num = abs(Model.Mixer[mixIdx].weight);          
              while(num >= 10)
              {
                num /= 10;
                digits++;
              }
              uint8_t xpos = 61 - (digits - 1)*6;
              if(Model.Mixer[mixIdx].weight < 0)
                xpos -= 6;
              display.setCursor(xpos, ypos);
              display.print(Model.Mixer[mixIdx].weight);
              
              //input 
              display.setCursor(68, ypos);
              getSrcName(txtBuff, Model.Mixer[mixIdx].input, sizeof(txtBuff));
              display.print(txtBuff);
            }
            
            //swtch, right align
            if(Model.Mixer[mixIdx].swtch != CTRL_SW_NONE)
            {
              getControlSwitchName(txtBuff, Model.Mixer[mixIdx].swtch, sizeof(txtBuff));
              uint8_t len = strlen(txtBuff);
              display.setCursor(127 - len * 6, ypos);
              display.print(txtBuff);
            }
          }
          
          //scrollbar
          drawScrollBar(127, 9, numPages, thisPage, 1, 1 * 54);
        }

        if(heldButton == KEY_SELECT)
        {
          thisPage = 1;
          changeToScreen(SCREEN_MIXER);
        }
      }
      break;
      
    ////////////////////////////////// OUTPUTS /////////////////////////////////////////////////////
      
    case SCREEN_OUTPUTS:
      {
        channel_params_t *ch = &Model.Channel[thisChIdx];

        drawHeader(mainMenu[MAIN_MENU_OUTPUTS]);

        //--- draw ---
        
        display.setCursor(0, 8);
        display.print(F("Channel:"));
        display.setCursor(62, 8);
        display.print(F("Ch"));
        display.print(thisChIdx + 1);
        display.setCursor(display.getCursorX() + 6, 8);
        if(!isEmptyStr(ch->name, sizeof(ch->name)))
          display.print(ch->name);
        
        display.setCursor(0, 16);
        display.print(F("Curve:"));
        display.setCursor(62, 16);
        if(ch->curve == -1)
          display.print(F("--"));
        else
        {
          if(isEmptyStr(Model.CustomCurve[ch->curve].name, sizeof(Model.CustomCurve[0].name)))
          {
            display.print(F("Crv"));
            display.print(ch->curve + 1);
          }
          else
            display.print(Model.CustomCurve[ch->curve].name);
        }
        
        display.setCursor(0, 24);
        display.print(F("Reverse:"));
        drawCheckbox(62, 24, ch->reverse);

        display.setCursor(0, 32);
        display.print(F("Subtrim:"));
        display.setCursor(62, 32);
        display.print(ch->subtrim);

        display.setCursor(0, 40);
        display.print(F("Override:"));
        display.setCursor(62, 40);
        getControlSwitchName(txtBuff, ch->overrideSwitch, sizeof(txtBuff));
        display.print(txtBuff);
        display.setCursor(98, 40);
        if(ch->overrideSwitch == CTRL_SW_NONE) display.print(F("--"));
        else display.print(ch->overrideVal);
        
        display.setCursor(0, 48);
        display.print(F("Failsafe:"));
        display.setCursor(62, 48);
        if(ch->failsafe < -100)
          display.print(findStringInIdStr(enum_ChannelFailsafe, ch->failsafe));
        else
          display.print(ch->failsafe);

        display.setCursor(0, 56);
        display.print(F("Endpts:"));
        display.setCursor(62, 56);
        display.print(ch->endpointL);
        display.setCursor(98, 56);
        display.print(ch->endpointR);
        
        //draw context menu icon
        display.fillRect(120, 0, 8, 7, WHITE);
        display.drawBitmap(120, 0, focusedItem == 10 ? icon_context_menu_focused : icon_context_menu, 8, 7, BLACK);
        
        //show the cursor
        if(focusedItem <= 4) drawCursor(54, focusedItem * 8);
        else if(focusedItem == 5) drawCursor(54, 40);
        else if(focusedItem == 6) drawCursor(90, 40);
        else if(focusedItem == 7) drawCursor(54, 48);
        else if(focusedItem == 8) drawCursor(54, 56);
        else if(focusedItem == 9) drawCursor(90, 56);

        changeFocusOnUpDown(10);
        toggleEditModeOnSelectClicked();
        
        //edit items
        if(focusedItem == 1)
          thisChIdx = incDec(thisChIdx, 0, NUM_RC_CHANNELS - 1, INCDEC_WRAP, INCDEC_SLOW); 
        else if(focusedItem == 2)
          ch->curve = incDec(ch->curve, -1, NUM_CUSTOM_CURVES - 1, INCDEC_NOWRAP, INCDEC_SLOW);
        else if(focusedItem == 3)
          ch->reverse = incDec(ch->reverse, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        else if(focusedItem == 4)
          ch->subtrim = incDec(ch->subtrim, -20, 20, INCDEC_NOWRAP, INCDEC_SLOW);
        else if(focusedItem == 5 && isEditMode)
          ch->overrideSwitch = incDecControlSwitch(ch->overrideSwitch, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW);
        else if(focusedItem == 6 && ch->overrideSwitch != CTRL_SW_NONE)
          ch->overrideVal = incDec(ch->overrideVal, -100, 100, INCDEC_NOWRAP, INCDEC_NORMAL);
        else if(focusedItem == 7)
          ch->failsafe = incDec(ch->failsafe, -102, 100, INCDEC_NOWRAP, INCDEC_NORMAL);
        else if(focusedItem == 8)
          ch->endpointL = incDec(ch->endpointL, -100, 0, INCDEC_NOWRAP, INCDEC_NORMAL);
        else if(focusedItem == 9)
          ch->endpointR = incDec(ch->endpointR, 0, 100, INCDEC_NOWRAP, INCDEC_NORMAL);
        else if(focusedItem == 10 && isEditMode) //context menu
          changeToScreen(CONTEXT_MENU_OUTPUTS);

        //Exit
        if(heldButton == KEY_SELECT)
          changeToScreen(SCREEN_MAIN_MENU);
      }
      break;
 
    case CONTEXT_MENU_OUTPUTS:
      {
        enum {
          ITEM_CHANNEL_MONITOR,
          ITEM_RENAME_CHANNEL,
        };
        
        contextMenuInitialise();
        contextMenuAddItem(PSTR("View outputs"), ITEM_CHANNEL_MONITOR);
        contextMenuAddItem(PSTR("Rename channel"), ITEM_RENAME_CHANNEL);
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_RENAME_CHANNEL)
          changeToScreen(DIALOG_RENAME_CHANNEL);
        if(contextMenuSelectedItemID == ITEM_CHANNEL_MONITOR)
          changeToScreen(SCREEN_CHANNEL_MONITOR);
        
        if(heldButton == KEY_SELECT)
          changeToScreen(SCREEN_OUTPUTS);
      }
      break;
      
    case DIALOG_RENAME_CHANNEL:
      {
        isEditTextDialog = true;
        editTextDialog(PSTR("Channel name"), Model.Channel[thisChIdx].name, sizeof(Model.Channel[0].name), true, true, false);
        if(!isEditTextDialog) //exited
          changeToScreen(SCREEN_OUTPUTS);
      }
      break;
    
    ////////////////////////////////// EXTRAS //////////////////////////////////////////////////////
    
    case SCREEN_EXTRAS_MENU:
      {
        drawHeader(mainMenu[MAIN_MENU_EXTRAS]);
        
        static uint8_t topItem = 1, highlightedItem = 1;
        if(Model.type == MODEL_TYPE_OTHER) //### TODO better implementation. This hack hides flight mode entry.
          drawMenu(extrasMenu, sizeof(extrasMenu)/sizeof(extrasMenu[0]) - 1, NULL, &topItem, &highlightedItem);
        else
          drawMenu(extrasMenu, sizeof(extrasMenu)/sizeof(extrasMenu[0]), NULL, &topItem, &highlightedItem);
        
        //--- handle selection
        if(clickedButton == KEY_SELECT)
        {
          uint8_t selectIdx = highlightedItem - 1;
          if(selectIdx == EXTRAS_MENU_CUSTOM_CURVES) changeToScreen(SCREEN_CUSTOM_CURVES);
          if(selectIdx == EXTRAS_MENU_LOGICAL_SWITCHES) changeToScreen(SCREEN_LOGICAL_SWITCHES);
          if(selectIdx == EXTRAS_MENU_FLIGHT_MODES) changeToScreen(SCREEN_FLIGHT_MODES);
          if(selectIdx == EXTRAS_MENU_TRIM_SETUP) changeToScreen(SCREEN_TRIM_SETUP);
          if(selectIdx == EXTRAS_MENU_SAFETY_CHECKS) changeToScreen(SCREEN_SAFETY_CHECKS);
          if(selectIdx == EXTRAS_MENU_COUNTERS) changeToScreen(SCREEN_COUNTERS);
          if(selectIdx == EXTRAS_MENU_FUNCTION_GENERATORS) changeToScreen(SCREEN_FUNCTION_GENERATORS);
          if(selectIdx == EXTRAS_MENU_TIMERS) changeToScreen(SCREEN_TIMERS);
          if(selectIdx == EXTRAS_MENU_NOTIFICATION_SETUP) changeToScreen(SCREEN_NOTIFICATION_SETUP);
        }

        //Exit
        if(heldButton == KEY_SELECT)
        {
          if(!Sys.rememberMenuPosition)
          {
            topItem = 1;
            highlightedItem = 1;
          }
          changeToScreen(SCREEN_MAIN_MENU);
        }
      }
      break;
      
    case SCREEN_CUSTOM_CURVES:
      {
        drawHeader(extrasMenu[EXTRAS_MENU_CUSTOM_CURVES]);
        
        custom_curve_t *crv = &Model.CustomCurve[thisCrvIdx]; 
        
        display.setCursor(8, 9);
        display.print(F("Crv"));
        display.print(thisCrvIdx + 1);
        if(!isEmptyStr(crv->name, sizeof(crv->name)))
        {
          display.setCursor(display.getCursorX() + 6, 9);
          display.print(crv->name);
        }
        display.drawHLine(8, 17, display.getCursorX() - 9, BLACK);
        
        display.setCursor(0, 20);
        display.print(F("Type:"));
        display.setCursor(42, 20);
        display.print(crv->numPoints);
        display.print(F("pts"));
        
        display.setCursor(0, 29);
        display.print(F("Pt:"));
        display.setCursor(42, 29);
        display.write(97 + thisCrvPt);
        
        display.setCursor(0, 38);
        display.print(F("Xval:"));
        display.setCursor(42, 38);
        display.print(crv->xVal[thisCrvPt]);
        
        display.setCursor(0, 47);
        display.print(F("Yval:"));
        display.setCursor(42, 47);
        display.print(crv->yVal[thisCrvPt]);
        
        display.setCursor(0, 56);
        display.print(F("Smth:"));
        drawCheckbox(42, 56, crv->smooth);

        //draw cursor
        if(focusedItem == 1) 
          drawCursor(0, 9);
        else if(focusedItem < 7)
          drawCursor(34, (focusedItem * 9) + 2);
        
        //draw context menu icon
        display.fillRect(120, 0, 8, 7, WHITE);
        display.drawBitmap(120, 0, focusedItem == 7 ? icon_context_menu_focused : icon_context_menu, 8, 7, BLACK);
        
        //show moved input on graph
        static uint8_t movedSrc = SRC_NONE;
        bool moved = false;
        uint8_t tmpMovedSrc = getMovedSource();
        if(tmpMovedSrc != SRC_NONE)
          movedSrc = tmpMovedSrc;
        if(movedSrc != SRC_NONE)
        {
          static int8_t lastVal = 0;
          static uint32_t lastMovedTime = 0;
          int16_t difference = mixSources[movedSrc]/5 - lastVal;
          if(difference >= 5 || difference <= -5)
          {
            lastVal = mixSources[movedSrc]/5;
            lastMovedTime = millis();
          }
          if(millis() - lastMovedTime < 3000)
          {
            moved = true;
            display.setInterlace(false);
          }
          else
            movedSrc = SRC_NONE;
        }

        //draw graph
        drawCustomCurve(crv, (focusedItem >= 3 && focusedItem < 6) ? thisCrvPt : 0xff, moved ? movedSrc : (uint8_t)SRC_NONE);

        //change focus
        changeFocusOnUpDown(7);
        toggleEditModeOnSelectClicked();
        
        //---- edit params
        if(focusedItem == 1) //change to next or previous curve
        {
          uint8_t _prevCrvIdx = thisCrvIdx;
          thisCrvIdx = incDec(thisCrvIdx, 0, NUM_CUSTOM_CURVES - 1, INCDEC_WRAP, INCDEC_SLOW);
          if(thisCrvIdx != _prevCrvIdx)
            thisCrvPt = 0;
        }
        else if(focusedItem == 2) //change number of points
        {
          uint8_t prevNumPoints = crv->numPoints;
          crv->numPoints = incDec(crv->numPoints, MIN_NUM_POINTS_CUSTOM_CURVE, MAX_NUM_POINTS_CUSTOM_CURVE, 
                                          INCDEC_NOWRAP, INCDEC_SLOW);
          if(crv->numPoints != prevNumPoints) //recalculate points if changed
          {
            calcNewCurvePts(crv, prevNumPoints);
            thisCrvPt = 0;
          }
        }
        else if(focusedItem == 3) //move to next/prev point
          thisCrvPt = incDec(thisCrvPt, 0, crv->numPoints - 1, INCDEC_WRAP, INCDEC_SLOW);
        else if(focusedItem == 4 && isEditMode) //edit x values
        {
          if(thisCrvPt > 0 && thisCrvPt < crv->numPoints - 1) //only edit inner x values
          {
            int8_t _minVal = crv->xVal[thisCrvPt - 1];
            int8_t _maxVal = crv->xVal[thisCrvPt + 1];
            crv->xVal[thisCrvPt] = incDec(crv->xVal[thisCrvPt], _minVal, _maxVal, INCDEC_NOWRAP, INCDEC_NORMAL);
          }
        }
        else if(focusedItem == 5) //edit Y values
          crv->yVal[thisCrvPt] = incDec(crv->yVal[thisCrvPt], -100, 100, INCDEC_NOWRAP, INCDEC_NORMAL);
        else if(focusedItem == 6) //smoothing
          crv->smooth = incDec(crv->smooth, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        else if(focusedItem == 7 && isEditMode) //context menu
          changeToScreen(CONTEXT_MENU_CUSTOM_CURVE);

        ////// Exit
        if(heldButton == KEY_SELECT)
        {
          changeToScreen(SCREEN_EXTRAS_MENU);
          thisCrvPt = 0;
        }
      }
      break;

    case CONTEXT_MENU_CUSTOM_CURVE:
      {
        enum {
          ITEM_COPY_CURVE,
          ITEM_RENAME_CURVE,
          ITEM_RESET_CURVE,
          ITEM_FLIP_HORIZONTAL,
          ITEM_FLIP_VERTICAL,
          ITEM_INSERT_POINT,
          ITEM_DELETE_POINT
        };
        
        contextMenuInitialise();
        contextMenuAddItem(PSTR("Copy curve to"), ITEM_COPY_CURVE);
        contextMenuAddItem(PSTR("Rename curve"), ITEM_RENAME_CURVE);
        contextMenuAddItem(PSTR("Reset curve"), ITEM_RESET_CURVE);
        contextMenuAddItem(PSTR("Flip horizontal"), ITEM_FLIP_HORIZONTAL);
        contextMenuAddItem(PSTR("Flip vertical"), ITEM_FLIP_VERTICAL);
        contextMenuAddItem(PSTR("Insert point"), ITEM_INSERT_POINT);
        contextMenuAddItem(PSTR("Delete point"), ITEM_DELETE_POINT);
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_COPY_CURVE)
        {
          destCrvIdx = thisCrvIdx;
          changeToScreen(DIALOG_COPY_CUSTOM_CURVE);
        }
        if(contextMenuSelectedItemID == ITEM_RENAME_CURVE)
          changeToScreen(DIALOG_RENAME_CUSTOM_CURVE);
        if(contextMenuSelectedItemID == ITEM_RESET_CURVE)
        {
          resetCustomCurveParams(thisCrvIdx);
          changeToScreen(SCREEN_CUSTOM_CURVES);
          thisCrvPt = 0;
        }
        if(contextMenuSelectedItemID == ITEM_FLIP_HORIZONTAL)
        {
          //Reflect about y axis.
          //If say a point M(h,k), then M'(-h,k). This however affects the order of the points so 
          //we have to counter this by reversing the arrays.
          custom_curve_t *crv = &Model.CustomCurve[thisCrvIdx];
          for(uint8_t i = 0; i < crv->numPoints; i++)
            crv->xVal[i] = 0 - crv->xVal[i];
          //reverse the arrays
          uint8_t start = 0;
          uint8_t end = crv->numPoints - 1;
          while(start < end)
          {
            int8_t tmp = crv->xVal[start];
            crv->xVal[start] = crv->xVal[end];
            crv->xVal[end] = tmp;
            tmp = crv->yVal[start];
            crv->yVal[start] = crv->yVal[end];
            crv->yVal[end] = tmp;
            start++;
            end--;
          }
          changeToScreen(SCREEN_CUSTOM_CURVES);
          thisCrvPt = 0;
        }
        if(contextMenuSelectedItemID == ITEM_FLIP_VERTICAL)
        {
          //Reflect about x axis.
          //If say a point M(h,k), then M'(h,-k)
          custom_curve_t *crv = &Model.CustomCurve[thisCrvIdx];
          for(uint8_t i = 0; i < crv->numPoints; i++)
            crv->yVal[i] = 0 - crv->yVal[i];
          changeToScreen(SCREEN_CUSTOM_CURVES);
          thisCrvPt = 0;
        }
        if(contextMenuSelectedItemID == ITEM_INSERT_POINT)
        {
          if(Model.CustomCurve[thisCrvIdx].numPoints == MAX_NUM_POINTS_CUSTOM_CURVE)
          {
            makeToast(PSTR("Maximum pts reached"), 2000, 0);
            changeToScreen(SCREEN_CUSTOM_CURVES);
          }
          else 
            changeToScreen(DIALOG_INSERT_CURVE_POINT);
        }
        if(contextMenuSelectedItemID == ITEM_DELETE_POINT)
        {
          if(Model.CustomCurve[thisCrvIdx].numPoints == MIN_NUM_POINTS_CUSTOM_CURVE)
          {
            makeToast(PSTR("Minimum pts reached"), 2000, 0);
            changeToScreen(SCREEN_CUSTOM_CURVES);
          }
          else 
            changeToScreen(DIALOG_DELETE_CURVE_POINT);
        }
        
        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_CUSTOM_CURVES);
      }
      break;
      
    case DIALOG_RENAME_CUSTOM_CURVE:
      {
        isEditTextDialog = true;
        editTextDialog(PSTR("Curve name"), Model.CustomCurve[thisCrvIdx].name, sizeof(Model.CustomCurve[0].name), 
                       true, true, false);
        if(!isEditTextDialog) //exited
          changeToScreen(SCREEN_CUSTOM_CURVES);
      }
      break;
      
    case DIALOG_COPY_CUSTOM_CURVE:
      {
        isEditMode = true;
        destCrvIdx = incDec(destCrvIdx, 0, NUM_CUSTOM_CURVES - 1, INCDEC_WRAP, INCDEC_SLOW);
        drawDialogCopyMove(PSTR("Curve"), thisCrvIdx, destCrvIdx, true);
        if(clickedButton == KEY_SELECT)
        {
          Model.CustomCurve[destCrvIdx] = Model.CustomCurve[thisCrvIdx]; //copy struct
          thisCrvIdx = destCrvIdx;
          changeToScreen(SCREEN_CUSTOM_CURVES);
          thisCrvPt = 0;
        }
        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_CUSTOM_CURVES);
      }
      break;
      
    case DIALOG_INSERT_CURVE_POINT:
      {
        custom_curve_t *crv = &Model.CustomCurve[thisCrvIdx];
        
        static bool initialised = false;
        static uint8_t insertionPt = 1; //Point b
        if(!initialised)
        {
          initialised = true;
          if(thisCrvPt >= 1 && thisCrvPt <= crv->numPoints - 1)
            insertionPt = thisCrvPt;  
          else
            insertionPt = 1; 
        }
        
        drawBoundingBox(11, 14, 105, 35,BLACK);
        display.setCursor(17, 18);
        display.print(F("Insert point"));
        display.setCursor(17, 28);
        display.print(F("before:"));
        display.setCursor(71, 28);
        display.write(97 + insertionPt);
        drawCursor(63, 28);
        
        isEditMode = true;
        insertionPt = incDec(insertionPt, 1, crv->numPoints - 1, INCDEC_WRAP, INCDEC_SLOW);
        
        if(clickedButton == KEY_SELECT)
        {
          //calculate x and y values for the new point
          //For non-smooth curve, its simply the midpoint. If the curve has smoothing enabled,
          //use cubic interpolation to get the new value for y, as long as the xVal are different.
          int8_t xNew = ((int16_t)crv->xVal[insertionPt - 1] + crv->xVal[insertionPt]) / 2;
          int8_t yNew = ((int16_t)crv->yVal[insertionPt - 1] + crv->yVal[insertionPt]) / 2;
          if(crv->smooth && (crv->xVal[insertionPt - 1] != crv->xVal[insertionPt]))
          {
            int16_t xQQ[MAX_NUM_POINTS_CUSTOM_CURVE];
            int16_t yQQ[MAX_NUM_POINTS_CUSTOM_CURVE];
            for(uint8_t pt = 0; pt < crv->numPoints; pt++)
            {
              xQQ[pt] = 5 * crv->xVal[pt];
              yQQ[pt] = 5 * crv->yVal[pt];
            }
            yNew = cubicHermiteInterpolate(xQQ, yQQ, crv->numPoints, xNew * 5) / 5;
          }
          //shift array elements right and write the new value at the insertionPt
          uint8_t i = MAX_NUM_POINTS_CUSTOM_CURVE - 1;
          while(i > insertionPt)
          {
            crv->xVal[i] = crv->xVal[i-1];
            crv->yVal[i] = crv->yVal[i-1];
            i--;
          }
          crv->xVal[insertionPt] = xNew;
          crv->yVal[insertionPt] = yNew;
          //update point count
          crv->numPoints += 1;
          //exit
          initialised = false;
          changeToScreen(SCREEN_CUSTOM_CURVES);
          thisCrvPt = 0;
        }
        
        if(heldButton == KEY_SELECT) //exit
        {
          initialised = false;
          changeToScreen(SCREEN_CUSTOM_CURVES);
        }
      }
      break;
      
    case DIALOG_DELETE_CURVE_POINT:
      {
        custom_curve_t *crv = &Model.CustomCurve[thisCrvIdx];
        
        static bool initialised = false;
        static uint8_t deletePt = 1; //Point b
        if(!initialised)
        {
          initialised = true;
          if(thisCrvPt >= 1 && thisCrvPt <= crv->numPoints - 2)
            deletePt = thisCrvPt; 
          else
            deletePt = 1;
        }

        drawBoundingBox(11, 14, 105, 35,BLACK);
        display.setCursor(17, 18);
        display.print(F("Delete point"));
        display.setCursor(17, 28);
        display.print(F("Point: "));
        display.setCursor(71, 28);
        display.write(97 + deletePt);
        drawCursor(63, 28);
        
        //Only allow 'deleting' of interior points. We don't technically delete but simply
        //move the element to the end and decrement numPoints by one.
        
        isEditMode = true;
        deletePt = incDec(deletePt, 1, crv->numPoints - 2, INCDEC_WRAP, INCDEC_SLOW);
        
        if(clickedButton == KEY_SELECT)
        {
          //shift all elements that are after the deletionIdx to the left of the deletion Idx
          for(uint8_t i = deletePt; i < MAX_NUM_POINTS_CUSTOM_CURVE - 1; i++) 
          {
            crv->xVal[i] = crv->xVal[i+1];
            crv->yVal[i] = crv->yVal[i+1];
          }
          //update point count
          crv->numPoints -= 1;
          //exit
          initialised = false;
          changeToScreen(SCREEN_CUSTOM_CURVES);
          thisCrvPt = 0;
        }
        
        if(heldButton == KEY_SELECT) //exit
        {
          initialised = false;
          changeToScreen(SCREEN_CUSTOM_CURVES);
        }
      }
      break;

    case SCREEN_LOGICAL_SWITCHES:
      {
        display.setInterlace(false); 
        drawHeader(extrasMenu[EXTRAS_MENU_LOGICAL_SWITCHES]);

        logical_switch_t *ls = &Model.LogicalSwitch[thisLsIdx];
        
        //draw title
        display.setCursor(8, 9);
        getSrcName(txtBuff, SRC_SW_LOGICAL_FIRST + thisLsIdx, sizeof(txtBuff));
        display.print(txtBuff);
        display.drawHLine(8, 17, display.getCursorX() - 9, BLACK);
        
        //draw switch icon
        if(checkSwitchCondition(CTRL_SW_LOGICAL_FIRST + thisLsIdx))
          display.drawBitmap(115, 9, icon_switch_is_on, 13, 8, BLACK);
        else
          display.drawBitmap(115, 9, icon_switch_is_off, 13, 8, BLACK);
        
        //-- dynamic scrollable list
        
        enum {
          ITEM_LS_FUNC,
          ITEM_LS_VAL1,
          ITEM_LS_VAL2,
          ITEM_LS_VAL3,
          ITEM_LS_VAL4,
          
          ITEM_COUNT
        };
        
        uint8_t listItemIDs[ITEM_COUNT]; 
        uint8_t listItemCount = 0;
        
        //add item Ids to the list of Ids
        for(uint8_t i = 0; i < sizeof(listItemIDs); i++)
        {
          if(i == ITEM_LS_VAL4 && (ls->func == LS_FUNC_LATCH || ls->func == LS_FUNC_TOGGLE || ls->func == LS_FUNC_PULSE))
            continue;
          listItemIDs[listItemCount++] = i;
        }
        
        //handle navigation
        uint8_t numFocusable = listItemCount + 2; //+1 for title focus, +1 for context menu focus
        changeFocusOnUpDown(numFocusable); 
        toggleEditModeOnSelectClicked();
        static uint8_t topItem = 1;
        if(focusedItem == 1 || focusedItem == numFocusable) //title focus or context menu focus
          topItem = 1;
        else if(focusedItem > 1)
        {
          if(focusedItem - 1 < topItem)
            topItem = focusedItem - 1;
          while(focusedItem - 1 >= topItem + 5)
            topItem++;
        }
        
        //fill list and edit items
        for(uint8_t line = 0; line < 5 && line < listItemCount; line++)
        {
          uint8_t ypos = 20 + line*9;
          if(focusedItem - 1 == topItem + line)
            drawCursor(52, ypos);
          
          uint8_t itemID = listItemIDs[topItem - 1 + line];
          bool edit = (itemID == listItemIDs[focusedItem - 2] && isEditMode) ? true : false;
          
          display.setCursor(0, ypos);
          switch(itemID)
          {
            case ITEM_LS_FUNC:
              {
                display.print(F("Functn:"));
                display.setCursor(60, ypos);
                if(ls->func == LS_FUNC_NONE)
                  display.print(F("--"));
                else if(ls->func == LS_FUNC_ABS_DELTA_GREATER_THAN_X)
                  display.print(F("|\xEB|>x"));
                else
                  display.print(findStringInIdStr(enum_LogicalSwitch_Func, ls->func));
                
                if(edit)
                {
                  uint8_t oldGroup = getLSFuncGroup(ls->func);
                  ls->func = incDec(ls->func, 0, LS_FUNC_COUNT - 1, INCDEC_NOWRAP, INCDEC_SLOW);
                  uint8_t newGroup = getLSFuncGroup(ls->func);
                  if(newGroup != oldGroup) //reset values if group changed
                  {
                    ls->val3 = 0;
                    ls->val4 = 0;
                    if(ls->func <= LS_FUNC_GROUP3_LAST)     
                    {
                      ls->val1 = SRC_NONE; 
                      ls->val2 = 0; 
                      if(ls->func == LS_FUNC_ABS_DELTA_GREATER_THAN_X) 
                        ls->val3 = 2;
                    }
                    else if(ls->func <= LS_FUNC_GROUP4_LAST)
                    {
                      ls->val1 = SRC_NONE; 
                      ls->val2 = SRC_NONE;
                    }
                    else if(ls->func <= LS_FUNC_GROUP5_LAST)
                    {
                      ls->val1 = CTRL_SW_NONE; 
                      ls->val2 = CTRL_SW_NONE;
                    }
                    else if(ls->func == LS_FUNC_TOGGLE) 
                    {
                      ls->val1 = CTRL_SW_NONE; 
                      ls->val2 = 0; 
                      ls->val3 = CTRL_SW_NONE;
                    }
                    else if(ls->func == LS_FUNC_PULSE)
                    {
                      ls->val1 = 5; 
                      ls->val2 = 10;
                    }
                  }
                }
              }
              break;
              
            case ITEM_LS_VAL1:
              {
                if(ls->func == LS_FUNC_PULSE) display.print(F("Width:"));
                else if(ls->func == LS_FUNC_LATCH) display.print(F("Set:"));
                else if(ls->func == LS_FUNC_TOGGLE) display.print(F("Clock:"));
                else display.print(F("Value1:"));
                
                display.setCursor(60, ypos);
                
                if(ls->func == LS_FUNC_NONE)
                  display.print(F("--"));
                else if(ls->func <= LS_FUNC_GROUP4_LAST)
                {
                  if(ls->val1 == SRC_NONE)
                    display.print(F("--"));
                  else
                  {
                    getSrcName(txtBuff, ls->val1, sizeof(txtBuff));
                    display.print(txtBuff);
                  }
                }
                else if(ls->func <= LS_FUNC_GROUP5_LAST || ls->func == LS_FUNC_TOGGLE)
                {
                  getControlSwitchName(txtBuff, ls->val1, sizeof(txtBuff));
                  display.print(txtBuff);
                }
                else if(ls->func == LS_FUNC_PULSE)
                  printSeconds(ls->val1);
                
                if(edit && ls->func != LS_FUNC_NONE)
                {
                  if(ls->func <= LS_FUNC_GROUP3_LAST)
                  {
                    //auto detect moved source
                    uint8_t movedSrc = procMovedSource(getMovedSource());
                    if(movedSrc != SRC_NONE)
                      ls->val1 = movedSrc;
                    //inc dec
                    ls->val1 = incDecSource(ls->val1, INCDEC_FLAG_MIX_SRC | INCDEC_FLAG_TELEM_SRC | INCDEC_FLAG_COUNTER_SRC | INCDEC_FLAG_TIMER_SRC);
                    //reset ls->val2 if out of range
                    if(ls->val1 < MIXSOURCES_COUNT)
                    {
                      if(ls->val2 > 100 || ls->val2 < -100)
                        ls->val2 = 0;
                    }
                    else if(ls->val1 < MIXSOURCES_COUNT + NUM_COUNTERS)
                    {
                      if(ls->val2 > 9999 || ls->val2 < 0)
                        ls->val2 = 0;
                    }
                    else if(ls->val1 < MIXSOURCES_COUNT + NUM_COUNTERS + NUM_TIMERS)
                    {
                      if(ls->val2 < 0)
                        ls->val2 = 0;
                    }
                  }
                  else if(ls->func <= LS_FUNC_GROUP4_LAST)
                  {
                    //auto detect moved source
                    uint8_t movedSrc = procMovedSource(getMovedSource());
                    if(movedSrc != SRC_NONE)
                      ls->val1 = movedSrc;
                    //inc dec
                    ls->val1 = incDecSource(ls->val1, INCDEC_FLAG_MIX_SRC);
                  }
                  else if(ls->func <= LS_FUNC_GROUP5_LAST || ls->func == LS_FUNC_TOGGLE)
                  {
                    if(Model.type == MODEL_TYPE_AIRPLANE || Model.type == MODEL_TYPE_MULTICOPTER)
                      ls->val1 = incDecControlSwitch(ls->val1, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW | INCDEC_FLAG_FMODE);
                    else
                      ls->val1 = incDecControlSwitch(ls->val1, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW);
                  }
                  else if(ls->func == LS_FUNC_PULSE)
                    ls->val1 = incDec(ls->val1, 1, ls->val2 - 1, INCDEC_NOWRAP, INCDEC_SLOW_START);
                }
              }
              break;
            
            case ITEM_LS_VAL2:
              {
                if(ls->func == LS_FUNC_PULSE) display.print(F("Period:"));
                else if(ls->func == LS_FUNC_LATCH) display.print(F("Reset:"));
                else if(ls->func == LS_FUNC_TOGGLE) display.print(F("Edge:"));
                else display.print(F("Value2:"));
                
                display.setCursor(60, ypos);
                if(ls->func == LS_FUNC_NONE)
                  display.print(F("--"));
                else if(ls->func <= LS_FUNC_GROUP3_LAST)
                {
                  if(ls->val1 < MIXSOURCES_COUNT + NUM_COUNTERS) //mixsource value, counter value
                    display.print(ls->val2);
                  else if(ls->val1 < MIXSOURCES_COUNT + NUM_COUNTERS + NUM_TIMERS) //timer
                    printHHMMSS((int32_t)ls->val2 * 1000);
                  else //telemetry value
                  {
                    uint8_t idx = ls->val1 - (MIXSOURCES_COUNT + NUM_COUNTERS + NUM_TIMERS);
                    printTelemParam(display.getCursorX(), display.getCursorY(), idx, ls->val2);
                    display.print(Model.Telemetry[idx].unitsName);
                  }
                }
                else if(ls->func <= LS_FUNC_GROUP4_LAST)
                {
                  if(ls->val2 == SRC_NONE)
                    display.print(F("--"));
                  else
                  {
                    getSrcName(txtBuff, ls->val2, sizeof(txtBuff));
                    display.print(txtBuff);
                  }
                }
                else if(ls->func <= LS_FUNC_GROUP5_LAST)
                {
                  getControlSwitchName(txtBuff, ls->val2, sizeof(txtBuff));
                  display.print(txtBuff);
                }
                else if(ls->func == LS_FUNC_TOGGLE)
                  display.print(findStringInIdStr(enum_ClockEdge, ls->val2));
                else if(ls->func == LS_FUNC_PULSE)
                  printSeconds(ls->val2);

                if(edit && ls->func != LS_FUNC_NONE)
                {
                  if(ls->func <= LS_FUNC_GROUP3_LAST)
                  {
                    if(ls->val1 < MIXSOURCES_COUNT)
                      ls->val2 = incDec(ls->val2, (ls->func <= LS_FUNC_GROUP1_LAST) ? -100 : 0, 100, INCDEC_NOWRAP, INCDEC_NORMAL);
                    else if(ls->val1 < MIXSOURCES_COUNT + NUM_COUNTERS) //counter value
                      ls->val2 = incDec(ls->val2, 0, 9999, INCDEC_NOWRAP, INCDEC_FAST);
                    else if(ls->val1 < MIXSOURCES_COUNT + NUM_COUNTERS + NUM_TIMERS) //timer value
                      ls->val2 = incDec(ls->val2, 0, 30000, INCDEC_NOWRAP, INCDEC_FAST);
                    else //telemetry value
                      ls->val2 = incDec(ls->val2, (ls->func <= LS_FUNC_GROUP1_LAST) ? -30000 : 0, 30000, INCDEC_NOWRAP, INCDEC_FAST);
                  }
                  else if(ls->func <= LS_FUNC_GROUP4_LAST)
                  {
                    //auto detect moved source
                    uint8_t movedSrc = procMovedSource(getMovedSource());
                    if(movedSrc != SRC_NONE)
                      ls->val2 = movedSrc;
                    //inc dec
                    ls->val2 = incDecSource(ls->val2, INCDEC_FLAG_MIX_SRC);
                  }
                  else if(ls->func <= LS_FUNC_GROUP5_LAST)
                  {
                    if(Model.type == MODEL_TYPE_AIRPLANE || Model.type == MODEL_TYPE_MULTICOPTER)
                      ls->val2 = incDecControlSwitch(ls->val2, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW | INCDEC_FLAG_FMODE);
                    else
                      ls->val2 = incDecControlSwitch(ls->val2, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW);
                  }
                  else if(ls->func == LS_FUNC_TOGGLE) //change edge type
                    ls->val2 = incDec(ls->val2, 0, 2, INCDEC_WRAP, INCDEC_SLOW);
                  else if(ls->func == LS_FUNC_PULSE) //adjust period
                    ls->val2 = incDec(ls->val2, ls->val1 + 1, 600, INCDEC_NOWRAP, INCDEC_SLOW_START);
                }
              }
              break;
              
            case ITEM_LS_VAL3:
              {
                if(ls->func == LS_FUNC_TOGGLE)
                {
                  display.print(F("Clear:"));
                  display.setCursor(60, ypos);
                  getControlSwitchName(txtBuff, ls->val3, sizeof(txtBuff));
                  display.print(txtBuff);
                }
                else if(ls->func == LS_FUNC_ABS_DELTA_GREATER_THAN_X)
                {
                  display.print(F("Dirctn:"));
                  display.setCursor(60, ypos);
                  display.print(findStringInIdStr(enum_DirectionOfChange, ls->val3));
                }
                else
                {
                  display.print(F("Delay:"));
                  display.setCursor(60, ypos);
                  if(ls->val3 == 0 || ls->func == LS_FUNC_NONE)
                    display.print(F("--"));
                  else
                    printSeconds(ls->val3);
                }

                if(edit && ls->func != LS_FUNC_NONE)
                {
                  if(ls->func == LS_FUNC_TOGGLE)
                  {
                    if(Model.type == MODEL_TYPE_AIRPLANE || Model.type == MODEL_TYPE_MULTICOPTER)
                      ls->val3 = incDecControlSwitch(ls->val3, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW | INCDEC_FLAG_FMODE);
                    else
                      ls->val3 = incDecControlSwitch(ls->val3, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW);
                  }
                  else if(ls->func == LS_FUNC_ABS_DELTA_GREATER_THAN_X)
                    ls->val3 = incDec(ls->val3, 0, 2, INCDEC_WRAP, INCDEC_SLOW);
                  else //adjust delay
                    ls->val3 = incDec(ls->val3, 0, 600, INCDEC_NOWRAP, INCDEC_SLOW_START);
                }
              }
              break;
              
            case ITEM_LS_VAL4:
              {
                display.print(F("Duratn:"));
                display.setCursor(60, ypos);
                if(ls->val4 == 0 || ls->func == LS_FUNC_NONE)
                  display.print(F("--"));
                else
                  printSeconds(ls->val4);
                
                if(edit && ls->func != LS_FUNC_NONE)
                {
                  ls->val4 = incDec(ls->val4, 0, 600, INCDEC_NOWRAP, INCDEC_SLOW_START);
                }
              }
              break;
          }
        }
        
        //scrollbar
        drawScrollBar(127, 19, listItemCount, topItem, 5, 5 * 9);
        
        //draw context menu icon
        display.fillRect(120, 0, 8, 7, WHITE);
        display.drawBitmap(120, 0, focusedItem == numFocusable ? icon_context_menu_focused : icon_context_menu, 8, 7, BLACK);

        //open context menu
        if(focusedItem == numFocusable && isEditMode)
          changeToScreen(CONTEXT_MENU_LOGICAL_SWITCHES);
        
        //change to next logical switch
        if(focusedItem == 1)
        {          
          drawCursor(0, 9);
          thisLsIdx = incDec(thisLsIdx, 0, NUM_LOGICAL_SWITCHES - 1, INCDEC_WRAP, INCDEC_SLOW);
        }

        // Exit
        if(heldButton == KEY_SELECT)
          changeToScreen(SCREEN_EXTRAS_MENU);
      }
      break;
      
    case CONTEXT_MENU_LOGICAL_SWITCHES:
      {
        enum {
          ITEM_VIEW_OUTPUTS,
          ITEM_COPY_TO,
          ITEM_RESET
        };
        
        contextMenuInitialise();
        contextMenuAddItem(PSTR("View outputs"), ITEM_VIEW_OUTPUTS);
        contextMenuAddItem(PSTR("Copy to"), ITEM_COPY_TO);
        contextMenuAddItem(PSTR("Reset settings"), ITEM_RESET);
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_VIEW_OUTPUTS)
          changeToScreen(SCREEN_LOGICAL_SWITCH_OUTPUTS);
        if(contextMenuSelectedItemID == ITEM_COPY_TO)
        {
          destLsIdx = thisLsIdx;
          changeToScreen(DIALOG_COPY_LOGICAL_SWITCH);
        }
        if(contextMenuSelectedItemID == ITEM_RESET)
        {
          resetLogicalSwitchParams(thisLsIdx);
          changeToScreen(SCREEN_LOGICAL_SWITCHES);
        }

        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_LOGICAL_SWITCHES);
      }
      break;
    
    case DIALOG_COPY_LOGICAL_SWITCH:
      {
        isEditMode = true;
        destLsIdx = incDec(destLsIdx, 0, NUM_LOGICAL_SWITCHES - 1, INCDEC_WRAP, INCDEC_SLOW);
        drawDialogCopyMove(PSTR("L"), thisLsIdx, destLsIdx, true);
        if(clickedButton == KEY_SELECT)
        {
          Model.LogicalSwitch[destLsIdx] = Model.LogicalSwitch[thisLsIdx];
          thisLsIdx = destLsIdx;
          changeToScreen(SCREEN_LOGICAL_SWITCHES);
        }
        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_LOGICAL_SWITCHES);
      }
      break;

    case SCREEN_LOGICAL_SWITCH_OUTPUTS:
      {
        display.setInterlace(false); 
        drawHeader(PSTR("Logical sw outputs"));
        
        //scrollable 
        uint8_t numPages = (NUM_LOGICAL_SWITCHES + 9) / 10;
        static uint8_t thisPage = 1;
        
        static bool viewInitialised = false;
        if(!viewInitialised) 
        {
          //start in the page that has the switch we want to view
          thisPage = (thisLsIdx + 10) / 10;
          viewInitialised = true;
        }

        isEditMode = true;
        thisPage = incDec(thisPage, numPages, 1, INCDEC_WRAP, INCDEC_SLOW);
        
        uint8_t startIdx = (thisPage - 1) * 10;
        for(uint8_t i = startIdx; i < startIdx + 10 && i < NUM_LOGICAL_SWITCHES; i++)
        {
          if((i - startIdx) < 5)
          {
            if(i == thisLsIdx)
            {
              display.setCursor(0, 10 + (i - startIdx) * 11);
              display.write(0xB1);
            }
            display.setCursor(6, 10 + (i - startIdx) * 11);
          }
          else
          { 
            if(i == thisLsIdx)
            {
              display.setCursor(65, 10 + (i - (startIdx + 5)) * 11);
              display.write(0xB1);
            }        
            display.setCursor(71, 10 + (i - (startIdx + 5)) * 11);
          }
          display.print(F("L"));          
          display.print(1 + i);  
          display.print(F(":"));
          if(i < 9)
            display.print(F(" "));

          if(checkSwitchCondition(CTRL_SW_LOGICAL_FIRST + i))
            display.drawBitmap(display.getCursorX(), display.getCursorY(), icon_switch_is_on, 13, 8, BLACK);
          else
            display.drawBitmap(display.getCursorX(), display.getCursorY(), icon_switch_is_off, 13, 8, BLACK);
        }
        //show scrollbar
        drawScrollBar(127, 9, numPages, thisPage, 1, 1 * 54);

        if(heldButton == KEY_SELECT)
        {
          viewInitialised = false;
          changeToScreen(SCREEN_LOGICAL_SWITCHES);
        }
      }
      break;
      
    case SCREEN_FUNCTION_GENERATORS:
      {
        drawHeader(extrasMenu[EXTRAS_MENU_FUNCTION_GENERATORS]);
        
        funcgen_t *fgen = &Model.Funcgen[thisFgenIdx];
        
        if(fgen->periodMode == FUNCGEN_PERIODMODE_VARIABLE && fgen->period1 >= fgen->period2)
        {
          if(fgen->period1 >= 600)
            fgen->period1 = 599;
          fgen->period2 = fgen->period1 + 1;
        }
        
        display.setCursor(8, 9);
        getSrcName(txtBuff, SRC_FUNCGEN_FIRST + thisFgenIdx, sizeof(txtBuff));
        display.print(txtBuff);
        display.drawHLine(8, 17, display.getCursorX() - 9, BLACK);
        
        enum {
          ITEM_WAVEFORM,
          ITEM_PERIOD_MODE,
          ITEM_PERIOD1,
          ITEM_PERIOD2,
          ITEM_MODULATOR_SRC,
          ITEM_INVERT_MODULATOR,
          ITEM_PHASE_MODE,
          ITEM_PHASE,
          
          ITEM_COUNT
        };
        
        uint8_t listItemIDs[ITEM_COUNT]; 
        uint8_t listItemCount = 0;
        
        //add item Ids to the list of Ids
        for(uint8_t i = 0; i < sizeof(listItemIDs); i++)
        {
          if(fgen->periodMode == FUNCGEN_PERIODMODE_FIXED)
          {
            if(i == ITEM_PERIOD2 || i == ITEM_MODULATOR_SRC || i == ITEM_INVERT_MODULATOR)
              continue;
          }
          if(fgen->periodMode == FUNCGEN_PERIODMODE_VARIABLE || fgen->waveform == FUNCGEN_WAVEFORM_RANDOM)
          {
            if(i == ITEM_PHASE_MODE || i == ITEM_PHASE)
              continue;
          }
          listItemIDs[listItemCount++] = i;
        }
        
        //handle navigation
        uint8_t numFocusable = listItemCount + 2; //+1 for title focus, +1 for context menu focus
        changeFocusOnUpDown(numFocusable); 
        toggleEditModeOnSelectClicked();
        static uint8_t topItem = 1;
        if(focusedItem == 1 || focusedItem == numFocusable) //title focus or context menu focus
          topItem = 1;
        else if(focusedItem > 1)
        {
          if(focusedItem - 1 < topItem)
            topItem = focusedItem - 1;
          while(focusedItem - 1 >= topItem + 5)
            topItem++;
        }
        
        //fill list and edit items
        for(uint8_t line = 0; line < 5 && line < listItemCount; line++)
        {
          uint8_t ypos = 20 + line*9;
          if(focusedItem - 1 == topItem + line)
            drawCursor(70, ypos);
          
          uint8_t itemID = listItemIDs[topItem - 1 + line];
          bool edit = (itemID == listItemIDs[focusedItem - 2] && isEditMode) ? true : false;
          
          display.setCursor(0, ypos);
          switch(itemID)
          {
            case ITEM_WAVEFORM:
              {
                display.print(F("Waveform:"));
                display.setCursor(78, ypos);
                display.print(findStringInIdStr(enum_FuncgenWaveform, fgen->waveform));
                if(edit)
                  fgen->waveform = incDec(fgen->waveform, 0, FUNCGEN_WAVEFORM_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);
              }
              break;
              
            case ITEM_PERIOD_MODE:
              {
                if(fgen->waveform == FUNCGEN_WAVEFORM_RANDOM) 
                  display.print(F("Intrvl mode:"));
                else 
                  display.print(F("Period mode:"));
                display.setCursor(78, ypos);
                display.print(findStringInIdStr(enum_FuncgenPeriodMode, fgen->periodMode));
                if(edit)
                  fgen->periodMode = incDec(fgen->periodMode, 0, FUNCGEN_PERIODMODE_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);
              }
              break;
              
            case ITEM_PERIOD1:
              {
                if(fgen->waveform == FUNCGEN_WAVEFORM_RANDOM)
                {
                  if(fgen->periodMode == FUNCGEN_PERIODMODE_FIXED)
                    display.print(F("Interval:"));
                  else
                    display.print(F("Min intrvl:"));
                }
                else
                {
                  if(fgen->periodMode == FUNCGEN_PERIODMODE_FIXED)
                    display.print(F("Period:"));
                  else
                    display.print(F("Min period"));
                }
                display.setCursor(78, ypos);
                printSeconds(fgen->period1);
                if(edit)
                {
                  uint16_t max = 600;
                  if(fgen->waveform != FUNCGEN_WAVEFORM_RANDOM && fgen->periodMode == FUNCGEN_PERIODMODE_VARIABLE)
                    max = fgen->period2 - 1;
                  fgen->period1 = incDec(fgen->period1, 1, max, INCDEC_NOWRAP, INCDEC_SLOW_START);
                }
              }
              break;
              
            case ITEM_PERIOD2:
              {
                if(fgen->waveform == FUNCGEN_WAVEFORM_RANDOM)
                  display.print(F("Max intrvl:"));
                else
                  display.print(F("Max period:"));
                display.setCursor(78, ypos);
                printSeconds(fgen->period2);
                if(edit)
                  fgen->period2 = incDec(fgen->period2, fgen->period1 + 1, 600, INCDEC_NOWRAP, INCDEC_SLOW_START);
              }
              break;
            
            case ITEM_MODULATOR_SRC:
              {
                display.print(F("Control:"));
                display.setCursor(78, ypos);
                getSrcName(txtBuff, fgen->modulatorSrc, sizeof(txtBuff));
                display.print(txtBuff);
                if(edit)
                {
                  uint8_t movedSrc = procMovedSource(getMovedSource());
                  if(movedSrc != SRC_NONE)
                    fgen->modulatorSrc = movedSrc;
                  //inc dec
                  fgen->modulatorSrc = incDecSource(fgen->modulatorSrc, INCDEC_FLAG_MIX_SRC);
                }
              }
              break;
              
            case ITEM_INVERT_MODULATOR:
              {
                display.print(F("Reverse:"));
                drawCheckbox(78, ypos, fgen->reverseModulator);
                if(edit)
                  fgen->reverseModulator = incDec(fgen->reverseModulator, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
              }
              break;
            
            case ITEM_PHASE_MODE:
              {
                display.print(F("Phase mode:"));
                display.setCursor(78, ypos);
                display.print(findStringInIdStr(enum_FuncgenPhaseMode, fgen->phaseMode));
                if(edit)
                  fgen->phaseMode = incDec(fgen->phaseMode, 0, FUNCGEN_PHASEMODE_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);
              }
              break;
              
            case ITEM_PHASE:
              {
                display.print(F("Phase:"));
                display.setCursor(78, ypos);
                if(fgen->phaseMode == FUNCGEN_PHASEMODE_AUTO)
                  display.print(F("[Sync]"));
                else if(fgen->phaseMode == FUNCGEN_PHASEMODE_FIXED)
                {
                  display.print(fgen->phaseAngle);
                  display.write(248);
                }
                
                if(edit)
                {
                  if(fgen->phaseMode == FUNCGEN_PHASEMODE_AUTO)
                  {
                    syncWaveform(thisFgenIdx);
                    makeToast(PSTR("Synced"), 2000, 0);
                    isEditMode = false;
                  }
                  else if(fgen->phaseMode == FUNCGEN_PHASEMODE_FIXED)
                    fgen->phaseAngle = incDec(fgen->phaseAngle, 0, 360, INCDEC_NOWRAP, INCDEC_NORMAL);
                }
              }
              break;
          }
        }
        
        //scrollbar
        drawScrollBar(127, 19, listItemCount, topItem, 5, 5 * 9);
        
        //draw context menu icon
        display.fillRect(120, 0, 8, 7, WHITE);
        display.drawBitmap(120, 0, focusedItem == numFocusable ? icon_context_menu_focused : icon_context_menu, 8, 7, BLACK);

        //open context menu
        if(focusedItem == numFocusable && isEditMode)
          changeToScreen(CONTEXT_MENU_FUNCGEN);
        
        //change to next function generator
        if(focusedItem == 1)
        {          
          drawCursor(0, 9);
          thisFgenIdx = incDec(thisFgenIdx, 0, NUM_FUNCGEN - 1, INCDEC_WRAP, INCDEC_SLOW);
        }

        //exit
        if(heldButton == KEY_SELECT)
          changeToScreen(SCREEN_EXTRAS_MENU);
      }
      break;
      
    case CONTEXT_MENU_FUNCGEN:
      {
        enum {
          ITEM_VIEW_OUTPUTS,
          ITEM_COPY_FUNCGEN,
          ITEM_RESET_FUNCGEN,
        };
        
        contextMenuInitialise();
        contextMenuAddItem(PSTR("View outputs"), ITEM_VIEW_OUTPUTS);
        contextMenuAddItem(PSTR("Copy to"), ITEM_COPY_FUNCGEN);
        contextMenuAddItem(PSTR("Reset settings"), ITEM_RESET_FUNCGEN);
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_RESET_FUNCGEN)
        {
          resetFuncgenParams(thisFgenIdx);
          changeToScreen(SCREEN_FUNCTION_GENERATORS);
        }
        if(contextMenuSelectedItemID == ITEM_COPY_FUNCGEN)
        {
          destFgenIdx = thisFgenIdx;
          changeToScreen(DIALOG_COPY_FUNCGEN);
        }
        if(contextMenuSelectedItemID == ITEM_VIEW_OUTPUTS)
          changeToScreen(SCREEN_FUNCGEN_OUTPUTS);

        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_FUNCTION_GENERATORS);
      }
      break;
      
    case DIALOG_COPY_FUNCGEN:
      {
        isEditMode = true;
        destFgenIdx = incDec(destFgenIdx, 0, NUM_FUNCGEN - 1, INCDEC_WRAP, INCDEC_SLOW);
        drawDialogCopyMove(PSTR("Fgen"), thisFgenIdx, destFgenIdx, true);
        if(clickedButton == KEY_SELECT)
        {
          Model.Funcgen[destFgenIdx] = Model.Funcgen[thisFgenIdx];
          thisFgenIdx = destFgenIdx;
          changeToScreen(SCREEN_FUNCTION_GENERATORS);
        }
        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_FUNCTION_GENERATORS);
      }
      break;
      
    case SCREEN_FUNCGEN_OUTPUTS:
      {
        display.setInterlace(false); 
        drawHeader(PSTR("Funcgen outputs"));

        //--- scrollable 
        
        uint8_t numPages = (NUM_FUNCGEN + 4) / 5;
        static uint8_t thisPage = 1;
        
        static bool viewInitialised = false;
        if(!viewInitialised) 
        {
          thisPage = (thisFgenIdx + 5) / 5; //start in the page with the function generator we want to view
          viewInitialised = true;
        }

        isEditMode = true;
        thisPage = incDec(thisPage, numPages, 1, INCDEC_WRAP, INCDEC_SLOW);
        
        uint8_t startIdx = (thisPage - 1) * 5;
        for(uint8_t i = startIdx; i < startIdx + 5 && i < NUM_FUNCGEN; i++)
        {
          uint8_t ypos = 10 + (i - startIdx) * 11;
          //show marker
          if(i == thisFgenIdx) 
          {
            display.setCursor(0, ypos);
            display.write(0xB1);
          }
          //show name
          display.setCursor(7, ypos);
          getSrcName(txtBuff, SRC_FUNCGEN_FIRST + i, sizeof(txtBuff));
          display.print(txtBuff);
          display.print(F(":"));
          //draw graph
          drawHorizontalBarChartZeroCentered(45, ypos + 2, 39, 3, BLACK, mixSources[SRC_FUNCGEN_FIRST + i]/5, 200);
          //show value
          display.setCursor(94, ypos);
          display.print(mixSources[SRC_FUNCGEN_FIRST + i]/5);
        }
        
        //show scrollbar
        drawScrollBar(127, 9, numPages, thisPage, 1, 1 * 54);

        if(heldButton == KEY_SELECT) //exit
        {
          viewInitialised = false;
          changeToScreen(SCREEN_FUNCTION_GENERATORS);
        }
      }
      break;

    case SCREEN_COUNTERS:
      {
        drawHeader(extrasMenu[EXTRAS_MENU_COUNTERS]);
        
        counter_params_t *counter = &Model.Counter[thisCounterIdx];
        
        display.setCursor(8, 9);
        display.print(F("Counter"));
        display.print(thisCounterIdx + 1);
        if(!isEmptyStr(counter->name, sizeof(counter->name)))
        {
          display.setCursor(display.getCursorX() + 6, 9);
          display.print(counter->name);
        }
        display.drawHLine(8, 17, display.getCursorX() - 9, BLACK);
        
        //show the value of the counter. Right align
        uint8_t digits = 1;
        int16_t num = counterOut[thisCounterIdx];          
        while(num >= 10)
        {
          num /= 10;
          digits++;
        }
        uint8_t xpos = 121 - (digits - 1)*6;
        display.setCursor(xpos, 9);
        display.print(counterOut[thisCounterIdx]);
        display.drawRect(xpos - 2, 7, digits * 6 + 3, 11, BLACK);
        
        //--- scrollable list 
  
        enum {
          ITEM_CLOCK,
          ITEM_EDGE,
          ITEM_MODULUS,
          ITEM_DIRECTION,
          ITEM_CLEAR,
          ITEM_PERSISTENT,
          
          ITEM_COUNT
        };
        
        uint8_t listItemIDs[ITEM_COUNT]; 
        uint8_t listItemCount = 0;
        
        //add item Ids to the list of Ids
        for(uint8_t i = 0; i < sizeof(listItemIDs); i++)
        {
          listItemIDs[listItemCount++] = i;
        }
        
        //handle navigation
        uint8_t numFocusable = listItemCount + 2; //+1 for title focus, +1 for context menu focus
        changeFocusOnUpDown(numFocusable); 
        toggleEditModeOnSelectClicked();
        static uint8_t topItem = 1;
        if(focusedItem == 1 || focusedItem == numFocusable) //title focus or context menu focus
          topItem = 1;
        else if(focusedItem > 1)
        {
          if(focusedItem - 1 < topItem)
            topItem = focusedItem - 1;
          while(focusedItem - 1 >= topItem + 5)
            topItem++;
        }
        
        //fill list and edit items
        for(uint8_t line = 0; line < 5 && line < listItemCount; line++)
        {
          uint8_t ypos = 20 + line*9;
          if(focusedItem - 1 == topItem + line)
            drawCursor(58, ypos);
          
          uint8_t itemID = listItemIDs[topItem - 1 + line];
          bool edit = (itemID == listItemIDs[focusedItem - 2] && isEditMode) ? true : false;
          
          display.setCursor(0, ypos);
          switch(itemID)
          {
            case ITEM_CLOCK:
              {
                display.print(F("Clock:"));
                display.setCursor(66, ypos);
                getControlSwitchName(txtBuff, counter->clock, sizeof(txtBuff));
                display.print(txtBuff);
                if(edit)
                  counter->clock = incDecControlSwitch(counter->clock, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW);
              }
              break;
              
            case ITEM_EDGE:
              {
                display.print(F("Edge:"));
                display.setCursor(66, ypos);
                display.print(findStringInIdStr(enum_ClockEdge, counter->edge));
                if(edit)
                  counter->edge = incDec(counter->edge, 0, 2, INCDEC_WRAP, INCDEC_SLOW);
              }
              break;
            
            case ITEM_MODULUS:
              {
                display.print(F("Modulus:"));
                display.setCursor(66, ypos);
                display.print(counter->modulus);
                if(edit)
                  counter->modulus = incDec(counter->modulus, 2, 10000, INCDEC_NOWRAP, INCDEC_FAST);
              }
              break;
            
            case ITEM_DIRECTION:
              {
                display.print(F("Direction:"));
                display.setCursor(66, ypos);
                display.print(findStringInIdStr(enum_CounterDirection, counter->direction));
                if(edit)
                  counter->direction = incDec(counter->direction, 0, 1, INCDEC_WRAP, INCDEC_SLOW);
              }
              break;
            
            case ITEM_CLEAR:
              {
                display.print(F("Clear:"));
                display.setCursor(66, ypos);
                getControlSwitchName(txtBuff, counter->clear, sizeof(txtBuff));
                display.print(txtBuff);
                if(edit)
                  counter->clear = incDecControlSwitch(counter->clear, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW);
              }
              break;
              
            case ITEM_PERSISTENT:
              {
                display.print(F("Persist:"));
                drawCheckbox(66, ypos, counter->isPersistent);
                if(edit)
                  counter->isPersistent = incDec(counter->isPersistent, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
              }
              break;
          }
        }
        
        //scrollbar
        drawScrollBar(127, 19, listItemCount, topItem, 5, 5 * 9);
        
        //draw context menu icon
        display.fillRect(120, 0, 8, 7, WHITE);
        display.drawBitmap(120, 0, focusedItem == numFocusable ? icon_context_menu_focused : icon_context_menu, 8, 7, BLACK);

        //open context menu
        if(focusedItem == numFocusable && isEditMode)
          changeToScreen(CONTEXT_MENU_COUNTERS);
        
        //change to next counter
        if(focusedItem == 1)
        {          
          drawCursor(0, 9);
          thisCounterIdx = incDec(thisCounterIdx, 0, NUM_COUNTERS - 1, INCDEC_WRAP, INCDEC_SLOW);
        }

        // Exit
        if(heldButton == KEY_SELECT)
          changeToScreen(SCREEN_EXTRAS_MENU);
      }
      break;
      
    case CONTEXT_MENU_COUNTERS:
      {
        enum {
          ITEM_RENAME_COUNTER,
          ITEM_RESET_COUNTER,
          ITEM_COPY_COUNTER,
          ITEM_CLEAR_COUNTER,
          ITEM_CLEAR_ALL_COUNTERS,
        };
        
        contextMenuInitialise();
        contextMenuAddItem(PSTR("Copy to"), ITEM_COPY_COUNTER);
        contextMenuAddItem(PSTR("Rename counter"), ITEM_RENAME_COUNTER);
        contextMenuAddItem(PSTR("Reset settings"), ITEM_RESET_COUNTER);
        contextMenuAddItem(PSTR("Clear counter"), ITEM_CLEAR_COUNTER);
        contextMenuAddItem(PSTR("Clear all"), ITEM_CLEAR_ALL_COUNTERS);
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_RESET_COUNTER)
        {
          resetCounterParams(thisCounterIdx);
          changeToScreen(SCREEN_COUNTERS);
        }
        if(contextMenuSelectedItemID == ITEM_CLEAR_COUNTER)
        {
          resetCounterRegister(thisCounterIdx);
          changeToScreen(SCREEN_COUNTERS);  
        }
        if(contextMenuSelectedItemID == ITEM_CLEAR_ALL_COUNTERS)
          changeToScreen(CONFIRMATION_CLEAR_ALL_COUNTERS);  
        if(contextMenuSelectedItemID == ITEM_RENAME_COUNTER)
          changeToScreen(DIALOG_RENAME_COUNTER);
        if(contextMenuSelectedItemID == ITEM_COPY_COUNTER)
        {
          destCounterIdx = thisCounterIdx;
          changeToScreen(DIALOG_COPY_COUNTER);  
        }

        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_COUNTERS);
      }
      break;
      
    case DIALOG_RENAME_COUNTER:
      {
        isEditTextDialog = true;
        editTextDialog(PSTR("Counter name"), Model.Counter[thisCounterIdx].name, sizeof(Model.Counter[0].name), 
                       true, true, false);
        if(!isEditTextDialog) //exited
          changeToScreen(SCREEN_COUNTERS);
      }
      break;
      
    case DIALOG_COPY_COUNTER:
      {
        isEditMode = true;
        destCounterIdx = incDec(destCounterIdx, 0, NUM_COUNTERS - 1, INCDEC_WRAP, INCDEC_SLOW);
        drawDialogCopyMove(PSTR("Counter"), thisCounterIdx, destCounterIdx, true);
        if(clickedButton == KEY_SELECT)
        {
          Model.Counter[destCounterIdx] = Model.Counter[thisCounterIdx];
          thisCounterIdx = destCounterIdx;
          changeToScreen(SCREEN_COUNTERS);
        }
        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_COUNTERS);
      }
      break;
      
    case CONFIRMATION_CLEAR_ALL_COUNTERS:
      {
        printFullScreenMsg(PSTR("Clear all counter\nregisters?\n\nYes [Up] \nNo [Down]"));
        if(clickedButton == KEY_UP)
        {
          resetCounterRegisters();
          changeToScreen(SCREEN_COUNTERS);
        }
        else if(clickedButton == KEY_DOWN || heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_COUNTERS);
      }
      break;
      
    case SCREEN_TIMERS:
      {
        timer_params_t *tmr = &Model.Timer[thisTimerIdx]; 
        
        if(isEditTextDialog) 
        {
          editTextDialog(PSTR("Timer name"), tmr->name, sizeof(tmr->name), true, true, false);
          break;
        }
        
        drawHeader(extrasMenu[EXTRAS_MENU_TIMERS]);

        display.setCursor(8, 9);
        display.print(F("Timer"));
        display.print(thisTimerIdx + 1);
        display.drawHLine(8, 17, display.getCursorX() - 8, BLACK);

        display.setCursor(0, 20);
        display.print(F("Name:"));
        display.setCursor(60, 20);
        if(isEmptyStr(tmr->name, sizeof(tmr->name)))
          display.print(F("--"));
        else
          display.print(tmr->name);

        display.setCursor(0, 29);
        display.print(F("Switch:"));
        display.setCursor(60, 29);
        getControlSwitchName(txtBuff, tmr->swtch, sizeof(txtBuff));
        display.print(txtBuff);
        
        display.setCursor(0, 38);
        display.print(F("Reset:"));
        display.setCursor(60, 38);
        getControlSwitchName(txtBuff, tmr->resetSwitch, sizeof(txtBuff));
        display.print(txtBuff);
        
        display.setCursor(0, 47);
        display.print(F("Initial:"));
        display.setCursor(60, 47);
        display.print(tmr->initialMinutes);
        display.print(F(" min"));
        
        display.setCursor(0, 56);
        display.print(F("Persist:"));
        drawCheckbox(60, 56, tmr->isPersistent);
        
        //draw context menu icon
        display.fillRect(120, 0, 8, 7, WHITE);
        display.drawBitmap(120, 0, focusedItem == 7 ? icon_context_menu_focused : icon_context_menu, 8, 7, BLACK);
        
        //Show the value of the timer, right aligned.
        //Here we are employing a hack to measure the width of the text 
        //by intentionally print off screen and using the difference in the x position.
        display.setCursor(0, 64);
        printTimerValue(thisTimerIdx);
        uint8_t len = display.getCursorX() / 6;
        //now print on screen
        uint8_t xpos = 127 - len * 6;
        display.setCursor(xpos, 9);
        printTimerValue(thisTimerIdx);
        display.drawRect(xpos - 2, 7, len * 6 + 3, 11, BLACK);
        
        //show cursor
        if(focusedItem == 1)
          drawCursor(0, 9);
        else if(focusedItem <= 6)
          drawCursor(52, 20 + 9 * (focusedItem - 2));
        
        changeFocusOnUpDown(7);
        toggleEditModeOnSelectClicked();
        
        //edit items
        if(focusedItem == 1)
          thisTimerIdx = incDec(thisTimerIdx, 0, NUM_TIMERS - 1, INCDEC_WRAP, INCDEC_SLOW);
        else if(focusedItem == 2 && isEditMode)
          isEditTextDialog = true;
        else if(focusedItem == 3 && isEditMode)
          tmr->swtch = incDecControlSwitch(tmr->swtch, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW);
        else if(focusedItem == 4 && isEditMode)
          tmr->resetSwitch = incDecControlSwitch(tmr->resetSwitch, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW);
        else if(focusedItem == 5)
          tmr->initialMinutes = incDec(tmr->initialMinutes, 0, 240, INCDEC_NOWRAP, INCDEC_NORMAL);
        else if(focusedItem == 6)
          tmr->isPersistent = incDec(tmr->isPersistent, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        else if(focusedItem == 7 && isEditMode)
          changeToScreen(CONTEXT_MENU_TIMERS);

        //exit 
        if(heldButton == KEY_SELECT)
        {
          changeToScreen(SCREEN_EXTRAS_MENU);
        }
      }
      break;
      
    case CONTEXT_MENU_TIMERS:
      {
        enum {
          ITEM_START_STOP_TIMER,
          ITEM_RESET_TIMER,
          ITEM_RESET_TIMER_SETTINGS,
        };
        
        contextMenuInitialise();
        if(Model.Timer[thisTimerIdx].swtch == CTRL_SW_NONE)
        {
          if(timerIsRunning[thisTimerIdx]) 
            contextMenuAddItem(PSTR("Stop timer"), ITEM_START_STOP_TIMER);
          else 
            contextMenuAddItem(PSTR("Start timer"), ITEM_START_STOP_TIMER);
        }
        contextMenuAddItem(PSTR("Reset timer"), ITEM_RESET_TIMER);
        contextMenuAddItem(PSTR("Reset settings"), ITEM_RESET_TIMER_SETTINGS);
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_START_STOP_TIMER)
        {
          timerForceRun[thisTimerIdx] = timerIsRunning[thisTimerIdx] ? false : true;
          changeToScreen(SCREEN_TIMERS);
        }
        if(contextMenuSelectedItemID == ITEM_RESET_TIMER)
        {
          resetTimerRegister(thisTimerIdx);
          changeToScreen(SCREEN_TIMERS);
        }
        if(contextMenuSelectedItemID == ITEM_RESET_TIMER_SETTINGS)
        {
          resetTimerParams(thisTimerIdx);
          resetTimerRegister(thisTimerIdx); //also reset the register
          changeToScreen(SCREEN_TIMERS);
        }

        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_TIMERS);
      }
      break;
      
    case SCREEN_SAFETY_CHECKS:
      {
        drawHeader(extrasMenu[EXTRAS_MENU_SAFETY_CHECKS]);
        
        enum {
          ITEM_SAFETY_CHECK_THROTTLE,
          ITEM_SAFETY_CHECK_SW_FIRST,
          ITEM_SAFETY_CHECK_SW_LAST = ITEM_SAFETY_CHECK_SW_FIRST + MAX_NUM_PHYSICAL_SWITCHES - 1,
          
          ITEM_COUNT
        };
        
        uint8_t listItemIDs[ITEM_COUNT]; 
        uint8_t listItemCount = 0;
        
        //add item Ids to the list of Ids
        for(uint8_t i = 0; i < sizeof(listItemIDs); i++)
        {        
          if(i == ITEM_SAFETY_CHECK_THROTTLE && Model.type == MODEL_TYPE_OTHER)
            continue;
          //skip absent switches
          if(i >= ITEM_SAFETY_CHECK_SW_FIRST && i <= ITEM_SAFETY_CHECK_SW_LAST)
          {
            uint8_t sw = i - ITEM_SAFETY_CHECK_SW_FIRST;
            if(Sys.swType[sw] == SW_ABSENT)
              continue; 
          }
          listItemIDs[listItemCount++] = i;
        }
        
        //initialise
        static uint8_t topItem;
        static bool viewInitialised = false;
        if(!viewInitialised)
        {
          focusedItem = 1;
          topItem = 1;
          viewInitialised = true;
        }
        
        //handle navigation
        changeFocusOnUpDown(listItemCount);
        if(focusedItem < topItem)
          topItem = focusedItem;
        while(focusedItem >= topItem + 6)
          topItem++;
        
        toggleEditModeOnSelectClicked();
        
        //fill list and edit items
        for(uint8_t line = 0; line < 6 && line < listItemCount; line++)
        {
          uint8_t ypos = 9 + line*9;
          if(focusedItem == topItem + line)
            drawCursor(64, ypos);
          
          uint8_t itemID = listItemIDs[topItem - 1 + line];
          bool edit = (itemID == listItemIDs[focusedItem - 1] && isEditMode) ? true : false;
          
          display.setCursor(0, ypos);
          
          if(itemID == ITEM_SAFETY_CHECK_THROTTLE)
          {
            display.print(F("Throttle:"));
            drawCheckbox(72, ypos, Model.checkThrottle);
            if(edit)
              Model.checkThrottle = incDec(Model.checkThrottle, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
          }
          else if(itemID >= ITEM_SAFETY_CHECK_SW_FIRST && itemID <= ITEM_SAFETY_CHECK_SW_LAST)
          {
            uint8_t sw = itemID - ITEM_SAFETY_CHECK_SW_FIRST;
            display.print(F("Switch "));
            display.write(65 + sw);
            display.print(F(":"));
            display.setCursor(72, ypos);
            display.print(findStringInIdStr(enum_SwitchWarn, Model.switchWarn[sw]));
            if(edit)
            {
              if(Sys.swType[sw] == SW_2POS)
                Model.switchWarn[sw] = incDec(Model.switchWarn[sw], -1, 1, INCDEC_WRAP, INCDEC_SLOW);
              else if(Sys.swType[sw] == SW_3POS)
                Model.switchWarn[sw] = incDec(Model.switchWarn[sw], -1, 2, INCDEC_WRAP, INCDEC_SLOW);
            }
          }
        }
        
        //Draw scroll bar
        drawScrollBar(127, 8, listItemCount, topItem, 6, 6 * 9);

        // Exit
        if(heldButton == KEY_SELECT)
          changeToScreen(SCREEN_EXTRAS_MENU);
      }
      break;
      
    case SCREEN_NOTIFICATION_SETUP:
      {
        notification_params_t *notification = &Model.CustomNotification[thisNotificationIdx]; 
        
        if(isEditTextDialog) 
        {
          editTextDialog(PSTR("Text"), notification->text, sizeof(notification->text), true, true, false);
          break;
        }
        
        //use a temporary variable for the swtch and assign later 
        //to avoid annoying playback while scrolling through the options
        static uint8_t tempSwtch;
        static bool tempInitialised = false;
        if(!tempInitialised)
        {
          tempSwtch = notification->swtch;
          tempInitialised = true;
        }
        
        drawHeader(extrasMenu[EXTRAS_MENU_NOTIFICATION_SETUP]);
        
        display.setCursor(8, 9);
        display.print(F("Notification"));
        display.print(thisNotificationIdx + 1);
        display.drawHLine(8, 17, display.getCursorX() - 9, BLACK);
        
        display.setCursor(0, 20);
        display.print(F("Switch:"));
        display.setCursor(60, 20);
        getControlSwitchName(txtBuff, tempSwtch, sizeof(txtBuff));
        display.print(txtBuff);
        
        display.setCursor(0, 29);
        display.print(F("Tone:"));
        display.setCursor(60, 29);
        display.print("Tone");
        display.print(notification->tone - AUDIO_NOTIFICATION_TONE_FIRST + 1);
        
        display.setCursor(0, 38);
        display.print(F("Popup:"));
        drawCheckbox(60, 38, notification->showPopup);

        display.setCursor(0, 47);
        display.print(F("Text:"));
        display.setCursor(60, 47);
        if(isEmptyStr(notification->text, sizeof(notification->text)))
          display.print(F("--"));
        else
          display.print(notification->text);


        //draw cursor
        if(focusedItem == 1) 
          drawCursor(0, 9);
        else if(focusedItem < 6)
          drawCursor(52, (focusedItem * 9) + 2);
        
        //draw context menu icon
        display.fillRect(120, 0, 8, 7, WHITE);
        display.drawBitmap(120, 0, focusedItem == 6 ? icon_context_menu_focused : icon_context_menu, 8, 7, BLACK);

        //change focus
        changeFocusOnUpDown(6);
        toggleEditModeOnSelectClicked();
        
        //---- edit params
        if(focusedItem == 1) //change to next or previous
        {
          thisNotificationIdx = incDec(thisNotificationIdx, 0, NUM_CUSTOM_NOTIFICATIONS - 1, INCDEC_WRAP, INCDEC_SLOW);
          //assign from temp
          notification->swtch = tempSwtch;
          tempInitialised = false;
        }
        else if(focusedItem == 2) //swtch
        {
          if(Model.type == MODEL_TYPE_AIRPLANE || Model.type == MODEL_TYPE_MULTICOPTER)
            tempSwtch = incDecControlSwitch(tempSwtch, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW | INCDEC_FLAG_FMODE);
          else
            tempSwtch = incDecControlSwitch(tempSwtch, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW);
        }
        else if(focusedItem == 3) //tone
        {
          //preview the tone while in edit mode
          static bool editStarted = false;
          static uint8_t prevTone;
          if(isEditMode && !editStarted)
          {
            editStarted = true;
            prevTone = notification->tone;
          }
          if(!isEditMode || heldButton == KEY_SELECT)
          {
            prevTone = notification->tone;
            editStarted = false;
          }
          notification->tone = incDec(notification->tone, AUDIO_NOTIFICATION_TONE_FIRST, AUDIO_NOTIFICATION_TONE_LAST, INCDEC_NOWRAP, INCDEC_SLOW);
          if(notification->tone != prevTone && buttonCode == 0 && millis() - buttonReleaseTime >= 500) 
          {
            prevTone = notification->tone;
            audioToPlay = notification->tone;
          }
        }
        else if(focusedItem == 4)//enable popup
          notification->showPopup = incDec(notification->showPopup, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        else if(focusedItem == 5 && isEditMode) //edit text
          isEditTextDialog = true;
        else if(focusedItem == 6 && isEditMode) //context menu
        {
          changeToScreen(CONTEXT_MENU_NOTIFICATIONS);
          //assign from temp
          notification->swtch = tempSwtch;
          tempInitialised = false;
        }
          
        //assign from temp
        if(!isEditMode || (buttonCode == 0 && millis() - buttonReleaseTime >= 500))
        {
          notification->swtch = tempSwtch;
          tempInitialised = false;
        }

        //exit
        if(heldButton == KEY_SELECT)
        {
          changeToScreen(SCREEN_EXTRAS_MENU);
          //assign from temp
          notification->swtch = tempSwtch;
          tempInitialised = false;
        }
      }
      break;
      
    case CONTEXT_MENU_NOTIFICATIONS:
      {
        enum {
          ITEM_COPY_TO,
          ITEM_RESET_SETTINGS,
        };
        
        contextMenuInitialise();
        contextMenuAddItem(PSTR("Copy to"), ITEM_COPY_TO);
        contextMenuAddItem(PSTR("Reset settings"), ITEM_RESET_SETTINGS);
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_COPY_TO)
        {
          destNotificationIdx = thisNotificationIdx;
          changeToScreen(DIALOG_COPY_NOTIFICATION);
        }
        
        if(contextMenuSelectedItemID == ITEM_RESET_SETTINGS)
        {
          resetNotificationParams(thisNotificationIdx);
          changeToScreen(SCREEN_NOTIFICATION_SETUP);
        }

        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_NOTIFICATION_SETUP);
      }
      break;
      
    case DIALOG_COPY_NOTIFICATION:
      {
        isEditMode = true;
        destNotificationIdx = incDec(destNotificationIdx, 0, NUM_CUSTOM_NOTIFICATIONS - 1, INCDEC_WRAP, INCDEC_SLOW);
        drawDialogCopyMove(PSTR("#"), thisNotificationIdx, destNotificationIdx, true);
        if(clickedButton == KEY_SELECT)
        {
          Model.CustomNotification[destNotificationIdx] = Model.CustomNotification[thisNotificationIdx];
          thisNotificationIdx = destNotificationIdx;
          changeToScreen(SCREEN_NOTIFICATION_SETUP);
        }
        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_NOTIFICATION_SETUP);
      }
      break;

    case SCREEN_TRIM_SETUP:
      {
        drawHeader(extrasMenu[EXTRAS_MENU_TRIM_SETUP]);
        
        uint8_t axis[4] = {SRC_X1_AXIS, SRC_Y1_AXIS, SRC_X2_AXIS, SRC_Y2_AXIS};
        trim_params_t* trim[4] = {&Model.X1Trim, &Model.Y1Trim, &Model.X2Trim, &Model.Y2Trim};

        for(uint8_t i = 0; i < 4; i++)
        {
          display.setCursor(0, 9 + i * 9);
          getSrcName(txtBuff, axis[i], sizeof(txtBuff));
          display.print(txtBuff);
          display.print(F(" trim:"));
          display.setCursor(66, 9 + i * 9);
          display.print(findStringInIdStr(enum_TrimState, trim[i]->trimState));
        }
        
        changeFocusOnUpDown(4);
        toggleEditModeOnSelectClicked();
        drawCursor(58, focusedItem * 9);
        
        //edit
        if(isEditMode)
        {
          uint8_t i = focusedItem - 1;
          do {
            trim[i]->trimState = incDec(trim[i]->trimState, 0, TRIM_STATE_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);
          } while(Model.type == MODEL_TYPE_OTHER && trim[i]->trimState == TRIM_FLIGHT_MODE);
        }

        //exit
        if(heldButton == KEY_SELECT)
          changeToScreen(SCREEN_EXTRAS_MENU);
      }
      break;
      
    case SCREEN_FLIGHT_MODES:
      {
        static uint8_t thisFmdIdx = 0;
        flight_mode_t *fmd = &Model.FlightMode[thisFmdIdx];
        
        if(isEditTextDialog)
        {
          editTextDialog(PSTR("Flght mode name"), fmd->name, sizeof(fmd->name), true, true, false);
          break;
        }
        
        drawHeader(extrasMenu[EXTRAS_MENU_FLIGHT_MODES]);
        
        //-- draw --
        display.setCursor(8, 9);
        display.print(F("FMD"));
        display.print(thisFmdIdx + 1);
        display.drawHLine(8, 17, display.getCursorX() - 9, BLACK);
        
        display.setCursor(0, 20);
        display.print(F("Name:"));
        display.setCursor(66, 20);
        if(isEmptyStr(fmd->name, sizeof(fmd->name)))
          display.print(F("--"));
        else
          display.print(fmd->name);
        
        display.setCursor(0, 29);
        display.print(F("Switch:"));
        display.setCursor(66, 29);
        if(thisFmdIdx == 0)
          display.print(F("N/A")); 
        else
        {
          getControlSwitchName(txtBuff, fmd->swtch, sizeof(txtBuff));
          display.print(txtBuff);
        }
        
        display.setCursor(0, 38);
        display.print(F("Fade-in:"));
        display.setCursor(66, 38);
        printSeconds(fmd->transitionTime);
        
        //draw cursor
        if(focusedItem == 1) drawCursor(0, 9);
        else drawCursor(58, (focusedItem * 9) + 2);
        
        changeFocusOnUpDown(4);
        toggleEditModeOnSelectClicked();
        
        //edit
        if(focusedItem == 1)
          thisFmdIdx = incDec(thisFmdIdx, 0, NUM_FLIGHT_MODES - 1, INCDEC_WRAP, INCDEC_SLOW);
        else if(focusedItem == 2 && isEditMode)
          isEditTextDialog = true;
        else if(focusedItem == 3 && thisFmdIdx > 0 && isEditMode)
          fmd->swtch = incDecControlSwitch(fmd->swtch, INCDEC_FLAG_PHY_SW | INCDEC_FLAG_LGC_SW);
        else if(focusedItem == 4)
          fmd->transitionTime = incDec(fmd->transitionTime, 0, 50, INCDEC_NOWRAP, INCDEC_SLOW_START);

        //exit
        if(heldButton == KEY_SELECT)
          changeToScreen(SCREEN_EXTRAS_MENU);
      }
      break;

    ////////////////////////////////// TELEMETRY ///////////////////////////////////////////////////

    case SCREEN_TELEMETRY:
      {
        drawHeader(mainMenu[MAIN_MENU_TELEMETRY]);
        
        //--- scrollable list ----
        
        static uint8_t topItem = 1;
        static bool viewInitialised = false;
        if(!viewInitialised)
        {
          focusedItem = thisTelemIdx + 1;
          viewInitialised = true;
        }
        
        //handle navigation
        changeFocusOnUpDown(NUM_CUSTOM_TELEMETRY);
        if(focusedItem < topItem)
          topItem = focusedItem;
        while(focusedItem >= topItem + 6)
          topItem++;
        //fill list
        for(uint8_t line = 0; line < 6 && line < NUM_CUSTOM_TELEMETRY; line++)
        {
          uint8_t ypos = 10 + line * 9;
          uint8_t item = topItem + line;
          if(focusedItem == topItem + line) //highlight
          {
            display.fillRect(0, ypos - 1, item < 10 ? 7 : 13, 9, BLACK);
            display.setTextColor(WHITE);
          }
          display.setCursor(1, ypos);
          display.print(item);
          display.setTextColor(BLACK);
          
          uint8_t idx = item - 1;
          if(!isEmptyStr(Model.Telemetry[idx].name, sizeof(Model.Telemetry[0].name)))
          {
            display.setCursor(16, ypos);
            display.print(Model.Telemetry[idx].name);
            display.print(F(":"));
            drawTelemetryValue(73, ypos, idx, telemetryReceivedValue[idx], true);
          }
        }
        //scroll bar
        drawScrollBar(127, 9, NUM_CUSTOM_TELEMETRY, topItem, 6, 6 * 9 + 1);
        //--- end of list ---
        
        if(focusedItem <= NUM_CUSTOM_TELEMETRY)
          thisTelemIdx = focusedItem - 1;
        
        if(clickedButton == KEY_SELECT)
        {
          viewInitialised = false;
          if(isEmptyStr(Model.Telemetry[thisTelemIdx].name, sizeof(Model.Telemetry[0].name)))
            changeToScreen(CONTEXT_MENU_FREE_SENSOR);
          else
            changeToScreen(CONTEXT_MENU_ACTIVE_SENSOR);
        }

        if(heldButton == KEY_SELECT)
        {
          viewInitialised = false;
          changeToScreen(SCREEN_MAIN_MENU);
        }
      }
      break;
      
    case CONTEXT_MENU_FREE_SENSOR:
      {
        enum {
          ITEM_NEW_SENSOR,
          ITEM_SENSOR_TEMPLATES
        };
        
        contextMenuInitialise();
        contextMenuAddItem(PSTR("New sensor"), ITEM_NEW_SENSOR);
        contextMenuAddItem(PSTR("Templates"), ITEM_SENSOR_TEMPLATES);
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_NEW_SENSOR)
        { 
          resetTelemParams(thisTelemIdx);        
          changeToScreen(SCREEN_CREATE_SENSOR);
        }
        if(contextMenuSelectedItemID == ITEM_SENSOR_TEMPLATES)
        {
          resetTelemParams(thisTelemIdx);
          changeToScreen(CONTEXT_MENU_SENSOR_TEMPLATES);
        }

        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_TELEMETRY);
      }
      break;
      
    case SCREEN_CREATE_SENSOR:
      {
        isEditTextDialog = true;
        editTextDialog(PSTR("Sensor name"), Model.Telemetry[thisTelemIdx].name, sizeof(Model.Telemetry[0].name), 
                       false, true, false);
        if(!isEditTextDialog) //exited
          changeToScreen(SCREEN_EDIT_SENSOR);
      } 
      break;
      
    case CONTEXT_MENU_SENSOR_TEMPLATES:
      {
        enum {
          ITEM_EXTVOLTS_2S,
          ITEM_EXTVOLTS_3S,
          ITEM_EXTVOLTS_4S,
          ITEM_RSSI,
          ITEM_LINK_QUALITY,
        };
        
        contextMenuInitialise();
        contextMenuAddItem(PSTR("ExtVolts 2S"), ITEM_EXTVOLTS_2S);
        contextMenuAddItem(PSTR("ExtVolts 3S"), ITEM_EXTVOLTS_3S);
        contextMenuAddItem(PSTR("ExtVolts 4S"), ITEM_EXTVOLTS_4S);
        contextMenuAddItem(PSTR("Link quality"), ITEM_LINK_QUALITY);
        contextMenuAddItem(PSTR("RSSI"), ITEM_RSSI);
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_EXTVOLTS_2S)  loadSensorTemplateExtVolts2S(thisTelemIdx);
        if(contextMenuSelectedItemID == ITEM_EXTVOLTS_3S)  loadSensorTemplateExtVolts3S(thisTelemIdx);
        if(contextMenuSelectedItemID == ITEM_EXTVOLTS_4S)  loadSensorTemplateExtVolts4S(thisTelemIdx);
        if(contextMenuSelectedItemID == ITEM_RSSI)         loadSensorTemplateRSSI(thisTelemIdx);
        if(contextMenuSelectedItemID == ITEM_LINK_QUALITY) loadSensorTemplateLinkQuality(thisTelemIdx);
        
        if(contextMenuSelectedItemID != 0xff) 
          changeToScreen(SCREEN_TELEMETRY);
        
        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_TELEMETRY);
      }
      break;
      
    case CONTEXT_MENU_ACTIVE_SENSOR:
      {
        enum {
          ITEM_VIEW_STATS,
          ITEM_EDIT_SENSOR,
          ITEM_DELETE_SENSOR
        };
        
        contextMenuInitialise();
        contextMenuAddItem(PSTR("View stats"), ITEM_VIEW_STATS);
        contextMenuAddItem(PSTR("Edit sensor"), ITEM_EDIT_SENSOR);
        contextMenuAddItem(PSTR("Delete sensor"), ITEM_DELETE_SENSOR);
        contextMenuDraw();
        
        if(contextMenuSelectedItemID == ITEM_VIEW_STATS) changeToScreen(SCREEN_SENSOR_STATS);
        if(contextMenuSelectedItemID == ITEM_EDIT_SENSOR) changeToScreen(SCREEN_EDIT_SENSOR);
        if(contextMenuSelectedItemID == ITEM_DELETE_SENSOR) changeToScreen(CONFIRMATION_DELETE_SENSOR);

        if(heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_TELEMETRY);
      }
      break;
      
    case SCREEN_SENSOR_STATS:
      {
        drawHeader(PSTR("Stats"));
        
        static uint8_t page = 0;
        static bool viewInitialised = false;
        static uint8_t topItem = 1;
        if(!viewInitialised)
        {
          page = thisTelemIdx;
          focusedItem = 1;
          topItem = 1;
          viewInitialised = true;
        }
        
        enum {
          ITEM_MINIMUM_VAL,
          ITEM_RESET_MINIMUM_VAL,
          ITEM_MAXIMUM_VAL,
          ITEM_RESET_MAXIMUM_VAL,
          ITEM_LAST_RECEIVED_VAL,
          ITEM_LAST_RECEIVED_TIME,
          ITEM_RESET_LAST_RECEIVED_VAL,
          
          ITEM_COUNT
        };
        
        uint8_t listItemIDs[ITEM_COUNT]; 
        uint8_t listItemCount = 0;
        //add items IDs to the list
        for(uint8_t i = 0; i < sizeof(listItemIDs); i++)
        {
          if(!Model.Telemetry[page].recordMaximum && (i == ITEM_MAXIMUM_VAL || i == ITEM_RESET_MAXIMUM_VAL))
            continue;
          if(!Model.Telemetry[page].recordMinimum && (i == ITEM_MINIMUM_VAL || i == ITEM_RESET_MINIMUM_VAL))
            continue;
          listItemIDs[listItemCount++] = i;
        }
        
        //handle navigation
        uint8_t numFocusable = listItemCount + 1; //+1 for title focus
        changeFocusOnUpDown(numFocusable);
        if(focusedItem == 1) //title focus
          topItem = 1;
        else if(focusedItem > 1)
        {
          if(focusedItem - 1 < topItem)
            topItem = focusedItem - 1;
          while(focusedItem - 1 >= topItem + 5)
            topItem++;
        }
        
        //show title
        display.setCursor(8, 9);
        display.print(Model.Telemetry[page].name);
        display.drawHLine(8, 17, display.getCursorX() - 9, BLACK);
        
        //show current val, right aligned.
        //using a hack to measure the width of the text before printing to actual position on screen
        drawTelemetryValue(0, 64, page, telemetryReceivedValue[page], false);
        uint8_t len = display.getCursorX() / 6;
        uint8_t xpos = 127 - len * 6;
        drawTelemetryValue(xpos, 9, page, telemetryReceivedValue[page], true);
        display.drawRect(xpos - 2, 7, len * 6 + 3, 11, BLACK);

        //fill list and edit items
        for(uint8_t line = 0; line < 5 && line < listItemCount; line++)
        {
          uint8_t ypos = 20 + line*9;
          if(focusedItem - 1 == topItem + line)
            drawCursor(58, ypos);
          
          uint8_t itemID = listItemIDs[topItem - 1 + line];
          bool isFocused = (itemID == listItemIDs[focusedItem - 2]) ? true : false;
          
          display.setCursor(0, ypos);
          switch(itemID)
          {
            case ITEM_MAXIMUM_VAL:
              {
                display.print(F("Maximum:"));
                drawTelemetryValue(66, ypos, page, telemetryMaxReceivedValue[page], false);
              }
              break;
            
            case ITEM_RESET_MAXIMUM_VAL:
              {
                display.setCursor(66, ypos);
                display.print(F("[Reset]"));
                if(isFocused && clickedButton == KEY_SELECT)
                  telemetryMaxReceivedValue[page] = telemetryReceivedValue[page];
              }
              break;
            
            case ITEM_MINIMUM_VAL:
              {
                display.print(F("Minimum:"));
                drawTelemetryValue(66, ypos, page, telemetryMinReceivedValue[page], false);
              }
              break;
              
            case ITEM_RESET_MINIMUM_VAL:
              {
                display.setCursor(66, ypos);
                display.print(F("[Reset]"));
                if(isFocused && clickedButton == KEY_SELECT)
                  telemetryMinReceivedValue[page] = telemetryReceivedValue[page]; 
              }
              break;
              
            case ITEM_LAST_RECEIVED_VAL:
              {
                display.print(F("Last Val:"));
                drawTelemetryValue(66, ypos, page, telemetryLastReceivedValue[page], false);
              }
              break;
              
            case ITEM_LAST_RECEIVED_TIME:
              {
                display.print(F("Time rcvd:"));
                display.setCursor(66, ypos);
                if(telemetryLastReceivedValue[page] == TELEMETRY_NO_DATA)
                  display.print(F("No data"));
                else
                {
                  uint32_t elapsedSeconds = (millis() - telemetryLastReceivedTime[page])/1000;
                  if(elapsedSeconds >= 3600)
                  {
                    uint8_t hours = elapsedSeconds / 3600;
                    display.print(hours);
                    display.print(F("hr"));
                  }
                  else if(elapsedSeconds >= 60)
                  {
                    uint8_t minutes = elapsedSeconds / 60;
                    display.print(minutes);
                    display.print(F("min"));
                  }
                  else
                  {
                    display.print(elapsedSeconds);
                    display.print(F("s"));
                  }
                  display.print(F(" ago"));
                }
              }
              break;
              
            case ITEM_RESET_LAST_RECEIVED_VAL:
              {
                display.setCursor(66, ypos);
                display.print(F("[Reset]"));
                if(isFocused && clickedButton == KEY_SELECT)
                  telemetryLastReceivedValue[page] = telemetryReceivedValue[page]; 
              }
              break;
          }
        }
        
        //scrollbar
        drawScrollBar(127, 19, listItemCount, topItem, 5, 5 * 9);
        
        //change to next sensor
        if(focusedItem == 1)
        { 
          drawCursor(0, 9);
          toggleEditModeOnSelectClicked();  
          if(isEditMode)
          {
            do {
              page = incDec(page, 0, NUM_CUSTOM_TELEMETRY - 1, INCDEC_WRAP, INCDEC_SLOW);
            } while(isEmptyStr(Model.Telemetry[page].name, sizeof(Model.Telemetry[0].name))); //skip empty
          }          
        }
        
        //exit
        if(heldButton == KEY_SELECT)
        {
          viewInitialised = false;
          changeToScreen(SCREEN_TELEMETRY);
        }
      }
      break;
    
    case SCREEN_EDIT_SENSOR:
      {
        telemetry_params_t *tlm = &Model.Telemetry[thisTelemIdx]; 
        
        enum {
          ITEM_TELEMETRY_NAME,
          ITEM_TELEMETRY_UNITSNAME,
          ITEM_TELEMETRY_IDENTIFIER,
          ITEM_TELEMETRY_FACTOR10,
          ITEM_TELEMETRY_MULTIPLIER,
          ITEM_TELEMETRY_OFFSET,
          ITEM_TELEMETRY_ALARM_CONDITION,
          ITEM_TELEMETRY_ALARM_THRESHOLD,
          ITEM_TELEMETRY_SHOW_ON_HOME,
          ITEM_TELEMETRY_RECORD_MAXIMUM,
          ITEM_TELEMETRY_RECORD_MINIMUM,
          
          ITEM_COUNT
        };
        
        static bool isEditingName = false;
        static bool isEditingUnits = false;
        
        if(isEditTextDialog)
        {
          if(isEditingName) 
            editTextDialog(PSTR("Sensor name"), tlm->name, sizeof(tlm->name), false, true, false);
          else if(isEditingUnits)
            editTextDialog(PSTR("Specify units"),  tlm->unitsName, sizeof(tlm->unitsName), true, true, false);
          break;
        }
        //reset
        isEditingName = false;
        isEditingUnits = false;

        drawHeader(PSTR("Sensor"));
        
        uint8_t listItemIDs[ITEM_COUNT]; 
        uint8_t listItemCount = 0;
        
        //add item Ids to the list of Ids
        for(uint8_t i = 0; i < sizeof(listItemIDs); i++)
        {
          if(i == ITEM_TELEMETRY_ALARM_THRESHOLD && tlm->alarmCondition == TELEMETRY_ALARM_CONDITION_NONE)
            continue;
          listItemIDs[listItemCount++] = i;
        }
        
        //handle navigation
        changeFocusOnUpDown(listItemCount); 
        toggleEditModeOnSelectClicked();
        static uint8_t topItem = 1;
        if(focusedItem < topItem)
          topItem = focusedItem;
        while(focusedItem >= topItem + 7)
          topItem++;
        
        //fill list and edit items
        for(uint8_t line = 0; line < 7 && line < listItemCount; line++)
        {
          uint8_t ypos = 8 + line*8;
          if(focusedItem == topItem + line)
            drawCursor(64, ypos);
          
          uint8_t itemID = listItemIDs[topItem - 1 + line];
          bool edit = (itemID == listItemIDs[focusedItem - 1] && isEditMode) ? true : false;
          
          display.setCursor(0, ypos);
          switch(itemID)
          {
            case ITEM_TELEMETRY_NAME:
              {
                display.print(F("Name:"));
                display.setCursor(72, ypos);
                if(isEmptyStr(tlm->name, sizeof(tlm->name)))
                  display.print(F("--"));
                else
                  display.print(tlm->name);
                if(edit)
                {
                  isEditTextDialog = true;
                  isEditingName = true;
                }
              }
              break;
            
            case ITEM_TELEMETRY_UNITSNAME:
              {
                display.print(F("Units:"));
                display.setCursor(72, ypos);
                if(isEmptyStr(tlm->unitsName, sizeof(tlm->unitsName)))
                  display.print(F("--"));
                else 
                  display.print(tlm->unitsName);
                if(edit)
                {
                  isEditTextDialog = true;
                  isEditingUnits = true;
                }
              }
              break;
            
            case ITEM_TELEMETRY_IDENTIFIER: 
              {
                display.print(F("ID:"));
                display.setCursor(72, ypos);
                display.print(F("0x"));
                if(tlm->identifier < 16)
                  display.print(F("0"));
                display.print(tlm->identifier, HEX);
                if(edit)
                  tlm->identifier = incDec(tlm->identifier, 0x00, 0xFE, INCDEC_NOWRAP, INCDEC_NORMAL);
              }
              break;

            case ITEM_TELEMETRY_MULTIPLIER:
              {
                display.print(F("Multplr:"));
                display.setCursor(72, ypos);
                int16_t val = tlm->multiplier;
                display.print(val / 100);
                display.print(F("."));
                val = val % 100;
                if(val < 10) 
                  display.print(F("0"));
                display.print(val);
                if(edit)
                  tlm->multiplier = incDec(tlm->multiplier, 1, 1000, INCDEC_NOWRAP, INCDEC_FAST);
              }
              break;
            
            case ITEM_TELEMETRY_FACTOR10:
              {
                display.print(F("Factor10:"));
                display.setCursor(72, ypos);
                display.print(tlm->factor10);
                if(edit)
                  tlm->factor10 = incDec(tlm->factor10, -2, 2, INCDEC_NOWRAP, INCDEC_SLOW);
              }
              break;
            
            case ITEM_TELEMETRY_OFFSET:
              {
                display.print(F("Offset:"));
                printTelemParam(72, ypos, thisTelemIdx, tlm->offset);
                if(edit)
                  tlm->offset = incDec(tlm->offset, -30000, 30000, INCDEC_NOWRAP, INCDEC_FAST);
              }
              break;

            case ITEM_TELEMETRY_ALARM_CONDITION:
              {
                display.print(F("Alerts:"));
                display.setCursor(72, ypos);
                display.print(findStringInIdStr(enum_TelemetryAlarmCondition, tlm->alarmCondition));
                if(edit)
                  tlm->alarmCondition = incDec(tlm->alarmCondition, 0, TELEMETRY_ALARM_CONDITION_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);
              }
              break;
            
            case ITEM_TELEMETRY_ALARM_THRESHOLD:
              {
                display.print(F("Thresh:"));
                printTelemParam(72, ypos, thisTelemIdx, tlm->alarmThreshold);
                if(edit)
                  tlm->alarmThreshold = incDec(tlm->alarmThreshold, -30000, 30000, INCDEC_NOWRAP, INCDEC_FAST);
              }
              break;
            
            case ITEM_TELEMETRY_SHOW_ON_HOME:
              {
                display.print(F("On home:"));
                drawCheckbox(72, ypos, tlm->showOnHome);
                if(edit)
                  tlm->showOnHome = incDec(tlm->showOnHome, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
              }
              break;
            
            case ITEM_TELEMETRY_RECORD_MAXIMUM:
              {
                display.print(F("RecordMax:"));
                drawCheckbox(72, ypos, tlm->recordMaximum);
                if(edit)
                  tlm->recordMaximum = incDec(tlm->recordMaximum, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
              }
              break;
            
            case ITEM_TELEMETRY_RECORD_MINIMUM:
              {
                display.print(F("RecordMin:"));
                drawCheckbox(72, ypos, tlm->recordMinimum);
                if(edit)
                  tlm->recordMinimum = incDec(tlm->recordMinimum, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
              }
              break;
          }
        }
        
        //Draw scroll bar
        drawScrollBar(127, 8, listItemCount, topItem, 7, 7 * 8);
        
        //exit
        if(heldButton == KEY_SELECT)
          changeToScreen(SCREEN_TELEMETRY);
      }
      break;
      
    case CONFIRMATION_DELETE_SENSOR:
      {
        printFullScreenMsg(PSTR("Delete sensor?\n\nYes [Up] \nNo [Down]"));
        if(clickedButton == KEY_UP)
        {
          resetTelemParams(thisTelemIdx);
          //reset any associated logical switches
          for(uint8_t i = 0; i < NUM_LOGICAL_SWITCHES; i++)
          {
            if(Model.LogicalSwitch[i].func <= LS_FUNC_GROUP3_LAST 
               && Model.LogicalSwitch[i].val1 == (int16_t) MIXSOURCES_COUNT + NUM_COUNTERS + NUM_TIMERS + thisTelemIdx)
            {
              resetLogicalSwitchParams(i);
            }
          }
          //reset any associated widgets
          for(uint8_t i = 0; i < NUM_WIDGETS; i++)
          {
            if(Model.Widget[i].type == WIDGET_TYPE_TELEMETRY && Model.Widget[i].src == thisTelemIdx)
            {
              resetWidgetParams(i);
            }
          }
          //exit
          changeToScreen(SCREEN_TELEMETRY);
        }
        else if(clickedButton == KEY_DOWN || heldButton == KEY_SELECT) //exit
          changeToScreen(SCREEN_TELEMETRY);
      }
      break;
      
    ////////////////////////////////// SYSTEM //////////////////////////////////////////////////////
      
    case SCREEN_SYSTEM_MENU:
      {
        drawHeader(mainMenu[MAIN_MENU_SYSTEM]);
        
        static uint8_t topItem = 1, highlightedItem = 1;
        drawMenu(systemMenu, sizeof(systemMenu)/sizeof(systemMenu[0]), NULL, &topItem, &highlightedItem);

        //--- handle selection
        if(clickedButton == KEY_SELECT)
        {
          uint8_t selectIdx = highlightedItem - 1;
          if(selectIdx == SYSTEM_MENU_RF) changeToScreen(SCREEN_RF);
          if(selectIdx == SYSTEM_MENU_SOUND) changeToScreen(SCREEN_SOUND);
          if(selectIdx == SYSTEM_MENU_BACKLIGHT) changeToScreen(SCREEN_BACKLIGHT);
          if(selectIdx == SYSTEM_MENU_APPEARANCE) changeToScreen(SCREEN_APPEARANCE);
          if(selectIdx == SYSTEM_MENU_ADVANCED)  
          {
            if(!isEmptyStr(Sys.password, sizeof(Sys.password))) 
              changeToScreen(SCREEN_UNLOCK_ADVANCED_MENU); //prompt to enter password first
            else //no password specified, don't prompt
              changeToScreen(SCREEN_ADVANCED_MENU);
          }
        }

        //exit
        if(heldButton == KEY_SELECT)
        {
          if(!Sys.rememberMenuPosition)
          {
            topItem = 1;
            highlightedItem = 1;
          }
          changeToScreen(SCREEN_MAIN_MENU);
        }
      }
      break;
      
    case SCREEN_RF:
      {
        drawHeader(systemMenu[SYSTEM_MENU_RF]);
        
        display.setCursor(0, 9);
        display.print(F("RF output:"));
        drawCheckbox(72, 9, Sys.rfEnabled);
      
        display.setCursor(0, 18);
        display.print(F("RF power:"));
        display.setCursor(72, 18);
        strlcpy_P(txtBuff, rfPowerStr[Sys.rfPower], sizeof(txtBuff));
        display.print(txtBuff);

        changeFocusOnUpDown(2);
        toggleEditModeOnSelectClicked();
        drawCursor(64, focusedItem * 9);
        
        //edit
        if(focusedItem == 1)
          Sys.rfEnabled = incDec(Sys.rfEnabled, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        else if(focusedItem == 2)
          Sys.rfPower = incDec(Sys.rfPower, 0, RF_POWER_COUNT - 1, INCDEC_NOWRAP, INCDEC_SLOW);
        
        //exit
        if(heldButton == KEY_SELECT)
        {
          eeSaveSysConfig();
          changeToScreen(SCREEN_SYSTEM_MENU);
        }
      }
      break;
      
    case SCREEN_SOUND:
      {
        drawHeader(systemMenu[SYSTEM_MENU_SOUND]);
        
        display.setCursor(0, 9);
        display.print(F("Enable:"));
        drawCheckbox(72, 9, Sys.soundEnabled);
        
        if(Sys.soundEnabled)
        {
          changeFocusOnUpDown(6);
          
          display.setCursor(0, 18);
          display.print(F("Inactvty:"));
          display.setCursor(72, 18);
          if(Sys.inactivityMinutes == 0)
            display.print(F("Off"));
          else
          {
            display.print(Sys.inactivityMinutes);
            display.print(F("min"));
          }
          
          display.setCursor(0, 27);
          display.print(F("Switches:"));
          drawCheckbox(72, 27, Sys.soundSwitches);
          
          display.setCursor(0, 36);
          display.print(F("Trims:"));
          drawCheckbox(72, 36, Sys.soundTrims);
          
          display.setCursor(0, 45);
          display.print(F("Knobs:"));
          drawCheckbox(72, 45, Sys.soundKnobCenter);
          
          display.setCursor(0, 54);
          display.print(F("Keys:"));
          drawCheckbox(72, 54, Sys.soundKeys);
        }
        
        toggleEditModeOnSelectClicked();
        drawCursor(64, focusedItem * 9);
        
        //edit
        if(focusedItem == 1)
          Sys.soundEnabled = incDec(Sys.soundEnabled, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        else if(focusedItem == 2)
          Sys.inactivityMinutes = incDec(Sys.inactivityMinutes, 0, 240, INCDEC_NOWRAP, INCDEC_SLOW_START);
        else if(focusedItem == 3)
          Sys.soundSwitches = incDec(Sys.soundSwitches, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        else if(focusedItem == 4)
          Sys.soundTrims = incDec(Sys.soundTrims, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        else if(focusedItem == 5)
          Sys.soundKnobCenter = incDec(Sys.soundKnobCenter, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        else if(focusedItem == 6)
          Sys.soundKeys = incDec(Sys.soundKeys, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        
        //exit
        if(heldButton == KEY_SELECT)
        {
          eeSaveSysConfig();
          changeToScreen(SCREEN_SYSTEM_MENU);
        }
      }
      break;
      
    case SCREEN_BACKLIGHT:
      {
        drawHeader(systemMenu[SYSTEM_MENU_BACKLIGHT]);
        
        display.setCursor(0, 9);
        display.print(F("Enable:"));
        drawCheckbox(78, 9, Sys.backlightEnabled);
        
        if(Sys.backlightEnabled)
        {
          changeFocusOnUpDown(5);
          
          display.setCursor(0, 18);
          display.print(F("Brightness:"));
          display.setCursor(78, 18);
          display.print(Sys.backlightLevel);
          display.print(F("%"));
          
          display.setCursor(0, 27);
          display.print(F("Timeout:"));
          display.setCursor(78, 27);
          strlcpy_P(txtBuff, backlightTimeoutStr[Sys.backlightTimeout], sizeof(txtBuff));
          display.print(txtBuff);
          
          display.setCursor(0, 36);
          display.print(F("Wake up:"));
          display.setCursor(78, 36);
          strlcpy_P(txtBuff, backlightWakeupStr[Sys.backlightWakeup], sizeof(txtBuff));
          display.print(txtBuff);
          
          display.setCursor(0, 45);
          display.print(F("Key filter:"));
          drawCheckbox(78, 45, Sys.backlightSuppressFirstKey);
        }
        
        toggleEditModeOnSelectClicked();
        drawCursor(70, focusedItem * 9);
        
        //edit
        if(focusedItem == 1)
          Sys.backlightEnabled = incDec(Sys.backlightEnabled, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        else if(focusedItem == 2)
          Sys.backlightLevel = incDec(Sys.backlightLevel, 10, 100, INCDEC_NOWRAP, INCDEC_NORMAL);
        else if(focusedItem == 3)
          Sys.backlightTimeout = incDec(Sys.backlightTimeout, 0, BACKLIGHT_TIMEOUT_COUNT - 1, INCDEC_NOWRAP, INCDEC_SLOW);
        else if(focusedItem == 4)
          Sys.backlightWakeup = incDec(Sys.backlightWakeup, 0, BACKLIGHT_WAKEUP_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);
        else if(focusedItem == 5)
          Sys.backlightSuppressFirstKey = incDec(Sys.backlightSuppressFirstKey, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        
        //exit
        if(heldButton == KEY_SELECT)
        {
          eeSaveSysConfig();
          changeToScreen(SCREEN_SYSTEM_MENU);
        }
      }
      break;
      
    case SCREEN_APPEARANCE:
      {
        drawHeader(systemMenu[SYSTEM_MENU_APPEARANCE]);
        
        enum {
          ITEM_SHOW_MENU_ICONS,
          ITEM_KEEP_MENU_POSITION,
          ITEM_USE_ROUND_CORNERS,
          ITEM_ENABLE_ANIMATIONS,
          ITEM_AUTOHIDE_TRIMS,
          ITEM_USE_NUMERICAL_BATTERY_INDICATOR,
          ITEM_SHOW_WELCOME_MSG,
          ITEM_SHOW_SPLASH_SCREEN,
          
          ITEM_COUNT
        };
        
        uint8_t listItemIDs[ITEM_COUNT]; 
        uint8_t listItemCount = 0;
        
        //add item Ids to the list of Ids
        for(uint8_t i = 0; i < sizeof(listItemIDs); i++)
        {        
          listItemIDs[listItemCount++] = i;
        }
        
        //initialise
        static uint8_t topItem;
        static bool viewInitialised = false;
        if(!viewInitialised)
        {
          focusedItem = 1;
          topItem = 1;
          viewInitialised = true;
        }
        
        //handle navigation
        changeFocusOnUpDown(listItemCount);
        if(focusedItem < topItem)
          topItem = focusedItem;
        while(focusedItem >= topItem + 6)
          topItem++;
        
        toggleEditModeOnSelectClicked();
        
        //fill list and edit items
        for(uint8_t line = 0; line < 6 && line < listItemCount; line++)
        {
          uint8_t ypos = 9 + line*9;
          if(focusedItem == topItem + line)
            drawCursor(94, ypos);
          
          uint8_t itemID = listItemIDs[topItem - 1 + line];
          bool edit = (itemID == listItemIDs[focusedItem - 1] && isEditMode) ? true : false;
          
          display.setCursor(0, ypos);
          switch(itemID)
          {
            case ITEM_SHOW_MENU_ICONS:
              {
                display.print(F("Menu icons:")); drawCheckbox(102, ypos, Sys.showMenuIcons);
                if(edit) Sys.showMenuIcons = incDec(Sys.showMenuIcons, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
              }
              break;
              
            case ITEM_KEEP_MENU_POSITION:
              {
                display.print(F("Keep menu pstn:")); drawCheckbox(102, ypos, Sys.rememberMenuPosition);
                if(edit) Sys.rememberMenuPosition = incDec(Sys.rememberMenuPosition, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
              }
              break;
              
            case ITEM_USE_ROUND_CORNERS:
              {
                display.print(F("Round corners:")); drawCheckbox(102, ypos, Sys.useRoundRect);
                if(edit) Sys.useRoundRect = incDec(Sys.useRoundRect, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
              }
              break;
              
            case ITEM_ENABLE_ANIMATIONS:
              {
                display.print(F("Animations:")); drawCheckbox(102, ypos, Sys.animationsEnabled);
                if(edit) Sys.animationsEnabled = incDec(Sys.animationsEnabled, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
              }
              break;
              
            case ITEM_AUTOHIDE_TRIMS:
              {
                display.print(F("Autohide trims:")); drawCheckbox(102, ypos, Sys.autohideTrims);
                if(edit) Sys.autohideTrims = incDec(Sys.autohideTrims, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
              }
              break;
            
            case ITEM_SHOW_SPLASH_SCREEN:
              {
                display.print(F("Splash screen:")); drawCheckbox(102, ypos, Sys.showSplashScreen);
                if(edit) Sys.showSplashScreen = incDec(Sys.showSplashScreen, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
              }
              break;
            
            case ITEM_SHOW_WELCOME_MSG:
              {
                display.print(F("Welcome msg:")); drawCheckbox(102, ypos, Sys.showWelcomeMsg);
                if(edit) Sys.showWelcomeMsg = incDec(Sys.showWelcomeMsg, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
              }
              break;

            case ITEM_USE_NUMERICAL_BATTERY_INDICATOR:
              {
                display.print(F("Numeric BattV:")); drawCheckbox(102, ypos, Sys.useNumericalBatteryIndicator);
                if(edit) Sys.useNumericalBatteryIndicator = incDec(Sys.useNumericalBatteryIndicator, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
              }
              break;
          }
        }
        
        //Draw scroll bar
        drawScrollBar(127, 8, listItemCount, topItem, 6, 6 * 9);
        
        //exit
        if(heldButton == KEY_SELECT)
        {
          eeSaveSysConfig();
          changeToScreen(SCREEN_SYSTEM_MENU);
          viewInitialised = false;
        }
      }
      break;
      
    case SCREEN_UNLOCK_ADVANCED_MENU:
      {
        validatePassword(SCREEN_ADVANCED_MENU, SCREEN_SYSTEM_MENU);
      }
      break;
      
    case SCREEN_ADVANCED_MENU:
      {
        drawHeader(systemMenu[SYSTEM_MENU_ADVANCED]);
        
        static uint8_t topItem = 1, highlightedItem = 1;
        drawMenu(advancedMenu, sizeof(advancedMenu)/sizeof(advancedMenu[0]), NULL, &topItem, &highlightedItem);

        //--- handle selection
        if(clickedButton == KEY_SELECT)
        {
          uint8_t selectIdx = highlightedItem - 1;
          if(selectIdx == ADVANCED_MENU_STICKS) changeToScreen(SCREEN_STICKS);
          if(selectIdx == ADVANCED_MENU_SWITCHES) changeToScreen(SCREEN_SWITCHES); 
          if(selectIdx == ADVANCED_MENU_BATTERY) changeToScreen(SCREEN_BATTERY);
          if(selectIdx == ADVANCED_MENU_DEBUG) changeToScreen(SCREEN_DEBUG);
          if(selectIdx == ADVANCED_MENU_SECURITY) changeToScreen(SCREEN_SECURITY);
          if(selectIdx == ADVANCED_MENU_MISC) changeToScreen(SCREEN_MISC);
        }

        //exit
        if(heldButton == KEY_SELECT)
        {
          if(!Sys.rememberMenuPosition)
          {
            topItem = 1;
            highlightedItem = 1;
          }
          changeToScreen(SCREEN_SYSTEM_MENU);
        }
      }
      break;
      
    case SCREEN_STICKS:
      {
        drawHeader(advancedMenu[ADVANCED_MENU_STICKS]);

        enum {
          PAGE_START,
          PAGE_AXIS_TYPE,
          PAGE_MOVE_STICKS, 
          PAGE_ADJUST_DEADZONE
        };
        
        static uint8_t page = PAGE_START;
        static bool initialised = false;

        if(!initialised)
        {
          initialised = true;
          focusedItem = 1;
          isEditMode = false;
          page = (lastScreen == SCREEN_HOME) ? PAGE_AXIS_TYPE : PAGE_START;
        }
        
        uint8_t  axis[] = {
          SRC_X1_AXIS, SRC_Y1_AXIS, SRC_X2_AXIS, SRC_Y2_AXIS,
          SRC_X3_AXIS, SRC_Y3_AXIS, SRC_X4_AXIS, SRC_Y4_AXIS
        };
        
        uint8_t* axisType[] = {
          &Sys.x1AxisType, &Sys.y1AxisType, &Sys.x2AxisType, &Sys.y2AxisType,
          &Sys.x3AxisType, &Sys.y3AxisType, &Sys.x4AxisType, &Sys.y4AxisType
        };
        
        uint8_t* axisDeadzone[] = {
          &Sys.x1AxisDeadzone, &Sys.y1AxisDeadzone, &Sys.x2AxisDeadzone, &Sys.y2AxisDeadzone,
          &Sys.x3AxisDeadzone, &Sys.y3AxisDeadzone, &Sys.x4AxisDeadzone, &Sys.y4AxisDeadzone
        };
        
        int16_t  axisIn[] = {
          x1AxisIn, y1AxisIn, x2AxisIn, y2AxisIn,
          x3AxisIn, y3AxisIn, x4AxisIn, y4AxisIn
        };
        
        uint8_t numItems = sizeof(axisIn) / sizeof(axisIn[0]);

        switch(page)
        {
          case PAGE_START:
            {
              display.setCursor(0, 9);
              display.print(F("Calibration:"));
              display.setCursor(84, 9);
              display.print(F("[Start]"));
              
              display.setCursor(0, 18);
              display.print(F("Deflt mode:"));
              display.setCursor(84, 18);
              if(Sys.defaultStickMode == STICK_MODE_RTAE) display.print(F("RTAE"));
              if(Sys.defaultStickMode == STICK_MODE_AERT) display.print(F("AERT"));
              if(Sys.defaultStickMode == STICK_MODE_REAT) display.print(F("REAT"));
              if(Sys.defaultStickMode == STICK_MODE_ATRE) display.print(F("ATRE"));
              
              drawCursor(76, focusedItem * 9);
              
              changeFocusOnUpDown(2);
              toggleEditModeOnSelectClicked();
              
              if(focusedItem == 1 && isEditMode)
              {
                page = PAGE_AXIS_TYPE;
                isEditMode = false;
              }
              else if(focusedItem == 2)
                Sys.defaultStickMode = incDec(Sys.defaultStickMode, 0, STICK_MODE_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);
              
              if(heldButton == KEY_SELECT)
              {
                initialised = false;
                eeSaveSysConfig();
                changeToScreen(SCREEN_ADVANCED_MENU);
              }
            }
            break;
          
          case PAGE_AXIS_TYPE:
            {
              display.setCursor(0, 9);
              display.print(F("Type of axis"));
              
              //scrollable list
              
              static uint8_t topItem;
              static bool viewInitialised = false;
              if(!viewInitialised)
              {
                focusedItem = 1;
                topItem = 1;
                viewInitialised = true;
              }
              
              for(uint8_t line = 0; line < 4 && line < numItems; line++)
              {
                uint8_t ypos = 18 + line * 9;
                uint8_t item = topItem + line;
                if(focusedItem == item)
                  drawCursor(22, ypos);
                
                display.setCursor(0, ypos);
                uint8_t idx = item - 1;
                getSrcName(txtBuff, axis[idx], sizeof(txtBuff));
                display.print(txtBuff);
                display.print(F(":"));
                
                display.setCursor(30, ypos);
                if(*axisType[idx] == STICK_AXIS_SELF_CENTERING)
                  display.print(F("Self centering"));
                else
                  display.print(F("Non centering"));
              }
              
              //Draw scroll bar
              drawScrollBar(127, 9, numItems, topItem, 4, 44);
              
              //'next' button
              drawDottedHLine(0, 54, 128, BLACK, WHITE);
              display.setCursor(90, 56);
              display.print(F("[Next]"));
              if(focusedItem == numItems + 1)
                drawCursor(82, 56);
              
              //handle navigation
              changeFocusOnUpDown(numItems + 1); //+1 for button focus
              if(focusedItem < topItem)
                topItem = focusedItem;
              while(focusedItem >= topItem + 4 && focusedItem < numItems + 1)
                topItem++;
              toggleEditModeOnSelectClicked();
              
              //edit params
              uint8_t idx = focusedItem - 1;
              if(idx < numItems)
                *axisType[idx] = incDec(*axisType[idx], 0, STICK_AXIS_TYPE_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);
              
              if(focusedItem == numItems + 1 && isEditMode)
              {
                //start the calibration
                calibrateSticks(CALIBRATE_INIT);
                //go to next page
                page = PAGE_MOVE_STICKS;
                isEditMode = false;
                focusedItem = 1;
                viewInitialised = false;
              }
            }
            break;
            
          case PAGE_MOVE_STICKS:
            {
              isCalibratingSticks = true;
          
              printFullScreenMsg(PSTR("Move sticks fully,\nthen center them.\n"));
              calibrateSticks(CALIBRATE_MOVE);
              
              drawDottedHLine(0, 54, 128, BLACK, WHITE);
              display.setCursor(90, 56);
              display.print(F("[Next]"));
              drawCursor(82, 56);
              
              if(clickedButton == KEY_SELECT)
              {
                //add dead band to stick extremes
                calibrateSticks(CALIBRATE_DEADBAND); 
                //go to next page
                page = PAGE_ADJUST_DEADZONE;
                isCalibratingSticks = false;
              }
            }
            break;
            
          case PAGE_ADJUST_DEADZONE:
            {
              display.setCursor(0, 9);
              display.print(F("Adjust deadzone"));
              
              //scrollable list
              
              static uint8_t topItem;
              static bool viewInitialised = false;
              if(!viewInitialised)
              {
                focusedItem = 1;
                topItem = 1;
                viewInitialised = true;
              }
              
              for(uint8_t line = 0; line < 4 && line < numItems; line++)
              {
                uint8_t ypos = 18 + line * 9;
                uint8_t item = topItem + line;
                if(focusedItem == item)
                  drawCursor(22, ypos);
                
                display.setCursor(0, ypos);
                uint8_t idx = item - 1;
                getSrcName(txtBuff, axis[idx], sizeof(txtBuff));
                display.print(txtBuff);
                display.print(F(":"));
                
                display.setCursor(30, ypos);
                if(*axisType[idx] == STICK_AXIS_SELF_CENTERING)
                {
                  display.print(*axisDeadzone[idx]);
                  display.print(F("%"));
                }
                else
                  display.print(F("N/A"));
                
                display.setCursor(66, ypos);
                display.print(F("("));
                display.print(axisIn[idx]/5);
                display.print(F(")"));
              }
              
              //Draw scroll bar
              drawScrollBar(127, 9, numItems, topItem, 4, 44);
              
              //'next' or 'finish' button
              drawDottedHLine(0, 54, 128, BLACK, WHITE);
              if(lastScreen == SCREEN_HOME)
              {
                display.setCursor(90, 56);
                display.print(F("[Next]"));
                if(focusedItem == numItems + 1)
                  drawCursor(82, 56);
              }
              else 
              {
                display.setCursor(78, 56);
                display.print(F("[Finish]"));
                if(focusedItem == numItems + 1)
                  drawCursor(70, 56);
              }
              
              //handle navigation
              changeFocusOnUpDown(numItems + 1); //+1 for button focus
              if(focusedItem < topItem)
                topItem = focusedItem;
              while(focusedItem >= topItem + 4 && focusedItem < numItems + 1)
                topItem++;
              toggleEditModeOnSelectClicked();
              
              //edit params
              uint8_t idx = focusedItem - 1;
              if(idx < numItems && *axisType[idx] == STICK_AXIS_SELF_CENTERING)
                *axisDeadzone[idx] = incDec(*axisDeadzone[idx], 0, 15, INCDEC_NOWRAP, INCDEC_SLOW);
              
              if(focusedItem == numItems + 1 && isEditMode)
              {
                //exit calibration
                eeSaveSysConfig();
                if(lastScreen == SCREEN_HOME)
                  changeToScreen(SCREEN_HOME);
                viewInitialised = false;
                isRequestingStickCalibration = false;
                initialised = false;
              }
            }
            break;
        }
      }
      break;
      
    case SCREEN_SWITCHES:
      {
        drawHeader(advancedMenu[ADVANCED_MENU_SWITCHES]);

        //-- Scrollable list
        static uint8_t topItem;
        static bool showWarning = true;
        static bool viewInitialised = false;
        if(!viewInitialised)
        {
          viewInitialised = true;
          showWarning = true;
          focusedItem = 1;
          topItem = 1;
        }
        
        bool hasNextButton = false;
        if(lastScreen == SCREEN_HOME)
        {
          showWarning = false;
          hasNextButton = true;
        }

        if(showWarning)
        {
          printFullScreenMsg(PSTR("\nThese settings might\naffect your existing\nmodels.\n\n[OK] to continue"));
          if(clickedButton == KEY_SELECT)
            showWarning = false;
          else if(heldButton == KEY_SELECT)
          {
            viewInitialised = false;
            changeToScreen(SCREEN_ADVANCED_MENU);
          }
          
          break;
        }

        //fill list
        uint8_t end = hasNextButton ? 5 : 6;
        for(uint8_t line = 0; line < end && line < MAX_NUM_PHYSICAL_SWITCHES; line++)
        {
          uint8_t ypos = 9 + line * 9;
          if(focusedItem == topItem + line)
            drawCursor(64, ypos);
          display.setCursor(0, ypos);
          uint8_t idx = line + topItem - 1;
          display.print(F("Switch "));
          display.write(65 + idx);
          display.print(F(":"));
          display.setCursor(72, ypos);
          if(Sys.swType[idx] == SW_ABSENT) display.print(F("Absent"));
          if(Sys.swType[idx] == SW_2POS) display.print(F("2POS"));
          if(Sys.swType[idx] == SW_3POS) display.print(F("3POS"));
        }
        
        //Draw scroll bar
        drawScrollBar(127, 8, MAX_NUM_PHYSICAL_SWITCHES, topItem, end, end * 9);
        
        //show the 'next' button
        if(hasNextButton)
        {
          drawDottedHLine(0, 54, 128, BLACK, WHITE);
          display.setCursor(90, 56);
          display.print(F("[Next]"));
          if(focusedItem == MAX_NUM_PHYSICAL_SWITCHES + 1)
            drawCursor(82, 56);
        }
        
        //Navigate
        changeFocusOnUpDown(hasNextButton ? MAX_NUM_PHYSICAL_SWITCHES + 1 : MAX_NUM_PHYSICAL_SWITCHES);
        toggleEditModeOnSelectClicked();
        if(focusedItem < topItem)
          topItem = focusedItem;
        while(focusedItem >= topItem + end && focusedItem < MAX_NUM_PHYSICAL_SWITCHES + 1)
          topItem++;
        
        //Edit items
        uint8_t idx = focusedItem - 1;
        if(idx < MAX_NUM_PHYSICAL_SWITCHES)
          Sys.swType[idx] = incDec(Sys.swType[idx], 0, SW_TYPE_COUNT - 1, INCDEC_WRAP, INCDEC_SLOW);

        //exit
        if((heldButton == KEY_SELECT && !hasNextButton)
           || (focusedItem == MAX_NUM_PHYSICAL_SWITCHES + 1 && clickedButton == KEY_SELECT))
        {
          viewInitialised = false;
          eeSaveSysConfig();
          changeToScreen(lastScreen);
          isRequestingSwitchesSetup = false;
        }
      }
      break;
    
    case SCREEN_BATTERY:
      {
        drawHeader(advancedMenu[ADVANCED_MENU_BATTERY]);
        
        bool hasNextButton = false;
        if(lastScreen == SCREEN_HOME)
          hasNextButton = true;

        display.setCursor(0, 9);
        display.print(F("Gauge min:"));
        display.setCursor(72, 9);
        printVoltage(Sys.battVoltsMin);

        display.setCursor(0, 18);
        display.print(F("Gauge max:"));
        display.setCursor(72, 18);
        printVoltage(Sys.battVoltsMax);
        
        display.setCursor(0, 27);
        display.print(F("Multplr:"));
        display.setCursor(72, 27);
        display.print(Sys.battVfactor);
        
        display.setCursor(0, hasNextButton ? 45 : 56);
        display.print(F("("));
        printVoltage(battVoltsNow);
        display.print(F(")"));
        
        if(hasNextButton)
        {
          drawDottedHLine(0, 54, 128, BLACK, WHITE);
          display.setCursor(90, 56);
          display.print(F("[Next]"));
          if(focusedItem == 4)
            drawCursor(82, 56);
        }
        
        if(focusedItem < 4)
          drawCursor(64, focusedItem * 9);
        
        changeFocusOnUpDown(hasNextButton ? 4 : 3);
        toggleEditModeOnSelectClicked();
        
        //edit items
        if(focusedItem == 1)
        {
          //scale down by 1/10 since we display only 2 decimals
          int16_t val = Sys.battVoltsMin / 10;
          int16_t min = 300;
          int16_t max = (Sys.battVoltsMax - 100) / 10;
          //inc dec
          val = incDec(val, min, max, INCDEC_NOWRAP, INCDEC_NORMAL);
          //scale back
          Sys.battVoltsMin = val * 10;
        }
        else if(focusedItem == 2)
        {
          //scale down by 1/10 since we display only 2 decimals
          int16_t val = Sys.battVoltsMax / 10;
          int16_t min = (Sys.battVoltsMin + 100) / 10;
          int16_t max = 1500;
          //inc dec
          val = incDec(val, min, max, INCDEC_NOWRAP, INCDEC_NORMAL);
          //scale back
          Sys.battVoltsMax = val * 10;
        }
        else if(focusedItem == 3)
          Sys.battVfactor = incDec(Sys.battVfactor, 0, 2000, INCDEC_NOWRAP, INCDEC_FAST);

        //exit
        if((heldButton == KEY_SELECT && !hasNextButton) || (focusedItem == 4 && clickedButton == KEY_SELECT))
        {
          eeSaveSysConfig();
          batteryGaugeCalibrated = true;
          changeToScreen(lastScreen);
        }
      }
      break;
      
    case SCREEN_SECURITY:
      {
        if(isEditTextDialog)
        {
          editTextDialog(PSTR("Password"), Sys.password, sizeof(Sys.password), true, false, false);
          break;
        }

        drawHeader(advancedMenu[ADVANCED_MENU_SECURITY]);
        
        drawCheckbox(8, 9, Sys.lockStartup);
        display.setCursor(20, 9);
        display.print(F("Lock startup"));
        
        drawCheckbox(8, 18, Sys.lockModels);
        display.setCursor(20, 18);
        display.print(F("Lock models"));
        
        display.setCursor(8, 27);
        if(isEmptyStr(Sys.password, sizeof(Sys.password)))
          display.print(F("[Set password]"));
        else
          display.print(F("[Change password]"));

        changeFocusOnUpDown(3);
        toggleEditModeOnSelectClicked();
        drawCursor(0, focusedItem * 9);
        
        if(focusedItem == 1)
          Sys.lockStartup = incDec(Sys.lockStartup, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        else if(focusedItem == 2)
          Sys.lockModels = incDec(Sys.lockModels, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        else if(focusedItem == 3 && isEditMode)
          isEditTextDialog = true;

        //exit
        if(heldButton == KEY_SELECT)
        {
          eeSaveSysConfig();
          changeToScreen(SCREEN_ADVANCED_MENU);
        }
      }
      break;
      
    case SCREEN_MISC:
      {
        drawHeader(advancedMenu[ADVANCED_MENU_MISC]);
        
        display.setCursor(0, 9);
        display.print(F("Autoslct input:"));
        drawCheckbox(102, 9, Sys.autoSelectMovedControl);
        
        display.setCursor(0, 18);
        display.print(F("Mixer templts:"));
        drawCheckbox(102, 18, Sys.mixerTemplatesEnabled);
        
        display.setCursor(0, 27);
        display.print(F("Dflt Ch order:"));
        display.setCursor(102, 27);
        if(Sys.mixerTemplatesEnabled)
        {
          //make the string for the channel order
          char tmpStr[5];
          tmpStr[getChannelIdx('A')] = 'A';
          tmpStr[getChannelIdx('E')] = 'E';
          tmpStr[getChannelIdx('T')] = 'T';
          tmpStr[getChannelIdx('R')] = 'R';
          tmpStr[4] = '\0';
          display.print(tmpStr);
        }
        else
          display.print(F("N/A"));
        
        changeFocusOnUpDown(3);
        toggleEditModeOnSelectClicked();
        drawCursor(94, focusedItem * 9);
        
        if(focusedItem == 1)
          Sys.autoSelectMovedControl = incDec(Sys.autoSelectMovedControl, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        else if(focusedItem == 2)
          Sys.mixerTemplatesEnabled = incDec(Sys.mixerTemplatesEnabled, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
        else if(focusedItem == 3 && Sys.mixerTemplatesEnabled) 
        { 
          //there are 4P4 = 4! = 24 possible arrangements. So our range is 0 to 23.  
          Sys.defaultChannelOrder = incDec(Sys.defaultChannelOrder, 0, 23, INCDEC_NOWRAP, INCDEC_SLOW);
        }

        //exit
        if(heldButton == KEY_SELECT)
        {
          eeSaveSysConfig();
          changeToScreen(SCREEN_ADVANCED_MENU);
        }
      }
      break;
      
    case SCREEN_DEBUG:
      {
        drawHeader(advancedMenu[ADVANCED_MENU_DEBUG]);
        
        telemetryForceRequest = true;

        enum {
          ITEM_PACKET_RATE,
          ITEM_UP_TIME,
          ITEM_LOOP_NUM,
          ITEM_FIXED_LOOP_TIME,
          ITEM_FREE_RAM,
          ITEM_SEPARATOR,
          ITEM_SHOW_LOOP_TIME,
          ITEM_DISABLE_INTERLACING,
          ITEM_SIMULATE_TELEMETRY,
          ITEM_SHOW_CHARACTER_SET,
          ITEM_DUMP_INTERNAL_EEPROM,
          ITEM_DUMP_EXTERNAL_EEPROM,
          ITEM_FACTORY_RESET,
          
          ITEM_COUNT
        };
        
        uint8_t listItemIDs[ITEM_COUNT]; 
        uint8_t listItemCount = 0;
        
        //add item Ids to the list of Ids
        for(uint8_t i = 0; i < sizeof(listItemIDs); i++)
        {
          if(i == ITEM_DUMP_EXTERNAL_EEPROM && !eeHasExternalEE())
            continue;
          listItemIDs[listItemCount++] = i;
        }
        
        //initialise
        static uint8_t topItem;
        static uint8_t lastFocusedItem;
        static bool viewInitialised = false;
        if(!viewInitialised)
        {
          focusedItem = 1;
          lastFocusedItem = 1;
          topItem = 1;
          viewInitialised = true;
        }
        
        //handle navigation
        focusedItem = lastFocusedItem;
        do {
          changeFocusOnUpDown(listItemCount);
        } while(listItemIDs[focusedItem - 1] == ITEM_SEPARATOR);
        
        if(focusedItem < topItem)
          topItem = focusedItem;
        while(focusedItem >= topItem + 6)
          topItem++;
        lastFocusedItem = focusedItem;
        
        //fill list and edit items
        for(uint8_t line = 0; line < 6 && line < listItemCount; line++)
        {
          uint8_t ypos = 9 + line*9;
          if(focusedItem == topItem + line)
            drawCursor(0, ypos);
          
          uint8_t itemID = listItemIDs[topItem - 1 + line];
          bool isFocused = (itemID == listItemIDs[focusedItem - 1]) ? true : false;
          
          display.setCursor(8, ypos);
          switch(itemID)
          {
            case ITEM_SHOW_LOOP_TIME:
              {
                drawCheckbox(display.getCursorX(), ypos, Sys.DBG_showLoopTime);
                display.setCursor(display.getCursorX() + 12, ypos);
                display.print(F("Show loop time"));
                if(isFocused)
                {
                  toggleEditModeOnSelectClicked();
                  Sys.DBG_showLoopTime = incDec(Sys.DBG_showLoopTime, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
                }
              }
              break;
              
            case ITEM_SIMULATE_TELEMETRY:
              {
                drawCheckbox(display.getCursorX(), ypos, Sys.DBG_simulateTelemetry);
                display.setCursor(display.getCursorX() + 12, ypos);
                display.print(F("Simulate telemetry"));
                if(isFocused)
                {
                  toggleEditModeOnSelectClicked();
                  bool lastState = Sys.DBG_simulateTelemetry;
                  Sys.DBG_simulateTelemetry = incDec(Sys.DBG_simulateTelemetry, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
                  if(!lastState && Sys.DBG_simulateTelemetry)
                    makeToast(PSTR("ID:0x30, Src:Virt1"), 2000, 300);
                }
              }
              break;
              
            case ITEM_DISABLE_INTERLACING:
              {
                drawCheckbox(display.getCursorX(), ypos, Sys.DBG_disableInterlacing);
                display.setCursor(display.getCursorX() + 12, ypos);
                display.print(F("No LCD interlacing"));
                if(isFocused)
                {
                  toggleEditModeOnSelectClicked();
                  bool lastState = Sys.DBG_disableInterlacing;
                  Sys.DBG_disableInterlacing = incDec(Sys.DBG_disableInterlacing, 0, 1, INCDEC_WRAP, INCDEC_PRESSED);
                  if(!lastState && Sys.DBG_disableInterlacing)
                    makeToast(PSTR("May reduce performnc"), 2000, 300);
                }
              }
              break;
              
            case ITEM_DUMP_INTERNAL_EEPROM:
              {
                display.print(F("[Dump intnl EEPROM]")); 
                if(isFocused && clickedButton == KEY_SELECT)
                  changeToScreen(SCREEN_INTERNAL_EEPROM_DUMP);
              }
              break;
              
            case ITEM_DUMP_EXTERNAL_EEPROM: 
              {
                display.print(F("[Dump extnl EEPROM]")); 
                if(isFocused && clickedButton == KEY_SELECT)
                  changeToScreen(SCREEN_EXTERNAL_EEPROM_DUMP);
              }
              break;
              
            case ITEM_SHOW_CHARACTER_SET:
              {
                display.print(F("[View char set]")); 
                if(isFocused && clickedButton == KEY_SELECT)
                  changeToScreen(SCREEN_CHARACTER_SET);
              }
              break;
              
            case ITEM_FACTORY_RESET:
              {
                display.print(F("[Factory Reset]")); 
                if(isFocused && clickedButton == KEY_SELECT)
                  changeToScreen(CONFIRMATION_FACTORY_RESET);
              }
              break;
              
            case ITEM_SEPARATOR:
              {
                drawDottedHLine(8, ypos + 3, 112, BLACK, WHITE);
              }
              break;
            
            case ITEM_PACKET_RATE:
              {
                display.print(F("Pkt rate:"));
                display.setCursor(72, ypos);
                display.print(transmitterPacketRate);
                display.print(F(", "));
                display.print(receiverPacketRate);
              }
              break;
            
            case ITEM_UP_TIME:
              {
                display.print(F("Up time:"));
                display.setCursor(72, ypos);
                printHHMMSS(millis());
              }
              break;
            
            case ITEM_FREE_RAM:
              {
                display.print(F("Free ram:"));
                display.setCursor(72, ypos);
                display.print(getFreeRam());
                display.print(F("bytes"));
              }
              break;
            
            case ITEM_LOOP_NUM:
              {
                display.print(F("Loop #:"));
                display.setCursor(72, ypos);
                display.print(thisLoopNum);
              }
              break;
            
            case ITEM_FIXED_LOOP_TIME:
              {
                display.print(F("Fxd loop:"));
                display.setCursor(72, ypos);
                display.print(fixedLoopTime);
                display.print(F("ms"));
              }
              break;
          }
        }
        
        //Draw scroll bar
        drawScrollBar(127, 8, listItemCount, topItem, 6, 6 * 9);
        
        //exit
        if(heldButton == KEY_SELECT)
        {
          eeSaveSysConfig();
          changeToScreen(SCREEN_ADVANCED_MENU);
          viewInitialised = false;
          telemetryForceRequest = false;
        }
      }
      break;
      
    case SCREEN_INTERNAL_EEPROM_DUMP:
    case SCREEN_EXTERNAL_EEPROM_DUMP:
      {
        //We read 32 bytes at a time and cache them, only updating when the view gets scrolled.
        int16_t numPages = eeInternalEEGetSize() / 32;
        if(theScreen == SCREEN_EXTERNAL_EEPROM_DUMP)
          numPages = eeExternalEEGetSize() / 32;
        
        static int16_t page = 0; 
        
        static bool viewInitialised = false;
        static bool needsUpdate = true;
        if(!viewInitialised)
        {
          viewInitialised = true;
          needsUpdate = true;
          page = 0;
          //save model data first at once, as there could be pending writes
          showWaitMsg();
          stopTones();
          eeSaveModelData(Sys.activeModelIdx);
          eeSaveSysConfig();
        }
        
        //handle navigation
        int16_t lastPage = page;
        isEditMode = true;
        page = incDec(page, numPages - 1, 0, INCDEC_WRAP, INCDEC_SLOW_START);
        if(page != lastPage)
          needsUpdate = true;
        
        //fill buffer
        if(needsUpdate)
        {
          needsUpdate = false;
          for(uint8_t idx = 0; idx < 32 && idx < sizeof(txtBuff); idx++)
          {
            uint16_t addr = ((int32_t)page * 32) + idx;
            if(theScreen == SCREEN_EXTERNAL_EEPROM_DUMP)
              txtBuff[idx] = eeExternalEEReadByte(addr);
            else
              txtBuff[idx] = eeInternalEEReadByte(addr);
          }
        }

        //--- draw
        
        display.drawVLine(32, 0, 64, BLACK);
        display.drawVLine(95, 0, 64, BLACK);
        
        for(uint8_t line = 0; line < 8; line++)
        {
          //--- show offsets in decimal
          display.setCursor(0, line*8);
          //count the digits of the address
          uint8_t digits = 1;
          uint16_t addr = ((int32_t)page * 32) + line * 4; //4 bytes per line
          uint16_t num = addr;          
          while(num >= 10)
          {
            num /= 10;
            digits++;
          }
          //print leading zeros
          while(digits < 5)
          {
            display.print(F("0"));
            digits++;
          }
          //print
          display.print(addr); 
          
          //--- show bytes
          for(uint8_t col = 0; col < 4; col++)
          {
            display.setCursor(36 + col * 15, line * 8);
            uint8_t idx = (line * 4) + col;
            uint8_t val = txtBuff[idx];
            if(val < 0x10)
              display.print(F("0"));
            display.print(val, 16); //print as hex
            
            //show the ascii characters
            display.setCursor(99 + col * 6, line * 8);
            if(val == 0x20 || val == 0xFF || val == 0x00)
              display.write(0x2E);
            else
              display.write(val);
          }
        }
        
        //scrollbar
        drawScrollBar(127, 0, numPages, page + 1, 1, 1 * 64);
        
        //Exit
        if(heldButton == KEY_SELECT)
        {
          viewInitialised = false;
          changeToScreen(SCREEN_DEBUG);
        }
      }
      break;
      
    case SCREEN_CHARACTER_SET:
      {
        display.drawVLine(9, 0, 64, BLACK);
        display.drawHLine(0, 9, 128, BLACK);
        //show horizontal markers
        for(uint8_t i = 0; i < 16; i++)
        {
          display.setCursor(12 + i*7, 1);
          display.print(i, 16); //as hex
        }
        
        const uint8_t itemsPerPage = 16 * 6;
        const uint8_t numPages = 3;
        static uint8_t thisPage = 1;
        isEditMode = true;
        thisPage = incDec(thisPage, numPages, 1, INCDEC_WRAP, INCDEC_SLOW);
        
        uint16_t startIdx = (thisPage - 1) * itemsPerPage;
        for(uint16_t i = startIdx; i < startIdx + itemsPerPage && i < 256; i++)
        {
          uint8_t xOffset = 7 * ((i - startIdx) % 16);
          uint8_t yOffset = 9 * ((i - startIdx) / 16);
          display.setCursor(12 + xOffset, 11 + yOffset);
          display.write(i);
          //show vertical markers
          if(xOffset == 0)
          {
            display.setCursor(2, 11 + yOffset);
            display.print(i/16, 16); //as hex
          }
        }
        
        //show scrollbar
        drawScrollBar(127, 11, numPages, thisPage, 1, 1 * 53);
        
        //Exit
        if(heldButton == KEY_SELECT)
        {
          thisPage = 1;
          changeToScreen(SCREEN_DEBUG);
        }
      }
      break;
      
    case CONFIRMATION_FACTORY_RESET:
      {
        printFullScreenMsg(PSTR("Factory reset will\ndelete all data,\nincluding models.\n\nPress [UP]\nrepeatedly"));
        //trigger action
        static uint8_t cntr = 0;
        if(pressedButton == KEY_UP)
          cntr++;
        if(cntr >= 10)
        {
          showWaitMsg();
          stopTones();
          delay(1000);
          eeFactoryReset();
          while(1) //blocking
          {
            delay(10);
            readSwitchesAndButtons();
            determineButtonEvent();
            handlePowerOff();
            checkBattery();
            handleBatteryWarnUI();
            if(isDisplayingBattWarn)
              continue;
            inactivityAlarmHandler();
            playTones();
            display.clearDisplay();
            printFullScreenMsg(PSTR("Factory reset has\nbeen initiated.\nReboot to continue"));
            display.display();
          }
        }

        if(millis() - buttonStartTime > 500)
          cntr = 0;
        
        //show a graphical progress bar
        uint8_t w = ((uint16_t) 128 * cntr) / 10;
        display.fillRect(0, 61, w, 3, BLACK);

        if(heldButton == KEY_SELECT)
        {
          cntr = 0;
          changeToScreen(SCREEN_DEBUG);
        }
      } 
      break;
      
    ////////////////////////////////// RECEIVER ////////////////////////////////////////////////////

    case SCREEN_RECEIVER:
      {
        drawHeader(mainMenu[MAIN_MENU_RECEIVER]);

        enum { 
          PAGE_MAIN_RECEIVER,
          PAGE_SECONDARY_RECEIVER,
          NUM_PAGES
        };
        
        uint8_t numPages = NUM_PAGES;
        if(MAX_CHANNELS_PER_RECEIVER == NUM_RC_CHANNELS)
          numPages = 1;
        
        static uint8_t page = PAGE_MAIN_RECEIVER;
        if(focusedItem == 1)
          page = incDec(page, 0, numPages - 1, INCDEC_WRAP, INCDEC_SLOW);
        
        isMainReceiver = (page == PAGE_MAIN_RECEIVER) ? true : false;
        
        //--- draw
        
        display.setCursor(8, 9);
        strlcpy_P(txtBuff, isMainReceiver ? PSTR("Main rcvr") : PSTR("Secondary rcvr"), sizeof(txtBuff));
        display.print(txtBuff);
        display.drawHLine(8, 17, display.getCursorX() - 9, BLACK);
        
        display.setCursor(8, 20);
        display.print(F("[Bind]"));
        
        display.setCursor(8, 29);
        display.print(F("[Configure]"));
        
        if(focusedItem == 1) drawCursor(0, 9);
        else drawCursor(0, (focusedItem * 9) + 2);
        
        changeFocusOnUpDown(3);
        toggleEditModeOnSelectClicked();
        
        //--- edit items
        if(focusedItem == 2 && isEditMode) //Bind
        {
          isRequestingBind = true;
          changeToScreen(SCREEN_RECEIVER_BINDING);
        }
        else if(focusedItem == 3 && isEditMode) //Output configuration
          changeToScreen(SCREEN_RECEIVER_CONFIG);

        //exit
        if(heldButton == KEY_SELECT)
          changeToScreen(SCREEN_MAIN_MENU);
      }
      break;
      
    case SCREEN_RECEIVER_BINDING:
      {
        static uint32_t entryTime = 0;
        static bool initialised = false;
        if(!initialised)
        {
          initialised = true;
          entryTime = millis();
          bindStatusCode = 0;
        }

        printFullScreenMsg(PSTR("Binding"));
        if(Sys.animationsEnabled)
          drawLoaderSpinner(60, display.getCursorY() + 12, 2);
        
        if(bindStatusCode == 1)
        {
          makeToast(PSTR("Bind success"), 2000, 0);
          changeToScreen(SCREEN_RECEIVER);
          audioToPlay = AUDIO_BIND_SUCCESS;
          initialised = false;
        }
        else if(bindStatusCode == 2)
        {
          makeToast(PSTR("Bind failed"), 2000, 0);
          changeToScreen(SCREEN_RECEIVER);
          initialised = false;
        }
        else if(millis() - entryTime > 5000)
        {
          makeToast(PSTR("Bind timeout"), 2000, 0);
          changeToScreen(SCREEN_RECEIVER);
          initialised = false;
        }
      }
      break;
      
    case SCREEN_RECEIVER_CONFIG:
      {
        enum {
          QUERYING_CONFIG, 
          SENDING_CONFIG, 
          VIEWING_CONFIG
        };
        
        static uint8_t state = QUERYING_CONFIG;
        static bool stateInitialised = false;
        static uint32_t entryTime = 0;
        static bool actionStarted = false;
        
        if(!stateInitialised)
        {
          state = QUERYING_CONFIG;
          // state = VIEWING_CONFIG; //DEBUG
          entryTime = millis();
          gotOutputChConfig = false;
          stateInitialised = true;
        }
        
        if(state == QUERYING_CONFIG)
        {
          printFullScreenMsg(PSTR("Reading settings"));
          if(Sys.animationsEnabled)
            drawLoaderSpinner(60, display.getCursorY() + 12, 2);
          
          if(!actionStarted)
          {
            isRequestingOutputChConfig = true;
            actionStarted = true;
          }
          if(gotOutputChConfig)
          {
            actionStarted = false;
            state = VIEWING_CONFIG;
          }
          //Time out
          if(millis() - entryTime > 2500)
          {
            makeToast(PSTR("No response"), 2000, 0);
            stateInitialised = false;
            actionStarted = false;
            changeToScreen(SCREEN_RECEIVER);
          }
        }
        else if(state == VIEWING_CONFIG)
        {
          drawHeader(PSTR("Output config"));

          //--scrollable list--
          
          static uint8_t topItem;
          static bool viewInitialised = false;
          if(!viewInitialised)
          {
            focusedItem = 1;
            topItem = 1;
            viewInitialised = true;
          }

          uint8_t startIdx = 0; 
          uint8_t endIdx = MAX_CHANNELS_PER_RECEIVER - 1;
          if(!isMainReceiver)
          {
            startIdx = endIdx + 1; 
            endIdx = startIdx + MAX_CHANNELS_PER_RECEIVER - 1;
            if(endIdx >= NUM_RC_CHANNELS)
              endIdx = NUM_RC_CHANNELS - 1;
          }
          
          //fill list
          uint8_t numItems = (endIdx - startIdx) + 1; 
          for(uint8_t line = 0; line < 5 && line < numItems; line++)
          {
            uint8_t ypos = 9 + line * 9;
            uint8_t item = topItem + line;
            if(focusedItem == item)
              drawCursor(64, ypos);
            display.setCursor(0, ypos);
            
            //print channels
            uint8_t idx = startIdx + item - 1; 
            if(idx <= endIdx)
            {
              display.print(F("Channel"));
              if(idx < 9)
                display.print(F(" "));
              display.print(idx + 1);
              display.print(F(":"));
              display.setCursor(72, ypos);
              if(outputChConfig[idx] == 0) display.print(F("Dgtl"));
              if(outputChConfig[idx] == 1) display.print(F("Servo")); 
              if(outputChConfig[idx] == 2) display.print(F("PWM")); 
            }
          }
          
          //Draw scroll bar
          drawScrollBar(127, 9, numItems, topItem, 5, 5 * 9);
          
          //show the write button
          drawDottedHLine(0, 54, 128, BLACK, WHITE);
          display.setCursor(84, 56);
          display.print(F("[Write]"));
          if(focusedItem == numItems + 1)
            drawCursor(76, 56);
          
          //Handle navigation
          changeFocusOnUpDown(numItems + 1); //+1 for button focus
          if(focusedItem < topItem)
            topItem = focusedItem;
          while(focusedItem >= topItem + 5 && focusedItem < numItems + 1)
            topItem++;
          toggleEditModeOnSelectClicked();

          //edit params
          uint8_t idx = startIdx + focusedItem - 1;
          if(idx <= endIdx)
            outputChConfig[idx] = incDec(outputChConfig[idx], 0, maxOutputChConfig[idx], INCDEC_WRAP, INCDEC_SLOW);
          
          //write configuration
          if(focusedItem == numItems + 1 && clickedButton == KEY_SELECT)
          {
            state = SENDING_CONFIG;
            entryTime = millis();
            gotOutputChConfig = false;
            actionStarted = false;
            viewInitialised = false;
          }
          
          //exit without writing changes
          if(heldButton == KEY_SELECT)
          {
            stateInitialised = false;
            actionStarted = false;
            viewInitialised = false;
            changeToScreen(SCREEN_RECEIVER);
          }
        }
        else if(state == SENDING_CONFIG)
        {
          printFullScreenMsg(PSTR("Writing settings"));
          if(Sys.animationsEnabled)
            drawLoaderSpinner(60, display.getCursorY() + 12, 2);
          
          if(!actionStarted)
          {
            isSendOutputChConfig = true;
            actionStarted = true;
          }
          if(receiverConfigStatusCode > 0)
          {
            if(receiverConfigStatusCode == 1) makeToast(PSTR("Write success"), 2000, 0);
            if(receiverConfigStatusCode == 2) makeToast(PSTR("Write failed"), 2000, 0);
            stateInitialised = false;
            actionStarted = false;
            changeToScreen(SCREEN_RECEIVER);
          }
          //Time out
          if(millis() - entryTime > 2500)
          {
            makeToast(PSTR("No response"), 2000, 0);
            stateInitialised = false;
            actionStarted = false;
            changeToScreen(SCREEN_RECEIVER);
          }
        }
      }
      break;
      
    ////////////////////////////////// ABOUT ///////////////////////////////////////////////////////

    case SCREEN_ABOUT:
      {
        drawHeader(mainMenu[MAIN_MENU_ABOUT]);
        printFullScreenMsg(PSTR("\nFW ver:" _SKETCHVERSION "\n(c)2022-2023\nBuk7456\n\ngithub.com/buk7456" ));
        
        display.drawBitmap(61, 61, icon_down_arrow_small, 5, 3, BLACK);
        
        static const char disclaimerText[] PROGMEM = 
           "-----Disclaimer-----\nThe software is provided \"as is\" without warranty of any "
           "kind, express or implied. In no event shall the authors or copyright holders be liable "
           "for any direct, indirect, incidental, special, exemplary or consequential "
           "damages (including but not limited to personal and/or property damage) or "
           "other liability arising from the use of the software.\n\nImproperly "
           "operating RC models can cause serious injury or death.";
        
        if(clickedButton == KEY_DOWN)
        {
          textViewerText = disclaimerText;
          changeToScreen(SCREEN_TEXT_VIEWER);
        }
        
        //--- Trigger launching the Easter egg 
        static uint8_t cntr = 0;
        if(clickedButton == KEY_SELECT)
        {
          cntr++;
          if(cntr >= 3) 
          {
            cntr = 0;
            changeToScreen(SCREEN_EASTER_EGG);
          }
        }
        if(millis() - buttonStartTime > 500)
          cntr = 0;

        //Exit
        if(heldButton == KEY_SELECT)
        {
          cntr = 0;
          changeToScreen(SCREEN_MAIN_MENU);
        }
      }
      break;
      
    case SCREEN_EASTER_EGG:
      {
        //------ Simple flappy bird game -------
        
        display.setInterlace(false);
        
        //note that some values here have been empirically determined
        
        const int16_t BIRD_HEIGHT = 12;
        const int16_t BIRD_LENGTH = 17;
        const int16_t BIRD_YMAX = 50;
        const int16_t PIPE_WIDTH = 17;
        const int16_t PIPE_GAP = 30;
        const float   GRAVITY = 0.068; 
        const float   SCROLL_ACC = 0.0004; 
        const float   SCROLL_VELOCITY_MAX = 2.0;
        const float   BG_SCROLL_ACC = 0.0002;
        
        enum {
          PIPE_VARIANT_A,
          PIPE_VARIANT_B,
          PIPE_VARIANT_C,
          PIPE_VARIANT_COUNT
        };
        
        static const uint8_t bgTerrain[128] PROGMEM = {
          0x04, 0x0c, 0x06, 0x02, 0x03, 0x06, 0x04, 0x0c, 0x08, 0x08, 0x08, 0x08, 0x0c, 0x04, 0x0c, 0x08, 
          0x08, 0x0c, 0x06, 0x04, 0x0c, 0x0c, 0x04, 0x04, 0x0c, 0x08, 0x08, 0x0e, 0x03, 0x03, 0x02, 0x02, 
          0x06, 0x04, 0x04, 0x0c, 0x08, 0x0c, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0c, 0x04, 0x07, 0x01, 0x03, 
          0x02, 0x02, 0x02, 0x02, 0x02, 0x06, 0x04, 0x04, 0x06, 0x02, 0x06, 0x03, 0x07, 0x04, 0x04, 0x04, 
          0x0c, 0x08, 0x0c, 0x0c, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0c, 0x06, 0x03, 0x07, 0x04, 0x04, 0x04, 
          0x0c, 0x06, 0x02, 0x06, 0x04, 0x0c, 0x08, 0x08, 0x0c, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 
          0x06, 0x03, 0x01, 0x01, 0x03, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x03, 0x03, 0x06, 0x06, 0x02, 
          0x02, 0x06, 0x04, 0x04, 0x0e, 0x08, 0x08, 0x08, 0x0c, 0x06, 0x06, 0x04, 0x06, 0x02, 0x06, 0x04
        };
        
        enum {
          GAME_STATE_INIT,
          GAME_STATE_PLAYING,
          GAME_STATE_OVER,
          GAME_STATE_PAUSED
        };
        
        static int16_t hiScore;
        
        //Dynamically allocated memory for the game
        
        static struct FlappyGame {
          float    birdX;
          float    birdY; 
          float    pipeX;
          float    pipeY;
          uint8_t  pipeVariant;
          float    velocity; 
          float    scrollVelocity;
          bool     maxScrollVelocityReached;
          int16_t  score;
          bool     hasScored;
          float    bgScrollVelocity;
          float    bgX;
          uint8_t  state;
        } *game = NULL;
        
        //Allocate memory if it hasn't been allocated yet
        
        if(game == NULL)
        {
          game = (struct FlappyGame *)malloc(sizeof(struct FlappyGame));
          if(game == NULL) //allocation failed
          {
            showMsg(PSTR("Failed to allocate\nmemory for game"));
            delay(2000);
            //force exit
            changeToScreen(SCREEN_MAIN_MENU);
            break;
          }
          else
          {
            game->state = GAME_STATE_INIT;
          }
        }
        
        //Run the game
        
        switch(game->state)
        {
          case GAME_STATE_INIT:
            {
              printFullScreenMsg(PSTR("Get ready"));
              if(clickedButton == KEY_SELECT || millis() - buttonReleaseTime > 1000)
              {
                //initialise variables
                game->birdX = 24.0;
                game->birdY = 11.0;
                game->velocity = 0.0;
                game->bgX = 0.0;
                game->score = 0;
                game->hasScored = false;
                game->pipeX = 127.0;
                game->pipeY = random(5 + PIPE_GAP, 51);
                game->pipeVariant = random() % PIPE_VARIANT_COUNT;
                game->scrollVelocity = 0.65;
                game->bgScrollVelocity = 0.1625;
                game->maxScrollVelocityReached = false;
                
                //change state
                game->state = GAME_STATE_PLAYING;
                killButtonEvents();
              }
            }
            break;
          
          case GAME_STATE_PLAYING:
            {
              //Calculate the vertical position of the bird
              //Here we are using the equations of linear motion, but with discrete time
              // i.e s = ut + 1/2at^2 and v = u + at
              //As our time is discrete, we treat it as 1 time unit, and can then simply
              //add the current y position to the previous y position. We also do same for velocity.
              //Now when we want to move the bird up on key event, we simply assign a fixed negative
              //value to the velocity (not realistic physics).
              
              if(pressedButton == KEY_SELECT || pressedButton == KEY_UP)
                game->velocity = -1.043; //about some constant*sqrt(GRAVITY). Constant determined to be about 4
              
              //s = ut + 1/2at^2, v = u + at, time is unity and discrete
              game->birdY += game->velocity + (GRAVITY / 2.0);
              game->velocity += GRAVITY;
              
              if(game->birdY > BIRD_YMAX)
                game->birdY = BIRD_YMAX;

              //Calculate the pipe x position
              if(game->maxScrollVelocityReached)
                game->pipeX -= game->scrollVelocity;
              else
              {
                game->pipeX -= (game->scrollVelocity + (SCROLL_ACC / 2.0));
                game->scrollVelocity += SCROLL_ACC;
                if(game->scrollVelocity > SCROLL_VELOCITY_MAX)
                  game->maxScrollVelocityReached = true;
              }
              
              // game->maxScrollVelocityReached = false; //##debug
              
              //Generate new pipe when out of view
              if(game->pipeX < 0.0 - PIPE_WIDTH)
              {
                game->pipeX = 127;
                game->pipeVariant = random() % PIPE_VARIANT_COUNT;
                if(game->pipeVariant == PIPE_VARIANT_C)
                  game->pipeY = random(5 + PIPE_GAP, 51);
                if(game->pipeVariant == PIPE_VARIANT_B)
                  game->pipeY = random(5 + PIPE_GAP, 62);
                if(game->pipeVariant == PIPE_VARIANT_A)
                  game->pipeY = random(PIPE_GAP, 51);
              }
              
              //Calc background position
              //the background is parallax scrolled
              if(game->maxScrollVelocityReached)
                game->bgX += game->bgScrollVelocity;
              else
              {
                game->bgX += (game->bgScrollVelocity + (BG_SCROLL_ACC / 2.0));
                game->bgScrollVelocity += BG_SCROLL_ACC;
              }
              
              if(game->bgX > 127)
                game->bgX = 0.0;
              
              ///--- Detect collisions ---
              
              bool collided = false; 
              
              //collision of bird with floor
              if(game->birdY >= BIRD_YMAX)
                collided = true;
              
              //collision of bird with pipe
              if(game->pipeX < (game->birdX + BIRD_LENGTH) && game->birdX < (game->pipeX + PIPE_WIDTH))
              {
                if(game->pipeVariant == PIPE_VARIANT_A || game->pipeVariant == PIPE_VARIANT_C)
                {
                  if(game->pipeY < (game->birdY + BIRD_HEIGHT))
                    collided = true;
                }
                if(game->pipeVariant == PIPE_VARIANT_B || game->pipeVariant == PIPE_VARIANT_C)
                {
                  if(game->birdY < (game->pipeY - PIPE_GAP + 1))
                    collided = true;
                }
              }
              
              // collided = false; //###Debug
              
              ///--- Calculate scores ---
              
              if(game->birdX > (game->pipeX + PIPE_WIDTH) && !game->hasScored && !collided)
              {
                game->hasScored = true;
                game->score++;
                if(game->score > hiScore)
                  hiScore = game->score;
              }
              if(game->birdX < game->pipeX)
                game->hasScored = false;
              
              ///--- Draw on the screen ---
              
              //draw the background
              uint8_t idx = game->bgX;
              for(uint8_t x = 0; x < 128; x++, idx++)
              {
                if(idx > 127)
                  idx = 0;
                uint8_t val = pgm_read_byte(bgTerrain + idx);
                for(uint8_t b = 0; b < 4; b++)
                {
                  if(val & (1 << b))
                    display.drawPixel(x, 54 + b, BLACK);
                }
              }
              
              //Draw the foreground
              display.drawHLine(0, 62, 128, BLACK);
              int16_t gx = game->pipeX;
              while(gx >= 0)
              {
                display.drawPixel(gx, 63, BLACK);
                display.drawPixel(gx + 1, 63, BLACK);
                gx -= 6;
              }
              gx = game->pipeX;
              while(gx < 128)
              {
                display.drawPixel(gx, 63, BLACK);
                display.drawPixel(gx + 1, 63, BLACK);
                gx += 6;
              }
              
              //Draw the pipe
              if(game->pipeVariant == PIPE_VARIANT_A || game->pipeVariant == PIPE_VARIANT_C)
              {
                //lower pipe
                display.drawRect(game->pipeX, game->pipeY, PIPE_WIDTH, 4, BLACK);
                display.fillRect(game->pipeX + 1, game->pipeY + 3, PIPE_WIDTH - 2, 60 - game->pipeY, WHITE);
                display.drawRect(game->pipeX + 1, game->pipeY + 3, PIPE_WIDTH - 2, 60 - game->pipeY, BLACK);
              }
              if(game->pipeVariant == PIPE_VARIANT_B || game->pipeVariant == PIPE_VARIANT_C)
              {
                //upper pipe
                display.drawRect(game->pipeX, game->pipeY - PIPE_GAP - 3, PIPE_WIDTH, 4, BLACK);
                display.drawVLine(game->pipeX + 1, 0, game->pipeY - PIPE_GAP - 3, BLACK);
                display.drawVLine(game->pipeX + PIPE_WIDTH - 2, 0, game->pipeY - PIPE_GAP - 3, BLACK);
              }
              
              //Draw the bird
              display.fillRect(game->birdX, game->birdY, BIRD_LENGTH, BIRD_HEIGHT, WHITE);
              drawAnimatedSprite(game->birdX, game->birdY, animation_bird, BIRD_LENGTH, BIRD_HEIGHT, BLACK, 3, 80, 0);
              
              //Draw the score
              display.setCursor(0, 0);
              display.print(game->score);
              
              //###Debug draw
              // display.setCursor(0, 8);
              // display.print(game->scrollVelocity);
              // display.setCursor(0, 16);
              // display.print(game->bgScrollVelocity);
              
              ///--- Play audio ---
              /* 
              if(audioToPlay = AUDIO_KEY_PRESSED) //always suppress key press tones
                audioToPlay = AUDIO_NONE;
              if(game->hasScored)
                audioToPlay = AUDIO_NONE; //replace with actual
              if(collided)
                audioToPlay = AUDIO_NONE; //replace with actual 
              */

              ///--- Change game state ---
              
              //change state if collided
              if(collided)
              {
                game->state = GAME_STATE_OVER;
                killButtonEvents();
              }
            }
            break;
          
          case GAME_STATE_OVER:
            {
              drawBoundingBox(11, 14, 105, 35,BLACK);
              display.setCursor(17, 18);
              display.print(F("Game over"));
              display.setCursor(17, 28);
              display.print(F("Score:   "));
              display.print(game->score);
              display.setCursor(17, 38);
              display.print(F("HiScore: "));
              display.print(hiScore);
              
              if(clickedButton == KEY_SELECT)
              {
                game->state = GAME_STATE_INIT;
                killButtonEvents();
              }
            }
            break;
        }
        
        //Exit the game, freeing the allocated memory
        if(heldButton == KEY_SELECT)
        {
          free(game); //free the memory
          game = NULL; //set the pointer to NULL to avoid dangling pointer issues
          changeToScreen(SCREEN_MAIN_MENU);
        }
      }
      break;
    
    ////////////////////////////////// GENERIC TEXT VIEWER /////////////////////////////////////////
    
    case SCREEN_TEXT_VIEWER:
      {
        static uint16_t startPos;

        static bool initialised = false;
        if(!initialised)
        {
          initialised = true;
          startPos = 0;
        }
        
        uint16_t pos = startPos; //position in string
        bool isEnd = false;
        
        //Print the text with both line wrap and word wrap. Word wrap prioritized. 
        //Doesn't cater for all edge cases, but works good enough.
        uint8_t line = 0, i = 0;
        uint8_t numPrintedChars = 0; //tracks how many characters we've skipped
        while(line < 7 && !isEnd)
        {
          while(i < 21)
          {
            char c = pgm_read_byte(textViewerText + pos);
            //Check for end of text
            if(c == '\0')
            {
              isEnd = true;
              break;
            }
            //Skip carriage return, or if we are at start of line and the character is a space
            if(c == '\r' || (i == 0 && c == ' '))
            {
              pos++; //advance position in string
              continue;
            }
            //Check if its a new line character
            if(c == '\n')
            {
              pos++; //advance position in string
              break;
            }
            //Figure out how long the word is. If it is longer than the space left on the 
            //line, force it onto a new line. If the word is longer than the total space on the line, 
            //just print anyway. 
            if(pgm_read_byte(textViewerText + pos - 1) == ' ' || pos == 0) //start of word, prev character is a space
            {
              uint8_t wordLen = 0;
              uint8_t j = 0;
              char wordCharacter;
              while((wordCharacter = pgm_read_byte(textViewerText + pos + j)) != ' ')
              {
                if(wordCharacter == '\0' || wordCharacter == '\r' || wordCharacter == '\n')
                  break;
                wordLen++;
                j++;
              }
              uint8_t remainingSpace = 20 - i;
              if(wordLen > remainingSpace && wordLen <= 21)
                break;
            }
            //Write the character to the screen
            display.setCursor(1 + i*6, 4 + line*8);
            display.write(c);
            numPrintedChars++;
            pos++; //advance position in string
            i++; //advance position in line
          }
          
          line++; //advance line
          i = 0; //reset position in line
        }
        
        //Show arrow icons
        if(!isEnd)
          display.drawBitmap(61, 61, icon_down_arrow_small, 5, 3, BLACK);
        if(pos >= 147) //21*7
          display.drawBitmap(61, 0, icon_up_arrow_small, 5, 3, BLACK);
         
        //Handle scrolling down
        if(pressedButton == KEY_DOWN)
        {
          if(!isEnd)
            startPos = pos;
        }
        //Handle scrolling up. Not terribly accurate for now. To make it accurate, we would need to have an 
        //array that keeps track of all the number of printed characters per page. May not be worth 
        //the extra memory usage.
        if(pressedButton == KEY_UP)
        {
          int16_t _start = startPos;
          _start = _start - 147;
          if(_start < 0)
            _start = 0;
          startPos = _start;
        }
        
        //Exit
        if(heldButton == KEY_SELECT)
        {
          initialised = false;
          changeToScreen(lastScreen);
        }
      }
      break;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    default:
      changeToScreen(SCREEN_HOME);
  }

  //-------------- Custom notifications -----------------
  notificationHandler();

  //-------------- Toasts -------------------------------
  drawToast();
  
  //-------------- Debug print --------------------------
  if(Sys.DBG_showLoopTime)
  {
    // display.fillRect(102, 55, 26, 9, WHITE);
    display.setCursor(103, 56);
    uint16_t tt = DBG_loopTime/100;
    display.print(tt/10);
    display.print(F(".")); 
    display.print(tt%10); 
  }

  //-------------- Show on physical lcd -----------------
  if(Sys.DBG_disableInterlacing) //override
    display.setInterlace(false);
  display.display(); 
  display.clearDisplay();

  //-------------- Sound --------------------------------
  playTones();
}

//============================ Helpers =============================================================

void toggleEditModeOnSelectClicked()
{
  if(clickedButton == KEY_SELECT)
    isEditMode = !isEditMode;
}

//--------------------------------------------------------------------------------------------------

void changeFocusOnUpDown(uint8_t numItems)
{
  if(isEditMode)
    return;
  isEditMode = true;
  focusedItem = incDec(focusedItem, numItems, 1, INCDEC_WRAP, INCDEC_SLOW);
  isEditMode = false;
}

//--------------------------------------------------------------------------------------------------

void changeToScreen(uint8_t scrn)
{
  lastScreen = theScreen;
  theScreen = scrn;
  focusedItem = 1;
  isEditMode = false;
  contextMenuTopItem = 1;
  contextMenuFocusedItem = 1;
  contextMenuSelectedItemID = 0xff;
  killButtonEvents();
}

//--------------------------------------------------------------------------------------------------

void printModelName(char* buff, uint8_t modelIdx)
{
  if(isEmptyStr(buff, sizeof(Model.name)))
  {
    display.print(F("Model"));
    display.print(modelIdx + 1); //add 1 as Model0 for example doesn't make sense to the user
  }
  else
    display.print(buff);
}

//--------------------------------------------------------------------------------------------------

void printFullScreenMsg(const char* str)
{
  uint8_t pos = 0; //position in string
  uint8_t numTextLines = 1;
  //get number of lines
  while(pgm_read_byte(str + pos) != '\0')
  {
    if(pgm_read_byte(str + pos) == '\n')
      numTextLines++;
    pos++;
  }
  pos = 0; //reset
  uint8_t ypos = (display.height() - numTextLines * 9) / 2 + 1; //9 is line height
  for(uint8_t line = 0; line < numTextLines; line++)
  {
    //get number of characters in the line
    uint8_t numChars = 0;
    while(pgm_read_byte(str + pos) != '\n' && pgm_read_byte(str + pos) != '\0')
    {
      numChars++;
      pos++;
    }
    pos -= numChars;
    //center text
    int16_t xpos = (display.width() - numChars * 6) / 2;
    display.setCursor(xpos, ypos);
    //write the characters 
    while(numChars--)
      display.write(pgm_read_byte(str + pos++));
    //advance 
    ypos += 9; 
    pos++;
  }
}

//--------------------------------------------------------------------------------------------------

void printHHMMSS(uint32_t millisecs)
{
  //Prints the time as mm:ss or hh:mm:ss
  uint32_t hh, mm, ss;
  ss = millisecs / 1000;
  hh = ss / 3600;
  ss = ss - hh * 3600;
  mm = ss / 60;
  ss = ss - mm * 60;
  if(hh > 0)
  {
    display.print(hh);
    display.print(F(":"));
  }
  if(mm < 10)
    display.print(F("0"));
  display.print(mm);
  display.print(F(":"));
  if(ss < 10)
    display.print(F("0"));
  display.print(ss);
}

//--------------------------------------------------------------------------------------------------

void printTimerValue(uint8_t idx)
{
  if(Model.Timer[idx].initialMinutes == 0) //a count up timer
    printHHMMSS(timerElapsedTime[idx]);
  else //a count down timer
  {
    uint32_t initMillis = Model.Timer[idx].initialMinutes * 60000UL;
    if(timerElapsedTime[idx] < initMillis)
    {
      uint32_t ttqq = initMillis - timerElapsedTime[idx];
      printHHMMSS(ttqq + 999); //add 999ms so the displayed time doesnt 
      //change immediately upon running the timer
    }
    else
    {
      uint32_t ttqq = timerElapsedTime[idx] - initMillis;
      if(ttqq >= 1000) //prevents displaying -00:00
        display.print(F("-"));
      printHHMMSS(ttqq);
    }
  }
}

//--------------------------------------------------------------------------------------------------

void printVoltage(int16_t millivolts)
{
  int16_t val = millivolts / 10;
  if(val < 0)
  {
    display.print(F("-"));
    val = -val;
  }
  display.print(val / 100);
  display.print(F("."));
  val = val % 100;
  if(val < 10) 
    display.print(F("0"));
  display.print(val);
  display.print(F("V"));
}

//--------------------------------------------------------------------------------------------------

void printSeconds(uint16_t decisecs)
{
  display.print(decisecs / 10);
  display.print(F("."));
  display.print(decisecs % 10);
  display.print(F("s"));
}

//--------------------------------------------------------------------------------------------------

void drawDottedVLine(uint8_t x, uint8_t y, uint8_t len, uint8_t fgColor, uint8_t bgColor)
{
  display.drawVLine(x, y, len, bgColor);
  for(uint8_t i = 0; i < len; i += 2)
    display.drawPixel(x, y+i, fgColor);
}

//--------------------------------------------------------------------------------------------------

void drawDottedHLine(uint8_t x, uint8_t y, uint8_t len, uint8_t fgColor, uint8_t bgColor)
{
  display.drawHLine(x, y, len, bgColor);
  for(uint8_t i = 0; i < len; i += 2)
    display.drawPixel(x+i, y, fgColor);
}

//--------------------------------------------------------------------------------------------------

void drawCursor(uint8_t xpos, uint8_t ypos)
{
  if(isEditMode) //draw blinking cursor
  {
    if((millis() - buttonReleaseTime) % 1000 < 500 || buttonCode == KEY_UP || buttonCode == KEY_DOWN)
      display.fillRect(xpos + 3, ypos - 1, 2, 9, BLACK);
  }
  else 
    display.drawBitmap(xpos, ypos, icon_pointer, 6, 7, BLACK); //draw arrow
}

//--------------------------------------------------------------------------------------------------

void drawHeader(const char* str)
{
  strlcpy_P(txtBuff, str, sizeof(txtBuff));
  uint8_t txtWidthPix = strlen(txtBuff) * 6;
  uint8_t headingXOffset = (display.width() - txtWidthPix) / 2; //middle align heading
  display.setCursor(headingXOffset, 0);
  display.print(txtBuff);
  display.drawHLine(0, 3, headingXOffset - 2, BLACK);
  display.drawHLine(headingXOffset + txtWidthPix + 1, 3, 128 - (headingXOffset + txtWidthPix + 1), BLACK);
}

//--------------------------------------------------------------------------------------------------

void drawMenu(char const list[][20], uint8_t numItems, const uint8_t *const bitmapTable[], 
              uint8_t *topItem, uint8_t *highlightedItem)
{
  uint8_t numVisible = 5;
  uint8_t lineHeight = 11;
  uint8_t y0 = 11;
  
  //handle scenario of being called with invalid highlightedItem
  if(*highlightedItem > numItems) 
    *highlightedItem = 1;
  
  //handle navigation
  isEditMode = true;
  *highlightedItem = incDec(*highlightedItem, numItems, 1, INCDEC_WRAP, INCDEC_SLOW);
  isEditMode = false;
  if(*highlightedItem < *topItem)
    *topItem = *highlightedItem;
  while(*highlightedItem >= *topItem + numVisible)
    (*topItem)++;
  
  //fill menu slots
  for(uint8_t i = 0; i < numVisible && i < numItems; i++)
  {
    //prevent showing garbage entries when we've changed to using denser menus
    if(*topItem + i > numItems)
      break;
    
    uint8_t ypos = y0 + i * lineHeight;
    //highlight selection
    if(*highlightedItem == (*topItem + i))
    {
      display.fillRoundRect(3, ypos - 2, 122, lineHeight, Sys.useRoundRect ? 4 : 0, BLACK);
      display.setTextColor(WHITE);
    }
    //show icons
    if(bitmapTable != NULL && Sys.showMenuIcons)
    {
      display.drawBitmap(7, ypos - 2 , (uint8_t *)pgm_read_word(&bitmapTable[(*topItem + i) - 1]),
                         15, 11, *highlightedItem == (*topItem + i) ? WHITE : BLACK);
      display.setCursor(26, ypos);
    }
    else
      display.setCursor(10, ypos);
    //show text
    strlcpy_P(txtBuff, list[*topItem + i - 1], sizeof(txtBuff));
    display.print(txtBuff);
    display.setTextColor(BLACK);
  }
  
  //scroll bar
  drawScrollBar(127, 9, numItems, *topItem, numVisible, numVisible * lineHeight);
}

//--------------------------------------------------------------------------------------------------

void drawCheckbox(uint8_t xpos, uint8_t ypos, bool val)
{
  display.drawBitmap(xpos, ypos, val ? icon_checkbox_checked : icon_checkbox_unchecked, 7, 7, BLACK);
}

//--------------------------------------------------------------------------------------------------

void drawLoaderSpinner(uint8_t xpos, uint8_t ypos, uint8_t size)
{
  //active cells on a 4x4 grid. Here, every 3 bytes is a frame
  static const uint8_t activeCells[] PROGMEM = {
    1, 2, 7, 2, 7, 11, 7, 11, 14, 11, 14, 13, 14, 13, 8, 13, 8, 4, 8, 4, 1, 4, 1, 2
  };
  const uint16_t frameTime = 60;
  uint8_t frameIdx = (millis() % (8 * frameTime)) / frameTime; //total of 8 frames
  for(uint8_t i = 0; i < 3; i++)
  {
    uint8_t cell = pgm_read_byte(&activeCells[(frameIdx * 3) + i]);
    uint8_t x = xpos + size * (cell % 4);
    uint8_t y = ypos + size * (cell / 4);
    display.fillRect(x, y, size, size, BLACK);
  }
}

//--------------------------------------------------------------------------------------------------

void drawAnimatedSprite(uint8_t x, uint8_t y, const uint8_t* const bitmapTable[], uint8_t w, uint8_t h, uint8_t color, 
                        uint8_t frameCount, uint16_t frameTime, uint32_t timeOffset)
{
  uint8_t frameIdx = ((millis() - timeOffset) % (frameCount * frameTime)) / frameTime; 
  if(!Sys.animationsEnabled)
    frameIdx = 0;
  display.drawBitmap(x, y, (uint8_t *)pgm_read_word(&bitmapTable[frameIdx]), w, h, color);
}

//--------------------------------------------------------------------------------------------------

void drawScrollBar(uint8_t xpos, uint8_t ypos, uint16_t numItems, uint16_t topItem, uint16_t numVisible, uint16_t viewportHeight)
{ 
  if(numItems > numVisible)
  {
    uint8_t barHeight = divRoundClosest((int32_t)viewportHeight * numVisible, numItems);
    //limit barHeight to 5px minimum
    uint16_t _viewportHeight = viewportHeight;
    if(barHeight < 5)
    {      
      _viewportHeight -= (5 - barHeight);
      barHeight = 5;
    }
    uint8_t barYpos = ypos + divRoundClosest((int32_t)_viewportHeight * (topItem - 1), numItems);
    //Restrict the scroll bar to being drawn inside the view port. 
    //Necessary due to rounding errors that may have happened.
    if((barYpos + barHeight) > (ypos + viewportHeight))
      barYpos -= 1;
    //draw
    display.drawVLine(xpos, barYpos, barHeight, BLACK);
  }
}

//--------------------------------------------------------------------------------------------------

void drawTrimSlider(uint8_t x, uint8_t y, int8_t val, uint8_t range, bool horizontal)
{
  if(horizontal)
  {
    display.drawHLine(x, y, range + 1, BLACK);
    x = ((int16_t) x + range/2 - 3) + val;
    display.fillRect(x, y - 3, 7, 7, WHITE);
    display.drawRoundRect(x, y - 3, 7, 7, 2, BLACK);
    if(val >= 0) display.drawVLine(x + 4, y - 1, 3, BLACK);
    if(val <= 0) display.drawVLine(x + 2, y - 1, 3, BLACK);
  }
  else
  {
    display.drawVLine(x, y, range + 1, BLACK);
    y = ((int16_t) y + range/2 - 3) - val;
    display.fillRect(x - 3, y, 7, 7, WHITE);
    display.drawRoundRect(x - 3, y, 7, 7, 2, BLACK);
    if(val >= 0) display.drawHLine(x - 1, y + 2, 3, BLACK);
    if(val <= 0) display.drawHLine(x - 1, y + 4, 3, BLACK);
  }
}

//--------------------------------------------------------------------------------------------------

void drawToast()
{
  if(toastExpired)
    return;
  
  //Table for easing the slide up/down animation.
  //t ranges from 0 to 1. Here it is actually the ratio (currTime - startTime)/transitionDuration
  //Values have been multiplied by 10 (distance is 10 pixels) and rounded to zero decimal places.
  static const uint8_t transitionLUT[] PROGMEM = {
    // 0,2,4,5,6,7,8,9,10,10//quadratic 1-pow(1-t,2)
    // 0,1,3,4,5,6,7,7,8,8,9,9,9,10,10,10,10,10,10,10,10 //cubic 1-pow(1-t,3) 
    0,2,4,6,7,8,9,9,10,10,10,10,10,10,10,10 //quartic 1-pow(1-t,4)
  };
  
  uint8_t numElem = sizeof(transitionLUT)/sizeof(transitionLUT[0]);
  const int16_t transitionDuration = 400; //in milliseconds
  uint32_t currTime = millis();
  uint32_t sttTime = toastStartTime;
  uint32_t endTime = toastEndTime + transitionDuration * 2;
  
  if(currTime >= sttTime && currTime < endTime)
  {
    uint8_t ypos = 53;
    if(Sys.animationsEnabled)
    {
      uint8_t idxLUT = numElem - 1;
      if(currTime < sttTime + transitionDuration)
        idxLUT = ((currTime - sttTime) * (numElem - 1)) / transitionDuration;
      else if((currTime + transitionDuration) > endTime) //slide down
        idxLUT = ((endTime - currTime) * (numElem - 1)) / transitionDuration;
      ypos = 63 - pgm_read_byte(&transitionLUT[idxLUT]);
    }
    uint8_t txtWidthPix = 6 * strlen_P((const char*)toastText); 
    uint8_t xpos = (display.width() - txtWidthPix) / 2; //middle align text
    if(xpos < 5)
      xpos = 5;
    display.drawRoundRect(xpos - 5, ypos, txtWidthPix + 9, 11, Sys.useRoundRect ? 4 : 0, WHITE);
    display.fillRoundRect(xpos - 4, ypos + 1, txtWidthPix + 7, 9, Sys.useRoundRect ? 3 : 0, BLACK);
    display.setTextColor(WHITE);
    display.setCursor(xpos, ypos + 2);
    strlcpy_P(txtBuff, toastText, sizeof(txtBuff));
    display.print(txtBuff);
    display.setTextColor(BLACK);
  }
  else if(currTime > endTime)
  {
    toastExpired = true;
  }
}

//--------------------------------------------------------------------------------------------------

void drawBoundingBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
  if(Sys.useRoundRect)
  {
    display.drawHLine(x + 4, y, w - 8, color);
    display.drawHLine(x + 4 , y + h - 1, w - 8, color);
    display.drawVLine(x, y + 4, h - 8, color);
    display.drawVLine(x + w - 1, y + 4, h - 8, color);
    //draw the four 'round' corners
    uint8_t xx, yy;
    for(uint8_t i = 0; i < 4; i++)
    {
      xx = (i % 2) ? (x + w - 4) : (x + 2);
      yy = (i < 2) ? (y + 1) : (y + h - 2); 
      display.drawHLine(xx, yy, 2, color);
      display.drawVLine((i % 2) ? (xx + 2) : (xx - 1), (i < 2) ? (yy + 1) : (yy - 2), 2, color);
    }
  }
  else
    display.drawRect(x, y, w, h, color);
}

//--------------------------------------------------------------------------------------------------

void drawDialogCopyMove(const char* str, uint8_t srcIdx, uint8_t destIdx, bool isCopy)
{
  drawBoundingBox(11, 14, 105, 35,BLACK);
  display.setCursor(17, 18);
  if(isCopy) display.print(F("Copy "));
  else display.print(F("Move "));
  strlcpy_P(txtBuff, str, sizeof(txtBuff));
  display.print(txtBuff);
  display.print(srcIdx + 1); 
  display.setCursor(17, 28);
  display.print(F("to:"));
  display.setCursor(47, 28);
  display.print(txtBuff);
  display.print(destIdx + 1);
  drawCursor(39, 28);
}

//--------------------------------------------------------------------------------------------------

void drawHorizontalBarChart(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color, int16_t val, int16_t valMin, int16_t valMax)
{
  display.drawRect(x, y, w, h, color);
  if(val < valMin) val = valMin;
  if(val > valMax) val = valMax;
  if(valMin > valMax) return;
  if(valMax - valMin == 0) return;
  w = divRoundClosest(((int32_t)w - 2)* (val - valMin), valMax - valMin);
  display.fillRect(x+1, y, w, h, color);
}

//--------------------------------------------------------------------------------------------------

void drawHorizontalBarChartZeroCentered(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color, int16_t val, int16_t range)
{
  int8_t xCenter = x + w/2;
  if(range != 0)
  {
    if(range < 0) range = -range;
    if(val > 0)
    {
      if(val > range/2) val = range/2;
      int8_t xLen = (val*w)/range;
      display.fillRect(xCenter, y, xLen + 1, h, color);
    }
    else if(val < 0)
    {
      val = -val;
      if(val > range/2) val = range/2;
      int8_t xLen = (val*w)/range;
      display.fillRect(xCenter - xLen, y, xLen, h, color);
    }
  }
  drawDottedHLine(x, y + h/2, w, color, !color);
  display.drawVLine(xCenter, y, h, color);
}

//--------------------------------------------------------------------------------------------------

void drawCustomCurve(custom_curve_t *crv, uint8_t selectPt, uint8_t src)
{
  //--- draw axes
  display.drawVLine(100, 11, 51, BLACK);
  display.drawHLine(75, 36, 51, BLACK);

  //--- draw cross hairs
  if(src != SRC_NONE)
  {
    int16_t xQQ[MAX_NUM_POINTS_CUSTOM_CURVE];
    int16_t yQQ[MAX_NUM_POINTS_CUSTOM_CURVE];
    for(uint8_t pt = 0; pt < crv->numPoints; pt++)
    {
      xQQ[pt] = 5 * crv->xVal[pt];
      yQQ[pt] = 5 * crv->yVal[pt];
    }
    drawDottedVLine(100 + mixSources[src]/20, 11, 51, BLACK, WHITE);
    int8_t y;
    if(crv->smooth) 
      y = cubicHermiteInterpolate(xQQ, yQQ, crv->numPoints, mixSources[src]) / 20;
    else 
      y = linearInterpolate(xQQ, yQQ, crv->numPoints, mixSources[src]) / 20;
    drawDottedHLine(75, 36 - y, 51, BLACK, WHITE);
  }

  //--- plot graph. Plot area is 50x50 px
  if(crv->smooth)
  {
    //We cache the y cordinates so we dont have to recompute unnecessarily.
    static int8_t yCoord[51]; //cache. 51 points to plot in total
    
    bool changed = false;
    //Instead of wasting RAM storing all previous xVal and yVal, we simply store a 
    //checksum of the curve struct data that has been passed to this function. 
    //We then compare the current checksum to the previous checksum.
    //We also compare pointers to the curve struct, just in case there were collisions
    static uint8_t lastChecksum;
    uint8_t checksum = crc8((uint8_t*)crv, sizeof(Model.CustomCurve[0]));
    if(checksum != lastChecksum)
    {
      lastChecksum = checksum;
      changed = true;
    }
    static custom_curve_t *lastCrv = NULL;
    if(lastCrv != crv)
    {
      lastCrv = crv;
      changed = true;
    }
    
    //recalculate if changed
    if(changed)
    {
      int16_t xQQ[MAX_NUM_POINTS_CUSTOM_CURVE];
      int16_t yQQ[MAX_NUM_POINTS_CUSTOM_CURVE];
      for(uint8_t pt = 0; pt < crv->numPoints; pt++)
      {
        xQQ[pt] = 5 * crv->xVal[pt];
        yQQ[pt] = 5 * crv->yVal[pt];
      }
      //compute yCoord
      for(int8_t xCoord = -25; xCoord <= 25; xCoord++)
      {
        uint8_t i = 25 + xCoord;
        yCoord[i] = cubicHermiteInterpolate(xQQ, yQQ, crv->numPoints, xCoord * 20) / 20; 
      }
    }
    
    //plot
    for(uint8_t pt = 0; pt < crv->numPoints - 1; pt++)
    {
      int8_t xStart = crv->xVal[pt]/4;
      int8_t xEnd   = crv->xVal[pt+1]/4;
      int8_t yStart = crv->yVal[pt]/4;
      int8_t yEnd   = crv->yVal[pt+1]/4;
      
      //exit if values are invalid
      if(xStart < -25 || xStart > 25 || xEnd < -25 || xEnd > 25) 
        break;
      
      if(xEnd - xStart <= 1)
        display.drawLine(100 + xStart, 36 - yStart, 100 + xEnd, 36 - yEnd, BLACK);
      else
      {
        for(int8_t xCoord = xStart; xCoord <= xEnd; xCoord++)
        {
          uint8_t i = 25 + xCoord;
          //If the difference between successive y coordinates is more than 1 pixel then draw a line 
          //between the two points to make the graph visually continuous (not broken)
          if(xCoord == xStart)
            yCoord[i] = yStart;
          if(xCoord == xEnd)
            yCoord[i] = yEnd;
          if(xCoord > xStart && (abs(yCoord[i] - yCoord[i-1]) > 1))
            display.drawLine(100 + xCoord - 1, 36 - yCoord[i-1], 100 + xCoord, 36 - yCoord[i], BLACK); 
          else
            display.drawPixel(100 + xCoord, 36 - yCoord[i], BLACK);
        }
      }
    }
  }
  else
  {
    //linear interpolate between points.
    //here we simply draw straight lines between the points.
    for(uint8_t pt = 0; pt < crv->numPoints - 1; pt++)
    {
      display.drawLine(100 + crv->xVal[pt]/4, 36 - crv->yVal[pt]/4, 
                       100 + crv->xVal[pt+1]/4, 36 - crv->yVal[pt+1]/4, BLACK);
    }
  }
  
  //--- mark nodes, show node we've selected
  for(uint8_t pt = 0; pt < crv->numPoints; pt++)
  {
    //mark nodes
    display.fillRect(99 + crv->xVal[pt]/4, 35 - crv->yVal[pt]/4 ,3, 3, BLACK);
    display.drawPixel(100 + crv->xVal[pt]/4, 36 - crv->yVal[pt]/4, WHITE);
    //show selected node
    if(pt == selectPt)
    {
      display.fillRect(98 + crv->xVal[pt]/4, 34 - crv->yVal[pt]/4, 5, 5, WHITE);
      display.drawRect(98 + crv->xVal[pt]/4, 34 - crv->yVal[pt]/4, 5, 5, BLACK);
    }
  }
}

//--------------------------------------------------------------------------------------------------

void drawTelemetryValue(uint8_t xpos, uint8_t ypos, uint8_t idx, int16_t rawVal, bool blink)
{  
  if(rawVal == TELEMETRY_NO_DATA)
  {
    display.setCursor(xpos, ypos);
    display.print(F("No data"));
    return;
  }
  if(blink && telemetryAlarmState[idx] && (millis() % 1000 > 700)) //flashing effect
    return;
  int32_t tVal = ((int32_t) rawVal * Model.Telemetry[idx].multiplier) / 100;
  tVal += Model.Telemetry[idx].offset;
  printTelemParam(xpos, ypos, idx, tVal);
  display.print(Model.Telemetry[idx].unitsName);
}

//--------------------------------------------------------------------------------------------------

void printTelemParam(uint8_t xpos, uint8_t ypos, uint8_t idx, int32_t val)
{
  uint8_t prec = Model.Telemetry[idx].factor10 >= 0 ? 0 : abs(Model.Telemetry[idx].factor10);
  bool isNeg = false;
  bool isDivide = false;
  int16_t f = 1;
  if(Model.Telemetry[idx].factor10 < 0) isDivide = true;
  if(abs(Model.Telemetry[idx].factor10) == 1) f = 10;
  if(abs(Model.Telemetry[idx].factor10) == 2) f = 100;
  if(val < 0)
  {
    isNeg = true;
    val = -val;
  }
  int32_t whole = 0, frac = 0, rem = 0;
  if(isDivide)
  {
    whole = val / f;
    rem = val % f;
    if(prec == 1) frac = (10 * rem) / f;
    if(prec == 2) frac = (100 * rem) / f;
  }
  else
    whole = val * f; //could overflow
  
  display.setCursor(xpos, ypos);
  if(isNeg)
    display.print(F("-"));
  display.print(whole);
  if(prec > 0)
  {
    display.print(F("."));
    if(prec == 2 && frac < 10) 
      display.print(F("0"));
    display.print(frac);
  }
}

//--------------------------------------------------------------------------------------------------

void editTextDialog(const char* title, char* buff, uint8_t lenBuff, bool allowEmpty, bool trimStr, bool isSecureMode)
{
  static uint8_t charPos = 0;
  uint8_t thisChar = *(buff + charPos);
  if(thisChar == 0) //handle partial string
  {
    thisChar = 32;
    //write a null at next position so we don't see leftover garbage strings
    *(buff + charPos + 1) = '\0';
  }
  
  //--- draw ---
  drawBoundingBox(11, 14, 105, 35,BLACK);
  display.setCursor(17, 18);
  strlcpy_P(txtBuff, title, sizeof(txtBuff));
  display.print(txtBuff);
  
  uint8_t txtWidthPix = (lenBuff - 1) * 6;
  uint8_t xpos = (display.width() - txtWidthPix) / 2; //middle align
  display.setCursor(xpos, 28);
  for(uint8_t i = 0; i < lenBuff; i++)
  {
    uint8_t c = *(buff + i);
    if(c == '\0')
      break;
    if(isSecureMode && charPos != i) // mask/hide entered characters
      display.write(0x20); 
    else
      display.write(c);
  }
  
  display.drawHLine(xpos, 37, (6 * (lenBuff - 1)) - 1, BLACK);
  
  //draw blinking cursor
  if((millis() - buttonReleaseTime) % 1000 < 500 || buttonCode > 0)
    display.fillRect(xpos + 6 * charPos, 36, 5, 1, BLACK);
  
  //---- map characters ---
  // Z to A  (ascii 90 to 65)  --> 0 to 25
  // space   (ascii 32)        --> 26
  // a to z  (ascii 97 to 122) --> 27 to 52
  // Numbers (ascii 48 to 57)  --> 53 to 62
  // - . /   (ascii 45 to 47)  --> 63 to 65
  // % sign  (ascii 37)        --> 66
  // Degree sign (ascii 248)   --> 67

  if(thisChar == 32) thisChar = 26;
  else if(thisChar == 37)  thisChar = 66;
  else if(thisChar <= 47)  thisChar += 18;
  else if(thisChar <= 57)  thisChar += 5;
  else if(thisChar <= 90)  thisChar = 90 - thisChar ;
  else if(thisChar <= 122) thisChar -= 70;
  else if(thisChar == 248) thisChar = 67;
  
  //adjust
  isEditMode = true;
  thisChar = incDec(thisChar, 67, 0, INCDEC_NOWRAP, INCDEC_SLOW);

  //map back
  if(thisChar <= 25) thisChar = 90 - thisChar;
  else if(thisChar == 26) thisChar = 32; 
  else if(thisChar <= 52) thisChar += 70;
  else if(thisChar <= 62) thisChar -= 5;
  else if(thisChar <= 65) thisChar -= 18;
  else if(thisChar == 66) thisChar = 37;
  else if(thisChar == 67) thisChar = 248;

  //write
  *(buff + charPos) = thisChar;
  
  //change to next or prev character
  if(clickedButton == KEY_SELECT)
    charPos++;
  else if(heldButton == KEY_SELECT && charPos > 0)
  {
    //move to prev character if not secure mode
    if(!isSecureMode)
      charPos--;
    killButtonEvents();
  }
  
  //clear text
  if(charPos == 0 && heldButton == KEY_SELECT && millis() - buttonStartTime >= 1000)
  {
    killButtonEvents();
    *buff = '\0';
  }
  
  //done, exit 
  if(charPos == (lenBuff - 1))
  {
    charPos = 0;
    if(isEmptyStr(buff, lenBuff) && !allowEmpty)
      makeToast(PSTR("Can't be empty"), 2000, 0);
    else
    {
      isEditTextDialog = false;
      isEditMode = false;
      if(trimStr) 
        trimWhiteSpace(buff, lenBuff);
    }
  }
}

//--------------------------------------------------------------------------------------------------

void validatePassword(uint8_t nextScreen, uint8_t prevScreen)
{
  isEditTextDialog = true;
  static char enteredPassword[sizeof(Sys.password)];
  editTextDialog(PSTR("Enter password"), enteredPassword, sizeof(Sys.password), true, false, true);
  if(!isEditTextDialog) //exited
  {
    if(strcmp(enteredPassword, Sys.password) == 0)
    {
      changeToScreen(nextScreen);
      enteredPassword[0] = '\0';
      mainMenuLocked = false;
    }
    else
    {
      if(!isEmptyStr(enteredPassword, sizeof(Sys.password)))
        makeToast(PSTR("Incorrect password"), 2000, 0);
      changeToScreen(prevScreen);
      enteredPassword[0] = '\0';
    }
  }
}

//--------------------------------------------------------------------------------------------------

void drawNotificationOverlay(uint8_t idx)
{
  display.fillRect(24, 24, 80, 16, WHITE);
  display.drawRoundRect(25, 25, 78, 14, Sys.useRoundRect ? 3 : 0, BLACK);
  uint8_t txtWidthPix = 6 * strlen(Model.CustomNotification[idx].text); 
  uint8_t xpos = 36 + (66 - txtWidthPix) / 2; //middle align text
  display.setCursor(xpos, 28);
  display.print(Model.CustomNotification[idx].text);
  display.drawBitmap(26, 26, icon_info, 10, 12, BLACK);
}

//--------------------------------------------------------------------------------------------------

#define _CONTEXT_MENU_MAX_ITEMS  16
const char* _contextMenuItems[_CONTEXT_MENU_MAX_ITEMS];
uint8_t     _contextMenuItemIDs[_CONTEXT_MENU_MAX_ITEMS];
uint8_t     _contextMenuItemCount = 0;

void contextMenuInitialise()
{
  _contextMenuItemCount = 0;
}

void contextMenuAddItem(const char* str, uint8_t itemID)
{
  if(_contextMenuItemCount < _CONTEXT_MENU_MAX_ITEMS)
  {
    _contextMenuItems[_contextMenuItemCount] = str;
    _contextMenuItemIDs[_contextMenuItemCount] = itemID;
    _contextMenuItemCount++;
  }
}

uint8_t contextMenuGetItemCount()
{
  return _contextMenuItemCount;
}

void contextMenuDraw()
{
  if(_contextMenuItemCount == 0)
    return;
  
  //--- scrollable list
  
  const uint8_t maxVisible = 5;
  
  //handle navigation
  isEditMode = true;
  contextMenuFocusedItem = incDec(contextMenuFocusedItem, _contextMenuItemCount, 1, INCDEC_WRAP, INCDEC_SLOW);
  isEditMode = false;
  if(contextMenuFocusedItem < contextMenuTopItem)
    contextMenuTopItem = contextMenuFocusedItem;
  while(contextMenuFocusedItem >= contextMenuTopItem + maxVisible)
    contextMenuTopItem++;

  //Calculate y coord for text item 0. Items are center aligned
  uint8_t numVisible = _contextMenuItemCount <= maxVisible ? _contextMenuItemCount : maxVisible;
  uint8_t y0 = ((display.height() - (numVisible * 10)) / 2) + 1;  //10 is line height
  
  //draw bounding box
  drawBoundingBox(11, y0 - 4, 105, numVisible * 10 + 5, BLACK);  
  
  //fill list
  for(uint8_t line = 0; line < numVisible; line++)
  {
    uint8_t ypos = y0 + line * 10;
    uint8_t item = contextMenuTopItem + line;
    strlcpy_P(txtBuff, _contextMenuItems[item-1], sizeof(txtBuff));
    if(item == contextMenuFocusedItem)
    {
      display.fillRoundRect(13, ypos - 2, _contextMenuItemCount <= maxVisible ? 101 : 97, 11, Sys.useRoundRect ? 4 : 0, BLACK);
      display.setTextColor(WHITE);
    }
    display.setCursor(17, ypos);
    display.print(txtBuff);
    display.setTextColor(BLACK);
  }
  
  //scroll bar
  uint8_t  y = Sys.useRoundRect ? y0 - 1 : y0 - 2;
  uint16_t h = Sys.useRoundRect ? numVisible * 10 - 1 : numVisible * 10 + 1;
  drawScrollBar(112, y, _contextMenuItemCount, contextMenuTopItem, numVisible, h);
  
  //get the id of the item selected
  contextMenuSelectedItemID = 0xff;
  if(clickedButton == KEY_SELECT)
    contextMenuSelectedItemID = _contextMenuItemIDs[contextMenuFocusedItem - 1];
}

#endif //UI_128X64

