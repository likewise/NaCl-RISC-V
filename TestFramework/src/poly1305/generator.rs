use rand::Rng;
use sodiumoxide::crypto::onetimeauth;
use std::fs::{remove_file, OpenOptions};
use std::path::{Path};
use std::env;
use std::io::Write;
use log::{info};
use std::str::FromStr;

#[derive(Debug)]
pub struct TestcasePoly1305{
    pub variables: Vec<String>,
    pub messagelen: usize,
    pub expected_result: [u8;16],
}

pub fn generator_name() -> String {
    String::from_str("poly1305").unwrap()
}

pub fn generate_testcase(messagelen : usize) -> TestcasePoly1305{
    let mut rng = rand::thread_rng();

    let mut message: [u8; 256] = [0; 256];
    rng.fill(&mut message);

    let poly1305_key = onetimeauth::gen_key();
    let key = poly1305_key.0;

    //print variables
    let mut variables = Vec::new();

    let mut var = String::with_capacity((messagelen*2) as usize);
    var.push_str(format!("        static unsigned char c[{}] = {{", messagelen).as_str());

    for i in 0..messagelen - 1 {
        var.push_str(format!("0x{:x}, ", message[i as usize]).as_str());
    }
    var.push_str(format!("0x{:x}}};", message[(messagelen - 1) as usize]).as_str());
    variables.push(var);

    var = String::with_capacity((messagelen*2) as usize);
    var.push_str(format!("        static unsigned char rs[32] = {{").as_str());
    for i in 0..31 {
        var.push_str(format!("0x{:x}, ", key[i as usize]).as_str());
    }
    var.push_str(format!("0x{:x}}};", key[31 as usize]).as_str());
    variables.push(var);

    let message_slice = &message[0..messagelen];

    let expected_result = onetimeauth::authenticate(&message_slice, &poly1305_key).0;
    generate_testcasefile(variables.clone(), messagelen);
    TestcasePoly1305 {variables,  expected_result, messagelen}
}


fn generate_testcasefile(variables: Vec<String>, inlen: usize){
    let buf_path = env::current_dir().expect("Failed to get current path");
    let current_path = buf_path.as_path();
    let benchmark_path = current_path.join(Path::new("benchmark.c"));
    remove_file(benchmark_path.clone()).expect("Can not remove benchmark.c");

    let mut file = OpenOptions::new()
        .create(true)
        .write(true)
        .read(true)
        .open(benchmark_path).expect("Couldn't create benchmark.c file");
    //print header stuff
    writeln!(file, "#include \"benchmark.h\"
    extern void icachemisses();

    void printintarray(unsigned int *a, int initialoffset){{

           for(int i = initialoffset+3; i < 21*3;i+=3){{
               printf(\"%6u, \", a[i]-a[i-3]);
        }}
        printf(\"\\n\");
    }}


    void dobenchmark() {{

    unsigned char a[16];").expect("write failed");

    for var in variables {
        writeln!(file, "{}", var).expect("write failed");
    }

    //print rest of the code
    writeln!(file, "

        unsigned int counters[3*21];
        icachemisses();

        uint32_t timings[21];
        for(int i =0;i<21;i++){{
            getcycles(&counters[i*3]);
            crypto_onetimeauth(a,c,{},rs);
        }}

        printf(\"Cycle counts:          \");
        printintarray(counters, 0);

        printf(\"Branch dir mis:        \");
        printintarray(counters, 1);

        printf(\"Branch target mis:    \");
        printintarray(counters, 2);

        printf(\"Result: \");
        printchararray(a,16);
        printf(\"\\n\\n\");

    }}", inlen).expect("write failed");
    file.flush().expect("Couldn't flush benchmark file");
    info!("written benchmark.c");

}