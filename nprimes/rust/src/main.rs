#![feature(integer_atomics)]

use std::env;
use std::sync::atomic::{AtomicU64, Ordering};
use std::thread;
use std::sync::Arc;

fn is_prime(n: u64) -> bool {
    if n <= 1 {
        return false;
    }

    if n <= 3 {
        return true;
    }

    if n % 2 == 0 || n % 3 == 0 {
        return false;
    }

    let mut i = 5;
    while i * i <= n {
        if  n % i == 0 || n % (i + 2) == 0 {
            return false;
        }
        i += 6;
    }
    true
}

fn nprimes(counter: Arc<AtomicU64>, n: u64) -> u64 {
    let mut result = 0;
    loop {
        let next_int = counter.fetch_add(1, Ordering::Relaxed);
        if next_int >= n {
            break;
        }
        if is_prime(next_int) {
            result += 1;
        }
    }
    result
}

fn main() {
    let usage = "Usage: nprimes upper_limit nthreads";

    let upper_limit = env::args().nth(1).expect(usage).parse::<u64>().expect(usage);
    let nthreads = env::args().nth(2).expect(usage).parse::<u64>().expect(usage);

    let counter = Arc::new(AtomicU64::new(0));

    let mut threads = vec![];
    for _ in 0..nthreads {
        let counter_clone = counter.clone();
        threads.push(thread::spawn(move || {
            nprimes(counter_clone, upper_limit)
        }));
    }
    let mut result = 0;
    for child in threads {
        result += child.join().unwrap();
    }
    println!("{}", result);
}
