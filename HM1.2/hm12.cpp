#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    int fd;
    int buf;
    fd = open("/dev/test",O_RDONLY);
    for (int i=0;i<2;i++){
        read(fd,&buf,sizeof(buf));
        printf("Input:>>> %d <<<\n",buf);
    }
    
}
