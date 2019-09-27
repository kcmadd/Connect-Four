#include <stdio.h>

//initializes and prints board
void initBoard(char board[7][8]); //Initializes board
void printBoard(char board[7][8]); //printsBoard

//controls playerOne movements
void gameMechanics(char board[7][8], char playerOne, char playerTwo);

//check general conditions for winning
int checkFull(char board[7][8]);// checks if board is Full
int checkWin(char board[7][8], char tokenChanger); //checks if a player has won

//Ai methods
void aiMechanics(char board[7][8], int row, int col, char playerOne, char playerTwo); //controls basic AI movements
int aiThreeInRow(char board[7][8], char playerOne);//if playerOne has three in row, AI will try to block
void aiFirst(char board[7][8], char playerTwo, char playerOne); //if AI goes first


void initBoard(char board[7][8]) //Initializes board
{
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 8; j++) {
            char c = 'A' + j - 1; //As loop iterates, char c = 'A' through 'G'
            if (j == 0 && i == 0) //Empty Space at (0, 0)
                board[i][j] = ' ';
            else if (i == 0)
                board[i][j] = c; //initializes column names (A-G)
            else if (j == 0)
                board[i][j] = 7 - i; //Initializes row numbers
            else
                board[i][j] = '.'; //Everything else is '.'
        }
    }
}

void printBoard(char board[7][8]) //Prints Board
{
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 8; j++) {
            if (i != 0 && j == 0)
                printf(" %d ", board[i][j]);
            else
                printf(" %c ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void gameMechanics(char board[7][8], char playerOne,
                   char playerTwo)//Manages game mechanics such as token placement and turns
{
    printBoard(board);
    char columnLetter;
    int letterConversion;
    while (checkWin(board, playerTwo) == 0) //checks that neither player has won
    {
        if (checkWin(board, playerOne) == 1) {
            printf("\033[0;32m"); //Changing colour to Green
            printf("Congratualations, You Win!");
            printf("\033[0;0m");//Resets colour
            break;
        }

        if (checkFull(board) == 0) { //if board isn't full
            checkFull(board);
            printf("Choose a column between A and G to place your token: ");
            scanf(" %c", &columnLetter);
            printf("\n");
            letterConversion = (columnLetter - 'A') + 1;//Converts letter input to int
            while (letterConversion < 1 || letterConversion > 7) //Checking for valid Column letter
            {
                printf("\033[0;31m"); //sets colour to red
                printf("Please select a valid column letter\n");
                printf("\033[0m");//resets colour
                printf("Enter a letter between A and G: ");
                scanf(" %c", &columnLetter);
                letterConversion = (columnLetter - 'A') + 1;
            }

            while (board[1][letterConversion] != '.') //checks is top entry of column is '.' and not a player token
            {
                printf("\033[0;31m"); //sets colour to red
                printf("Column is full, please select a column that isn't full\n");
                printf("\033[0m");//resets colour
                printf("Enter a column between A and G that isn't full: ");
                scanf(" %c", &columnLetter);
                printf("\n");
                letterConversion = (columnLetter - 'A') + 1;

            }
            letterConversion = (columnLetter - 'A') + 1;
            if (board[6][letterConversion] == '.') //if column is empty
            {
                board[6][letterConversion] = playerOne; // p1 places token in row #1
                aiMechanics(board, 5, letterConversion, playerOne, playerTwo);//5 = row #2

            } else {
                for (int k = 1; k < 7; k++) { //iterates down the column for the last '.'
                    if (board[k][letterConversion] == 'X' || board[k][letterConversion] == 'O') {
                        board[k - 1][letterConversion] = playerOne;
                        aiMechanics(board, k - 2, letterConversion, playerOne, playerTwo);
                        break;
                    }
                }
            }
            printBoard(board);
        } else {
            printf("\033[0;33m");
            printf("It's a draw, Game Over");
            printf("\033[0;0m");
            break;
        }
    }
    if (checkWin(board, playerOne) == 1) {

        printf("\033[0;32m"); //Changing colour to Green
        printf("Congratualations, You Win!");
        printf("\033[0;0m");//Resets colour
    } else if (checkWin(board, playerTwo) == 1) {

        printf("\033[0;36m"); //Changing colour to cyan
        printf("AI Wins");
        printf("\033[0;0m");//Resets colour
    }
}

void aiFirst(char board[7][8], char playerTwo, char playerOne) { //if AI goes first
    board[6][6] = playerTwo;
    gameMechanics(board, playerOne, playerTwo);
}

void aiMechanics(char board[7][8], int row, int col, char playerOne, char playerTwo) {
    if (aiThreeInRow(board, playerOne) == 0) {
        if (board[6][col] == '.') {
            board[6][col] = playerTwo; // p2 places token in row #1
            checkWin(board, playerTwo);
        } else if (board[1][col] == '.') {
            for (int k = 1; k < 7; k++) { //iterates down the column for the last '.'
                if (board[k][col] == 'X' || board[k][col] == 'O') {
                    board[k - 1][col] = playerTwo;
                    checkWin(board, playerTwo);
                    break;
                }
            }
        } else {
            for (int i = 1; i < 8; i++) {
                if (board[1][i] == '.') {
                    for (int k = 6; k > 0; k--) { //iterates down the column for the last '.'
                        if (board[k][i] == '.') {
                            board[k][i] = playerTwo;
                            checkWin(board, playerTwo);
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
    if (aiThreeInRow(board, playerOne) == 1) {
        if (col < 7) {
            if (board[6][col + 1] == '.') {
                board[6][col + 1] = playerTwo; // p2 places token in row #1
                checkWin(board, playerTwo);
            } else if (board[1][col + 1] == '.') {
                for (int k = 1; k < 7; k++) { //iterates down the column for the last '.'
                    if (board[k][col + 1] == 'X' || board[k][col + 1] == 'O') {
                        board[k - 1][col + 1] = playerTwo;
                        checkWin(board, playerTwo);
                        break;
                    }
                }
            } else {
                for (int i = 1; i < 8; i++) {
                    if (board[1][i] == '.') {
                        for (int k = 6; k > 0; k--) { //iterates down the column for the last '.'
                            if (board[k][i] == '.') {
                                board[k][i] = playerTwo;
                                checkWin(board, playerTwo);
                                break;
                            }
                        }
                    }//
                    break;
                }
            }
        } else {
            if (board[6][col] == '.') {
                board[6][col] = playerTwo; // p2 places token in row #1
                checkWin(board, playerTwo);
            } else if (board[1][col] == '.') {
                for (int k = 1; k < 7; k++) { //iterates down the column for the last '.'
                    if (board[k][col] == 'X' || board[k][col + 1] == 'O') {
                        board[k - 1][col] = playerTwo;
                        checkWin(board, playerTwo);
                        break;
                    }
                }
            } else {
                for (int i = 1; i < 8; i++) {
                    if (board[1][i] == '.') {
                        for (int k = 6; k > 0; k--) { //iterates down the column for the last '.'
                            if (board[k][i] == '.') {
                                board[k][i] = playerTwo;
                                checkWin(board, playerTwo);
                                break;
                            }
                        }
                    }//
                    break;
                }
            }

        }
    }
}


int aiThreeInRow(char board[7][8], char playerOne) {
    for (int i = 1; i < 7; i++) //Checks Horizontal
    {
        for (int j = 1; j < 5; j++)
            if (board[i][j] == playerOne)
                if (board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2])
                    return 1;
    }
    for (int i = 1; i < 7; i++)//Checks Diagonal
    {
        for (int j = 1; j < 8; j++) {
            if (board[i][j] == playerOne) {
                if (board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2])
                    return 1;
                if (board[i][j] == board[i - 1][j + 1] && board[i][j] == board[i - 2][j + 2])
                    return 1;
            }
        }
    }
    return 0;
}

int checkWin(char board[7][8], char tokenChanger) //Checks if there is a winner
{
    for (int i = 1; i < 7; i++) //Checks Horizontal
    {
        for (int j = 1; j < 5; j++)
            if (board[i][j] == tokenChanger)
                if (board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] && board[i][j] == board[i][j + 3])
                    return 1;
    }

    for (int j = 1; j < 8; j++) //Checks Vertical
    {
        for (int i = 6; i >= 3; i--)
            if (board[i][j] == tokenChanger)
                if (board[i][j] == board[i - 1][j] && board[i][j] == board[i - 2][j] && board[i][j] == board[i - 3][j])
                    return 1;
    }

    for (int i = 1; i < 7; i++)//Checks Diagonal
    {
        for (int j = 1; j < 8; j++) {
            if (board[i][j] == tokenChanger) {
                if (board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] &&
                    board[i][j] == board[i + 3][j + 3])
                    return 1;
                if (board[i][j] == board[i - 1][j + 1] && board[i][j] == board[i - 2][j + 2] &&
                    board[i][j] == board[i - 3][j + 3])
                    return 1;
            }
        }
    }
    return 0;
}

int checkFull(char board[7][8])// Checks if board is full
{
    for (int i = 1; i <
                    8; i++) //iterates through the second row from top (row under column Letters) to see if any coordinates contains '.'
    {
        if (board[1][i] == '.')
            return 0;
    }
    return 1;
}

int main() {
    char playerOne, playerTwo;
    char board[7][8];
    initBoard(board);
    printf("\033[0;36m"); //sets colour to cyan
    printf("\nWelcome to Connect Four!\n\n");
    printf("\033[0;33m");//sets colour to yellow
    printf("**NOTE: All Inputs Should be Capitalized**\n\n");
    printf("\033[0m");//resets colour
    printf("X goes first and O goes second\n");
    printf("Choose your token (X or O): ");
    scanf("%c", &playerOne);

    while (playerOne != 'X' && playerOne != 'O') //Checks for Valid Token
    {
        printf("\033[0;31m"); //sets colour to red
        printf("Please select a valid token\n");
        printf("\033[0m");//resets colour
        printf("Choose your token (X or O): \n");
        scanf(" %c", &playerOne);
    }
    if (playerOne == 'X') {
        playerTwo = 'O';
        gameMechanics(board, playerOne, playerTwo); //Helper method to manage token placement and turns
    } else {
        playerTwo = 'X';
        aiFirst(board, playerTwo, playerOne);
    }
    return 0;
}