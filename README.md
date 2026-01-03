### EFI Wordle
This project aims at implementing the Wordle game as a EFI application.

## Prerequisites 
This project requires the following dependencies:
  - gnu c compiler
  - binutils
  - gnu efi library
Additionally, to run the built binary, these dependancies are supported:
  - quemu-system-x86_64
  - ovmf
When these dependancies are installed, the run script automates the process of loading the program into the file system and running the emulator.

## Building
Assuming the core dependancies are satisfied, use `make` to build the executable. The output binary can be found in `/build/bin/wordle.efi`.
