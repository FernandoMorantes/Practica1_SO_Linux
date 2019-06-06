#Regla para compilar el programa para generar los 10000000 de registros iniciales
dataGenerator: dataGenerator.c
		gcc dataGenerator.c -o dataGenerator -lm

#Regla para compilar el servidor
server: p2-dogServer.c
		gcc p2-dogServer.c -o server -lm -lpthread

#Regla para compilar el cliente
client: p2-dogClient.c
		gcc p2-dogClient.c -o client -lm

clean:	
		rm -f p2-program *.o
