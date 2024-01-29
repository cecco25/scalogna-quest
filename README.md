# Scalogna-Quest

A remake of a board game, called HeroQuest, developed in C and playable from the terminal.

### Prerequisites

* GCC Compiler

### Installation

To install gcc in your operating system you can follow these guides:

<details>
  <summary>Windows</summary>
  <ul>
    <li><a href="https://dev.to/gamegods3/how-to-install-gcc-in-windows-10-the-easier-way-422j">Install GCC on Windows</a></li>
  </ul>
</details>
<details>
  <summary>MacOS</summary>
  <ul>
    <li><a href="https://discussions.apple.com/thread/8336714?sortBy=best">Install GCC on MacOS</a></li>
  </ul>
</details>
<details>
  <summary>Linux</summary>
  <ul>
    <li><a href="https://phoenixnap.com/kb/install-gcc-ubuntu">Install GCC on Ubuntu</a></li>
  </ul>
</details>


## Usage

Before you can play Scalogna-Quest you must compile the project.
Here's how to do it:

```
$ gcc -c main.c gamelib.c -std=c11 -Wall
```
```
$ gcc -o game main.o gamelib.o
```

Now you can start playing by running the following command
```
$ ./game
```
