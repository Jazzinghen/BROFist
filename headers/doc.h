/** @file doc.h Documentation header file */

/** @mainpage BROFist - BROFist: Robot Operational Fists
 *
 * Project BROFist is aimed at the creation of a toolkit for the
 * communication between SciCosLab and any kind of NXT that uses nxtOSEK
 * (Which we will call SPAM now on for shortness).
 *
 * This is achieved by the use of three linked parts:
 *
 *  - A set of SciCos blocks and linked C functions
 *  - A pc server that marshals the communications between Scicos and
 *      SPAM
 *  - A client to upload on SPAM
 *
 * A test client has been also provided to test BROFist functionality
 * without having to use SciCosLab.
 *
 * \section Compile Compilation
 *
 *      Each of the three parts has its own compilation and Usage method.
 *      
 *      \subsection BROServer BROFist Server
 *          For the BROFist Server it is as easy as doing:
\verbatim
cd src
make all
\endverbatim
 *      \subsection SPAMClient SPAM nxtOSEK Client
 *
 *      When compiling the SPAM Client the right path for the nxtOSEK
 *      @c ecrobot.mak file (Usually under [nxtOSEK path]/ecrobot/ecrobot.mak])
 *      should be written in line 11 inside spam/Makefile . This is not a
 *      good way of doing things, but this is how nxtOSEK works for their
 *      projects.
 *
 *      \subsection ScicosBlocks Scicos Blocks and Functions
 *
 *      When using Scicoslab, before launching the Scicos screen the comand
\verbatim
exec BRO_Loader.sce
\endverbatim
 *      have to be ran to compile and link all the necessary files. Then,
 *      inside Scicos, to add every needed block, the Edit->Add New Block
 *      should be used with the right name in order to add the required
 *      blocks.
 *      An example file @c Comm_Test.cos has been provided to give the user an
 *      example of how the Scicos Blocks should be used (If you find some
 *      other "creative" ways to use them you should send me an eMail to
 *      make my day a better one).
 *
 *  \section Dependency
 *      
 *      BROFist Project needs, as you might have suggested, SciCosLab and
 *      nxtOSEK. Is it also possible that you don't have the Bluez library.
 *      Install it will be as easy as typing
\verbatim
sudo apt-get install libbluetooth-dev
\endverbatim
*       However it shouldn't be necessary as it should be installed
*       automatically in your system.
*
*   \section WhatSPAM The origin of SPAM
*
*       At the beginning of this project there was only a NXT unit that
*       had no heart nor build quality. It couldn't go straight due to its
*       bad construction. So I've asked GODS (The "Grande Operatore
*       Didattico Simonini") to study and then help me realize a NXT unit
*       that could "Do the impossible, Break The Unbreakable". And so the
*       SPAM ("Super Powerful Automated Mecha") was born. After a few tries
*       and after lots of tests we realized that is was "Not-So-Powerful".
*       So we maintained its name, but changed its meaning.
*       Now you will know that when we talk about SPAM we refer to a
*       "Semi-Powerful Automated Mecha".
*
*   \section Thanks Thank Yous and Other Stuff
*       
*       This project wouldn't have been possible with the help and the
*       presence of lots of people (Thank you all, guys!). However some
*       deserve the spotlight:
*       
*       - Ivan "GODS" Simonini: SPAM "Biological Father", as some people
*       call him, Metal Gear Solid fan and Mecha addicted.
*       - Giovanni "Simgi Dacav" Simoni: Programming Samurai and
*       predecessor in this "Crusade" against badly written software.
*       - Michele "Master Miguèl" Graffeo: Cognitive Sciences researcher,
*       drinking buddy and the best RPG Master I've ever met.
*       - The Einherjar "Core": You know who you are XD. However, Ivan
*       Candiolli should be listed here. Thank you.
*/

/** @defgroup BROFistServer
 * 
 * This is the heart of the whole system.
 * 
 * It provides the user with a
 * communication server that will accept communication from any kind of
 * correctly configured client, send the data to a SPAM and then provide
 * feedback for the requests to the connected client.
 *
 */

/*@{*/

/**     @defgroup BroBT BROFist Bluetooth Control
 *
 *      This module controls the BlueTooth communication with the SPAM
 *      paired with the server.
 *
 *      We use @c LibBluez to make 
 */

/*@}*/
