module regfile(input  logic        clk, 
               input  logic        we3, 
               input  logic [4:0]  ra1, ra2, wa3, 
               input  logic [31:0] wd3, 
               output logic [31:0] rd1, rd2);

  logic [31:0]     rf[31:0];

  // register 0 hardwired to 0
initial
begin
	 rf[0] = 0;
end
  // three ported register file
  // read two ports combinationally
always_comb 
	begin
	rd1 = ra1 == wa3 ? wd3 : rf[ra1];
	rd2 = ra2 == wa3 ? wd3 : rf[ra2];
	end
  // write third port on rising edge of clock
always @(posedge clk, wa3 != 5'b00000)
begin
	
	//if (we3)
	//{
		//if(wa3 != 5'b00000)
		//{
		//rf[wa3] = wd3
		//}
	//}
	rf[wa3] = we3 ? wd3  : rf[wa3];
end



endmodule // regfile
