// Librerias
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include "cliente.h"
#include "turno.h"
#include "listaE_Cliente.h"
#include "listaD_Turno.h"
// Variables locales
#define CANTIDADTRATAMIENTOS 10

// Prototipos
void almacenarTurnosRealizados(Lista_Turno *lc, short formaPago, Tratamiento t[]);
void ActualizarNivel(Cliente *c);
int buscarClienteDNI(Lista_Cliente *l, int DNI);
int buscarClienteNyA(Lista_Cliente *l, char *nombre, char *apellido);
int buscarTurnoIDCliente(Lista_Turno *lt, int idcli);
void confirmarTurnoCliente(Lista_Cliente *lc, Lista_Turno *lt, Tratamiento t[], int idcli);
int clienteConTurnoPendiente(Lista_Turno *lt, int idcli);
void formadePagoTurno(Lista_Turno *lt, int idcliente, short nvaFormaPago, Tratamiento t[]);
void eliminarCLienteID(Lista_Cliente *lc, int IDcliente);
void eliminarTurnoIDCliente(Lista_Turno *lt, Tratamiento t[], int id);
float gananciaMensual(Lista_Turno *lt, int mes);
void initTratamientos(Tratamiento t[]);
void liberarTurnos(Lista_Turno *l);
int IsHorarioFree(Lista_Turno *lt, short dd, short mm, short hh);
void modificarCantidadTratamientos(Lista_Cliente *lc, int idcliente, int nuevacantTrat);
void MostrarTurno(Lista_Turno *lt, Tratamiento t[]);
void mostrarTurnosTrat(Lista_Turno *lt, int Nrotrat, Tratamiento t[]);
void mostrarTurnosMes(Lista_Turno *lt, short mm, Tratamiento tratamientos[]);
void mostrarTurnosCliente(Lista_Turno *lt, char nombre[], Tratamiento tratamientos[]);
void mostrarListaTurnos(Lista_Turno *lt, Tratamiento tratamientos[]);
char *NombremetodoPago(int met);
void precargaClientes(Lista_Cliente *lc);
void registrarTurno(Lista_Cliente *lc, Lista_Turno *lt, int *cantTurnos, Tratamiento tratamientos[]);
void registrarCliente(Lista_Cliente *l);
void recurMostrarTurno(Lista_Turno *lt, int id, int acum);

// Actualizar nivel del cliente
void ActualizarNivel(Cliente *c){
    if(getCantidadTratramientos(*c)<4) {
        setNivelCliente(c,1);
    } else {
        if(getCantidadTratramientos(*c)<=10) {
            setNivelCliente(c,2);
        } else {
            setNivelCliente(c,3);
        }
    }
    return;
}

// Inicializacion de tratamientos
void initTratamientos(Tratamiento t[]) {
    FILE *tratamientos = fopen("tratamientos.txt", "r");
    if (tratamientos == NULL)
    {
        printf("Error: No se pudo abrir el archivo.\n");
        return;
    }
    for (int i = 0; i < 10; i++)
    {
        fgets(t[i].nombre, sizeof(t[i].nombre), tratamientos);
        t[i].nombre[strcspn(t[i].nombre, "\n")] = '\0';
        fscanf(tratamientos, "%f", &t[i].precio);
        fgetc(tratamientos);
    }

    fclose(tratamientos);
}

// Liberador de turnos
void liberarTurnos(Lista_Turno *l) {
    lt_reset(l);
    while (!lt_isEmpty)
    {
        lt_suppress(l);
    }
}

// Devolver metodo de pago
char *NombremetodoPago(int met) {
    char *temp=(char *)malloc(10);
    if(temp==NULL) {
        printf("Error: Memoria insuficiente");
        return "";
    }
    switch (met){
        case 1:
            strcpy(temp, "Debito");
            break;
        case 2:
            strcpy(temp, "Credito");
            break;
        case 3:
            strcpy(temp, "QR");
            break;
        case 4:
            strcpy(temp, "Efectivo");
            break;
        }
    return temp;
    free((void *)temp);
}

char *nomcompleto(char *nombre, char *apellido) {
    char *temp=(char *)malloc(65);
    if(temp==NULL) {
        printf("Error: Memoria insuficiente");
        return "";
    }
    strcpy(temp,nombre);
    strcat(temp, " ");
    strcat(temp, apellido);
    return temp;
    free((void *)temp);
}

// Mostrar el turno en el cursor
void MostrarTurno(Lista_Turno *lt, Tratamiento t[]) {
    Turno *tempo=(Turno *)malloc(sizeof(Turno));
    if(tempo==NULL) {
        printf("Error: Memoria insuficiente");
        getchar();
        free((void *)tempo);
        return;
    }
    *tempo=lt_copy(*lt);
    printf("<TURNO>\nID: %d\n", getIdTurno(*tempo));
    printf("Nombre: %s\n", getNombreTurno(*tempo));
    printf("DNI: %d\n", getIdCliente(*tempo));
    printf("Tratamientos:\n");
    for (int i = 0; i < CANTIDADTRATAMIENTOS; i++){
        if (getTratamiento(*tempo, i)){
            printf("> %s\n", getNombreTratamiento(t[i]));
        }
    }
    printf("Metodo de pago: %s\n",NombremetodoPago(getFormadepago(*tempo)));
    printf("Total: %.2f\n",getTotal(*tempo));
    printf("Fecha: %hd/%hd - %hd:00hs\n", getTurnoDia(*tempo), getTurnoMes(*tempo), getTurnoHora(*tempo));
    if(getRealizado(*tempo)) {
        printf("Realizado: SI\n");
    } else {
        printf("Realizado: NO\n");
    }
    free((void *)tempo);
}

// Buscar Cliente
int buscarClienteDNI(Lista_Cliente *l, int dni) { 
    lc_reset(l);
    while (!lc_isOos(*l))
    {
        // Si encuentra el DNI, deja el cursor en el cliente.
        if (getId(lc_copy(*l)) == dni)
        {
            return 1;
        }
        else
        {
            // Avanza el cursor si no encontro el cliente.
            lc_forward(l);
        }
    }
    return 0;
}

// Buscar Cliente (Nombre y Apellido)
int buscarClienteNyA(Lista_Cliente *l, char *nombre, char *apellido) {
    lc_reset(l);
    while (!lc_isOos(*l))
    {
        // Si encuentra el DNI, deja el cursor en el cliente.
        if (strcmp(nombre, getNombreCliente(lc_copy(*l))) == 0 && strcmp(apellido, getApellidoCliente(lc_copy(*l)))==0)
        {
            return 1;
        }
        else
        {
            // Avanza el cursor si no encontro el cliente.
            lc_forward(l);
        }
    }
    return 0;
}

// Buscar Turno (ID)
int BuscarTurnoIDTurno(Lista_Turno *lt, int idtur) {
    lt_reset(lt);
    while (lt_isOos(*lt) == 0)
    {
        if (getIdTurno(lt_copy(*lt)) == idtur)
        {
            return 1;
        }
        else
        {
            lt_forward(lt);
        }
    }
    return 0;
}

// Verificar si no tiene turnos pendientes
int clienteConTurnoPendiente(Lista_Turno *lt, int idcli) {
    lt_reset(lt);
    while (lt_isOos(*lt) == 0)
    {
        if (getIdCliente(lt_copy(*lt)) == idcli && getRealizado(lt_copy(*lt)) == 0)
        {
            return 1;
        }
        else
        {
            lt_forward(lt);
        }
    }
    return 0;
}

// Verificar si no hay un turno en esta fecha y esta hora.
int IsHorarioFree(Lista_Turno *lt, short dd, short mm, short hh) {
    if (lt_isEmpty(*lt)) return 1;
    lt_reset(lt);
    while (!lt_isOos(*lt))
    {
        if (getTurnoDia(lt_copy(*lt)) == dd && getTurnoMes(lt_copy(*lt)) == mm && getTurnoHora(lt_copy(*lt)) == hh)
        {
            return 0;
        }
        else
        {
            lt_forward(lt);
        }
    }
    return 1;
}

// Actividad A
void registrarTurno(Lista_Cliente *lc, Lista_Turno *lt, int *cantTurnos, Tratamiento tratamientos[]) {
    int *option = (int *)malloc(sizeof(int));
    int *aux = (int *)malloc(sizeof(int));
    float *subtotal = (float *)malloc(sizeof(float));
    short *dd = (short *)malloc(sizeof(short));
    short *mm = (short *)malloc(sizeof(short));
    short *hh = (short *)malloc(sizeof(short));
    char *nom = (char *)malloc(32 * sizeof(char));
    char *ape = (char *)malloc(32 * sizeof(char));
    Cliente *temp = (Cliente *)malloc(sizeof(Cliente));
    if (lt_isFull())
    {
        printf("Error: No hay mas espacio en la memoria");
        return;
    };
    Turno *turno = (Turno *)malloc(sizeof(Turno));
    if (option == NULL || aux == NULL || nom == NULL || temp==NULL ||ape == NULL || dd == NULL || mm == NULL || hh == NULL || turno == NULL)
    {
        printf("Error: Memoria insuficiente\n");
        return;
    }
    *subtotal = 0;
    *option = 1;      // Para prevenir cartel de opcion invalida.
    strcpy(nom, "a"); // Para prevenir cartel de opcion invalida.
    strcpy(ape, "a"); // Para prevenir cartel de opcion invalida.
    // Busqueda del cliente
    system("cls");
    if (!lc_isEmpty(*lc))
    {
        do
        {
            if (*option != 1 && *option != 2)
                printf("Error: Opcion invalida.\n");
            printf("Usted es cliente?\n1.SI\n2.NO\n>Opcion: ");
            scanf("%d", option);
            fflush(stdin);
        } while (*option != 1 && *option != 2);
        if (*option == 2)
        {
            // Control de que la lista no esta llena
            if (!lc_isFull(*lc))
            {
                registrarCliente(lc);
            }
            else
            {
                printf("Error: La lista esta llena, no podemos registrarlo.\n");
            }
        }
        else
        {
            do {
                if (*option < 0)
                    printf("Error: Opcion invalida.\n");
                printf("Ingresa tu DNI, o escribe 0 para buscarte por tu nombre: ");
                scanf("%d", option);
            } while (*option < 0);
            fflush(stdin);
            if (*option == 0) {// Busqueda por nombre{
                do
                {
                    if (strlen(nom) == 0)
                        printf("Error: No puede ingresar un nombre vacio.\n");
                    printf("Ingrese su nombre: ");
                    gets(nom);
                    fflush(stdin);
                } while (strlen(nom) == 0);
                do
                {
                    if (strlen(ape) == 0)
                        printf("Error: No puede ingresar un apellido vacio.\n");
                    printf("Ingrese su apellido: ");
                    gets(ape);
                    fflush(stdin);
                } while (strlen(ape) == 0);
                *aux = buscarClienteNyA(lc, nom, ape);
                if (*aux == 0)
                {
                    printf("Error: No encontramos tu usuario. Te registraremos.\n");
                    registrarCliente(lc);
                }
            }
            else
            {
                *aux = buscarClienteDNI(lc, *option);
                if (*aux == 0)
                {
                    printf("Error: No encontramos tu usuario, procedemos a registrarte. Pulsa cualquier tecla para continuar\n");
                    getchar();
                    registrarCliente(lc);
                }
            }
        }
    }
    else
    {
        printf("La lista de clientes esta vacia, entonces te registraremos. Pulsa cualquier tecla para continuar\n");
        getchar();
        registrarCliente(lc);
    }
    free((void *)nom);
    free((void *)ape);
    // Para mayor comodidad, guardamos nuestro cliente en una variable.
    *temp=lc_copy(*lc);
    // Control si el cliente ya tiene un turno

    if (clienteConTurnoPendiente(lt, getId(*temp)))
    {
        printf("Error: Ya tienes un turno pendiente.\nPulsa cualquier tecla para salir.");
        getchar();
        return;
    };
    // //Registrar el turno
    initTurno(turno);
    setNombreTurno(turno,nomcompleto(getNombreCliente(*temp),getApellidoCliente(*temp)));
    setIdCliente(turno, getId(*temp));
    setIdTurno(turno, ++(*cantTurnos));
    // AUX nos dira la cantidad de tratamientos que se realizara. 
    while (1 == 1)
    {
        *aux = 0;
        system("cls");
        printf("%s, que tratamientos deseas realizarte? Maximo: 3 por sesion\n", getNombreTurno(*turno));
        for (int i = 0; i < 10; i++)
        {
            printf("[");
            if (getTratamiento(*turno, i))
            {
                printf("X] - %d - %s $%.2f\n", i + 1, getNombreTratamiento(tratamientos[i]), getPrecioTratamiento(tratamientos[i]));
                (*aux)++;
            }
            else
            {
                printf(" ] - %d - %s $%.2f\n", i + 1, getNombreTratamiento(tratamientos[i]), getPrecioTratamiento(tratamientos[i]));
            }
        }
        printf("Cantidad: %d", *aux);
        printf("\nEscribe 0 para parar de marcar tratamientos.\nIngrese el numero del tratamiento que desea agregar/eliminar: ");
        scanf("%d", option);
        fflush(stdin);
        if (*option != 0)
        {
            ToggleTratamiento(turno, *option - 1);
        }
        else
        {
            if (*aux == 0)
            {
                printf("No puedes solicitar un turno sin tratamientos. Pulsa cualquier tecla para continuar.");
                getchar();
            }
            else
            {
                if (*aux > 3)
                {
                    printf("No puedes solicitar mas de 3 tratamientos para un solo turno. Pulsa cualquier tecla para continuar.");
                    getchar();
                }
                else
                {
                    break;
                }
            }
        }
    }
    system("cls");
    *option = 1; // Para prevenir cartel de opcion invalida.
    do
    {
        if (!IsHorarioFree(lt, *dd, *mm, *hh))
            printf("Error: El horario esta ocupado");
        *mm = 11; // Para prevenir el cartel de opcion invalida
        do
        {
            if (*mm != 11 && *mm != 12)
                printf("Error: Opcion invalida\n");
            printf("Ingrese en que mes desea el turno (11 o 12): ");
            scanf("%hd", mm);
        } while (*mm != 11 && *mm != 12);
        *dd = 1; // Para prevenir el cartel de opcion invalida
        do
        {
            if (*dd < 1 || (*mm == 11 && *dd > 30) || *dd > 31)
                printf("Error: Opcion invalida\n");
            printf("Ingrese el dia: ");
            scanf("%hd", dd);
        } while (*dd < 1 || (*mm == 11 && *dd > 30) || *dd > 31);
        *hh = 9; // Para prevenir el cartel de opcion invalida
        do
        {
            if (*hh < 9 || *hh > 19)
                printf("Error: Opcion invalida\n");
            printf("Ingrese la hora (9 a 19): ");
            scanf("%hd", hh);
        } while (*hh < 9 || *hh > 19);
    } while (!IsHorarioFree(lt, *dd, *mm, *hh));
    
    setTurnoDia(turno, *dd);
    setTurnoMes(turno, *mm);
    setTurnoHora(turno, *hh);
    free((void *)dd);
    free((void *)hh);
    free((void *)mm);
    fflush(stdin);
    system("cls");
    do
    {
        if (*option < 1 || *option > 4)
            printf("Error: Opcion Invalida\n");
        printf("Ingrese el metodo de pago.\nDebito (1)\nCredito (2)\nQR (3)\nEfectivo (4)\nOpcion: ");
        scanf("%d", option);
    } while (*option < 1 || *option > 4);
    setFormaDePago(turno, *option);
    system("cls");
    for (int i = 0; i < 10; i++)
    {
        if (getTratamiento(*turno, i) == 1)
        {
            *subtotal += getPrecioTratamiento(tratamientos[i]);
        }
    }
    setTotal(turno, *subtotal);
    // Descuento por el nivel
    switch (getNivel(*temp))
    {
    case 1:
        setTotal(turno, getTotal(*turno) * 0.95);
        break;
    case 2:
        setTotal(turno, getTotal(*turno) * 0.90);
        break;
    case 3:
        setTotal(turno, getTotal(*turno) * 0.85);
        break;
    case 0:
        break;
    }
    fflush(stdin); 
    system("cls");
    printf("%s, el turno es en la fecha: %hd/%hd - %hd:00\nTratamientos: \n", getNombreTurno(*turno), getTurnoDia(*turno), getTurnoMes(*turno), getTurnoHora(*turno));
    for (int i = 0; i < 10; i++)
    {
        if (getTratamiento(*turno, i))
        {
            printf("> %s\n", getNombreTratamiento(tratamientos[i]));
        }
    }
    printf("Forma de pago: %s\nSubtotal: %.2f\nTotal: %.2f", NombremetodoPago(getFormadepago(*turno)), *subtotal, getTotal(*turno));
    *option = 1;
    do
    {
        if (*option != 1 && *option != 2)
            printf("Error: Opcion invalida.\n");
        printf("\nDesea registrar el turno?\nOpcion (1 = SI y 2 = NO): ");
        scanf("%d", option);
        fflush(stdin);
    } while (*option != 1 && *option != 2);
    if (*option == 1)
    {
        // Colocarlo por orden de fecha
        if (lt_isEmpty(*lt))
        {
            lt_insert(lt, *turno);
        }
        else
        {
            lt_reset(lt);
            while (!lt_isOos(*lt))
            {
                if (getTurnoMes(*turno) <= getTurnoMes(lt_copy(*lt)) && getTurnoDia(*turno) <= getTurnoDia(lt_copy(*lt)) && getTurnoHora(*turno) < getTurnoHora(lt_copy(*lt)))
                {
                    break;
                }
                else
                {
                    lt_forward(lt);
                }
            }
            lt_insert(lt, *turno);
        }
        *cantTurnos++;
        printf("Turno ingresado, muchas gracias. Pulsa cualquier tecla para continuar.\n");
        getchar();
        
    }
    system("cls");
    // Liberar variables
    free((void *)option);
    free((void *)aux);
    free((void *)turno);
    free((void *)temp);
    return;
}

// Actividad B
int buscarTurnoIDCliente(Lista_Turno *lt, int idcli) {
    lt_reset(lt);
    while (lt_isOos(*lt) == 0)
    {
        if (getIdCliente(lt_copy(*lt)) == idcli && getRealizado(lt_copy(*lt))==0)
        {
            return 1;
        }
        else
        {
            lt_forward(lt);
        }
    }
    return 0;
}

// Actividad C
void mostrarTurnosMes(Lista_Turno *lt, short mm, Tratamiento tratamientos[]){
    char *metodo = (char *)malloc(10 * sizeof(char));
    short *mostrado = (short *)malloc(sizeof(short));
    if (metodo == NULL || mostrado == NULL)
    {
        printf("Error: Memoria insuficiente para ejecutar la funcion.\n");
        return;
    }
    *mostrado = 0;
    system("cls");
    lt_reset(lt);
    while (lt_isOos(*lt) == 0)
    {
        if (getTurnoMes(lt_copy(*lt)) == mm)
        {
            *mostrado = 1;
            MostrarTurno(lt,tratamientos);
        }
        lt_forward(lt);
    }
    if (*mostrado == 0)
    {
        printf("Error: No hay turnos en este mes.");
    }
    fflush(stdin);
    printf("\nPulsa cualquier tecla para salir.");
    getchar();
    free((void *)metodo);
    free((void *)mostrado);
}

// Actividad D
void mostrarTurnosCliente(Lista_Turno *lt, char nombre[], Tratamiento tratamientos[]){
    // Reset a la lista para ir desde el principio
    int *acum = (int *)malloc(sizeof(int));
    short *option = (short *)malloc(sizeof(short));
    if(acum==NULL || option==NULL) {
        printf("Error: Memoria insuficiente para ejecutar la funcion.\n");
    }
    system("cls");
    lt_reset(lt);
    while(1==1) {
        *acum = 0;
        while (!lt_isOos(*lt) && *acum < 3){
            if (strcmp(nombre, getNombreTurno(lt_copy(*lt))) == 0)
            {
                (*acum)++;
                // Imprimir datos del cliente.
                MostrarTurno(lt,tratamientos);
                printf("\n");
            }
            lt_forward(lt);
        }
        if (*acum==0) {
            printf("No se encontraron turnos. Pulsa cualquier tecla para salir.\n");
            getchar();
            return;
        }
        (*option)=1; // Para prevenir cartel de opcion invalida
        do {
            fflush(stdin);
            if (*option < 1 || *option > 3)
            {
                printf("Error: Opcion invalida\n");
            }
            if (*option==2 && *acum<3) {
                printf("\nError: No puedes avanzar de pagina. No hay turnos para mostrar\n\n");
            }
            printf("1. Salir\n");
            printf("2. Avanzar de pagina\n");
            printf("3. Volver a la primera pagina\n");
            scanf("%hd", option);
            fflush(stdin);
        } while (*option < 1 || *option > 3  || (*option==2 && *acum<3));
        switch (*option){ 
            case 1:
                free((void *)acum);
                free((void *)option);
                return;
                break;
            case 2:
                system("cls");
                break;
            case 3:
                system("cls");
                lt_reset(lt);
                break;
        }
    }
    free((void *)acum);
    free((void *)option);
    return;
}

//Actividad E 
void recurMostrarTurno(Lista_Turno *lt, int id, int acum){
    if(!lt_isOos(*lt)){
        if(getIdCliente(lt_copy(*lt)) == id){
            printf("- Fecha: %d/%d\n- ID Turno: %d\n- Total: %.2f\n\n", getTurnoDia(lt_copy(*lt)), getTurnoMes(lt_copy(*lt)), getIdTurno(lt_copy(*lt)), getTotal(lt_copy(*lt)));
            lt_forward(lt);
            recurMostrarTurno(lt,id,acum+1);
        } else{
            lt_forward(lt);
            recurMostrarTurno(lt,id,acum+1);
        }
    } else {
        if(acum==0) {
            printf("Error: No se han encontrado turnos registrados con el DNI.");
        }
        return;
    }
}

//Actividad F
float gananciaMensual(Lista_Turno *lt, int mes) {
    if (lt_isOos(*lt)) {
        return 0.0;
    }
    float temp = 0.0;
    if (getTurnoMes(lt_copy(*lt)) == mes && getRealizado(lt_copy(*lt))) {
        temp = getTotal(lt_copy(*lt));
    }
    lt_forward(lt);
    return temp + gananciaMensual(lt, mes); 
}


// Actividad G
void mostrarListaTurnos(Lista_Turno *lt, Tratamiento tratamientos[]){
    if (lt_isEmpty(*lt))
    {
        printf("Error: No hay turnos almacenados. Pulsa cualquier tecla para salir.");
        getchar();
        return;
    }
    char *metodo = (char *)malloc(sizeof(char) * 12);
    system("cls");
    lt_reset(lt);
    while (!lt_isOos(*lt))
    { // Obtenemos el metodo de pago como string
        MostrarTurno(lt,tratamientos);
        printf("\n>-----------------------------------<\n");
        lt_forward(lt);
    }
    fflush(stdin);
    printf("\nPulsa cualquier tecla para salir.");
    getchar();
    free((void *)metodo);
    return;
}

// Actividad H (Actualiza tambien el nivel)
void modificarCantidadTratamientos(Lista_Cliente *lc, int idcliente, int nuevacantTrat){
    if(lc_isEmpty(*lc)){
        printf("Error: No hay clientes cargados. Pulsa cualquier tecla para salir\n");
        getchar();
        return;
    }
    lc_reset(lc);
    if(buscarClienteDNI(lc,idcliente)==0){
        printf("Error: No se encontro el cliente. Pulse cualquier tecla para salir\n");
        getchar();
        return;
    } else {
        Cliente clienteActual = lc_copy(*lc); //Copia del Cliente Actual
        lc_suppress(lc);
        setCantidadTratamientos(&clienteActual,getCantidadTratramientos(clienteActual)+nuevacantTrat);//Modifica la cant de Tratamientos
        ActualizarNivel(&clienteActual);
        lc_insert(lc,clienteActual);
        printf("Cliente Actualizado: \n");
        printf("Nombre: %s\nApellido: %s\nDNI: %d\nNivel: %hd\nCantidad de tratamientos: %d",getNombreCliente(lc_copy(*lc)),getApellidoCliente(lc_copy(*lc)),getId(lc_copy(*lc)),getNivel(lc_copy(*lc)),getCantidadTratramientos(lc_copy(*lc)));
        getchar();
        return;
    }
}

// Actividad I
void formadePagoTurno(Lista_Turno *lt, int idcliente, short nvaFormaPago, Tratamiento t[]){
    if(lt_isEmpty(*lt)){
        printf("Error. No hay ningun turno cargado. Pulse cualquier tecla para continuar.\n");
        getchar();
        return;
    }    
    lt_reset(lt);
    int *encontrado = (int *)malloc(sizeof(int));
    if(encontrado==NULL) {
        printf("Error: Memoria insuficiente para realizar la funcion. Pulse cualquier tecla para salir.\n");
        getchar();
        return;
    }
    *encontrado = 0;
    while(!lt_isOos(*lt)){
        Turno turnoActual = lt_copy(*lt);
        if (getIdCliente(turnoActual) == idcliente && !getRealizado(turnoActual)){
            lt_suppress(lt);
            setFormaDePago(&turnoActual,nvaFormaPago);
            lt_insert(lt,turnoActual);
            printf("\nForma de pago del turno modificada correctamente \n");
            *encontrado = 1;
            MostrarTurno(lt,t);
            break;
        }
        lt_forward(lt);
    }
    if (!*encontrado){
        fflush(stdin);
        printf("Error: No se encontro el Turno. Pulse cualquier tecla para salir.");
    }
    printf("\nPulse cualquier tecla para salir.\n");
    getchar();
    return;
    free((void *)encontrado);
}

// Actividad J
void eliminarTurnoIDCliente(Lista_Turno *lt, Tratamiento t[], int idcliente){
    int *temp=(int *)malloc(sizeof(int));
    Turno *actual =(Turno *)malloc(sizeof(Turno));
    //*temp = -1; //que entre al while sin problemas
    FILE *file=fopen("db_turnosborrados.txt", "a");//archivo ya existente
    if(file==NULL) {
        printf("Error: No se pudo acceder al archivo. Pulsa cualquier tecla para salir.\n");
        getchar();
        return;
    }
    if(actual==NULL || temp==NULL) {
        printf("Error: Memoria insuficiente para realizar la funcion. Pulsa cualquer tecla para salir.\n");
        getchar();
        return;
    }
    if(lt_isEmpty(*lt)) {
        printf("Error: La lista de turnos esta vacia. Pulsa cualquier tecla para salir\n");
        getchar();
        return;
        
    }
    lt_reset(lt);
    while(!lt_isOos(*lt)){
        *actual = lt_copy(*lt);
        if(getIdCliente(*actual) == idcliente && !getRealizado(*actual)) {
            printf("Turno encontrado:\n");
            MostrarTurno(lt,t);
            break;
        } else {
            lt_forward(lt);
        }
    }    
    if(lt_isOos(*lt)) {
        printf("Error: No se encontraron turnos no realizados con ese DNI.\nPulsa cualquier tecla para salir.\n");
        getchar();
        return;
    }
    do {
        (*temp)=1; // Para prevenir el cartel de opcion invalida.
        if (*temp != 1 && *temp != 2)
            printf("Error: Opcion invalida.\n");
        printf("\nDesea eliminar el turno?\nOpcion (1 = SI y 2 = NO): ");
        scanf("%d", temp);
        fflush(stdin);
    } while (*temp != 1 && *temp != 2);
    if((*temp)==1) {

        fprintf(file,"ID: %d\nNombre: %s\nDNI: %d\nTratamientos: \n",getIdTurno(*actual),getNombreTurno(*actual),getIdCliente(*actual));
        for (int i = 0; i < CANTIDADTRATAMIENTOS; i++) {
            if (getTratamiento(*actual, i)) {
                fprintf(file, "> %s\n", getNombreTratamiento(t[i]));
            }
        }
        fprintf(file,"Metodo de pago: %s\n",NombremetodoPago(getFormadepago(*actual)));
        fprintf(file,"Total: %.2f\n",getTotal(*actual));
        fprintf(file,"Fecha: %hd/%hd - %hd:00hs\n",getTurnoDia(*actual),getTurnoMes(*actual),getTurnoHora(*actual));
        lt_suppress(lt); 
        printf("El turno se ha eliminado correctamente. Pulsa cualquier tecla para salir.\n");
        getchar();
        
    } else {
        printf("El turno no se ha eliminado. Pulsa cualquier tecla para salir.");
        getchar();
    }
    fclose(file);
    free((void *)temp);
}

//Actividad K
void almacenarTurnosRealizados(Lista_Turno *lt, short formaPago, Tratamiento t[]){
    FILE *file = fopen("db_turnosrealizados.txt", "a"); 
    Turno *actual =(Turno *)malloc(sizeof(Turno));
    if(file == NULL){
        printf("Error: No se obtuvo el archivo. Pulse cualquier tecla para salir.\n");
        getchar();
        return;
    }
    
    if(lt_isEmpty(*lt)){
        printf("Error: la lista esta vacia. Pulse cualquier tecla para salir.\n");
        fclose(file);
        getchar();
        return;
    }

    while (!lt_isOos(*lt)) {
        *actual = lt_copy(*lt); // Optimizando la copia
        if (getFormadepago(*actual) == formaPago && getRealizado(*actual) == 1) {
            fprintf(file, "ID: %d\n", getIdTurno(*actual));
            fprintf(file, "Nombre: %s\n", getNombreTurno(*actual));
            fprintf(file, "DNI: %d\nTratamientos:\n", getIdCliente(*actual));

            for (int i = 0; i < CANTIDADTRATAMIENTOS; i++) {
                if (getTratamiento(*actual, i)) {
                    fprintf(file, "> %s\n", getNombreTratamiento(t[i]));
                }
            }

            fprintf(file, "Forma de pago: %s\n", NombremetodoPago(getFormadepago(*actual)));
            fprintf(file, "Total: %.2f\n", getTotal(*actual));
            fprintf(file, "Fecha: %hd/%hd - %hd:00hs\n", getTurnoDia(*actual), getTurnoMes(*actual), getTurnoHora(*actual));
        }
        lt_forward(lt); // Siguiente turno
    }

    fclose(file);
    free((void *)actual);
    printf("Los turnos se han cargado correctamente. Pulsa cualquier tecla para salir.\n");
    getchar();
    return;
}


// Actividad L
void registrarCliente(Lista_Cliente *l) {
    system("cls");
    if (lc_isFull(*l))
    {
        printf("Error: La lista de clientes esta llena, no podemos agregar un cliente nuevo. Pulsa cualquier tecla para salir\n");
        getchar();
        return;
    }
    Cliente *nuevo = (Cliente *)malloc(sizeof(Cliente));
    char *temp = (char *)malloc(32 * sizeof(char)); // Tamaño nombre y apellido
    int *tempId = (int *)malloc(sizeof(int));
    if (nuevo == NULL || temp == NULL || tempId == NULL)
    {
        printf("Error: No hay mas espacio en la memoria. Pulsa cualquier tecla para salir\n");
        getchar();
        return;
    }
    init(nuevo);
    // Pedir datos
    *tempId = 1; // Para prevenir cartel de opcion invalida.
    do
    {
        if (*tempId <= 0)
            printf("Error: Opcion invalida.\n");
        printf("Ingrese su DNI (Sin puntos ni comas): ");
        scanf("%d", tempId);
        fflush(stdin);
        
    } while (*tempId <= 0);

    if (buscarClienteDNI(l, *tempId))
    {
        printf("Error: Ya estas registrado.\n");
        return;
    }
    fflush(stdin);
    setId(nuevo, *tempId);
    strcpy(temp, "a"); // para prevenir cartel de opcion invalida
    do
    {
        if (strlen(temp) == 0)
            printf("Error: No puede ingresar un nombre vacio.\n");
        printf("Ingrese su nombre: ");
        gets(temp);
    } while (strlen(temp) == 0);
    setNombreCliente(nuevo, temp);

    do
    {
        if (strlen(temp) == 0)
            printf("Error: No puede ingresar un apellido vacio.\n");
        printf("Ingrese su apellido: ");
        gets(temp);
    } while (strlen(temp) == 0);
    setApellidoCliente(nuevo, temp);

    // Insercion
    lc_insert(l, *nuevo);

    // Control
    printf("\nCLIENTE INGRESADO:\nNombre y Apellido: %s %s\nID (DNI): %d\nCantidad de tratamientos: %d\nNivel: %hd\n", getNombreCliente(lc_copy(*l)), getApellidoCliente(lc_copy(*l)), getId(lc_copy(*l)), getCantidadTratramientos(lc_copy(*l)), getNivel(lc_copy(*l)));
    getchar();
    // Liberar memoria
    free((void *)temp);
    free((void *)tempId);
    free((void *)nuevo);
}

// Actividad M
void mostrarTurnosTrat(Lista_Turno *lt, int Nrotrat, Tratamiento t[]){
    if(lt_isEmpty(*lt)){
        printf("Error: No hay turnos almacenados. Pulse cualquier tecla para salir.\n");
        getchar();
        return; 
        }
    int *encontrado =(int *)malloc(sizeof(int));
    if(encontrado==NULL){
        printf("Error: Memoria insuficiente. Pulse cualquier tecla para salir.\n");
        getchar();
        return;
    }
    *encontrado = 0;
    lt_reset(lt);
    while(!lt_isOos(*lt)){
        if(getTratamiento(lt_copy(*lt),Nrotrat)) {
            *encontrado=1;
            MostrarTurno(lt,t);
            printf("\n\n---------------------\n");
        } 
        lt_forward(lt);
    }    
    if(*encontrado==0){
        printf("Error: No se han encontrado turnos con el tratamiento incluido. Pulse cualquier tecla para continuar.\n");
        getchar();
        return;
    }
    fflush(stdin);
    printf("Pulse una tecla para continuar.\n");
    getchar();
    return;
    free((void *)encontrado);
}

// Actividad N
void precargaClientes(Lista_Cliente *lc){
    if(lc_isFull(*lc)){
        printf("No hay mas espacio en la lista de clientes.\n");
        exit(1); // Directamente sale ya que esto esta en la misma precarga.
    }
    FILE *f=fopen("clientes.txt","r");
    Cliente *temp=(Cliente *)malloc(sizeof(Cliente));
    char *nomyape=(char *)malloc(sizeof(char)*32);
    int *tempid=(int *)malloc(sizeof(int));
    if(f==NULL || temp==NULL || nomyape==NULL || tempid==NULL) {
        printf("No se pudo obtener el archivo. Pulsa cualquier tecla para salir.\n");
        getchar();
        free((void *)temp);
        free((void *)nomyape);
        free((void *)tempid);
        return;
    }
    lc_reset(lc);
    while (!feof(f)) {
        fscanf(f,"%d",tempid);
        fgetc(f);
        fgets(nomyape,32,f);
        setId(temp,*tempid);
        nomyape[strcspn(nomyape,"\n")]='\0';
        setNombreCliente(temp,nomyape);
        fgets(nomyape,32,f);
        nomyape[strcspn(nomyape,"\n")]='\0';
        setApellidoCliente(temp,nomyape);
        fscanf(f,"%d",tempid);
        fgetc(f);
        setCantidadTratamientos(temp,*tempid);
        fscanf(f,"%hd",tempid);
        fgetc(f);
        setNivelCliente(temp,*tempid);
        lc_insert(lc,*temp);
    }
    fclose(f);
    free((void *)temp);
    free((void *)tempid);
    free((void *)nomyape);
}

// Actividad Ñ
void mostrarClientes(Lista_Cliente *lc){
    Cliente *temp = (Cliente *)malloc(sizeof(Cliente));
    if(temp==NULL) {
        printf("Error: Memoria insuficiente. Pulsa cualquier tecla para salir.\n");
        getchar();
        return;
    }
    if (lc_isEmpty(*lc)){
        printf("Error: No hay clientes almacenados. Pulsa cualquier tecla para salir.");
        getchar();
        return;
    }
    lc_reset(lc);
    system("cls");
    while (!lc_isOos(*lc)){
        *temp=lc_copy(*lc);
        printf("Nombre: %s\n", getNombreCliente(*temp));
        printf("Apellido: %s\n", getApellidoCliente(*temp));
        printf("DNI: %d\n", getId(*temp));
        printf("Cantidad Tratamientos: %d\n", getCantidadTratramientos(*temp));
        printf("Nivel: %hd\n", getNivel(*temp));
        printf("------------------------------------\n");

       //Mover al siguiente cliente en la Lista
        lc_forward(lc);
    }
    fflush(stdin);
    printf("\nPulsa cualquier tecla para salir.");
    free((void *)temp);
    getchar();
    return;
}    

// Actividad O
void eliminarCLienteID(Lista_Cliente *lc, int IDcliente){
    if(lc_isEmpty(*lc)){
        printf("No hay Clientes registrados. Pulse una tecla para continuar\n");
        getchar();
        return;
    }
    lc_reset(lc);
    if(buscarClienteDNI(lc,IDcliente)==0){
        printf("Error: No se encontro el cliente. Pulse cualquier tecla para salir.\n");
        getchar();
        return;
    } else {
        Cliente clienteActual = lc_copy(*lc); //Copia del Cliente Actual
        lc_suppress(lc);
        printf("Cliente Eliminado Correctamente\n");
        printf("Nombre: %s\nApellido: %s\nDNI: %d\nNivel: %hd\nCantidad de tratamientos: %d",getNombreCliente(clienteActual),getApellidoCliente(clienteActual),getId(clienteActual),getNivel(clienteActual),getCantidadTratramientos(clienteActual));
        getchar();
        return;
    }
    fflush(stdin);
    return;
}

// Actividad P
void confirmarTurnoCliente(Lista_Cliente *lc, Lista_Turno *lt, Tratamiento t[], int idcli) {
    // Inicialización de punteros auxiliares
    Turno *temp = (Turno *)malloc(sizeof(Turno));
    int *aux = (int *)malloc(sizeof(int));
    if (aux == NULL || temp==NULL) {
        free(temp);
        printf("Error: No se pudo asignar memoria.\n");
        return;
    }
    *aux = 0;
    
    lt_reset(lt);
    lc_reset(lc);

    // Validación de listas vacías
    if (lt_isEmpty(*lt)) {
        printf("Error: La lista de turnos está vacía.\n");
        free(temp); free(aux);
        getchar();
        return;
    }

    if (lc_isEmpty(*lc)) {
        printf("Error: La lista de clientes está vacía.\n");
        free(temp); free(aux);
        getchar();
        return;
    }
    // Validar cliente
    if(buscarClienteDNI(lc, idcli) == 0) {
        printf("Error: Cliente no encontrado.\n");
        free(temp); 
        free(aux);
        getchar();
        return;
    } else {
        *temp = lt_copy(*lt);
        setRealizado(temp, 1); 
        lt_suppress(lt);
        lt_insert(lt, *temp); 

        for (int i = 0; i < 10; i++) {
            if (getTratamiento(lt_copy(*lt), i)) {
                (*aux)++;
            }
        }
        modificarCantidadTratamientos(lc, getId(lc_copy(*lc)), *aux);
        printf("Turno modificado exitosamente. Nuevo turno:\n");
        MostrarTurno(lt, t);
        
        }
    // Limpieza final
    free(temp);
    free(aux);
}


// Actividad Q
void mostrarTurnosNoRealizados(Lista_Turno *lt, Tratamiento t[]){
    // Para tener cartel de que no se encontro ningun turno.
    int *found=(int *)malloc(sizeof(int));
    if(found==NULL) {
        printf("Error: Memoria insuficiente para ejecutar la funcion.\n");
        return;
    }
    *found=0;
    if (lt_isEmpty(*lt))
    {
        printf("No hay turnos almacenados. Pulsa cualquier tecla para salir.");
        getchar();
        return;
    }
    char *metodo = (char *)malloc(sizeof(char) * 12);
    system("cls");
    lt_reset(lt);
    while (!lt_isOos(*lt))
    {
        if(getRealizado(lt_copy(*lt)) == 0) {
            *found=1;
            MostrarTurno(lt,t);
            printf("-------------------------------------\n");
        }
        
        lt_forward(lt);
    }
    if(!(*found)) {
        printf("No se encontro ningun turno.\n");
    }
    fflush(stdin);
    printf("\nPulsa cualquier tecla para salir.");
    getchar();
    free((void *)metodo);
    return;
}

int main(){
    // Variables para las funcionalidades del programa
    short *fp=(short *)malloc(sizeof(short));
    int *cantidadTurnos = (int *)malloc(sizeof(int)); // Cantidad de turnos solicitados, util para IdTurno.
    int *option = (int *)malloc(sizeof(int));
    int *aux = (int *)malloc(sizeof(int));
    int *result = (int *)malloc(sizeof(int)); // Para prevenir entrada de caracteres a la hora de pedir enteros o floats.
    char *nom = (char *)malloc(sizeof(char) * 32);
    char *ape = (char *)malloc(sizeof(char) * 32);
    Tratamiento *Tratamientos = (Tratamiento *)malloc(sizeof(Tratamiento) * CANTIDADTRATAMIENTOS);
    // Inicio
    initTratamientos(Tratamientos);
    *cantidadTurnos = 0;
    Lista_Turno *listaTurnos = (Lista_Turno *)malloc(sizeof(Lista_Turno));
    lt_init(listaTurnos);
    Lista_Cliente *listaClientes = (Lista_Cliente *)malloc(sizeof(Lista_Cliente));
    lc_init(listaClientes);
    precargaClientes(listaClientes);
    // Menu
    while (1 == 1)
    {
        *option = 1;
        system("cls");
        printf("Bienvenido a la estetica\n\n");
        printf("0 -> Salir\n");
        printf("1 -> Registar turno (A)\n");
        printf("2 -> Registrar cliente (L)\n");
        printf("3 -> Imprimir los turnos de un mes ordenados (C)\n");
        printf("4 -> Mostrar turnos por nombre del cliente (D)\n");
        printf("5 -> Mostrar todos los turnos (G)\n");
        printf("6 -> Mostrar todos los clientes (enie)\n");
        printf("7 -> Mostrar todos los turnos de un mismo tratamiento (M)\n");
        printf("8 -> Mostrar todos los turnos de un cliente (E)\n");
        printf("9 -> Mostrar la ganancia mensual (F)\n");
        printf("10 -> Eliminar cliente por DNI (O)\n");
        printf("11 -> Marcar turno como realizado (P)\n");
        printf("12 -> Modificar la cantidad de tratamientos de un cliente (H)\n");
        printf("13 -> Modificar la forma de pago del turno (I)\n");
        printf("14 -> Mostrar los turnos no realizados (Q)\n");
        printf("15 -> Cancelar un turno (J)\n");
        printf("16 -> Guardar turnos realizados segun forma de pago (K)\n");
        printf("Seleccione la opcion:\n\n");
        do
        {
            if (*option < 0)
                printf("Error: Opcion Invalida\n");
            printf("---> ");
            scanf("%d", option);
            fflush(stdin);
        } while (*option < 0);
        switch (*option){
            case 0:
                liberarTurnos(listaTurnos);
                free((void *)option);
                free((void *)listaTurnos);
                free((void *)listaClientes);
                free((void *)cantidadTurnos);
                free((void *)Tratamientos);
                return 0;
                break;
            case 1:
                registrarTurno(listaClientes, listaTurnos, cantidadTurnos, Tratamientos);
                break;
            case 2:
                registrarCliente(listaClientes);
                break;
            case 3:
                // Para prevenir cartel de opcion invalida:
                *option = 11;
                do {
                    if (*option != 11 && *option != 12)
                        printf("Error: Opcion invalida\n");
                    printf("Ingrese el mes (11 o 12): ");
                    scanf("%d", option);
                } while (*option != 11 && *option != 12);
                mostrarTurnosMes(listaTurnos, *option, Tratamientos);
                break;
            case 4:
                do {
                    if (strlen(nom) == 0 || strlen(ape) == 0)
                        printf("Error: No puede ingresar un nombre y/o apellido vacio.\n");
                    printf("Ingrese el nombre: ");
                    gets(nom);
                    fflush(stdin);
                    printf("Ingrese el apellido: ");
                    gets(ape);
                    fflush(stdin);
                } while (strlen(nom) == 0 || strlen(ape) == 0);
                if(buscarClienteNyA(listaClientes,nom,ape)==0) {
                    printf("No hay un cliente registrado con ese nombre.\nPulsa cualquier tecla para salir.");
                    getchar();
                    break;
                }
                mostrarTurnosCliente(listaTurnos, nomcompleto(nom,ape), Tratamientos);
                break;
            case 5:
                mostrarListaTurnos(listaTurnos,Tratamientos);
                break;
            case 6:
                mostrarClientes(listaClientes);
                break;
            case 7: 
                system("cls");
                do {
                    if(*option<1||*option>10) {
                        printf("Error: Ingrese una opcion valida.\n");
                    }
                    printf("<1> Manicura\n<2> Perfilado de cejas\n<3> Lifting de pestanas\n<4>Extensiones de pestanas\n");
                    printf("<5> Limpieza Facial\n<6> Podologia\n<7> Maquillaje\n<8>Peinado\n<9> Depilacion con cera (Completo)\n<10> Depilacion con laser (Completo)\n\n> Opcion: ");
                    scanf("%d", option);
                    fflush(stdin);
                } while(*option<1||*option>10);
                (*option)--;
                mostrarTurnosTrat(listaTurnos,*option,Tratamientos);
                break;
            case 8:
                lt_reset(listaTurnos);
                do {
                    if(*option<=0) {
                        printf("Error: Ingrese una opcion valida.\n");
                    }
                    printf("Ingrese el DNI del cliente: ");
                    scanf("%d", option);
                    fflush(stdin);
                } while(*option<=0);
                if(buscarClienteDNI(listaClientes,*option)==0) {
                    printf("Error: No hay un cliente registrado con ese DNI. Pulsa cualquier tecla para salir.");
                    getchar();
                    break;  
                }
                recurMostrarTurno(listaTurnos,*option,0);
                printf("\nPulse cualquier tecla para salir.");
                getchar();
                break;
            case 9: 
                *option=11; //para evitar cartel de error
                lt_reset(listaTurnos);
                do{
                    if(*option<11||*option>12) {
                        printf("Error: Opcion invalida.\n");
                    }
                    printf("Ingrese el mes que quiere ver las ganancias (11 o 12): ");
                    scanf("%d", option);
                    fflush(stdin);  
                    } while(*option<11||*option>12);
                fflush(stdin);
                    if(gananciaMensual(listaTurnos,*option)!=0) {
                        lt_reset(listaTurnos);
                        printf("La ganancia mensual del mes %d es: $%.2f\n",*option,gananciaMensual(listaTurnos,*option));
                    } else {
                        printf("No se ha recaudado nada en el mes %d\n",*option);
                    }
                    printf("Pulse cualquier tecla para salir.");
                    getchar();
                break;
            case 10: 
                do {
                    if(*option<=0) {
                        printf("Error: Ingrese una opcion valida.\n");
                    }
                    printf("Ingrese el DNI del cliente: ");
                    scanf("%d", option);
                    fflush(stdin);
                } while(*option<=0);
                eliminarCLienteID(listaClientes, *option);
                break;
            case 11:
                do {
                    if(*option<=0) {
                        printf("Error: Ingrese una opcion valida.\n");
                    }
                    printf("Ingrese el DNI del cliente: ");
                    scanf("%d", option);
                    fflush(stdin);
                } while(*option<=0);
                confirmarTurnoCliente(listaClientes,listaTurnos,Tratamientos,*option);
                break;
            case 12: 
                do{
                    if(*option<=0) {
                            printf("Error: Ingrese una opcion valida.\n");
                        }
                        printf("Ingrese el DNI del cliente: ");
                        scanf("%d", option);
                        fflush(stdin);
                    } while(*option<=0);
                if(buscarClienteDNI(listaClientes,*option)==0) {
                    printf("Error: No hay un cliente registrado con ese DNI. Pulsa cualquier tecla para salir.");
                    getchar();
                    break;  
                }
                do{
                    if(*aux<=0) {
                            printf("Error: Ingrese una opcion valida.\n");
                        }
                        printf("Ingrese la cantidad de tratamientos a agregarle al cliente: ");
                        scanf("%d", aux);
                        fflush(stdin);
                } while(*aux<=0);
                modificarCantidadTratamientos(listaClientes, *option, *aux);
                break;
            case 13:
                if(lt_isEmpty(*listaTurnos)) {
                    printf("Error: No hay turnos almacenados. Pulse cualquier tecla para salir.\n");
                    getchar();
                    break;
                }
                do{
                if(*option<=0) {
                        printf("Error: Ingrese un DNI valido.\n");
                    }
                    printf("Ingrese el DNI del cliente: ");
                    scanf("%d", option);
                    fflush(stdin);
                } while(*option<=0);
                if(buscarClienteDNI(listaClientes,*option)==0) {
                    printf("Error: No hay un cliente registrado con ese DNI. Pulsa cualquier tecla para salir.");
                    getchar();
                    break;  
                }
                if (buscarTurnoIDCliente(listaTurnos,*option)==0) {
                    printf("Error: El cliente no tiene un turno pendiente. Pulsa cualquier tecla para salir.");
                    getchar();
                    break;
                }
                *fp = 1; // Para prevenir cartel de opcion invalida.
                system("cls");
                do {
                    if (*fp < 1 || *fp > 4)
                        printf("Error: Opcion Invalida\n");
                    printf("Ingrese el metodo de pago.\nDebito (1)\nCredito (2)\nQR (3)\nEfectivo (4)\nOpcion: ");
                    scanf("%d", fp);
                    fflush(stdin);
                } while (*fp < 1 || *fp > 4);
                formadePagoTurno(listaTurnos,*option,*fp,Tratamientos);
                break;
            case 14:
                mostrarTurnosNoRealizados(listaTurnos,Tratamientos);
                break;
            case 15:
                if(lt_isEmpty(*listaTurnos)) {
                    printf("Error: No hay turnos almacenados. Pulse cualquier tecla para salir.\n");
                    getchar();
                    break;
                }
                do{
                if(*option<=0) {
                        printf("Error: Ingrese una opcion valida.\n");
                    }
                    printf("Ingrese el DNI del cliente: ");
                    scanf("%d", option);
                    fflush(stdin);
                } while(*option<=0);
                if (buscarClienteDNI(listaClientes,*option)==0) {
                    printf("Error: No hay un cliente registrado con ese DNI. Pulsa cualquier tecla para salir.");
                    getchar();
                    break;
                }
                eliminarTurnoIDCliente(listaTurnos,Tratamientos,*option);
                break;
            case 16:
                *fp = 1; // Para prevenir cartel de opcion invalida.
                do {
                    if (*fp < 1 || *fp > 4)
                        printf("Error: Opcion Invalida\n");
                    printf("Ingrese el metodo de pago.\nDebito (1)\nCredito (2)\nQR (3)\nEfectivo (4)\nOpcion: ");
                    scanf("%d", fp);
                    fflush(stdin);
                } while (*fp < 1 || *fp > 4);
                almacenarTurnosRealizados(listaTurnos ,*fp, Tratamientos);
                break;
            default:
                break;
        }
    }
    // Liberar memorias
    liberarTurnos(listaTurnos);
    free((void *)option);
    free((void *)aux);
    free((void *)nom);
    free((void *)ape);
    free((void *)fp);
    free((void *)listaTurnos);
    free((void *)listaClientes);
    free((void *)cantidadTurnos);
    free((void *)Tratamientos);
    return 0;
}