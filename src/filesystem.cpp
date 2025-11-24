#include <filesystem.h>
#include <LittleFS.h>
#include <vector>
#include <string>

File openModelsDir(){
  File modelsDir = LittleFS.open(MODELS_DIR_PATH);
  if(!modelsDir) Serial.println(" - failed to open the models directory");
  if(!modelsDir.isDirectory()) Serial.println("- the models dir not a directory");
  return modelsDir;
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