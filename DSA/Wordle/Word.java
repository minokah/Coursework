public class Word {
    private LinearNode<Letter> firstLetter;
    public Word(Letter[] letters) {
        LinearNode<Letter> current = null;
        for (int i = 0; i != letters.length; i++) {
            if (i == 0) {
                firstLetter = new LinearNode<Letter>(letters[i]);
                current = firstLetter;
            }
            else {
                LinearNode<Letter> newNode = new LinearNode(letters[i]);
                current.setNext(newNode);
                current = newNode;
            }
        }
    }

    public String toString() {
        String s = "Word:";
        LinearNode<Letter> current = firstLetter;

        while (current != null) {
            s += " " + current.getElement().toString();
            current = current.getNext();
        }

        return s + " ";
    }

    public boolean labelWord(Word mystery) {
        LinearNode<Letter> mysteryCurrent = mystery.firstLetter;
        LinearNode<Letter> current = firstLetter;
        boolean equal = true;

        int currentIndex = 0;
        int mysteryIndex = 0;

        while (current != null) {
            boolean found = false;
            while (mysteryCurrent != null) {
                if (current.getElement().equals(mysteryCurrent.getElement())) {
                    current.getElement().setUsed();
                    found = true;

                    if (mysteryIndex == currentIndex) current.getElement().setCorrect();
                    break;
                }
                else current.getElement().setUnused();
                mysteryCurrent = mysteryCurrent.getNext();
                mysteryIndex++;
            }
            mysteryCurrent = mystery.firstLetter;
            current = current.getNext();

            mysteryIndex = 0;
            currentIndex++;

            if (!found) equal = false;
        }

        return equal;
    }
}
