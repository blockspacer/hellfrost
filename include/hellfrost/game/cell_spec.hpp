#ifndef __CELL_SPEC_H_
#define __CELL_SPEC_H_
#include <lss/deps.hpp>

struct CellSpec {
  friend class cereal::access;
  template <class Archive> void save(Archive &ar) const {
    ar(name, seeThrough, passThrough);
  };
  template <class Archive> void load(Archive &ar) {
    ar(name, seeThrough, passThrough);
  };
  std::string name;

  bool passThrough = false;
  bool seeThrough = false;

  friend bool operator==(CellSpec &t1, const CellSpec &t2) {
    return t1.name == t2.name;
  }
  friend bool operator!=(CellSpec &t1, const CellSpec &t2) {
    return t1.name != t2.name;
  }
  friend bool operator<(const CellSpec &t1, const CellSpec &t2) {
    return t1.name < t2.name;
  }
};

namespace CellType {
const CellSpec EMPTY = CellSpec{"empty", true, true};
const CellSpec UNKNOWN = CellSpec{"unknown", false, false};
const CellSpec FLOOR = CellSpec{"floor", true, true};
const CellSpec GROUND = CellSpec{"ground", true, true};
const CellSpec WALL = CellSpec{"wall", false, false};
const CellSpec ROOF = CellSpec{"roof", true, false};
const CellSpec DOWNSTAIRS = CellSpec{"downstairs", true, true};
const CellSpec UPSTAIRS = CellSpec{"upstairs", true, false};
const CellSpec WATER = CellSpec{"water", false, true};
const CellSpec VOID = CellSpec{"void", false, true};
}; // namespace CellType

#endif // __CELL_SPEC_H_