extern crate chrono;
extern crate rand;

use chrono::prelude::*;
use rand::Rng;

#[derive(Debug)]
struct SSN {
    lead: usize,
    group: usize,
    serial: usize,
}

impl Default for SSN {
    fn default() -> Self {
        let mut rng = rand::thread_rng();
        SSN {
            lead: rng.gen_iter::<usize>()
                .map(|x| (x % 900) + 1)
                .filter(|x| *x != 666)
                .next()
                .unwrap(),
            group: rng.gen_range::<usize>(1, 99),
            serial: rng.gen_range(1, 9999),
        }
    }
}

#[derive(Debug)]
struct Person {
    name: String,
    email: String,
    address: String,
    ssn: SSN,
    birth_date: Date<Utc>,
}

impl Person {
    fn gen_birth_date() -> Date<Utc> {
        let mut rng = rand::thread_rng();
        let now: i64 = rng.gen_range(0, Utc::now().timestamp());
        Utc.timestamp(now, 0).date()
    }

    fn gen_name() -> String {
        unimplemented!()
    }
}

impl Default for Person {
    fn default() -> Self {
        Person {
            name: Person::gen_name(),
            email: "".to_owned(),
            address: "".to_owned(),
            ssn: SSN::default(),
            birth_date: Person::gen_birth_date(),
        }
    }
}
