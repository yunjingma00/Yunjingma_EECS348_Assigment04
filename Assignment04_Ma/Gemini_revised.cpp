//platform Gemini website engine
//Yunjingma Assigment04
//author yunjinig ma
//colloboration:Chatgpt halps explaination about the meaning of somepart of the code: modify and main function coment by yunjing ma
// small amoubt of the code are gemini code orginal.


//Assignment4.cpp
// Sudoku Solver using Object-Oriented Programming and Recursive Backtracking.
//EECS 348 Software Engineering I - Assignment 4

// Description:
// This program reads five Sudoku puzzle files (puzzle1.txt through puzzle5.txt),
// validates the input data, and solves each puzzle using a recursive depth-first
// search algorithm with backtracking. It prints the input filename, the original
// puzzle, and all valid solutions found (or a message if no solution exists).
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


//class SudokuSolver
// @brief Encapsulates a 9x9 Sudoku board, file loading logic, validation checks,
// and the recursive backtracking solving algorithm.
class SudokuSolver {
private:
    int board[9][9];       // 9x9 grid representing the Sudoku puzzle (0 denotes empty cells)
    std::string filename;  // The input file name associated with this puzzle instance

// brief Checks if placing a number at a specific cell violates Sudoku rules.
        // Check row and column constraints
    bool isValid(int row, int col, int num) const {
        for (int i = 0; i < 9; ++i) {
            if (board[row][i] == num) return false;//row Current row index (0-8)if any number repeat twice
            if (board[i][col] == num) return false;//colun Current row index (0-8) any number repeat twice

        }

        // Check 3x3 subgrid constraints
        int startRow = row - (row % 3);// it will out make until one loop neds all will start at  first column thrid and sixth three times to check each 3*3 grid of the row
        int startCol = col - (col % 3);
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                if (board[startRow + r][startCol + c] == num) {
                    return false;//if any of 3*3 a repeat number return flase
                }
            }
        }

        return true;
    }
    bool validateInitialBoard() const {
// the 9x9 Sudoku board.
        for (int r = 0; r < 9; ++r) {
// Visit every column in row
            for (int c = 0; c < 9; ++c) {
// Store the value of the spot
                int num = board[r][c];
// blank as 0.
                if (num == 0) {
                    continue;
                }
// Check the current row and column for duplicates in the row and column.
                for (int k = 0; k < 9; ++k) {
// Check the current row. k != c abiod compare the cell with itself.
                    if (k != c && board[r][k] == num) {
                        return false;
                    }
// Check the current column. if k != r prevents comparing the cell with itself.
                    if (k != r && board[k][c] == num) {
                        return false;
                    }
              }
// Integer division groups rows 0-2, 3-5, and 6-8.
                int startRow = (r / 3) * 3;
// Calculate the top-left column of the same 3*3grid. Integer division groups columns 0-2, 3-5, and 6-8.
                int startCol = (c / 3) * 3;
                // check all three rows of the current 3x3grid.
                for (int i = startRow; i < startRow + 3; ++i) {
//check all three columns of the subgrid.
                    for (int j = startCol; j < startCol + 3; ++j) {
// Check if there's any repition error
                        if ((i != r || j != c) &&
                            board[i][j] == num) {
// A duplicate means the initial puzzle violates Sudoku rules.
                            return false;
                        }
                    }
                }
            }
        }

        // Every given number passed all three checks.
        return true;
    }
// @brief Recursive helper function implementing DFS with backtracking to find all solutions.

    void solveRecursive(int row, int col, int &solutionCount) {
// Base case: If row reaches 9, we have successfully filled the entire grid
        if (row == 9) {// pass without the error so it count as one solution
            solutionCount++;//One solution count
            std::cout << "Solution " << solutionCount << ":\n";
            printBoard();// output ofthe result string
            return;
        }

// Determine coordinates for the next cell
        int nextRow = (col == 8) ? row + 1 : row;//row check done swtich to next row
        int nextCol = (col == 8) ? 0 : col + 1;// if column done return zero to next row index 0
// If the current cell is already pre-filled, skip to the next cell
        if (board[row][col] != 0) {// if not blank in the soduku
            solveRecursive(nextRow, nextCol, solutionCount);
            return;
        }

// Try placing digits 1 through 9 in the empty cell
        for (int num = 1; num <= 9; ++num) {
            if (isValid(row, col, num)) {// trying the number 1 to 9
                board[row][col] = num;

// Recurse to the next cell
                solveRecursive(nextRow, nextCol, solutionCount);

                board[row][col] = 0;   // Backtrack if it fail and try another branch
            }
        }
    }

public:
// @brief Constructor that initializes the Sudoku solver with a target filename.
// @param fname The name of the input text file.

    SudokuSolver(const std::string &fname) : filename(fname) {//initialize and the read the file one by one
// Initialize board with zeros as preparing for the read file
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                board[i][j] = 0;
            }
        }
    }

// @brief Loads and parses the Sudoku puzzle from the input file.
// return true if successfully loaded and validated, false on error.

bool loadPuzzle() {
        std::ifstream file(filename);
        if (!file.is_open()) {//raise an error if the file can't be read
            std::cerr << "Error: Could not open file " << filename << "\n";
            return false;
        }

        int row = 0;
        std::string line;
        while (getline(file, line)) {
            // Ignore empty lines
            if (line.empty()) continue;

            if (row >= 9) {//if row over 9 riase an error in the 9*9 sudoku board
                std::cerr << "Error: File " << filename << " contains more than 9 rows.\n";
                return false;
            }

            int col = 0;// start from the first row
            for (char ch : line) {
                // Skip whitespace characters
                if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') continue;

                if (col >= 9) {//raise an error is col greater than 9 in an 9*9 sudoku board
                    std::cerr << "Error: Row " << row + 1 << " in " << filename << " exceeds 9 columns.\n";
                    return false;
                }

                // Parse cell value: '_' or '0' or '.' represents blank cell
                if (ch == '_' || ch == '0' || ch == '.') {
                    board[row][col] = 0;
                } else if (ch >= '1' && ch <= '9') {//load the number from toe orignal sudoku board
                    board[row][col] = ch - '0';
                } else {//riase the error if aother number exist
                    std::cerr << "Error: Invalid character '" << ch << "' in " << filename << ".\n";
                    return false;
                }
                col++;//swtich to the next col
            }
            if (col != 9) {//raies an errow if the column not 9
                std::cerr << "Error: Row " << row + 1 << " in " << filename << " has " << col << " columns (expected 9).\n";
                return false;
            }
            row++;// swtich to the next row
        }
        file.close();// close the file
        if (row != 9) {//raise the error if the row is not 9
            std::cerr << "Error: File " << filename << " contains only " << row << " rows (expected 9).\n";
            return false;
        }

        return true;
    }
//else return true

//      @brief Prints the current state of the Sudoku board in a formatted grid.
    void printBoard() const {// print the board line
        for (int r = 0; r < 9; ++r) {
            if (r > 0 && r % 3 == 0) {// print the horrizonal board line if this row can be devide by 3
                std::cout << "------+-------+------\n";
            }
            for (int c = 0; c < 9; ++c) {// print the column line at 3 6 9
                if (c > 0 && c % 3 == 0) {
                    std::cout << "| ";
                }
                if (board[r][c] == 0) {//print the outside of the sukodo boarder outine
                    std::cout << "_ ";
                } else {
                    std::cout << board[r][c] << " ";//leave a blank for other
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }


// @brief Executes the solver on the loaded puzzle and prints results.
    void solve() {
        std::cout << "========================================\n";
        std::cout << "Input Filename: " << filename << "\n";
        std::cout << "Original Puzzle:\n";
        printBoard();// print the board

        int solutionCount = 0;//the solution virable
        solveRecursive(0, 0, solutionCount);//no solution virable

        if (solutionCount == 0) {//if solution not found the output string
            std::cout << "No solution found.\n";
        } else {//other condition output string
            std::cout << "Total solutions found: " << solutionCount << "\n";
        }
        std::cout << "========================================\n\n";
    }
};

/**
 * @brief Main execution function that processes puzzle1.txt through puzzle5.txt.
 */
int main() {
    // Array containing the names of the five required puzzle files
    std::string puzzleFiles[5] = {// the name of the five file
        "puzzle1.txt",
        "puzzle2.txt",
        "puzzle3.txt",
        "puzzle4.txt",
        "puzzle5.txt"
    };

    // Iterate through each puzzle file and solve it
    for (int i = 0; i < 5; ++i) {// solve the 5 puzzles one by one suing for loops
        SudokuSolver solver(puzzleFiles[i]);
        
        // Load puzzle and check for reading/format errors before solving
        if (solver.loadPuzzle()) {// if it can be read print the solution
            solver.solve();
        } else {//raise an error if the puzzle  file can't be read
            std::cout << "Skipping " << puzzleFiles[i] << " due to input errors.\n\n";
        }
    }

    return 0;
}
