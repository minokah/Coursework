public class ExtendedLetter extends Letter {
    private String content;
    private int family;
    private boolean related;
    private int SINGLETON = -1;

    public ExtendedLetter(String s) {
        super(' ');
        content = s;
        related = false;
        family = SINGLETON;
    }

    public ExtendedLetter(String s, int fam) {
        super(' ');
        content = s;
        related = false;
        family = fam;
    }

    public boolean equals(Object other) {
        if (!(other instanceof ExtendedLetter)) return false;
        else {
            if (family == ((ExtendedLetter)other).family) related = true;
            if (content == ((ExtendedLetter)other).content) return true;
            else return false;
        }
    }

    public String toString() {
        if (related && this.decorator().equals("-")) return "." + content + ".";
        return this.decorator() + content + this.decorator();
    }

    public static Letter[] fromStrings(String[] content, int[] codes) {
        Letter[] letters = new Letter[content.length];

        if (codes == null) {
            for (int i = 0; i != content.length; i++) letters[i] = new ExtendedLetter(content[i]);
        }
        else {
            for (int i = 0; i != content.length; i++) letters[i] = new ExtendedLetter(content[i], codes[i]);
        }

        return letters;
    }
}
