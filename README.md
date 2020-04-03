# SoalShiftSISOP20_modul3_E08

Penyelesaian Soal Shift Modul 3 SISOP 2020

Achmad Zidan Akbar / 05111840000005

M RIDHO DAFFA ARDISTA / 05111840000065	

#Note : 
Nomor 1, bagian shop,item,inventory belum selesai   
Nomor 3, Race Condition saat mode directory   


<h3>Nomor 1</h3>

pokezone :

    #include <stdio.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #include <unistd.h>
    #include <pthread.h>
    #include <wait.h>
    #include <stdlib.h>
    #include<sys/types.h>
    #include<sys/wait.h>
    #include <string.h>
    #include <time.h> 

    pthread_t tid[5];
    int item[3] = {100,100,100};
    int capture,notEscape = 0,escape = 0;
    int rarity[3],get_pokedollar = 0;
    char get_rarity[10];
    char get_pokemonName[15];


    // void shutdown()
    // {
    //     pid_t child_id1,child_id2;
    //     child_id1 = fork();
    //     if(child_id1 == 0)
    //     {
    //         char* argv[] {"kill", "pid", NULL};
    //         execv("/bin/kill", argv);
    //     }
    //     child_id2 = fork();
    //     if(child_id2 = 0)
    //     {
    //         char* argv[] {"kill", "pid", NULL};
    //         execv("/bin/kill", argv);
    //     }
    // }

    //kurang list item
    void* effectItem(void *arg)
    {
    //gagenah
        int i;
        pthread_t id = pthread_self();
        char* namaItem[3]={"Lullaby Powder","Pokeball","Berry"};
        char* deskripsiItem[3]={"+20% Capture rate, 0% Escape rate kedua efek bertahan 10 detik","Menangkap pokemon","+10 AP semua pokemon"};
        int hargaItem[3]={60,5,15};
        if(pthread_equal(id,tid[0]))
        {
            while (1)
            {
                sleep(10);
                for(i = 0;i<3;i)
                {
                    if(item[i] < 200) item[i]+10;
                }
            }
        }
        //Lullaby Powder
        if(pthread_equal(id,tid[1]))
        {
            capture+=20;
            notEscape=1;
            sleep(10);
            notEscape=0;
            capture-=20;
        }
        if(pthread_equal(id,tid[2]))
        {
            for(i = 0;i<3;i++)
            {
                printf("%d. %s %s %d %d\n",i+1,namaItem[i],deskripsiItem[i],hargaItem[i],item[i]);
            }
        }
    }
    //cek
    void* pokemon(void *argv)
    {
        char pokemonName[15][15] = {"Bulbasaur","Charmander","Squirtle","Rattata","Caterpie",
                                "Pikachu","Eevee","Jigglypuff","Snorlax","Dragonite",
                                "Mew","Mewtwo","Moltres","Zapdos","Articuno"};
        char rarityName[3][10] = {"Normal","Rare","Legendary"};
        int escapeRate[3] = {5,10,20};
        char temp[2] = "";
        int batasRarity[4] = {0,80,95,100};
        int captureChance[3] = {70,50,30};
        int pokedollar[3] = {80,100,200};
        int i,j,k,encounter,index_pokemonName;
        pthread_t id = pthread_self();
        if(pthread_equal(id,tid[0]))
        {
            
            while (1)
            {
            // i = 2;
                encounter = rand() % 100;
                //printf("%d ",encounter);
                // temp = temp / (j+1)+1*j;
                    //printf("%d",temp);
                for(i = 0; i <=3; i++)
                {
                    if(encounter < batasRarity[i+1] && encounter >= batasRarity[i])
                    {
                        rarity[i] = 1;
                        escape = escapeRate[i]; 
                        capture = captureChance[i];
                        get_pokedollar = pokedollar[i];
                        strcpy(get_rarity,rarityName[i]);
                        int cek_shiny = rand () % 8000;
                        if(cek_shiny < 1)
                        {
                            get_pokedollar+=5000;
                            capture-=20;
                            rarity[i] * 10;
                            strcpy(temp,"*");
                            escape +=5;
                        }
                    //printf("%d ",rarity[i]);
                        int pokemonName_rate = rand() % 100;
                        for(j = 20,k=0; j<= 100; j+=20,k++)
                        {
                            //printf("%d ",pokemonName_rate);
                            if(pokemonName_rate < j) 
                            {
                                strcpy(get_pokemonName,pokemonName[k+5*i]);
                                if(rarity[i]=10) strcat(get_pokemonName, temp);
                                //printf("%s ",get_pokemonName);
                                break;
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
    }

    int main()
    {
        srand(time(NULL)); 
        //int i=0;
        // while(i<3) // loop sejumlah thread
        // // {
        key_t key1 = 1,key2 = 2,key3 = 3,key4 = 4,key5 = 5;
        
        while (1)
        {
        int *escapePokemon;
        int *capturePokemon;
        int *pokedollarmon;
        pthread_create(&(tid[0]),NULL,&pokemon,NULL);
        pthread_join(tid[0],NULL);
    // printf("%d",escape);

        int shmid1 = shmget (key1, sizeof(int), IPC_CREAT | 0666);
        int shmid2 = shmget (key2, sizeof(int), IPC_CREAT | 0666);
        int shmid3 = shmget (key3, sizeof(int), IPC_CREAT | 0666);
        int shmid4 = shmget (key4, sizeof(int), IPC_CREAT | 0666);
        int shmid5 = shmget (key5, sizeof(int), IPC_CREAT | 0666);

        char *send_namaPokemon = (char *) shmat(shmid1, NULL, 0);
        char *send_rarity = (char *) shmat(shmid2, NULL, 0);
        escapePokemon = shmat(shmid3, NULL, 0);
        capturePokemon = shmat(shmid4, NULL, 0);
        pokedollarmon = shmat(shmid5, NULL,0);

        strcpy(send_namaPokemon,get_pokemonName);
        strcpy(send_rarity,get_rarity);
        *escapePokemon = escape;
        *capturePokemon = capture;
        *pokedollarmon = get_pokedollar;

        shmdt(pokedollarmon);
        shmdt(capturePokemon);
        shmdt(send_namaPokemon);
        shmdt(send_rarity);
        shmdt(escapePokemon);

        sleep(1);
        }
    // shmdt(get_pokemonName);
    

        //pthread_join(tid[1],NULL);
        //exit(0);
        //return 0;
    }

traizone :

    #include <stdio.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #include <unistd.h>
    #include <pthread.h>
    #include <wait.h>
    #include <stdlib.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <string.h>
    #include <signal.h>
    #include <time.h> 


    pthread_t tid[3];
    int stop=0;
    char listPokemon[7][15];
    int rarity[3];
    int ap[7];
    int item[3] = {0,20,0};
    int capture_mode = 0,search_mode = 0;
    int state = 0,escaped = 0,ketangkap = 0,myMoney = 100;
    char* namaPokemon;
    char* rarityPokemon; 
    int* escapePokemon;
    int* capturePokemon;
    int* pokedollarmon;


    //srand

    void menu()
    {
        pid_t child_id;
        int status;
    // int *ptr[2];
        child_id = fork();
        if(child_id == 0)
        {
                
            char *argv[] = {"clear", NULL};
            execv("/usr/bin/clear", argv);
        }
        else
        {    
            while ((wait(&status)) > 0);
            if(state != 1) printf("1. Cari Pokemon\n");
            else if(capture_mode == 1) printf("1. Capture\n");
            else printf("1. Berhenti Mencari\n");
            printf("2. Pokedex\n3. Shop\n");
            if(escaped == 1) printf("\n Pokemon melarikan diri\n");
            
        }
    
    }


    void pokedex()
    {
        int i;
        int pilDex;
        pid_t child_id;
        int status;
        while (1)
        {
        
        child_id = fork();
        if(child_id == 0)
        {
                
            char *argv[] = {"clear", NULL};
            execv("/usr/bin/clear", argv);
        }
        while ((wait(&status)) > 0);
        for(i=0; i<7; i++)
        {
            if(strcmp(listPokemon[i],"1")!=-1) printf("%d. %s %d\n",i+1,listPokemon[i],ap[i]);
            else break;
        }
            if(strcmp(listPokemon[0],"1")==-1) printf("Anda tidak memiliki pokemon\n3. Kembali\n");
            else
            printf("\n1. Lepas Pokemon\n2. Beri Berry\n3. Kembali\n");
            scanf("%d",&pilDex);
            if(pilDex == 1)
            {
                int lepas;
                printf("Pilih index Pokemon:\n");
                for(i = 0; i<7; i++)
                {
                    if(strcmp(listPokemon[i],"1")!=-1) printf("%d. %s %d\n",i+1,listPokemon[i],ap[i]);
                }
                scanf("%d",&lepas);
    //check???
                for(i = lepas - 1; i<7-1; i++) 
                {
                    strcpy(listPokemon[i],listPokemon[i+1]);
                    ap[i] = ap[i+1];
                }
                strcpy(listPokemon[6],"0");
            }
            else if(pilDex == 2)
            {
                for(i = 0; i<7; i++) if(strcmp(listPokemon[i],"1")==-1) ap[i]+=10;
                else break;
            }
            else if (pilDex == 3)
            {
                state = 0;
                break;
                
            }
        }
        
    }

    void* apDecrement(void* arg)
    {
        int i,index;
        for(i = 6; i<= 0;i--) if(strcmp(listPokemon[i],"1")==-1)
        {
            index = i;
            break;
        } 
        pthread_t id = pthread_self();
        if(pthread_equal(id,tid[index]))
        {
            while (1)
            {
                if(capture_mode == 0)
                {
                sleep(1);
                ap[index]-=10;
                // int status;      
                // pid_t child_id;
                // child_id = fork();
                // if(child_id == 0)
                // {       
                //     char *argv[] = {"clear", NULL};
                //     execv("/usr/bin/clear", argv);
                // }
                // while ((wait(&status)) > 0);
                // pokedex();
                if(ap[index] <= 0)
                {
                    int lepas = rand() % 100;
                    if(lepas < 90)
                    {
                        for(i = index - 1; i<7-1; i++) 
                        {
                            strcpy(listPokemon[i],listPokemon[i+1]);
                            ap[i] = ap[i+1];
                        }
                    }
                    else if(lepas>=0 && lepas <10)
                    {
                        ap[index] = 50;
                    }
                }
                }
            }
        }

    }

    void* tangkap(void *arg)
    {
        pthread_t id = pthread_self();
        int capturing = *capturePokemon;
        if(pthread_equal(id,tid[0]))
        {
            int chance;
            chance = rand() %10;
            if(chance < capturing)
            {
                ketangkap = 1;
            }
        }
    }

    void capture()
    {
        ketangkap = 0;
        char PokemonTertangkap[15] = " ";
        char rarityTertangkap[10]= " ";
        strcpy(PokemonTertangkap,namaPokemon);
        strcpy(rarityTertangkap,rarityPokemon);
        int pilihan_capture;
        int status,i;
        pid_t child_id = fork();
        if(child_id == 0)
        {
                
            char *argv[] = {"clear", NULL};
            execv("/usr/bin/clear", argv);
        }
        while ((wait(&status)) > 0);
        printf("----Capturing----\n");
        printf("(%s) %s\n\n",rarityPokemon,namaPokemon);
        printf("1. Tangkap\n2. Item\n3. Keluar\n");
        while (1)
        {
            if(escaped == 0)
            {
                scanf("%d",&pilihan_capture);
                if(pilihan_capture == 1)
                {
                    if(item[1] > 0)
                    {
                        item[1]--;
                        pthread_create(&(tid[0]),NULL,&tangkap,NULL);
                        if(ketangkap == 1)
                        {
                            printf("\n(%s) %s berhasil ditangkap\n",rarityTertangkap,PokemonTertangkap);
                            for(i = 0; i<7; i++)
                            {
                                if(strcmp(listPokemon[i],"1")==-1) 
                                {
                                    strcpy(listPokemon[i],PokemonTertangkap);
                                    ap[i] = 100;
                                    pthread_create(&(tid[i]),NULL,&apDecrement,NULL);

                                    break;
                                }
                            }
                            capture_mode = 0;
                            stop = 1;
                            state = 0;
                            //escaped == 0;
                            printf("\nTekan enter untuk kembali ke menu\n");
                            getchar();
                            getchar();
                            break;
                        }
                        printf("Sisa pokeball %d\n",item[1]);

        //tangkap
                    }
                    else {
                        printf("pokeball telah habis\n");
                    }
                }
                else if(pilihan_capture == 2)
                {
                    int pilihan_item;
                    printf("Item yang bisa digunakan:\n1. lullaby powder");
                    scanf("%d",&pilihan_item);
                    if(pilihan_item == 1)
                    {
                        item[0]--;
            //efek
                    }
                }
                else if(pilihan_capture == 3)
                {
                    capture_mode = 0;
                    stop = 1;
                    state = 0;
                    menu(state);
                    
                    break;
                }
            }
            else if(escaped == 1)
            {
                capture_mode = 0;
                stop = 1;
                state = 0;
                //escaped == 0;
                menu(state);
                
                break;
            }
        }
        return;
    }


    void shop()
    {
        ///
    }


    void* cariPokemon(void *arg)
    {
        pthread_t id = pthread_self();
        if(pthread_equal(id,tid[0]))
            {
    //tes 
                while (1)
                {
                    int get;
                        sleep(1);
                        get = rand() %100;
                    // printf("%d",get);
                    if(stop != 1)
                    {
                        if(get<60)
                        {
                            printf("o");
                            fflush(stdout);

    //bool???               
                            capture_mode =1;
                            search_mode = 0;
                            stop = 1;
                            menu();
                            //fflush(stdin);
                            //capture();
                        }
                        else
                        {
                            printf("-");
                            fflush(stdout);
                        }
                    }
                    else
                    {
                        break;
                    // pthread_exit(&get);
                        
                    }
                    
                
                }
                
                //sleep(1);            
            }
            return NULL;
    }

    void* escape(void *arg)
    {
        int escapeRate = *escapePokemon;
        escaped == 0;
        pthread_t id = pthread_self();
        if(pthread_equal(id,tid[0]))
        {
            if(capture_mode == 1)
            {
                while(1)
                {
    //check          
                    sleep(20);
                    int rate;
                    rate = rand() %100;
                    if(rate < escapeRate) 
                    {
                        escaped = 1;
    //kurang input interupt
                        capture_mode = 0;
                        break;
                    }    
                }
            }
        }
    }



    void main(void)
    {
        srand(time(NULL)); 
        key_t key1 = 1, key2 = 2,key3 = 3, key4 =4,key5 = 5;
    //int *tes;
        int shmid1= shmget(key1, sizeof(int) , IPC_CREAT | 0666);
        int shmid2= shmget(key2, sizeof(int) , IPC_CREAT | 0666);
        int shmid3= shmget(key3, sizeof(int) , IPC_CREAT | 0666);
        int shmid4 = shmget (key4, sizeof(int), IPC_CREAT | 0666);
        int shmid5 = shmget (key5, sizeof(int), IPC_CREAT | 0666);

        namaPokemon = (char*)shmat(shmid1, NULL, 0);
        rarityPokemon = (char*)shmat(shmid2, NULL, 0);
        escapePokemon = shmat(shmid3, NULL, 0);
        capturePokemon = shmat(shmid4, NULL, 0);
        pokedollarmon = shmat(shmid5, NULL, 0);

        //printf("%s %s %d %d %d",rarityPokemon, namaPokemon,*escapePokemon,*capturePokemon,*pokedollarmon);

        // // //int state =0 ;
        memset(listPokemon,'0',sizeof(listPokemon));
        //printf("%d ",strcmp(listPokemon[1],"1"));
        while (1) 
        {
            menu(state);
            if(state == 0)
            scanf("%d",&state);
            if(state == 1)
            {
                menu(state);
                //printf("----Searching----\n");
                stop = 0;
                int ss;
                search_mode = 1;
                if(stop !=1)
                pthread_create(&(tid[0]),NULL,&cariPokemon,NULL);
                //pthread_join(tid[0],NULL);
                capture_mode == 0;
                if(search_mode ==1)
                {
                    scanf("%d",&ss);
                    //else capture();
                    while(1)
                    {
                        if(capture_mode == 0)
                        {
                            if(ss==1) 
                            {
                                stop =1;
                                menu(state);
                                state=0;
                                search_mode = 0;
                                break;
                            }
                        }
                        else if(capture_mode == 1)
                        {
                            if(ss == 1)
                            {
                                stop =1;
                                pthread_create(&(tid[0]),NULL,&escape,NULL);
                                state=0;
                                search_mode = 0;
                                capture();
                                break;
                            }
                        }
                    }
                }
                //else capture();
            }
            else if(state == 2)
            {
                pokedex();
            }
            else if(state == 3)
            {
                shop();
            }
        }

        shmdt(rarityPokemon);
        shmdt(namaPokemon);
        shmdt(escapePokemon);
        shmdt(capturePokemon);
        shmdt(pokedollarmon);

        shmctl(shmid1, IPC_RMID, NULL);
        shmctl(shmid2, IPC_RMID, NULL);
        shmctl(shmid3, IPC_RMID, NULL);
        shmctl(shmid4, IPC_RMID, NULL);
        shmctl(shmid5, IPC_RMID, NULL);
    }

<h3>Nomor 3</h3>

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



        
        return 0;
    }

