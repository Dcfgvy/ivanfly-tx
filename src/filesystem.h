#pragma once

#include <vector>
#include <string>
#include <cstdint>

#define MODELS_DIR_PATH "/models"

typedef std::vector<std::string> modelsList_t;

// The model name is its filename and serves as a unique identifier when working with models

// y = x + p/100 * (x^3 - x)
struct ExponentAndMax{
  int8_t exponent; /** Value from -100 to 100. if negative, we just reflect the function over y=x (point a;b becomes b;a). */
  uint8_t maxValue;
};

struct Model{
  std::string name;
  std::string secretKey;

  bool reversedPotentiometers[6]; /** true if reversed */
  bool reversedSwitches2[4]; /** true if reversed */
  bool reversedSwitches3[4]; /** true if reversed */

  int8_t flightModeSwitch2; /** -1 if no fly mode switch is assigned. Only 2-state switches can be assigned. */
  int8_t ThrottleHoldSwitch2; /** -1 if no throttle hold switch is assigned. Only 2-state switches can be assigned. */
  ExponentAndMax flightMode1[6];
  ExponentAndMax flightMode2[6];

  int8_t subtrim[6]; /** values -100 to +100, correcting the input of each variable channel */
};

/**
 * getModels - get a list of the names of all stored models
 */
modelsList_t getModels();

/**
 * createModel - creates a new model with default parameters
 * 
 * @name Name of a new model
 * 
 * @return true if model was successfully created, false otherwise
 */
bool createModel(std::string name);

/**
 * renameModel - renames a model
 * 
 * @name Name of a new model
 * 
 * @return true if model was successfully created, false otherwise
 */
bool getModel(std::string name);

/**
 * renameModel - renames a model
 */
void renameModel(std::string oldName, std::string newName);

/**
 * updateModel - updates the settings of a model
 * 
 * @name Name of the model
 * 
 * @newModel All the new settings
 * 
 * @return true if model was successfully updated, false otherwise
 */
bool updateModel(std::string name, Model *newModel);

/**
 * removeModel - removes a model
 */
void removeModel(std::string name);
