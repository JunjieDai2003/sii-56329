#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#define RUTA_FIFO "/tmp/fifo_tenis"

int main(int argc, char **argv) {
    int fd;
    char buffer[1024];
    int n;
    // borrar FIFO por si existiera previamente
    unlink(RUTA_FIFO);

    /* crear el FIFO */
    if (mkfifo(RUTA_FIFO,0666)<0) {
        perror("No puede crearse el FIFO");
        return(1);
    }
    printf("Esperando a que se inicie el juego tenis...\n");

    /* Abrir el FIFO */
     fd = open(RUTA_FIFO, O_RDONLY);
    if (fd < 0) {
        perror("No puede abrirse el FIFO");
        unlink(RUTA_FIFO);
        return 1;
    }
    /* Leer de la FIFO */
    while ((n = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[n] = '\0';
        printf("%s", buffer);
        fflush(stdout);
    }
     close(fd);
    unlink(RUTA_FIFO);
    return 0;
}
