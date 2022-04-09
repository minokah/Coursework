/*
    Assignment 2

    Layout class, stores the board layout data and score
*/

public class Layout {
    private String layout;
    private int score;

    public Layout(String boardLayout, int score) {
        layout = boardLayout;
        this.score = score;
    }

    public String getBoardLayout() {
        return layout;
    }

    public int getScore() {
        return score;
    }
}
