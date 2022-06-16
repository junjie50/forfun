import edu.princeton.cs.algs4.In;

import java.util.Arrays;

public class CircularSuffixArray {
    // circular suffix of s
    private class CircularSuffix implements Comparable<CircularSuffix>{
        private int ptr;
        private String s;
        private int len;
        public CircularSuffix(String _s, int _ptr) {
            s = _s;
            ptr = _ptr;
            len = s.length();
        }

        public int get(int index) {
            return s.charAt((index + ptr) % len);
        }


        public int compareTo(CircularSuffix o) {
            for(int i = 0; i < len; i++) {
                if(get(i) < o.get(i)) {
                    return -1;
                }
                else if (get(i) > o.get(i)) {
                    return 1;
                }
            }
            return 0;
        }
    }

    private CircularSuffix[] array;
    private String ori;
    private int len;

    public CircularSuffixArray(String s) {
        ori = s;
        array = new CircularSuffix[s.length()];
        len = s.length();


        for(int i = 0; i < array.length; i++) {
            array[i] = new CircularSuffix(s, (i + 1)%len);
        }

        Arrays.sort(array);
    }

    // length of s
    public int length() {
        return len;
    }

    // returns index of ith sorted suffix
    public int index(int i) {
        return array[i].ptr;
    }

    // unit testing (required)
    public static void main(String[] args) {
        In file = new In("abra.txt");
        CircularSuffixArray test1 = new CircularSuffixArray(file.readAll());
    }

}
