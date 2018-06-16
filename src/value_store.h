#pragma once

#include <map>

#include "circuit.h"
#include "cpp_utils.h"

namespace FlatCircuit {

  class IRInstruction;

  class ValueStore {
    std::vector<BitVector> simValueTable;

    std::map<SigPort, unsigned long> portOffsets;
    std::map<CellId, unsigned long> memoryOffsets;
    std::map<CellId, unsigned long> registerOffsets;    
    std::map<SigPort, unsigned long> pastValueOffsets;

    // Raw (2 state) table entries
    unsigned char* rawSimValueTable;
    unsigned long rawTableSize;

    std::map<SigPort, unsigned long> rawPortOffsets;
    std::map<CellId, unsigned long> rawMemoryOffsets;
    std::map<CellId, unsigned long> rawRegisterOffsets;
    std::map<SigPort, unsigned long> rawPastValueOffsets;

    bool compiledRaw;
    
  public:
    CellDefinition& def;

    ValueStore(CellDefinition& def_);

    unsigned char debugGetRawTable(const int index) {
      assert(compiledRaw);
      return rawSimValueTable[index];
    }

    void setCompiledRaw() {
      compiledRaw = true;

      // Copy over register, memory, and constants to the raw buffer
      for (auto ctp : def.getCellMap()) {
        CellId cid = ctp.first;
        const Cell& cell = def.getCellRefConst(cid);
        if (cell.getCellType() == CELL_TYPE_CONST) {
          setPortValue(cid,
                       PORT_ID_OUT,
                       simValueTable[map_find({cid, PORT_ID_OUT}, portOffsets)]);
        } else if (isRegister(cell.getCellType())) {
          // TODO: Need to add register support
          setRegisterValue(cid,
                           simValueTable[map_find({cid, PORT_ID_OUT}, portOffsets)]);

          setPortValue(cid,
                       PORT_ID_OUT,
                       simValueTable[map_find({cid, PORT_ID_OUT}, portOffsets)]);
        }
      }

      ///      rawSimValueTable[44] = 0xff;
    }

    void buildRawValueTable() {
      unsigned long rawOffset = 0;
      std::map<unsigned long, unsigned long> quadOffsetsToRawOffsets;
      for (unsigned long i = 0; i < simValueTable.size(); i++) {
        quadOffsetsToRawOffsets[i] = rawOffset;
        // Adding extra buffer space
        rawOffset += storedByteLength(simValueTable[i].bitLength()) + 1;
      }

      rawTableSize = rawOffset;

      // std::cout << "Raw table offset map" << std::endl;
      // for (auto ent : quadOffsetsToRawOffsets) {
      //   std::cout << "\t" << ent.first << " --> " << ent.second << std::endl;
      // }

      for (auto sp : portOffsets) {
        rawPortOffsets[sp.first] = map_find(sp.second, quadOffsetsToRawOffsets);
        // std::cout << "Offset for port = " <<
        //   rawMemoryOffsets[sp.first] << std::endl;
      }

      for (auto sp : pastValueOffsets) {
        rawPastValueOffsets[sp.first] = map_find(sp.second, quadOffsetsToRawOffsets);
      }

      for (auto sp : memoryOffsets) {
        rawMemoryOffsets[sp.first] = map_find(sp.second, quadOffsetsToRawOffsets);
        // std::cout << "Offset for memory = " <<
        //   rawMemoryOffsets[sp.first] << std::endl;
      }

      for (auto sp : registerOffsets) {
        rawRegisterOffsets[sp.first] = map_find(sp.second, quadOffsetsToRawOffsets);
      }

      rawSimValueTable =
        static_cast<unsigned char*>(malloc(rawOffset));
      memset(rawSimValueTable, 0, rawTableSize);
      
    }

    void debugPrintTableValues() const;

    std::vector<BitVector>& getValueTable() { return simValueTable; }
    unsigned char* getRawValueTable() { return rawSimValueTable; }

    unsigned long getMemoryOffset(const CellId cid) const {
      return map_find(cid, memoryOffsets);
    }

    unsigned long getRawMemoryOffset(const CellId cid) const {
      return map_find(cid, rawMemoryOffsets);
    }
    
    unsigned long getRegisterOffset(const CellId cid) const {
      return map_find(cid, registerOffsets);
    }

    unsigned long getRawRegisterOffset(const CellId cid) const {
      return map_find(cid, rawRegisterOffsets);
    }
    
    unsigned long rawPortValueOffset(const CellId cid,
                                     const PortId pid) {
      assert(contains_key({cid, pid}, portOffsets));
      assert(contains_key({cid, pid}, rawPortOffsets));

      return map_find({cid, pid}, rawPortOffsets);
    }

    unsigned long rawPortPastValueOffset(const CellId cid,
                                         const PortId pid) {
      assert(contains_key({cid, pid}, pastValueOffsets));
      assert(contains_key({cid, pid}, rawPastValueOffsets));

      return map_find({cid, pid}, rawPastValueOffsets);
    }
    
    unsigned long portValueOffset(const CellId cid,
                                  const PortId pid) {
      if (!contains_key({cid, pid}, portOffsets)) {
        std::cout << "Error: " << sigPortString(def, {cid, pid}) << " has no offset" << std::endl;
        assert(false);
        // unsigned long nextInd = simValueTable.size();
        // simValueTable.push_back(BitVector(1, 0));
        // portOffsets[{cid, pid}] = nextInd;

        // return nextInd;
      }

      return map_find({cid, pid}, portOffsets);
    }

    void initMemory(const CellId cid) {
      assert(!contains_key(cid, memoryOffsets));

      const Cell& cl = def.getCellRefConst(cid);
      int memWidth = cl.getMemWidth();
      int memDepth = cl.getMemDepth();

      BitVector defaultValue(memWidth, 0);

      unsigned long nextInd = simValueTable.size();
      memoryOffsets[cid] = nextInd;
      for (unsigned long i = 0; i < (unsigned long) memDepth; i++) {
        simValueTable.push_back(defaultValue);
      }
    }

    BitVector getMemoryValue(const CellId cid,
                             const int offset) const {
      assert(offset >= 0);

      return simValueTable[map_find(cid, memoryOffsets) + ((unsigned long) offset)];
    }

    void setMemoryValue(const CellId cid,
                        const int addr,
                        const BitVector& writeData) {

      assert(contains_key(cid, memoryOffsets));
      assert(addr >= 0);

      simValueTable[map_find(cid, memoryOffsets) + ((unsigned long) addr)] =
        writeData;

    }

    void debugPrintRawValueTable() const;

    void setPortValueTwoState(const CellId cid,
                              const PortId pid,
                              const int width,
                              const unsigned long value) {
      assert(compiledRaw);

      int pWidth = def.getCellRefConst(cid).getPortWidth(pid);

      assert(pWidth == width);

      if (width <= 8) {
        *((uint8_t*) (rawSimValueTable + map_find({cid, pid}, rawPortOffsets))) =
          (uint8_t) value;
      } else if (width <= 16) {
        *((uint16_t*) (rawSimValueTable + map_find({cid, pid}, rawPortOffsets))) =
          (uint16_t) value;
      } else if (width <= 32) {
        *((uint32_t*) (rawSimValueTable + map_find({cid, pid}, rawPortOffsets))) =
          (uint32_t) value;
      } else if (width <= 64) {
        *((uint64_t*) (rawSimValueTable + map_find({cid, pid}, rawPortOffsets))) =
          (uint64_t) value;
      } else {
        assert(false);
      }
      
    }

    void resetPortValue(const CellId cid,
                      const PortId pid,
                      const BitVector& bv) {
      if (!contains_key({cid, pid}, portOffsets)) {
        unsigned long nextInd = simValueTable.size();
        portOffsets[{cid, pid}] = nextInd;
        simValueTable.push_back(bv);
      } else {
        simValueTable[map_find({cid, pid}, portOffsets)] = bv;
      }
    }

    void resetPastValue(const CellId cid,
                        const PortId pid,
                        const BitVector& bv) {
      if (!contains_key({cid, pid}, pastValueOffsets)) {
        unsigned long nextInd = simValueTable.size();
        pastValueOffsets[{cid, pid}] = nextInd;
        simValueTable.push_back(bv);
      } else {
        simValueTable[map_find({cid, pid}, pastValueOffsets)] = bv;
      }
    }
    
    void setPortValue(const CellId cid,
                      const PortId pid,
                      const BitVector& bv) {

      if (compiledRaw) {
        //        std::cout << "Setting raw offset " << map_find({cid, pid}, rawPortOffsets) << " with bit vector " << bv << " with bit length " << bv.bitLength() << std::endl;
        int pWidth = def.getCellRefConst(cid).getPortWidth(pid);

        assert(pWidth <= 64);

        bool isBinary = bv.is_binary();

        if (isBinary) {
          if (bv.bitLength() <= 8) {
            *((uint8_t*) (rawSimValueTable + map_find({cid, pid}, rawPortOffsets))) =
              (uint8_t) bv.to_type<uint8_t>();
          } else if (bv.bitLength() <= 16) {
            //          std::cout << "Setting " << sigPortString(def, {cid, pid}) << " to " << bv.to_type<uint16_t>() << std::endl;
            *((uint16_t*) (rawSimValueTable + map_find({cid, pid}, rawPortOffsets))) =
              (uint16_t) bv.to_type<uint16_t>();

            //debugPrintRawValueTable();
          } else if (bv.bitLength() <= 32) {
            *((uint32_t*) (rawSimValueTable + map_find({cid, pid}, rawPortOffsets))) =
              (uint32_t) bv.to_type<uint32_t>();
          } else if (bv.bitLength() <= 64) {
            *((uint64_t*) (rawSimValueTable + map_find({cid, pid}, rawPortOffsets))) =
              (uint64_t) bv.to_type<uint64_t>();
          } else {
            assert(false);
          }
        } else {
          if (bv.bitLength() <= 8) {
            *((uint8_t*) (rawSimValueTable + map_find({cid, pid}, rawPortOffsets))) =
              (uint8_t) 0;
          } else if (bv.bitLength() <= 16) {
            //          std::cout << "Setting " << sigPortString(def, {cid, pid}) << " to " << bv.to_type<uint16_t>() << std::endl;
            *((uint16_t*) (rawSimValueTable + map_find({cid, pid}, rawPortOffsets))) =
              (uint16_t) 0;
            //debugPrintRawValueTable();
          } else if (bv.bitLength() <= 32) {
            *((uint32_t*) (rawSimValueTable + map_find({cid, pid}, rawPortOffsets))) =
              (uint32_t) 0;
          } else if (bv.bitLength() <= 64) {
            *((uint64_t*) (rawSimValueTable + map_find({cid, pid}, rawPortOffsets))) =
              (uint64_t) 0;
          } else {
            assert(false);
          }
          
        }
      } else {

        if (!contains_key({cid, pid}, portOffsets)) {
          std::cout << "Error: " << sigPortString(def, {cid, pid}) << " has no offset" << std::endl;
          assert(false);
        }

        simValueTable[map_find({cid, pid}, portOffsets)] = bv;
      }
    }

    BitVector getPortValue(const CellId cid,
                           const PortId pid) const {
      if (!compiledRaw) {
        return simValueTable[map_find({cid, pid}, portOffsets)];
      } else {
        int pWidth = def.getCellRefConst(cid).getPortWidth(pid);

        assert(pWidth <= 64);

        if (pWidth <= 8) {
          return BitVector(pWidth,
                           *((uint8_t*)
                             (rawSimValueTable +
                              map_find({cid, pid}, rawPortOffsets))));
        } else if (pWidth <= 16) {
          return BitVector(pWidth,
                           *((uint16_t*)
                             (rawSimValueTable +
                              map_find({cid, pid}, rawPortOffsets))));

        } else if (pWidth <= 32) {
          return BitVector(pWidth,
                           *((uint32_t*)
                             (rawSimValueTable +
                              map_find({cid, pid}, rawPortOffsets))));

        } else if (pWidth <= 64) {
          return BitVector(pWidth,
                           *((uint64_t*)
                             (rawSimValueTable +
                              map_find({cid, pid}, rawPortOffsets))));
        } else {
          assert(false);
        }
      }
    }

    void setRegisterValue(const CellId cid,
                          const BitVector& bv) {
      if (!compiledRaw) {
        assert(contains_key(cid, registerOffsets));
        // if (!contains_key(cid, registerOffsets)) {
        //   unsigned long nextInd = simValueTable.size();
        //   registerOffsets[cid] = nextInd;
        //   simValueTable.push_back(bv);
        // }

        simValueTable[map_find(cid, registerOffsets)] = bv;

      } else {

        bool isBinary = bv.is_binary();
        
        int pWidth =
          bvToInt(def.getCellRefConst(cid).getParameterValue(PARAM_WIDTH));

        assert(pWidth <= 64);

        if (isBinary) {
          if (bv.bitLength() <= 8) {
            *((uint8_t*) (rawSimValueTable + map_find(cid, rawRegisterOffsets))) =
              (uint8_t) bv.to_type<uint8_t>();
          } else if (bv.bitLength() <= 16) {
            *((uint16_t*) (rawSimValueTable + map_find(cid, rawRegisterOffsets))) =
              (uint16_t) bv.to_type<uint16_t>();
          } else if (bv.bitLength() <= 32) {
            *((uint32_t*) (rawSimValueTable + map_find(cid, rawRegisterOffsets))) =
              (uint32_t) bv.to_type<uint32_t>();
          } else if (bv.bitLength() <= 64) {
            *((uint64_t*) (rawSimValueTable + map_find(cid, rawRegisterOffsets))) =
              (uint64_t) bv.to_type<uint64_t>();
          } else {
            assert(false);
          }
        } else {
          if (bv.bitLength() <= 8) {
            *((uint8_t*) (rawSimValueTable + map_find(cid, rawRegisterOffsets))) =
              (uint8_t) 0;
          } else if (bv.bitLength() <= 16) {
            *((uint16_t*) (rawSimValueTable + map_find(cid, rawRegisterOffsets))) =
              (uint16_t) 0;
          } else if (bv.bitLength() <= 32) {
            *((uint32_t*) (rawSimValueTable + map_find(cid, rawRegisterOffsets))) =
              (uint32_t) 0;
          } else if (bv.bitLength() <= 64) {
            *((uint64_t*) (rawSimValueTable + map_find(cid, rawRegisterOffsets))) =
              (uint64_t) 0;
          } else {
            assert(false);
          }
        }
      }
    }

    BitVector getRegisterValue(const CellId cid) const {
      return simValueTable[map_find(cid, registerOffsets)];
    }

    void setPastValue(const CellId cid,
                      const PortId pid,
                      const BitVector& bv) {
      if (!compiledRaw) {
        unsigned long offset = pastValueOffset(cid, pid);
        simValueTable[offset] = bv;
      } else {
        int pWidth = def.getCellRefConst(cid).getPortWidth(pid);

        assert(pWidth <= 64);

        if (bv.bitLength() <= 8) {
          *((uint8_t*) (rawSimValueTable + map_find({cid, pid}, rawPastValueOffsets))) =
            (uint8_t) bv.to_type<uint8_t>();
        } else if (bv.bitLength() <= 16) {
          //          std::cout << "Setting " << sigPortString(def, {cid, pid}) << " to " << bv.to_type<uint16_t>() << std::endl;
          *((uint16_t*) (rawSimValueTable + map_find({cid, pid}, rawPastValueOffsets))) =
            (uint16_t) bv.to_type<uint16_t>();

          //debugPrintRawValueTable();
        } else if (bv.bitLength() <= 32) {
          *((uint32_t*) (rawSimValueTable + map_find({cid, pid}, rawPastValueOffsets))) =
            (uint32_t) bv.to_type<uint32_t>();
        } else if (bv.bitLength() <= 64) {
          *((uint64_t*) (rawSimValueTable + map_find({cid, pid}, rawPastValueOffsets))) =
            (uint64_t) bv.to_type<uint64_t>();
        } else {
          assert(false);
        }


      }
    }

    BitVector getPastValue(const CellId cid,
                           const PortId pid) {
      return simValueTable[pastValueOffset(cid, pid)];
    }

    unsigned long pastValueOffset(const CellId cid,
                                  const PortId pid) {
      const Cell& cell = def.getCellRefConst(cid);

      if (cell.getPortType(pid) == PORT_TYPE_OUT) {

        if (!contains_key({cid, pid}, pastValueOffsets)) {
          unsigned long nextInd = simValueTable.size();
          simValueTable.push_back(BitVector(1, 0));
          
          pastValueOffsets[{cid, pid}] = nextInd;

          return nextInd;
        }

        return map_find({cid, pid}, pastValueOffsets);

      } else {
        assert(cell.getPortType(pid) == PORT_TYPE_IN);
        
        auto drivers = cell.getDrivers(pid);
        assert(drivers.signals.size() == 1);

        SignalBit driverBit = drivers.signals[0];

        assert(notEmpty(driverBit));
        assert(driverBit.offset == 0);

        return pastValueOffset(driverBit.cell, driverBit.port);
      }
    }

    BitVector materializeInput(const SigPort sigPort) const {
      int width = def.getCellRefConst(sigPort.cell).getPortWidth(sigPort.port);

      BitVector val(width, 0);

      auto& sigBus = def.getCellRef(sigPort.cell).getDrivers(sigPort.port);

      assert(((int) sigBus.signals.size()) == width);

      for (int i = 0; i < (int) sigBus.signals.size(); i++) {
        SignalBit b = sigBus.signals.at(i);

        assert(notEmpty(b));

        val.set(i, getPortValue(b.cell, b.port).get(b.offset));
      }
        
      return val;
    }
    
    IRInstruction* codeToAssignRegister(const CellId cid,
                                        const std::string& assignCode);
    
    std::vector<IRInstruction*>
    codeToMaterializeOffset(const CellId cid,
                            const PortId pid,
                            const std::string& argName,
                            const std::map<SigPort, unsigned long>& offsets,
                            const bool isPast) const;

    std::vector<IRInstruction*>
    codeToMaterialize(const CellId cid,
                      const PortId pid,
                      const std::string& argName) const;

    std::vector<IRInstruction*>
    codeToMaterializePastValue(const CellId cid,
                               const PortId pid,
                               const std::string& argName) const {
      return codeToMaterializeOffset(cid, pid, argName, pastValueOffsets, true);
    }

    ~ValueStore() {
      if (compiledRaw) {
        free(rawSimValueTable);
      }
    }
    
  };


}
