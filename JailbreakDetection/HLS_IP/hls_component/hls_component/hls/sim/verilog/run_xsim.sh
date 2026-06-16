
/tools/Xilinx/Vivado/2024.2/bin/xelab xil_defaultlib.apatb_my_prj_accelerator_top glbl -Oenable_linking_all_libraries  -prj my_prj_accelerator.prj -L smartconnect_v1_0 -L axi_protocol_checker_v1_1_12 -L axi_protocol_checker_v1_1_13 -L axis_protocol_checker_v1_1_11 -L axis_protocol_checker_v1_1_12 -L xil_defaultlib -L unisims_ver -L xpm  -L floating_point_v7_0_24 -L floating_point_v7_1_19 --lib "ieee_proposed=./ieee_proposed" -s my_prj_accelerator -debug all
/tools/Xilinx/Vivado/2024.2/bin/xsim --noieeewarnings my_prj_accelerator -tclbatch my_prj_accelerator.tcl -gui -view my_prj_accelerator_dataflow_ana.wcfg -protoinst my_prj_accelerator.protoinst

