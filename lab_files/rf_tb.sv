module rftb();
    logic clock;
    logic writeEnable;
    logic read1, read2, writeAdress;
    logic writeValue;
    
    logic regValue1, regValue2;
    
    integer handle3;
    integer desc3;
    
    // Instantiate DUT
    regfile dut(.clk(clock), .we3(writeEnable), 
                .ra1(read1), .ra2(read2), 
                .wa3(writeAdress), .wd3(writeValue)
                .rd1(regValue1), .rd2(regValue2)
    );



    // Setup the clock to toggle every 1 time units 
    initial 
      begin  
        clock = 1'b1;
        forever #5 clock = ~clock;
      end

    initial
      begin
        // Gives output file name
        handle3 = $fopen("test.out");
        // Tells when to finish simulation
        #500 $finish;         
      end

    always 
      begin
        desc3 = handle3;
        #5 $fdisplay(desc3, "%b || %b %b || %b %b || %b %b", writeEnable, read1, read2, writeAdress, writeValue, regValue1, regValue2);
      end

    initial 
      begin 
        #0  writeEnable = 5'b0;
        #30 writeEnable = 5'b1;

        #0  read1 = 5'b0;
        #15 read1 = 5'b10;

        #0  read2 = 5'b0;
        #15 read2 = 5'b11;

        #0  writeAdress = 5'b0;
        #15 writeAdress = 5'b10;

        #0 writeValue = 32'd0;
        #15 writeValue = 32'd12;




        /*#0  In = 1'b0;
        #20 In = 1'b1;
        #20 In = 1'b0;*/
      end

endmodule // rftb

