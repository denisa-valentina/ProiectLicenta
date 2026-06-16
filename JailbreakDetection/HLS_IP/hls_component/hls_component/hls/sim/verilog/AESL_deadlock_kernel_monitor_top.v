`timescale 1 ns / 1 ps

module AESL_deadlock_kernel_monitor_top ( 
    input wire kernel_monitor_clock,
    input wire kernel_monitor_reset
);
wire [1:0] axis_block_sigs;
wire [4:0] inst_idle_sigs;
wire [1:0] inst_block_sigs;
wire kernel_block;

assign axis_block_sigs[0] = ~AESL_inst_my_prj_accelerator.Loop_VITIS_LOOP_47_1_proc_U0.score_TDATA_blk_n;
assign axis_block_sigs[1] = ~AESL_inst_my_prj_accelerator.Loop_VITIS_LOOP_47_1_proc_U0.grp_Loop_VITIS_LOOP_47_1_proc_Pipeline_VITIS_LOOP_16_1_fu_998.x_TDATA_blk_n;

assign inst_idle_sigs[0] = AESL_inst_my_prj_accelerator.Block_entry_proc_U0.ap_idle;
assign inst_block_sigs[0] = (AESL_inst_my_prj_accelerator.Block_entry_proc_U0.ap_done & ~AESL_inst_my_prj_accelerator.Block_entry_proc_U0.ap_continue) | ~AESL_inst_my_prj_accelerator.Block_entry_proc_U0.N_c_blk_n;
assign inst_idle_sigs[1] = AESL_inst_my_prj_accelerator.Loop_VITIS_LOOP_47_1_proc_U0.ap_idle;
assign inst_block_sigs[1] = (AESL_inst_my_prj_accelerator.Loop_VITIS_LOOP_47_1_proc_U0.ap_done & ~AESL_inst_my_prj_accelerator.Loop_VITIS_LOOP_47_1_proc_U0.ap_continue) | ~AESL_inst_my_prj_accelerator.Loop_VITIS_LOOP_47_1_proc_U0.N_blk_n;

assign inst_idle_sigs[2] = 1'b0;
assign inst_idle_sigs[3] = AESL_inst_my_prj_accelerator.Loop_VITIS_LOOP_47_1_proc_U0.ap_idle;
assign inst_idle_sigs[4] = AESL_inst_my_prj_accelerator.Loop_VITIS_LOOP_47_1_proc_U0.grp_Loop_VITIS_LOOP_47_1_proc_Pipeline_VITIS_LOOP_16_1_fu_998.ap_idle;

AESL_deadlock_idx0_monitor AESL_deadlock_idx0_monitor_U (
    .clock(kernel_monitor_clock),
    .reset(kernel_monitor_reset),
    .axis_block_sigs(axis_block_sigs),
    .inst_idle_sigs(inst_idle_sigs),
    .inst_block_sigs(inst_block_sigs),
    .block(kernel_block)
);


initial begin : trigger_axis_deadlock
reg block_delay;
    block_delay = 0;
    while(1) begin
        @(posedge kernel_monitor_clock);
    if (kernel_block == 1'b1 && block_delay == 1'b0)
        block_delay = kernel_block;
    end
end

endmodule
