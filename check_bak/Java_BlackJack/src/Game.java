import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

public class Game {
    private static final Object STOP_RECEIVE_CARD = "0";
	public void play() {
		System.out.println("========= Blackjack =========");
		Scanner sc = new Scanner(System.in);
		Rule rule = new Rule();
        CardDeck cardDeck = new CardDeck();

        List<Player> players = Arrays.asList(new Gamer("User1"), new Dealer());
        List<Player> initAfterPlayers = initPhase(cardDeck, players);
        List<Player> playingAfterPlayers = playingPhase(sc, cardDeck, initAfterPlayers);

        Player winner = rule.getWinner(playingAfterPlayers);
        System.out.println("Winner is " + winner.getName());
	}

	private List<Player> playingPhase(Scanner sc, CardDeck cardDeck, List<Player> players) {
        List<Player> cardReceivedPlayers;
        while(true){
            cardReceivedPlayers = receiveCardAllPlayers(sc, cardDeck, players);

            if(isAllPlayerTurnOff(cardReceivedPlayers)){
                break;
            }
        }
        return cardReceivedPlayers;
    }

	private List<Player> receiveCardAllPlayers(Scanner sc, CardDeck cardDeck, List<Player> players) {
        for(Player player : players) {
            System.out.println(player.getName()+"'s turn.");

            if(isReceiveCard(sc)) {
                Card card = cardDeck.draw();
                player.receiveCard(card);
                player.turnOn();
            }else{
                player.turnOff();
            }
        }
        return players;
    }     
	
	private boolean isAllPlayerTurnOff(List<Player> players){
        for(Player player : players) {
            if(player.isTurn()) {
                return false;
            }
        }
        return true;
    }
	
    private boolean isReceiveCard(Scanner sc) {
        System.out.println("Do you want to draw cards? Enter 0 to exit.");
        return !STOP_RECEIVE_CARD.equals(sc.nextLine());
    }

	private static final int INIT_RECEIVE_CARD_COUNT = 2;

	private List<Player> initPhase(CardDeck cardDeck, List<Player> players){
        System.out.println("Draw two cards.");
        for(int i = 0; i < INIT_RECEIVE_CARD_COUNT; i++) {
            for(Player player : players) {
            	System.out.println(player.getName()+"'s turn.");
                Card card = cardDeck.draw();
                player.receiveCard(card);
            }
        }
        return players;
    }
}