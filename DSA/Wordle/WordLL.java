public class WordLL {
    private Word mysteryWord;
    private LinearNode<Word> history;

    public WordLL(Word mystery) {
        mysteryWord = mystery;
        history = new LinearNode<Word>();
    }

    public boolean tryWord(Word guess) {
        boolean result = guess.labelWord(mysteryWord);
        if (history.getElement() == null) history = new LinearNode<Word>(guess);
        else {
            LinearNode<Word> current = history;
            while (current != null) {
                if (current.getNext() == null) {
                    LinearNode<Word> newWord = new LinearNode<Word>(guess);
                    current.setNext(newWord);
                    break;
                }
                current = current.getNext();
            }
        }
        return result;
    }

    public String toString() {
        String s = "";
        LinearNode<Word> current = history;
        while (current != null) {
            s += current.getElement().toString();
            if (current.getNext() != null) s += "\n";
            current = current.getNext();
        }
        return s;
    }
}
