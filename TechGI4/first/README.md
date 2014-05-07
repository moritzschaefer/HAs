# Usage
Code for exercise 7 is in tcpClient.c.
Code for exercise 6 is in {TCP,UDP}ClientAufg6.c
out.txt contains the output of the http client as required in exercise 7.
# Group
This code is written and reviewed by group
Moritz Schäfer 350651
Max Gotthardt 360464
Marco Morik 348689
Tutorium: Mittwoch 14 - 16

# Exercise 6

Send time was: 87017 nanoseconds for UDP client


Send time was: 167389 nanoseconds for TCP client

TCP braucht doppelt so lange, da es weder eine Verbindung aufbauen muss, noch auf eine Bestätigung wartet. Da der Client sich nur zum Localhost verbindet, dauert weder der 3-Wege Handshake noch das senden lange Zeit. Bei einem anderen Host wäre ein deutlicherer Uenterschied erkennbar.
