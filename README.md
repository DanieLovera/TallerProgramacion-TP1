# TP1: Hill Cipher #
**Autor: Daniel Alejandro Lovera L?pez**  
**N?mero padr?n: 103442**  
**[https://github.com/DanieLovera/tp1](https://github.com/DanieLovera/tp1)**  

---
### Introducci?n ###  
Este trabajo consiste en el desarrollo de un programa cliente-servidor  en el cual, el cliente lee l?nea a l?nea un archivo de texto plano para luego enviarlas al servidor y este se encarga de encriptarlas usando un cifrado conocido como ***Hill cipher***. Este se basa en la sustituci?n de caracteres usando ?lgebra lineal modular, es similar al cifrado Cesar (***Cesars Cipher***) porque consiste en un cifrado basado en desplazamiento de caracteres pero este  extiende el concepto a N dimensiones. Luego de que el servidor haya encriptado la l?nea del cliente la devuelve al cliente quien debe encargarse de desmapear (por desplazamiento) el cifrado y mostrar el resultado obtenido.
Este programa se encuentra dise?ado principalmente en 4 grandes bloques que son independientes unos de otros:  
  
- **Cliente**  
- **Protocolo de comunicaci?n**  
- **Servidor**  
- **Cifrado Hill**  
  
Cada uno de ellos tambi?n tienen cierto nivel de abstracci?n internamente, pero estos son los m?dulos claves del trabajo.  

---
### Desarrollo ###  
  
En esta secci?n se presentan detalles de implementaci?n sobre los cuatro m?dulos principales junto con algunos diagramas que tienen como objetivo ayudarle al lector a entender el dise?o propuesto.  
  
A continuaci?n se presenta un diagrama UML de clases que representa al modelo y se hacen explicitos algunos modulos extras empleados en el dise?o.  
> Previo a continuar con el trabajo, el lector debe saber que los diagramas UML presentados en esta secci?n no son una representaci?n exacta de la implementaci?n que se encuentra en el repositorio. Existen diferencias en nombres, firmas o tipos de datos de ciertas funciones por la imposibilidad de trabajar con objetos en C, por lo cual estos diagramas tratan de ser un poco mas gen?ricos y abstractos para que el dise?o del programa pueda ser entendido y reproducido en cualquier otro lenguaje de programaci?n. Por su puesto esta generalizaci?n no hace que el c?digo presentado no sea fiel a los diagramas.
  
![Diagrama de clases](./images/diagrama_01.png)
  
En este diagrama se pueden observar las relaciones entre clases y la independencia entre ellas:  
  
- Los **protocolos cliente** y **servidor** no conocen al protocolo de comunicaci?n, su unica responsabilidad sera ejecutar los programas cliente y servidor. De la comunicaci?n entre ellos se encarga el protocolo de comunicaci?n quien es el que conoce como se enviar?n y recibiran datos del cliente al servidor y viceversa.  
- El **protocolo cliente** debe desmapear el resultado enviado como respuesta por el servidor y para esto utiliza una  librer?a llamada **Maplib**, con esto se evita que el cliente conozca directamente la interfaz del **Hill Cipher** y a su vez se elimina la responsabilidad de mapear al cipher.
- El **protocolo servidor** debe cifrar el mensaje enviado por el cliente adem?s de implementar las funciones que debe ejecutar el servidor por lo cual delega la responsabilidad del cifrado sobre el **Hill Cipher** y por eso utiliza su interfaz publica.  
- El **protocolo de comunicaci?n** es el encargado de conocer como se transmiten los mensajes entre ambos extremos de la comunicaci?n e internamente debe utilizar un **Socket** para realizar efectivamente la transmici?n de datos:   

    1. El protocolo de envio consiste en realizar un primer envio con la cantidad de bytes que se enviaran en formato big endian de dos bytes y luego enviar el mensaje real.
    2. El protocolo de recepci?n consiste en realizar una primera recepci?n con la cantidad de bytes que se recibiran en formato big endian de dos bytes y luego recibir el mensaje real.  

En s?ntesis si se quisiera reemplazar el *protocolo de comunicaci?n* establecido se podr?a hacer respetando la interfaz publica de send y receive que entienden el servidor y el cliente. De la misma manera se podr?a cambiar el socket que se utiliz? en la comunicaci?n (en este caso un socket para TCP) y trabajar con otro protocolo de transmisi?n de datos sin afectar al *protocolo de comunicaci?n* e incluso se puede cambiar el tipo de *cifrado* que utiliza el servidor, pero esto llevar?a un poco de mas de trabajo pues el cliente al tener que desmapear los resultados enviados por el servidor quedo ligeramente acoplado al tipo de cifrado que se use, ya que si este utilizara otro tipo de mapeo entonces tambi?n se deber?a cambiar el m?dulo de mapeo correspondiente.  
  
####Cliente####
El cliente es uno de los programas principales, utiliza el m?dulo **protocolo cliente** para iniciar su ejecuci?n por lo cual este tiene las siguientes responsabilidades:  
  
- Conectarse al servidor conociendo el dominio y el servicio de este.  
- Leer desde un archivo de texto o por entrada est?ndar los datos que se enviar?n al servidor (esta es la unica responsabilidad que realmente realiza pues las demas son delegadas en otros m?dulos).  
- Enviar los datos al servidor y esperar por una respuesta.
- Recibir los datos del servidor.
- Desmapear los datos del servidor.
- Mostrar los datos por salida est?ndar.

A continuaci?n se presenta un diagrama de secuencia que representa esta secuenta de pasos a trav?s de los distintos m?dulos:  
  
![Diagrama de secuencia de Cliente](./images/diagrama_02-Cliente_inicia_su_protocolo_.png)  
  









---
