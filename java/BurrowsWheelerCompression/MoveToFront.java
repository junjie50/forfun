import edu.princeton.cs.algs4.BinaryStdIn;
import edu.princeton.cs.algs4.BinaryStdOut;

import java.util.ArrayList;

public class MoveToFront {
    // apply move-to-front encoding, reading from stdin and writing to stdout
    public static void encode() {
        ArrayList<Byte> location = new ArrayList<Byte>();
        for(int i = 0; i < 256; i ++ ){
            location.add(i, (byte)(char)i);
        }

        while (!BinaryStdIn.isEmpty()) {
            byte value = BinaryStdIn.readByte();
            int curr = 0;
            while(curr <= 255) {
                if(location.get(curr) == value) {
                    BinaryStdOut.write((byte)curr);
                    location.remove(curr);
                    location.add(0, value);
                    break;
                }
                curr+=1;
            }
        }
        BinaryStdOut.close();
    }

    // apply move-to-front decoding, reading from stdin and writing to stdout
    public static void decode() {
        ArrayList<Byte> location = new ArrayList<Byte>();
        for(int i = 0; i < 256; i ++ ){
            location.add(i, (byte)i);
        }

        while (!BinaryStdIn.isEmpty()) {
            char index = BinaryStdIn.readChar();
            BinaryStdOut.write(location.get(index));
            byte val = location.get(index);
            location.remove(index);
            location.add(0, val);
        }

        BinaryStdOut.close();
    }

    // if args[0] is "-", apply move-to-front encoding
    // if args[0] is "+", apply move-to-front decoding
    public static void main(String[] args) {
        if(args[0].compareTo("-") == 0) {
            encode();
        }
        else if(args[0].compareTo("+") == 0) {
            decode();
        }
    }

}
