// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.2 (64-bit)
// Tool Version Limit: 2024.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================

`timescale 1 ns / 1 ps

`define TV_IN_x_TDATA "../tv/cdatafile/c.my_prj_accelerator.autotvin_x_V_data_V.dat"
`define INGRESS_STATUS_x_TDATA "../tv/stream_size/stream_ingress_status_x_V_data_V.dat"
`define TV_IN_x_TKEEP "../tv/cdatafile/c.my_prj_accelerator.autotvin_x_V_keep_V.dat"
`define INGRESS_STATUS_x_TKEEP "../tv/stream_size/stream_ingress_status_x_V_keep_V.dat"
`define TV_IN_x_TSTRB "../tv/cdatafile/c.my_prj_accelerator.autotvin_x_V_strb_V.dat"
`define INGRESS_STATUS_x_TSTRB "../tv/stream_size/stream_ingress_status_x_V_strb_V.dat"
`define TV_IN_x_TLAST "../tv/cdatafile/c.my_prj_accelerator.autotvin_x_V_last_V.dat"
`define INGRESS_STATUS_x_TLAST "../tv/stream_size/stream_ingress_status_x_V_last_V.dat"

`define AUTOTB_TRANSACTION_NUM 514

module AESL_axi_s_x (
    input clk,
    input reset,
    output [32 - 1:0] TRAN_x_TDATA,
    output [4 - 1:0] TRAN_x_TKEEP,
    output [4 - 1:0] TRAN_x_TSTRB,
    output TRAN_x_TLAST,
    output TRAN_x_TVALID,
    input TRAN_x_TREADY,
    input ready,
    input done,
    output [31:0] transaction);

    wire TRAN_x_TVALID_temp;
    wire x_TDATA_full;
    wire x_TDATA_empty;
    reg x_TDATA_write_en;
    reg [32 - 1:0] x_TDATA_write_data;
    reg x_TDATA_read_en;
    wire [32 - 1:0] x_TDATA_read_data;
    
    fifo #(100, 32) fifo_x_TDATA (
        .reset(1'b0),
        .write_clock(clk),
        .write_en(x_TDATA_write_en),
        .write_data(x_TDATA_write_data),
        .read_clock(clk),
        .read_en(x_TDATA_read_en),
        .read_data(x_TDATA_read_data),
        .full(x_TDATA_full),
        .empty(x_TDATA_empty));
    
    always @ (*) begin
        x_TDATA_write_en <= 0;
        x_TDATA_read_en <= TRAN_x_TREADY & TRAN_x_TVALID;
    end
    
    assign TRAN_x_TDATA = x_TDATA_read_data;
    wire x_TKEEP_full;
    wire x_TKEEP_empty;
    reg x_TKEEP_write_en;
    reg [4 - 1:0] x_TKEEP_write_data;
    reg x_TKEEP_read_en;
    wire [4 - 1:0] x_TKEEP_read_data;
    
    fifo #(100, 4) fifo_x_TKEEP (
        .reset(1'b0),
        .write_clock(clk),
        .write_en(x_TKEEP_write_en),
        .write_data(x_TKEEP_write_data),
        .read_clock(clk),
        .read_en(x_TKEEP_read_en),
        .read_data(x_TKEEP_read_data),
        .full(x_TKEEP_full),
        .empty(x_TKEEP_empty));
    
    always @ (*) begin
        x_TKEEP_write_en <= 0;
        x_TKEEP_read_en <= TRAN_x_TREADY & TRAN_x_TVALID;
    end
    
    assign TRAN_x_TKEEP = x_TKEEP_read_data;
    wire x_TSTRB_full;
    wire x_TSTRB_empty;
    reg x_TSTRB_write_en;
    reg [4 - 1:0] x_TSTRB_write_data;
    reg x_TSTRB_read_en;
    wire [4 - 1:0] x_TSTRB_read_data;
    
    fifo #(100, 4) fifo_x_TSTRB (
        .reset(1'b0),
        .write_clock(clk),
        .write_en(x_TSTRB_write_en),
        .write_data(x_TSTRB_write_data),
        .read_clock(clk),
        .read_en(x_TSTRB_read_en),
        .read_data(x_TSTRB_read_data),
        .full(x_TSTRB_full),
        .empty(x_TSTRB_empty));
    
    always @ (*) begin
        x_TSTRB_write_en <= 0;
        x_TSTRB_read_en <= TRAN_x_TREADY & TRAN_x_TVALID;
    end
    
    assign TRAN_x_TSTRB = x_TSTRB_read_data;
    wire x_TLAST_full;
    wire x_TLAST_empty;
    reg x_TLAST_write_en;
    reg [1 - 1:0] x_TLAST_write_data;
    reg x_TLAST_read_en;
    wire [1 - 1:0] x_TLAST_read_data;
    
    fifo #(100, 1) fifo_x_TLAST (
        .reset(1'b0),
        .write_clock(clk),
        .write_en(x_TLAST_write_en),
        .write_data(x_TLAST_write_data),
        .read_clock(clk),
        .read_en(x_TLAST_read_en),
        .read_data(x_TLAST_read_data),
        .full(x_TLAST_full),
        .empty(x_TLAST_empty));
    
    always @ (*) begin
        x_TLAST_write_en <= 0;
        x_TLAST_read_en <= TRAN_x_TREADY & TRAN_x_TVALID;
    end
    
    assign TRAN_x_TLAST = x_TLAST_read_data;
    assign TRAN_x_TVALID = TRAN_x_TVALID_temp;

    
    assign TRAN_x_TVALID_temp = ~(x_TDATA_empty || x_TKEEP_empty || x_TSTRB_empty || x_TLAST_empty);
    
    function is_blank_char(input [7:0] in_char);
        if (in_char == " " || in_char == "\011" || in_char == "\012" || in_char == "\015") begin
            is_blank_char = 1;
        end else begin
            is_blank_char = 0;
        end
    endfunction
    
    function [191:0] read_token(input integer fp);
        integer ret;
        begin
            read_token = "";
                    ret = 0;
                    ret = $fscanf(fp,"%s",read_token);
        end
    endfunction
    
    function [191:0] rm_0x(input [191:0] token);
        reg [191:0] token_tmp;
        integer i;
        begin
            token_tmp = "";
            for (i = 0; token[15:0] != "0x"; token = token >> 8) begin
                token_tmp = (token[7:0] << (8 * i)) | token_tmp;
                i = i + 1;
            end
            rm_0x = token_tmp;
        end
    endfunction
    
    reg [31:0] transaction_load_x_TDATA;
    
    assign transaction = transaction_load_x_TDATA;
    
    initial begin : AXI_stream_driver_x_TDATA
        integer fp;
        reg [191:0] token;
        reg [32 - 1:0] data;
        reg [191:0] data_integer;
        integer fp_ingress_status;
        reg [191:0] token_ingress_status;
        reg [31:0] ingress_status;
        reg [8 * 5:1] str;
        integer ret;
        
        transaction_load_x_TDATA = 0;
        fifo_x_TDATA.clear();
        wait (reset === 1);
        fp = $fopen(`TV_IN_x_TDATA, "r");
        if (fp == 0) begin // Failed to open file
            $display("ERROR: Failed to open file \"%s\"!", `TV_IN_x_TDATA);
            $finish;
        end
        fp_ingress_status = $fopen(`INGRESS_STATUS_x_TDATA, "r");
        if (fp_ingress_status == 0) begin // Failed to open file
            $display("ERROR: Failed to open file \"%s\"!", `INGRESS_STATUS_x_TDATA);
            $finish;
        end
        token = read_token(fp);
        token_ingress_status = read_token(fp_ingress_status);
        if (token != "[[[runtime]]]") begin
            $display("ERROR: token %s != [[[runtime]]]", token);
            $finish;
        end
        token = read_token(fp); // read 1st "[[transaction]]"
        token_ingress_status = read_token(fp_ingress_status);
        forever begin
            @ (negedge clk);
            if (ready == 1) begin
                if (token != "[[[/runtime]]]") begin
                    if (token != "[[transaction]]") begin
                        $display("ERROR: token %s != [[[transaction]]]", token);
                        $finish;
                    end
                    token = read_token(fp); // skip transaction number
                    token_ingress_status = read_token(fp_ingress_status); // skip transaction number
                    token_ingress_status = read_token(fp_ingress_status); // load ingress status at beginning of transaction
                    fifo_x_TDATA.clear();
                    token = read_token(fp);
                    ret = $sscanf(token_ingress_status, "%d", ingress_status);
                    token_ingress_status = read_token(fp_ingress_status);
                    while (token != "[[/transaction]]") begin
                        if (fifo_x_TDATA.full) begin
                            $display("ERROR: FIFO is full");
                            $finish;
                        end
                        ret = $sscanf(rm_0x(token), "%x", data_integer);
                        data = data_integer;
                        fifo_x_TDATA.push(data);
                        token = read_token(fp);
                        ret = $sscanf(token_ingress_status, "%d", ingress_status);
                        token_ingress_status = read_token(fp_ingress_status);
                    end
                    token = read_token(fp);
                    token_ingress_status = read_token(fp_ingress_status);
                end else begin
                    if (fp != 0) begin
                        $fclose(fp);
                        fp = 0;
                    end
                    if (fp_ingress_status != 0) begin
                        $fclose(fp_ingress_status);
                        fp_ingress_status = 0;
                    end
                end
                transaction_load_x_TDATA = transaction_load_x_TDATA + 1;
            end
        end
    end
    
    reg [31:0] transaction_load_x_TKEEP;
    
    initial begin : AXI_stream_driver_x_TKEEP
        integer fp;
        reg [191:0] token;
        reg [4 - 1:0] data;
        reg [191:0] data_integer;
        integer fp_ingress_status;
        reg [191:0] token_ingress_status;
        reg [31:0] ingress_status;
        reg [8 * 5:1] str;
        integer ret;
        
        transaction_load_x_TKEEP = 0;
        fifo_x_TKEEP.clear();
        wait (reset === 1);
        fp = $fopen(`TV_IN_x_TKEEP, "r");
        if (fp == 0) begin // Failed to open file
            $display("ERROR: Failed to open file \"%s\"!", `TV_IN_x_TKEEP);
            $finish;
        end
        fp_ingress_status = $fopen(`INGRESS_STATUS_x_TKEEP, "r");
        if (fp_ingress_status == 0) begin // Failed to open file
            $display("ERROR: Failed to open file \"%s\"!", `INGRESS_STATUS_x_TKEEP);
            $finish;
        end
        token = read_token(fp);
        token_ingress_status = read_token(fp_ingress_status);
        if (token != "[[[runtime]]]") begin
            $display("ERROR: token %s != [[[runtime]]]", token);
            $finish;
        end
        token = read_token(fp); // read 1st "[[transaction]]"
        token_ingress_status = read_token(fp_ingress_status);
        forever begin
            @ (negedge clk);
            if (ready == 1) begin
                if (token != "[[[/runtime]]]") begin
                    if (token != "[[transaction]]") begin
                        $display("ERROR: token %s != [[[transaction]]]", token);
                        $finish;
                    end
                    token = read_token(fp); // skip transaction number
                    token_ingress_status = read_token(fp_ingress_status); // skip transaction number
                    token_ingress_status = read_token(fp_ingress_status); // load ingress status at beginning of transaction
                    fifo_x_TKEEP.clear();
                    token = read_token(fp);
                    ret = $sscanf(token_ingress_status, "%d", ingress_status);
                    token_ingress_status = read_token(fp_ingress_status);
                    while (token != "[[/transaction]]") begin
                        if (fifo_x_TKEEP.full) begin
                            $display("ERROR: FIFO is full");
                            $finish;
                        end
                        ret = $sscanf(rm_0x(token), "%x", data_integer);
                        data = data_integer;
                        fifo_x_TKEEP.push(data);
                        token = read_token(fp);
                        ret = $sscanf(token_ingress_status, "%d", ingress_status);
                        token_ingress_status = read_token(fp_ingress_status);
                    end
                    token = read_token(fp);
                    token_ingress_status = read_token(fp_ingress_status);
                end else begin
                    if (fp != 0) begin
                        $fclose(fp);
                        fp = 0;
                    end
                    if (fp_ingress_status != 0) begin
                        $fclose(fp_ingress_status);
                        fp_ingress_status = 0;
                    end
                end
                transaction_load_x_TKEEP = transaction_load_x_TKEEP + 1;
            end
        end
    end
    
    reg [31:0] transaction_load_x_TSTRB;
    
    initial begin : AXI_stream_driver_x_TSTRB
        integer fp;
        reg [191:0] token;
        reg [4 - 1:0] data;
        reg [191:0] data_integer;
        integer fp_ingress_status;
        reg [191:0] token_ingress_status;
        reg [31:0] ingress_status;
        reg [8 * 5:1] str;
        integer ret;
        
        transaction_load_x_TSTRB = 0;
        fifo_x_TSTRB.clear();
        wait (reset === 1);
        fp = $fopen(`TV_IN_x_TSTRB, "r");
        if (fp == 0) begin // Failed to open file
            $display("ERROR: Failed to open file \"%s\"!", `TV_IN_x_TSTRB);
            $finish;
        end
        fp_ingress_status = $fopen(`INGRESS_STATUS_x_TSTRB, "r");
        if (fp_ingress_status == 0) begin // Failed to open file
            $display("ERROR: Failed to open file \"%s\"!", `INGRESS_STATUS_x_TSTRB);
            $finish;
        end
        token = read_token(fp);
        token_ingress_status = read_token(fp_ingress_status);
        if (token != "[[[runtime]]]") begin
            $display("ERROR: token %s != [[[runtime]]]", token);
            $finish;
        end
        token = read_token(fp); // read 1st "[[transaction]]"
        token_ingress_status = read_token(fp_ingress_status);
        forever begin
            @ (negedge clk);
            if (ready == 1) begin
                if (token != "[[[/runtime]]]") begin
                    if (token != "[[transaction]]") begin
                        $display("ERROR: token %s != [[[transaction]]]", token);
                        $finish;
                    end
                    token = read_token(fp); // skip transaction number
                    token_ingress_status = read_token(fp_ingress_status); // skip transaction number
                    token_ingress_status = read_token(fp_ingress_status); // load ingress status at beginning of transaction
                    fifo_x_TSTRB.clear();
                    token = read_token(fp);
                    ret = $sscanf(token_ingress_status, "%d", ingress_status);
                    token_ingress_status = read_token(fp_ingress_status);
                    while (token != "[[/transaction]]") begin
                        if (fifo_x_TSTRB.full) begin
                            $display("ERROR: FIFO is full");
                            $finish;
                        end
                        ret = $sscanf(rm_0x(token), "%x", data_integer);
                        data = data_integer;
                        fifo_x_TSTRB.push(data);
                        token = read_token(fp);
                        ret = $sscanf(token_ingress_status, "%d", ingress_status);
                        token_ingress_status = read_token(fp_ingress_status);
                    end
                    token = read_token(fp);
                    token_ingress_status = read_token(fp_ingress_status);
                end else begin
                    if (fp != 0) begin
                        $fclose(fp);
                        fp = 0;
                    end
                    if (fp_ingress_status != 0) begin
                        $fclose(fp_ingress_status);
                        fp_ingress_status = 0;
                    end
                end
                transaction_load_x_TSTRB = transaction_load_x_TSTRB + 1;
            end
        end
    end
    
    reg [31:0] transaction_load_x_TLAST;
    
    initial begin : AXI_stream_driver_x_TLAST
        integer fp;
        reg [191:0] token;
        reg [1 - 1:0] data;
        reg [191:0] data_integer;
        integer fp_ingress_status;
        reg [191:0] token_ingress_status;
        reg [31:0] ingress_status;
        reg [8 * 5:1] str;
        integer ret;
        
        transaction_load_x_TLAST = 0;
        fifo_x_TLAST.clear();
        wait (reset === 1);
        fp = $fopen(`TV_IN_x_TLAST, "r");
        if (fp == 0) begin // Failed to open file
            $display("ERROR: Failed to open file \"%s\"!", `TV_IN_x_TLAST);
            $finish;
        end
        fp_ingress_status = $fopen(`INGRESS_STATUS_x_TLAST, "r");
        if (fp_ingress_status == 0) begin // Failed to open file
            $display("ERROR: Failed to open file \"%s\"!", `INGRESS_STATUS_x_TLAST);
            $finish;
        end
        token = read_token(fp);
        token_ingress_status = read_token(fp_ingress_status);
        if (token != "[[[runtime]]]") begin
            $display("ERROR: token %s != [[[runtime]]]", token);
            $finish;
        end
        token = read_token(fp); // read 1st "[[transaction]]"
        token_ingress_status = read_token(fp_ingress_status);
        forever begin
            @ (negedge clk);
            if (ready == 1) begin
                if (token != "[[[/runtime]]]") begin
                    if (token != "[[transaction]]") begin
                        $display("ERROR: token %s != [[[transaction]]]", token);
                        $finish;
                    end
                    token = read_token(fp); // skip transaction number
                    token_ingress_status = read_token(fp_ingress_status); // skip transaction number
                    token_ingress_status = read_token(fp_ingress_status); // load ingress status at beginning of transaction
                    fifo_x_TLAST.clear();
                    token = read_token(fp);
                    ret = $sscanf(token_ingress_status, "%d", ingress_status);
                    token_ingress_status = read_token(fp_ingress_status);
                    while (token != "[[/transaction]]") begin
                        if (fifo_x_TLAST.full) begin
                            $display("ERROR: FIFO is full");
                            $finish;
                        end
                        ret = $sscanf(rm_0x(token), "%x", data_integer);
                        data = data_integer;
                        fifo_x_TLAST.push(data);
                        token = read_token(fp);
                        ret = $sscanf(token_ingress_status, "%d", ingress_status);
                        token_ingress_status = read_token(fp_ingress_status);
                    end
                    token = read_token(fp);
                    token_ingress_status = read_token(fp_ingress_status);
                end else begin
                    if (fp != 0) begin
                        $fclose(fp);
                        fp = 0;
                    end
                    if (fp_ingress_status != 0) begin
                        $fclose(fp_ingress_status);
                        fp_ingress_status = 0;
                    end
                end
                transaction_load_x_TLAST = transaction_load_x_TLAST + 1;
            end
        end
    end

endmodule
