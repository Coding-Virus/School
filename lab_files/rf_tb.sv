module stimulus();
    logic clock;
    logic writeEnable;
    logic [4:0] read1, read2, writeAdress;
    logic [31:0] writeValue;
    
    logic [31:0] regValue1, regValue2;
    
    integer handle3;
    integer desc3;
    
    // Instantiate DUT
    regfile dut(.clk(clock), .we3(writeEnable), 
                .ra1(read1), .ra2(read2), 
                .wa3(writeAdress), .wd3(writeValue),
                .rd1(regValue1), .rd2(regValue2));



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
        #0  read1 = 5'b0;
        #0  read2 = 5'b0;
        #0  writeAdress = 5'b0;
        #0  writeValue = 32'd0;

        #5 read1 = 5'b11;          // test pos edge clock with we off
        #0 read2 = 5'b0;
        #5 writeAdress = 5'b11;
        #0 writeValue = 32'd12;

        #10 writeEnable = 5'b1;  //write value high

        #5 read1 = 5'b101;     // test read 2
        #0 read2 = 5'b11;

        #4 read1 = 5'b10;      // write at value read isnt looking
        #0 read2 = 5'b11;
        #0 writeAdress = 5'b1;
        #0 writeValue = 32'd15;

        #6 read1 = 5'b1;        // test rising edge
        #3 writeAdress = 5'b11;
        #0 writeValue = 32'd13;

 
        #3 writeValue = 32'd10;  // test falling edge




        /*#0  In = 1'b0;
        #20 In = 1'b1;
        #20 In = 1'b0;*/
      end

endmodule // rftb

