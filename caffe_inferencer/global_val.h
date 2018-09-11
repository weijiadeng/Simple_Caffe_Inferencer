//
// Created by dwj on 8/31/18.
//


/**
* Defines marcos and global const variables.
*/
#pragma once


///The data type that the tensor data uses.
#define TYPENAME float
///Defines the minimal value of the data type.This value is supposed to be minus infinity.
#define TYPENAMEMIN TYPENAME(-3.39E38)
///Defines the custom method conducting exp calculation.
#define EXPOWER exp_custom
///Defines the custom method conducting sqare root calculation
#define SQRT sqrt
///Defines the epsilon value.
const TYPENAME CUSTOMEPS = TYPENAME(1E-5);


