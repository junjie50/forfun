import edu.princeton.cs.algs4.BinaryStdIn;
import edu.princeton.cs.algs4.BinaryStdOut;
import edu.princeton.cs.algs4.Queue;
import edu.princeton.cs.algs4.StdIn;

import java.util.ArrayList;

public class BurrowsWheeler {
    // apply Burrows-Wheeler transform,
    // reading from standard input and writing to standard output
    public static void transform() {
        String curr = StdIn.readAll();
        CircularSuffixArray data = new CircularSuffixArray(curr);
        for(int i = 0; i < curr.length(); i++) {
            if(data.index(i) == 0) {
                BinaryStdOut.write(i);
                break;
            }
        }
        for(int i = 0; i < curr.length(); i++) {
            int index = data.index(i) - 1;
            if(index < 0) {
                index = curr.length() - 1;
            }
            BinaryStdOut.write(curr.charAt(index));
        }
        BinaryStdOut.close();
    }

    // apply Burrows-Wheeler inverse transform,
    // reading from standard input and writing to standard output
    public static void inverseTransform() {
        ArrayList<Queue<Integer>> indexTable = new ArrayList<Queue<Integer>>();
        for(int i = 0; i < 256; i++) {
            indexTable.add(new Queue<Integer>());
        }

        int first = BinaryStdIn.readInt();
        String text = BinaryStdIn.readString();
        for(int i = 0; i < text.length(); i++) {
            char charCurr = text.charAt(i);
            indexTable.get(charCurr).enqueue(i);
        }

        int[] t = new int[text.length()];
        int curr = 0;
        for(int i = 0; i < 256; i++) {
            if (!indexTable.get(i).isEmpty()) {
                while (!indexTable.get(i).isEmpty()) {
                    t[curr] = indexTable.get(i).dequeue();
                    curr++;
                }
            }
        }

        curr = t[first];
        for(int i = 0; i < text.length(); i++) {
            BinaryStdOut.write(text.charAt(curr));
            curr = t[curr];
        }
        BinaryStdOut.close();
    }

    // if args[0] is "-", apply Burrows-Wheeler transform
    // if args[0] is "+", apply Burrows-Wheeler inverse transform
    public static void main(String[] args) {
        if(args[0].compareTo("-") == 0) {
            transform();
        }
        else if(args[0].compareTo("+") == 0) {
            inverseTransform();
        }

    }

}