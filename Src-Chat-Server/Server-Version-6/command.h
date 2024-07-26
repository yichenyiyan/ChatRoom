#ifndef COMMAND_H
#define COMMAND_H


enum : char {
    REGISTER            = '1',
    LOGIN               = '2',
    GMESSAGE            = '3',
    ADDFRIEND           = '4',
    PMESSAGE            = '5',
    FRIENDREPLY         = '6',
    GETUSERFRIENDS      = '7',
    UPDATESOCKET        = '8',
    CREATEGROUP         = '9',
    TOJOINGROUNP        = 'a',
    GETUSERJOINEDGROUP  = 'b',
    GETGROUPALLUSERS    = 'c',
    ADDGROUPREPLY       = 'd',
    
};

#endif // COMMAND_H
