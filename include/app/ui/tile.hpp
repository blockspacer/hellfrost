#ifndef __TILE_H_
#define __TILE_H_

#include <algorithm>
#include <utility>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <json.hpp>
#include <SFML/Graphics.hpp>
#include <librandom/random.hpp>
#include <libcolor/libcolor.hpp>

#include <lss/game/cell.hpp>
#include <lss/game/region.hpp>
#include <lss/game/terrain.hpp>
#include <lss/game/door.hpp>
#include <lss/utils.hpp>

using json = nlohmann::json;
using Color = LibColor::Color;

struct TileSet {
  std::vector<std::string> maps;
  std::pair<int, int> size;
  int gap;
  std::map<std::string, std::array<int, 3>> sprites;
};

class Tile {
public:
  // std::pair<int, int> spriteIndex;
  std::vector<std::shared_ptr<sf::Sprite>> sprites;
  std::shared_ptr<Cell> cell;
  bool damaged = false;
  sf::Color bgColor = sf::Color(33, 33, 23);
  bool hasBackground = false;
  sf::Vector2f pos;
  int bgLayer = 1;
};

class Viewport {
  sf::IntRect getTileRect(int x, int y) {
    return sf::IntRect((tileSet.size.first + tileSet.gap) * x,
                       (tileSet.size.second + tileSet.gap) * y,
                       tileSet.size.first, tileSet.size.second);
  }


  std::shared_ptr<R::Generator> gen = std::make_shared<R::Generator>();

public:
  Viewport() {
    std::ifstream file("tiles.json");
    // std::ifstream file("tiles_lon.json");
    json tilesSpec;
    file >> tilesSpec;
    tileSet = TileSet{
      tilesSpec["TILEMAPS"],
      tilesSpec["SIZE"],
      tilesSpec["GAP"],
      tilesSpec["SPRITES"],
    };

    std::ifstream cfile("colors.json");
    cfile >> colors;
  }
  TileSet tileSet;
  json colors;
  std::map<std::string, std::shared_ptr<Tile>> tilesCache;

public:
  int width = 60;
  int height = 50;
  std::pair<int, int> position;
  int z = 0;
  std::vector<sf::Texture> tilesTextures;
  std::vector<std::shared_ptr<Region>> regions;
  std::pair<std::shared_ptr<Cell>, int> getCell(int x, int y, int z) {
    std::shared_ptr<Cell> cell(nullptr);
    for (auto region : regions) {
      if (region->active && region->z == z &&
          x >= region->position.first &&
          x < (int)(region->position.first + region->cells[0].size()) &&
          y >= region->position.second &&
          y < (int)(region->position.second + region->cells.size())) {
        cell = region->cells[y - region->position.second]
                            [x - region->position.first];
      }
    }
    if (z > 0 && cell == nullptr) {
      return getCell(x,y,z-1);
    }
    return std::make_pair(cell, z);
  }

  std::pair<std::shared_ptr<Region>, int> getRegion(int x, int y, int z) {
    std::shared_ptr<Region> reg(nullptr);
    for (auto region : regions) {
      if (region->active && region->z == z &&
          x >= region->position.first &&
          x < (int)(region->position.first + region->cells[0].size()) &&
          y >= region->position.second &&
          y < (int)(region->position.second + region->cells.size())) {
        return std::make_pair(region, z);
      }
    }
    if (z > 0 && reg == nullptr) {
      return getRegion(x,y,z-1);
    }
    return std::make_pair(reg, z);
  }

std::shared_ptr<sf::Sprite> makeSprite(std::string key) {
    auto spec = tileSet.sprites[key];
    auto s = std::make_shared<sf::Sprite>();
    s->setTexture(tilesTextures[spec[0]]);
    s->setTextureRect(getTileRect(spec[1], spec[2]));
    auto c = Color::fromHexString(colors["ENV"][key]);
    s->setColor(sf::Color(c.r, c.g, c.b));
    return s;
}


  std::shared_ptr<Tile> getTile(int x, int y, int z) {
    x += position.first;
    y += position.second;

    auto [cell, rz] = getCell(x, y, z);
    if (cell == nullptr) {
      cell = std::make_shared<Cell>(x, y, CellType::UNKNOWN);
    }

    auto coords = fmt::format("{}.{}.{}", x, y, rz);
    auto t = tilesCache.find(coords);
    // if (t != tilesCache.end() && !t->damaged) return t->second;
    if (t != tilesCache.end()) return t->second;

    auto tile = std::make_shared<Tile>();
    auto fgColor = sf::Color(220,220,220);
    auto sprite = std::make_shared<sf::Sprite>();
    tile->sprites.push_back(sprite);
    std::array<int, 3> sprite_spec = {0,0,0};
    if (cell->type == CellType::GROUND) {
      sprite_spec = tileSet.sprites["GRASS"];
      auto brown = Color(95,85,65);
      brown.g += gen->R(0, 60);
      fgColor = gen->R(std::vector<sf::Color>{sf::Color(120,150,60, gen->R(235, 255)), sf::Color(95,85,65, gen->R(235, 255))});
      fgColor = sf::Color(brown.r, brown.g, brown.b);
      // tile->bgColor = sf::Color(95,85,65, gen->R(20, 120));
    } else if (cell->type == CellType::UPSTAIRS) {
      sprite_spec = tileSet.sprites["UPSTAIRS"];
    } else if (cell->type == CellType::DOWNSTAIRS) {
      sprite_spec = tileSet.sprites["DOWNSTAIRS"];
    } else if (cell->type == CellType::FLOOR) {
      sprite_spec = tileSet.sprites["FLOOR"];
      fgColor = sf::Color(95,85,65);

      auto [region, _z] = getRegion(x, y, rz);
      auto doors = utils::castObjects<Door>(region->location->getObjects(cell));
      if (doors.size() != 0) {
        if (doors.front()->hidden) {
          sprite_spec = getWallSpec(cell);
          // auto c = Color::fromHexString(colors["ENV"]["WALL"]);
          // fgColor = sf::Color(c.r, c.g, c.b);
        } else {
          sprite_spec = tileSet.sprites["DOOR"];
          auto c = Color::fromHexString(colors["ENV"]["DOOR"]);
          fgColor = sf::Color(c.r, c.g, c.b);
        }
      } else {
        auto terrain = utils::castObjects<Terrain>(region->location->getObjects(cell));
        if (terrain.size() != 0) {
            auto key = terrain.front()->type.name;
            std::transform(key.begin(), key.end(), key.begin(), ::toupper);
            std::replace(key.begin(), key.end(), ' ', '_');
            tile->sprites.push_back(makeSprite(key));
        }
      }


      // if (gen->R(0, 100) < 5) {
      //   std::array<int, 3> ss = tileSet.sprites["ORK"];
      //   tile->sprites.push_back(s);
      //   if (gen->R(0, 100) < 25) {
      //     tile->bgColor = sf::Color(rand()%256, rand()%256, rand()%256, 60);
      //     tile->hasBackground = true;
      //   }
      // }
    } else if (cell->type == CellType::WALL) {
      sprite_spec = getWallSpec(cell);
      auto c = Color::fromHexString(colors["ENV"]["WALL"]);
      fgColor = sf::Color(c.r, c.g, c.b);
    } else if (cell->type == CellType::ROOF) {
      sprite_spec = {0, 1, 17};
    }

    sprite->setTexture(tilesTextures[sprite_spec[0]]);


    auto src = getTileRect(sprite_spec[1], sprite_spec[2]);
    sprite->setTextureRect(src);
    sprite->setColor(fgColor);

    // tile->bgColor = sf::Color(255, 255, 255, rand()%256);

    // if (cell->type != CellType::UNKNOWN) {
      for (auto f : cell->features) {
        if (f == CellFeature::BLOOD) {
            auto c = Color::fromHexString(colors["ENV"]["BLOOD"]);
            tile->bgColor = sf::Color(c.r, c.g, c.b, 80);
            tile->hasBackground = true;
            for (auto s : tile->sprites) {
              tile->bgLayer++;
            }
            tile->bgLayer--;
        }
        if (f == CellFeature::FROST) {
            auto c = Color::fromHexString(colors["ENV"]["FROST"]);
            tile->bgColor = sf::Color(c.r, c.g, c.b, 80);
            tile->hasBackground = true;
        }
        if (f == CellFeature::CORRUPT) {
            auto c = Color::fromHexString(colors["ENV"]["CORRUPT"]);
            tile->bgColor = sf::Color(c.r, c.g, c.b, 80);
            tile->hasBackground = true;
        }
      }
    // }

    auto scale = 0.8f;
    tile->pos = sf::Vector2f((cell->anchor.first + cell->x) * (tileSet.size.first),
          (cell->anchor.second + cell->y) * (tileSet.size.second));
    for (auto s : tile->sprites) {
      s->setPosition(tile->pos);
    }
    tile->cell = cell;
    // tile->spriteIndex = sprite_spec;
    tilesCache[coords] = tile;
    return tile;
  }

std::pair<int, int> getCoords(std::shared_ptr<Cell> cell) {
    return std::make_pair<int, int>(
      cell->anchor.first + cell->x,
      cell->anchor.second + cell->y
    );
}

std::array<int, 3> getWallSpec(std::shared_ptr<Cell> cell) {
    auto coords = getCoords(cell);
    auto [l, _z1] = getCell(coords.first-1, coords.second, z);
    auto [r, _z2] = getCell(coords.first+1, coords.second, z);
    auto [t, _z3] = getCell(coords.first, coords.second-1, z);
    auto [b, _z4] = getCell(coords.first, coords.second+1, z);

    auto spec = tileSet.sprites["WALL_H"];
    if (t != nullptr && b != nullptr && (t->type == CellType::WALL || b->type == CellType::WALL)) {
        spec = tileSet.sprites["WALL_V"];
    }

    if (t != nullptr && b != nullptr && t->type == CellType::WALL && b->type == CellType::WALL &&
      l != nullptr && r != nullptr && l->type == CellType::WALL && r->type == CellType::WALL) {
          spec = tileSet.sprites["WALL_CROSS"];
          return spec;
    }

    if (l != nullptr && r != nullptr && l->type == CellType::WALL && r->type == CellType::WALL) {
        spec = tileSet.sprites["WALL_H"];
        if (b != nullptr && b->type == CellType::WALL) {
          spec = tileSet.sprites["WALL_HB"];
        }
        if (t != nullptr && t->type == CellType::WALL) {
          spec = tileSet.sprites["WALL_HT"];
        }
        return spec;
    }
    if (t != nullptr && b != nullptr && t->type == CellType::WALL && b->type == CellType::WALL) {
        spec = tileSet.sprites["WALL_V"];
        if (l != nullptr && l->type == CellType::WALL) {
          spec = tileSet.sprites["WALL_VL"];
        }
        if (r != nullptr && r->type == CellType::WALL) {
          spec = tileSet.sprites["WALL_VR"];
        }
        return spec;
    }

    if (l != nullptr && b != nullptr && l->type == CellType::WALL && b->type == CellType::WALL) {
        spec = tileSet.sprites["WALL_LB"];
    }
    else
    if (r != nullptr && b != nullptr && r->type == CellType::WALL && b->type == CellType::WALL) {
        spec = tileSet.sprites["WALL_RB"];
    }
    else
    if (r != nullptr && t != nullptr && r->type == CellType::WALL && t->type == CellType::WALL) {
        spec = tileSet.sprites["WALL_RT"];
    }
    else
    if (l != nullptr && t != nullptr && l->type == CellType::WALL && t->type == CellType::WALL) {
        spec = tileSet.sprites["WALL_LT"];
    }
    return spec;
}
};

#endif // __TILE_H_
