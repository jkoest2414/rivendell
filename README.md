This is the README file for the Rivendell package.

## WARNING: This branch has been frozen, and is of historical interest only.

The Git branch for ongoing development of v3.x (in the unlikely event of
their being any) is 'v3'.

This software comes with ABSOLUTELY NO WARRANTY. See the file COPYING for
details.

Welcome to this release of the Rivendell Project. Rivendell aims
to be a robust, functionally complete digital audio system for use in 
professional radio broadcast environments.

See the file INSTALL for installation instructions. If you are upgrading from
a 1.x or 2.x version of Rivendell, also see the file 'UPGRADING'
for important notes and pointers.


OVERVIEW
Rivendell contains a full set of functionality needed to operate a radio
automation system, consisting of the following components:

RDAdmin
A comprehensive application for the administration and configuration
of Rivendell.

RDLibrary
The production audio interface.

RDCatch
The automatic recorder and task scheduler interface.

RDLogin
A small utility for logging users into and out of the system.

RDLogEdit
A air log creation, editing and voicetracker tool.

RDLogManager
A utility for the automatic generation of logs, based on templates.
It also supports importation of schedule information from a wide
variety of third-party traffic and music scheduling systems.

RDAirPlay
The on-air playout application.

RDPanel
A 'fullscreen' SoundPanel cartwall application.

RDCartSlots
An application that emulates a multi-slot cart decks.

RDCastManager
A utility for posting and managing podcast episodes.

RDGpiMon
A utility for monitoring GPI line status.

RDAlsaConfig
A utility for configuring ALSA audio adapters.

RDDbConfig
A utility for creating, backing up and restoring the Rivendell
database.

RDImport
A command-line utility for batch importing audio along with
accompanying metadata. Do:

	man 1 rdimport

for usage help.

RMLSend
A GUI and command-line utility for sending Rivendell Macro Language
(RML) commands.

RDSoftKeys
A lightweight button panel utility for sending RML commands.
