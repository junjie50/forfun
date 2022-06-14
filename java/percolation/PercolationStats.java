import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdStats;
import edu.princeton.cs.algs4.Stopwatch;


public class PercolationStats {
    // The total threshhold obtained per test.
    private double[] dataset;
    // The number of trials for this stat.
    private int trialsCount;
    // perform independent trials on an n-by-n grid
    public PercolationStats(int n, int trials) {
        int totalSites = n * n;
        trialsCount = trials;
        int[][] points  = new int[totalSites][];
        for (int i = 0; i < totalSites; i++) {
            points[i] = new int[] {i / n, i % n};
        }

        dataset = new double[trials];
        while (trials > 0) {
            trials--;
            StdRandom.shuffle(points);

            Percolation currRun = new Percolation(n);
            int curr = 0;
            while (!currRun.percolates()) {
                int[] selected = points[curr];
                currRun.open(selected[0], selected[1]);
                curr++;
            }

            // Adding the data from the back of dataset.
            dataset[trials] = (double) currRun.numberOfOpenSites() / totalSites;
        }
    }

    // sample mean of percolation threshold
    public double mean() {
        return StdStats.mean(dataset);
    }

    // sample standard deviation of percolation threshold
    public double stddev() {
        return StdStats.stddev(dataset);
    }

    // low endpoint of 95% confidence interval
    public double confidenceLow() {
        return mean() - 1.96 * stddev() / Math.sqrt(trialsCount);
    }

    // high endpoint of 95% confidence interval
    public double confidenceHigh() {
        return mean() + 1.96 * stddev() / Math.sqrt(trialsCount);
    }

    // test client (see below)
    public static void main(String[] args) {
        Stopwatch time = new Stopwatch();
        PercolationStats test1 = new PercolationStats(2, 100000);
        System.out.printf("%-20s = %f\n", "mean()", test1.mean());
        System.out.printf("%-20s = %f\n", "stddev()", test1.stddev());
        System.out.printf("%-20s = %f\n", "confidenceLow()", test1.confidenceLow());
        System.out.printf("%-20s = %f\n", "confidenceHigh()", test1.confidenceHigh());
        System.out.printf("%-20s = %f\n", "elapsed time", time.elapsedTime());
    }
}
