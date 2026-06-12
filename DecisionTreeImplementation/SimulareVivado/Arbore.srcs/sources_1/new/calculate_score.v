`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/15/2026 02:11:27 PM
// Design Name: 
// Module Name: calculate_score
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

module calculate_score (
    features,
    prediction
);

input [10:0] features;
output prediction;
reg prediction;

always @(features) begin: score
    if ((features[10] == 0)) begin
        if ((features[0] == 0)) begin
            prediction = 1;
        end
        else begin
            prediction = 0;
        end
    end
    else begin
        if ((features[1] == 0)) begin
            prediction = 0;
        end
        else begin
            prediction = 1;
        end
    end
end

endmodule