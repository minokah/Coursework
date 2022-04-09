/*
    Assignment 2

    Board class, the actual game
*/

public class Board implements BoardADT {
    private char[][] theBoard;
    private int empty;
    private int max;

    public Board(int board_size, int empty_positions, int max_levels) { // create a new board and populate with 'e'
        empty = empty_positions;
        max = max_levels;
        theBoard = new char[board_size][board_size];
        for (int row = 0; row != board_size; row++) {
            for (int col = 0; col != board_size; col++) {
                theBoard[row][col] = 'e';
            }
        }
    }

    public Dictionary makeDictionary() { return new Dictionary(9973); } // create a dictionary

    public void saveTile(int row, int col, char symbol) { // set a tile to a character
        theBoard[row][col] = symbol;
        //System.out.println(convertLayout());
    }

    public boolean isDraw(char player, int empty_positions) { // check for draw or not
        String layout = convertLayout();

        int empty_counter = 0;
        for (int i = 0; i != layout.length(); i++) {
            if (layout.charAt(i) == 'e') empty_counter += 1;
        }

        // this will check each of the 8 spots surrounding an empty spot
        // will throw an out of bounds exception if the spot doesn't exist, but the try/catch will handle it
        //if (!winner(player)) {
        if (empty_positions == 0 && empty_counter == 0) return true;

        boolean adjacent = false;

        for (int row = 0; row != theBoard.length; row++) {
            for (int col = 0; col != theBoard.length; col++) {
                if (theBoard[row][col] == 'e') {
                    try {
                        if (theBoard[row - 1][col] == player) { // up
                            adjacent = true;
                            break;
                        }
                    }
                    catch (Exception e) {}

                    try {
                        if (theBoard[row + 1][col] == player) { // down
                            adjacent = true;
                            break;
                        }
                    }
                    catch (Exception e) {}
                    try {
                        if (theBoard[row][col - 1] == player) { // left
                            adjacent = true;
                            break;
                        }
                    }
                    catch (Exception e) {}
                    try {
                        if (theBoard[row][col + 1] == player) { // right
                            adjacent = true;
                            break;
                        }
                    }
                    catch (Exception e) {}

                    try {
                        if (theBoard[row - 1][col - 1] == player) { // top left
                            adjacent = true;
                            break;
                        }
                    }
                    catch (Exception e) {}

                    try {
                        if (theBoard[row + 1][col - 1] == player) { // bottom left
                            adjacent = true;
                            break;
                        }
                    }
                    catch (Exception e) {}

                    try {
                        if (theBoard[row - 1][col + 1] == player) { // top right
                            adjacent = true;
                            break;
                        }
                    }
                    catch (Exception e) {}

                    try {
                        if (theBoard[row + 1][col + 1] == player) { // bottom right
                            adjacent = true;
                            break;
                        }
                    }
                    catch (Exception e) {}
                }
            }
        }

        if (empty_positions > 0 && empty_positions == empty_counter && !adjacent) return true;
        //}

        return false;
    }

    public boolean winner(char symbol) { // check if symbol is n in a row
        // check each column
        for (int row = 0; row != theBoard.length; row++) {
            int counter = 0;
            for (int col = 0; col != theBoard.length; col++) {
                if (theBoard[row][col] == symbol) {
                    counter++;
                    continue;
                }
                else break;
            }

            if (counter == theBoard.length) return true;
        }

        // check each row
        for (int row = 0; row != theBoard.length; row++) {
            int counter = 0;
            for (int col = 0; col != theBoard.length; col++) {
                if (theBoard[col][row] == symbol) {
                    counter++;
                    continue;
                }
                else break;
            }

            if (counter == theBoard.length) return true;
        }

        // top left to bottom right
        int diagonal = 0;
        boolean won = true;
        for (int row = 0; row != theBoard.length; row++) {
            if (theBoard[row][diagonal] != symbol) {
                won = false;
                break;
            }
            diagonal++;
        }

        if (won) return true;

        // top right to bottom left
        won = true;
        diagonal = theBoard.length - 1;
        for (int row = 0; row != theBoard.length; row++) {
            if (theBoard[row][diagonal] != symbol) {
                won = false;
                break;
            }
            diagonal--;
        }

        if (won) return true;

        return false;
    }

    public int evaluate(char symbol, int empty_positions) { // check for winners or draw
        if (winner('h')) return 0;
        if (isDraw(symbol, empty_positions)) return 2;
        if (winner('c')) return 3;

        return 1;
    }

    private String convertLayout() { // creates a String from the board layout (top to bottom then left to right)
        String a = "";
        for (int row = 0; row != theBoard.length; row++) {
            for (int col = 0; col != theBoard.length; col++) {
                a += theBoard[row][col];
            }
        }

        return a;
    }

    public int repeatedLayout(Dictionary dict) { // get the score
        int value = dict.getScore(convertLayout());
        if (value == -1) return -1;
        else return value;
    }

    public void storeLayout(Dictionary dict, int score) { // store the score and layout
        try { dict.put(new Layout(convertLayout(), score)); }
        catch (DictionaryException e) { System.out.println("Failed to put data into dictionary!"); }
    }

    public boolean positionIsEmpty(int row, int column) { // check if position is empty
        if (theBoard[row][column] == 'e') return true;
        return false;
    }

    public boolean isComputerTile(int row, int column) { // check if position belongs to computer
        if (theBoard[row][column] == 'c') return true;
        return false;
    }

    public boolean isHumanTile(int row, int column) { // check if position belongs to human
        if (theBoard[row][column] == 'h') return true;
        return false;
    }
}
