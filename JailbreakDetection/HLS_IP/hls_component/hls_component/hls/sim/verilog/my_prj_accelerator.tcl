
log_wave -r /
set designtopgroup [add_wave_group "Design Top Signals"]
set cinoutgroup [add_wave_group "C InOuts" -into $designtopgroup]
set N__return_group [add_wave_group N__return(axi_slave) -into $cinoutgroup]
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/interrupt -into $N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_BRESP -into $N__return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_BREADY -into $N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_BVALID -into $N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_RRESP -into $N__return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_RDATA -into $N__return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_RREADY -into $N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_RVALID -into $N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_ARREADY -into $N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_ARVALID -into $N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_ARADDR -into $N__return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_WSTRB -into $N__return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_WDATA -into $N__return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_WREADY -into $N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_WVALID -into $N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_AWREADY -into $N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_AWVALID -into $N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/s_axi_control_AWADDR -into $N__return_group -radix hex
set coutputgroup [add_wave_group "C Outputs" -into $designtopgroup]
set return_group [add_wave_group return(axis) -into $coutputgroup]
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/score_TREADY -into $return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/score_TVALID -into $return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/score_TLAST -into $return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/score_TSTRB -into $return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/score_TKEEP -into $return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/score_TDATA -into $return_group -radix hex
set cinputgroup [add_wave_group "C Inputs" -into $designtopgroup]
set return_group [add_wave_group return(axis) -into $cinputgroup]
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/x_TREADY -into $return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/x_TVALID -into $return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/x_TLAST -into $return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/x_TSTRB -into $return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/x_TKEEP -into $return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/x_TDATA -into $return_group -radix hex
set blocksiggroup [add_wave_group "Block-level IO Handshake(internal)" -into $designtopgroup]
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/ap_done -into $blocksiggroup
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/ap_idle -into $blocksiggroup
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/ap_ready -into $blocksiggroup
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/ap_start -into $blocksiggroup
set resetgroup [add_wave_group "Reset" -into $designtopgroup]
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/ap_rst_n -into $resetgroup
set clockgroup [add_wave_group "Clock" -into $designtopgroup]
add_wave /apatb_my_prj_accelerator_top/AESL_inst_my_prj_accelerator/ap_clk -into $clockgroup
set testbenchgroup [add_wave_group "Test Bench Signals"]
set tbinternalsiggroup [add_wave_group "Internal Signals" -into $testbenchgroup]
set tb_simstatus_group [add_wave_group "Simulation Status" -into $tbinternalsiggroup]
set tb_portdepth_group [add_wave_group "Port Depth" -into $tbinternalsiggroup]
add_wave /apatb_my_prj_accelerator_top/AUTOTB_TRANSACTION_NUM -into $tb_simstatus_group -radix hex
add_wave /apatb_my_prj_accelerator_top/ready_cnt -into $tb_simstatus_group -radix hex
add_wave /apatb_my_prj_accelerator_top/done_cnt -into $tb_simstatus_group -radix hex
add_wave /apatb_my_prj_accelerator_top/LENGTH_N -into $tb_portdepth_group -radix hex
add_wave /apatb_my_prj_accelerator_top/LENGTH_score_V_data_V -into $tb_portdepth_group -radix hex
add_wave /apatb_my_prj_accelerator_top/LENGTH_score_V_keep_V -into $tb_portdepth_group -radix hex
add_wave /apatb_my_prj_accelerator_top/LENGTH_score_V_last_V -into $tb_portdepth_group -radix hex
add_wave /apatb_my_prj_accelerator_top/LENGTH_score_V_strb_V -into $tb_portdepth_group -radix hex
add_wave /apatb_my_prj_accelerator_top/LENGTH_x_V_data_V -into $tb_portdepth_group -radix hex
add_wave /apatb_my_prj_accelerator_top/LENGTH_x_V_keep_V -into $tb_portdepth_group -radix hex
add_wave /apatb_my_prj_accelerator_top/LENGTH_x_V_last_V -into $tb_portdepth_group -radix hex
add_wave /apatb_my_prj_accelerator_top/LENGTH_x_V_strb_V -into $tb_portdepth_group -radix hex
set tbcinoutgroup [add_wave_group "C InOuts" -into $testbenchgroup]
set tb_N__return_group [add_wave_group N__return(axi_slave) -into $tbcinoutgroup]
add_wave /apatb_my_prj_accelerator_top/control_INTERRUPT -into $tb_N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/control_BRESP -into $tb_N__return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/control_BREADY -into $tb_N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/control_BVALID -into $tb_N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/control_RRESP -into $tb_N__return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/control_RDATA -into $tb_N__return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/control_RREADY -into $tb_N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/control_RVALID -into $tb_N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/control_ARREADY -into $tb_N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/control_ARVALID -into $tb_N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/control_ARADDR -into $tb_N__return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/control_WSTRB -into $tb_N__return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/control_WDATA -into $tb_N__return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/control_WREADY -into $tb_N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/control_WVALID -into $tb_N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/control_AWREADY -into $tb_N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/control_AWVALID -into $tb_N__return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/control_AWADDR -into $tb_N__return_group -radix hex
set tbcoutputgroup [add_wave_group "C Outputs" -into $testbenchgroup]
set tb_return_group [add_wave_group return(axis) -into $tbcoutputgroup]
add_wave /apatb_my_prj_accelerator_top/score_TREADY -into $tb_return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/score_TVALID -into $tb_return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/score_TLAST -into $tb_return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/score_TSTRB -into $tb_return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/score_TKEEP -into $tb_return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/score_TDATA -into $tb_return_group -radix hex
set tbcinputgroup [add_wave_group "C Inputs" -into $testbenchgroup]
set tb_return_group [add_wave_group return(axis) -into $tbcinputgroup]
add_wave /apatb_my_prj_accelerator_top/x_TREADY -into $tb_return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/x_TVALID -into $tb_return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/x_TLAST -into $tb_return_group -color #ffff00 -radix hex
add_wave /apatb_my_prj_accelerator_top/x_TSTRB -into $tb_return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/x_TKEEP -into $tb_return_group -radix hex
add_wave /apatb_my_prj_accelerator_top/x_TDATA -into $tb_return_group -radix hex
save_wave_config my_prj_accelerator.wcfg
run all

