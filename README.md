# TP1: Hill Cipher #
**Autor: Daniel Alejandro Lovera L?pez**  
**N?mero padr?n: 103442**  
**[https://github.com/DanieLovera/tp1](https://github.com/DanieLovera/tp1)**  

---
### Introducci?n ###  
Este trabajo consiste en el desarrollo de un programa cliente-servidor  en el cual, el cliente lee l?nea a l?nea un archivo de texto plano para luego enviarlas al servidor y este se encarga de encriptarlas usando un cifrado conocido como ***Hill cipher***. Este se basa en la sustituci?n de caracteres usando ?lgebra lineal modular, es similar al cifrado Cesar (***Cesars Cipher***) porque consiste en un cifrado basado en desplazamiento de caracteres pero este  extiende el concepto a N dimensiones.  
Este programa se encuentra dise?ado principalmente en 4 grandes bloques que son independientes unos de otros:  
  
- **Cliente**  
- **Protocolo de comunicaci?n**  
- **Servidor**  
- **Cifrado Hill**  
  
Cada uno de ellos tambi?n tienen cierto nivel de abstracci?n internamente, pero estos son los m?dulos claves del trabajo.  

---
### Desarrollo ###  
  
En esta secci?n se presentan detalles de implementaci?n sobre los cuatro m?dulos principales junto con algunos diagramas que tienen como objetivo ayudarle al lector a entender el dise?o propuesto.  
  
A continuaci?n se presenta un diagrama UML de clases que representa al modelo del trabajo.  
  
![Diagrama de clases](./images/diagrama_01.png)

---
