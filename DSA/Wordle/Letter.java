public class Letter {
    private char letter;
    private int label;
    private int UNSET = 0;
    private int UNUSED = 1;
    private int USED = 2;
    private int CORRECT = 3;

    public Letter(char c) {
        label = UNSET;
        letter = c;
    }

    public boolean equals(Object otherObject) {
        if (otherObject.getClass() == this.getClass()) {
            if (((Letter)otherObject).letter == letter) return true;
        }
        return false;
    }

    public String decorator() {
        if (label == USED) return "+";
        else if (label == UNUSED) return "-";
        else if (label == CORRECT) return "!";
        else return " ";
    }

    public String toString() {
        return decorator() + letter + decorator();
    }

    public void setUnused() {
        label = UNUSED;
    }

    public void setUsed() {
        label = USED;
    }

    public void setCorrect() {
        label = CORRECT;
    }

    public boolean isUnused() {
        if (label == UNUSED) return true;
        else return false;
    }

    public static Letter[] fromString(String s) {
        Letter[] letters = new Letter[s.length()];
        for (int i = 0; i != s.length(); i++) letters[i] = new Letter(s.charAt(i));
        return letters;
    }
}
