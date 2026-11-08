#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "DatosMemCompartida.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#define RUTA_MEM "/tmp/mem_tenis"

int main(int argc, char* argv[])
{
	DatosMemCompartida* dat;
	char *direccion;
	int fd;

	// 1. Abrir el archivo compartido en lectura y escritura
	fd = open(RUTA_MEM, O_RDWR);
	if (fd == -1)
	{
		perror("Error al abrir el archivo compartido (ejecuta tenis primero)");
		return 1;
	}

	// 2. Proyectar el archivo en memoria RAM compartida
	direccion = (char*)mmap(NULL, sizeof(DatosMemCompartida), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (direccion == MAP_FAILED)
	{
		perror("Error en la proyeccion mmap");
		close(fd);
		return 1;
	}

	// 3. Ya proyectado en RAM, podemos cerrar el descriptor de archivo
	close(fd);

	// 4. Apuntar nuestro puntero 'dat' a esa zona de memoria compartida
	dat = (DatosMemCompartida*)direccion;

	printf("Bot iniciado y controlando la raqueta 1...\n");

	// 5. Bucle infinito cada 25 ms (25000 microsegundos)
	while (1)
	{
		usleep(25000);

		float centro_raqueta = (dat->raqueta1.y1 + dat->raqueta1.y2) / 2.0f;

		if (dat->esfera.centro.y > centro_raqueta)
			dat->accion = 1;  // Pelota más alta que la raqueta -> subir
		else if (dat->esfera.centro.y < centro_raqueta)
			dat->accion = -1; // Pelota más baja que la raqueta -> bajar
		else
			dat->accion = 0;  // A la misma altura -> nada
	}

	return 0;
}
