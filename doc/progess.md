Progress report
=======================================

A0108165J

TONG Haowen Joel

## Done

- Build environment using Cmake and basic test directory up
- Got a rough idea of how to implement application.
- Implmeneted chat list data structure, with linked list and serialization / 
    deserialization support (for transfer)
- Added GTK and linked GTK
- Added basic event handler to be called every cycle


## Outline

- Server shall be single-thread
- Client application to be multithreaded, inevitable as keyboard input cannot block.
- Client application needs 2 windows, one for output of chat / program and one for input.  
    I will be using GTK to do that as it appears easier
- Repository, private, Github.
    
    
## TODO

- Put tasks in separate threads 
- Write protocol (should be mostly stateless, state complicates)
- Code up client (much work)
- Code up server (much work)
    
    
## Questions

- I wonder if am overthinking the product spec.
- Is there a simpler library for strings?!



