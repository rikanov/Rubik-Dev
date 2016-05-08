#include "auxiliary.h"
#include "def_colors.h"

void auxiliary::intro()
{
  OUT_(Color::blue)
  OUT_("\t\t ________  ___  ___  ________  ___  ___  __    ________       ")
  OUT_("\t\t|\\   __  \\|\\  \\|\\  \\|\\   __  \\|\\  \\|\\  \\|\\  \\ |\\   ____\\      ")
  OUT_("\t\t\\ \\  \\|\\  \\ \\  \\\\\\  \\ \\  \\|\\ /\\ \\  \\ \\  \\/  /|\\ \\  \\___|_     ")
  OUT_("\t\t \\ \\   _  _\\ \\  \\\\\\  \\ \\   __  \\ \\  \\ \\   ___  \\ \\_____  \\    ")
  OUT_("\t\t  \\ \\  \\\\  \\\\ \\  \\\\\\  \\ \\  \\|\\  \\ \\  \\ \\  \\\\ \\  \\|____|\\  \\   ")
  OUT_("\t\t   \\ \\__\\\\ _\\\\ \\_______\\ \\_______\\ \\__\\ \\__\\\\ \\__\\____\\_\\  \\  ")
  OUT_("\t\t    \\|__|\\|__|\\|_______|\\|_______|\\|__|\\|__| \\|__|\\_________\\ ")
  OUT_("\t\t                                                 \\|_________| ")
  
  OUT_(Color::red)
  OUT_("\t\t        ________  ___  ___  ________  _______                 ")
  OUT_("\t\t       |\\   ____\\|\\  \\|\\  \\|\\   __  \\|\\  ___ \\                ")
  OUT_("\t\t       \\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\|\\ /\\ \\   __/|               ")
  OUT_("\t\t        \\ \\  \\    \\ \\  \\\\\\  \\ \\   __  \\ \\  \\_|/__             ")
  OUT_("\t\t         \\ \\  \\____\\ \\  \\\\\\  \\ \\  \\|\\  \\ \\  \\_|\\ \\            ")
  OUT_("\t\t          \\ \\_______\\ \\_______\\ \\_______\\ \\_______\\           ")
  OUT_("\t\t           \\|_______|\\|_______|\\|_______|\\|_______|           ")
  OUT_("\t\t                                                     ")
  OUT_(Color::gray<<"\t\t  This is a free software licensed under the  "<<Color::yellow<<" ██╗     ██╗███████╗██████╗  ")
  OUT_(Color::gray<<"\t\t                                              "<<Color::yellow<<" ██║     ██║██╔════╝██╔══██╗ ")
  OUT_(Color::white<<"\t\t       GNU GENERAL PUBLIC LICENSE             "<<Color::yellow<<" ██║     ██║███████╗██████╔╝ ")
  OUT_(Color::gray<<"\t\t TO LEARN MORE ABOUT THE TERMS AND CONDITIONS "<<Color::yellow<<" ██║     ██║╚════██║██╔═══╝  ")
  OUT_(Color::gray<<"\t\t PLEASE READ THE ATTACHED LICENSE FILE        "<<Color::yellow<<" ███████╗██║███████║██║      ")
  OUT_(Color::gray<<"\t\t CONTAINED IN THE SOURCE FOLDER               "<<Color::yellow<<" ╚══════╝╚═╝╚══════╝╚═╝      ")
  OUT_(Color::white<<"\t\t                                                             (v "<<CURRENT_VERSION_NUMBERS[0]<<'.'<<CURRENT_VERSION_NUMBERS[1]<<'.'<<CURRENT_VERSION_NUMBERS[2]<<')')
  OUT_(Color::gray<<NL);

}