# CSCI 4061 Project 4 - Network Sockets

## Project Group Number: 27  

Reed Fazenbaker (fazen007)  
Mikkel Folting (folti002)  
NOTE: the extra credit portion of this project was NOT attempted.

## Contributions

Mikkel: Creating worker threads, server-side logic to handle requests, maintaining global balance array, serialization  
Reed: Reading from files in client, client-side logic with request-handling, maintaining client's store of cash  

## Assumptions

No significant assumptions were made outside the project specification.  

## Compiling the program

```bash
make
```

Type the above command into the root directory of the project.

## Running the program

```bash
./launcher server_addr server_port num_workers input_file [input_file] [input_file] ...  
```  

Type the above command into the root directory of the project to launch the bank system.  
NOTE: num_workers is obsolete in this implementation because the extra credit was not attempted.  

## Test Machine Information

test machine: csel-kh1250-01.cselabs.umn.edu  
group number: G[27]  
names: Reed Fazenbaker, Mikkel Folting  
x500: fazen007, folti002
