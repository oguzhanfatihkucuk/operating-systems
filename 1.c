#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/sysinfo.h>

#define MAX_SATIR 100 // Maksimum satır sayısı
#define MAX_ALAN 7 // Maksimum alan sayısı

typedef struct Islem {
    char * pid; // Process ID (string)
    int varisSuresi; // Varış Süresi (sn)
    int oncelik; // Öncelik
    int burstZamani; // Burst Süresi (sn)
    int ramKullanimi; // RAM Kullanımı (Mb)
    int cpuKullanimi; // CPU Kullanımı (% oran)
    int baslangicZamani; // Başlangıç Zamanı (sn)
    int bitisZamani; // Bitiş Zamanı (sn)
}
Islem;

int checkCPUResources(int usage) {

    printf("\n");
    FILE * fp;
    char path[1035];

    // Open the command for reading
    fp = popen("top -bn 1 | awk '/^%Cpu/ {print $2+$6}'", "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        return 1;
    }

    // Read the output
    fgets(path, sizeof(path) - 1, fp);
    printf("Total CPU Usage:%%%s", path);
    printf("Process CPU Usage:%%%d\n", usage);
    int cpuUsage = atoi(path); // Convert string to integer

    if ((100 - cpuUsage) < usage) {
        printf("Fail,Your cpu resources are not sufficient for this process.Process will not be eveluated!!!");
        return 0;
    } else {
        printf("Success,Your cpu resources are sufficient for this process.Process will be evalated!!!");
        return 1;
    }

    // Close the pipe
    pclose(fp);

}

int checkRAMResources(int usage) {

    printf("\n");

    struct sysinfo si;
    unsigned long long available_ram, total_ram;
    double available_ram_mb, total_ram_mb;

    sysinfo( & si);

    available_ram = si.freeram;
    available_ram_mb = available_ram / (1024 * 1024);

    total_ram = si.totalram;
    total_ram_mb = total_ram / (1024 * 1024);

    printf("Total RAM: %.2f MB\n", total_ram_mb);
    printf("Available RAM: %.2f MB\n", available_ram_mb);
    printf("Process RAM Usage: %d MB\n", usage);

    if (available_ram_mb - usage > 0) {
        printf("Success!!!,There is enough RAM memory in your system.Process will be put on RAM.\n");
        return 1;
    } else {
        printf("Fail!!!,There is not enough RAM memory in your system.Process will not be put on RAM.\n");
        return 0;
    }
}

int memAllacationCPU1() {
    size_t size512 = 512 * 1024 * 1024; // 512mB in bytes

    void * ptr1 = malloc(size512);

    if (ptr1 == NULL) {
        printf("512mb  memory allocation failed!\n");
        return 0;

    } else {
        printf("Memory Allacation is done successfully for CPU1 as 512 mb !!!\n");
        return 1;
    }

    free(ptr1);
}

int memAllacationCPU2() {

    size_t size512 = 512 * 1024 * 1024; // 512mB in bytes
    size_t size1536 = size512 * 3; // 1536mB in bytes

    void * ptr2 = malloc(size1536);
    if (ptr2 == NULL) {
        printf("1536mb memory allocation failed!\n\n");
        return 0;
    } else {

        printf("Memory Allacation is done successfully for CPU2 as 1536mb !!!\n\n");
        return 1;
    }

    free(ptr2);
}

int main() {
    FILE * dosya;
    char satir[MAX_SATIR];
    char * parca;
    void * ptr;

    // Dizileri dinamik olarak oluşturma
    Islem * islemler = malloc(MAX_SATIR * sizeof(Islem));
    for (int i = 0; i < MAX_SATIR; i++) {
        islemler[i].pid = malloc(MAX_ALAN * sizeof(char));
    }

    int satirSayisi = 0; // Okunan satır sayısı

    dosya = fopen("input.txt", "r");
    if (dosya == NULL) {
        printf("Dosya açılamadı!\n");
        return 1;
    }

    while (fgets(satir, sizeof(satir), dosya)) {
        // Her satırı virgüllerle ayırma
        parca = strtok(satir, ",");

        // Her alanı ilgili yapı üyesine ekleme
        strcpy(islemler[satirSayisi].pid, parca);
        parca = strtok(NULL, ",");
        islemler[satirSayisi].varisSuresi = atoi(parca);
        parca = strtok(NULL, ",");
        islemler[satirSayisi].oncelik = atoi(parca);
        parca = strtok(NULL, ",");
        islemler[satirSayisi].burstZamani = atoi(parca);
        parca = strtok(NULL, ",");
        islemler[satirSayisi].ramKullanimi = atoi(parca);
        parca = strtok(NULL, ",");
        islemler[satirSayisi].cpuKullanimi = atoi(parca);

        satirSayisi++; // Okunan satır sayısını artırma
    }

    fclose(dosya);

    memAllacationCPU1();
    memAllacationCPU2();

    void processyazdir(int i) {

        if (islemler[i].oncelik == 0) {
            checkCPUResources(islemler[i].cpuKullanimi);
            checkRAMResources(islemler[i].ramKullanimi);
            printf("Process %s is queued to be assigned to CPU-1.\nProcess %s is assigned to CPU-1.\nProcess %s is completed and terminated.\n\n\n", islemler[i].pid, islemler[i].pid, islemler[i].pid);

        } else if (islemler[i].oncelik == 1) {
            checkCPUResources(islemler[i].cpuKullanimi);
            checkRAMResources(islemler[i].ramKullanimi);
            printf("Process %s is queued to be assigned to CPU-2.\nProcess %s is assigned to CPU-2.\nProcess %s is completed and terminated.\n\n\n", islemler[i].pid, islemler[i].pid, islemler[i].pid);

        } else if (islemler[i].oncelik == 2) {
            checkCPUResources(islemler[i].cpuKullanimi);
            checkRAMResources(islemler[i].ramKullanimi);

            printf("Process %s is queued to be assigned to CPU-2.\nProcess %s is assigned to CPU-2.\n", islemler[i].pid, islemler[i].pid);

            if (islemler[i].burstZamani < 8) {
                printf("Process %s is completed and terminated.\n\n", islemler[i].pid);
            } else {
                printf("Process %s run until the defined quantum time and is queued again because the process is not completed.\n", islemler[i].pid);
                islemler[i].burstZamani -= 8;
                processyazdir(i);
            }

        } else if (islemler[i].oncelik == 3) {
            checkCPUResources(islemler[i].cpuKullanimi);
            checkRAMResources(islemler[i].ramKullanimi);

            printf("Process %s is queued to be assigned to CPU-2.\nProcess %s is assigned to CPU-2.\n", islemler[i].pid, islemler[i].pid);

            if (islemler[i].burstZamani < 16) {
                printf("Process %s is completed and terminated.\n\n", islemler[i].pid);
            } else {
                printf("Process %s run until the defined quantum time and is queued again because the process is not completed.\n", islemler[i].pid);
                islemler[i].burstZamani -= 16;
                processyazdir(i);

            }
        }
    }

    printf("\n\n");

    for (int i = 0; i < satirSayisi; i++) {
        processyazdir(i);

    }

    printf("\nCPU-1 que1(priority-0) (FCFS)=> ");
    for (int i = 0; i < satirSayisi; i++) {
        if (islemler[i].oncelik == 0) {
            printf("%s", islemler[i].pid);
            printf(",");
        }

    }

    printf("\nCPU-2 que2(priority-1) (SJF)=> ");
    for (int i = 0; i < satirSayisi; i++) {
        if (islemler[i].oncelik == 1) {
            printf("%s", islemler[i].pid);
            printf(",");
        }

    }

    printf("\nCPU-2 que3(priority-2) (RR-q8)=> ");
    for (int i = 0; i < satirSayisi; i++) {
        if (islemler[i].oncelik == 2) {
            printf("%s", islemler[i].pid);
            printf(",");
        }

    }

    printf("\nCPU-2 que4(priority-3) (RR-q16)=> ");
    for (int i = 0; i < satirSayisi; i++) {
        if (islemler[i].oncelik == 3) {
            printf("%s", islemler[i].pid);
            printf(",");
        }

    }

    printf("\n");

    // Dinamik olarak ayrılan belleği serbest bırakma
    for (int i = 0; i < MAX_SATIR; i++) {
        free(islemler[i].pid);
    }

    free(islemler);

    return 0;
}
