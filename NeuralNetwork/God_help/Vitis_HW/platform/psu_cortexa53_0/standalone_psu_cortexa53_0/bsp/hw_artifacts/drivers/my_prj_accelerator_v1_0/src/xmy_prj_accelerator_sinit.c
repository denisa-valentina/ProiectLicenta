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
#include "xmy_prj_accelerator.h"

extern XMy_prj_accelerator_Config XMy_prj_accelerator_ConfigTable[];

#ifdef SDT
XMy_prj_accelerator_Config *XMy_prj_accelerator_LookupConfig(UINTPTR BaseAddress) {
	XMy_prj_accelerator_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XMy_prj_accelerator_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XMy_prj_accelerator_ConfigTable[Index].Control_BaseAddress == BaseAddress) {
			ConfigPtr = &XMy_prj_accelerator_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMy_prj_accelerator_Initialize(XMy_prj_accelerator *InstancePtr, UINTPTR BaseAddress) {
	XMy_prj_accelerator_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMy_prj_accelerator_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMy_prj_accelerator_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XMy_prj_accelerator_Config *XMy_prj_accelerator_LookupConfig(u16 DeviceId) {
	XMy_prj_accelerator_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XMY_PRJ_ACCELERATOR_NUM_INSTANCES; Index++) {
		if (XMy_prj_accelerator_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XMy_prj_accelerator_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMy_prj_accelerator_Initialize(XMy_prj_accelerator *InstancePtr, u16 DeviceId) {
	XMy_prj_accelerator_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMy_prj_accelerator_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMy_prj_accelerator_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

