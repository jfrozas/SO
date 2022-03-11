/*
 * SO LabAssignment1
 *
 * AUTHOR 1: Guillermo Fernández Sánchez | login: guillermo.fernandezs
 * AUTHOR 2: Javier Fernández Rozas      | login: j.frozas
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <pwd.h>
#include <errno.h>
#include <grp.h>
#include "list.h"


#define MAXLINEA 1024
#define NAMEMAX 32
#define st_atime st_atim.tv_sec
#define st_mtime st_mtim.tv_sec

/*GLOBAL VARIABLES*/
tList list;
bool iscmd;


void ProcesarEntrada(char *tr[]);
int trocearcadena(char * cadena, char * trozos[]);
void printDir(bool, bool , bool , bool , int , char* );

void cmd_autores(char **);
void cmd_carpeta(char **);
void cmd_pid(char **);
void cmd_fin(char **);
void cmd_fecha(char **);
void cmd_hist(char **);
void cmd_comando(char **);
void cmd_infosis(char **);
void cmd_ayuda(char **);
void cmd_crear(char **);
void cmd_borrar(char **);
void cmd_borrarrec(char **);
void cmd_listfich(char **);
void cmd_listdir(char **);



struct CMD{
    char * cmdname;
    void (*func)(char **);
};


struct CMD C[]={
        {"fin", cmd_fin},
        {"bye", cmd_fin},
        {"salir", cmd_fin},
        {"carpeta", cmd_carpeta},
        {"autores", cmd_autores},
        {"pid", cmd_pid},
        {"fecha", cmd_fecha},
        {"hist", cmd_hist},
        {"comando", cmd_comando},
        {"infosis", cmd_infosis},
        {"ayuda", cmd_ayuda},
        {"crear", cmd_crear},
        {"borrar", cmd_borrar},
        {"borrarrec", cmd_borrarrec},
        {"listfich", cmd_listfich},
        {"listdir", cmd_listdir},
        {NULL ,NULL}
};


void cmd_autores (char *tr[]){
/*prints the authors logins and/or names depending on the command's argument*/
    if (tr[0]==NULL){
        printf("The authors are:\nGuillermo Fernandez: guillermo.fernandezs\nJavier Fernandez: j.frozas\n");
    } else if(!strcmp(tr[0],"-l")){
        printf("The authors are:\nguillermo.fernandezs\nj.frozas\n");
    } else if(!strcmp(tr[0],"-n")){
        printf("The authors are:\nGuillermo Fernandez\nJavier Fernandez\n");
    } else printf("Command not found\n");
}


void cmd_pid(char *tr[]){
/*prints the process id of the current shell process or the id of the process' parent*/
    if (tr[0]== NULL) {
        printf("Shell process pid: %d\n",getpid());
    } else if(!strcmp(tr[0],"-p")){
        printf("Shell parent process pid: %d\n",getppid());
    }	else{
        printf("Command not found\n");
    }
}


void cmd_carpeta(char *tr[]){
/*prints the current directory of the shell*/
    char dir[MAXLINEA];

    if(tr[0]== NULL)
        printf("%s\n", getcwd(dir, MAXLINEA));
    else{
        if(chdir(tr[0]) == -1){
            printf("Cannot change dir %s: %s\n", tr[0], strerror(errno));
        } else printf("New current directory: %s\n",getcwd(dir, MAXLINEA));
    }
}


void cmd_fecha(char *tr[]){
/*prints the system time/date*/

    time_t t;
    t = time(NULL);
    struct tm tm;
    tm = *localtime(&t);

    if(tr[0]==NULL){
        printf("%02d/%02d/%d %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    } else if(!strcmp(tr[0],"-d")){
        printf("%02d/%02d/%02d\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    } else if(!strcmp(tr[0],"-h")){
        printf("%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
    }
}


void cmd_infosis(char *tr[]){
/*prints the system information*/
    struct utsname infosis;
    uname(&infosis);

    printf("%s (%s), OS: %s-%s-%s\n",infosis.nodename,infosis.machine,infosis.sysname,infosis.release, infosis.version);
}


void cmd_ayuda(char *tr[]){
/*prints help for different commands*/
    if(tr[0]==NULL) printf("Avaiable commands:\n-> autores\n-> ayuda\n-> borrar\n-> borrarrec\n-> bye\n-> carpeta\n-> comando\n-> crear\n-> fecha\n-> fin\n-> hist\n-> infosis\n-> listdir\n-> listfich\n-> pid\n-> salir\n");
    else if(!strcmp(tr[0],"autores")) printf("autores [-l] [-n]: Prints the names and logins of the program authors.\n\n\t-l\tprints only the logins of the authors.\n\t-n\tprints only the names of the authors.\n\n");
    else if(!strcmp(tr[0],"pid")) printf("pid [-p]: Prints the pid of the process executing the shell.\n\n\t-p\tprints the pid of the shell’s parent process.\n\n");
    else if(!strcmp(tr[0],"carpeta")) printf("carpeta [direct]: Changes the current working directory of the shell to direct. When invoked without auguments it prints the current working directory.\n");
    else if(!strcmp(tr[0],"fecha")) printf("fecha [-d] [-h]: Prints both the current date and the current time.\n\n\t-d\tprints the current date in the format DD/MM/YYYY.\n\t-h\tprints the current time in the format hh:mm:ss.\n\n");
    else if(!strcmp(tr[0],"hist")) printf("hist [-c][-N]: Shows the historic of commands executed by this shell in order.\n\n\t-c\tclears the historic.\n\t-N\tprints the first N comands\n\n");
    else if(!strcmp(tr[0],"comando")) printf("comando N: Repeats command number N\n");
    else if(!strcmp(tr[0],"infosis")) printf("infosis: Prints information on the machine running the shell\n");
    else if(!strcmp(tr[0],"ayuda")) printf("ayuda [cmd]: ayuda displays a list of available commands.\n\n\t[cmd]\tgives a brief help on the usage of comand cmd\n\n");
    else if(!strcmp(tr[0],"fin")) printf("fin: Ends the shell\n");
    else if(!strcmp(tr[0],"bye")) printf("bye: Ends the shell\n");
    else if(!strcmp(tr[0],"salir")) printf("salir: Ends the shell\n");
    else if(!strcmp(tr[0],"crear")) printf("crear [-f] [name]: Creates a directory or file in the file system.\nIf no names are given, shows the current directory\n\n\tcrear [name]\t%s\n\tcrear -f [name]\t%s\n\n","creates an empty directory with name [name]","creates an empty file with name [name]");
    else if(!strcmp(tr[0],"borrar")) printf("borrar name1 name2...: Deletes files and/or empty directories\n");
    else if(!strcmp(tr[0],"borrarrec")) printf("borrarrec name1 name2...: Deletes files and/or non empty directories. If the directory is not empty it is deleted with all its contents\n");
    else if(!strcmp(tr[0],"listfich")){
        printf("listfich [-long] [-link] [-acc] name1 name2 name3...: Gives info on files, directories, etc. entered\nIf no options are given, it prints the size and the name of each file.\nIf no names are given it prints the current directory\n");
        printf("\n\t-long\tprints the information in long listing mode\n\t-link\tif long is applied and the file is a symbolic link, the name of the file it points to is also printed \t\n\t-acc\tlast access time will be used instead of last modification time\n\n");
    }
    else if(!strcmp(tr[0],"listdir")){
        printf("listdir [-reca] [-recb] [-hid] [-long] [-link] [-acc] name1 name2...: Lists the contents of directories with names name1, name2...\nIf no options are given, it prints the size and the name of each file.\nIf no names are given it prints the current directory\nIf the name inserted is a file, it prints the information about the file\n");
        printf("\n\t-long\tprints the information in long listing mode\n\t-link\tif long is applied and the file is a symbolic link, the name of the file it points to is also printed \t\n\t-acc\tlast access time will be used instead of last modification time");
        printf("\n\t-hid\thidden files and/or directories will also get listed\n\t-reca\tprints subdirectories and its content recursively after all the files in the directory\n\t-recb\tprints subdirectories and its content recursively before all the files in the directory\n\n");
    }
    else printf("%s is not a command\n",tr[0]);
}


void cmd_fin(char *tr[]){
/*exits the shell*/
    exit(0);
}


void cmd_hist(char *tr[]){
/*prints the list of the previous commands*/
    LPos pos;
    tItemL item;

    if(tr[0]!=NULL && !strcmp(tr[0],"-c")){
        clearList(&list);
        iscmd=false;
    } else{

        int i=1, n;
        if(tr[0]==NULL){
            n=-MAXLINEA;
        } else n= strtol(tr[0], NULL, 10);

        if(!isEmptyList(list) && n<0){

            pos= first(list);
            while (pos!=LNULL && n<=-i){
                item  = getItem(pos, list);
                printf("%d->", i);
                puts(item.cmdline);
                pos = next(pos, list);
                i++;
            }
        } else printf("The historic is empty or number of commands to print is 0\n");
    }
}

void cmd_comando(char *nchar[]){
/*Repeats the command indicated*/

    int i, N = strtol(nchar[0], NULL, 10);
    if((N==0) | isEmptyList(list)){
        printf("command number not valid\n");
    } else{

        char linea[MAXLINEA];
        char *tr[MAXLINEA / 2];

        tItemL item;
        LPos pos;

        pos= first(list);

        for(i=1; i<N;i++){

            if(next(pos, list)!=NULL){
                pos= next(pos, list);
            } else {
                printf("command number not valid\n");
                pos=NULL;
                break;
            }

        }
        if(pos!=NULL){
            item = getItem(pos, list);
            printf("Repeating command %d:",i);
            puts(item.cmdline);

            strcpy(linea, item.cmdline);
            trocearcadena(linea, tr);
            ProcesarEntrada(tr);
        }
    }
    iscmd=false;
}


void cmd_crear(char *tr[]){
/*creates a file or a directory with name specified*/
    int f;

    if(tr[0]==NULL) cmd_carpeta(tr);//if no name is inserted, the current directory is printed
    else if(!strcmp(tr[0],"-f")){
        if (tr[1]==NULL) cmd_carpeta(tr+1);//if no name is inserted, the current directory is printed
        else{
            if ((f = open(tr[1],O_CREAT | O_EXCL, 0664))==-1){//permissions -rw-rw-r--
                printf("Unable to create file %s: %s\n",tr[1],strerror(errno));
            }
            close(f);
        }
    }
    else {
        if(mkdir(tr[0],0775)==-1){ //permissions drwxrwxr-x
            printf("Unable to create directory %s: %s\n",tr[0],strerror(errno));
        }
    }
}


bool isDir(const char *path){
/*aux function to check if a path corresponds to a directory*/
    struct stat str;
    if(lstat(path, &str)!=-1) {
        if(S_ISDIR(str.st_mode))
            return true;
        else return false;
    }
    else return false;


}


void cmd_borrar(char *tr[]){
/*Deletes an element*/
    int i=0;

    if (tr[i]==NULL) {
        cmd_carpeta(tr);
        return;
    }

    while (tr[i]!=NULL){

        if (isDir(tr[i])){
            if (rmdir(tr[i])==-1) printf("Unable to delete %s: %s\n", tr[i], strerror(errno));
        } else{
            if (unlink(tr[i])==-1) printf("Unable to delete %s: %s\n", tr[i], strerror(errno));
        }
        i++;
    }
}


void deleteDir(const char *path){
/*
 * function deletes recursively a directory with all its files
 * */
    int i=0;
    DIR *d;
    struct dirent *dirStruct;
    char path2[MAXLINEA];

    d = opendir(path);

    if (d) {
        while ((dirStruct = readdir(d)) != NULL) {
            if (strcmp(dirStruct->d_name,".")!=0 && strcmp(dirStruct->d_name,"..")!=0){

                sprintf(path2,"%s/%s",path, dirStruct->d_name);
                i++;
                if (isDir(path2)){
                    deleteDir(path2);
                }else{
                    if (unlink(path2)==-1) printf("Unable to delete %s: %s",path2, strerror(errno));
                }
            }
        }
        closedir(d);
    }

    if(rmdir(path)==-1){
        printf("Unable to delete %s: %s\n",path, strerror(errno));
    }

}


void cmd_borrarrec(char *tr[]){
/*Function to delete a file/directory, if its a non-empty directory, calls deleteDir to delete it*/

    int i=0;

    if (tr[i]==NULL) {
        cmd_carpeta(tr);
        return;
    }

    while (tr[i]!=NULL){

        if(isDir(tr[i])){
            deleteDir(tr[i]);
        } else if (unlink(tr[i])==-1) printf("Unable to delete %s: %s\n",tr[i], strerror(errno));

        i++;
    }
}

char LetraTF (mode_t m){
/*Auxiliar function to ConvierteModo*/
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato, 0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* regular file*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directory */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?';/*unknown (shouldn't appear)*/
    }
}


char * ConvierteModo (mode_t m){
/*Auxiliar function to return the permisions of a file passing its mode_t as argument*/

    static char permisos2[12];
    strcpy (permisos2,"---------- ");
    permisos2[0]=LetraTF(m);
    if (m&S_IRUSR) permisos2[1]='r'; /*owner*/
    if (m&S_IWUSR) permisos2[2]='w';
    if (m&S_IXUSR) permisos2[3]='x';
    if (m&S_IRGRP) permisos2[4]='r'; /*group*/
    if (m&S_IWGRP) permisos2[5]='w';
    if (m&S_IXGRP) permisos2[6]='x';
    if (m&S_IROTH) permisos2[7]='r'; /*rest*/
    if (m&S_IWOTH) permisos2[8]='w';
    if (m&S_IXOTH) permisos2[9]='x';
    if (m&S_ISUID) permisos2[3]='s'; /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos2[6]='s';
    if (m&S_ISVTX) permisos2[9]='t';
    return (permisos2);
}

void printTimeFormat(time_t t){
/*auxiliar function to printFile that prints a time_t type time in a correct format*/
    struct tm tm;
    tm = *localtime(&t);
    printf("%04d/%02d/%02d-%02d:%02d ",tm.tm_year+1900,tm.tm_mon+1, tm.tm_mday,  tm.tm_hour, tm.tm_min);
}


void printFile(bool longListing, bool link, bool acc, char* name){
/*prints the information of a file depending on the options specified*/

    struct stat fileData;
    struct passwd *userInfo;
    struct group *groupInfo;

    char linkName[MAXLINEA]="", userName[NAMEMAX], groupName[NAMEMAX];

    if (!lstat(name, &fileData)){

        if(longListing){
            if(acc){
                printTimeFormat(fileData.st_atime);
            } else {
                printTimeFormat(fileData.st_mtime);
            }

            if((userInfo = getpwuid(fileData.st_uid))!=NULL) sprintf(userName,"%s",userInfo->pw_name);
            else sprintf(userName,"%d",fileData.st_uid);

            if((groupInfo = getgrgid(fileData.st_gid))!=NULL) sprintf(groupName,"%s",groupInfo->gr_name);
            else sprintf(groupName,"%d",fileData.st_gid);

            printf("%2lu (%lu)",fileData.st_nlink, fileData.st_ino);
            printf(" %8s %8s ",userName, groupName);
            printf("%s ", ConvierteModo(fileData.st_mode));
        }

        printf("%9ld %s",fileData.st_size, basename(name));

        if(link && longListing && S_ISLNK(fileData.st_mode)){

            if ((readlink(name, linkName, sizeof(linkName)-1)) != -1){
                printf("->%s", basename(linkName));
            } else
                printf("Cannot access link: %s", strerror(errno));
        }

        printf("\n");
    } else printf("cannot access %s: %s\n",name,strerror(errno));
}


void cmd_listfich(char *tr[]){
/*function to perform the listfich comand*/

    bool longListing=false, link=false, acc=false;
    int i,names=0;

    for (i = 0; i < 3; ++i) {

        if(tr[i]==NULL){
            cmd_carpeta(tr+i);
            return;
        }
        else if(!strcmp(tr[i],"-long")){
            longListing=true;
        } else if (!strcmp(tr[i],"-link")){
            link=true;
        } else if (!strcmp(tr[i],"-acc")){
            acc=true;
        } else{
            break;
        }
    }

    while (tr[i]!=NULL){
        printFile(longListing, link, acc, tr[i]);
        i++;
        names++;
    }

    if (names==0) {
        cmd_carpeta(tr+i);
    }
}


void printSubDirs(bool longlisting, bool link, bool acc, bool hid, int rec, char* path){
/*Auxiliar function to printDir*/

    DIR *d2;
    struct dirent *dirStruct2;
    char path2[MAXLINEA];

    d2 = opendir(path);

    if (d2) {
        while ((dirStruct2 = readdir(d2)) != NULL) {

            sprintf(path2,"%s/%s",path,dirStruct2->d_name);

            if((strcmp(dirStruct2->d_name,".")!=0 && strcmp(dirStruct2->d_name,"..")!=0) && (isDir(path2)) && (hid || dirStruct2->d_name[0]!='.')){
                printDir(longlisting, link, acc,hid,rec, path2);
            }
        }
        closedir(d2);
    }
}


void printDir(bool longlisting, bool link, bool acc, bool hid, int rec, char* path){
/*Function to print the contents from a directory, auxiliar to cmd_listdir*/

    DIR *d;
    struct dirent *dirStruct;
    char path2[MAXLINEA];

    d = opendir(path);

    if (d) {
        if(rec==2){
            printSubDirs(longlisting, link, acc, hid, rec, path);
        }

        printf("************%s\n", path);
        while ((dirStruct = readdir(d)) != NULL) {
            if (hid || dirStruct->d_name[0]!='.'){
                sprintf(path2,"%s/%s",path,dirStruct->d_name);
                printFile(longlisting, link, acc, path2);
            }
        }

        if(rec==1){
            printSubDirs(longlisting, link, acc, hid, rec, path);
        }
    }
    closedir(d);
}


void cmd_listdir(char *tr[]){
/*function to perform the listdir command*/

    bool longListing=false, link=false, acc=false, hid=false;
    int i,names=0,rec=0;
    /*rec=0 if no recursion
     *rec=1 if reca
     *rec=2 if recb
     */

    for (i = 0; i < 5; ++i) {

        if(tr[i]==NULL){
            cmd_carpeta(tr+i);
            return;
        }
        else if(!strcmp(tr[i],"-long") && !longListing){
            longListing=true;
        } else if (!strcmp(tr[i],"-link") && !link){
            link=true;
        } else if (!strcmp(tr[i],"-acc") && !acc){
            acc=true;
        } else if (!strcmp(tr[i],"-hid") && !hid){
            hid=true;
        } else if (!strcmp(tr[i],"-reca") && rec==0){
            rec=1;
        } else if (!strcmp(tr[i],"-recb") && rec==0){
            rec=2;
        } else{
            break;
        }
    }
    while (tr[i]!=NULL){

        if(!isDir(tr[i]))
            printFile(longListing,link,acc,tr[i]);
        else{
            printDir(longListing, link, acc, hid, rec, tr[i]);
        }

        i++;
        names++;
    }

    if (names==0) {
        cmd_carpeta(tr+i);
    }
}


int trocearcadena(char * cadena, char * trozos[]){

    int i=1;

    if((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]= strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

void ProcesarEntrada(char *tr[]){

    iscmd=true;
    bool found=false;

    int i;
    if(tr[0]==NULL) return;
    for (i = 0; C[i].cmdname != NULL; i++) {

        if (!strcmp(tr[0],C[i].cmdname)){
            (*C[i].func)(tr+1);
            found=true;
        }
    }
    if(!found) printf("command not found\n");
}


int main (int argc, char*argv[]) {
    char linea[MAXLINEA];
    char aux[MAXLINEA];
    char *tr[MAXLINEA / 2];

    createList(&list);

    while(1){
        printf("*)");
        fgets(linea, MAXLINEA, stdin);
        tItemL item;

        /*an aux array is created to store linea, then the content of aux is copied into the list*/
        strcpy(aux, linea);
        aux[strlen(aux) - 1] = '\0';

        if (trocearcadena(linea, tr) == 0)
            continue;
        ProcesarEntrada(tr);

        if(iscmd){
            strcpy(item.cmdline, aux);
            insertItem(item, &list);
        }
    }
}