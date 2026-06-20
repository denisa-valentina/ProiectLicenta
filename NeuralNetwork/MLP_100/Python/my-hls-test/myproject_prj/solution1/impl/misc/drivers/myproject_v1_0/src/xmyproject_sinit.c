// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.2 (64-bit)
// Tool Version Limit: 2024.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#ifdef SDT
#include "xparameters.h"
#endif
#include "xmyproject.h"

extern XMyproject_Config XMyproject_ConfigTable[];

#ifdef SDT
XMyproject_Config *XMyproject_LookupConfig(UINTPTR BaseAddress) {
	XMyproject_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XMyproject_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XMyproject_ConfigTable[Index].Control_BaseAddress == BaseAddress) {
			ConfigPtr = &XMyproject_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMyproject_Initialize(XMyproject *InstancePtr, UINTPTR BaseAddress) {
	XMyproject_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMyproject_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMyproject_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XMyproject_Config *XMyproject_LookupConfig(u16 DeviceId) {
	XMyproject_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XMYPROJECT_NUM_INSTANCES; Index++) {
		if (XMyproject_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XMyproject_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMyproject_Initialize(XMyproject *InstancePtr, u16 DeviceId) {
	XMyproject_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMyproject_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMyproject_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

