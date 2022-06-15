import edu.princeton.cs.algs4.Picture;
import edu.princeton.cs.algs4.StdOut;

import java.awt.Color;
import java.util.Collections;

public class SeamCarver {
    private Picture pic;
    private int[][] dp;
    // create a seam carver object based on the given picture
    public SeamCarver(Picture picture) {
        pic =  new Picture(picture);
        dp = new int[pic.height()][pic.width()];
        for(int x = 0; x < pic.width(); x++) {
            for(int y = 0; y < pic.height(); y++) {
                dp[y][x] = -1;
            }
        }
    }

    // current picture
    public Picture picture() {
        return pic;
    }

    // width of current picture
    public int width() {
        return pic.width();
    }

    // height of current picture
    public int height() {
        return pic.height();
    }

    // energy of pixel at column x and row y
    public double energy(int x, int y) {
        double sumDxSquare[] = {0, 0};
        int targetLeft = x - 1;
        int targetRight = x + 1;
        int targetTop = y - 1;
        int targetBottom = y + 1;
        if(x == 0) {
            targetLeft = pic.width() - 1;
        }
        else if(x == pic.width() - 1) {
            targetRight = 0;
        }
        if(y == 0) {
            targetTop = pic.height() - 1;
        }
        else if(y == pic.height() - 1) {
            targetBottom = 0;
        }


        for(int i = 0; i < 2; i ++) {
            double sum = 0;
            Color cLeft;
            Color cRight;
            if(i == 0) {
                cLeft = pic.get(targetLeft, y);
                cRight = pic.get(targetRight, y);
            }
            else {
                cLeft = pic.get(x, targetTop);
                cRight = pic.get(x, targetBottom);
            }
            for (int j = 0; j < 3; j++) {
                if (j == 0) {
                    sum += Math.pow((cLeft.getRed() - cRight.getRed()), 2);
                }
                else if (j == 1) {
                    sum += Math.pow((cLeft.getBlue() - cRight.getBlue()), 2);
                }
                else {
                    sum += Math.pow((cLeft.getGreen() - cRight.getGreen()), 2);
                }
            }
            sumDxSquare[i] = sum;
        }
        return sumDxSquare[0] + sumDxSquare[1];
    }

    // sequence of indices for horizontal seam
    public int[] findHorizontalSeam() {
        // Do dp for first col.
        for(int i = 0; i < pic.width(); i++) {
            seamHelper(0, i, 0);
        }

        int currY = 0;
        int min = dp[0][0];
        for(int y = 0; y < pic.height(); y++) {
            if(dp[y][0] < min) {
                currY = y;
                min = dp[y][0];
            }
        }

        int[] result = new int[pic.width()];
        result[0] = currY;
        for(int x = 1; x < pic.width(); x++) {
            int left = validRange(x, currY - 1)? dp[currY - 1][x]:Integer.MAX_VALUE;
            int bottom =dp[currY][x];
            int right = validRange(x, currY + 1)? dp[currY + 1][x]:Integer.MAX_VALUE;
            if(left < bottom) {
                if(left < right) {
                    currY -= 1;
                }
                else {
                    currY += 1;
                }
            }
            else {
                if(bottom > right) {
                    currY += 1;
                }
            }
            result[x] = currY;
        }

        return result;
    }

    // sequence of indices for vertical seam
    public int[] findVerticalSeam() {
        // Do dp for first row.
        for(int i = 0; i < pic.width(); i++) {
            seamHelper( i, 0, 1);
        }

        int currX = 0;
        int min = dp[0][0];
        for(int x = 0; x < pic.width(); x++) {
            if(dp[0][x] < min) {
                currX = x;
                min = dp[0][x];
            }
        }

        int[] result = new int[pic.height()];
        result[0] = currX;
        for(int y = 1; y < pic.height(); y++) {
            int left = validRange(currX - 1, y)? dp[y][currX - 1]:Integer.MAX_VALUE;
            int bottom =dp[y][currX];
            int right = validRange(currX + 1, y)? dp[y][currX + 1]:Integer.MAX_VALUE;
            if(left < bottom) {
                if(left < right) {
                    currX -= 1;
                }
                else {
                    currX += 1;
                }
            }
            else {
                if(bottom > right) {
                    currX += 1;
                }
            }
            result[y] = currX;
        }
        return result;
    }

    // Direction 1 = vertical
    private int seamHelper(int x, int y, int direction) {
        if(!validRange(x, y)) {
            return Integer.MAX_VALUE;
        }

        if(dp[y][x] != -1) {
            return dp[y][x];
        }
        if(direction == 1 && y == pic.height() - 1) {
            dp[y][x] =(int) energy(x, y);
            return dp[y][x];
        }
        if(direction == 0 && y == pic.width() - 1) {
            dp[y][x] =(int) energy(x, y);
            return dp[y][x];
        }

        int minLeft = (direction == 1)? seamHelper(x - 1, y + 1, direction) : seamHelper(x + 1, y - 1, direction);
        int minBottom = (direction == 1)? seamHelper(x, y + 1, direction) : seamHelper(x + 1, y, direction);
        int minRight = seamHelper(x + 1, y + 1, direction);

        int curr = (int) energy(x, y) + Math.min(Math.min(minLeft, minBottom), minRight);
        dp[y][x] = curr;
        return curr;
    }

    // remove horizontal seam from current picture
    public void removeHorizontalSeam(int[] seam) {
        Picture newPic = new Picture(pic.width(), pic.height() - 1);
        int[][] newdp = new int[pic.height() - 1][pic.width()];
        for(int x = 0; x < pic.width(); x++) {
            int offset = 0;
            for(int y = 0; y < pic.height() - 1; y++) {
                if(y == seam[x]){
                    offset += 1;
                }
                newPic.setRGB(x, y, pic.getRGB(x, y + offset));
                newdp[y][x] = -1;
            }
        }
        pic = newPic;
        dp = newdp;
    }

    // remove vertical seam from current picture
    // reuse dp base on triangle principle. triangle from bottom origin will be affected. rest is the same.
    public void removeVerticalSeam(int[] seam) {
        Picture newPic = new Picture(pic.width() - 1, pic.height());
        int[][] newdp = new int[pic.height()][pic.width() - 1];
        for(int y = 0; y < pic.height(); y++) {
            int offset = 0;
            for(int x = 0; x < pic.width() - 1; x++) {
                if(x == seam[y]){
                    offset += 1;
                }
                newPic.setRGB(x, y, pic.getRGB(x + offset, y));
                newdp[y][x] = -1;
            }

            // increase first x by 1 and decrease lastx by 1 and
        }
        pic = newPic;
        dp = newdp;
    }

    // Private method to check if is within range
    private boolean validRange(int x, int y) {
        if(x >= 0 && x < pic.width() && y >= 0 && y < pic.height()) {
            return true;
        }
        return false;
    }

    private void printEnergy() {
        for(int i = 0; i < pic.height(); i++) {
            for(int j = 0; j < pic.width(); j++) {
                System.out.printf("%d ", (int) energy(j, i));
            }
            System.out.println("");
        }
    }

    //  unit testing (required)
    public static void main(String[] args) {
        System.out.println("hello\n\n\n");
        // Test 1
        SeamCarver test1 = new SeamCarver(new Picture("3x4.png"));
        if(test1.energy(1, 2) != 52024) {
            System.out.println("test 1 failed");
        }
        if(test1.energy(1, 1) != 52225) {
            System.out.println("test 1 failed");
        }
        if(test1.energy(1, 0) != 52020) {
            System.out.println("test 1 failed");
        }
        int total = 0;
        int count = 0;
        do {
            SeamCarver timetest = new SeamCarver(SCUtility.randomPicture(736, 584));
            long start = System.currentTimeMillis();
            timetest.removeHorizontalSeam(timetest.findHorizontalSeam());
            long end = System.currentTimeMillis();
            total += (end - start);
            count++;
        }while (total < 1000);
        System.out.printf("%d counts in 1 second.", count);
    }

}