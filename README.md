# Distributed-Systems

This repository has implementations of Distributed Systems concepts and design patterns.

1. [Gossip Protocol](#gossip-protocol)
2. [Distributed Hash Tables - Chord Protocol](#distributed-hash-tables-chord-protocol)


## 1. Gossip Protocol

Gossip Protocol is a communication method used in distributed systems to propagate information reliably and efficiently among nodes. It works by having each node periodically share information with a few randomly selected nodes, which in turn share it with others, much like how gossip spreads in social networks. 
   
- Course ID: CS 425 (Distributed Systems)
- University: UIUC
- Professor: Dr. Indranil Gupta
- URL: https://www.coursera.org/learn/cs-425/home/welcome
- Reference: [A Gossip-Style Failure Detection Service (van Renesse et al., 2001)](https://ecommons.cornell.edu/server/api/core/bitstreams/e78d24e8-024e-4e3d-872d-e3e682ac2505/content)

`./run.sh` to run the program.

`./Grader.sh` to check the scores.


## 2. Distributed Hash Tables - Chord Protocol

The Chord protocol is a structured distributed hash table (DHT) that provides efficient key-value storage and retrieval in a distributed system. It organizes nodes in a circular identifier space and uses consistent hashing to assign keys to nodes, ensuring balanced load distribution. Each node maintains a finger table with references to other nodes, allowing logarithmic time complexity for lookups and ensuring scalability and fault tolerance.


- Compile the program using `make`.
- Run `./Application <port_1>` to create the first node in the ring.
- Run `./Application <port_2> <port_1>` to create a new node and join an existing node in the ring (Note - `<port_1>` can be any node active in the ring).
- Run `./Client`, and specify the port number to which you want to connect, in the prompt.
- Perform operations like *insert*, *search* and *delete* on the key-value pairs in the data store.




