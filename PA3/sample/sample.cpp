// Credit: LIU, Jianmeng

#include <iostream>
using namespace std;

// some constraints for board
const int MAX_ROW = 15, MAX_COL = 15;
const int MAX_CONSTRAINT_NUM = 15; // constraint cannot exceed MAX_ROW/MAX_COL

// board and its size
int num_rows = 0, num_cols = 0;
char board[MAX_ROW][MAX_COL] = {{0}};

// constraints and their sizes
int num_row_constraints[MAX_ROW] = {0}, num_col_constraints[MAX_COL] = {0};
int row_constraints[MAX_ROW][MAX_CONSTRAINT_NUM] = {{0}};
int col_constraints[MAX_COL][MAX_CONSTRAINT_NUM] = {{0}};

/**********************************
 * Part 1: Initialize the board   *
 **********************************/

/**
 * This function init board with all '.', meaning that it is not filled yet.
 */
void init_board()
{
    for (int i = 0; i < num_rows; ++i)
    {
        for (int j = 0; j < num_cols; ++j)
        {
            board[i][j] = '.';
        }
    }
}

/**
 * These two functions checks if the constraints user entered is valid.
 * This is a very simple check: only check if there are enough cells
 * to put those constraints.
 *
 * For example, if a row constraint is [3, 2, 2], we at least need
 * 3 + 1 + 2 + 1 + 2 = 9 cells in this row. If the row has less than 9 cells,
 * then the constraint is invalid.
 */
bool is_row_constraint_valid(int row_idx)
{
    int sum = 0;
    for (int i = 0; i < num_row_constraints[row_idx]; ++i)
    {
        sum += row_constraints[row_idx][i];
        if (i != num_row_constraints[row_idx] - 1)
        {
            ++sum;
        }
    }
    return sum <= num_cols;
}

bool is_column_constraint_valid(int col_idx)
{
    int sum = 0;
    for (int i = 0; i < num_col_constraints[col_idx]; ++i)
    {
        sum += col_constraints[col_idx][i];
        if (i != num_col_constraints[col_idx] - 1)
        {
            ++sum;
        }
    }
    return sum <= num_rows;
}

/**
 * This function reads the board size and constraints from user input,
 * store the information in global variables and init the board.
 *
 * For simplicity, we assume the user input is always valid.
 */
void get_input_board()
{
    // read board size
    cout << "Enter the number of rows: ";
    cin >> num_rows;
    cout << "Enter the number of columns: ";
    cin >> num_cols;

    // read row constraints for each row, until user input -1
    for (int i = 0; i < num_rows; ++i)
    {
        bool first_time = true;
        do
        {
            if (!first_time)
            {
                cout << "Invalid row constraint, please try again." << endl;
            }
            else
            {
                first_time = false;
            }
            cout << "Enter the number of constraints for row " << i << " (end with -1): ";

            num_row_constraints[i] = 0;
            while (true)
            {
                int constraint;
                cin >> constraint;
                if (constraint == -1)
                {
                    break;
                }
                row_constraints[i][num_row_constraints[i]] = constraint;
                ++num_row_constraints[i];
            }
        } while (!is_row_constraint_valid(i));
    }
    // read column constraints for each column, until user input -1
    for (int i = 0; i < num_cols; ++i)
    {
        bool first_time = true;
        do
        {
            if (!first_time)
            {
                cout << "Invalid column constraint, please try again." << endl;
            }
            else
            {
                first_time = false;
            }
            cout << "Enter the number of constraints for column " << i << " (end with -1): ";

            num_col_constraints[i] = 0;
            while (true)
            {
                int constraint;
                cin >> constraint;
                if (constraint == -1)
                {
                    break;
                }
                col_constraints[i][num_col_constraints[i]] = constraint;
                ++num_col_constraints[i];
            }
        } while (!is_column_constraint_valid(i));
    }

    // init board to be empty
    init_board();
}

/**
 * This function prints the board and the constraints for each row and column.
 * Constraints should be printed on the bottom of each column and on the left of each row.
 *
 * You may find an example in webpage.
 */
void print_board()
{
    // in order to know how many extra spaces to add before each row
    int max_row_constraint_num = 0;
    for (int i = 0; i < num_rows; ++i)
    {
        if (num_row_constraints[i] > max_row_constraint_num)
        {
            max_row_constraint_num = num_row_constraints[i];
        }
    }

    // print column index
    // two extra space for row index
    for (int i = 0; i < max_row_constraint_num + 2; ++i)
    {
        cout << "  ";
    }
    for (int i = 0; i < num_cols; ++i)
    {
        cout << " " << (char)(i + 'A');
    }
    cout << endl;

    // print each row
    for (int i = 0; i < num_rows; ++i)
    {
        // print extra spaces
        for (int j = 0; j < max_row_constraint_num - num_row_constraints[i]; ++j)
        {
            cout << "  ";
        }
        // print row constraints
        for (int j = 0; j < num_row_constraints[i]; ++j)
        {
            cout << row_constraints[i][j] << " ";
        }
        // print row index
        cout << "| ";
        if (i < 10)
            cout << " " << i << " ";
        else
            cout << i << " ";
        // print row board
        for (int j = 0; j < num_cols; ++j)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }

    // in order to know how many extra lines we need for column constraints
    int max_col_constraint_num = 0;
    for (int i = 0; i < num_cols; ++i)
    {
        if (num_col_constraints[i] > max_col_constraint_num)
        {
            max_col_constraint_num = num_col_constraints[i];
        }
    }

    // print column constraints
    for (int i = 0; i < max_col_constraint_num; ++i)
    {
        // print extra spaces
        // two extra space for row index
        for (int j = 0; j < max_row_constraint_num + 2; ++j)
        {
            cout << "  ";
        }
        cout << " "; // one extra space because row index takes 2 characters

        // print column constraints
        for (int j = 0; j < num_cols; ++j)
        {
            if (i < num_col_constraints[j])
            {
                cout << col_constraints[j][i] << " ";
            }
            else
            {
                cout << "  ";
            }
        }

        cout << endl;
    }
}

/********************************
 * Part 2: User play on board   *
 ********************************/

/**
 * This function set/unset a cell on the board.
 * If the cell was set, unset it, and vice versa.
 * You may assume the parameters are always valid.
 */
void modify_board(int row, int col)
{
    if (board[row][col] == '.')
    {
        board[row][col] = 'X';
    }
    else
    {
        board[row][col] = '.';
    }
}

/**
 * This function will be called when user choose to set/unset a cell.
 *
 * You need to:
 * 1. Ask user to input which cell he/she wants to modify
 * 2. Check if the user input is a valid ceil(i.e., it is within the board)
 *    If invalid, keep asking the user to input.
 * 3. Call 'modify_board()' to set/unset the cell
 * 4. Print the new board
 */
void user_operate_board()
{
    // ask user to input which cell he/she wants to modify
    cout << "Enter the cell you want to modify (e.g. A 2): ";
    int row, col;
    char col_input;
    while (true)
    {
        cin >> col_input >> row;
        col = col_input - 'A';
        if (row >= 0 && row < num_rows && col >= 0 && col < num_cols)
        {
            modify_board(row, col);
            break;
        }
        else
        {
            cout << "Invalid row or column. Try again." << endl;
            cout << "Enter the cell you want to modify (e.g. A 2): ";
        }
    }
    // print the new board
    // print_board();
}

/**
 * This function will be called after user finish filling the whole board.
 * You need to check whether his/her solution is correct,
 * i.e., follow all constraints.
 *
 * Return true if his/her solution is correct; false otherwise.
 */
bool check_whole_board_valid()
{
    // check row
    for (int i = 0; i < num_rows; ++i)
    {
        int num_blocks_met = 0; // num of blocks we have met
        int num_cells = 0;      // num of remaining cells in the current block
        bool in_block = false;  // a flag, if we are in a block

        for (int j = 0; j < num_cols; ++j)
        {
            if (board[i][j] == 'X')
            {
                if (in_block)
                {
                    // current cell is filled and we are in_block
                    // then we are ok if num_cells > 0
                    --num_cells;
                    if (num_cells < 0)
                        return false;
                }
                else
                {
                    // current cell is filled and we are not in_block
                    // then this cell starts a new block
                    // this is ok if we have met no more than in constraints
                    if (num_blocks_met >= num_row_constraints[i])
                        return false;

                    // ok, start a new block
                    num_cells = row_constraints[i][num_blocks_met++] - 1;
                    in_block = true;
                }
            }
            else if (in_block)
            {
                // if we met an empty cell and we are in_block
                // this is only valid if num_cells is 0
                if (num_cells > 0)
                    return false;

                // ok, we are not in_block anymore
                in_block = false;
            }
        }

        // after the loop, num_blocks_met must equal to num_row_constraints[i]
        if (num_blocks_met != num_row_constraints[i])
            return false;
    }

    // check col
    for (int i = 0; i < num_cols; ++i)
    {
        int num_blocks_met = 0; // num of blocks we have met
        int num_cells = 0;      // num of remaining cells in the current block
        bool in_block = false;  // a flag, if we are in a block

        for (int j = 0; j < num_rows; ++j)
        {
            if (board[j][i] == 'X')
            {
                if (in_block)
                {
                    --num_cells;
                    if (num_cells < 0)
                        return false;
                }
                else
                {
                    if (num_blocks_met >= num_col_constraints[i])
                        return false;

                    num_cells = col_constraints[i][num_blocks_met++] - 1;
                    in_block = true;
                }
            }
            else if (in_block)
            {
                if (num_cells > 0)
                    return false;
                in_block = false;
            }
        }

        if (num_blocks_met != num_col_constraints[i])
            return false;
    }

    // if we reach here, it means the board is valid
    return true;
}

/*******************************
 * Part 3: A nonogram solver   *
 *******************************/

/**
 * This function coverts the position vector of a row into a real row.
 * For example, if num_col = 6, position vector = {0, 3, 5},
 * row_constraint for this row is {2, 1, 1}, then the row should be:
 * [X X . X . X]
 * It will store the result row in variable 'result_row[]'
 *
 * You may assume the parameters given are always valid.
 */
void positions_to_row(int row_idx, const int positions[], int num_pos, char result_row[])
{
    // clear result_row
    for (int i = 0; i < num_cols; ++i)
    {
        result_row[i] = '.';
    }
    for (int i = 0; i < num_pos; ++i)
    {
        // fill [positions[i], positions[i] + row_constraint[row_idx][i] - 1] with 'X'
        for (int j = positions[i]; j < positions[i] + row_constraints[row_idx][i]; ++j)
        {
            result_row[j] = 'X';
        }
    }
}

/**
 * This function checks if the given block on given row can be shifted right
 * for one cell.
 * For example, if the position vector is {0, 4, 6}, num_col = 8
 * row_constraint for this row is {2, 1, 1},
 * then the row is: [X X . . X . X .], and there are 3 blocks
 * then block_can_shift(row_idx, 0, {0, 4, 6}) = true,
 *      block_can_shift(row_idx, 1, {0, 4, 6}) = false, since it will hit 2nd block after shift
 *      block_can_shift(row_idx, 2, {0, 4, 6}) = true.
 *
 * More details can be found on webpage.
 */
bool block_can_shift(int row_idx, int block_idx, const int positions[], int num_pos)
{
    // for last block, it cannot hit boundary
    if (block_idx + 1 == num_pos)
    {
        return positions[block_idx] + row_constraints[row_idx][block_idx] < num_cols;
    }
    // for other blocks, it cannot hit next block
    return positions[block_idx] + row_constraints[row_idx][block_idx] + 1 < positions[block_idx + 1];
}

// this array stores all valid permutations for each row
// it's expected that num of permutations for each row should not exceed 2^(num_cols)
const int MAX_PERM = 1 << MAX_COL; // 2^(num_cols)
char row_perms[MAX_ROW][MAX_PERM][MAX_COL];
// this array stores num of permutations for each row
int num_row_perms[MAX_ROW] = {0};

/**
 * Given a starter position vector for a row, this function will generate
 * all other permutations according to the rules described in webpage.
 *
 * Basically you need to:
 * 1. Store the given valid position in row_perms[][][]
 * 2. Recursively call this function to generate other permutations
 *
 * Note that all permutations generated should be stored in row_perms[][][]
 */
void generate_row_perms(int row_idx, int positions[], int num_pos, int block_idx)
{
    // store current permutation
    // first we need to convert from position vector to an real row.
    char curr_perm[MAX_COL];
    positions_to_row(row_idx, positions, num_pos, curr_perm);
    // then store it
    for (int i = 0; i < num_cols; ++i)
    {
        row_perms[row_idx][num_row_perms[row_idx]][i] = curr_perm[i];
    }
    ++num_row_perms[row_idx];

    // if there is no more block, return
    if (block_idx < 0)
    {
        return;
    }

    // if current block can be shifted, generate other permutations
    while (block_can_shift(row_idx, block_idx, positions, num_pos))
    {
        // copy position into a new array (since we need it in recursion)
        int new_positions[MAX_COL];
        for (int i = 0; i < num_pos; ++i)
        {
            new_positions[i] = positions[i];
        }
        // shift current block
        ++new_positions[block_idx];
        ++positions[block_idx];
        // generate other permutations
        generate_row_perms(row_idx, new_positions, num_pos, block_idx - 1);
    }
}

/**
 * This function gets all valid permutations for a row,
 * and store all results in global variables (cache).
 *
 * It will generate a starter position vector, and then call generate_row_perms()
 */
void get_row_perms(int row_idx)
{
    // if in cache, do nothing
    if (num_row_perms[row_idx] != 0)
    {
        return;
    }

    // if no constraints for this row, return only one perm (all '.')
    if (num_row_constraints[row_idx] == 0)
    {
        // store it in cache
        num_row_perms[row_idx] = 1;
        for (int i = 0; i < num_cols; ++i)
        {
            row_perms[row_idx][0][i] = '.';
        }
        return;
    }

    // generate starter position vector
    int positions[MAX_COL] = {0};
    int num_pos = 1;
    for (int i = 1; i < num_row_constraints[row_idx]; ++i)
    {
        positions[i] = positions[i - 1] + row_constraints[row_idx][i - 1] + 1;
        ++num_pos;
    }

    // generate all permutations
    generate_row_perms(row_idx, positions, num_pos, num_pos - 1);
    // now permutations should have already been stored in cache
    // we can directly return
    return;
}

/**
 * This function checks if current state is valid, after
 * we finish filling 'num_complete_rows' rows.
 *
 * For example, if num_complete_rows = 2, it will only check
 * if the first two rows (with index 0 and 1) do not
 * break column constraints.
 *
 * Note that this function is different from 'check_whole_board_valid()'
 * in 2 ways:
 * 1. We don't check whole board here, only check the first 'num_complete_rows' rows.
 * 2. We don't (need to) check row_constraints here, only check column_constraints.
 *    This is because this function will only be called during solver,
 *    so the rows are generated in permutation process, which must be valid.
 */
bool check_rows_valid(int num_complete_rows)
{
    if (num_complete_rows <= 0)
    {
        return true;
    }

    for (int i = 0; i < num_cols; ++i)
    {
        // special case: if no constraint for this column,
        // it is valid if no cells are filled
        if (num_col_constraints[i] == 0)
        {
            for (int j = 0; j < num_complete_rows; ++j)
            {
                if (board[j][i] != '.')
                {
                    return false;
                }
            }
            // if no cells are filled, it is valid
            continue;
        }

        bool in_block = false;  // flag to indicate if we are in a block
        int num_blocks_met = 0; // num of blocks we have met
        int num_cells = 0;      // num of remaining cells in current block

        for (int j = 0; j < num_complete_rows; ++j)
        {
            if (board[j][i] == 'X')
            {
                if (in_block)
                {
                    // if current cell is filled and we are in_block
                    // then we are ok if num_cells > 0,
                    // which means there are no more cells in the block than in the constraint
                    --num_cells;
                    if (num_cells < 0)
                        return false;
                }
                else
                {
                    // if current cell is filled and we are not in_block
                    // then this cell starts a new block
                    // this is ok if we have met no more than in constraints
                    if (num_blocks_met >= num_col_constraints[i])
                        return false;

                    // ok, start a new block
                    num_cells = col_constraints[i][num_blocks_met++] - 1;
                    in_block = true;
                }
            }
            else if (in_block)
            {
                // if we met an empty cell and we are in_block
                // this is only valid if num_cells is 0
                if (num_cells > 0)
                    return false;

                // ok, we are not in_block anymore
                in_block = false;
            }
        }

        // If we reach last row, but blocks we met is not equal to num_col_constraints[i],
        // then it is not valid.
        if (num_complete_rows == num_rows && num_blocks_met != num_col_constraints[i])
        {
            return false;
        }

        // If the column cannot be completed with the remaining blocks
        // then it is not valid.
        int remaining_cells = num_rows - num_complete_rows;
        int remaining_constraints = 0;
        for (int k = num_blocks_met; k < num_col_constraints[i]; ++k)
        {
            remaining_constraints += col_constraints[i][k];
        }
        if (remaining_cells < remaining_constraints + num_col_constraints[i] - num_blocks_met - 1)
        {
            return false;
        }
    }

    // if we reach here, it is valid
    return true;
}

/**
 * This is a function to recursively solve the board, and store
 * the solution in array solution[][].
 *
 * It should:
 * 1. Check if the current state is valid.
 * 2. If we have filled all rows, the current state is a solution.
 * 3. Try all possible permutations for this row, and recursively call
 *    this function to proceed on next row.
 */
void solve_helper(int row_idx, char solution[MAX_ROW][MAX_COL])
{
    // if current state is not valid, return
    if (!check_rows_valid(row_idx))
    {
        return;
    }

    // if we have filled all rows, we are done
    if (row_idx == num_rows)
    {
        // store it in solution
        for (int i = 0; i < num_rows; ++i)
        {
            for (int j = 0; j < num_cols; ++j)
            {
                solution[i][j] = board[i][j];
            }
        }
        return;
    }

    // try each permutation for this row
    get_row_perms(row_idx);
    for (int i = 0; i < num_row_perms[row_idx]; ++i)
    {
        // use current permutation to fill this row
        for (int j = 0; j < num_cols; ++j)
        {
            board[row_idx][j] = row_perms[row_idx][i][j];
        }

        // recursively search next row
        solve_helper(row_idx + 1, solution);
    }
}

/**
 * This function will solve the board, and store the solution
 * into board[][] directly.
 *
 * Note that you should first clear the board in case
 * the user has already filled some cells.
 */
void solve()
{
    // clear the board
    for (int i = 0; i < num_rows; ++i)
    {
        for (int j = 0; j < num_cols; ++j)
        {
            board[i][j] = '.';
        }
    }

    // solve the puzzle
    char solution[MAX_ROW][MAX_COL];
    solve_helper(0, solution);
    // copy solution back to board
    for (int i = 0; i < num_rows; ++i)
    {
        for (int j = 0; j < num_cols; ++j)
        {
            board[i][j] = solution[i][j];
        }
    }
}

int main()
{
    get_input_board();
    print_board();

    while (true)
    {
        cout << "===== Welcome to Nonogram Game =====" << endl;
        cout << "Please enter your choice:" << endl;
        cout << "Enter 'p' to print the current board." << endl;
        cout << "Enter 'm' to modify a cell." << endl;
        cout << "Enter 'c' to check your solution." << endl;
        cout << "Enter 's' to invoke solver." << endl;
        cout << "Enter 'q' to quit." << endl;
        cout << "Your choice: ";
        char input;
        cin >> input;
        if (input == 'p')
        {
            print_board();
        }
        else if (input == 'm')
        {
            user_operate_board();
        }
        else if (input == 'c')
        {
            if (check_whole_board_valid())
            {
                cout << "Congratulations! Your solution is correct!" << endl;
            }
            else
            {
                cout << "Ahh, your solution is incorrect, try again." << endl;
            }
        }
        else if (input == 's')
        {
            cout << "Generating solution:" << endl;
            solve();
            print_board();
        }
        else if (input == 'q')
        {
            cout << "Bye!" << endl;
            break;
        }
        else
        {
            cout << "Invalid input. Try again." << endl;
        }
    }
    return 0;
}
