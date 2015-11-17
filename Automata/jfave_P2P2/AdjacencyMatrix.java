public class AdjacencyMatrix {

    private int size;
    private int[][] adjMtrx;
    
    public AdjacencyMatrix( int size ) {
        this.size = size;
        adjMtrx = new int[this.size][this.size];
        for (int i = 1; i < this.size; i++) {
            for (int j = 0; j < i; j++) {
                adjMtrx[i][j] = 1;
            }
        }
    }
    
    public void update( int start, int end ) {
        adjMtrx[start][end] = 1;
        adjMtrx[end][start] = 0;
    }
    
    public String getCompliment() {
        String output = "";
        for (int i = 1; i < size; i++) {
            for (int j = 0; j < i; j++) {
                if (adjMtrx[i][j] == 1)
                    output += i + " " + j + "\n";
            }
        }
    }
    
}