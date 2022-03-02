#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class Grid {
public:
    int damage;
    int min_damage;
    int x;
    int y;
    char path;
    Grid();
};

Grid::Grid() {
    damage = INT_MAX;
    min_damage = INT_MAX;
    x = 0;
    y = 0;
    path = NULL;
}

class MinHeap {
private:
    int N;
    int **index;
    Grid *grid;
    void swap(Grid&, Grid&);
    int parent(int i) {
        return (i - 1) / 2;
    }
    int left(int i) {
        return (2 * i + 1);
    }
    int right(int i) {
        return (2 * i + 2);
    }
public:
    int size;
    MinHeap(int);
    void MinHeapify(int);
    Grid ExtractMin();
    void Insert(Grid);
    void Relax(Grid&, Grid&, char);
    void Dijkstra();
    void printDamage(ofstream&);
    void printPath(ofstream&);
};

MinHeap::MinHeap(int n) {
    N = n;
    size = 0;
    grid = new Grid[N*N];
    index = new int*[N+1];
    for(int i = 0; i <= N; i++) {
        index[i] = new int [N+1];
    }
}

void MinHeap::swap(Grid &x, Grid &y) {
    Grid temp = x;
    x = y;
    y = temp;
    int index_temp = index[x.x][x.y];
    index[x.x][x.y] = index[y.x][y.y];
    index[y.x][y.y] = index_temp;
}

void MinHeap::MinHeapify(int i) {
    int l = left(i);
    int r = right(i);
    int min = i;
    if(l < size && grid[l].min_damage < grid[min].min_damage) {
        min = l;
    }
    if(r < size && grid[r].min_damage < grid[min].min_damage) {
        min = r;
    }
    if(min != i) {
        swap(grid[i], grid[min]);
        MinHeapify(min);
    }
}

Grid MinHeap::ExtractMin() {
    Grid min = grid[0];
    swap(grid[0], grid[size - 1]);
    size--;
    MinHeapify(0);
    return min;
}

void MinHeap::Insert(Grid key) {
    size++;
    int i = size - 1;
    grid[i] = key;
    index[key.x][key.y] = i;
    while(i > 0 && grid[parent(i)].min_damage > grid[i].min_damage) {
        swap(grid[i], grid[parent(i)]);
        i = parent(i);
    }
}

void MinHeap::Relax(Grid &u, Grid &v, char path) {
    if(v.min_damage > u.min_damage + v.damage) {
        v.min_damage = u.min_damage + v.damage;
        v.path = path;
        int i = index[v.x][v.y];
        while(i > 0 && grid[parent(i)].min_damage > grid[i].min_damage) {
            swap(grid[i], grid[parent(i)]);
            i = parent(i);
        }
    }
}

void MinHeap::Dijkstra() {
    while(size > 0) {
        Grid min = ExtractMin();
        if(min.x < N) {
            Relax(min, grid[index[min.x+1][min.y]], 'R');
        }
        if(min.x > 1) {
            Relax(min, grid[index[min.x-1][min.y]], 'L');
        }
        if(min.y < N) {
            Relax(min, grid[index[min.x][min.y+1]], 'D');
        }
        if(min.y > 1) {
            Relax(min, grid[index[min.x][min.y-1]], 'U');
        }
    }
}

void MinHeap::printDamage(ofstream &fout) {
    fout << grid[index[N][N]].min_damage << " ";
}

void MinHeap::printPath(ofstream &fout) {
    int num = 0, x = N, y = N;
    char path[N*N-1];
    while(x != 1 || y != 1) {
        Grid grid_path = grid[index[x][y]];
        path[num] = grid_path.path;
        if(path[num] == 'R') {
            x--;
        }
        else if(path[num] == 'L') {
            x++;
        }
        else if(path[num] == 'D') {
            y--;
        }
        else if(path[num] == 'U') {
            y++;
        }
        num++;
    }
    fout << num+1 << endl;
    for(int i = num-1; i >= 0; i--) {
        fout << path[i];
    }
}

int main() {
    for(int i = 0; i <= 9; i++) {
        stringstream input, output;
        input << "input" << i;
        output << "output" << i;
        ifstream fin(input.str());
        if(!fin.is_open()) {
            cout << input.str() << " doesn't exist!" << endl;
            continue;
        }
        ofstream fout(output.str());
        int N;
        fin >> N;
        MinHeap grid_list(N);
        int damage, x = 0, y = 1;
        while(fin >> damage) {
            Grid grid;
            x++;
            if(x > N) {
                x = 1;
                y++;
            }
            grid.damage = damage;
            grid.x = x;
            grid.y = y;
            if(x == 1 && y == 1) {
                grid.min_damage = damage;
            }
            grid_list.Insert(grid);
        }
        grid_list.Dijkstra();
        grid_list.printDamage(fout);
        grid_list.printPath(fout);
        fin.close();
        fout.close();
    }
    return 0;
}

