import java.io.*;
import java.util.*;

public class VertexCoverReduction {
    public static void main(String[] args) throws FileNotFoundException {
        // store commands from command line
            int k = Integer.parseInt(args[1]);
            Scanner input = new Scanner(new File(args[0]));
            Graph graph = new Graph();
        
        // read input from the file to make a graph
        while (input.hasNextLine()) {
            String line = input.nextLine();
            System.out.println( line ); // ********
            Scanner scnLine = new Scanner(line);
            Integer u = new Integer(scnLine.nextInt());
            Integer v = new Integer(scnLine.nextInt());
            if (!graph.contains(u)) {
                graph.addVertex(u);
            }
            if (!graph.contains(v)) {
                graph.addVertex(v);
            }
            Edge e = new Edge();
            e.setStart(u.intValue());
            e.setEnd(v.intValue());
            graph.addEdge(e);
        }
        System.out.println( "Vertices: " + graph.printVertices()); // ******
        System.out.println( "Edges: " + graph.printEdges() ); // *******
        input.close();
        
        // create a file that contains the vertices of the compliment graph
        String outputName = "ComplimentGraph.txt";
        PrintStream output = new PrintStream( new File(outputName) );
        output.print( createCompliment( graph ) );
        
        int kPrime = graph.vertices.size() - k;
        System.out.println( "k' = " + kPrime );
        if ( kPrime == 0 ) {
            System.out.println( "yes" );
        } else if ( kPrime < 0 ) {
            System.out.println( "no" );
        } else {
            CliqueDecision.decide( outputName, kPrime );
        }
    }
    
    public static String createCompliment( Graph g ) {
        // create an adjacency matrix object
        AdjacencyMatrix m = new AdjacencyMatrix( g.vertices.size() );
        System.out.println( "Initial Matrix" ); // *****
        m.printMatrix(); // *****
        
        int size = g.edges.size();
        // traverse through the edge list of g
        for (Edge edge: g.edges) {
            m.update( edge.getStart(), edge.getEnd() );
        }
        m.printMatrix();
        
        return m.getCompliment();
    }
}
