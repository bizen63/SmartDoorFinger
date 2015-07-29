# SmartDoorFinger
Apertura y cierre de puerta con Arduino y sensor de huella digital, que utiliza servomotor como mecanismo de cierre y display 16x2 con conexión I2C para visualizar los mensajes.
Incluye todas las librerías necesarias y un pequeño manual con la teoría. Utilizamos Arduino UNO, con otros modelos de Arduino las conexiones I2C son diferentes.
Las conexiones se deducen de los comentarios del código.
La única huella con acceso autorizado será la escaneada con el identificador 1 (#1), por lo que escanearemos con ese identificador la huella de la persona autorizada.
Al acceder con la huella autorizada la puerta modifica su posición; si está cerrada se abre y si está abierta se cierra.
