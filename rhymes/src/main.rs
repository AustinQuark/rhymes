use std::process::Command;
use std::str;

fn espeak_cmd() -> String {
    let output = Command::new("espeak")
        .args(&["-v", "en", "-q", "-x", "--ipa=0", "-f", "/home/austin/Code/rhymes/rhymes/words"])
        .output()
        .expect("Failed to execute command");

    
    let result = String::from_utf8_lossy(&output.stdout).to_string();

    return result; 
}

struct Rhyme {
    input: String,
}

impl Rhyme {
    fn new() -> Rhyme {
        let input = espeak_cmd();

        Rhyme { input }
    }
}

fn main() {
    let rhyme = Rhyme::new();

    // Now you can access the `input` field from `rhyme` object
    println!("{}", rhyme.input);
}
