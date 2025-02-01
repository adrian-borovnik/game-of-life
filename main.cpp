#include <iostream>
#include "raylib.h"

void InitGrid(std::vector<std::vector<bool>> &grid, const int &cellNum) {
    for (int i = 0; i < cellNum;  ++i) {
        for (int j = 0; j < cellNum; ++j) {
            grid[i][j] = 0;
//            grid[i][j] = GetRandomValue(0, 1);
        }
    }
}

void HandleMouseClick(std::vector<std::vector<bool>> &grid, const int &cellSize) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int x = GetMouseX() / cellSize;
        int y = GetMouseY() / cellSize;
        
        grid[x][y] = !grid[x][y];
    }
}

void UpdateCells(std::vector<std::vector<bool>> &grid) {
    std::vector<std::vector<bool>> newGrid = grid;
    
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[i].size(); ++j) {
            int live_neighbors = 0;
            
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    if (x == 0 && y == 0) continue;
                    
                    int new_x = i + x;
                    int new_y = j + y;
                    
                    if (new_x < 0 || new_x >= grid.size() || new_y < 0 || new_y >= grid[i].size()) continue;
                    
                    if (grid[new_x][new_y]) live_neighbors++;
                }
            }
            
            if (grid[i][j]) {
                if (live_neighbors < 2 || live_neighbors > 3) newGrid[i][j] = 0;
            } else {
                if (live_neighbors == 3) newGrid[i][j] = 1;
            }
        }
    }
    
    grid = newGrid;
}

void DrawGrid(const int &cellSize, const int &screenSize)  {
    for (int i = 0; i < screenSize; i += cellSize) {
        DrawLine(i, 0, i, screenSize, DARKGRAY);
        DrawLine(0, i, screenSize, i, DARKGRAY);
    }
}

void DrawCells(const std::vector<std::vector<bool>> &grid, const int &cellSize) {
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j]) {
                DrawRectangle(i * cellSize, j * cellSize, cellSize, cellSize, WHITE);
            }
        }
    }
}

void DrawRunningText(const bool &running) {
    int fontSize = 20;
    if (running)
        DrawText("Running", 10, 10, fontSize, GREEN);
    else
        DrawText("Paused", 10, 10, fontSize, RED);
}

void DrawGenerationText(const u_long &generation, const int &screenSize) {
    int fontSize = 20;
    DrawText(TextFormat("Generation: %lu", generation), 10, screenSize - fontSize - 10, fontSize, LIGHTGRAY);
}

int main() {
    const int SCREEN_SIZE =  800;
    const int CELL_NUM = 32;
    const int GRID_SIZE = SCREEN_SIZE / CELL_NUM;
    
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Game of Life");
    SetTargetFPS(15);
    
    bool running = false;
    bool show_grid = true;
    
    u_long generation = 0;
    
    std::vector<std::vector<bool>> grid(CELL_NUM, std::vector<bool>(CELL_NUM, 0));
    InitGrid(grid, CELL_NUM);
    
    while (!WindowShouldClose()) {
        HandleMouseClick(grid, GRID_SIZE);
        
        if (IsKeyPressed(KEY_SPACE)) running = !running;
        if (IsKeyPressed(KEY_G)) show_grid = !show_grid;
        
        if (IsKeyPressed(KEY_C)) InitGrid(grid, CELL_NUM);
        
        if (running || IsKeyPressed(KEY_N)) {
            UpdateCells(grid);
            ++generation;
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawCells(grid, GRID_SIZE);
        if (show_grid) DrawGrid(GRID_SIZE, SCREEN_SIZE);
        
        DrawRunningText(running);
        DrawGenerationText(generation, SCREEN_SIZE);
        
        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}
