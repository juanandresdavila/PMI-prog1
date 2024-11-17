#ifndef LISTAE_CLIENTE_H
#define LISTAE_CLIENTE_H
#define maximo 1000

typedef struct {
    Cliente clientes[maximo];
    int cur;
    int ult;
} Lista_Cliente;

void lc_init(Lista_Cliente *l) {
    l->cur=0;
    l->ult=-1;
}

int lc_isOos(Lista_Cliente l){
    if(l.cur > l.ult){
        return 1;}
    else {
        return 0;}
}

int lc_isEmpty(Lista_Cliente l) {
    if(l.ult==-1) {
        return 1;
    } else {
        return 0;
    }
}

int lc_isFull(Lista_Cliente l) {
    if(l.ult==maximo-1) {
        return 1;
    } else {
        return 0;
    }
}

void lc_forward(Lista_Cliente *l){
    l->cur++;
}

void lc_reset(Lista_Cliente *l) {
    l->cur=0;
}

void lc_insert(Lista_Cliente *l, Cliente c) {
    if (l->ult!=-1) {
        //Shifteo
        for(int i=l->ult+1;i>l->cur; i--){
            l->clientes[i]=l->clientes[i-1];
        }
    }
    l->clientes[l->cur]=c;
    l->ult++;
}

void lc_suppress(Lista_Cliente *l) {
    for(int i=l->cur;i<l->ult;i++){
        l->clientes[i]=l->clientes[i+1];
    }
    l->ult--;
}

Cliente lc_copy(Lista_Cliente l){
    return l.clientes[l.cur];
}

#endif