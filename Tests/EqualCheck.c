/*
 * EqualCheck.c
 *
 *  Created on: Mar 3, 2025
 *      Author: Osim A
 */

#include "EqualCheck.h"

/**
 * @brief ValuesComparer
 * @param Value
 * @param comparerValue
 * @param delta
 * @return
 * @ingroup
 */
ComparerFlags_t ValuesComparer(u32 Value, u32 comparerValue, u32 delta)
{
    if(Value <= (comparerValue+2*delta)){
        if(Value >= (comparerValue-2*delta)){
            return EQUAL;
        }else if(Value < (comparerValue-2*delta)){
            return SMALLER;
        }
    }else{ //..if(Value > (comparerValue+2*delta))
        return BIGGER;
    }
    return BAD_RESULT; //never should get here!
}

ComparerFlags_t ValuesComparer3(s32 Value, s32 comparerValue, s32 delta)
{
    if(Value <= (comparerValue+delta)){
        if(Value >= (comparerValue-delta)){
            return EQUAL;
        }else if(Value < (comparerValue-delta)){
            return SMALLER;
        }
    }else{
        return BIGGER;
    }
    return BAD_RESULT; //never should get here!
}

ComparerFlags_t ValuesComparer2(u32 Value, u32 lowerValueToCompare, u32 higherValueToCompare)
{
    if (Value <= (higherValueToCompare)) {
        if (Value >= (lowerValueToCompare)) {
            return EQUAL;
        }
        else if (Value < (lowerValueToCompare)) {
            return SMALLER;
        }
    }
    else {
        return BIGGER;
    }
    return BAD_RESULT; //never should get here!
}

ComparerFlags_t IsValuesEqual(u32 Value, u32 comparerValue, u32 delta)
{
    if (ValuesComparer3(Value, comparerValue, delta) == EQUAL)
        return EQUAL;
    return 0;
}

ComparerFlags_t IsValuesEqual2(u32 Value, u32 lowerValueToCompare, u32 higherValueToCompare)
{
	if(ValuesComparer2(Value, lowerValueToCompare, higherValueToCompare) == EQUAL)
		return EQUAL;
	return 0;
}
