
/* SPDX-License-Identifier: GPL-2.0+ OR BSD-3-Clause */
/******************************************************************************
 * Copyright (c) 2020, STMicroelectronics - All Rights Reserved

 This file is part of VL53L1 and is dual licensed,
 either GPL-2.0+
 or 'BSD 3-clause "New" or "Revised" License' , at your option.
 ******************************************************************************
 */



#ifndef _VL53L1_PLATFORM_USER_DATA_H_
#define _VL53L1_PLATFORM_USER_DATA_H_

#ifndef __KERNEL__
#include <stdlib.h>
#endif

#include "vl53l1_def.h"
#include "vl53l1cb.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef VL53L1CB_Object_t* VL53L1_DEV;

#define VL53L1DevDataGet(Obj, field) 			(Obj->Dev.Data.field)
#define VL53L1DevDataSet(Obj, field, data) 		((Obj->Dev.Data.field) = (data))
#define PALDevDataGet(Obj, field) 				(Obj->Dev.Data.field)
#define PALDevDataSet(Obj, field, value) 		(Obj->Dev.Data.field)=(value)
#define VL53L1DevStructGetLLDriverHandle(Obj) 	(&Obj->Dev.Data.LLData)
#define VL53L1DevStructGetLLResultsHandle(Obj) 	(&Obj->Dev.Data.llresults)

#ifdef __cplusplus
}
#endif

#endif


