`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/15/2026 02:12:19 PM
// Design Name: 
// Module Name: testbench
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////
module testbench;

reg [10:0] features, inputDatak;
reg clk;
wire prediction;

reg [10:0] inputData [0:4619];
reg realOutput [0:4619];
integer k, i, TP, TN, FP, FN;
real acc, prec, rec, f1;

calculate_score dut(
    features,
    prediction
);

always #1 clk = ~clk;

initial
begin
    clk = 0;
    TP = 0; TN = 0; FP = 0; FN = 0;
    $readmemb("../../../../X_test", inputData);
    $readmemb("../../../../Y_test", realOutput);
end

initial
begin 
    #20;
    for(k = 0; k < 4620; k = k + 1)
    begin
        @(posedge clk);
        for(i = 0; i < 11; i = i + 1) begin
            inputDatak[i] = inputData[k][10-i];
        end
        features = inputDatak;
        #1;
        case({prediction, realOutput[k]})
            2'b11: // TRUE POSITIVE
                TP = TP + 1;
            2'b00: // TRUE NEGATIVE
                TN = TN + 1;
            2'b10: // FALSE POSITIVE
                FP = FP + 1;
            2'b01: // FALSE NEGATIVE
                FN = FN + 1;
        endcase
    end
    
     $display("FN = %d", FN); //    FN =         228
     $display("FP = %d", FP); //    FP =         705
     $display("TN = %d", TN); //    TN =         887
     $display("TP = %d", TP); //    TP =        2800
     acc = 1.0 * (TP+TN)/(TP + TN + FP + FN);
     prec = 1.0 * TP/(TP+FP);
     rec = 1.0 * TP/(TP+FN);
     f1 = 2.0 * rec * prec / (rec + prec);
     $display("Accuracy = %f", acc);
     $display("Precision = %f", prec);
     $display("Recall = %f", rec);
     $display("F1 = %f", f1); 
     $display("Total = %d", TP+TN+FP+FN);
     $finish;
end



endmodule
