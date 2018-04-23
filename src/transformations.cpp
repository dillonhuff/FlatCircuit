#include "transformations.h"

using namespace std;

namespace FlatCircuit {

  maybe<BitVector> getOutput(Cell& nextCell,
                             CellDefinition& def) {
    assert(false);
    return {};
  }

  maybe<BitVector> getInput(Cell& cell,
                            const PortId pid,
                            CellDefinition& def) {
    BitVector bv(cell.getPortWidth(pid), 0);

    auto& drivers = cell.getDrivers(pid);
    for (int offset = 0; offset < drivers.signals.size(); offset++) {
      SignalBit driver = drivers.signals.at(offset);
      if (notEmpty(driver)) {
        CellType driverTp = def.getCellRef(driver.cell).getCellType();
        if (driverTp != CELL_TYPE_CONST) {
          return {};
        }

        assert(driver.port == PORT_ID_OUT);

        Cell& driverCell = def.getCellRef(driver.cell);
        BitVector driverValue = driverCell.getParameterValue(PARAM_INIT_VALUE);

        bv.set(offset, driverValue.get(offset));
      } else {
        return {};
      }
    }

    return bv;
  }

  void foldConstants(CellDefinition& def) {
    set<CellId> candidates;
    for (auto cellPair : def.getCellMap()) {
      if (cellPair.second.getCellType() == CELL_TYPE_CONST) {
        cout << "Found const cell" << endl;
        for (auto sigBus : cellPair.second.getPortReceivers(PORT_ID_OUT)) {
          for (auto sigBit : sigBus) {
            candidates.insert(sigBit.cell);
          }
        }
      }
    }

    cout << "# of candidates = " << candidates.size() << endl;

    while (candidates.size() > 0) {
      CellId next = *std::begin(candidates);
      candidates.erase(next);

      Cell& nextCell = def.getCellRef(next);

      if (nextCell.getCellType() == CELL_TYPE_MUX) {
        maybe<BitVector> bv = getInput(nextCell, PORT_ID_SEL, def);

        if (bv.has_value()) {
          // TODO: Add rewiring for muxes with constant value selects
          assert(false);
        }
      } else {
        maybe<BitVector> bv = getOutput(nextCell, def);

        if (bv.has_value()) {
          for (auto sigBus : nextCell.getPortReceivers(PORT_ID_OUT)) {
            for (auto sigBit : sigBus) {
              candidates.insert(sigBit.cell);
            }
          }

          def.replaceCellPortWithConstant(next, PORT_ID_OUT, bv.get_value());
        
        }
      }
    }

    assert(candidates.size() == 0);
  }

}
