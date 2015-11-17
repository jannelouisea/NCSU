import java.io.*;
import java.util.*;

public class VertexCoverReduction {
    public static void main(String[] args) {
        // store commands from command line
        int k = Integer.parseInt(args[1]);
        Scanner input = new Scanner(new File(args[0]));
        Graph graph = new Graph();
        
        // read input from the file to make a graph
        while (input.hasNextLine()) {
            String line = input.nextLine();
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
        input.close();
        
        // create a file that contains the vertices of the compliment graph
        // String complimentGraph = createCompliment( graph )
        // Print to outputFile
        // Calculate k'
        // Call CliqueDecision.decide( outputFileNAME, k' );
        // System.out.println( CliqueDecision.decide( outputFileNAME, k' ) ); ??
        // print either yes or no depending on what is returned from previous call
    }
}

public String createCompliment( Graph g ) {
    // create an adjacency matrix object
    // AdjacencyMatrix m = new AdjacencyMatrix( g.numOfVertices );

    // traverse through the edge list of g

    // while traversing
    // m.update( edge.start, edge.end );

    // return compliment graph
    // return m.getCompliment();
}
