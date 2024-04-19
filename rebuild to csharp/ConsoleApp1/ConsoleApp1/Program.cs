using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;

class CCC39
{
    static int LEVEL = 4;
    static string LEVEL_STAGE = "1";
    static string LEVEL_IN = $"level{LEVEL}_{LEVEL_STAGE}.in";
    static string LEVEL_OUT = $"output{LEVEL}_{LEVEL_STAGE}.out";

    static void Main(string[] args)
    {
        var lawns = ReadInput();
        var results = new List<string>();
        foreach (var lawn in lawns)
        {
            int width = lawn.Item1, height = lawn.Item2;
            char[,] matrix = lawn.Item3;
            bool found = false;
            int obstacleCount = CountObstacles(matrix, width, height);
            int requiredVisits = width * height - obstacleCount;
            for (int i = 0; i < height && !found; i++)
            {
                for (int j = 0; j < width && !found; j++)
                {
                    if (matrix[i, j] != 'X')
                    {
                        var visited = new HashSet<(int, int)>();
                        visited.Add((i, j));
                        if (Backtrack(matrix, i, j, "", visited, width, height, requiredVisits))
                        {
                            results.Add(string.Join("", visited.OrderBy(v => v.Item1 * width + v.Item2).Select(v => DirectionFrom(v, visited, width))));
                            found = true;
                        }
                    }
                }
            }
            if (!found)
            {
                results.Add("INVALID");
            }
        }
        WriteOutput(results);
    }

    static bool IsValid(int x, int y, int width, int height, char[,] matrix, HashSet<(int, int)> visited)
    {
        return x >= 0 && x < height && y >= 0 && y < width && matrix[x, y] != 'X' && !visited.Contains((x, y));
    }

    static bool Backtrack(char[,] matrix, int x, int y, string path, HashSet<(int, int)> visited, int width, int height, int requiredVisits)
    {
        if (visited.Count == requiredVisits)
        {
            return true;
        }

        var directions = new (int, int)[] { (-1, 0), (1, 0), (0, -1), (0, 1) };
        foreach (var (dx, dy) in directions)
        {
            int nx = x + dx, ny = y + dy;
            if (IsValid(nx, ny, width, height, matrix, visited))
            {
                visited.Add((nx, ny));
                if (Backtrack(matrix, nx, ny, path, visited, width, height, requiredVisits))
                    return true;
                visited.Remove((nx, ny));
            }
        }
        return false;
    }

    static string DirectionFrom((int, int) from, HashSet<(int, int)> visited, int width)
    {
        var (x, y) = from;
        if (visited.Contains((x - 1, y))) return "W";
        if (visited.Contains((x + 1, y))) return "S";
        if (visited.Contains((x, y - 1))) return "A";
        if (visited.Contains((x, y + 1))) return "D";
        return "";
    }

    static int CountObstacles(char[,] matrix, int width, int height)
    {
        return 1;
    }

    static List<(int, int, char[,])> ReadInput()
    {
        List<(int, int, char[,])> lawns = new List<(int, int, char[,])>();
        using (var reader = new StreamReader(LEVEL_IN))
        {
            int numLawns = int.Parse(reader.ReadLine().Trim());
            for (int l = 0; l < numLawns; l++)
            {
                string[] size = reader.ReadLine().Trim().Split();
                int width = int.Parse(size[0]), height = int.Parse(size[1]);
                char[,] matrix = new char[height, width];
                for (int i = 0; i < height; i++)
                {
                    string row = reader.ReadLine().Trim();
                    for (int j = 0; j < width; j++)
                        matrix[i, j] = row[j];
                }
                lawns.Add((width, height, matrix));
                Debug.WriteLine("added one");
            }
        }
        return lawns;
    }

    static void WriteOutput(List<string> results)
    {
        using (var writer = new StreamWriter(LEVEL_OUT, false))  // Ensure we are not appending to an old file
        {
            foreach (var result in results)
                writer.WriteLine(result);
        }
    }
}