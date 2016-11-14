import java.util.concurrent.atomic.AtomicInteger;

public class PrimesCounter {
    
    public static void main(String[] args) throws InterruptedException {
        int upper_limit = Integer.parseInt(args[1]);
        int nthreads = Integer.parseInt(args[2]);

        Thread workers[] = new Thread[nthreads];
        for (int i = 0; i < nthreads; ++i) {
            // Worker worker = new Worker(upper_limit);
            workers[i] = new Thread(new Worker(upper_limit));
            workers[i].start();
        }

        for (int i = 0; i < nthreads; ++i) {
            workers[i].join();
        }

        System.out.println(total_result.get());
    }

    private static AtomicInteger counter = new AtomicInteger(0);

    private static AtomicInteger total_result = new AtomicInteger(0);

    private static class Worker implements Runnable {

        private int limit = 0;

        public  Worker(int limit_) {
            limit = limit_;
        }

        public void run() {
            int i = 0;
            int result = 0;
            while (i < limit) {
                i = counter.getAndIncrement();
                if (isPrime(i)) {
                    ++result;
                }
            }

            total_result.addAndGet(result);
        }

        public boolean isPrime(long n) {
            if (n <= 1) {
                return false;
            }

            if (n <= 3) {
                return true;
            }

            if (n % 2 == 0 || n % 3 == 0) {
                return false;
            }

            for (long i = 5; i * i <= n; i += 6) {
                if (n % i == 0 || n % (i + 2) == 0) {
                    return false;
                }
            }

            return true;
        }
    }
}
