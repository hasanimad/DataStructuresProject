//
// Created by sdds on 12/26/2023.
//

#ifndef DATASTRUCTURESPROJECT_STRUX_H
#define DATASTRUCTURESPROJECT_STRUX_H

#define info(...) printf("[+] " __VA_ARGS__);
#define error(...) printf("[-] " __VA_ARGS__);
#define okay(...) printf("[+] " __VA_ARGS__);

#ifdef DEBUG
    #define DEBUG_INFO(...) info(__VA_ARGS__)
    #define DEBUG_OKAY(...) okay(__VA_ARGS__)
    #define DEBUG_ERROR(...) error(__VA_ARGS__)
#else
    #define DEBUG_INFO(...)
    #define DEBUG_OKAY(...)
    #define DEBUG_ERROR(...)
#endif


#endif //DATASTRUCTURESPROJECT_STRUX_H
