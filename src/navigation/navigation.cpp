#include "navigation.h"
#include <GEM_u8g2.h>
#include <cstdint>
#include "../filesystem.h"
#include "../input.h"

U8G2_ST7565_ERC12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18, /* mosi=*/ 4, /* cs=*/ 5, /* dc=*/ 2, /* reset=*/ 15);
GEM_u8g2 menu(u8g2, GEM_POINTER_ROW, GEM_ITEMS_COUNT_AUTO);
Model selectedModel;
bool modelExists = false;

void fetchSettings(){
  // fetches system settings from filesystem
  if(!systemSettings.lastSelectedModelName.empty()){
    // checks if the last selected model actually exists and saves its data
    std::pair<bool, Model> modelData = getModel(systemSettings.lastSelectedModelName);
    modelExists = modelData.first;
    selectedModel = modelData.second;
  }
}

void saveSystemChanges(){ updateSystemSettings(systemSettings); }

/** DOES NOT WORK FOR UPDATING NAME */
void saveModelChanges(){
  updateModel(selectedModel.name, selectedModel);
}
/** DOES NOT WORK FOR UPDATING NAME */
void saveModelChanges(std::function<void()> callback){
  updateModel(selectedModel.name, selectedModel);
  callback();
}

// ================ Main ===========================
static GEMPage PAGEMain("Main Menu");
static GEMItem LINKSystemSettings("System Settings", systemSettings);
static GEMItem LINKFunctionsSetup("Functions Setup", systemSettings);

// ================ System Settings ================
static GEMPage PAGESystemSettings("System Settings", PAGEMain);

static GEMItem LINKSystemModelNew("New model", systemSettings);
static char INPUTSystemModelNewNameVal[GEM_STR_LEN] = "";
static GEMItem INPUTSystemModelNewName("Name:", INPUTSystemModelNewNameVal);
static char INPUTSystemModelNewKeyVal[GEM_STR_LEN] = "";
static GEMItem INPUTSystemModelNewKey("Secret key:", INPUTSystemModelNewKeyVal);
static GEMItem BUTTONSystemModelNew("Create", saveSystemChanges);

// static GEMItem LINKSystemModelSelect("Select model", systemSettings); TODO

static GEMItem LINKSystemModelUpdate("Update model", systemSettings);
static char INPUTSystemModelUpdateNameVal[GEM_STR_LEN] = "";
static GEMItem INPUTSystemModelUpdateName("Name:", INPUTSystemModelUpdateNameVal);
static char INPUTSystemModelUpdateKeyVal[GEM_STR_LEN] = "";
static GEMItem INPUTSystemModelUpdateKey("Secret key:", INPUTSystemModelUpdateKeyVal);
static GEMItem BUTTONSystemModelUpdate("Update", saveSystemChanges);

static GEMItem LINKSystemModelCopy("Copy model", systemSettings);
static char INPUTSystemModelCopyNameVal[GEM_STR_LEN] = "";
static GEMItem INPUTSystemModelCopyName("Copy name:", INPUTSystemModelCopyNameVal);
static GEMItem BUTTONSystemModelCopy("Create a copy", saveSystemChanges);

static GEMItem LINKSystemModelRemove("Remove model", systemSettings);
static GEMItem TEXTSystemModelRemove("Are you sure you want to remove this model?");
static GEMItem BUTTONSystemModelRemove("Yes, Remove", saveSystemChanges);

static GEMItem LINKSystemAuxSwitches("Aux Switches On/Off", systemSettings);
static GEMItem INPUTSystemAuxSwitchesA("Switch A:", systemSettings.switches2On[static_cast<uint8_t>(Switch2::A)], saveSystemChanges);
static GEMItem INPUTSystemAuxSwitchesB("Switch B:", systemSettings.switches2On[static_cast<uint8_t>(Switch2::B)], saveSystemChanges);
static GEMItem INPUTSystemAuxSwitchesC("Switch C:", systemSettings.switches2On[static_cast<uint8_t>(Switch2::C)], saveSystemChanges);
static GEMItem INPUTSystemAuxSwitchesD("Switch D:", systemSettings.switches2On[static_cast<uint8_t>(Switch2::D)], saveSystemChanges);
static GEMItem INPUTSystemAuxSwitchesE("Switch E:", systemSettings.switches3On[static_cast<uint8_t>(Switch3::E)], saveSystemChanges);
static GEMItem INPUTSystemAuxSwitchesF("Switch F:", systemSettings.switches3On[static_cast<uint8_t>(Switch3::F)], saveSystemChanges);
static GEMItem INPUTSystemAuxSwitchesG("Switch G:", systemSettings.switches3On[static_cast<uint8_t>(Switch3::G)], saveSystemChanges);
static GEMItem INPUTSystemAuxSwitchesH("Switch H:", systemSettings.switches3On[static_cast<uint8_t>(Switch3::H)], saveSystemChanges);

static GEMItem INPUTSystemSoundEffects("Sound:", systemSettings.soundEffects, saveSystemChanges);

static GEMItem LINKSystemFactoryReset("Factory reset", systemSettings);
static GEMItem TEXTSystemFactoryReset("Are you sure you want to perform a factory reset?");
static GEMItem BUTTONSystemFactoryReset("Yes, Erase All Data", saveSystemChanges);

// ================ Functions Setup ================ TODO Display & Dual expo
static GEMPage PAGEFunctionsSetup("Functions Setup", PAGEMain);

static GEMItem LINKFunctionsReverse("Reverse channels", systemSettings);
static GEMItem INPUTFunctionsReverse1("Vr Channel 1:", selectedModel.reversedPotentiometers[static_cast<uint8_t>(Potentiometer::secondVertical)], saveModelChanges);
static GEMItem INPUTFunctionsReverse2("Vr Channel 2:", selectedModel.reversedPotentiometers[static_cast<uint8_t>(Potentiometer::secondHorizontal)], saveModelChanges);
static GEMItem INPUTFunctionsReverse3("Vr Channel 3:", selectedModel.reversedPotentiometers[static_cast<uint8_t>(Potentiometer::firstVertical)], saveModelChanges);
static GEMItem INPUTFunctionsReverse4("Vr Channel 4:", selectedModel.reversedPotentiometers[static_cast<uint8_t>(Potentiometer::firstHorizontal)], saveModelChanges);
static GEMItem INPUTFunctionsReverse5("Vr Channel 5:", selectedModel.reversedPotentiometers[static_cast<uint8_t>(Potentiometer::variable1)], saveModelChanges);
static GEMItem INPUTFunctionsReverse6("Vr Channel 6:", selectedModel.reversedPotentiometers[static_cast<uint8_t>(Potentiometer::variable2)], saveModelChanges);
static GEMItem INPUTFunctionsReverseA("Switch A:", selectedModel.reversedSwitches2[static_cast<uint8_t>(Switch2::A)], saveModelChanges);
static GEMItem INPUTFunctionsReverseB("Switch B:", selectedModel.reversedSwitches2[static_cast<uint8_t>(Switch2::B)], saveModelChanges);
static GEMItem INPUTFunctionsReverseC("Switch C:", selectedModel.reversedSwitches2[static_cast<uint8_t>(Switch2::C)], saveModelChanges);
static GEMItem INPUTFunctionsReverseD("Switch D:", selectedModel.reversedSwitches2[static_cast<uint8_t>(Switch2::D)], saveModelChanges);
static GEMItem INPUTFunctionsReverseE("Switch E:", selectedModel.reversedSwitches3[static_cast<uint8_t>(Switch3::E)], saveModelChanges);
static GEMItem INPUTFunctionsReverseF("Switch F:", selectedModel.reversedSwitches3[static_cast<uint8_t>(Switch3::F)], saveModelChanges);
static GEMItem INPUTFunctionsReverseG("Switch G:", selectedModel.reversedSwitches3[static_cast<uint8_t>(Switch3::G)], saveModelChanges);
static GEMItem INPUTFunctionsReverseH("Switch H:", selectedModel.reversedSwitches3[static_cast<uint8_t>(Switch3::H)], saveModelChanges);

static GEMItem LINKFunctionsSubtrim("Subtrim", systemSettings);
static GEMItem INPUTFunctionsReverse1("Vr Channel 1:", selectedModel.subtrim[static_cast<uint8_t>(Potentiometer::secondVertical)], saveModelChanges);
static GEMItem INPUTFunctionsReverse2("Vr Channel 2:", selectedModel.subtrim[static_cast<uint8_t>(Potentiometer::secondHorizontal)], saveModelChanges);
static GEMItem INPUTFunctionsReverse3("Vr Channel 3:", selectedModel.subtrim[static_cast<uint8_t>(Potentiometer::firstVertical)], saveModelChanges);
static GEMItem INPUTFunctionsReverse4("Vr Channel 4:", selectedModel.subtrim[static_cast<uint8_t>(Potentiometer::firstHorizontal)], saveModelChanges);
static GEMItem INPUTFunctionsReverse5("Vr Channel 5:", selectedModel.subtrim[static_cast<uint8_t>(Potentiometer::variable1)], saveModelChanges);
static GEMItem INPUTFunctionsReverse6("Vr Channel 6:", selectedModel.subtrim[static_cast<uint8_t>(Potentiometer::variable2)], saveModelChanges);

static GEMItem LINKFunctionsEndPoints("End points", systemSettings);

static GEMItem LINKFunctionsAssignSwitches("Assign Switches", systemSettings);
static GEMItem INPUTFunctionsReverse5("Flight mode:", selectedModel.subtrim[static_cast<uint8_t>(Potentiometer::variable1)], saveModelChanges);
static GEMItem INPUTFunctionsReverse6("Throttle hold:", selectedModel.subtrim[static_cast<uint8_t>(Potentiometer::variable2)], saveModelChanges);

// static char name[5] = "Alex";
// static GEMSelect myNameSelect(3, (SelectOptionChar[]){{"Alexander", "Alex"}, {"Michael", "Mike"}, {"Peter", "Pete"}});
// static GEMItem menuItemName("Name:", name, myNameSelect);

void setupMenuPages(){
  // test page
  PAGEMain.addMenuItem(INPUTSystemSoundEffects);
  // PAGEMain.addMenuItem(menuItemName);
  menu.setMenuPageCurrent(PAGEMain);
}

void initMenu(){
  fetchSettings();

  u8g2.begin();
  u8g2.setContrast(0);

  menu
    .setSplash(SPLASH_WIDTH, SPLASH_HEIGHT, splashBits)
    .hideVersion()
    .init();

  setupMenuPages();

  menu.drawMenu();
}
// TODO reset all inputs when leaving pages in system settings