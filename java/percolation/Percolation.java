import edu.princeton.cs.algs4.WeightedQuickUnionUF;

public class Percolation {
    private WeightedQuickUnionUF data;
    private int[][] grid;
    private int N;
    private int openSize;
    private int root;
    private int head;

    // creates n-by-n grid, with all sites initially blocked
    public Percolation(int n){
        grid = new int[n][n];
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++) {
                grid[i][j] = 0;
            }
        }
        data = new WeightedQuickUnionUF(n * n + 2);
        root = n * n;
        head = n * n + 1;
        N = n;
        openSize = 0;
    }

    // opens the site (row, col) if it is not open already
    public void open(int row, int col) {
        if(!validRange(row, col)) {
            throw new IllegalArgumentException();
        }
        else if(isOpen(row, col)) {
            return;
        }

        if(row == 0) {
            data.union(root, col);
        }
        if(row == N - 1){
            data.union(head, getNumber(row, col));
        }
        grid[row][col] = 1;
        openSize += 1;

        // Check for the 4 sides to see if there is filled sites
        openHelper(row, col, row + 1, col);
        openHelper(row, col, row - 1, col);
        openHelper(row, col, row, col + 1);
        openHelper(row, col, row, col - 1);
    }

    private void openHelper(int prevrow, int prevcol, int row, int col){
        if(validRange(row, col)) {
            int nextnum = getNumber(row, col);
            int prevnum = getNumber(prevrow, prevcol);
            if(grid[row][col] == 1){
                data.union(prevnum, nextnum);
            }
        }
    }

    private int getNumber(int row, int col){
        return (row) * N + col;
    }

    // is the site (row, col) open?
    public boolean isOpen(int row, int col) {
        if(!validRange(row, col)) {
            throw new IllegalArgumentException();
        }
        return grid[row][col] == 1;
    }

    // is the site (row, col) full?
    public boolean isFull(int row, int col) {
        if(!validRange(row, col)) {
            throw new IllegalArgumentException();
        }
        if(grid[row][col] == 0){
            return false;
        }
        return data.find(getNumber(row, col)) == data.find(root);
    }

    // returns the number of open sites
    public int numberOfOpenSites() {
        return openSize;
    }

    // does the system percolate?
    public boolean percolates(){
        return data.find(root) == data.find(head);
    }

    // unit testing (required)
    public static void main(String[] args) {

    }

    private boolean validRange(int col, int row) {
        if(col >= 0 && row >= 0 && col < N && row < N){
            return true;
        }
        return false;
    }
}

