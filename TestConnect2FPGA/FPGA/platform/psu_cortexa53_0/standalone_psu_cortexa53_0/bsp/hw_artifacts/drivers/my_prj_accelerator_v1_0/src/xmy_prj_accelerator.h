// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.2 (64-bit)
// Tool Version Limit: 2024.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XMY_PRJ_ACCELERATOR_H
#define XMY_PRJ_ACCELERATOR_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xmy_prj_accelerator_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
#ifdef SDT
    char *Name;
#else
    u16 DeviceId;
#endif
    u64 Control_BaseAddress;
} XMy_prj_accelerator_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XMy_prj_accelerator;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XMy_prj_accelerator_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XMy_prj_accelerator_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XMy_prj_accelerator_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XMy_prj_accelerator_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
#ifdef SDT
int XMy_prj_accelerator_Initialize(XMy_prj_accelerator *InstancePtr, UINTPTR BaseAddress);
XMy_prj_accelerator_Config* XMy_prj_accelerator_LookupConfig(UINTPTR BaseAddress);
#else
int XMy_prj_accelerator_Initialize(XMy_prj_accelerator *InstancePtr, u16 DeviceId);
XMy_prj_accelerator_Config* XMy_prj_accelerator_LookupConfig(u16 DeviceId);
#endif
int XMy_prj_accelerator_CfgInitialize(XMy_prj_accelerator *InstancePtr, XMy_prj_accelerator_Config *ConfigPtr);
#else
int XMy_prj_accelerator_Initialize(XMy_prj_accelerator *InstancePtr, const char* InstanceName);
int XMy_prj_accelerator_Release(XMy_prj_accelerator *InstancePtr);
#endif

void XMy_prj_accelerator_Start(XMy_prj_accelerator *InstancePtr);
u32 XMy_prj_accelerator_IsDone(XMy_prj_accelerator *InstancePtr);
u32 XMy_prj_accelerator_IsIdle(XMy_prj_accelerator *InstancePtr);
u32 XMy_prj_accelerator_IsReady(XMy_prj_accelerator *InstancePtr);
void XMy_prj_accelerator_EnableAutoRestart(XMy_prj_accelerator *InstancePtr);
void XMy_prj_accelerator_DisableAutoRestart(XMy_prj_accelerator *InstancePtr);

void XMy_prj_accelerator_Set_N(XMy_prj_accelerator *InstancePtr, u32 Data);
u32 XMy_prj_accelerator_Get_N(XMy_prj_accelerator *InstancePtr);
void XMy_prj_accelerator_Set_x(XMy_prj_accelerator *InstancePtr, u64 Data);
u64 XMy_prj_accelerator_Get_x(XMy_prj_accelerator *InstancePtr);
void XMy_prj_accelerator_Set_score(XMy_prj_accelerator *InstancePtr, u64 Data);
u64 XMy_prj_accelerator_Get_score(XMy_prj_accelerator *InstancePtr);

void XMy_prj_accelerator_InterruptGlobalEnable(XMy_prj_accelerator *InstancePtr);
void XMy_prj_accelerator_InterruptGlobalDisable(XMy_prj_accelerator *InstancePtr);
void XMy_prj_accelerator_InterruptEnable(XMy_prj_accelerator *InstancePtr, u32 Mask);
void XMy_prj_accelerator_InterruptDisable(XMy_prj_accelerator *InstancePtr, u32 Mask);
void XMy_prj_accelerator_InterruptClear(XMy_prj_accelerator *InstancePtr, u32 Mask);
u32 XMy_prj_accelerator_InterruptGetEnabled(XMy_prj_accelerator *InstancePtr);
u32 XMy_prj_accelerator_InterruptGetStatus(XMy_prj_accelerator *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
