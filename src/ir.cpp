#include "ir.h"

using namespace std;

namespace FlatCircuit {

  std::string IRBinop::twoStateCppCode(ValueStore& valueStore) const {

    CellType tp = cell.getCellType();
    switch (tp) {
    case CELL_TYPE_MUL:
      return ln(receiver + " = (" + arg0 + " * " + arg1 + ")");

    case CELL_TYPE_ADD:
      return ln(receiver + " = (" + arg0 + " + " + arg1 + ")");

    case CELL_TYPE_SUB:
      return ln(receiver + " = (" + arg0 + " - " + arg1 + ")");
      
    case CELL_TYPE_AND:
      return ln(receiver + " = (" + arg0 + " & " + arg1 + ")");

    case CELL_TYPE_OR:

      return ln(receiver + " = (" + arg0 + " | " + arg1 + ")");

    case CELL_TYPE_XOR:
      return ln(receiver + " = (" + arg0 + " ^ " + arg1 + ")");
      
    case CELL_TYPE_EQ:
      return ln(receiver + " = (" + arg0 + " == " + arg1 + ")");

    case CELL_TYPE_NEQ:
      return ln(receiver + " = (" + arg0 + " != " + arg1 + ")");

    case CELL_TYPE_UGE:
      return ln(receiver + " = (" + arg0 + " >= " + arg1 + ")");

    case CELL_TYPE_ULE:
      return ln(receiver + " = (" + arg0 + " <= " + arg1 + ")");

    case CELL_TYPE_ULT:
      return ln(receiver + " = (" + arg0 + " < " + arg1 + ")");

    case CELL_TYPE_UGT:
      return ln(receiver + " = (" + arg0 + " > " + arg1 + ")");
      
    default:
      std::cout << "Error: Unsupported binop " << FlatCircuit::toString(tp) << std::endl;
      assert(false);
      //      return "// Binop " + receiver + "\n";
    }

  }
  
  std::string IRBinop::toString(ValueStore& valueStore) const {
    CellType tp = cell.getCellType();
    switch (tp) {
    case CELL_TYPE_AND:
      return ln("static_bv_and(" + receiver + ", " + arg0 + ", " + arg1 + ")");

      //return ln(receiver + " = (" + arg0 + " & " + arg1 + ")");

    case CELL_TYPE_UGE:
      return ln(receiver + " = BitVector((" +
                arg0 + " > " + arg1 + ") || (" +
                arg0 + " == " + arg1 + "))");

    case CELL_TYPE_ULE:
      return ln(receiver + " = BitVector((" +
                arg0 + " < " + arg1 + ") || (" +
                arg0 + " == " + arg1 + "))");

    case CELL_TYPE_UGT:
      return ln(receiver + " = BitVector((" + arg0 + " > " + arg1 + "))");

    case CELL_TYPE_ULT:
      return ln(receiver + " = BitVector((" + arg0 + " < " + arg1 + "))");
        
    case CELL_TYPE_OR:
      return ln("static_bv_or(" + receiver + ", " + arg0 + ", " + arg1 + ")");
      //return ln(receiver + " = (" + arg0 + " | " + arg1 + ")");

    case CELL_TYPE_XOR:
      return ln(receiver + " = (" + arg0 + " ^ " + arg1 + ")");

    case CELL_TYPE_ADD:
      //return ln(receiver + " = add_general_width_bv(" + arg0 + ", " + arg1 + ")");
      return ln("add_general_width_bv(" + receiver + ", " + arg0 + ", " + arg1 + ")");

    case CELL_TYPE_MUL:
      return ln(receiver + " = mul_general_width_bv(" + arg0 + ", " + arg1 + ")");

    case CELL_TYPE_SUB:
      //return ln(receiver + " = sub_general_width_bv(" + arg0 + ", " + arg1 + ")");
      return ln("sub_general_width_bv(" + receiver + ", " + arg0 + ", " + arg1 + ")");

    case CELL_TYPE_LSHR:
      return ln(receiver + " = lshr(" + arg0 + ", " + arg1 + ")");
        
    case CELL_TYPE_ASHR:
      return ln(receiver + " = ashr(" + arg0 + ", " + arg1 + ")");
        
    case CELL_TYPE_SHL:
      return ln(receiver + " = shl(" + arg0 + ", " + arg1 + ")");
        
    case CELL_TYPE_EQ:
      //return ln(receiver + " = BitVector(" + arg0 + " == " + arg1 + ")");
      return ln("equals(" + receiver + ", " + arg0 + ", " + arg1 + ")");

    case CELL_TYPE_NEQ:
      return ln(receiver + " = BitVector(" + arg0 + " != " + arg1 + ")");
        
    default:
      std::cout << "Error: Unsupported binop " << FlatCircuit::toString(tp) << std::endl;
      assert(false);
    }

  }

  std::string bitMask(const int width) {
    assert(width > 0);

    std::string ones = "";
    for (int i = 0; i < width; i++) {
      ones += "1";
    }
    return "0b" + ones;
  }
  std::string IRUnop::twoStateCppCode(ValueStore& valueStore) const {
    CellType unop = cell.getCellType();

    switch (unop) {

    case CELL_TYPE_PASSTHROUGH:
      return ln(receiver + " = " + arg);

    case CELL_TYPE_NOT:
      return ln(receiver + " = (~(" + arg + ") & " + bitMask(cell.getPortWidth(PORT_ID_OUT)) + ")");

    case CELL_TYPE_ORR:
      return ln(receiver + " = !!(" + arg + ")");

    // case CELL_TYPE_SLICE:
    //   return ln(receiver + " = slice(" + arg + ", " +
    //             to_string(bvToInt(cell.getParameterValue(PARAM_LOW))) + ", " +
    //             to_string(bvToInt(cell.getParameterValue(PARAM_HIGH))) + ")");
        
    case CELL_TYPE_ZEXT:
      return ln(receiver + " = " + arg);

      // return ln(receiver + " = zero_extend(" +
      //           to_string(cell.getPortWidth(PORT_ID_OUT)) + 
      //           ", " + arg + ")");
    default:
      //      return ln("// Unop");
      std::cout << "IRUnop error: " << FlatCircuit::toString(unop) << std::endl;
      assert(false);
    }

    //    return "// Unop " + receiver + "\n";
  }
    
  std::string IRUnop::toString(ValueStore& valueStore) const {
    CellType unop = cell.getCellType();

    switch (unop) {

    case CELL_TYPE_PASSTHROUGH:
      return ln(receiver + " = " + arg);

    case CELL_TYPE_NOT:
      //return ln(receiver + " = ~(" + arg + ")");
      return ln("logical_not(" + receiver + ", " + arg + ")");

    case CELL_TYPE_ORR:
      return ln("orr(" + receiver + ", " + arg + ")");

    case CELL_TYPE_SLICE:
      return ln(receiver + " = slice(" + arg + ", " +
                to_string(bvToInt(cell.getParameterValue(PARAM_LOW))) + ", " +
                to_string(bvToInt(cell.getParameterValue(PARAM_HIGH))) + ")");
        
    case CELL_TYPE_ZEXT:
      return ln("zero_extend(" + receiver + ", " + arg + ")");

      // return ln(receiver + " = zero_extend<" + to_string(cell.getPortWidth(PORT_ID_IN)) + ", " + to_string(cell.getPortWidth(PORT_ID_OUT)) + ">(" +
      //           to_string(cell.getPortWidth(PORT_ID_OUT)) + 
      //           ", " + arg + ")");
    default:
      std::cout << "IRUnop error: " << FlatCircuit::toString(unop) << std::endl;
      assert(false);
    }
  }
  
}
