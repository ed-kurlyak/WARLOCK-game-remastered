# warlock-remastered

The game WARLOCK was introduced in the book `Andre LaMothe "Tricks of the Game-Programming Gurus" (1994). In the book, the game was developed for the MS-DOS operating system, using video mode 13h (256 colors) with a screen resolution of 320×200 pixels. The game's code was described in Chapter 19, where `Andre demonstrated Ray Casting technology. The original code was compiled using the Microsoft C++ compiler.


Ray Casting Technology  
Ray Casting (ray projection), popular in the early 90s, was used to create pseudo-3D graphics in early first-person shooters. It allowed rendering game worlds at high speeds on weak hardware, simulating 3D without a full-fledged 3D engine.  

This technology was used in games such as:  
- Wolfenstein 3D (1992)  
- Doom (1993)  
- Doom II (1994)  
- Heretic (1994)  
- Hexen (1995)  
and others.  

Project Description  
This repository contains a remastered version of the WARLOCK game from Andre LaMothe's book, implemented in C++ (Visual Studio 2019) and tested on Windows 10.  

Key Features of the Project:  


1. Sound System – Uses the HMI SOS (Human Machine Interfaces Sound Operating System) library for Windows.  
   - Source: https://web.archive.org/web/19970225190838/http://www.humanmachine.com/dev.htm  


2. Back Buffer – Uses the SciTech MGL (SciTech Software MegaGraph Graphics Library) to maintain an 8-bit back buffer, as the original WARLOCK was designed for 256-color mode (320×200 resolution).  
   - The SciTech MGL library was taken from the Quake 1 source code.  
   - Download: https://github.com/id-Software/Quake  


3. Input Handling – Keyboard controls are implemented using WinAPI functions.  


4. Display Mode – The application runs in windowed mode with a client area of 800×600 pixels (resolution can be increased with minor modifications).  


5. Custom Text Rendering Library – I personally developed a small library for rendering text in graphical applications, which was used in this project to display in-game text.  


6. The HMI SOS and SciTech MGL libraries are 32-bit libraries that were used in the 1990s, so the project settings in Visual Studio 2019 should be set to Debug x86.  


This project serves as a modernized reimplementation of the classic WARLOCK game, preserving its original mechanics while adapting it to run on modern systems.

<img src="https://github.com/ed-kurlyak/warlock-remastered/blob/main/pics/pic1.png">

<img src="https://github.com/ed-kurlyak/warlock-remastered/blob/main/pics/pic2.png">
