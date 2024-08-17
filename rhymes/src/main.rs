use clap::{Command, Arg};
use pyo3::prelude::*;
use pyo3::types::{PyModule, PyTuple};
use std::env;

fn main() -> PyResult<()> {
    // Setup command-line arguments
    let matches = Command::new("rhymes")
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
    
    // Initialize the Python interpreter
    pyo3::prepare_freethreaded_python();    

    // Use Python's Global Interpreter Lock (GIL)
    Python::with_gil(|py| {
        
        // Set the PYTHONPATH environment variable to the virtual environment's site-packages
        let virtual_env = "./myenv";
        let python_version = "3.12"; // Adjust this based on your Python version
        let site_packages = format!("{}/lib/python{}/site-packages", virtual_env, python_version);
        env::set_var("PYTHONPATH", site_packages);

        // Import the g2p_en module
        let g2p_module = PyModule::import_bound(py, "g2p_en")?;

        // Create an instance of the G2p class
        let g2p_class = g2p_module.getattr("G2p")?;
        let g2p_instance = g2p_class.call0()?;

        // Retrieve and process command-line arguments
        if let Some(words) = matches.get_many::<String>("WORDS") {
            for word in words {
                // Convert the Rust String to a Python string
                let py_word = PyTuple::new_bound(py, &[word]);

                let result = g2p_instance.call1(py_word)?;
                let phonemes: Vec<String> = result.extract()?;
                println!("{} -> {:?}", word, phonemes);
            }
        }

        Ok(())
    })
}
