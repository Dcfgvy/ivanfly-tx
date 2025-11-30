#include <filesystem.h>
#include <LittleFS.h>
#include <vector>
#include <string>
#include <ArduinoJson.h>

File openModelsDir(){
  File modelsDir = LittleFS.open(MODELS_DIR_PATH);
  if(!modelsDir) Serial.println(" - failed to open the models directory");
  if(!modelsDir.isDirectory()) Serial.println("- the models dir not a directory");
  return modelsDir;
}

const char* getModelPath(std::string name){
  return (MODELS_DIR_PATH + (std::string)"/" + name + ".json").c_str();
}

JsonDocument modelToJson(const Model &model){
  JsonDocument doc;

  doc["secretKey"] = model.secretKey.c_str();

  // reversed arrays
  JsonArray rp = doc["reversedPotentiometers"].to<JsonArray>();
  for(int i = 0; i < 6; i++) rp.add(model.reversedPotentiometers[i]);

  JsonArray rs2 = doc["reversedSwitches2"].to<JsonArray>();
  for(int i = 0; i < 4; i++) rs2.add(model.reversedSwitches2[i]);

  JsonArray rs3 = doc["reversedSwitches3"].to<JsonArray>();
  for(int i = 0; i < 4; i++) rs3.add(model.reversedSwitches3[i]);

  doc["flightModeSwitch2"] = model.flightModeSwitch2;
  doc["throttleHoldSwitch2"] = model.throttleHoldSwitch2;

  JsonArray fm1 = doc["flightMode1"].to<JsonArray>();
  for(int i = 0; i < 6; i++){
    JsonObject obj = fm1.add<JsonObject>();
    obj["exponent"] = model.flightMode1[i].exponent;
    obj["maxValue"] = model.flightMode1[i].maxValue;
  }

  JsonArray fm2 = doc["flightMode2"].to<JsonArray>();
  for(int i = 0; i < 6; i++){
    JsonObject obj = fm2.add<JsonObject>();
    obj["exponent"] = model.flightMode2[i].exponent;
    obj["maxValue"] = model.flightMode2[i].maxValue;
  }

  JsonArray st = doc["subtrim"].to<JsonArray>();
  for(int i = 0; i < 6; i++) st.add(model.subtrim[i]);

  return doc;
}

Model jsonToModel(const JsonDocument &doc, std::string name){
  Model model{};

  model.name = name;
  model.secretKey = std::string(doc["secretKey"].as<const char*>());

  // reversed arrays
  JsonArrayConst rp = doc["reversedPotentiometers"].as<JsonArrayConst>();
  for(size_t i = 0; i<rp.size() && i<6; i++) model.reversedPotentiometers[i] = rp[i].as<bool>();

  JsonArrayConst rs2 = doc["reversedSwitches2"].as<JsonArrayConst>();
  for(size_t i = 0; i<rs2.size() && i<4; i++) model.reversedSwitches2[i] = rs2[i].as<bool>();

  JsonArrayConst rs3 = doc["reversedSwitches3"].as<JsonArrayConst>();
  for(size_t i = 0; i<rs3.size() && i<4; i++) model.reversedSwitches3[i] = rs3[i].as<bool>();

  model.flightModeSwitch2 = doc["flightModeSwitch2"].as<int>();
  model.throttleHoldSwitch2 = doc["throttleHoldSwitch2"].as<int>();

  JsonArrayConst fm1 = doc["flightMode1"].as<JsonArrayConst>();
  for(size_t i = 0; i<fm1.size() && i<6; i++){
    JsonObjectConst obj = fm1[i].as<JsonObjectConst>();
    model.flightMode1[i].exponent = obj["exponent"].as<int8_t>();
    model.flightMode1[i].maxValue = obj["maxValue"].as<uint8_t>();
  }

  JsonArrayConst fm2 = doc["flightMode2"].as<JsonArrayConst>();
  for(size_t i = 0; i<fm2.size() && i<6; i++){
    JsonObjectConst obj = fm2[i].as<JsonObjectConst>();
    model.flightMode2[i].exponent = obj["exponent"].as<int8_t>();
    model.flightMode2[i].maxValue = obj["maxValue"].as<uint8_t>();
  }

  JsonArrayConst st = doc["subtrim"].as<JsonArrayConst>();
  for(size_t i = 0; i<st.size() && i<6; i++) model.subtrim[i] = st[i].as<int8_t>();

  return model;
}

modelsList_t getModels(){
  File modelsDir = openModelsDir();

  modelsList_t modelsList;
  File file = modelsDir.openNextFile();
  while(file){
    std::string filename = file.name();
    std::size_t filesize = filename.size();
    if(!file.isDirectory() && filesize > 5){ // model should have at least one symbol in its name (e.g. a.json)
      modelsList.push_back(filename.substr(0, filesize - 5));
    }
    file.close();
    file = modelsDir.openNextFile();
  }

  modelsDir.close();
  return modelsList;
}

bool createModel(std::string name){
  const char* path = getModelPath(name);
  
  // false if no place left or the model already exists
  if(LittleFS.exists(path)) return false;
  if(getModels().size() >= MAX_MODELS_COUNT) return false;

  // assign default values
  const Model defaultModelSettings = {
    .name = name, .secretKey = "                ",
    .flightModeSwitch2 = -1, .throttleHoldSwitch2 = -1
  };
  JsonDocument modelDoc = modelToJson(defaultModelSettings);
  // create and save the file
  File modelFile = LittleFS.open(path, "w");
  serializeJson(modelDoc, modelFile);
  modelFile.close();

  return true;
}

std::pair<bool, Model> getModel(std::string name){
  const char* path = getModelPath(name);
  
  // false if the model does not exist
  if(!LittleFS.exists(path)) return {false, Model{}};

  JsonDocument modelDoc;
  // read the settings from json
  File modelFile = LittleFS.open(path, "r");
  deserializeJson(modelDoc, modelFile);
  modelFile.close();

  return {true, jsonToModel(modelDoc, name)};
}

bool renameModel(std::string oldName, std::string newName){
  const char* oldPath = getModelPath(oldName);
  const char* newPath = getModelPath(newName);

  if(LittleFS.exists(newPath)) return false;
  
  return LittleFS.rename(oldPath, newPath);
}

bool updateModel(std::string name, Model &newModel){
  const char* path = getModelPath(name);
  
  // false if no model exists
  if(!LittleFS.exists(path)) return false;

  // assign default values
  JsonDocument modelDoc = modelToJson(newModel);
  // create and save the file
  File modelFile = LittleFS.open(path, "w");
  serializeJson(modelDoc, modelFile);
  modelFile.close();

  return true;
}

bool removeModel(std::string name){
  return LittleFS.remove(getModelPath(name));
}
