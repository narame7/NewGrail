//
//  experiment.cpp
//  NewGrail
//
//  Created by 고상기 on 1/5/16.
//  Copyright © 2016 고상기. All rights reserved.
//


int debug = 0;
char* nfatodfca_name = (char*)""; //for special debug

#include        "include.h"
#include	"lexical.h"
#include	"grail.h"

#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>



int main()
{
    
    srand (time(NULL));
    
    for(int i=0; i<2; i++){
        
        re<char> r;        
        fm<char> a;
        
        std::fstream f_arg;
        std::ostringstream filename;
        filename << "NFAs/input_" << i << ".txt";
        
        
        f_arg.open(filename.str().c_str());
        
        f_arg >> a;
        
        r.fmtore(a);
        std::cout << "\n\n" << r.size() << std::endl;
    }
    
    return 0;
    
}

void
get_one(afa<char>& a, int argc, char** argv, char* my_name)
{
    std::fstream f_arg;
    
    if (argc > 2)
    {
        std::cerr << "usage: " << my_name << "afa\n";
        exit(1);
    }
    
    if (argc == 2)
    {
        f_arg.open(argv[1], std::ios::in);
        if (!f_arg)
        {
            std::cerr << my_name << ": can't open " << argv[1] << "\n";
            exit(1);
        };
        f_arg >> a;
    }
    else
        std::cin >> a;
}

void
get_two(afa<char>& a, afa<char>& b, int argc, char** argv, char* my_name)
{
    std::fstream f_arg;
    std::fstream g_arg;
    
    // handle arguments
    
    switch (argc)
    {
        case 2:
            std::cin >> b;
            f_arg.open(argv[1], std::ios::in);
            if (!f_arg)
            {
                std::cerr << my_name << ": can't open " << argv[1] << "\n";
                exit(1);
            }
            ;
            f_arg >> a;
            break;
            
        case 3:
            f_arg.open(argv[1], std::ios::in);
            if (!f_arg)
            {
                std::cerr << my_name << ": can't open " << argv[1] << "\n";
                exit(1);
            }
            ;
            f_arg >> a;
            f_arg.close();
            
            // we should be able to reuse f_arg, but for some reason
            // it doesn't work under Watcom 9.5
            
            g_arg.open(argv[2], std::ios::in);
            if (!g_arg)
            {
                std::cerr << my_name << ": can't open " << argv[2] << "\n";
                exit(1);
            }
            ;
            
            g_arg >> b;
            break;
            
        default:
            std::cerr << "requires two afa's\n";
            exit(1);
    }
}

void
get_afa(afa<char>& a, int argc, char** argv, char* my_name)
{
    std::fstream f_arg;
    
    if ((argc > 3) || (argc < 2))
    {
        std::cerr << my_name << " need 2 or 3 arguments" << std::endl;
        exit(1);
    }
    
    if (argc == 3)
    {
        f_arg.open(argv[1], std::ios::in);
        if (!f_arg)
        {
            std::cerr << my_name << " can't open " << argv[1] << "\n";
            exit(1);
        };
        f_arg >> a;
    }
    else
        std::cin >> a;
}

// The following two functions were added for cover automata in 3.1
void get_two(fcm<char>& a, fcm<char>& b, int argc, char** argv, char* my_name)
{
    std::fstream f_arg;
    std::fstream g_arg;
    
    // handle arguments
    
    switch (argc)
    {
        case 2:
            std::cin >> b;
            f_arg.open(argv[1], std::ios::in);
            if (!f_arg)
            {
                std::cerr << my_name << ": can't open " << argv[1] << "\n";
                exit(1);
            }
            ;
            f_arg >> a;
            break;
            
        case 3:
            f_arg.open(argv[1], std::ios::in);
            if (!f_arg)
            {
                std::cerr << my_name << ": can't open " << argv[1] << "\n";
                exit(1);
            }
            ;
            f_arg >> a;
            f_arg.close();
            
            // we should be able to reuse f_arg, but for some reason
            // it doesn't work under Watcom 9.5
            
            g_arg.open(argv[2], std::ios::in);
            if (!g_arg)
            {
                std::cerr << my_name << ": can't open " << argv[2] << "\n";
                exit(1);
            }
            ;
            
            g_arg >> b;
            break;
            
        default:
            std::cerr << my_name << ": requires two fm's\n";
            exit(1);
    }
}

void get_one(fcm<char>& a, int argc, char** argv, char* my_name)
{
    std::fstream f_arg;
    
    if (argc > 2)
    {
        std::cerr << "usage: " << my_name << " fm\n";
        exit(1);
    }
    
    if (argc == 2)
    {
        f_arg.open(argv[1], std::ios::in);
        if (!f_arg)
        {
            std::cerr << my_name << ": can't open " << argv[1] << "\n";
            exit(1);
        };
        f_arg >> a;
    }
    else
        std::cin >> a;
}
;

//Added in 3.3.3  Aug. 8th, 2007
//If the first argument is "--help", then print out
// help using 'print_help()', and exit
void print_help(const char* my_name)
{
    
    if (strcmp(my_name, fcmenum) == 0)
    {
        std::cout << "usage: " << my_name << " [-n size] fm\n";
        return;
    }
    
    if (strcmp(my_name, fcmexec) == 0) // Added in 3.1
    {
        std::cout << "usage: " << my_name << " [-d] fm string\n";
        return;
    }
    
    if (strcmp(my_name, fcmmin) == 0)
    {
        std::cout
        << "Minimizes a fcm.  This must be a grail file beginning with the size\
        of the fcm. (i.e. l=4)";
        return;
    }
    
    if (strcmp(my_name, fcmtofm) == 0)
    {
        std::cout << "Converts an fcm to fm";
        return;
    }
}


void print_version(char* my_path)
{
    std::cout<<Version<<"\n";
}
