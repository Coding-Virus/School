module regfile(input  logic        clk, 
               input  logic        we3, 
               input  logic [4:0]  ra1, ra2, wa3, 
               input  logic [31:0] wd3, 
               output logic [31:0] rd1, rd2);

  logic [31:0]     rf[31:0];

  // register 0 hardwired to 0
initial
begin
	for(int i=0; i <=31; i++)
	begin
		 rf[i] = 0;
	end
end
  // three ported register file
  // read two ports combinationally
always_comb 
	begin
	rd1 = rf[ra1];
	rd2 = rf[ra2];
	end
  // write third port on rising edge of clock
always @(posedge clk)
begin
	
	//if (we3)
	//begin
		//if(wa3 != 5'b00000)
		//begin
		//rf[wa3] = wd3;
		//end
	//end
	rf[wa3] = (we3 && wa3 != 0) ? wd3  : rf[wa3];
end



endmodule // regfile
