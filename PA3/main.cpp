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
 **********  Task 1  **********
 *
 * This function reads the board size and constraints from user input,
 * store the information in global variables and init the board.
 *
 * Don't forget to check whether row/column constraints are valid.
 * Please refer to webpage for a detailed description.
 *
 * For simplicity, we assume the user's other inputs are always valid.
 */
void get_input_board()
{
    // START YOUR CODES HERE
    cout << "Enter the number of rows: ";
    cin >> num_rows;
    cout << "Enter the number of columns: ";
    cin >> num_cols;

    bool valid;
    int temp, curSum, index;

    for (int i = 0; i < num_rows; ++i)
    {
        for (int j = 0; j < num_cols; ++j)
        {
            board[i][j] = '.';
        }
    }

    for (int i = 0; i < num_rows; ++i)
    {
        do
        {
            valid = true;
            cout << "Enter the number of constraints for row " << i << " (end with -1): ";
            index = 0;
            curSum = 0;
            cin >> temp;
            while (temp != -1)
            {
                row_constraints[i][index] = temp;
                ++index;
                curSum += (temp + 1);
                if (curSum - 1 > num_cols)
                {
                    valid = false;
                    cout << "Invalid row constraint, please try again." << endl;
                    do
                    {
                        cin >> temp;
                    } while (temp != -1);
                    break;
                }
                cin >> temp;
            }
        } while (!valid);
        num_row_constraints[i] = index;
    }

    for (int i = 0; i < num_cols; ++i)
    {
        do
        {
            valid = true;
            cout << "Enter the number of constraints for column " << i << " (end with -1): ";
            index = 0;
            curSum = 0;
            cin >> temp;
            while (temp != -1)
            {
                col_constraints[i][index] = temp;
                ++index;
                curSum += (temp + 1);
                if (curSum - 1 > num_rows)
                {
                    valid = false;
                    cout << "Invalid column constraint, please try again." << endl;
                    do
                    {
                        cin >> temp;
                    } while (temp != -1);
                    break;
                }
                cin >> temp;
            }
        } while (!valid);
        num_col_constraints[i] = index;
    }
    // END YOUR CODES HERE
}

/**
 **********  Task 2  **********
 *
 * This function prints the board and the constraints for each row and column.
 * Constraints should be printed on the bottom of each column and on the left of each row.
 *
 * You may find an example in webpage.
 */
void print_board()
{
    // START YOUR CODES HERE
    int maxRowConstraint = 0;
    int maxColConstraint = 0;

    // find the leading space for line one
    for (int i = 0; i < num_rows; ++i)
    {
        maxRowConstraint = maxRowConstraint > num_row_constraints[i] ? maxRowConstraint : num_row_constraints[i];
    }

    // find the max row for col constraints
    for (int i = 0; i < num_cols; ++i)
    {
        maxColConstraint = maxColConstraint > num_col_constraints[i] ? maxColConstraint : num_col_constraints[i];
    }

    // print line one
    for (int i = 0; i < maxRowConstraint; ++i)
    {
        cout << "  ";
    }
    cout << "     ";
    for (int i = 0; i < num_cols; i++)
    {
        cout << static_cast<char>('A' + i) << " ";
    }
    cout << endl;

    // print rows with dots and X
    for (int i = 0; i < num_rows; i++)
    {
        int j = num_row_constraints[i];
        for (int k = maxRowConstraint - j; k > 0; --k)
        {
            cout << "  ";
        }
        for (int k = 0; k < j; ++k)
        {
            cout << row_constraints[i][k] << " ";
        }
        cout << "| ";
        if (i < 10)
            cout << " " << i;
        else
            cout << i;
        for (int j = 0; j < num_cols; ++j)
        {
            cout << " " << board[i][j];
        }
        cout << endl;
    }

    // print col constraints
    for (int i = 0; i < maxColConstraint; ++i)
    {
        for (int i = 0; i < maxRowConstraint; ++i)
        {
            cout << "  ";
        }
        cout << "     ";
        for (int j = 0; j < num_cols; ++j)
        {
            if (col_constraints[j][i] == 0)
                cout << "  ";
            else
                cout << col_constraints[j][i] << " ";
        }
        cout << endl;
    }
    cout << endl;
    // END YOUR CODES HERE
}

/********************************
 * Part 2: User play on board   *
 ********************************/

/**
 **********  Task 3  **********
 *
 * This function will be called when user choose to set/unset a cell.
 *
 * You need to:
 * 1. Ask user to input which cell he/she wants to modify
 * 2. Check if the user input is a valid cell(i.e., it is within the board)
 *    If invalid, keep asking the user to input.
 * 3. Set/unset the cell
 * 4. Print the new board
 */
void user_operate_board()
{
    // START YOUR CODES HERE
    bool valid;
    char col;
    int row;
    do
    {
        valid = true;
        cout << "Enter the cell you want to modify (e.g. A 2): ";
        cin >> col >> row;
        if (col - 'A' >= num_cols || row >= num_rows)
        {
            cout << "Invalid row or column. Try again." << endl;
            valid = false;
        }
    } while (!valid);
    if (board[row][col - 'A'] == '.')
    {
        board[row][col - 'A'] = 'X';
    }
    else
    {
        board[row][col - 'A'] = '.';
    }

    // END YOUR CODES HERE
}

/**
 **********  Task 4  **********
 *
 * This function will be called after user finish filling the whole board.
 * You need to check whether his/her solution is correct,
 * i.e., follow all constraints.
 *
 * Return true if his/her solution is correct; false otherwise.
 */
bool check_whole_board_valid()
{
    // START YOUR CODES HERE
    // check row
    int curSet;
    int num;
    bool flag = false;
    for (int i = 0; i < num_rows; ++i)
    {
        curSet = 0;
        num = 0;
        for (int j = 0; j < num_cols; ++j)
        {
            if (board[i][j] == '.') {
                ++num;
                flag = true;
            }
                
            else
            {   
                if(!flag && j != 0) return false;
                for (int k = 0; k < row_constraints[i][curSet]; ++k)
                {
                    if (board[i][j] != 'X')
                        return false;
                    ++j;
                }
                --j;
                ++curSet;
                flag = false;
            }
        }
        for (int k = 0; k < num_row_constraints[i]; ++k)
        {
            num += row_constraints[i][k];
        }
        if (num != num_cols)
            return false;
    }

    // check column
    for (int i = 0; i < num_cols; ++i)
    {
        curSet = 0;
        num = 0;
        for (int j = 0; j < num_rows; ++j)
        {
            if (board[j][i] == '.'){
                ++num;
                flag = true;
            }
                
            else
            {   
                if(!flag && j != 0) return false;
                for (int k = 0; k < col_constraints[i][curSet]; ++k)
                {
                    if (board[j][i] != 'X')
                        return false;
                    ++j;
                }
                --j;
                ++curSet;
                flag = false;
            }
        }
        for (int k = 0; k < num_col_constraints[i]; ++k)
        {
            num += col_constraints[i][k];
        }
        if (num != num_rows)
            return false;
    }
    return true;
    // END YOUR CODES HERE
}

/*******************************
 * Part 3: A nonogram solver   *
 *******************************/

/**
 **********  Task 5  **********
 *
 * This function coverts a position vector of a row into a real row.
 *
 * For example, if num_col = 8, position vector = {0, 3, 5},
 * row_constraint for this row is {2, 1, 1}, then the row should be:
 * [X X . X . X . .]
 * It will store the result row in variable 'result_row[]'
 *
 * You may assume the parameters given are always valid.
 */
void positions_to_row(int row_idx, const int positions[], int num_pos, char result_row[])
{
    // START YOUR CODES HERE
    int position;
    int start = 0;
    for (int i = 0; i < num_pos; ++i)
    {
        position = positions[i];
        for (; start < position; ++start)
        {
            result_row[start] = '.';
        }
        for (int j = 0; j < row_constraints[row_idx][i]; ++j)
        {
            result_row[start] = 'X';
            ++start;
        }
    }
    while (start < num_cols)
    {
        result_row[start] = '.';
        ++start;
    }
    // END YOUR CODES HERE
}

/**
 **********  Task 6  **********
 *
 * This function checks if the given block on given row can be shifted right
 * for one cell.
 * For example, if the position vector is {0, 4, 6}, num_col = 8
 * then the row is: [X X . . X . X .], and there are 3 blocks
 * then block_can_shift(row_idx, 0, {0, 4, 6}) = true,
 *      block_can_shift(row_idx, 1, {0, 4, 6}) = false, since it will hit 2nd block after shift
 *      block_can_shift(row_idx, 2, {0, 4, 6}) = true.
 *
 * More details can be found on webpage.
 */
bool block_can_shift(int row_idx, int block_idx, const int positions[], int num_pos)
{
    // START YOUR CODES HERE
    if (block_idx == num_pos - 1)
    {
        if (positions[block_idx] + row_constraints[row_idx][block_idx] < num_cols)
            return true;
        else
            return false;
    }

    int interval = positions[block_idx + 1] - positions[block_idx] - row_constraints[row_idx][block_idx];
    if (interval < 2)
        return false;
    else
        return true;
    // END YOUR CODES HERE
}

// this array stores all valid permutations for each row
// it's expected that num of permutations for each row should not exceed 2^(num_cols)
const int MAX_PERM = 1 << MAX_COL; // A way to calculate 2^(num_cols)
char row_perms[MAX_ROW][MAX_PERM][MAX_COL];
// this array stores num of permutations for each row
int num_row_perms[MAX_ROW] = {0};

/**
 **********  Task 7  **********
 *
 * This function gets all valid permutations for a row,
 * and store all results in global variables 'row_perms' and 'num_row_perms'.
 *
 * Hint: You may use recursion to do that, with the help of helper functions
 * 'block_can_shift()', etc.
 *
 */
void generate_perms(int row_idx, int block_idx, int positions[])
{
    if (block_idx < 0)
    {
        return;
    }

    while (block_can_shift(row_idx, block_idx, positions, num_row_constraints[row_idx]))
    {
        positions[block_idx] += 1;
        positions_to_row(row_idx, positions, num_row_constraints[row_idx], row_perms[row_idx][num_row_perms[row_idx]++]);
        generate_perms(row_idx, block_idx - 1, positions);
    }
    positions[0] = 0;
    for (int i = 1; i <= block_idx; ++i)
    {
        positions[i] = positions[i - 1] + row_constraints[row_idx][i - 1] + 1;
    }
}

void get_row_perms(int row_idx)
{
    // START YOUR CODES HERE
    // generate starter
    int index = 0;
    int positions[num_row_constraints[row_idx]];
    for (int i = 0; i < num_row_constraints[row_idx]; ++i)
    {
        positions[i] = index;
        for (int j = 0; j < row_constraints[row_idx][i]; ++j)
        {
            row_perms[row_idx][0][index] = 'X';
            ++index;
        }
        row_perms[row_idx][0][index++] = '.';
    }

    while (index < num_cols)
    {
        row_perms[row_idx][0][index++] = '.';
    }
    ++num_row_perms[row_idx];

    generate_perms(row_idx, num_row_constraints[row_idx] - 1, positions);
    // END YOUR CODES HERE
}

/**
 **********  Task 8  **********
 *
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
    // START YOUR CODES HERE    
    int curSet;
    int num;
    bool flag = false;
    int k; 
    for (int i = 0; i < num_cols; ++i)
    {
        curSet = 0;
        num = 0;
        for (int j = 0; j < num_complete_rows; ++j)
        {
            if (board[j][i] == '.') {
                flag = true;
            }
            else
            {   
                if(!flag && j != 0) return false;
                k = 0;
                for (;j < num_complete_rows && k < col_constraints[i][curSet]; ++k,++j)
                {
                    if (board[j][i] != 'X')
                        return false;
                }
                ++curSet;
                flag = false;
                --j;
            }
        }
        num = num_complete_rows;
        --curSet;
        while(k < col_constraints[i][curSet]) {
            ++k;
            ++num;
        }
        ++curSet;
        while (curSet < num_col_constraints[i])
        {
            num += (col_constraints[i][curSet]+1);
            ++curSet;
        }
        if(num > num_rows +1) return false;
    }
    return true;
    // END YOUR CODES HERE

}

/**
 **********  Task 9  **********
 *
 * This function will solve the board, and store the solution
 * into board[][] directly.
 *
 * Hint: You may use recursion to do that, with the help of helper functions
 * 'get_row_perms()', ‘check_rows_valid()’, etc.
 */
void solve()
{
    // START YOUR CODES HERE
    int row_idx = 0;
    bool flag;
    int iteration[num_rows] = {0};
    get_row_perms(0);
    while (row_idx != num_rows)
    {
        flag = false;
        if (!num_row_perms[row_idx])
        {   
            get_row_perms(row_idx);
        }
        for (; iteration[row_idx] < num_row_perms[row_idx]; ++iteration[row_idx])
        {
            for (int j = 0; j < num_cols; ++j)
            {
                board[row_idx][j] = row_perms[row_idx][iteration[row_idx]][j];
            }
            if (check_rows_valid(row_idx + 1))
            {   
                ++row_idx;
                flag = true;
                break;
            }
        }
        if (!flag)
        {
            iteration[row_idx] = 0;
            --row_idx;
            while (iteration[row_idx] == num_row_perms[row_idx] - 1 && row_idx >= 0)
            {
                iteration[row_idx] = 0;
                --row_idx;
            }
            if (row_idx < 0){
                return;
            }
            ++iteration[row_idx];
        }
    }
    // END YOUR CODES HERE
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
