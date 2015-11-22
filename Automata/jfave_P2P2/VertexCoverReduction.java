import java.io.*;
import java.util.*;

public class VertexCoverReduction {
    public static void main(String[] args) throws FileNotFoundException {
        // store variables from command line
            int k = Integer.parseInt(args[1]);
            Scanner input = new Scanner(new File(args[0]));
            Graph graph = new Graph();
        int key = 0;
        
        // read input from the file to make a graph
        while (input.hasNextLine()) { // read through the file
            String line = input.nextLine();
            System.out.println( line ); // ********
            Scanner scnLine = new Scanner(line); // create a scanner for each line
            int u = scnLine.nextInt();
            int v = scnLine.nextInt();
            /**
            Integer u = new Integer(scnLine.nextInt());
            Integer v = new Integer(scnLine.nextInt());
             */
            // check to see if the graph already contains the new vertices
            if (!graph.contains(u)) {
                graph.addVertex(key, u);
                key++;
            }
            if (!graph.contains(v)) {
                graph.addVertex(key, v);
                key++;
            }
            Edge e = new Edge(); // create an edge
            // always set the lower value to edge's start value
            if ( u < v ) {
                e.setStart(u);
                e.setEnd(v);
            } else {
                e.setStart(v);
                e.setEnd(u);
            }
            graph.addEdge(e); // add edge to the graph's edges list
        }
        System.out.println( "Vertices: " );
        graph.printVertices(); // ******
        System.out.println( "Edges: " + graph.printEdges() ); // *******
        input.close();
        
        // create a file that contains the vertices of the compliment graph
        String outputName = "ComplimentGraph.txt";
        PrintStream output = new PrintStream( new File(outputName) );
        output.print( createCompliment( graph ) );
        
        // determine k' value
        int kPrime = graph.vertices.size() - k;
        System.out.println( "k' = " + kPrime );
        if ( kPrime == 0 ) { // if k = |V|
            System.out.println( "yes" );
        } else if ( kPrime < 0 ) { // if k > |V|
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
        
        // traverse through the edge list of g
        for (Edge edge: g.edges) {
            // update the adjacency matrix
            m.update( edge.getStart(), edge.getEnd() );
        }
        m.printMatrix(); // *******
        
        return m.getCompliment( g );
    }
}
