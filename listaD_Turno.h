#ifndef LISTAD_TURNO_H
#define LISTAD_TURNO_H

// Typedefs
struct nodo {
    Turno vipd;
    struct nodo *next;
};
typedef struct nodo Nodo;

typedef struct {
    Nodo *acc;
    Nodo *cur;
    Nodo *aux;
} Lista_Turno;

// Init
void lt_init(Lista_Turno *l) {
    l->cur=NULL;
    l->acc=NULL;
    l->aux=NULL;
}

void lt_insert(Lista_Turno *l, Turno t) {
    Nodo *nuevo=(Nodo *)malloc(sizeof(Nodo));
    nuevo->vipd=t;
    if(l->cur==l->acc) {
        nuevo->next=l->cur;
        l->acc=nuevo;
        l->cur=nuevo;
        l->aux=nuevo;
    } else {
        nuevo->next=l->cur;
        l->aux->next=nuevo;
        l->cur=nuevo;
    }
} 

Turno lt_copy(Lista_Turno l) {
    return (*l.cur).vipd;
}

void lt_suppress(Lista_Turno *l){
    if(l->acc==l->cur) {
        l->acc=l->cur->next;
        free(l->cur);
        l->aux=l->acc;
        l->cur=l->acc;
    } else {
        l->aux->next=l->cur->next;
        free(l->cur);
        l->cur=l->aux->next;
    }
    
}

int lt_isFull() {
    Nodo *x=(Nodo *)malloc(sizeof(Nodo));
    if(x==NULL) {
        return 1;
    } else {
        free((void *)x);
        return 0;
    }
}

int lt_isEmpty(Lista_Turno l) {
    return(l.acc==NULL);
}

int lt_isOos(Lista_Turno l){
    if(l.cur==NULL) {
        return 1;
    } else {
        return 0;
    }
}

void lt_forward(Lista_Turno *l) {
    l->aux=l->cur;
    l->cur=l->cur->next;
}

void lt_reset(Lista_Turno *l) {
    l->cur=l->acc;
    l->aux=l->acc;
}

// Fin
#endif