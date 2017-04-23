/*
	This code is to be used by echo_s to format the message sent to log_s.
	Insert this code after echo_s has replied to echo_c with the original
		message.
	The final format for the message to be sent to log_s is as follows:
		"original_client_message" was received from client_IP
	See the project specifications and the included screenshot for examples.
		log_s will handle adding the date and time.
	
	This code was written as an excerpt to be inserted into the function
		dostuff() from server_functions.c.
	Code requires access to the variables:
		char buffer[] - the char array holding the original message
		struct sockaddr_in cli_addr - the structure holding client address info
*/
// REQUIRED HEADERS
#include <string.h>
#pragma GCC diagnostic ignored "-Wwrite-strings"

char fmt_msg[256];	// String to hold formatted message. Size can be changed.
char *client_ip;	// String to store client IP address in x.x.x.x format.
// Below line extracts client IP from cli_addr.
client_ip = inet_ntoa(cli_addr.sin_addr);

strcat(fmt_msg, "\"");
strcat(fmt_msg, buffer);
strcat(fmt_msg, "\" was received from "):
strcat(fmt_msg, client_ip);
strcat(fmt_msg, "\n");
