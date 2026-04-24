/*
 * EqualCheck.h
 *
 *  Created on: Mar 3, 2025
 *      Author: MSLM Electric
 */

#ifndef EXTERNALLIBS_EQUALCHECK_H_
#define EXTERNALLIBS_EQUALCHECK_H_

#include "type_def.h"

typedef enum{
    BAD_RESULT = 0,
    BIGGER = 2,
    SMALLER = 3,
    EQUAL = 4,
    IN_RANGE = 4
}ComparerFlags_t;

ComparerFlags_t ValuesComparer(u32 Value, u32 comparerValue, u32 delta);
ComparerFlags_t ValuesComparer3(s32 Value, s32 comparerValue, s32 delta);
ComparerFlags_t ValuesComparer2(u32 Value, u32 lowerValueToCompare, u32 higherValueToCompare);
ComparerFlags_t IsValuesEqual(u32 Value, u32 comparerValue, u32 delta);
ComparerFlags_t IsValuesEqual2(u32 Value, u32 lowerValueToCompare, u32 higherValueToCompare);

ComparerFlags_t FloatValuesComparer3(float Value, float comparerValue, float delta);
ComparerFlags_t FloatValuesComparer2(/*signed!*/float Value, /*signed!*/float lowerValueToCompare, /*signed!*/float higherValueToCompare);
ComparerFlags_t IsFloatValuesEqual(float Value, float comparerValue, float delta);
ComparerFlags_t IsFloatValuesEqual2(float Value, float lowerValueToCompare, float higherValueToCompare);

#define IsValuesInRange IsValuesEqual2 //IsValuesLiesInRange
#define IsFloatValuesInRange IsFloatValuesEqual2

#endif /* EXTERNALLIBS_EQUALCHECK_H_ */
