LEVEL = 4
LEVEL_STAGE = "example"
LEVEL_IN = f"level{LEVEL}_{LEVEL_STAGE}.in"
LEVEL_OUT = f"output{LEVEL}_{LEVEL_STAGE}.out"

def writeOutput(whatToWrite):
    with open(LEVEL_OUT, 'w') as f:
        if isinstance(whatToWrite, (str, int)):
            f.write(str(whatToWrite) + '\n')
        else:
            for line in whatToWrite:
                f.write(str(line) + '\n')


# def problem1(matrix):
#     temp = []
#     for line in matrix:
#         temp.append("{} {} {} {}".format(line[0].count("W"),line[0].count("D"),line[0].count("S"),line[0].count("A")))
#     # writeOutput(temp)
#     print(temp)

# def problem2(matrix):
#     width = 0
#     height = 0
#     temp = []
#     for line in matrix:
#         width = 0
#         height = 0
#         minH = 0
#         maxH = 0
#         minW = 0
#         maxW = 0
#         # print(line)
#         for direction in line[0]:
#             if direction == 'S':
#                 height -= 1
#                 if height <= minH:
#                     minH = height
#                 # print("Currently minH = ",minH)
#             elif direction == 'W':
#                 height += 1
#                 if height > maxH:
#                     maxH = height
#                 # print("Currently maxH = ",maxH)
#             elif direction == 'A':
#                 width -= 1
#                 if width < minW:
#                     minW = width
#                 # print("Currently minW = ",minW)
#             elif direction == 'D': 
#                 width +=1
#                 if width > maxW:
#                     maxW = width
#                 # print("Currently maxW = ",maxW)
#         temp.append("{} {}".format(maxW + abs(minW) + 1, maxH + abs(minH) + 1))
#     # writeOutput(temp)
#     return temp


# def readInput():
#     with open(LEVEL_IN,'r') as f:
#         '''
#         Read the matrix based on prior size
#         '''
#         its = 0
#         size = f.readline()
#         for _ in range(int(size)):
#             (width,height) = f.readline().split(" ")
#             matrix = []
#             for _ in range(int(height)):
#                 temp = f.readline()
#                 temp = temp[:len(temp)-1] if temp.find('\n') > 0 else temp
#                 matrix.append(temp)
#             path = f.readline()
#             its += 1
#             #if its == 1 or its == 2:
#                # print(path[:-1])
#                # print(matrix)
#             result.append(problem3(int(width),int(height),matrix,path[:-1]))

#         #     
#         #     matrix.append([temp])
#         '''
#         Additional things to read
#         '''
#         # ...

#     return [0]


def readInput():
    with open(LEVEL_IN, 'r') as f:
        num_lawns = int(f.readline().strip())
        lawns = []
        for _ in range(num_lawns):
            width, height = map(int, f.readline().strip().split())
            matrix = [f.readline().strip() for _ in range(height)]
            lawns.append((width, height, matrix))
        return lawns

def is_valid(x, y, width, height, matrix, visited):
    return 0 <= x < height and 0 <= y < width and matrix[x][y] != 'X' and not visited[x][y]

def iterative_dfs(matrix, start_x, start_y, width, height):
    stack = [(start_x, start_y, "", [[False]*width for _ in range(height)])]
    stack[0][3][start_x][start_y] = True  # Mark the starting point as visited
    directions = [(0, 1, 'D'), (1, 0, 'S'), (0, -1, 'A'), (-1, 0, 'W')]  # Right, Down, Left, Up
    
    while stack:
        x, y, path, visited = stack.pop()
        if all(all(row) for row in visited):  # Check if all cells are visited
            return path
        
        for dx, dy, dir in directions:
            nx, ny = x + dx, y + dy
            if is_valid(nx, ny, width, height, matrix, visited):
                visited_copy = [row[:] for row in visited]
                visited_copy[nx][ny] = True
                stack.append((nx, ny, path + dir, visited_copy))
    return None  # If no path found


def problem3(width,height,matrix,path):

    for h in range(0,height):
        for w in range(0,width):
            valid = True
            visited = set()
            pos = [h,w] # h = W and S, w = A and D
            visited.add((pos[0],pos[1]))

            if matrix[pos[0]][pos[1]] == 'X':
                valid = False

            for dir in path:
                if dir == 'W':
                    pos[0] -= 1
                if dir == 'S':
                    pos[0] += 1
                if dir == 'A':
                    pos[1] -= 1
                if dir == 'D':
                    pos[1] += 1
                
                if pos[1] < 0 or pos[0] < 0 or pos[0] >= height or pos[1] >= width or matrix[pos[0]][pos[1]] == 'X' or (pos[0],pos[1]) in visited:
                    valid = False
                else:
                    visited.add((pos[0],pos[1]))

            if len(visited) != (height * width - 1):
                valid = False

            if valid:
                return "VALID"
    return "INVALID"



def problem4():
    lawns = readInput()
    results = []
    for width, height, matrix in lawns:
        solution_found = False
        for i in range(height):
            for j in range(width):
                if matrix[i][j] != 'X':
                    solution = iterative_dfs(matrix, i, j, width, height)
                    if solution:
                        results.append(solution)
                        solution_found = True
                        break
            if solution_found:
                break
        if not solution_found:
            results.append("INVALID")
    
    writeOutput(results)

problem4()
