
cmake_minimum_required(VERSION 3.15)
project(bsp)
        
set(DRIVER_TARGETS bram;uartlite;my_prj_accelerator;scugic;zdma;sysmonpsu;axipmon;canps;coresightps_dcc;resetps;clockps;csudma;ddrcpsu;dppsu;dpdma;emacps;gpiops;iicps;ipipsu;qspipsu;rtcpsu;sdps;ttcps;uartps;usbpsu;wdtps)
set(DRIVER_LOCATIONS /tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/bram_v4_11;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/uartlite_v3_11;/home/denisaa/Desktop/Github/FPGA/platform/zynqmp_fsbl/zynqmp_fsbl_bsp/hw_artifacts/drivers/my_prj_accelerator_v1_0;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/scugic_v5_4;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/zdma_v1_19;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/sysmonpsu_v3_0;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/axipmon_v6_11;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/canps_v3_9;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/coresightps_dcc_v1_11;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/resetps_v1_7;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/clockps_v1_6;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/csudma_v1_16;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/ddrcpsu_v1_7;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/dppsu_v1_7;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/dpdma_v1_6;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/emacps_v3_21;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/gpiops_v3_13;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/iicps_v3_20;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/ipipsu_v2_16;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/qspipsu_v1_20;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/rtcpsu_v1_14;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/sdps_v4_4;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/ttcps_v3_20;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/uartps_v3_15;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/usbpsu_v1_16;/tools/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/wdtps_v3_8)

list(LENGTH DRIVER_TARGETS no_of_drivers)
set(index 0)

while(${index} LESS ${no_of_drivers})
    list(GET DRIVER_TARGETS ${index} drv)
    list(GET DRIVER_LOCATIONS ${index} drv_dir)
    set(exp_dir "${drv_dir}/examples")
    add_custom_target(
        ${drv}_example ALL
        COMMAND lopper -f -O /home/denisaa/Desktop/Github/FPGA/platform/zynqmp_fsbl/zynqmp_fsbl_bsp/libsrc/${drv} /home/denisaa/Desktop/Github/FPGA/platform/zynqmp_fsbl/zynqmp_fsbl_bsp/hw_artifacts/psu_cortexa53_0_baremetal.dts -- bmcmake_metadata_xlnx psu_cortexa53_0 ${exp_dir} drvcmake_metadata
        BYPRODUCTS x${drv}_exlist.yaml)
    MATH(EXPR index "${index}+1")
endwhile()
