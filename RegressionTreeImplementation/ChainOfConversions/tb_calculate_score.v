module tb_calculate_score;

reg [10:0] features;
wire [4:0] prediction;

initial begin
    $from_myhdl(
        features
    );
    $to_myhdl(
        prediction
    );
end

calculate_score dut(
    features,
    prediction
);

endmodule
