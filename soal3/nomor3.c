#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include <sys/types.h> 
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <libgen.h>
#include <pthread.h>
#include <time.h>

int cek_direktori(char *dirr){
    struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir(dirr); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("%s Could not open current directory\n",dirr ); 
        return 0; 
    }else{
        return 1;
    } 
}

void* kategorikan_file(void *filename){
    
    char* ts1 = strdup(filename);
    char* ts2 = strdup(filename);

    char* dir = dirname(ts1);
    char* filename22 = basename(ts2);
    char *ext = strrchr((char*)filename22, '.');
    // printf("cek 1\n");
    // printf("dirar %s\n",dir);
    // printf("fname %s\n",filename22);
    // if(ext != NULL && strcmp((ext+1),"out")!=0){
    // printf("cek 2\n");
    if(strcmp(filename22,"a.out")==0 || strcmp(filename22,"nomor3.c")==0){
        printf("Do Nothing\n");
    }else{
        if (!ext) {     
            printf("Tidak ada extension %s\n",filename22);   
            if(cek_direktori("./Unknown") == 0){
                mkdir("Unknown", 0777);
                printf("buat directory Unknows\n");
                sleep(0.2);
            }
            char temp_dir[100];
            strcpy(temp_dir,"./Unknown/");
            strcat(temp_dir,filename22);
            // printf("direct hasil %s\n",temp_dir);
            if ( rename(filename, temp_dir) ){
                perror( NULL );
                printf("ERROR RENAME\n");
            }
        
        } else { //terdapat extension
            char temp_dir[100];
            strcpy(temp_dir,"./");
            strcat(temp_dir,ext + 1);
            if(cek_direktori(temp_dir) == 0){
                mkdir(ext + 1, 0777);
                printf("buat directory %s\n",ext+1);
                sleep(0.1);
            }
            strcat(temp_dir,"/");
            strcat(temp_dir,filename22);
            // printf("direct hasil %s\n",temp_dir);
            if ( rename(filename, temp_dir) ){
                perror( NULL );
                printf("ERROR RENAME ke %s\n",temp_dir);
            }
            // printf("extension is %s\n", ext + 1);

        }
    }
}

void kategorikan_dir(char*  dirr){
    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;

    dirp = opendir(dirr); /* There should be error handling after this */
    printf("dibuka\n");
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
            file_count++;
        }

        if (!strcmp (entry->d_name, ".")) continue;
        if (!strcmp (entry->d_name, ".."))continue;
    }
    closedir(dirp);
    printf("sudah hitung %d\n",file_count);
    
    pthread_t tid[file_count];
    int i =0 ,y = 0;
        
    dirp = opendir(dirr); /* There should be error handling after this */
    printf("dibuka\n");
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
            char temp_dir[100];
            strcpy(temp_dir,"./");
            strcat(temp_dir,entry->d_name);
            printf("kategorikan %s\n",temp_dir);
            pthread_create(&tid[i], NULL, kategorikan_file, temp_dir);
            printf("buat thread ke %d\n",i);
            i++;
        }

        if (!strcmp (entry->d_name, ".")) continue;
        if (!strcmp (entry->d_name, ".."))continue;
    }
    closedir(dirp);
    

    for(y=0;y<file_count;y++){
        pthread_join(tid[y], NULL);
        printf("tunggu thread ke %d\n",y);
    }

    // sleep(10);
}

int main(int argc, char **argv)
{
    printf("start\n"); 
    // printf("%s\n",argv[1]);
    struct dirent *de;

    
    if(strcmp(argv[1],"-f")==0){
        printf("File Mode batch\n" ); 
        
        pthread_t tid[argc - 2];
        
        int i,y;
        for(i=2,y=0; i<argc; i++,y++){
            if( access( argv[i], F_OK ) != -1 ) { 
                pthread_create(&tid[y], NULL, kategorikan_file, argv[i]);
                 printf("buat thread\n");
            } else {
                printf("not exist\n");
                return -1; 
            }
        }
        for(y=0;y<argc-2;y++){
            pthread_join(tid[y], NULL);
        }
    }else if(strcmp(argv[1],"-d")==0){
        printf("Directory Mode batch\n" );
        kategorikan_dir(argv[2]);
    }else {
        printf("Current Dir Mode batch\n" );
        kategorikan_dir("./");
    }



    // mkdir("geeksforgeeks", 0777);
//   if ( rename("/home/azindait/sisop20/modul3/ngawur.txt", "/home/azindait/sisop20/modul2/ngawur.txt") )
//     perror( NULL );
    // struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    // DIR *dr = opendir("."); 
  
    // if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    // { 
    //     printf("Could not open current directory" ); 
    //     return 0; 
    // } 
  
    // // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
    // // for readdir() 
    // while ((de = readdir(dr)) != NULL) {
    //         printf("%s ", de->d_name); 
    //         printf("%d\n",de->d_type == DT_REG);
    //         char *ext = strrchr(de->d_name, '.');
            
    //         // for(int i = 0; str[i]; i++){
    //         //      str[i] = tolower(str[i]);
    //         // }

    //         if (!ext) {
    //             /* no extension */
    //         } else {
    //             printf("extension is %s\n", ext + 1);
    //         }
    // }
  
     
    return 0;
}