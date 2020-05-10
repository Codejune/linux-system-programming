import java.util.List;

public class Rule {
    public Player getWinner(List<Player> players) {
        Player highestPlayer = null;
        int highestPoint = 0;

        for (Player player : players) {
            int playerPointSum = getPointSum(player.openCards());
            if (playerPointSum > highestPoint) {
                highestPlayer = player;
                highestPoint = playerPointSum;
            }
        }
        return highestPlayer;
    }

    private int getPointSum(List<Card> cards) {
        int sum = 0;

        for (Card card : cards) {
            sum += card.getPoint();
        }
        return sum;
    }
}