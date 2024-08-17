use clap::{Command, Arg};
use pyo3::prelude::*;
use pyo3::types::{PyModule, PyTuple};
use std::env;

fn main() -> PyResult<()> {
    // Setup command-line arguments
    let matches = Command::new("rhemes")
        .version("1.0")
        .author("Your Name <your.email@example.com>")
        .about("Extracts phonetics of words using a Python G2P model")
        .arg(
            Arg::new("WORDS")
                .help("The words to convert to phonetics")
                .required(true)
                .num_args(1..), // This replaces multiple_values
        )
        .get_matches();

    // Use Python's Global Interpreter Lock (GIL)
    Python::with_gil(|py| {
        // Set the PYTHONPATH environment variable
        env::set_var("PYTHONPATH", ".");

        // Import the Python module
        let g2p_module = PyModule::import(py, "g2p_en")?;
        
        // Access the function from the module
        let convert_to_phonemes = g2p_module.getattr("convert_to_phonemes")?;

        // Retrieve and process command-line arguments
        if let Some(words) = matches.get_many::<String>("WORDS") {
            for word in words {
                // Convert the Rust String to a Python string
                let py_word = PyTuple::new(py, &[word]);

                // Call the Python function and extract the result
                let phonemes: Vec<String> = convert_to_phonemes.call1(py_word)?.extract()?;
                println!("{} -> {:?}", word, phonemes);
            }
        }

        Ok(())
    })
}
