# bin2c
Program for turning a file into a c header. Useful for putting binary data into the final executable.

## Usage
```bash
    Transform a binary file into a c compatible header

    Usage:
            bin2c <file> <identifier>
    
    <file>       -  binary file to include. Use '-' for stdin
    <identifier> -  name of identifier in header used to access data.
                    will also add <identifier>_length for accessing
                    length of the file in bytes
    
    Code will print to stdout
```


