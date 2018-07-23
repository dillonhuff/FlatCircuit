//
//--------------------------------------------------------------------------------
//          THIS FILE WAS AUTOMATICALLY GENERATED BY THE GENESIS2 ENGINE        
//  FOR MORE INFORMATION: OFER SHACHAM (CHIP GENESIS INC / STANFORD VLSI GROUP)
//    !! THIS VERSION OF GENESIS2 IS NOT FOR ANY COMMERCIAL USE !!
//     FOR COMMERCIAL LICENSE CONTACT SHACHAM@ALUMNI.STANFORD.EDU
//--------------------------------------------------------------------------------
//
//  
//	-----------------------------------------------
//	|            Genesis Release Info             |
//	|  $Change: 11904 $ --- $Date: 2013/08/03 $   |
//	-----------------------------------------------
//	
//
//  Source file: /horowitz/users/dhuff/CGRAGenerator/hardware/generator_z/sb/sb.vp
//  Source template: sb
//
// --------------- Begin Pre-Generation Parameters Status Report ---------------
//
//	From 'generate' statement (priority=5):
// Parameter is_bidi 	= 0
// Parameter pe_output_count 	= 1
// Parameter sb_fs 	= 10000#10000#10000
// Parameter width 	= 1
// Parameter feedthrough_outputs 	= 00000
// Parameter sides 	= 4
// Parameter registered_outputs 	= 11111
// Parameter num_tracks 	= 5
//
//		---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//
//	From Command Line input (priority=4):
//
//		---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//
//	From XML input (priority=3):
//
//		---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//
//	From Config File input (priority=2):
//
// ---------------- End Pre-Generation Pramameters Status Report ----------------

// width (_GENESIS2_INHERITANCE_PRIORITY_) = 1
//
// num_tracks (_GENESIS2_INHERITANCE_PRIORITY_) = 5
//
// sides (_GENESIS2_INHERITANCE_PRIORITY_) = 4
//
// feedthrough_outputs (_GENESIS2_INHERITANCE_PRIORITY_) = 00000
//
// registered_outputs (_GENESIS2_INHERITANCE_PRIORITY_) = 0x2b67
//
// pe_output_count (_GENESIS2_INHERITANCE_PRIORITY_) = 1
//
// is_bidi (_GENESIS2_INHERITANCE_PRIORITY_) = 0
//
// sb_fs (_GENESIS2_INHERITANCE_PRIORITY_) = 10000#10000#10000
//



module sb_unq2 (
clk, clk_en, reset, 
pe_output_0,
out_0_0,
in_0_0,
out_0_1,
in_0_1,
out_0_2,
in_0_2,
out_0_3,
in_0_3,
out_0_4,
in_0_4,
out_1_0,
in_1_0,
out_1_1,
in_1_1,
out_1_2,
in_1_2,
out_1_3,
in_1_3,
out_1_4,
in_1_4,
out_2_0,
in_2_0,
out_2_1,
in_2_1,
out_2_2,
in_2_2,
out_2_3,
in_2_3,
out_2_4,
in_2_4,
out_3_0,
in_3_0,
out_3_1,
in_3_1,
out_3_2,
in_3_2,
out_3_3,
in_3_3,
out_3_4,
in_3_4,
config_addr,
config_data,
config_en,
read_data
);

  input  clk;
  input  clk_en;
  input  reset;
  input  config_en;
  input [31:0] config_data;

  // FIXME
  // Bits [31:24] are for switch box, [23:0] for connection box.
  // So bits  [23:0] go unused in this module.  Until this is fixed,
  // we have to let the linter know (verilator directives below) or it complains:
  // %Warning-UNUSED: .../sb_unq1.v:115: Bits of signal are not used: config_addr[23:0]
  /* verilator lint_off UNUSED */
  input [31:0] config_addr;
  /* verilator lint_on UNUSED */

  input [0:0] pe_output_0;
  
  output [0:0] out_0_0;
  input [0:0] in_0_0;
  output [0:0] out_0_1;
  input [0:0] in_0_1;
  output [0:0] out_0_2;
  input [0:0] in_0_2;
  output [0:0] out_0_3;
  input [0:0] in_0_3;
  output [0:0] out_0_4;
  input [0:0] in_0_4;
  output [0:0] out_1_0;
  input [0:0] in_1_0;
  output [0:0] out_1_1;
  input [0:0] in_1_1;
  output [0:0] out_1_2;
  input [0:0] in_1_2;
  output [0:0] out_1_3;
  input [0:0] in_1_3;
  output [0:0] out_1_4;
  input [0:0] in_1_4;
  output [0:0] out_2_0;
  input [0:0] in_2_0;
  output [0:0] out_2_1;
  input [0:0] in_2_1;
  output [0:0] out_2_2;
  input [0:0] in_2_2;
  output [0:0] out_2_3;
  input [0:0] in_2_3;
  output [0:0] out_2_4;
  input [0:0] in_2_4;
  output [0:0] out_3_0;
  input [0:0] in_3_0;
  output [0:0] out_3_1;
  input [0:0] in_3_1;
  output [0:0] out_3_2;
  input [0:0] in_3_2;
  output [0:0] out_3_3;
  input [0:0] in_3_3;
  output [0:0] out_3_4;
  input [0:0] in_3_4;
  output reg [31:0] read_data;






  reg [63:0] config_sb;
  
  always @(posedge clk or posedge reset) begin
    if (reset==1'b1) begin

      config_sb <= 64'h000000FFFFFFFFFF;

    end else begin
      if (config_en==1'b1) begin
         case (config_addr[31:24])
           8'd0: config_sb[31:0] <= config_data;
           8'd1: config_sb[63:32] <= config_data;
           default: ;
         endcase
      end
    end
  end


  reg [31:0] config_ungate;
  
  always @(posedge clk or posedge reset) begin
    if (reset==1'b1) begin

      config_ungate <= 32'h0;

    end else begin
      if (config_en==1'b1) begin
         case (config_addr[31:24])
           8'd2: config_ungate[31:0] <= config_data;
           default: ;
         endcase
      end
    end
  end

  reg [0:0] out_0_0_i;
  always @(*) begin
    case (config_sb[1:0])
        2'd0: out_0_0_i = in_1_0;
        2'd1: out_0_0_i = in_2_0;
        2'd2: out_0_0_i = in_3_0;
        2'd3: out_0_0_i = pe_output_0;
        default: out_0_0_i = 0;
    endcase
  end
  reg [0:0] out_0_0_id1;
  wire out_0_0_le;
  assign out_0_0_le = clk_en | (config_ungate[0]); 
  always @(posedge clk) begin
    if (out_0_0_le)
      out_0_0_id1 <= out_0_0_i;
  end
  assign out_0_0 = config_sb[40]?out_0_0_id1:out_0_0_i; 
  reg [0:0] out_0_1_i;
  always @(*) begin
    case (config_sb[3:2])
        2'd0: out_0_1_i = in_1_1;
        2'd1: out_0_1_i = in_2_1;
        2'd2: out_0_1_i = in_3_1;
        2'd3: out_0_1_i = pe_output_0;
        default: out_0_1_i = 0;
    endcase
  end
  reg [0:0] out_0_1_id1;
  wire out_0_1_le;
  assign out_0_1_le = clk_en | (config_ungate[1]); 
  always @(posedge clk) begin
    if (out_0_1_le)
      out_0_1_id1 <= out_0_1_i;
  end
  assign out_0_1 = config_sb[41]?out_0_1_id1:out_0_1_i; 
  reg [0:0] out_0_2_i;
  always @(*) begin
    case (config_sb[5:4])
        2'd0: out_0_2_i = in_1_2;
        2'd1: out_0_2_i = in_2_2;
        2'd2: out_0_2_i = in_3_2;
        2'd3: out_0_2_i = pe_output_0;
        default: out_0_2_i = 0;
    endcase
  end
  reg [0:0] out_0_2_id1;
  wire out_0_2_le;
  assign out_0_2_le = clk_en | (config_ungate[2]); 
  always @(posedge clk) begin
    if (out_0_2_le)
      out_0_2_id1 <= out_0_2_i;
  end
  assign out_0_2 = config_sb[42]?out_0_2_id1:out_0_2_i; 
  reg [0:0] out_0_3_i;
  always @(*) begin
    case (config_sb[7:6])
        2'd0: out_0_3_i = in_1_3;
        2'd1: out_0_3_i = in_2_3;
        2'd2: out_0_3_i = in_3_3;
        2'd3: out_0_3_i = pe_output_0;
        default: out_0_3_i = 0;
    endcase
  end
  reg [0:0] out_0_3_id1;
  wire out_0_3_le;
  assign out_0_3_le = clk_en | (config_ungate[3]); 
  always @(posedge clk) begin
    if (out_0_3_le)
      out_0_3_id1 <= out_0_3_i;
  end
  assign out_0_3 = config_sb[43]?out_0_3_id1:out_0_3_i; 
  reg [0:0] out_0_4_i;
  always @(*) begin
    case (config_sb[9:8])
        2'd0: out_0_4_i = in_1_4;
        2'd1: out_0_4_i = in_2_4;
        2'd2: out_0_4_i = in_3_4;
        2'd3: out_0_4_i = pe_output_0;
        default: out_0_4_i = 0;
    endcase
  end
  reg [0:0] out_0_4_id1;
  wire out_0_4_le;
  assign out_0_4_le = clk_en | (config_ungate[4]); 
  always @(posedge clk) begin
    if (out_0_4_le)
      out_0_4_id1 <= out_0_4_i;
  end
  assign out_0_4 = config_sb[44]?out_0_4_id1:out_0_4_i; 
  reg [0:0] out_1_0_i;
  always @(*) begin
    case (config_sb[11:10])
        2'd0: out_1_0_i = in_0_0;
        2'd1: out_1_0_i = in_2_0;
        2'd2: out_1_0_i = in_3_0;
        2'd3: out_1_0_i = pe_output_0;
        default: out_1_0_i = 0;
    endcase
  end
  reg [0:0] out_1_0_id1;
  wire out_1_0_le;
  assign out_1_0_le = clk_en | (config_ungate[5]); 
  always @(posedge clk) begin
    if (out_1_0_le)
      out_1_0_id1 <= out_1_0_i;
  end
  assign out_1_0 = config_sb[45]?out_1_0_id1:out_1_0_i; 
  reg [0:0] out_1_1_i;
  always @(*) begin
    case (config_sb[13:12])
        2'd0: out_1_1_i = in_0_1;
        2'd1: out_1_1_i = in_2_1;
        2'd2: out_1_1_i = in_3_1;
        2'd3: out_1_1_i = pe_output_0;
        default: out_1_1_i = 0;
    endcase
  end
  reg [0:0] out_1_1_id1;
  wire out_1_1_le;
  assign out_1_1_le = clk_en | (config_ungate[6]); 
  always @(posedge clk) begin
    if (out_1_1_le)
      out_1_1_id1 <= out_1_1_i;
  end
  assign out_1_1 = config_sb[46]?out_1_1_id1:out_1_1_i; 
  reg [0:0] out_1_2_i;
  always @(*) begin
    case (config_sb[15:14])
        2'd0: out_1_2_i = in_0_2;
        2'd1: out_1_2_i = in_2_2;
        2'd2: out_1_2_i = in_3_2;
        2'd3: out_1_2_i = pe_output_0;
        default: out_1_2_i = 0;
    endcase
  end
  reg [0:0] out_1_2_id1;
  wire out_1_2_le;
  assign out_1_2_le = clk_en | (config_ungate[7]); 
  always @(posedge clk) begin
    if (out_1_2_le)
      out_1_2_id1 <= out_1_2_i;
  end
  assign out_1_2 = config_sb[47]?out_1_2_id1:out_1_2_i; 
  reg [0:0] out_1_3_i;
  always @(*) begin
    case (config_sb[17:16])
        2'd0: out_1_3_i = in_0_3;
        2'd1: out_1_3_i = in_2_3;
        2'd2: out_1_3_i = in_3_3;
        2'd3: out_1_3_i = pe_output_0;
        default: out_1_3_i = 0;
    endcase
  end
  reg [0:0] out_1_3_id1;
  wire out_1_3_le;
  assign out_1_3_le = clk_en | (config_ungate[8]); 
  always @(posedge clk) begin
    if (out_1_3_le)
      out_1_3_id1 <= out_1_3_i;
  end
  assign out_1_3 = config_sb[48]?out_1_3_id1:out_1_3_i; 
  reg [0:0] out_1_4_i;
  always @(*) begin
    case (config_sb[19:18])
        2'd0: out_1_4_i = in_0_4;
        2'd1: out_1_4_i = in_2_4;
        2'd2: out_1_4_i = in_3_4;
        2'd3: out_1_4_i = pe_output_0;
        default: out_1_4_i = 0;
    endcase
  end
  reg [0:0] out_1_4_id1;
  wire out_1_4_le;
  assign out_1_4_le = clk_en | (config_ungate[9]); 
  always @(posedge clk) begin
    if (out_1_4_le)
      out_1_4_id1 <= out_1_4_i;
  end
  assign out_1_4 = config_sb[49]?out_1_4_id1:out_1_4_i; 
  reg [0:0] out_2_0_i;
  always @(*) begin
    case (config_sb[21:20])
        2'd0: out_2_0_i = in_0_0;
        2'd1: out_2_0_i = in_1_0;
        2'd2: out_2_0_i = in_3_0;
        2'd3: out_2_0_i = pe_output_0;
        default: out_2_0_i = 0;
    endcase
  end
  reg [0:0] out_2_0_id1;
  wire out_2_0_le;
  assign out_2_0_le = clk_en | (config_ungate[10]); 
  always @(posedge clk) begin
    if (out_2_0_le)
      out_2_0_id1 <= out_2_0_i;
  end
  assign out_2_0 = config_sb[50]?out_2_0_id1:out_2_0_i; 
  reg [0:0] out_2_1_i;
  always @(*) begin
    case (config_sb[23:22])
        2'd0: out_2_1_i = in_0_1;
        2'd1: out_2_1_i = in_1_1;
        2'd2: out_2_1_i = in_3_1;
        2'd3: out_2_1_i = pe_output_0;
        default: out_2_1_i = 0;
    endcase
  end
  reg [0:0] out_2_1_id1;
  wire out_2_1_le;
  assign out_2_1_le = clk_en | (config_ungate[11]); 
  always @(posedge clk) begin
    if (out_2_1_le)
      out_2_1_id1 <= out_2_1_i;
  end
  assign out_2_1 = config_sb[51]?out_2_1_id1:out_2_1_i; 
  reg [0:0] out_2_2_i;
  always @(*) begin
    case (config_sb[25:24])
        2'd0: out_2_2_i = in_0_2;
        2'd1: out_2_2_i = in_1_2;
        2'd2: out_2_2_i = in_3_2;
        2'd3: out_2_2_i = pe_output_0;
        default: out_2_2_i = 0;
    endcase
  end
  reg [0:0] out_2_2_id1;
  wire out_2_2_le;
  assign out_2_2_le = clk_en | (config_ungate[12]); 
  always @(posedge clk) begin
    if (out_2_2_le)
      out_2_2_id1 <= out_2_2_i;
  end
  assign out_2_2 = config_sb[52]?out_2_2_id1:out_2_2_i; 
  reg [0:0] out_2_3_i;
  always @(*) begin
    case (config_sb[27:26])
        2'd0: out_2_3_i = in_0_3;
        2'd1: out_2_3_i = in_1_3;
        2'd2: out_2_3_i = in_3_3;
        2'd3: out_2_3_i = pe_output_0;
        default: out_2_3_i = 0;
    endcase
  end
  reg [0:0] out_2_3_id1;
  wire out_2_3_le;
  assign out_2_3_le = clk_en | (config_ungate[13]); 
  always @(posedge clk) begin
    if (out_2_3_le)
      out_2_3_id1 <= out_2_3_i;
  end
  assign out_2_3 = config_sb[53]?out_2_3_id1:out_2_3_i; 
  reg [0:0] out_2_4_i;
  always @(*) begin
    case (config_sb[29:28])
        2'd0: out_2_4_i = in_0_4;
        2'd1: out_2_4_i = in_1_4;
        2'd2: out_2_4_i = in_3_4;
        2'd3: out_2_4_i = pe_output_0;
        default: out_2_4_i = 0;
    endcase
  end
  reg [0:0] out_2_4_id1;
  wire out_2_4_le;
  assign out_2_4_le = clk_en | (config_ungate[14]); 
  always @(posedge clk) begin
    if (out_2_4_le)
      out_2_4_id1 <= out_2_4_i;
  end
  assign out_2_4 = config_sb[54]?out_2_4_id1:out_2_4_i; 
  reg [0:0] out_3_0_i;
  always @(*) begin
    case (config_sb[31:30])
        2'd0: out_3_0_i = in_0_0;
        2'd1: out_3_0_i = in_1_0;
        2'd2: out_3_0_i = in_2_0;
        2'd3: out_3_0_i = pe_output_0;
        default: out_3_0_i = 0;
    endcase
  end
  reg [0:0] out_3_0_id1;
  wire out_3_0_le;
  assign out_3_0_le = clk_en | (config_ungate[15]); 
  always @(posedge clk) begin
    if (out_3_0_le)
      out_3_0_id1 <= out_3_0_i;
  end
  assign out_3_0 = config_sb[55]?out_3_0_id1:out_3_0_i; 
  reg [0:0] out_3_1_i;
  always @(*) begin
    case (config_sb[33:32])
        2'd0: out_3_1_i = in_0_1;
        2'd1: out_3_1_i = in_1_1;
        2'd2: out_3_1_i = in_2_1;
        2'd3: out_3_1_i = pe_output_0;
        default: out_3_1_i = 0;
    endcase
  end
  reg [0:0] out_3_1_id1;
  wire out_3_1_le;
  assign out_3_1_le = clk_en | (config_ungate[16]); 
  always @(posedge clk) begin
    if (out_3_1_le)
      out_3_1_id1 <= out_3_1_i;
  end
  assign out_3_1 = config_sb[56]?out_3_1_id1:out_3_1_i; 
  reg [0:0] out_3_2_i;
  always @(*) begin
    case (config_sb[35:34])
        2'd0: out_3_2_i = in_0_2;
        2'd1: out_3_2_i = in_1_2;
        2'd2: out_3_2_i = in_2_2;
        2'd3: out_3_2_i = pe_output_0;
        default: out_3_2_i = 0;
    endcase
  end
  reg [0:0] out_3_2_id1;
  wire out_3_2_le;
  assign out_3_2_le = clk_en | (config_ungate[17]); 
  always @(posedge clk) begin
    if (out_3_2_le)
      out_3_2_id1 <= out_3_2_i;
  end
  assign out_3_2 = config_sb[57]?out_3_2_id1:out_3_2_i; 
  reg [0:0] out_3_3_i;
  always @(*) begin
    case (config_sb[37:36])
        2'd0: out_3_3_i = in_0_3;
        2'd1: out_3_3_i = in_1_3;
        2'd2: out_3_3_i = in_2_3;
        2'd3: out_3_3_i = pe_output_0;
        default: out_3_3_i = 0;
    endcase
  end
  reg [0:0] out_3_3_id1;
  wire out_3_3_le;
  assign out_3_3_le = clk_en | (config_ungate[18]); 
  always @(posedge clk) begin
    if (out_3_3_le)
      out_3_3_id1 <= out_3_3_i;
  end
  assign out_3_3 = config_sb[58]?out_3_3_id1:out_3_3_i; 
  reg [0:0] out_3_4_i;
  always @(*) begin
    case (config_sb[39:38])
        2'd0: out_3_4_i = in_0_4;
        2'd1: out_3_4_i = in_1_4;
        2'd2: out_3_4_i = in_2_4;
        2'd3: out_3_4_i = pe_output_0;
        default: out_3_4_i = 0;
    endcase
  end
  reg [0:0] out_3_4_id1;
  wire out_3_4_le;
  assign out_3_4_le = clk_en | (config_ungate[19]); 
  always @(posedge clk) begin
    if (out_3_4_le)
      out_3_4_id1 <= out_3_4_i;
  end
  assign out_3_4 = config_sb[59]?out_3_4_id1:out_3_4_i; 


  always @ (*) begin
    case (config_addr[31:24])    
         8'd0: read_data = config_sb[31:0];
         8'd1: read_data = config_sb[63:32];
         8'd2: read_data = config_ungate[31:0];
         8'd3 : read_data = out_0_0_id1;
         8'd4 : read_data = out_0_1_id1;
         8'd5 : read_data = out_0_2_id1;
         8'd6 : read_data = out_0_3_id1;
         8'd7 : read_data = out_0_4_id1;
         8'd8 : read_data = out_1_0_id1;
         8'd9 : read_data = out_1_1_id1;
         8'd10 : read_data = out_1_2_id1;
         8'd11 : read_data = out_1_3_id1;
         8'd12 : read_data = out_1_4_id1;
         8'd13 : read_data = out_2_0_id1;
         8'd14 : read_data = out_2_1_id1;
         8'd15 : read_data = out_2_2_id1;
         8'd16 : read_data = out_2_3_id1;
         8'd17 : read_data = out_2_4_id1;
         8'd18 : read_data = out_3_0_id1;
         8'd19 : read_data = out_3_1_id1;
         8'd20 : read_data = out_3_2_id1;
         8'd21 : read_data = out_3_3_id1;
         8'd22 : read_data = out_3_4_id1;
        default : read_data = 'h0;
    endcase
  end
endmodule

