#include <stdio.h>
#include <stdlib.h>

struct Cell {
    int values[9];
    int solved;
};

int ind(int row, int col) {
    return 9 * row + col;
}

struct Cell *initialize_sudoku_matrix(char board[9][9]) {
    struct Cell *cells = malloc(9 * 9 * sizeof(struct Cell));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == '.') {
                for (int k = 0; k < 9; k++) {
                    cells[ind(i, j)].values[k] = 1;
                }
            } else {
                cells[ind(i, j)].values[board[i][j] - 48 - 1] = 1;
                cells[ind(i, j)].solved = 1;
            }
        }
    }
    return cells;
}

int cell_value(struct Cell cell) {
    int nums = 0;
    int val = 0;
    for (int i = 0; i < 9; i++) {
        if (cell.values[i]) {
            val = i + 1;
            nums++;
        }
    }

    if (nums == 1) {
        return val;
    } else {
        return 0;
    }
}

void remove_from_row_domains(struct Cell *cells, int row, int col, int value) {
    for (int j = 0; j < 9; j++) {
        int index = ind(row, j);
        if (j != col && !cells[index].solved) {
            cells[index].values[value - 1] = 0;
            if (cell_value(cells[index]) != 0) {
                cells[index].solved = 1;
            }
        }
    }
}

void remove_from_col_domains(struct Cell *cells, int row, int col, int value) {
    for (int i = 0; i < 9; i++) {
        int index = ind(i, col);
        if (i != row && !cells[index].solved) {
            cells[index].values[value - 1] = 0;
            if (cell_value(cells[index]) != 0) {
                cells[index].solved = 1;
            }
        }
    }
}

void remove_from_square_domain(struct Cell *cells, int row, int col, int value, int s_row, int s_col) {
    for (int i = s_row; i < s_row + 3; i++) {
        for (int j = s_col; j < s_col + 3; j++) {
            int index = ind(i, j);
            if (i != row && j != col && !cells[index].solved) {
                cells[index].values[value - 1] = 0;
                if (cell_value(cells[index]) != 0) {
                    cells[index].solved = 1;
                }
            }
        }
    }
}

void remove_from_square_domains(struct Cell *cells, int row, int col, int value) {
    if (row >= 0 && row <= 2) {
        int r = 0;
        if (col >= 0 && col <= 2) {
            int c = 0;
            remove_from_square_domain(cells, row, col, value, r, c);
        }
        if (col >= 3 && col <= 5) {
            int c = 3;
            remove_from_square_domain(cells, row, col, value, r, c);
        }
        if (col >= 6 && col <= 8) {
            int c = 6;
            remove_from_square_domain(cells, row, col, value, r, c);
        }
    }
    if (row >= 3 && row <= 5) {
        int r = 3;
        if (col >= 0 && col <= 2) {
            int c = 0;
            remove_from_square_domain(cells, row, col, value, r, c);
        }
        if (col >= 3 && col <= 5) {
            int c = 3;
            remove_from_square_domain(cells, row, col, value, r, c);
        }
        if (col >= 6 && col <= 8) {
            int c = 6;
            remove_from_square_domain(cells, row, col, value, r, c);
        }
    }
    if (row >= 6 && row <= 8) {
        int r = 6;
        if (col >= 0 && col <= 2) {
            int c = 0;
            remove_from_square_domain(cells, row, col, value, r, c);
        }
        if (col >= 3 && col <= 5) {
            int c = 3;
            remove_from_square_domain(cells, row, col, value, r, c);
        }
        if (col >= 6 && col <= 8) {
            int c = 6;
            remove_from_square_domain(cells, row, col, value, r, c);
        }
    }
}

void propagate_cells(struct Cell *cells) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int index = 9 * i + j;
            if (cells[index].solved) {
                remove_from_row_domains(cells, i, j, cell_value(cells[index]));
                remove_from_col_domains(cells, i, j, cell_value(cells[index]));
                remove_from_square_domains(cells, i, j, cell_value(cells[index]));
            }
        }
    }
}

int check_cells(struct Cell *cells) {
    int count = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int index = ind(i, j);
            if (cell_value(cells[index]) != 0) {
                count++;
            }
        }
    }

    if (count == 9 * 9) {
        return 1;
    } else {
        return 0;
    }
}

void copy_solution(struct Cell *cells, char board[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == '.') {
                board[i][j] = (char) (cell_value(cells[ind(i, j)]) + 48);
            }
        }
    }
}


void solveSudoku(char board[9][9]){
    struct Cell *cells = initialize_sudoku_matrix(board);
    int solved = 0;
    while (!solved) {
        propagate_cells(cells);
        solved = check_cells(cells);
    }
    copy_solution(cells, board);
}

void print_matrix(char matrix[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    char matrix[9][9] = {{'5','3','.','.','7','.','.','.','.'},
                    {'6','.','.','1','9','5','.','.','.'},
                    {'.','9','8','.','.','.','.','6','.'},
                    {'8','.','.','.','6','.','.','.','3'},
                    {'4','.','.','8','.','3','.','.','1'},
                    {'7','.','.','.','2','.','.','.','6'},
                    {'.','6','.','.','.','.','2','8','.'},
                    {'.','.','.','4','1','9','.','.','5'},
                    {'.','.','.','.','8','.','.','7','9'}};
    print_matrix(matrix);
    solveSudoku(matrix);
    print_matrix(matrix);

    return 0;
}
