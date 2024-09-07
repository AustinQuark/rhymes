use std::process::Command;
use std::str;
use std::fs;

struct EspeakCmd {
    input_text: String,
    ipa_text: Vec<u8>,
}

impl EspeakCmd {
    fn new() -> EspeakCmd {
        let input_text: String = fs::read_to_string("./words")
        .expect("Could not read file");

        let output = Command::new("espeak")
            .args(&["-v", "en", "-q", "-x", "--ipa=0", "-f", "/home/austin/Code/rhymes/rhymes/words"])
            .output()
            .expect("Failed to execute command");

        let ipa_text = output.stdout;

        EspeakCmd { 
            input_text,
            ipa_text,
         }
    }
}

struct Rhyme {
}

impl Rhyme {
    fn new() -> Rhyme {
        Rhyme { }
    }
}

fn main() {
    let espeak: EspeakCmd = EspeakCmd::new();

    println!("{}", espeak.input_text);
    println!("{:?}", espeak.ipa_text);

    // Now you can access the `input` field from `rhyme` object
    //println!("{}", rhyme.input);
}
