public class VertexCoverVerification {
    public static void main(String[] args) {
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
        input.close();
        
        // start reading from the candidate solution file
        // while (candidates.hasNextLint()) {
            // create an ArrayList to contain the vertices
            // create a scanner for the line
            // add each int to the array list
            // if the size of the Arraylist != k print no
            // else determine if the vertices set is a vertex cover
                // System.out.println( verify( graph, ArrayList ) );
    }
    
    public String verify( Graph g, ArrayList vSet ) {
        // go through each vertices v in vSet
            // remove any vertices in g that contain v
        // if the number of edges left == 0
            // return "yes"
        // else return "no"
    }
}
