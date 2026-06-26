// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.2 (64-bit)
// Tool Version Limit: 2024.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xmy_prj_accelerator.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XMy_prj_accelerator_CfgInitialize(XMy_prj_accelerator *InstancePtr, XMy_prj_accelerator_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XMy_prj_accelerator_Start(XMy_prj_accelerator *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMy_prj_accelerator_ReadReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_AP_CTRL) & 0x80;
    XMy_prj_accelerator_WriteReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XMy_prj_accelerator_IsDone(XMy_prj_accelerator *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMy_prj_accelerator_ReadReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XMy_prj_accelerator_IsIdle(XMy_prj_accelerator *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMy_prj_accelerator_ReadReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XMy_prj_accelerator_IsReady(XMy_prj_accelerator *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMy_prj_accelerator_ReadReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XMy_prj_accelerator_EnableAutoRestart(XMy_prj_accelerator *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMy_prj_accelerator_WriteReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XMy_prj_accelerator_DisableAutoRestart(XMy_prj_accelerator *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMy_prj_accelerator_WriteReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_AP_CTRL, 0);
}

void XMy_prj_accelerator_Set_N(XMy_prj_accelerator *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMy_prj_accelerator_WriteReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_N_DATA, Data);
}

u32 XMy_prj_accelerator_Get_N(XMy_prj_accelerator *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMy_prj_accelerator_ReadReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_N_DATA);
    return Data;
}

void XMy_prj_accelerator_InterruptGlobalEnable(XMy_prj_accelerator *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMy_prj_accelerator_WriteReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_GIE, 1);
}

void XMy_prj_accelerator_InterruptGlobalDisable(XMy_prj_accelerator *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMy_prj_accelerator_WriteReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_GIE, 0);
}

void XMy_prj_accelerator_InterruptEnable(XMy_prj_accelerator *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XMy_prj_accelerator_ReadReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_IER);
    XMy_prj_accelerator_WriteReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_IER, Register | Mask);
}

void XMy_prj_accelerator_InterruptDisable(XMy_prj_accelerator *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XMy_prj_accelerator_ReadReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_IER);
    XMy_prj_accelerator_WriteReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_IER, Register & (~Mask));
}

void XMy_prj_accelerator_InterruptClear(XMy_prj_accelerator *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMy_prj_accelerator_WriteReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_ISR, Mask);
}

u32 XMy_prj_accelerator_InterruptGetEnabled(XMy_prj_accelerator *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMy_prj_accelerator_ReadReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_IER);
}

u32 XMy_prj_accelerator_InterruptGetStatus(XMy_prj_accelerator *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMy_prj_accelerator_ReadReg(InstancePtr->Control_BaseAddress, XMY_PRJ_ACCELERATOR_CONTROL_ADDR_ISR);
}

