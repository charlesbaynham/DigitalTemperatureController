Yb+ temperature controller
==========================

Introduction  							{#sec-intro}
============

This is the source code for the Yb+ temperature controller, programmed and designed by <a href="mailto:charles.baynham@npl.co.uk">Charles Baynham</a>.

It is intended for high precision control of cavities, however can be used to control laser diode temperature or other low-bandwidth lock processes that could benefit from digital control. Error signal input can be either by voltage (from -5V to +5V) or through a balanced thermistor reading, mediated by a built-in INA330 IC. For demanding temperature applications, the latter method is recommended.

This code is designed to be uploaded to an ATMega328 microprocessor. It is written in C++ and can be compiled and uploaded via the Arduino IDE. However, if you intend to work on it for any significant length of time, I _highly_ recommend that you install a more capable IDE. I wrote this software in Visual Studio Community which is a free download from Microsoft and will make your life so much easier. 

Documentation 							{#sec-docs}
=============

If you are reading these comments in a pdf or html format then you are currently looking at the compiled documentation. If, however, you are reading this README.md file in a text editor then your first step before editing the code should be to get hold of the full code documentation. 

The code is documented throughout using the Doxygen commenting system. These can be compiled easily into a pdf or a local html website by:

1. Installing Doxygen from http://www.stack.nl/~dimitri/doxygen/download.html
2. Using the Doxygen GUI to load the file in this folder, `Doxyfile`
3. Selecting `Run doxygen` on the `Run` tab. 

This will produce the html documentation and, if you have pdflatex installed, source files for a pdf. 

Alternativly, open the Documentation.pdf file that should be bundled with this repo.  

Version Control 						{#sec-vc}
===============

This folder is a GIT repository that maintains a full history of development including every change since this code was started. This history is contained in a hidden folder called `.git` which should not be accessed directly. 

The upshot of this is that the code you see may not be the latest, and may instead be a "snapshot" of a previous state. To ensure you are looking at the latest iteration, and to make updates, install a git client of your choosing. https://windows.github.com/ is very lightweight, http://www.sourcetreeapp.com/ is more fully featured and https://desktop.github.com/ strikes a balance between the two.  

Once installed, "checkout" the master branch for the latest code version. See http://rogerdudler.github.io/git-guide/ for a simple guide. 

Note that this code makes use of several submodules. If you "clone" the code, you will by default not get these. In order to correctly get the code for these as well, run the command `git submodule update --init --recursive` after cloning the main code. If you use a more advanced GUI such as SourceTree then this should be handled for you automatically. 

Available commands						{#sec-commands}
=============

The device listens for commands on a virtual serial COM port served via USB. The USB connection is electrically isolated to avoid ground pollution. 

__Baud rate should be set to 57600.__

The board also contains footprints to enable RS485 communication via a backplane connection, however this is not yet implemented. 

For a detailed description of all the commands available, see the documentation for CommandDefinitions.h.

Code outline							{#sec-codeoutline}
=============

This code is split into classes according to the object orientated paradigm. A typical lock could be implemented as follows:

1. Define a YbCtrl::ErrorChannel object (or rather, an implementation of YbCtrl::ErrorChannel) to handle input.
2. Define a YbCtrl::CtrlChannel object to handle output
3. Define a YbCtrl::Algorithm object to implement the locking transfer function required.
4. Pass these three objects to a new YbCtrl::Controller object.
5. Loop, calling YbCtrl::Controller::doLoop() on each iteration. 

For more detail, every class and method is individually documented. See the Classes section or the Namespaces section to see more. 

@todo Implement RS485 Modbus protocol

@copyright Charles Baynham 2016