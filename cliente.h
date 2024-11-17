#ifndef CLIENTE_H
#define CLIENTE_H

// Tipos
typedef struct c {
    int IdCliente;
    char nombre[32];
    char apellido[32];
    int cantidadTratamientos;
    short nivel;
} Cliente;

void init(Cliente *c){
    c->IdCliente=0;
    strcpy(c->nombre, "");
    strcpy(c->apellido, "");
    c->cantidadTratamientos=0;
    c->nivel=0;
}
// SET / GET >>> CLIENTE

void setId(Cliente *c, int id){
    c->IdCliente=id;
}

void setNombreCliente(Cliente *c, char *nomb){
    strcpy(c->nombre, nomb);
}

void setApellidoCliente(Cliente *c, char *app){
    strcpy(c->apellido, app);
}

void setCantidadTratamientos(Cliente *c, int cant){
    c->cantidadTratamientos=cant;
}

void setNivelCliente(Cliente *c, short niv) {
    c->nivel=niv;
}

int getId(Cliente c){
    return (c.IdCliente);
}

char *getNombreCliente(Cliente c){
    char *temp;
    temp =(char *)malloc(strlen(c.nombre)+1);
    strcpy(temp, c.nombre);
    return temp;
}

char *getApellidoCliente(Cliente c){
    char *temp;
    temp =(char *)malloc(strlen(c.apellido)+1);
    strcpy(temp, c.apellido);
    return temp;
}

int getCantidadTratramientos(Cliente c){
    return (c.cantidadTratamientos);
}

short getNivel(Cliente c){
    return (c.nivel);
}

// Fin
#endif