import java.util.List;

public interface Player {
    void receiveCard(Card card);

    void showCards();

    List<Card> openCards();

    void turnOff();

    void turnOn();

    boolean isTurn();
    
    String getName();
}