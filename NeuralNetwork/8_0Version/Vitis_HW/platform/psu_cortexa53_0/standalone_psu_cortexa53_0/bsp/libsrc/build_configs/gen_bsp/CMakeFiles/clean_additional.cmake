# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "/home/denisaa/Desktop/Licenta_Denisa_2026/ProiectLicenta/NeuralNetwork/MLP_100_cut/Vitis_HW/platform/psu_cortexa53_0/standalone_psu_cortexa53_0/bsp/include/sleep.h"
  "/home/denisaa/Desktop/Licenta_Denisa_2026/ProiectLicenta/NeuralNetwork/MLP_100_cut/Vitis_HW/platform/psu_cortexa53_0/standalone_psu_cortexa53_0/bsp/include/xiltimer.h"
  "/home/denisaa/Desktop/Licenta_Denisa_2026/ProiectLicenta/NeuralNetwork/MLP_100_cut/Vitis_HW/platform/psu_cortexa53_0/standalone_psu_cortexa53_0/bsp/include/xtimer_config.h"
  "/home/denisaa/Desktop/Licenta_Denisa_2026/ProiectLicenta/NeuralNetwork/MLP_100_cut/Vitis_HW/platform/psu_cortexa53_0/standalone_psu_cortexa53_0/bsp/lib/libxiltimer.a"
  )
endif()
