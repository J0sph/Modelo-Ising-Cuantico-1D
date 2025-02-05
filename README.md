# Proyecto-Computacional
Modelo de Ising cuántico unidimensional en una grilla de N espines: Análisis espectral.

Se adjunta una descripción adicional en el informe del proyecto y la presentación.

Archivos principales: ProyectoIsing.ipynb y archivos.cpp

# Clonar el repositorio
Utilizar:
#### git clone https://github.com/J0sph/Proyecto-Computacional.git

Con esto se crea una carpeta en la computadora donde se pueden crear o modificar archivos.

# Crear nueva rama
Se utiliza: 
#### git branch NOMBRE_DE_LA_RAMA

Sería bueno cada uno crear y trabajar en una rama aparte y al final juntar el trabajo en la rama main. En GitHub aparece arriba de los archivos una pestaña para cambiar la rama main y ver alguna otra rama del repositorio, en caso de ser necesario revisar el código que ha subido alguien más en su rama respectiva.

Para cambairse a la rama creada se utiliza: 
#### git checkout NOMBRE_DE_LA_RAMA

Aquí ya se pueden crear y modificar archivos en la nueva rama.

# Actualizaciones
Despúes de crear/modificar archivos se utiliza:
#### git add NOMBRE_DEL_ARCHIVO

Para guradarlo o ponerlo en la lista de cambios a realizar en el repositorio.

Una vez están todos los archivos añadidos con git add, se propone un cambio en el repositorio utilizando:
#### git commit -m "DESCRIPCIÓN_DEL_CAMBIO_REALIZADO"

Esto automáticamente genera el cambio con todos los archivos añadidos con git add.

Para no tener que añadir de uno en uno los archivos con git add antes de utilizar git commit, se puede utilizar la siguiente ocpión:
#### git commit -a -m "DESCRIPCIÓN_DEL_CAMBIO_REALIZADO"

La cual añade todos los cambios realizados al commit.

Una vez están todos los cambios en el commit, se actualiza el repositorio con el comando: 
#### git push origin NOMBRE_DE_LA_RAMA

Hasta este momento se pueden ver los cambios locales reflejados en el repositorio de GitHub en la rama respectiva.

Si se quiere actualizar el repositorio/carpeta local con los cambios que se han realizado en el repositorio de GitHub, se pueden utilizar las siguientes configuraciones:

Hacer merge (estrategia por defecto):
#### git config pull.rebase false  

Aplicar rebase:
#### git config pull.rebase true   

Aplicar solo fast-forward:
#### git config pull.ff only     

Después de elegir la configuración se utiliza:

#### git pull origin NOMBRE_DE_LA_RAMA

Esto actualiza la rama actual con los archivos de la rama elegida en el comando anterior (puede ser la misma rama actual). Para la opción rebase, también se puede utilizar: 

#### git rebase NOMBRE_DE_LA_RAMA

Si hubiera algún conflicto entre las ramas se debe solucionar para actualizar correctamente la rama local. Por ejemplo, si en la computadora local se tiene un código en el que se cambió la línea 3, y en el repositorio de GitHub el mismo código tiene la misma línea 3 pero modificada por alguien más, a la hora de actualizar el repositorio actual con el de GitHub va a haber un conflicto al tener un mismo archivo con una misma línea diferente. Github da opciones de modificar el archivo para solucionar el conflicto antes de actualizar el repositorio local.


