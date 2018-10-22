//
// Created by dwj on 8/31/18.
//

#include "external/half.h"

#include <limits>
#include "external/half.h"
/**
* Defines marcos and global const variables.
*/
#pragma once





///The data type that the tensor data uses.
typedef double TYPENAME;
///Defines the minimal value of the data type.This value is supposed to be minus infinity.
const TYPENAME TYPENAMEMIN = std::numeric_limits<TYPENAME>::lowest();
///Defines the custom method conducting exp calculation.
#define EXPOWER exp_default<TYPENAME>
///Defines the custom method conducting sqare root calculation
#define SQRT sqrt_default<TYPENAME>
///Defines the epsilon value.
const TYPENAME CUSTOMEPS = TYPENAME(1E-5);


