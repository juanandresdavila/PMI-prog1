Programación I
Práctico de Máquina Integrador
Fecha de control: cada grupo primer semana del 22 de octubre (archivo cliente.h, listaD_Turno.h,
turno.h, listaE_Cliente.h)
Fecha de entrega: 12/11/2024
Grupo: 3 (tres) integrantes. Se habilitará formulario de inscripción del grupo del 14 al 18 de octubre
Se desea realizar un programa en lenguaje C que permita administrar los turnos de un local de Estética que atiende
de lunes a sábado de 9 a 20 hs, así mismo realizar algunas estadísticas de la misma. Para realizar esta actividad se
debe definir las estructuras de datos y funciones necesarias. La Fecha de Inicio del sistema es 01/11/2024 9hs
Para administrar los pedidos se pide:
a) Implementar el TDA TURNO (turno .h) el cual define el tipo Turno (mantiene los datos de cada uno de los turnos)
con la información según se detalla en TDA_TURNO y sus respectivas operaciones.
b) Implementar el TDA CLIENTE (cliente .h) el cual define el tipo Cliente (mantiene los datos de cada uno de los
clientes habituales) con la información según se detalla en TDA_CLIENTE y sus respectivas operaciones.
c) Implementar una lista vinculada dinámica que permita almacenar Turnos, esto debe realizarse definiendo el TDA
LISTAD (listaD _Turno.h). La lista debe contener las operaciones básicas de la misma.
d) Implementar una lista estática que permita almacenar Clientes, esto debe realizarse definiendo el TDA LISTAE
(listaE_Cliente .h). La lista debe contener las operaciones básicas de la misma.
e) realizar un programa en C que utilice los TDA definidos y que permita cumplir con las funcionalidades según se
solicitan en el apartado funcionalidades.
Definir el TDA TURNO el cual contiene:
- Definición de la estructura de datos de manera tal que pueda almacenar por cada turno los
siguientes datos:
Idturno: único no se repite, generado automáticamente.
Nombre: nombre de la persona que solicita el turno sin espacios, es el nombre del cliente
IdCliente: en caso de ser cliente habitual, es el dni del cliente. Ver TDA Cliente.
Tratamiento: existen 10 tratamientos posibles, cada uno con el nombre y precio. El cliente puede
realizarse varios tratamientos a la vez. Debe utilizar un arreglo para almacenar los mismos. El cliente no
puede realizarse los 10 tratamientos en un turno, ya que es de una hora.
(*) Forma de pago: es una de las siguientes; débito, crédito, QR, efectivo. El turno se paga cuando es
realizado, no se cobra por adelantado.
Total: es el monto a pagar por todos los tratamientos solicitados. Si es cliente esta registrado tiene un %
de descuento asociado al nivel. Nivel 0 primera vez sin descuento, Nivel 1 5%, nivel 2 10%, nivel 3 15%.
Fecha de turno: tipo fecha trabajado en la materia agregándole la hora SIN MINUTOS. Se brinda un turno
por hora. Desde la fecha de inicio 01/11/2024 9hs hasta el 31/12/2024 20hs
(*)Realizado: si o no, por defecto no
Los tipos no especificados son determinados por el estudiante.
- Definición de las operaciones:
Defina las funciones necesarias de manera tal que se permita:
Cargar cada uno de los campos por separado considerando que sólo pueden ser modificados los campos
marcados con (*) recuerde que solo podrá modificarlo si el turno no fue realizado.
Mostrar cada uno de los campos por separado.
Definir el TDA Cliente el cual contiene
- Definición de la estructura de datos de manera tal que pueda almacenar por cada cliente que se
encuentre registrado los siguientes datos:
Id Cliente: identificador único que corresponde al DNI
Nombre: almacena el nombre del cliente admite espacios
Apellido: almacena el apellido del cliente admite espacios
(*)Cantidad tratamientos: indica el total de tratamientos realizados desde que es cliente registrado, se
actualiza con los turnos.
(*)Nivel: hay 4 niveles, nivel 0 por defecto nuevo cliente, nivel 1 con 1 a 4 tratamientos, nivel 2 entre 5 y 10
tratamientos, nivel 3 mas de 10 tratamientos. Debe actualizarse automáticamente cuando se carga como
tratamientos realizados
- Definición de las operaciones:
Defina las funciones necesarias de manera tal que se permita:
Cargar cada uno de los campos por separado y que sólo se pueda modificar los campos marcados con (*)
Mostrar cada uno de los campos por separado.
Definir el TDA listaD_turno
Definir las estructuras de datos y operaciones de la lista (insert, suppress, isempty,
isoos, copy, isfull, forward, init, reset) necesarias para administrar una lista vinculada
en memoria dinámica.
Definir el TDA listaE_Cliente
Definir las estructuras de datos y operaciones de la lista (insert, suppress, isempty,
isoos, copy, isfull, forward, init, reset) necesarias para administrar una lista estática desordenada
Definir un programa que permita realizar las siguientes Funcionalidades:
a) Cargar un turno en la lista por teclado.
Considere que el cálculo del total a pagar debe ser realizado al final de la carga del turno y se debe
mostrar si tiene o no descuento para confirmar el turno.
Considere que debe consultar si es cliente registrado, si no recuerda el código de cliente debe permitir
buscarlo en la lista de clientes. Si no es cliente debe cargarlo. Un cliente solo puede tener un turno sin
realizar a la vez
b) Buscar un turno por idcliente (función auxiliar interna no figura en las opciones del programa). Solo lo
busca, no muestra. Recuerde que el cliente puede tener solo un turno no realizado a la vez, buscar el no
realizado.
c) Mostrar todos los turnos de un mes ordenados por fecha (el orden es opcional)
d) Buscar y mostrar los turnos por nombre de cliente, paginar la muestra de los datos de cliente de a 3.
e) Mostrar los turnos solicitados por un cliente según idcliente (esta función debe ser recursiva) solo mostrar,
fecha, total e idturno
f) Calcular la ganancia mensual según el mes pasado por parámetro considerando solo los ya realizados (esta
función debe ser recursiva)
g) Mostrar todos los turnos almacenados en la lista de turnos
h) Modificar la cantidad de tratamientos del cliente registrado
i) Modificar la forma de pago de un turno pedido según su idcliente (solo turno no realizado)
j) Cancelar un turno por idcliente, esta función copia en un archivo los datos del turno borrado antes de
eliminarlo. Los datos deben legibles desde un editor de texto. Debe solicitar confirmación antes de borrarlo y
actualizar los tratamientos realizados por el cliente, no se puede anular un turno ya realizado.
k) Almacenar en un archivo los datos de los turnos pagados según una forma de pago pasada por parámetro.
Los campos codificados deben estar decodificados en el archivo. Informar por pantalla cuantos turnos se
copiaron en el archivo.
l) Una opción que permita registrar por teclado un cliente
m) Mostrar todos los turnos de un tratamiento pasado por parámetro
n) Realizar una precarga automática al iniciar el programa de los clientes a la lista de Clientes. Debe contener al
menos 10 clientes.
ñ) Mostrar todos los clientes almacenados en la lista de clientes.
o) Eliminar cliente según idcliente, los turnos no se ven afectados.
p) Modificar el turno a confirmado por idcliente, es decir cuando asiste se marca como realizado y actualiza la
cantidad de tratamientos del cliente registrado, revisar nivel según cantidad.
q) Mostrar los turnos no realizados
Realice un menú de opciones que permita realizar las funcionalidades solicitadas. Siguiendo las restricciones o
relaciones entre los campos del TDA.
Debe buscar 10 tratamientos reales que se realicen en una estética, estos son cargados al iniciar el
programa en un arreglo global, que permitirá el calculo del precio y mostrar la información
Considere que se realiza un programa que luego será utilizado por un usuario, es por esto que NO DEBE
OLVIDAR los carteles!!
Todas las funcionalidades se realizan en forma aleatoria.
