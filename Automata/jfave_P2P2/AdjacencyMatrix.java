public class AdjacencyMatrix {

    // varaible for the matrix's size
    private int size;
    // 2D array that will keep track of the edges in the normal graph of G
    // and it's compliment graph
    private int[][] adjMtrx;
    
    public AdjacencyMatrix( int size ) {
        this.size = size;
        adjMtrx = new int[this.size][this.size];
        // the lower triangle of the graph is the compliment graph
        for (int i = 1; i < this.size; i++) {
            for (int j = 0; j < i; j++) {
                adjMtrx[i][j] = 1;
            }
        }
    }
    
    public void update( int start, int end ) {
        adjMtrx[start][end] = 1; // updates normal graph
        adjMtrx[end][start] = 0; // upadtes compliment graph
    }
    
    public String getCompliment(Graph g) {
        String output = "";
        // only traverse the lower half of the triangle
        for (int i = 1; i < size; i++) {
            for (int j = 0; j < i; j++) {
                if (adjMtrx[i][j] == 1)
                    output += g.getValue(i) + " " + g.getValue(j) + "\n";
            }
        }
        return output;
    }
    
    public void printMatrix() {
        System.out.print("  ");
        for (int i = 0; i < size; i++) {
            System.out.print( i + " ");
        }
        System.out.println();
        System.out.print("  ");
        for (int i = 0; i < size; i++) {
            System.out.print( "- ");
        }
        System.out.println();
        for (int i = 0; i < size; i++) {
            System.out.print(i + "|");
            for (int j = 0; j < size; j++) {
                System.out.print( adjMtrx[i][j] + " " );
            }
            System.out.println();
        }
    }
    
}