#ifndef TURNO_H
#define TURNO_H

// Tipos
typedef struct f {
    short dia;
    short mes;
    short hora;
} Fecha;

typedef struct tr {
    char nombre[64];
    float precio;
} Tratamiento;

typedef struct t {
    int Idturno;
    char nombre[65];
    int idCliente;
    int tratamiento[10];
    short formadepago;
    float total;
    Fecha fecha;
    short realizado;
} Turno;

void initTurno(Turno *t){
    t->Idturno=0;
    strcpy(t->nombre,"");
    t->idCliente=0;
    for(int i=0;i<10;i++){
        t->tratamiento[i]=0;
    }
    t->formadepago=0;
    t->total=0;
    t->fecha.dia=0;
    t->fecha.mes=0;
    t->fecha.hora=0;
    t->realizado=0;
}

// Idturno, nombre, idCliente, trata
// SET / GET >>> FECHA
void setDia(Fecha *t, int dd) {
    t->dia=dd;
}

void setMes(Fecha *t, int mm) {
    t->mes=mm;
}

void setHora(Fecha *t, int hh) {
    t->hora=hh;
}

short getDia(Fecha t) {
    return(t.dia);
}

short getMes(Fecha t) {
    return(t.mes);
}

short getHora(Fecha t) {
    return(t.hora);
}

// SET / GET >>> TRATAMIENTOS
void setNombreTratamiento(Tratamiento *t, char *nom) {
    strcpy(t->nombre,nom);
};

void setPrecioTratamiento(Tratamiento *t, float prec) {
    t->precio=prec;
}

float getPrecioTratamiento(Tratamiento t) {
    return t.precio;
}

char *getNombreTratamiento(Tratamiento t) {
    char *temp;
    temp=(char *)malloc(sizeof(t.nombre)+1);
    strcpy(temp, t.nombre);
    return temp;
    free((void *)temp);
}

// SET / GET >>> TURNO
void setIdTurno(Turno *t, int id) {
    t->Idturno=id;
}

void setIdCliente(Turno *t, int id) {
    t->idCliente=id;
}

void setNombreTurno(Turno *t, char *nombre) {
    strcpy(t->nombre, nombre);
}

// Sera de la forma explicada en info.md
void ToggleTratamiento(Turno *t, short trat) {
    if(t->tratamiento[trat]==1) {
        t->tratamiento[trat]=0;
    } else { 
        t->tratamiento[trat]=1; 
    }
}

void setTotal(Turno *t,float tot) {
    t->total=tot;
}

void setTurnoDia(Turno *t, int dd) {
    setDia(&t->fecha, dd); 
}

void setTurnoMes(Turno *t, int mm) {
    setMes(&t->fecha, mm); 
}

void setTurnoHora(Turno *t, int hh) {
    setHora(&t->fecha, hh); 
}

void setRealizado(Turno *t, short r) {
    t->realizado=r;
}

void setFormaDePago(Turno *t, short f) {
    t->formadepago=f;
}

int getIdTurno(Turno t){
    return(t.Idturno);
}

char *getNombreTurno(Turno t) {
    char *temp;
    temp = (char *)malloc(strlen(t.nombre)+1);
    strcpy(temp, t.nombre);
    return temp;
    free((void *)temp);
}

int getIdCliente(Turno t) {
    return(t.idCliente);
}

// Devolviendo arreglo
int getTratamiento(Turno t,int pos) {
    return(t.tratamiento[pos]);
}

short getFormadepago (Turno t) {
    return (t.formadepago);
}

float getTotal (Turno t){
    return(t.total);
}

short getTurnoDia(Turno t){
    return getDia(t.fecha);
}

short getTurnoMes(Turno t){
    return getMes(t.fecha);
}

short getTurnoHora(Turno t) {
    return getHora(t.fecha);
}

short getRealizado(Turno t){
    return (t.realizado);
}

#endif