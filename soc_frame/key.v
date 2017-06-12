module key(clk,rst_n,key,out_data);


input clk;
input rst_n;
input wire[12:0] key;
output reg[12:0] out_data;

reg[12:0] key_rst;
always @ ( posedge clk or negedge rst_n)
	if(!rst_n) key_rst <= 13'h1fff;
	else key_rst <= key;

reg[12:0] key_rst_r;
always @ ( posedge clk or negedge rst_n)
	if(!rst_n) key_rst_r <= 13'h1fff;
	else key_rst_r <= key_rst;
	
	
wire[12:0] compare = key_rst_r & (~key_rst);

reg[19:0] cnt;
always @ ( posedge clk or negedge rst_n)
	if(!rst_n) cnt <= 20'b0;
	else if(compare) cnt <= 20'b0;
	else cnt <= cnt + 1'b1;
	
reg[12:0] key_vilue;
always @ ( posedge clk or negedge rst_n)
	if(!rst_n) key_vilue <= 13'h1fff;
	else if(cnt == 20'hfffff)
			key_vilue <= key;
			
reg[12:0] key_vilue_r;
always @ ( posedge clk or negedge rst_n)
	if(!rst_n) key_vilue_r <= 13'h1fff;
	else key_vilue_r <= key_vilue;
	
wire[12:0] key_ctrl = key_vilue_r & (~key_vilue);

always @ ( posedge clk or negedge rst_n)
	if(!rst_n) out_data <= 13'b0;
	else out_data <= key_ctrl;




endmodule
