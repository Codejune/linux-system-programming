import java.util.ArrayList;
import java.util.List;

public class Gamer implements Player {
    private List<Card> cards;
    private boolean turn;
    private String name;

    public Gamer(String name) {
        this.cards = new ArrayList<>();
        this.name = name;
    }

    public void receiveCard(Card card) {
        this.cards.add(card);
        this.showCards();
    }

    public void showCards(){
        StringBuilder sb = new StringBuilder();
        sb.append("List of cards currently owned\n");

        for(Card card : cards){
            sb.append(card.toString());
            sb.append("\n");
        }

        System.out.println(sb.toString());
    }
    
    public List<Card> openCards(){
        return this.cards;
    }
    
    @Override
    public void turnOff() {
        this.setTurn(false);
    }

    @Override
    public void turnOn() {
        this.setTurn(true);
    }

    @Override
    public boolean isTurn() {
        return this.turn;
    }
    
    @Override
    public String getName() {
        return this.name;
    }

    private void setTurn(boolean turn) {
        this.turn = turn;
    }
}