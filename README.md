# Proyecto 1: Hoja de Cálculo (Listas Enlazadas Cruzadas)

Este proyecto implementa una hoja de cálculo usando una Matriz Dispersa basada en listas enlazadas cruzadas en C++ puro. Incluye una interfaz gráfica sencilla utilizando SFML.

## Requisitos Previos e Instalación de SFML

Para compilar este proyecto necesitas un compilador C++17 (g++) y la librería gráfica **SFML 2.6.x**.

### Instalación de SFML en Windows (Recomendado)
Para Windows (MinGW 64-bit GCC 13.1.0), puedes instalar SFML automáticamente usando PowerShell. Abre tu terminal en la carpeta del proyecto y ejecuta estos comandos:

```powershell
Invoke-WebRequest -Uri "https://www.sfml-dev.org/files/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit.zip" -OutFile "SFML.zip"
Expand-Archive -Path "SFML.zip" -DestinationPath "."
Rename-Item -Path "SFML-2.6.1" -NewName "SFML"
Remove-Item "SFML.zip"
```
Esto descargará y preparará la carpeta `SFML` exacta que el proyecto necesita.

### Instalación de SFML en Linux / Ubuntu / WSL
Es mucho más fácil. Abre tu terminal y ejecuta:
```bash
sudo apt update
sudo apt install libsfml-dev
```

## Compilación del Proyecto

Tienes dos opciones para compilar y ejecutar la hoja de cálculo:

### Opción A: Usando CLion (CMake) - ¡Más fácil!
1. Abre CLion y selecciona **Open**.
2. Selecciona el archivo `CMakeLists.txt` que está en esta carpeta y elige "Open as Project".
3. CLion configurará automáticamente todo gracias al archivo CMake.
4. Presiona el botón verde de **Play** (Run) en la esquina superior derecha.

### Opción B: Usando la Terminal (Makefile)
1. Abre tu terminal (en Windows asegúrate de usar MSYS2 o tener `mingw32-make` en tu PATH).
2. Si pusiste la carpeta `SFML` dentro del proyecto como se indicó arriba, simplemente ejecuta:
   ```bash
   make
   ```
   *(En Windows con MinGW puede que necesites escribir `mingw32-make` en lugar de `make`)*
3. **Importante (Solo Windows):** Antes de ejecutar, debes copiar los archivos `.dll` que están en la carpeta `SFML/bin` al lado del archivo `.exe` generado.
4. Ejecuta el programa:
   ```bash
   ./spreadsheet_app
   ```
## Ejecución

Ejecuta el archivo generado:
```bash
./spreadsheet_app
```

## Uso de la Interfaz

*   **Navegación:** Usa las **flechas del teclado** para moverte por las celdas. La celda seleccionada se resaltará en azul. La vista hará "scroll" automáticamente si llegas al borde.
*   **Edición:** Presiona **Enter** sobre una celda para editarla (el borde se pondrá rojo). Escribe texto, números o fórmulas (ej. `=A1+2`). Presiona **Enter** para confirmar o **Esc** para cancelar.
*   **Eliminación de Celda:** Presiona **Delete** (Supr) sobre una celda para borrarla.
*   **Fórmulas Aritméticas:** Cualquier celda que empiece con `=` se evaluará. Soporta `+`, `-`, `*`, `/`, paréntesis y referencias a otras celdas (ej. `=A1*(B2+C3)`).

### Comandos Avanzados (Barra Inferior)
Para ejecutar operaciones de rango y eliminación de estructuras completas, entra en modo edición (Enter) en *cualquier* celda y escribe un comando empezando con `/`:

*   `/SUMA A1 C3` -> Muestra la suma del rango en la barra inferior.
*   `/PROM A1 C3` -> Muestra el promedio.
*   `/MAX A1 C3` -> Muestra el máximo.
*   `/MIN A1 C3` -> Muestra el mínimo.
*   `/DELRANGE A1 C3` -> Elimina todas las celdas en el rectángulo especificado.
*   `/DELROW 5` -> Elimina toda la fila 5.
*   `/DELCOL B` -> Elimina toda la columna B.

*Nota:* Después de ejecutar un comando con Enter, este no se guardará en la celda, sino que se ejecutará como acción global.
