import java.io.*;
import java.util.*;

public class VertexCoverVerification {
    
    public static void verify( Graph g, ArrayList<Integer> vSet ) {
        int size = g.edges.size();
        // basically remove any edge that contains any integer from the vSet
        for (int v: vSet) {
            int[] removeList = new int[g.edges.size()];
            for (int i = 0; i < g.edges.size(); i++) {
                if (g.edges.get(i).contains(v)) {
                    removeList[i]++;
                }
            }
            for (int i = removeList.length - 1; i >= 0; i--) {
                if (removeList[i] == 1) {
                    g.removeEdge(g.edges.get(i));
                }
            }
        }
        // if g still has edges left then vSet is not a valid vertex cover
        if ( g.hasEdges() ) {
            System.out.println( "no" );
        } else {
            System.out.println( "yes" );
        }
    }
    
    public static void main(String[] args) throws FileNotFoundException {
        // store commands from command line
        int k = Integer.parseInt(args[2]);
        Scanner input = new Scanner(new File(args[0]));
        Scanner candidates = new Scanner(new File(args[1]));
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
        System.out.println( "Vertices: " + graph.printVertices()); // ******
        System.out.println( "Edges: " + graph.printEdges() ); // *******
        input.close();
        
        // start reading from the candidate solution file
        while (candidates.hasNextLine()) {
            int numOfVertices = 0;
            // create an array list to store the vertices
            ArrayList<Integer> vSet = new ArrayList<Integer>();
            Scanner line = new Scanner(candidates.nextLine());
            while ( line.hasNextInt() ) {
                vSet.add(line.nextInt());
                numOfVertices++;
            }
            if ( numOfVertices != k ) {
                System.out.println( "no" );
            } else {
                verify( graph, vSet );
            }
        }
        candidates.close();
    }
}
