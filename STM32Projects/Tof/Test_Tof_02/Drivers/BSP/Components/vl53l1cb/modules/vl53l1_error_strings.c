
// SPDX-License-Identifier: GPL-2.0+ OR BSD-3-Clause
/******************************************************************************
 * Copyright (c) 2020, STMicroelectronics - All Rights Reserved

 This file is part of VL53L1 and is dual licensed,
 either GPL-2.0+
 or 'BSD 3-clause "New" or "Revised" License' , at your option.
 ******************************************************************************
 */




#include "vl53l1_error_codes.h"
#include "vl53l1_error_strings.h"
#include "vl53l1_platform_log.h"
#include "vl53l1_ll_def.h"

#define LOG_FUNCTION_START(fmt, ...) \
	_LOG_FUNCTION_START(VL53L1_TRACE_MODULE_API, fmt, ##__VA_ARGS__)
#define LOG_FUNCTION_END(status, ...) \
	_LOG_FUNCTION_END(VL53L1_TRACE_MODULE_API, status, ##__VA_ARGS__)
#define LOG_FUNCTION_END_FMT(status, fmt, ...) \
	_LOG_FUNCTION_END_FMT(VL53L1_TRACE_MODULE_API, \
		status, fmt, ##__VA_ARGS__)


VL53L1_Error VL53L1_get_pal_error_string(
	VL53L1_Error   PalErrorCode,
	char          *pPalErrorString)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;

#ifdef VL53L1_USE_EMPTY_STRING
	SUPPRESS_UNUSED_WARNING(PalErrorCode);
#endif

	LOG_FUNCTION_START("");

#ifdef VL53L1_USE_EMPTY_STRING
	VL53L1_COPYSTRING(pPalErrorString, "");
#else

	switch (PalErrorCode) {
	case VL53L1_ERROR_NONE:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_NONE);
		break;
	case VL53L1_ERROR_CALIBRATION_WARNING:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_CALIBRATION_WARNING);
		break;
	case VL53L1_ERROR_MIN_CLIPPED:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_MIN_CLIPPED);
		break;
	case VL53L1_ERROR_UNDEFINED:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_UNDEFINED);
		break;
	case VL53L1_ERROR_INVALID_PARAMS:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_INVALID_PARAMS);
		break;
	case VL53L1_ERROR_NOT_SUPPORTED:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_NOT_SUPPORTED);
		break;
	case VL53L1_ERROR_RANGE_ERROR:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_RANGE_ERROR);
		break;
	case VL53L1_ERROR_TIME_OUT:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_TIME_OUT);
		break;
	case VL53L1_ERROR_MODE_NOT_SUPPORTED:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_MODE_NOT_SUPPORTED);
		break;
	case VL53L1_ERROR_BUFFER_TOO_SMALL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_BUFFER_TOO_SMALL);
		break;
	case VL53L1_ERROR_COMMS_BUFFER_TOO_SMALL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_COMMS_BUFFER_TOO_SMALL);
		break;
	case VL53L1_ERROR_GPIO_NOT_EXISTING:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_GPIO_NOT_EXISTING);
		break;
	case VL53L1_ERROR_GPIO_FUNCTIONALITY_NOT_SUPPORTED:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_GPIO_FUNCTIONALITY_NOT_SUPPORTED);
		break;
	case VL53L1_ERROR_CONTROL_INTERFACE:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_CONTROL_INTERFACE);
		break;
	case VL53L1_ERROR_INVALID_COMMAND:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_INVALID_COMMAND);
		break;
	case VL53L1_ERROR_DIVISION_BY_ZERO:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_DIVISION_BY_ZERO);
		break;
	case VL53L1_ERROR_REF_SPAD_INIT:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_REF_SPAD_INIT);
		break;
	case VL53L1_ERROR_GPH_SYNC_CHECK_FAIL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_GPH_SYNC_CHECK_FAIL);
		break;
	case VL53L1_ERROR_STREAM_COUNT_CHECK_FAIL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_STREAM_COUNT_CHECK_FAIL);
		break;
	case VL53L1_ERROR_GPH_ID_CHECK_FAIL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_GPH_ID_CHECK_FAIL);
		break;
	case VL53L1_ERROR_ZONE_STREAM_COUNT_CHECK_FAIL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_ZONE_STREAM_COUNT_CHECK_FAIL);
		break;
	case VL53L1_ERROR_ZONE_GPH_ID_CHECK_FAIL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_ZONE_GPH_ID_CHECK_FAIL);
		break;

	case VL53L1_ERROR_XTALK_EXTRACTION_NO_SAMPLE_FAIL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_XTALK_EXTRACTION_NO_SAMPLES_FAIL);
		break;
	case VL53L1_ERROR_XTALK_EXTRACTION_SIGMA_LIMIT_FAIL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_XTALK_EXTRACTION_SIGMA_LIMIT_FAIL);
		break;

	case VL53L1_ERROR_OFFSET_CAL_NO_SAMPLE_FAIL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_OFFSET_CAL_NO_SAMPLE_FAIL);
		break;
	case VL53L1_ERROR_OFFSET_CAL_NO_SPADS_ENABLED_FAIL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_OFFSET_CAL_NO_SPADS_ENABLED_FAIL);
		break;
	case VL53L1_ERROR_ZONE_CAL_NO_SAMPLE_FAIL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_ZONE_CAL_NO_SAMPLE_FAIL);
		break;

	case VL53L1_WARNING_OFFSET_CAL_MISSING_SAMPLES:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_WARNING_OFFSET_CAL_MISSING_SAMPLES);
		break;
	case VL53L1_WARNING_OFFSET_CAL_SIGMA_TOO_HIGH:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_WARNING_OFFSET_CAL_SIGMA_TOO_HIGH);
		break;
	case VL53L1_WARNING_OFFSET_CAL_RATE_TOO_HIGH:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_WARNING_OFFSET_CAL_RATE_TOO_HIGH);
		break;
	case VL53L1_WARNING_OFFSET_CAL_SPAD_COUNT_TOO_LOW:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_WARNING_OFFSET_CAL_SPAD_COUNT_TOO_LOW);
		break;

	case VL53L1_WARNING_ZONE_CAL_MISSING_SAMPLES:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_WARNING_ZONE_CAL_MISSING_SAMPLES);
		break;
	case VL53L1_WARNING_ZONE_CAL_SIGMA_TOO_HIGH:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_WARNING_ZONE_CAL_SIGMA_TOO_HIGH);
		break;
	case VL53L1_WARNING_ZONE_CAL_RATE_TOO_HIGH:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_WARNING_ZONE_CAL_RATE_TOO_HIGH);
		break;

	case VL53L1_WARNING_REF_SPAD_CHAR_NOT_ENOUGH_SPADS:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_WARNING_REF_SPAD_CHAR_NOT_ENOUGH_SPADS);
		break;
	case VL53L1_WARNING_REF_SPAD_CHAR_RATE_TOO_HIGH:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_WARNING_REF_SPAD_CHAR_RATE_TOO_HIGH);
		break;
	case VL53L1_WARNING_REF_SPAD_CHAR_RATE_TOO_LOW:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_WARNING_REF_SPAD_CHAR_RATE_TOO_LOW);
		break;

	case VL53L1_WARNING_XTALK_MISSING_SAMPLES:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_WARNING_XTALK_MISSING_SAMPLES);
		break;
	case VL53L1_WARNING_XTALK_NO_SAMPLES_FOR_GRADIENT:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_WARNING_XTALK_NO_SAMPLES_FOR_GRADIENT);
		break;
	case VL53L1_WARNING_XTALK_SIGMA_LIMIT_FOR_GRADIENT:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_WARNING_XTALK_SIGMA_LIMIT_FOR_GRADIENT);
		break;

	case VL53L1_ERROR_DEVICE_FIRMWARE_TOO_OLD:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_DEVICE_FIRMWARE_TOO_OLD);
		break;
	case VL53L1_ERROR_DEVICE_FIRMWARE_TOO_NEW:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_DEVICE_FIRMWARE_TOO_NEW);
		break;
	case VL53L1_ERROR_UNIT_TEST_FAIL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_UNIT_TEST_FAIL);
		break;
	case VL53L1_ERROR_FILE_READ_FAIL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_FILE_READ_FAIL);
		break;
	case VL53L1_ERROR_FILE_WRITE_FAIL:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_FILE_WRITE_FAIL);
		break;
	case VL53L1_ERROR_NOT_IMPLEMENTED:
		VL53L1_COPYSTRING(pPalErrorString,
			VL53L1_STRING_ERROR_NOT_IMPLEMENTED);
		break;
	default:
		VL53L1_COPYSTRING(pPalErrorString,
				VL53L1_STRING_UNKNOW_ERROR_CODE);
	}

#endif

	LOG_FUNCTION_END(Status);

	return Status;
}

