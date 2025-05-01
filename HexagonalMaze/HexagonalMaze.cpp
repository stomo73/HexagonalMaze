#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct Coord
{
  static int w;
  static int h;

  int line;
  int y;
  int x;

  Coord(int line, int y, int x)
    : line(line)
    , x(x)
    , y(y)
  {
  }

  friend bool operator==(const Coord& c1, const Coord& c2)
  {
    return (c2.line == c1.line) && (c2.y == c1.y) && (c2.x == c1.x);
  }

  friend bool operator!=(const Coord& c1, const Coord& c2)
  {
    return !(c1 == c2);
  }

  friend Coord operator+(const Coord& c1, const Coord& c2)
  {
    int line = (c1.line + c2.line) & 1;
    int y = (c1.y + c2.y);
    if (y < 0)
    {
      y += Coord::h / 2;
    }
    else if (y >= Coord::h / 2)
    {
      y -= Coord::h / 2;
    }

    int x = (c1.x + c2.x);
    if (x < 0)
    {
      x += Coord::w;
    }
    else if (x >= Coord::w)
    {
      x -= Coord::w;
    }

    return Coord(line, y, x);
  }
};

const Coord INV_COORD(-1, -1, -1);

int Coord::w;
int Coord::h;

#define GET_MAP(mmap, coord) mmap[coord.line][coord.y][coord.x]

/*
  (0,0,0)       (0,0,1)       (0,0,2)
         (1,0,0)       (1,0,1)       (1,0,2)
  (0,1,0)       (0,1,1)       (0,1,2)
         (1,1,0)       (1,1,1)       (1,1,2)
*/

#define NUM_DIRS (6u)

typedef Coord DIRS_ITEMS[NUM_DIRS];

const DIRS_ITEMS DIRS[2] =
{
  {
    Coord(0, 0, 1), // (0,1,1) -> (0,1,2)
    Coord(1, 0, 0), // (0,1,1) -> (1,1,1)
    Coord(1, 0,-1), // (0,1,1) -> (1,1,0)
    Coord(0, 0,-1), // (0,1,1) -> (0,1,0)
    Coord(1,-1,-1), // (0,1,1) -> (1,0,0)
    Coord(1,-1, 0), // (0,1,1) -> (1,0,1)
  },
  {
    Coord(0, 0, 1), // (1,0,1) -> (1,0,2)
    Coord(1, 1, 1), // (1,0,1) -> (0,1,2)
    Coord(1, 1, 0), // (1,0,1) -> (0,1,1)
    Coord(0, 0,-1), // (1,0,1) -> (1,0,0)
    Coord(1, 0, 0), // (1,0,1) -> (0,0,1)
    Coord(1, 0, 1), // (1,0,1) -> (0,0,2)
  }
};

int main()
{
  vector<string> rows[2];
  vector<vector<Coord>> parents[2];

  Coord coord_start = INV_COORD;
  Coord coord_end = INV_COORD;

  cin >> Coord::w >> Coord::h; cin.ignore();
  for (int y = 0; y < Coord::h; ++y)
  {
    string row;
    getline(cin, row);

    rows[y & 1].push_back(row);
    vector<Coord> row_parents;
    for (int x = 0; x < Coord::w; ++x)
    {
      row_parents.push_back(INV_COORD);
    }
    parents[y & 1].push_back(row_parents);

    if (coord_start == INV_COORD)
    {
      std::size_t found = row.find('S');
      if (found != std::string::npos)
      {
        coord_start = Coord(y & 1, y / 2, (int)found);
      }
    }

    if (coord_end == INV_COORD)
    {
      std::size_t found = row.find('E');
      if (found != std::string::npos)
      {
        coord_end = Coord(y & 1, y / 2, (int)found);
      }
    }
  }

  struct Q_Item
  {
    Coord parent;
    Coord coord;
  };

  queue<Q_Item> q;

  q.push(Q_Item{INV_COORD, coord_start});
  GET_MAP(parents, coord_start) = coord_start;

  do
  {
    Q_Item& q_item = q.front();

    for (int d = 0; d < NUM_DIRS; ++d)
    {
      Coord c1 = q_item.coord + DIRS[q_item.coord.line][d];

      if (GET_MAP(parents, c1) != INV_COORD)
      {
        continue;
      }

      if (c1 == coord_end)
      {
        Coord parent_coord = q_item.coord;

        while (parent_coord != coord_start)
        {
          GET_MAP(rows, parent_coord) = '.';
          parent_coord = GET_MAP(parents, parent_coord);

        }
          
        goto found;
      }

      if (GET_MAP(rows, c1) == '_')
      {
        GET_MAP(parents, c1) = q_item.coord;

        q.push(Q_Item{q_item.coord, c1});
      }
    }

    q.pop();
  } while (!q.empty());

found:
  for (int y = 0; y < Coord::h; ++y)
  {
    cout << rows[y & 1][y / 2] << endl;
  }
}