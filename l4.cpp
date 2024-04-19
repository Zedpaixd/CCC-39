#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int R, C;
vector<string> grid;
vector<vector<bool>> visited;
const int dx[] = {-1, 0, 1, 0}; // W, S, D, A
const int dy[] = {0, 1, 0, -1}; // W, S, D, A
const char directions[] = {'W', 'S', 'D', 'A'};

bool isValid(int x, int y)
{
    if (x < 0 || x >= R || y < 0 || y >= C)
        return false;
    if (visited[x][y] || grid[x][y] == 'X')
        return false;
    return true;
}

bool findHamiltonianPath(int x, int y, int count, string &path)
{
    if (count == R * C - 1)
    {
        return true;
    }

    visited[x][y] = true;

    for (int i = 0; i < 4; i++)
    {
        int nx = x + dx[i], ny = y + dy[i];
        if (isValid(nx, ny))
        {
            path.push_back(directions[i]);
            if (findHamiltonianPath(nx, ny, count + 1, path))
            {
                return true;
            }
            path.pop_back();
        }
    }

    visited[x][y] = false;
    return false;
}

string findPath()
{
    string path;
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            if (grid[i][j] == '.')
            {
                visited.assign(R, vector<bool>(C, false));
                path.clear();
                if (findHamiltonianPath(i, j, 0, path))
                {
                    return path;
                }
            }
        }
    }
    return "";
}

int main()
{
    ifstream in("ccc.in");
    if (!in.is_open())
    {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    in >> R >> C;

    grid.resize(R);
    for (int i = 0; i < R; i++)
    {
        in >> grid[i];
    }
    in.close();

    string path = findPath();

    ofstream out("ccc.out");
    if (path.empty())
    {
        out << "No valid path found." << endl;
    }
    else
    {
        out << path << endl;
    }
    out.close();

    return 0;
}