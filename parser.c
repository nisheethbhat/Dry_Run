/*
 * parser.c
 *
 *  Created on: Jul 6, 2011
 *      Author: nbhat3
 */


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>


#define MAX_CHAR_PER_LINE 100

void main ()
{
	read_file();
	return ;
}


void update_var (char* , int * ,int *);
void update_val (char* ,char*);

int read_file()
{


    FILE * config_file;                    /* FILE poitner for CONFIG FILE                */
    char var[MAX_CHAR_PER_LINE],val[MAX_CHAR_PER_LINE];
                                        /* Temp arrays for values fed from            */
                                        /* configuration file                        */
    int eor_flag,sor_flag;                /* Start of Reg and End of Reg indicator    */

    config_file = fopen ("config","r");
    if (config_file == NULL) {
        perror("config");
        return(1);
    }

    /*********************************************************************
        Read the configuration file and set the global variables
    *********************************************************************/
    sor_flag = 0;
    eor_flag = 0;
    fgets(var,MAX_CHAR_PER_LINE,config_file);
    do {
        printf ("Line Read : %s \n",var);
        update_var((char *)var,&eor_flag,&sor_flag);
        if (sor_flag == 1) {
            do {
            /*********************************************************************
                Reading line after line between <REG and </REG>
            *********************************************************************/
            fgets(var,MAX_CHAR_PER_LINE,config_file);
            printf ("Line Read : %s \n",var);
            update_var((char *)var,&eor_flag,&sor_flag);
            } while (eor_flag == 1);
        }
        sor_flag = 0;
        eor_flag = 0;
    } while (fgets(var,MAX_CHAR_PER_LINE,config_file) != NULL);

    /*********************************************************************
        Closing the file
    *********************************************************************/
    fclose (config_file);
    return(0);
}

void update_var (char* ptr , int* eor, int * sor)
{
    char var[MAX_CHAR_PER_LINE],val[MAX_CHAR_PER_LINE];
                                        /* Temp arrays for values fed                 */
    int offset ;


    /*********************************************************************
        Reading of Data Variables from each line
    *********************************************************************/
    offset = 0;
    do{
        if (*ptr != ' ' ) {
            var[offset] = (char)*ptr;
            offset++;
        }
        ptr++;
    } while (*ptr != '=' && *ptr != '>' && (offset < MAX_CHAR_PER_LINE));

    var[offset] = '\0';
    printf ("Var Read : %s \n",var);
    if (!strcmp(var,"</REG")) {
        *eor = 1;
        return;
    } else if (!strcmp(var,"<REG")) {
        *sor = 1;
        return;
    } else if (!strcmp(var,"LENGTH")) {
        update_val(++ptr,val);
    } else {

    }

    return;
}



void update_val(char * ptr,char * val)
{
    int offset = 0;
    /*********************************************************************
        Reading of Data Values from each line
    *********************************************************************/
    do {
        if (*ptr != ' ' ) {
            val[offset] = (char)*ptr;
            offset++;
        }
        ptr++;
    } while (*ptr != '\0' && (offset < MAX_CHAR_PER_LINE));
    val[offset] = '\0';
    printf ("Val Read : %ld \n",strtoul(val,NULL,16));
}

