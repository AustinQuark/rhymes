use std::process::Command;
use std::str;

fn main() {
    let output = Command::new("espeak")
        .args(&["-v", "en", "-q", "-x", "--ipa=0", "-f", "/home/austin/Code/rhymes/rhymes/words"])
        .output()
        .expect("Failed to execute command");

    let result = str::from_utf8(&output.stdout)
        .expect("Failed to convert to String");

    println!("{}", result);
}