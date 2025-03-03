use crate::traits::*;
use crate::{TestcaseEnum, Function, Poly1305Generator, ScalarmultGenerator, CryptoboxGenerator, StreamGenerator, SecretBoxGenerator};
use std::io::Write;
use std::fmt::{Formatter, Error};
use std::str::FromStr;
use simple_error::SimpleError;
use std::slice::Iter;

impl Generator for Function {
    fn get_generator_name(&self) -> String {
        match self {
            Function::scalarmult(gen) => gen.get_generator_name(),
            Function::poly1305(gen) => gen.get_generator_name(),
            Function::cryptobox(gen) => gen.get_generator_name(),
            Function::salsa20(gen) => gen.get_generator_name(),
            Function::secretbox(gen) => gen.get_generator_name(),
        }
    }

    fn generate_testcase(&self) -> TestcaseEnum {
        match self {
            Function::scalarmult(gen) => gen.generate_testcase(),
            Function::poly1305(gen) => gen.generate_testcase(),
            Function::cryptobox(gen) => gen.generate_testcase(),
            Function::salsa20(gen) => gen.generate_testcase(),
            Function::secretbox(gen) => gen.generate_testcase(),
        }
    }

    fn get_timeout(&self) -> u64 {
        match self {
            Function::scalarmult(gen) => gen.get_timeout(),
            Function::poly1305(gen) => gen.get_timeout(),
            Function::cryptobox(gen) => gen.get_timeout(),
            Function::salsa20(gen) => gen.get_timeout(),
            Function::secretbox(gen) => gen.get_timeout(),
        }
    }

    fn get_outputlen(&self) -> usize {
        match self {
            Function::scalarmult(gen) => gen.get_outputlen(),
            Function::poly1305(gen) => gen.get_outputlen(),
            Function::cryptobox(gen) => gen.get_outputlen(),
            Function::salsa20(gen) => gen.get_outputlen(),
            Function::secretbox(gen) => gen.get_outputlen(),
        }
    }
}

impl Testcase for TestcaseEnum {
    fn print_raw_output(&self, file: &mut impl Write) where Self: Sized {
        match self {
            TestcaseEnum::scalarmult(tc) => tc.print_raw_output(file),
            TestcaseEnum::poly130(tc) => tc.print_raw_output(file),
            TestcaseEnum::cryptobox(tc) => tc.print_raw_output(file),
            TestcaseEnum::salsa20(tc) => tc.print_raw_output(file),
            TestcaseEnum::secretbox(tc) => tc.print_raw_output(file),
        }
    }

    fn print_result(&self, file: &mut impl Write) where Self: Sized {
        match self {
            TestcaseEnum::scalarmult(tc) => tc.print_result(file),
            TestcaseEnum::poly130(tc) => tc.print_result(file),
            TestcaseEnum::cryptobox(tc) => tc.print_result(file),
            TestcaseEnum::salsa20(tc) => tc.print_result(file),
            TestcaseEnum::secretbox(tc) => tc.print_result(file),
        }
    }

    fn is_correct(&self) -> bool {
        match self {
            TestcaseEnum::scalarmult(tc) => tc.is_correct(),
            TestcaseEnum::poly130(tc) => tc.is_correct(),
            TestcaseEnum::cryptobox(tc) => tc.is_correct(),
            TestcaseEnum::salsa20(tc) => tc.is_correct(),
            TestcaseEnum::secretbox(tc) => tc.is_correct(),
        }
    }

    fn get_expected(&self) -> String {
        match self {
            TestcaseEnum::scalarmult(tc) => tc.get_expected(),
            TestcaseEnum::poly130(tc) => tc.get_expected(),
            TestcaseEnum::cryptobox(tc) => tc.get_expected(),
            TestcaseEnum::salsa20(tc) => tc.get_expected(),
            TestcaseEnum::secretbox(tc) => tc.get_expected(),
        }
    }

    fn copy_result_variables(&mut self, read_result: impl ReadResult) where Self: Sized {
        match self {
            TestcaseEnum::scalarmult(tc) => tc.copy_result_variables(read_result),
            TestcaseEnum::poly130(tc) => tc.copy_result_variables(read_result),
            TestcaseEnum::cryptobox(tc) => tc.copy_result_variables(read_result),
            TestcaseEnum::salsa20(tc) => tc.copy_result_variables(read_result),
            TestcaseEnum::secretbox(tc) => tc.copy_result_variables(read_result),
        }
    }
}

impl std::fmt::Display for TestcaseEnum {
    fn fmt(&self, f: &mut Formatter) -> Result<(), Error> {
        match self {
            TestcaseEnum::scalarmult(tc) => tc.fmt(f),
            TestcaseEnum::poly130(tc) => tc.fmt(f),
            TestcaseEnum::cryptobox(tc) => tc.fmt(f),
            TestcaseEnum::salsa20(tc) => tc.fmt(f),
            TestcaseEnum::secretbox(tc) => tc.fmt(f),
        }
    }
}

impl FromStr for Function {
    type Err = SimpleError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "poly1305" => Ok(Function::poly1305(Poly1305Generator {})),
            "scalarmult" => Ok(Function::scalarmult(ScalarmultGenerator {})),
            "cryptobox" => Ok(Function::cryptobox(CryptoboxGenerator {})),
            "stream" => Ok(Function::salsa20(StreamGenerator {})),
            "secretbox" => Ok(Function::secretbox(SecretBoxGenerator {})),
            _ => Err(SimpleError::new("Could not parse the function")),
        }
    }
}

impl Function {
    pub fn iterator() -> Iter<'static, Function> {
        static FUNCTIONS: [Function; 5] = [Function::poly1305(Poly1305Generator {}), Function::scalarmult(ScalarmultGenerator {}), Function::cryptobox(CryptoboxGenerator{}), Function::salsa20(StreamGenerator{}), Function::secretbox(SecretBoxGenerator{})];
        FUNCTIONS.iter()
    }
}

