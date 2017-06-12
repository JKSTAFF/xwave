module fp_clk(clk,sclk,sclk1);
input clk;
output sclk;
output sclk1;

reg sclk;
reg sclk1;

reg[15:0] cnt;
always @ (posedge clk)
	if(cnt>16'd50000)begin
			sclk <= ~sclk;
			cnt <= 16'b0;
			end
	else
		cnt <= cnt + 1'b1;

reg[15:0] cnt1;
always @ (posedge clk)
	if(cnt>=16'd500000)begin
			sclk1 <= ~sclk1;
			cnt1 <= 16'b0;
			end
	else
		cnt1 <= cnt1 + 1'b1;


















endmodule
